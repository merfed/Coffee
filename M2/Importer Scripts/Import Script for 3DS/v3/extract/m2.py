'''
m2.py
'''
if __name__ == '__main__':
	import sys
	sys.path.append('..')

from math import sqrt
from vec import Vec2, Vec3
from struct import unpack

from OpenGL import *
from OpenGL.GL import *
from OpenGL.GLU import *


def modelVec3(x, y, z):
	return Vec3(x, z, -y)

def modelscaleVec3(x, y, z):
	return Vec3(x, z, y)

def modelrotVec4(x, y, z, w):
	return Vec4(-x, -z, y, w)



# HEADER


class ModelHeader:
	def __init__(self, f):
		self.fourcc = unpack('4s', f.read(4))
		self.version = unpack('4B', f.read(4))
		(	self.n_name,
			self.o_name,
			self.type, # UNKNOWN: 0, 1, 3
			self.n_globalsequences,
			self.o_globalsequences,
			self.n_animation,
			self.o_animation,
			self.n_c, # UNKNOWN: animlookup?
			self.o_c,
			self.n_d, # UNKNOWN: 203 (or 201) animlookup?
			self.o_d,
			self.n_bones,
			self.o_bones,
			self.n_f, # UNKNOWN: bonelookup?
			self.o_f,
			self.n_vertices,
			self.o_vertices,
			self.n_views,
			self.o_views,
			self.n_colors,
			self.o_colors,
			self.n_textures,
			self.o_textures,
			self.n_transparency,
			self.o_transparency,
			self.n_i, # UNKNOWN: 0
			self.o_i,
			self.n_texanims,
			self.o_texanims,
			self.n_k, # UNKNOWN: revtexlookup for replacable tex?
			self.o_k,
			self.n_renderflags,
			self.o_renderflags,
			self.n_y, # UNKNOWN: bonelookup for geomertyanim?
			self.o_y,
			self.n_texlookup,
			self.o_texlookup,
			self.n_texunits,
			self.o_texunits,
			self.n_translookup,
			self.o_translookup,
			self.n_texanimlookup,
			self.o_texanimlookup,
		) = unpack('43I', f.read(43*4))
		self.floats = unpack('14f', f.read(14*4)) # UNKNOWN
		(	self.n_boundingtriangles,
			self.o_boundingtriangles,
			self.n_boundingvertices,
			self.o_boundingvertices,
			self.n_boundingnormals,
			self.o_boundingnormals,
			self.n_o, # UNKNOWN: attachments
			self.o_o,
			self.n_p, # UNKNOWN: attachmentlookup
			self.o_p,
			self.n_q, # UNKNOWN: attachments
			self.o_q,
			self.n_lights,
			self.o_lights,
			self.n_cameras,
			self.o_cameras,
			self.n_cameralookup,
			self.o_cameralookup,
			self.n_ribbonemitters,
			self.o_ribbonemitters,
			self.n_particleemitters,
			self.o_particleemitters,
		) = unpack('22I', f.read(22*4))
	
	def __str__(self):
		s = ''
		for i in dir(self):
			s += i+' : '+repr(getattr(self, i)) + '\n'
		return s



# ANIMATION AND SKELETON


class ModelAnimation:
	def __init__(self, f):
		(	self.id,
			self.starttime,
			self.endtime,
			self.movespeed,
			self.once,
			self.flags,
			self.unknown1, # UNKNOWN
			self.unknown2, # UNKNOWN: 0
			self.playspeed,
		) = unpack('3If5I', f.read(36))
		self.boundingbox = unpack('6f', f.read(24)) # 2*Vec3 ?
		(	self.radius,
			self.unknown3,
			self.unknown4,
		)= unpack('fHH', f.read(8))


class AnimationBlock:
	def __init__(self, f):
		(	self.type, # interpolation type (0=none, 1=linear, 2=hermite)
			self.seq,
			self.n_ranges,
			self.o_ranges,
			self.n_times,
			self.o_times,
			self.n_keys,
			self.o_keys,
		) = unpack('hh6I', f.read(28))


