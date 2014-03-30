#!/usr/bin/python
import struct
import array


class WChunk: #Chunk Basic Class for World Data (like adt,wmo etc.)
	def __init__(self):
		self.magic = 0
		self.size = 0
		
	def unpack(self,f):
		self.magic,         = struct.unpack("i",f.read(4))
		self.size,         = struct.unpack("i",f.read(4))
		self.unpackData(f)
		return self
		
	def pack(self):
		temp = self.packData()
		self.size = len(temp)
		ret = struct.pack("i",self.magic)
		ret += struct.pack("i",self.size)
		ret += temp
		return ret
		
	def unpackData(self,f): #Dummy Method, overwrite!
		self.temp = f.read(self.size)
		
	def packData(self): #Dummy Method, overwrite!		
		return self.temp	
		
		
class MVER(WChunk):
	def __init__(self):
		self.magic = 1297499474
		self.size = 4
		self.version = 18
	def unpackData(self,f):
		self.version, = struct.unpack("i",f.read(4))
		
	def packData(self):
		ret = struct.pack("i",self.version)
		return ret
		
class FilenameChunk(WChunk):
	def __init__(self,magic):
		self.magic = magic
		self.size = 0
		self.filenames = []
		
	def unpackData(self,f):
		pos = 0
		tstr = ""
		while(pos < self.size):			
			pos += 1
			temp = f.read(1)
			tstr += temp
			while(temp != "\0"):
				temp = f.read(1)
				tstr += temp
				pos += 1
			self.filenames.append(tstr)
			tstr = ""
		#print len(self.filenames)
		#print self.filenames
		
	def packData(self):
		ret = ""
		for i in self.filenames:
			if len(i) == 0:
				i= "\0"
			if i[len(i)-1] != "\0":
				i += "\0"
			ret += i
		return ret

class EntryChunk(WChunk):

	def __init__(self,magic,entrytype):
		self.magic = magic
		self.size = 0
		self.nEntries = 0
		self.entries = []
		self.Entry = entrytype
		
	def unpackData(self,f):
		self.nEntries = self.size / self.Entry.entrySize
		self.entries = []
		for i in xrange(self.nEntries):
			self.entries.append(self.Entry().unpack(f))
			
	def packData(self):
		ret = ""
		for i in xrange(self.nEntries):
			ret += self.entries[i].pack()
		return ret

	def addEntry(self):
		self.nEntries += 1
		self.entries.append(self.Entry())
		
	def addEntry(self, entry):
		self.nEntries += 1
		self.entries.append(entry)
		
	def delEntry(self, entrie = 0):
		if (self.nEntries > entrie):
			del(self.entries[entrie])
			self.nEntries -= 1
		
class WoWFile:	
	def __init__(self):
		pass
		
	def readData(self,f):
		pass
		
	def writeData(self,f):
		return f
		
	def read(self,filename):
		f = open(filename,"r+b")		
		self.readData(f)			
		f.close()
		return self
	
	
	def write(self,filename):
		f = open(filename,"wb")		
		f = self.writeData(f)		
		f.close()
		