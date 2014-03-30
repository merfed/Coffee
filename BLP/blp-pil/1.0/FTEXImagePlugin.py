# -*- coding: utf-8 -*-

"""
Independence War 2: Edge Of Chaos - Texture File Format
Copyright © 2001 Particle Systems Ltd, All Rights Reserved.
Version 1.0
16 October 2001

The textures used for 3D objects in Independence War 2: Edge Of Chaos are in a
packed custom format called FTEX. This file format uses file extensions FTC and FTU.

* FTC files are compressed textures (using standard texture compression).
* FTU files are not compressed.

The game will happily use IFF format textures if they are present, but the
packed textures are used to decrease load times (compressing textures on the fly is
expensive) and give better image quality, since the mipmaps can be generated offline
using a slow but effective filter.

This document describes the FTEX file format specification for those users who are
interested in converting the game’s textures into an editable form, or who want to
convert their own textures into the game’s optimal format.
"""

import Image
import ImageFile
from struct import unpack
from cStringIO import StringIO

from decoders import dxt1

class FTEXImageFile(ImageFile.ImageFile):
	"""
	Texture File Format
	The FTC and FTU texture files both use the same format, called. This
	has the following structure:
	{header}
	{format_directory}
	{data}
	Where:
	{header} = { u32:magic, u32:version, u32:width, u32:height, u32:mipmap_count, u32:format_count }
	
	* The "magic" number is "FTEX".
	* "width" and "height" are the dimensions of the texture.
	* "mipmap_count" is the number of mipmaps in the texture.
	* "format_count" is the number of texture formats (different versions of the same texture) in this file.
	
	{format_directory} = format_count * { u32:format, u32:where }
	
	The format value is 0 for DXT1 compressed textures and 1 for 24-bit RGB uncompressed textures.
	The texture data for a format starts at the position "where" in the file.
	
	Each set of texture data in the file has the following structure:
	{data} = format_count * { u32:mipmap_size, mipmap_size * { u8 } }
	* "mipmap_size" is the number of bytes in that mip level. For compressed textures this is the
	size of the texture data compressed with DXT1. For 24 bit uncompressed textures, this is 3 * width * height.
	Following this are the image bytes for that mipmap level.
	
	Note: All data is stored in little-Endian (Intel) byte order.
	"""
	format = "FTEX"
	format_description = "Texture File Format"
	
	def _open(self):
		header = StringIO(self.fp.read(24))
		magic = header.read(4)
		if magic != "FTEX":
			raise ValueError("not a FTEX file")
		version = unpack("i", header.read(4))
		self.size = unpack("ii", header.read(8))
		linesize = (self.size[0] + 3) / 4 * 8
		mipmap_count, format_count = unpack("ii", header.read(8))
		self.mode = "RGB"
		
		self.tile = []
		for i in range(format_count):
			format, where = unpack("ii", self.fp.read(8))
			if format == 0:
				data = []
				self.fp.seek(where)
				size, = unpack("i", self.fp.read(4))
				for yb in xrange((self.size[1] + 3) / 4):
					decoded = dxt1.decodeDXT1(self.fp.read(linesize))
					
					for d in decoded:
						# Make sure that if we have a texture size that's not a
						# multiple of 4 that we correctly truncate the returned data
						data.append(d[:self.size[0]*3])
						#assert len(d) == len(d[:self.size[0]*3])
				data = "".join(data[:self.size[1]])
				self.im = Image.core.new(self.mode, self.size)
				return self.fromstring(data)
			elif format == 1: # Uncompressed RGB
				self.tile.append(("raw", (0, 0) + self.size, where+4, (self.mode, 0, 1)))
			else:
				raise ValueError("Invalid texture format (expected 0 or 1, got %i)" % (format))

Image.register_open("FTEX", FTEXImageFile)
Image.register_extension("FTEX", ".ftu") # uncompressed
Image.register_extension("FTEX", ".ftc") # compressed