class ModelBones:
	def __init__(self, f):
		(	self.animid,
			self.flags, # 8=billboard, 512=transformed
			self.parent,
			self.geoid,
		) = unpack('IIHH', f.read(12))
		self.trans = AnimationBlock(f)
		self.rot = AnimationBlock(f)
		self.scale = AnimationBlock(f)
		self.pivot = modelVec3(*unpack('fff', f.read(12))) # Vec3 ?



# GEOMETRY


class ModelVertex:
	def __init__(self, f):
		self.pos = modelVec3(*unpack('fff', f.read(12)))
		self.bone_weights = unpack('4B', f.read(4))
		self.bone_ids = unpack('4B', f.read(4))
		self.normal = modelVec3(*unpack('fff', f.read(12)))
		self.texcoords = Vec2(*unpack('ff', f.read(8)))
		self.unknown = unpack('ff', f.read(8)) # UNKNOWN: 0


class ModelGeoset:
	def __init__(self, f):
		(	self.id,
			self.id2, # UNKNOWN
			self.vstart,
			self.vcount,
			self.istart,
			self.icount,
			self.ycount, # UNKNOWN
			self.ystart, # UNKNOWN
			self.unknown1, # UNKNOWN
			self.unknown2, # UNKNOWN
		) = unpack('10H', f.read(10*2))
		self.vec = modelVec3(*unpack('fff', f.read(12))) # UNKNOWN


class ModelViewHeader:
	def __init__(self, f):
		(	self.n_vertlookup,
			self.o_vertlookup,
			self.n_tris,
			self.o_tris,
			self.n_props,
			self.o_props,
			self.n_sub,
			self.o_sub,
			self.n_tex,
			self.o_tex,
			self.lod,
		) = unpack('10Ii', f.read(44))


class ModelView:
	def __init__(self, f, header):
		assert header.n_vertlookup==header.n_props, 'n_vertlookup:%d n_props:%d'%(header.n_vertlookup, header.n_props)

		f.seek(header.o_vertlookup)
		self.vertlookup = unpack(header.n_vertlookup*'H', f.read(header.n_vertlookup*2))
		f.seek(header.o_tris)
		self.tris = unpack(header.n_tris*'H', f.read(header.n_tris*2))
		f.seek(header.o_props)
		self.props = unpack(header.n_props*'H', f.read(header.n_props*2))
		f.seek(header.o_sub)
		self.sub = [ModelGeoset(f) for i in range(header.n_sub)]
		f.seek(header.o_tex)
		self.tex = [ModelTexUnit(f) for i in range(header.n_tex)]
		
		self.lod = header.lod
		self.trisverts = [self.vertlookup[i] for i in self.tris]
		self.passes = []



# TEXTURES AND RENDERING


class ModelTextureHeader:
	def __init__(self, f):
		(	self.type,
			self.flags,
			self.n_name,
			self.o_name,
		) = unpack('4I', f.read(4*4))


class ModelTexture:
	def __init__(self, f, header):
		f.seek(header.o_name)
		self.name = f.read(header.n_name-1)
		self.type = header.type
		self.flags = header.flags


class ModelTexUnit:
	def __init__(self, f):
		(	self.flags,
			self.order,
			self.meshid,
			self.meshid2, # UNKNOWN: == meshid ?
			self.colorid,
			self.flagid,
			self.texunit,
			self.unknown, # UNKNOWN: 1
			self.texid,
			self.texunit2, # UNKNOWN: == texunit ?
			self.transid,
			self.animid,
		) = unpack('HhHHh7H', f.read(12*2))


class ModelRenderFlag:
	def __init__(self, f):
		self.flags, self.blend = unpack('HH', f.read(4))


class RenderPass:
	def __init__(self):
		self.vstrart = 0
		self.vend = 0
		self.istart = 0
		self.icount = 0
		self.texture = 0
		self.texture2 = 0
		self.usetex2 = False
		self.useenvmap = False
		self.cull = False
		self.trans = False
		self.unlit = False
		self.nozwrite = False
		self.p = 0.0
		self.texanim = 0
		self.color = 0
		self.opacity = 0
		self.blendmode = 0
		self.order = 0
		
	def __cmp__(self, rp):
		if self.order==rp.order:
			if self.blendmode==rp.blendmode:
				return cmp(self.p, rp.p)
			else:
				return cmp(self.blendmode, rp.blendmode)
		else:
			return cmp(self.order, rp.order)


