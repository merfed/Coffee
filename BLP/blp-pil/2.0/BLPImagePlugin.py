# -*- coding: utf-8 -*-

"""
Blizzard Mipmap Format

BLP1 files, used mostly in Warcraft III, are not fully supported.
All types of BLP2 files used in World of Warcraft are supported.

The BLP file structure consists of a header, up to 16 mipmaps of the
texture

Texture sizes must be powers of two, though the two dimensions do
not have to be equal; 512x256 is valid, but 512x200 is not.
The first mipmap (mipmap #0) is the full size image; each subsequent
mipmap halves both dimensions. The final mipmap should be 1x1.

BLP files come in many different flavours:
* JPEG-compressed (type == 0) - only supported for BLP1.
* RAW images (type == 1, encoding == 1). Each mipmap is stored as an
  array of 8-bit values, one per pixel, left to right, top to bottom.
  Each value is an index to the palette.
* DXT-compressed (type == 1, encoding == 2):
  - DXT1 compression is used if alphaEncoding == 0. An additional alpha
    bit is used if alphaDepth == 1.
  - DXT3 compression is used if alphaEncoding == 1.
  - DXT5 compression is used if alphaEncoding == 7.
"""

import Image
import ImageFile
from struct import pack, unpack, error as StructError
from cStringIO import StringIO

from decoders import dxtc as _dxtc


def getpalette(data):
	"""
	Helper to transform a StringIO object into a palette
	"""
	palette = []
	string = StringIO(data)
	while True:
		try:
			palette.append(unpack("<4B", string.read(4)))
		except StructError:
			break
	return palette


class BLPImageFile(ImageFile.ImageFile):
	"""
	Blizzard Mipmap Format
	"""
	format = "BLP"
	format_description = "Blizzard Mipmap Format"
	
	def __decode_blp1(self):
		header = StringIO(self.fp.read(28 + 16*4 + 16*4))
		
		magic, compression = unpack("<4si", header.read(8))
		encoding, alphaDepth, alphaEncoding, hasMips = unpack("<4b", header.read(4))
		self.size = unpack("<II", header.read(8))
		encoding, subtype = unpack("<ii", header.read(8))
		offsets = unpack("<16I", header.read(16*4))
		lengths = unpack("<16I", header.read(16*4))
		
		if compression == 0:
			from PIL.JpegImagePlugin import JpegImageFile
			jpegHeaderSize, = unpack("<I", self.fp.read(4))
			jpegHeader = self.fp.read(jpegHeaderSize)
			extraData = self.fp.read(offsets[0] - self.fp.tell()) # What IS this?
			data = self.fp.read(lengths[0])
			data = jpegHeader + data
			data = StringIO(data)
			image = JpegImageFile(data)
			image.show()
			raw_input()
			self.tile = image.tile # PIL is terrible
			self.fp = image.fp
			self.mode = image.mode
			return
		
		if compression == 1:
			if encoding in (3, 4):
				raise NotImplementedError
			
			elif encoding == 5:
				data = []
				palette_data = self.fp.read(256*4)
				palette = getpalette(palette_data)
				_data = StringIO(self.fp.read(lengths[0]))
				self.mode = "RGB"
				self.tile = []
				while True:
					try:
						offset, = unpack("<B", _data.read(1))
					except StructError:
						break
					b, g, r, a = palette[offset]
					data.append(pack("<BBB", r, g, b))
				
				data = "".join(data)
				self.im = Image.core.new(self.mode, self.size)
				self.fromstring(data)
				return
			
			raise ValueError("Expected encoding 3, 4 or 5, got %i instead" % (encoding))
		
		raise ValueError("Expected compression 0 or 1, got %i instead" % (compression))
	
	def __decode_blp2(self):
		header = StringIO(self.fp.read(20 + 16*4 + 16*4))
		
		magic, compression = unpack("<4si", header.read(8))
		encoding, alphaDepth, alphaEncoding, hasMips = unpack("<4b", header.read(4))
		self.size = unpack("<II", header.read(8))
		offsets = unpack("<16I", header.read(16*4))
		lengths = unpack("<16I", header.read(16*4))
		palette_data = self.fp.read(256*4)
		
		self.mode = "RGB"
		self.tile = []
		if compression == 1: # Uncompressed or DirectX compression
			data = []
			self.fp.seek(offsets[0])
			if encoding == 1: # uncompressed
				palette = getpalette(palette_data)
				_data = StringIO(self.fp.read(lengths[0]))
				while True:
					try:
						offset, = unpack("<B", _data.read(1))
					except StructError:
						break
					b, g, r, a = palette[offset]
					data.append(pack("<BBB", r, g, b))
			
			elif encoding == 2: # directx compression
				print "reading as %s, alphaDepth of %i" % (("DXT1", "DXT3", "2", "3", "4", "5", "6", "DXT5")[alphaEncoding], alphaDepth)
				if alphaEncoding == 0: # DXT1
					linesize = (self.size[0] + 3) / 4 * 8
					for yb in xrange((self.size[1] + 3) / 4):
						if alphaDepth:
							self.mode = "RGBA"
							decoded = _dxtc.decodeDXT1(self.fp.read(linesize), alpha=True)
						else:
							decoded = _dxtc.decodeDXT1(self.fp.read(linesize))
						for d in decoded:
							data.append(d)
				
				elif alphaEncoding == 1: # DXT3
					linesize = (self.size[0] + 3) / 4 * 16
					self.mode = "RGBA"
					for yb in xrange((self.size[1] + 3) / 4):
						decoded = _dxtc.decodeDXT3(self.fp.read(linesize))
						for d in decoded:
							data.append(d)
				
				elif alphaEncoding == 7: # DXT5
					linesize = (self.size[0] + 3) / 4 * 16
					self.mode = "RGBA"
					for yb in xrange((self.size[1] + 3) / 4):
						decoded = _dxtc.decodeDXT5(self.fp.read(linesize))
						for d in decoded:
							data.append(d)
				
				else:
					raise ValueError("Expected alphaEncoding 0, 1 or 7, got %i instead" % (alphaEncoding))
				
			self.im = Image.core.new(self.mode, self.size)
			
			data = "".join(data)
			self.fromstring(data)
	
	def _open(self):
		magic = self.fp.read(4)
		self.fp.seek(0)
		if magic == "BLP1":
			return self.__decode_blp1()
		
		if magic == "BLP2":
			return self.__decode_blp2()
		
		raise ValueError("not a BLP file (magic: %r)" % (magic))


Image.register_open("BLP", BLPImageFile)
Image.register_extension("BLP", ".blp")
