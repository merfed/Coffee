from chunk import *
from wowfile import *

WMO_COLLISION = 0x1
WMO_VERTEXCOLORS = 0x4
WMO_OUTDOOR = 0x8
WMO_LIGHTS = 0x200
WMO_PUNK = 0x400
WMO_DOODADS = 0x800
WMO_WATER = 0x1000
WMO_INDOOR = 0x2000
WMO_RUNK = 0x20000
WMO_SHOW_SKY = 0x40000	
WMO_NOT_OCEAN = 0x80000	

class MOGP(WChunk):
	def __init__(self):
		self.groupName = 0
		self.descrName = 0
		self.flags = 0
		self.minExtends = Vec3()
		self.maxExtends = Vec3()
		self.refMOPR = 0
		self.nMOPR = 0
		self.batchA = 0
		self.batchB = 0
		self.batchC = 0
		self.fogIndices = [0,0,0,0]
		self.liquid = 0
		self.wmoGroupId = 0
		self.unk1 = 0
		self.unk2 = 0
		self.mopy = WChunk()
		self.movi = WChunk()
		self.movt = EntryChunk(1297045076, Vec3)
		self.monr = WChunk()
		self.motv = EntryChunk(1297044566, Vec2)
		self.moba = WChunk()
		self.molr = WChunk()
		self.modr = EntryChunk(1297040466, Lookup)
		self.mobn = WChunk()
		self.mobr = WChunk()
		self.mpbv = WChunk()
		self.mpbp = WChunk()
		self.mpbi = WChunk()
		self.mpbg = WChunk()
		self.mocv = WChunk()
		self.mliq = WChunk()
		self.mori = WChunk()
		self.morb = WChunk()
		
	def unpackData(self,f):
		self.groupName, = struct.unpack("i",f.read(4))
		self.descrName, = struct.unpack("i",f.read(4))
		self.flags, = struct.unpack("i",f.read(4))
		self.minExtends.unpack(f)
		self.maxExtends.unpack(f)
		self.refMOPR, = struct.unpack("h",f.read(2))
		self.nMOPR, = struct.unpack("h",f.read(2))
		self.batchA, = struct.unpack("h",f.read(2))
		self.batchB, = struct.unpack("h",f.read(2))
		self.batchC, = struct.unpack("i",f.read(4)) ### Yes this is strange oO ###
		self.fogIndices = struct.unpack("4b",f.read(4))
		self.liquid, = struct.unpack("i",f.read(4))
		self.wmoGroupId, = struct.unpack("i",f.read(4))
		self.unk1, = struct.unpack("i",f.read(4))
		self.unk2, = struct.unpack("i",f.read(4))
		#subchunks
		self.mopy.unpack(f)
		self.movi.unpack(f)
		self.movt.unpack(f)
		self.monr.unpack(f)
		self.motv.unpack(f)
		self.moba.unpack(f)
		#not always existant subchunks
		if (self.flags & WMO_LIGHTS):
			#print "MOLR Exists"
			self.molr.unpack(f)
		if (self.flags & WMO_DOODADS):
			#print "MODR Exists"
			self.modr.unpack(f)
		if (self.flags & WMO_COLLISION):			
			#print "MOBN and MOBR Exist"
			self.mobn.unpack(f)
			self.mobr.unpack(f)
		if (self.flags & WMO_PUNK):
			#print "MP** Exists"
			self.mpbv.unpack(f)
			self.mpbp.unpack(f)
			self.mpbi.unpack(f)
			self.mpbg.unpack(f)
		if (self.flags & WMO_VERTEXCOLORS):
			#print "MOCV Exists"
			self.mocv.unpack(f)
		if (self.flags & WMO_WATER):
			#print "MLIQ Exists"
			self.mliq.unpack(f)
		if (self.flags & WMO_RUNK):
			#print "MOR* Exists"
			self.mori.unpack(f)
			self.morb.unpack(f)
			
	def packData(self):
		ret = struct.pack("i", self.groupName)
		ret += struct.pack("i", self.descrName)
		ret += struct.pack("i", self.flags)
		ret += self.minExtends.pack()
		ret += self.maxExtends.pack()
		ret += struct.pack("h", self.refMOPR)
		ret += struct.pack("h", self.nMOPR)
		ret += struct.pack("h", self.batchA)
		ret += struct.pack("h", self.batchB)
		ret += struct.pack("i", self.batchC)
		ret += struct.pack("4b", self.fogIndices[0], self.fogIndices[1], self.fogIndices[2], self.fogIndices[3])
		ret += struct.pack("i", self.liquid)
		ret += struct.pack("i", self.wmoGroupId)
		ret += struct.pack("i", self.unk1)
		ret += struct.pack("i", self.unk2)
		
		ret += self.mopy.pack()
		ret += self.movi.pack()
		ret += self.movt.pack()
		ret += self.monr.pack()
		ret += self.motv.pack()
		ret += self.moba.pack()
		if (self.flags & WMO_LIGHTS):
			ret += self.molr.pack()
		if (self.flags & WMO_DOODADS):
			ret += self.modr.pack()
		if (self.flags & WMO_COLLISION):
			ret += self.mobn.pack()
			ret += self.mobr.pack()
		if (self.flags & WMO_PUNK):
			ret += self.mpbv.pack()
			ret += self.mpbp.pack()
			ret += self.mpbi.pack()
			ret += self.mpbg.pack()
		if (self.flags & WMO_VERTEXCOLORS):
			ret += self.mocv.pack()
		if (self.flags & WMO_WATER):
			ret += self.mliq.pack()
		if (self.flags & WMO_RUNK):
			ret += self.mori.pack()
			ret += self.morb.pack()
		return ret
		
		
	def addDoodadRef(self, ref):
		dref = Lookup()
		if not(self.flags & WMO_DOODADS):
			self.flags |= WMO_DOODADS
		dref.Id = ref
		self.modr.addEntry(dref)
