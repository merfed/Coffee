# -*- coding: utf-8 -*-

"""
Blizzard Mipmap Format

BLP1 files, used mostly in Warcraft III, are currently unsupported.
All types of BLP2 files used in World of Warcraft are supported.

The BLP file structure consists of a header, up to 16 mipmaps of the
texture

Texture sizes must be powers of two, though the two dimensions do
not have to be equal; 512x256 is valid, but 512x200 is not.
The first mipmap (mipmap #0) is the full size image; each subsequent
mipmap halves both dimensions. The final mipmap should be 1x1.

BLP files come in many different flavours:
* JPEG-compressed (type == 0)- currently not supported.
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

from decoders import dxt1, dxt3, dxt5


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
	
	def _open(self):
		header = StringIO(self.fp.read(20 + 16*4 + 16*4)) # XXX
		magic = header.read(4)
		if magic != "BLP2":
			raise ValueError("not a BLP2 file")
		type, = unpack("i", header.read(4))
		encoding, alphaDepth, alphaEncoding, hasMips = unpack("4b", header.read(4))
		self.size = unpack("ii", header.read(8))
		offsets = unpack("16i", header.read(16*4))
		lengths = unpack("16i", header.read(16*4))
		palette_data = self.fp.read(256*4)
		
		self.mode = "RGB"
		self.tile = []
		if type == 1: # Uncompressed or DirectX compression
			data = []
			self.fp.seek(offsets[0])
			if encoding == 1: # uncompressed
				palette = getpalette(palette_data)
				_data = StringIO(self.fp.read(lengths[0]))
				while True:
					try:
						offset, = unpack("B", _data.read(1))
					except StructError:
						break
					b, g, r, a = palette[offset]
					if b > 5:
						pass
					data.append(pack("BBB", r, g, b))
			
			elif encoding == 2: # directx compression
				if alphaEncoding == 0: # DXT1
					linesize = (self.size[0] + 3) / 4 * 8
					for yb in xrange((self.size[1] + 3) / 4):
						if alphaDepth:
							self.mode = "RGBA"
							decoded = dxt1.decodeDXT1(self.fp.read(linesize), alpha=True)
						else:
							decoded = dxt1.decodeDXT1(self.fp.read(linesize))
						for d in decoded:
							data.append(d)
				
				elif alphaEncoding == 1: # DXT3
					linesize = (self.size[0] + 3) / 4 * 16
					self.mode = "RGBA"
					for yb in xrange((self.size[1] + 3) / 4):
						decoded = dxt3.decodeDXT3(self.fp.read(linesize))
						for d in decoded:
							data.append(d)
				
				elif alphaEncoding == 7: # DXT5
					linesize = (self.size[0] + 3) / 4 * 16
					self.mode = "RGBA"
					for yb in xrange((self.size[1] + 3) / 4):
						decoded = dxt5.decodeDXT5(self.fp.read(linesize))
						for d in decoded:
							data.append(d)
				
				else:
					raise ValueError("Expected alphaEncoding 0, 1 or 7, got %i instead" % (alphaEncoding))
				
			data = "".join(data)
			self.im = Image.core.new(self.mode, self.size)
			return self.fromstring(data)
		
		else:
			raise NotImplementedError("JPEG BLPs are unsupported")

Image.register_open("BLP", BLPImageFile)
Image.register_extension("BLP", ".blp")