class ModelTexAnim:
	def __init__(self, f):
		self.trans = AnimationBlock(f)
		self.rot = AnimationBlock(f)
		self.scale = AnimationBlock(f)


class ModelColor:
	def __init__(self, f):
		self.color = AnimationBlock(f)
		self.opacity = AnimationBlock(f)


class ModelTransparency:
	def __init__(self, f):
		self.trans = AnimationBlock(f)



# EFFECTS


class ModelParticleEmitter:
	def __init__(self, f):
		(	self.id,
			self.flags,
		) = unpack('ii', f.read(8))
		self.pos = modelVec3(*unpack('fff', f.read(12)))
		(	self.bone,
			self.tex,
			self.n_ints1,
			self.o_ints1,
			self.n_ints2,
			self.o_ints2,
			self.blend,
			self.emitter, # type
			self.particle,
			self.texrot,
			self.cols, # WIKI ERROR: rows, cols (modelheaders.h: cols, rows) 
			self.rows,
		) = unpack('hh4I6h', f.read(32))
		self.params = [AnimationBlock(f) for i in range(10)]
		# WIKI ERROR: float[36]
		self.mid, = unpack('f', f.read(4))
		self.color = unpack('III', f.read(12))
		self.size = unpack('fff', f.read(12))
		self.tileids = unpack('10h', f.read(20))
		self.unknown1 = unpack('fff', f.read(12))
		self.unknown2 = unpack('fff', f.read(12))
		(	self.slowdown,
			self.rot,
		) = unpack('ff', f.read(8))
		self.unknown3 = unpack('10f', f.read(12))
		self.unknown4 = unpack('6f', f.read(12))
		self.unknown5 = AnimationBlock(f)


class ModelRibbonEmitter:
	def __init__(self, f):
		(	self.id,
			self.bone, # WIKI ERROR: int32 (bone_id : int16 ???)
		) = unpack('ii', f.read(8))
		self.pos = modelVec3(*unpack('fff', f.read(12)))
		(	self.n_texref,
			self.o_texref,
			self.n_ints2,
			self.o_ints2,
		) = unpack('IIII', f.read(16)) # WIKI ERROR: int32 / uint32 ???
		self.color = AnimationBlock(f)
		self.opacity = AnimationBlock(f)
		self.above = AnimationBlock(f)
		self.below = AnimationBlock(f)
		(	self.res,
			self.length,
			self.unknown1,
		) = unpack('fff', f.read(12))
		self.blend = unpack('hh', f.read(4))
		self.unknown2 = AnimationBlock(f)
		self.unknown3 = AnimationBlock(f)



# MISC


class ModelLight:
	def __init__(self, f):
		self.type, self.bone = unpack('HH', f.read(4))
		self.pos = modelVec3(*unpack('fff', f.read(12))) # Vec3 ?
		self.amb_color = AnimationBlock(f)
		self.amb_intensity = AnimationBlock(f)
		self.color = AnimationBlock(f)
		self.intensity = AnimationBlock(f)
		self.start = AnimationBlock(f) # attenuation start
		self.end = AnimationBlock(f) # attenuation end
		self.unknown = AnimationBlock(f)


class ModelCamera:
	def __init__(self, f):
		(	self.id,
			self.fov,
			self.farclip,
			self.nearclip
		) = unpack('ifff', f.read(16))
		self.trans_pos = AnimationBlock(f)
		self.pos = modelVec3(*unpack('fff', f.read(12))) # Vec3
		self.trans_target = AnimationBlock(f)
		self.target = modelVec3(*unpack('fff', f.read(12))) # Vec3
		self.rot = AnimationBlock(f)



# UNKNOWN


class C:
	pass
class D:
	pass
class F:
	pass
class I:
	pass
class K:
	pass
class Y:
	pass
class O:
	pass
class P:
	pass
class Q:
	pass



# MODEL


