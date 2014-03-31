
cdef extern from "math.h":
	#float sqrt "sqrtf" (float f)
	#float sin "sinf" (float f)
	#float cos "cosf" (float f)
	#float acos "acosf" (float f)
	#float fabs "fabsf" (float f)
	double sqrt (double f)
	double sin (double f)
	double cos (double f)
	double acos (double f)
	double fabs (double f)


cdef class Vec2:
	cdef readonly float _x,_y
	
	def __init__(self, float x = 0.0, float y = 0.0):
		self._x = x
		self._y = y
	
	def __add__(self, Vec2 v not None):
		return Vec2(self._x+v._x, self._y+v._y)
	
	def __sub__(self, Vec2 v not None):
		return Vec2(self._x-v._x, self._y-v._y)
	
	def __neg__(self):
		return Vec2(-self._x, -self._y, -self._z)
	
	def __pow__(self, Vec2 v not None, z):
		return self._x*v._x+self._y*v._y
	
	def __mul__(self, f):
		try:
			return Vec2(self._x*f, self._y*f)
		except AttributeError:
			return Vec2(self*f._x, self*f._y)
	
	def __iadd__(self, Vec2 v not None):
		self._x = self._x+v._x
		self._y = self._y+v._y
		return self
	
	def __isub__(self, Vec2 v not None):
		self._x = self._x-v._x
		self._y = self._y-v._y
		return self
	
	def __imul__(self, f):
		self._x = self._x*f
		self._y = self._y*f
		return self
	
	def lengthSquared(self):
		return self._x*self._x+self._y*self._y
	
	def length(self):
		return sqrt(self._x*self._x+self._y*self._y)
	
	def normalize(self):
		return self.__imul__(1.0/self.length())
	
	def __invert__(self):
		return Vec2(self._x, self._y).normalize()
	
	def copy(self):
		return Vec2(self._x, self._y)
	
	property x:
		def __get__(self):
			return self._x
		def __set__(self, v):
			self._x = v
	
	property y:
		def __get__(self):
			return self._y
		def __set__(self, v):
			self._y = v
	
	property data:
		def __get__(self):
			return (self._x, self._y)
		def __set__(self, v):
			self._x = v[0]
			self._y = v[1]


cdef class Vec3:
	cdef readonly float _x,_y,_z # data[3]
	
	def __init__(self, float x = 0.0, float y = 0.0, float z = 0.0):
		self._x = x
		self._y = y
		self._z = z
	
	def __add__(self, Vec3 v not None):
		return Vec3(self._x+v._x, self._y+v._y, self._z+v._z)
	
	def __sub__(self, Vec3 v not None):
		return Vec3(self._x-v._x, self._y-v._y, self._z-v._z)
	
	def __neg__(self):
		return Vec3(-self._x, -self._y, -self._z)
	
	# v**u -> dot product
	def __pow__(self, Vec3 v not None, z):
		return self._x*v._x+self._y*v._y+self._z*v._z
	
	def __mul__(self, f):
		try:
			return Vec3(self._x*f, self._y*f, self._z*f)
		except AttributeError:
			#auuu:
			return Vec3(self*f._x, self*f._y, self*f._z)
	
	#def __rmul__(self, f):
	#	return Vec3(self._x*f, self._y*f, self._z*f)
	
	# v%u -> cross product
	def __mod__(self, Vec3 v not None):
		return Vec3(self._y*v._z-self._z*v._y, self._z*v._x-self._x*v._z, self._x*v._y-self._y*v._x)
	
	#grr: += not allowed
	def __iadd__(self, Vec3 v not None):
		self._x = self._x+v._x
		self._y = self._y+v._y
		self._z = self._z+v._z
		return self
	
	def __isub__(self, Vec3 v not None):
		self._x = self._x-v._x
		self._y = self._y-v._y
		self._z = self._z-v._z
		return self
	
	def __imul__(self, f):
		self._x = self._x*f
		self._y = self._y*f
		self._z = self._z*f
		return self
	
	def lengthSquared(self):
		return self._x*self._x+self._y*self._y+self._z*self._z
	
	def length(self):
		return sqrt(self._x*self._x+self._y*self._y+self._z*self._z)
	
	def normalize(self):
		return self.__imul__(1.0/self.length())
		#return self ???
	
	def __invert__(self):
		return Vec3(self._x, self._y, self._z).normalize()
		#r.normalize()
		#return r
	
	def copy(self):
		return Vec3(self._x, self._y, self._z)
	
	property x:
		def __get__(self):
			return self._x
		def __set__(self, v):
			self._x = v
	
	property y:
		def __get__(self):
			return self._y
		def __set__(self, v):
			self._y = v
	
	property z:
		def __get__(self):
			return self._z
		def __set__(self, v):
			self._z = v
	
	property data:
		def __get__(self):
			return (self._x, self._y, self._z)
		def __set__(self, v):
			self._x = v[0]
			self._y = v[1]
			self._z = v[2]
	
	#def as_tuple(self):
	#	return (self._x,self._y,self._z)
	
	#def as_str(self):
	#	return struct.pack('3f',self.x,self.y,self.z)


