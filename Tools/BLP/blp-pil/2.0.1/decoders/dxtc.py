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


def decodeDXT5(data):
	"""
	input: one "row" of data (i.e. will produce 4*width pixels)
	"""
	
	blocks = len(data) / 16  # number of blocks in row
	finalColor = ["", "", "", ""]  # row accumulators
	
	for block in xrange(blocks):
		block = data[block*16:block*16+16]
		# Decode next 16-byte block.
		alpha0, alpha1 = unpack("<BB", block[:2])
		
		bits = unpack("<6B", block[2:8])
		alphaCode1 = bits[2] | (bits[3] << 8) | (bits[4] << 16) | (bits[5] << 24)
		alphaCode2 = bits[0] | (bits[1] << 8)
		
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
			for i in xrange(4):
				# get next control op and generate a pixel
				alphaCodeIndex = 3*(4*j+i)
				
				if alphaCodeIndex <= 12:
					alphaCode = (alphaCode2 >> alphaCodeIndex) & 0x07
				elif alphaCodeIndex == 15:
					alphaCode = (alphaCode2 >> 15) | ((alphaCode1 << 1) & 0x06)
				else: # alphaCodeIndex >= 18 and alphaCodeIndex <= 45
					alphaCode = (alphaCode1 >> (alphaCodeIndex - 16)) & 0x07
				
				if alphaCode == 0:
					finalAlpha = alpha0
				elif alphaCode == 1:
					finalAlpha = alpha1
				else:
					if alpha0 > alpha1:
						finalAlpha = ((8-alphaCode)*alpha0 + (alphaCode-1)*alpha1)/7
					else:
						if alphaCode == 6:
							finalAlpha = 0
						elif alphaCode == 7:
							finalAlpha = 255
						else:
							finalAlpha = ((6-alphaCode)*alpha0 + (alphaCode-1)*alpha1)/5
				
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
