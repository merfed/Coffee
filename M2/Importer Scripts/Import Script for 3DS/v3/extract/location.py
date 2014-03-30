
from StringIO import StringIO


class Location:
	def __init__(self, path):
		self.path = path
		#self.size = 0
		#self.numfiles = 0
	
	def read(self, fn):
		# return str
		raise IOError, 'File not found: %s'%fn
	
	def open(self, fn):
		# return file
		return StringIO(self.read(fn))
	
	def namelist(self):
		return []
	
	def __contains__(self, fn):
		return False
	
	has_file = __contains__
	#filelist = namelist
	
	def close(self):
		pass #?
	
	def __del__(self):
		self.close()
	
	#def getinfo(self, fn): pass
	#def infolist(self): pass


#class LocationInfo: pass


import os
import os.path

class Directory(Location):
	def __init__(self, path):
		self.path = path
		#if not os.path.isdir(path):
		#	raise OSError, 'not a directory: %s'%path
		self.size = 0
		self.numfiles = 0
		for root, dirs, files in os.walk(path):
			self.size += sum(os.path.getsize(os.path.join(root, name)) for name in files)
			self.numfiles += len(files)
	
	def os_filename(self, fn):
		return os.path.join(self.path, fn.replace('\\', os.path.sep))
	
	def open(self, fn):
		return file(self.os_filename(fn), 'rb')
	
	def read(self, fn):
		return file(self.os_filename(fn), 'rb').read()
	
	def namelist(self):
		return sum(([os.path.join(root[len(self.path)+1:], fn).replace(os.path.sep, '\\') for fn in files] for root, dirs, files in os.walk(self.path)), [])
	
	def os_namelist(self):
		return sum(([os.path.join(root, fn) for fn in files] for root, dirs, files in os.walk(self.path)), [])
	
	def __contains__(self, fn):
		return os.path.isfile(self.os_filename(fn))


import mpq

class MpqArchive(mpq.Archive, Location):
	def __init__(self, path):
		mpq.Archive.__init__(self, path)
		self.path = path


from zipfile import ZipFile

class ZipArchive(ZipFile, Location):
	def __init__(self, path):
		ZipFile.__init__(self, path)
		self.path = path
		#self.size = 
		#self.numfiles = 
		
	def read(self, fn):
		ZipFile.read(self, fn.replace('\\', os.path.sep)) #???
	
	def namelist(self):
		# dont list dirs
		return [data.filename.replace('/', '\\') for data in self.filelist if data.filename[-1]!=os.path.sep] #???
	
	def __contains__(self, fn):
		#hmm: a\b\c in self ==> a/b/c in self
		return fn[-1]!='\\' and fn.replace('\\', os.path.sep) in self.NameToInfo


class Locator:
	def __init__(self):
		#self.loc_dict = {}
		self.locations = [] # in priority order
	
	def addlocation(self, loc, priority=None):
		if priority is None:
			self.locations.append(loc)
		else:
			self.locations.insert(priority, loc)
		return loc
		#self.loc_dict[loc.path] = loc
	
	def openlocation(self, path, priority=None):
		ext = os.path.splitext(path)[1].lower()
		if os.path.isdir(path):
			return self.addlocation(Directory(path), priority)
		elif ext == '.mpq':
			return self.addlocation(MpqArchive(path), priority)
		elif ext == '.zip':
			return self.addlocation(ZipArchive(path), priority)
		else:
			raise IOError, 'Not a directory: %s or not a location extension: %s'%(path, ext)
	
	def read(self, fn, loc = None):
		if loc is None:
			for loc in self.locations:
				if fn in loc:
					return loc.read(fn)
			raise IOError, 'File not found in any location: %s'%fn
		return loc.read(fn)
	
	def open(self, fn, loc = None):
		if loc is None:
			for loc in self.locations:
				if fn in loc:
					return loc.open(fn)
			raise IOError, 'File not found in any location: %s'%fn
		return loc.open(fn)
	
	def namelist(self):
		return sum((loc.namelist() for loc in self.locations), []) #.sort()
	
	filelist = namelist
	
	def __contains__(self, fn):
		for loc in self.locations:
			if fn in loc:
				return True
		return False
	
	#def rmlocation(self, loc): pass
	#def closelocation(self, path): pass
	#def closeall(self): pass
	#closelocations = closeall
	#def __del__(self): self.closeall()


if __name__=='__main__':
	rc = Locator()
	rc.openlocation('..')
	rc.openlocation('test.zip')
	
	for fn in rc.namelist():
		print fn
	
