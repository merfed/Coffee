from chunk import *
from adt_chunks import *


class ADTFile(WoWFile):
	def __init__(self):
		self.mver = MVER()
		self.mhdr = MHDR()
		self.mcin = EntryChunk(1296255310, MCINEntry)
		self.mtex = FilenameChunk(1297368408)
		self.mmdx = FilenameChunk(1296909400)
		self.mmid = EntryChunk(1296910660, Reference)
		self.mwmo = FilenameChunk(1297567055)
		self.mwid = EntryChunk(1297566020, Reference)
		self.mddf = EntryChunk(1296319558, MDDFEntry)
		self.modf = EntryChunk(1297040454, MODFEntry)
		self.mh2o = WChunk()
		self.mcnk = []
		self.mfbo = WChunk()
		self.mtfx = EntryChunk(1297368664, Reference)
		
	def readData(self,f):
		self.mver.unpack(f)
		self.mhdr.unpack(f)
		self.mcin.unpack(f)
		self.mtex.unpack(f)
		self.mmdx.unpack(f)
		self.mmid.unpack(f)
		self.mwmo.unpack(f)
		self.mwid.unpack(f)
		self.mddf.unpack(f)
		self.modf.unpack(f)
		if(self.mhdr.ofsMH2O != 0):
			self.mh2o.unpack(f)
		for i in range(256):
			self.mcnk.append(MCNK().unpack(f))
		if(self.mhdr.ofsMFBO != 0):
			self.mfbo.unpack(f)
		if(self.mhdr.ofsMTFX != 0):
			self.mtfx.unpack(f)
		
		
	def writeData(self,f):
		ret = self.mver.pack()
		ret += self.mhdr.pack()
		ret += self.mcin.pack()
		ret += self.mtex.pack()
		ret += self.mmdx.pack()
		ret += self.mmid.pack()
		ret += self.mwmo.pack()
		ret += self.mwid.pack()
		ret += self.mddf.pack()
		ret += self.modf.pack()
		if(self.mhdr.ofsMH2O != 0):
			ret += self.mh2o.pack()
		for i in range(256):
			ret += self.mcnk[i].pack()
		if(self.mhdr.ofsMFBO != 0):
			ret += self.mfbo.pack()
		if(self.mhdr.ofsMTFX != 0):
			ret += self.mtfx.pack()
		f.write(ret)
		return f
		
#adt = ADTFile()
#adt.read("Azeroth_31_49.adt")
#print adt.mcnk[0].pos
#adt.write("Test.adt")