cdef class Vec4:
	cdef readonly float _x,_y,_z,_w
	
	def __init__(self, float x = 0.0, float y = 0.0, float z = 0.0, float _w = 0.0):
		self._x = x
		self._y = y
		self._z = z
		self._w = w
	
	def __add__(self, Vec4 v not None):
		return Vec4(self._x+v._x, self._y+v._y, self._z+v._z, self._w+v._w)
	
	def __sub__(self, Vec4 v not None):
		return Vec4(self._x-v._x, self._y-v._y, self._z-v._z, self._w-v._w)
	
	def __neg__(self):
		return Vec4(-self._x, -self._y, -self._z, -self._w)
	
	def __pow__(self, Vec4 v not None, z):
		return self._x*v._x+self._y*v._y+self._z*v._z+self._w*v._w
	
	def __mul__(self, f):
		try:
			return Vec4(self._x*f, self._y*f, self._z*f, self._w*f)
		except AttributeError:
			return Vec4(self*f._x, self*f._y, self*f._z, self*f._w)
	
	def __iadd__(self, Vec4 v not None):
		self._x = self._x+v._x
		self._y = self._y+v._y
		self._z = self._z+v._z
		self._w = self._w+v._w
		return self
	
	def __isub__(self, Vec4 v not None):
		self._x = self._x-v._x
		self._y = self._y-v._y
		self._z = self._z-v._z
		self._w = self._w-v._w
		return self
	
	def __imul__(self, f):
		self._x = self._x*f
		self._y = self._y*f
		self._z = self._z*f
		self._w = self._w*f
		return self
	
	def lengthSquared(self):
		return self._x*self._x+self._y*self._y+self._z*self._z+self._w*self._w
	
	def length(self):
		return sqrt(self._x*self._x+self._y*self._y+self._z*self._z+self._w*self._w)
	
	def normalize(self):
		return self.__imul__(1.0/self.length())
	
	def __invert__(self):
		return Vec4(self._x, self._y, self._z, self._w).normalize()
	
	def copy(self):
		return Vec4(self._x, self._y, self._z, self._w)
	
	def copyVec3(self):
		return Vec3(self._x, self._y, self._z)
	
	property x:
		def __get__(self):
			return self._x
		def __set__(self, v):
			self._x = v
	
	property y:
		def __get__(self):
			return self._y
		def __set__(self, v):
			self._y = v
	
	property z:
		def __get__(self):
			return self._z
		def __set__(self, v):
			self._z = v
	
	property w:
		def __get__(self):
			return self._w
		def __set__(self, v):
			self._w = v
	
	property data:
		def __get__(self):
			return (self._x, self._y, self._z, self._w)
		def __set__(self, v):
			self._x = v[0]
			self._y = v[1]
			self._z = v[2]
			self._w = v[3]



def rotate(float x0, float y0, float x, float y, float angle):
	cdef float xa,ya
	xa = x - x0
	ya = y - y0
	return (xa*cos(angle) - ya*sin(angle) + x0, xa*sin(angle) + ya*cos(angle) + y0)

# u, v: unit length
def slerp(Vec4 u, Vec4 v, float t):
	cdef float dot, a
	dot = u**v
	if (fabs(dot) > 0.9995):
		return lerp(u, v, t)
	a = acos(dot)*t
	return u*cos(a) + (v - u*dot).normalize()*sin(a)

def lerp(u, v, float t):
	return u*(1.0-t) + v*t

