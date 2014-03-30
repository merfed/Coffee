#!/usr/bin/python
#This file contains some helpers for the files
import struct
from sys import version_info
if version_info < (3,0):
	xrange = xrange
else:
	xrange = range

SEEK_SET	= 0
SEEK_CUR	= 1
SEEK_END	= 2

def FillLine(f):
	i = f.tell()
	n = (16 - ((i) % 16))#Calculate the bytes needed to fill up the 0x10 ( == 16)
	for i in xrange(n):		
		f.write("\0")#write 0-Byte
		
	return f
	
def ReadBlock(f,chunk,func,animfiles = None):
	f.seek(chunk.offset)
	ret = []
	if(animfiles == None):
		for i in xrange(chunk.count):
			temp = func().unpack(f)
			ret.append(temp)
	else:
		for i in xrange(chunk.count):
			temp = func().unpack(f,animfiles)
			ret.append(temp)
	return ret
	
def WriteBlock(f,chunk,block):
	if(chunk.count != 0):
		chunk.offset = f.tell()
		for i in xrange(chunk.count):
			temp = block[i].pack()
			f.write(temp)
		FillLine(f)
	else:
		chunk.offset = 0
	
	
class Lookup:
	entrySize = 2
	def __init__(self):
		self.Id = 0
	def unpack(self,f):
		self.Id, = struct.unpack("h",f.read(2))
		return self
	def pack(self):
		return struct.pack("h",self.Id)
	
class Reference:
	entrySize = 4
	def __init__(self):
		self.Id = 0
	def unpack(self,f):
		self.Id, = struct.unpack("i", f.read(4))
		return self
	def pack(self):
		return struct.pack("i", self.Id)

class Chunk:
	def __init__(self):
		self.count     = 0
		self.offset    = 0
	def unpack(self,f):
		self.count,     = struct.unpack("i",f.read(4))
		self.offset,    = struct.unpack("i",f.read(4))
		return self		
	def pack(self):
		ret = struct.pack("2i",self.count,self.offset)
		return ret
		
class Triangle:
	def __init__(self):
		self.indices	= (0,0,0)
	def __str__(self):
		return "{ "+str(self.indices[0])+" , "+str(self.indices[1])+ " , "+str(self.indices[2])+" }"
	def unpack(self,f):
		self.indices	= struct.unpack("3H",f.read(6))
		return self
	def pack(self):
		return struct.pack("3H",self.indices[0],self.indices[1],self.indices[2])
		
class Vec3:
	entrySize = 12
	def __init__(self,x = 0.0,y = 0.0, z = 0.0):
		self.x = float(x)
		self.y = float(y)
		self.z = float(z)
	def __str__(self):
		return "{ "+str(self.x)+" , "+str(self.y)+ " , "+str(self.z)+" }"
		
	def __add__(self, val):
		return Vec3( self.x + val.x, self.y + val.y , self.z + val.z)   
		
	def __sub__(self,val):
		return Vec3( self.x - val.x, self.y - val.y , self.z - val.z)   
		
	def __iadd__(self, val):
		self.x = val.x + self.x
		self.y = val.y + self.y
		self.z = val.z + self.z
		return self
        
	def __isub__(self, val):
		self.x = self.x - val.x
		self.y = self.y - val.y
		self.z = self.z - val.z
		return self
		
	def unpack(self,f):
		self.x,	= struct.unpack("f",f.read(4))
		self.y,	= struct.unpack("f",f.read(4))
		self.z,	= struct.unpack("f",f.read(4))	
		return self
		
	def pack(self):
		ret = struct.pack("f",self.x)
		ret += struct.pack("f",self.y)
		ret += struct.pack("f",self.z)
		return ret
		
class Vec2:
	entrySize = 8
	def __init__(self):
		self.x	= 0
		self.y	= 0
	def __str__(self):
		return "{ "+str(self.x)+" , "+str(self.y)+" }"

	def unpack(self,f):
		self.x,	= struct.unpack("f",f.read(4))
		self.y,	= struct.unpack("f",f.read(4))
		return self		
	def pack(self):
		ret = struct.pack("f",self.x)
		ret += struct.pack("f",self.y)
		return ret
		
class Vec9:
	def __init__(self):
		self.x1 = 0
		self.x2 = 0
		self.x3 = 0
		self.y1 = 0
		self.y2 = 0
		self.y3 = 0
		self.z1 = 0
		self.z2 = 0
		self.z3 = 0
	def __str__(self):
		return "{ "+str(self.x1)+" , "+str(self.x2)+" , "+str(self.x3)+" , "+str(self.y1)+ " , "+str(self.y2)+ " , "+str(self.y3)+ " , "+str(self.z1)+ " , "+str(self.z2)+ " , "+str(self.z3)+" }"
	def unpack(self,f):
		self.x1, = struct.unpack("f",f.read(4))
		self.x2, = struct.unpack("f",f.read(4))
		self.x3, = struct.unpack("f",f.read(4))
		self.y1, = struct.unpack("f",f.read(4))
		self.y2, = struct.unpack("f",f.read(4))
		self.y3, = struct.unpack("f",f.read(4))
		self.z1, = struct.unpack("f",f.read(4))
		self.z2, = struct.unpack("f",f.read(4))
		self.z3, = struct.unpack("f",f.read(4))
		return self
	def pack(self):
		ret = struct.pack("f",self.x1)
		ret += struct.pack("f",self.x2)
		ret += struct.pack("f",self.x3)
		ret += struct.pack("f",self.y1)
		ret += struct.pack("f",self.y2)
		ret += struct.pack("f",self.y3)
		ret += struct.pack("f",self.z1)
		ret += struct.pack("f",self.z2)
		ret += struct.pack("f",self.z3)
		return ret
		
class Quat:
	def __init__(self):
		self.x = 0
		self.y = 0
		self.z = 0
		self.w = 0
	def __str__(self):
		return "{ "+str(self.x)+" , "+str(self.y)+" , "+str(self.z)+" , "+str(self.w)+" }"
	def unpack(self,f):
		self.x, = struct.unpack("h",f.read(2))
		self.y, = struct.unpack("h",f.read(2))
		self.z, = struct.unpack("h",f.read(2))
		self.w, = struct.unpack("h",f.read(2))
		return self	
	def pack(self):
		ret = struct.pack("h",self.x)
		ret += struct.pack("h",self.y)
		ret += struct.pack("h",self.z)
		ret += struct.pack("h",self.w)
		return ret
		
class Bounds:
	def __init__(self):
		self.minimumExtend	= Vec3()
		self.maximumExtend	= Vec3()
		self.Radius		= 0
	def unpack(self,f):
		self.minimumExtend	= Vec3().unpack(f)
		self.maximumExtend	= Vec3().unpack(f)
		self.Radius,		= struct.unpack("f",f.read(4))
		return self
	def pack(self):
		ret = self.minimumExtend.pack()
		ret += self.maximumExtend.pack()
		ret += struct.pack("f",self.Radius)
		return ret
	

class Color:
	entrySize = 4
	def __init__(self):
		self.alpha = 0
		self.red = 0
		self.green = 0
		self.blue = 0
	def unpack(self,f):
		self.alpha, = struct.unpack("b",f.read(1))
		self.red, = struct.unpack("b",f.read(1))
		self.green, = struct.unpack("b",f.read(1))
		self.blue, = struct.unpack("b",f.read(1))
		return self
	def pack(self):
		ret = struct.pack("b",self.alpha)
		ret += struct.pack("b",self.red)
		ret += struct.pack("b",self.green)
		ret += struct.pack("b",self.blue)
		return ret
		