# -*- coding: utf-8 -*-

from struct import unpack

def decodeDXT1(data, alpha=False):
	"""
	input: one "row" of data (i.e. will produce 4*width pixels)
	"""
	
	blocks = len(data) / 8  # number of blocks in row
	finalColor = ["", "", "", ""]  # row accumulators
	
	for block in xrange(blocks):
		# Decode next 8-byte block.
		color0, color1, bits = unpack("<HHI", data[block*8:block*8+8])
		
		# color 0, packed 5-6-5
		r0 = ((color0 >> 11) & 0x1f) << 3
		g0 = ((color0 >> 5) & 0x3f) << 2
		b0 = (color0 & 0x1f) << 3
		
		# color 1, packed 5-6-5
		r1 = ((color1 >> 11) & 0x1f) << 3
		g1 = ((color1 >> 5) & 0x3f) << 2
		b1 = (color1 & 0x1f) << 3
		
		# Decode this block into 4x4 pixels
		# Accumulate the results onto our 4 row accumulators
		for j in xrange(4):
			for i in xrange(4):
				# get next control op and generate a pixel
				
				control = bits & 3
				bits = bits >> 2
				if control == 0:
					finalColor[j] += chr(r0) + chr(g0) + chr(b0)
				elif control == 1:
					finalColor[j] += chr(r1) + chr(g1) + chr(b1)
				elif control == 2:
					if color0 > color1:
						finalColor[j] += chr((2 * r0 + r1) / 3) + chr((2 * g0 + g1) / 3) + chr((2 * b0 + b1) / 3)
					else:
						finalColor[j] += chr((r0 + r1) / 2) + chr((g0 + g1) / 2) + chr((b0 + b1) / 2)
				elif control == 3:
					if color0 > color1:
						finalColor[j] += chr((2 * r1 + r0) / 3) + chr((2 * g1 + g0) / 3) + chr((2 * b1 + b0) / 3)
					else:
						if alpha:
							finalColor[j] += "\0\0\0\0"
							continue
						else:
							finalColor[j] += "\0\0\0"
				if alpha:
					finalColor[j] += chr(0xFF)
	
	return tuple(finalColor)