class Model:
	def __init__(self, f):
		#self.f = f
		self.header = header = self._header(f)
		self.name = self._name(f, header)
		#self.globalsequences = self._globalsequences(f, header)
		#self.animations = self._animations(f, header)
		#self.bones = self._bones(f, header)
		self.vertices = self._vertices(f, header)
		self.radius = self._radius()
		self.textures = self._textures(f, header)
		self.texlookup = self._texlookup(f, header)
		self.texunitlookup = self._texunitlookup(f, header)
		#self.texanims = self._texanims(f, header)
		#self.texanimlookup = self._texanimlookup(f, header)
		#self.transparency = self._transparency(f, header)
		self.translookup = self._translookup(f, header)
		#self.colors = self._colors(f, header)
		self.renderflags = self._renderflags(f, header)
		#self.lights = self._lights(f, header)
		#self.cameras = self._cameras(f, header)
		#self.cameralookup = self._cameralookup(f, header)
		self.views = self._views(f)
		#self.boundingvertices = self._boundingvertices(f, header)
		#self.boundingtriangles = self._boundingtriangles(f, header)
		#self.boundingnormals = self._boundingnormals(f, header)
		#f.close()
	
	def _header(self, f):
		f.seek(0)
		return ModelHeader(f)
	
	def _name(self, f, header):
		f.seek(header.o_name)
		return f.read(header.n_name-1)
	
	def _globalsequences(self, f, header):
		f.seek(header.o_globalsequences)
		n = header.n_globalsequences
		return unpack(n*'i', f.read(n*4))
	
	def _animations(self, f, header):
		f.seek(header.o_animations)
		return [ModelAnimation(f) for i in range(header.n_animations)]
	
	def _bones(self, f, header):
		f.seek(header.o_bones)
		return [ModelBone(f) for i in range(header.n_bones)]
	
	def _vertices(self, f, header):
		f.seek(header.o_vertices)
		return [ModelVertex(f) for i in range(header.n_vertices)]
	
	def _radius(self):
		if self.vertices:
			return sqrt(max(v.pos.lengthSquared() for v in self.vertices))
		return 0.0
	
	def _textures(self, f, header):
		f.seek(header.o_textures)
		theaders = [ModelTextureHeader(f) for i in range(header.n_textures)]
		return [ModelTexture(f, th) for th in theaders]
	
	def _texlookup(self, f, header):
		f.seek(header.o_texlookup)
		n = header.n_texlookup
		return unpack(n*'H', f.read(n*2))
	
	def _texunitlookup(self, f, header):
		f.seek(header.o_texunits)
		n = header.n_texunits
		return unpack(n*'h', f.read(n*2))
	
	def _texanims(self, f, header):
		f.seek(header.o_texanims)
		return [ModelTexAnim(f) for i in range(header.n_texanims)]
	
	def _texanimlookup(self, f, header):
		f.seek(header.o_texanimlookup)
		n = header.n_texanimlookup
		return unpack(n*'H', f.read(n*2))
	
	def _transparency(self, f, header):
		f.seek(header.o_transparency)
		return [ModelTransparency(f) for i in range(header.n_transparency)]
	
	def _translookup(self, f, header):
		f.seek(header.o_translookup)
		n = header.n_translookup
		return unpack(n*'h', f.read(n*2))
	
	def _colors(self, f,header):
		f.seek(header.o_colors)
		return [ModelColor(f) for i in range(header.n_colors)]
	
	def _renderflags(self, f, header):
		f.seek(header.o_renderflags)
		return [ModelRenderFlag(f) for i in range(header.n_renderflags)]
	
	def _lights(self, f, header):
		f.seek(header.o_lights)
		return [ModelLight(f) for i in range(header.n_lights)]
	
	def _cameras(self, f, header):
		f.seek(header.o_cameras)
		return [ModelCamera(f) for i in range(header.n_cameras)]
	
	def _cameralookup(self, f, header):
		f.seek(header.o_cameralookup)
		n = header.n_cameralookup
		return unpack(n*'H', f.read(n*2))
	
	def _boundingvertices(self, f, header):
		f.seek(header.o_boundingvertices)
		return [modelVec3(*unpack('fff', f.read(12))) for i in range(header.n_boundingvertices)]
	
	def _boundingtriangles(self, f, header):
		f.seek(header.o_boundingtriangles)
		n = header.n_boundingtriangles
		return unpack(n*'H', f.read(n*2))
	
	def _boundingnormals(self, f, header):
		f.seek(header.o_boundingnormals)
		return [modelVec3(*unpack('fff', f.read(12))) for i in range(header.n_boundingnormals)]
	
	def _views(self, f):
		header = self.header
		vertices = self.vertices
		textures = self.textures
		texlookup = self.texlookup
		texunitlookup = self.texunitlookup
		translookup = self.translookup
		renderflags = self.renderflags
		
		f.seek(header.o_views)
		viewheaders = [ModelViewHeader(f) for i in range(header.n_views)]
		
		views = []
		for vh in viewheaders:
			view = ModelView(f, vh)
			#'''
			passes = view.passes
			sub = view.sub
			for t in view.tex:
				pss = RenderPass()
				geoset = sub[t.meshid]
				pss.vstart = geoset.vstart
				pss.vend = geoset.vstart + geoset.vcount
				pss.istart = geoset.istart
				pss.iend = geoset.istart + geoset.icount
				pss.order = t.order
				pss.texture = textures[texlookup[t.texid]]
				#???
				if t.texunit>=len(texunitlookup):
					print 'texunit:',t.texunit
					print 'texunitlookup:',texunitlookup
					print 'texture:',pss.texture.name
				else:
					pss.useenvmap = texunitlookup[t.texunit]==-1
				rf = renderflags[t.flagid]
				pss.blendmode = rf.blend
				pss.color = t.colorid
				pss.opacity = translookup[t.transid]
				pss.cull = (rf.flags & 4)==0 and rf.blend==0
				pss.unlit = (rf.flags & 3)!=0
				pss.nozwrite = rf.blend!=0
				pss.p = geoset.vec.x
				
				self.animtextures = False
				
				if self.animtextures:
					if t.flags & 16:
						pss.texanim = -1
					else:
						pss.texanim = texanimlookup[t.animid]
				else:
					pss.texanim = -1
				passes.append(pss)
			# transparent parts come later
			passes.sort()
			#'''
			
			views.append(view)
		
		return views
	
	def _animated(self):
		return self._animated_geometry() or self._animated_textures() or self._animated_misc()
	
	def _animated_geometry(self):
		return _animated_geometry_ind()[0]
	
	def _animated_textures(self):
		return bool(self.texanims)
		#header = self.header
		#return header.n_texanims > 0
	
	def _animated_misc(self):
		return self._animated_effects() or self._animated_colors() or self._animated_transparency()
	
	def _animated_bones(self):
		if self._animated_geometry() or self._animated_effects():
			return True
		for b in self.bones:
			if b.trans.type or b.rot.type or b.scale.type:
				return True
		return False
	
	def _animated_geometry_ind(self):
		for v in self.vertices:
			for i in range(4):
				if v.bone_weights[i] > 0:
					b = self.bones[v.bone_ids[i]]
					if b.trans.type or b.rot.type or b.scale.type or b.flags&8:
						if b.flags&8:
							#ind = True
							return True, True
						return True, False
						#animgeo = True
						#animbones = True
						#return True
		return False, False
	
	def _animated_effects(self):
		header = self.header
		return (header.n_cameras > 0 or
				header.n_lights > 0 or
				header.n_ribbonemitters > 0 or
				header.n_particleemitters > 0)
	
	def _animated_colors(self):
		for c in self.colors:
			if c.color.type or c.opacity.type:
				return True
		return False
	
	def _animated_transparency(self):
		for t in self.transparency:
			if t.trans.type:
				return True
		return False


if __name__=='__main__':
	pass
	#m = Model(f)
	#print m.header
	
	#for i in dir(m.header):
	#	print i, getattr(m.header, i)
	
	
	#app = wx.App()
	#f = wx.Frame(None, title = 'test')
	#v = Canvas(f)
	#mv = ModelViewer(v, [m])
	#mv.run()
	#f.Show()
	#app.MainLoop()
	
