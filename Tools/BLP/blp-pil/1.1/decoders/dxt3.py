# -*- coding: utf-8 -*-

from struct import unpack

def decodeDXT3(data):
	"""
	input: one "row" of data (i.e. will produce 4*width pixels)
	"""
	
	blocks = len(data) / 16  # number of blocks in row
	finalColor = ["", "", "", ""]  # row accumulators
	
	for block in xrange(blocks):
		block = data[block*16:block*16+16]
		# Decode next 16-byte block.
		bits = unpack("<8B", block[:8])
		color0, color1 = unpack("<HH", block[8:12])
		
		code, = unpack("<I", block[12:])
		
		# color 0, packed 5-6-5
		r0 = ((color0 >> 11) & 0x1f) << 3
		g0 = ((color0 >> 5) & 0x3f) << 2
		b0 = (color0 & 0x1f) << 3
		
		# color 1, packed 5-6-5
		r1 = ((color1 >> 11) & 0x1f) << 3
		g1 = ((color1 >> 5) & 0x3f) << 2
		b1 = (color1 & 0x1f) << 3
		for j in xrange(4):
			high = False # Do we want the higher bits?
			for i in xrange(4):
				if high:
					high = False
				else:
					high = True
				alphaCodeIndex = (4*j+i) / 2
				finalAlpha = bits[alphaCodeIndex]
				if high:
					finalAlpha &= 0xf
				else:
					finalAlpha >>= 4
				finalAlpha *= 17 # We get a value between 0 and 15
				
				colorCode = (code >> 2*(4*j+i)) & 0x03
				
				if colorCode == 0:
					finalColor[j] += chr(r0) + chr(g0) + chr(b0) + chr(finalAlpha)
				elif colorCode == 1:
					finalColor[j] += chr(r1) + chr(g1) +  chr(b1) + chr(finalAlpha)
				elif colorCode == 2:
					finalColor[j] += chr((2*r0+r1)/3) + chr((2*g0+g1)/3) + chr((2*b0+b1)/3) + chr(finalAlpha)
				elif colorCode == 3:
					finalColor[j] += chr((r0+2*r1)/3) + chr((g0+2*g1)/3) + chr((b0+2*b1)/3) + chr(finalAlpha)
	return tuple(finalColor)
