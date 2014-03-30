#!BPY

# """
# Name: 'M2 Model (.m2)...'
# Blender: 243
# Group: 'Import'
# Tooltip: 'Import a M2 Blizzard Model File'
# """

__author__ = "Richard Adenling"
__version__ = "0.7alpha 2007/03/11"
__url__ = ["Author's site, http://www.squishythoughts.com",
        "M2-format reference, http://wowdev.org/wiki/index.php/M2"]
__email__ = ["Author, dreeze:gmail*com"]
__bpydoc__ = """\
Description: Imports a .M2 file used by World of Warcraft (WoW)

Features:<br>
  * Imports mesh, normals and uv-coordinates.<br>
  * GUI-interface and preview for models containing multiple parts (hair,
beards, etc).<br>
  * Loading and binding of armature to mesh objects.<br>

Usage:<br>
  Click "Select file" and choose the .M2 file to load. Click
"Load model". The next menu have two options:<br>

  "Configure model"<br>
This opens a menu allowing the user to select which parts of 
the model to keep. The user will have to open a 3D-view to
see what parts are active (active parts are in layer 1, the other
parts are in layer 2).
Clicking on "Freeze" will remove all the unused parts in layer 2.

  "Animations"<br>
Allows the user to load the bones data from the .M2 file. It will
automatically be set as the armature for all the geoset objects.

Other tools needed:<br>
  .M2 files are extracted from WoW's .MPQ files with tools like
MPQEdit or WinMPQ.<br>
  Textures (.BLP-files) from WoW can be converted with
Wowimage (http://wowdev.org/wiki/index.php/Wowimage).

Notes:<br>
  .M2 files for player models contain all the different hair and
beard combinations, and some of the armors and clothes. These parts
are called geosets. Separating the geoset's by hand is possible but
not a very pleasant task. It is still possible to do this, though. By
not clicking "Freeze" all objects which are not active will be in
layer 2.

  Currently, textures are not loaded and will have to be assigned by the
user.

"""
# M2 importer (World of Warcraft models) for Blender
#
# ***** BEGIN LICENSE BLOCK ****
#
# Copyright (C) 2007  Richard Adenling, dreeze at gmail dot com
# 
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
#
# ***** END LICENSE BLOCK *****

import math
import Blender
import struct
import array
import time
import profile
from Blender.BGL import *
from Blender.Mathutils import *

scene = Blender.Scene.GetCurrent()

# 8 bytes
class M2Chunk:
    def __init__(self,f):
        self.count,     = struct.unpack("l",f.read(4))
        self.offset,    = struct.unpack("l",f.read(4))

# 324 bytes
class M2Header:
    def __init__(self,f):
        self.magic,         = struct.unpack("4s",f.read(4))
        self.version        = struct.unpack("4B",f.read(4))
        self.name_length,   = struct.unpack("l",f.read(4))
        self.name_offset,   = struct.unpack("l",f.read(4))
        self.model_type,    = struct.unpack("l",f.read(4))
        self.global_sequences = M2Chunk(f)
        self.animations     = M2Chunk(f)
        self.C              = M2Chunk(f)
        self.D              = M2Chunk(f)
        self.bones          = M2Chunk(f)
        self.F              = M2Chunk(f)
        self.vertices       = M2Chunk(f)
        self.views          = M2Chunk(f)
        self.colors         = M2Chunk(f)
        self.textures       = M2Chunk(f)
        self.transparency   = M2Chunk(f)
        self.I              = M2Chunk(f)
        self.texanims       = M2Chunk(f)
        self.K              = M2Chunk(f)
        self.render_flags   = M2Chunk(f)
        self.Y              = M2Chunk(f)
        self.tex_lookup     = M2Chunk(f)
        self.tex_units      = M2Chunk(f)
        self.trans_lookup   = M2Chunk(f)
        self.tex_anim_lookup = M2Chunk(f)
        f.seek(14*4,1)      # skip unknown data
        self.bounding_triangles = M2Chunk(f)
        self.bounding_vertices = M2Chunk(f)
        self.bounding_normals = M2Chunk(f)
        self.O              = M2Chunk(f)
        self.P              = M2Chunk(f)
        self.Q              = M2Chunk(f)
        self.lights         = M2Chunk(f)
        self.cameras        = M2Chunk(f)
        self.camera_lookup  = M2Chunk(f)
        self.ribbon_emitters = M2Chunk(f)
        self.particle_emitters = M2Chunk(f)

# 48 bytes
class M2Vertex:
    def __init__(self,f):
        self.pos        = struct.unpack("3f",f.read(12))
        self.bweights   = struct.unpack("4B",f.read(4))
        self.bindices   = struct.unpack("4B",f.read(4))
        self.normal     = struct.unpack("3f",f.read(12))
        self.uv         = struct.unpack("2f",f.read(8))
        self.uv         = (self.uv[0],1.0 - self.uv[1])
        unknown         = struct.unpack("2f",f.read(8))

# 48 bytes
class M2Mesh:
    def __init__(self,f):
        self.mesh_id,       = struct.unpack("l",f.read(4))
        self.verts_offset,  = struct.unpack("H",f.read(2))
        self.num_verts,     = struct.unpack("H",f.read(2))
        self.tris_offset,   = struct.unpack("H",f.read(2))
        self.num_tris,      = struct.unpack("H",f.read(2))
        f.seek(36,1)    # skip the rest

# 24 bytes
class M2Texture:
    def __init__(self,f):
        self.flags,         = struct.unpack("H",f.read(2))
        self.render_order,  = struct.unpack("h",f.read(2))
        self.submesh,       = struct.unpack("H",f.read(2))
        unknown,            = struct.unpack("H",f.read(2))
        self.color_index,   = struct.unpack("h",f.read(2))
        self.render_flags_i,= struct.unpack("H",f.read(2))
        self.texture_unit,  = struct.unpack("H",f.read(2))
        unknown,            = struct.unpack("H",f.read(2))
        self.texture,       = struct.unpack("H",f.read(2))
        unknown,            = struct.unpack("H",f.read(2))
        self.transparency,  = struct.unpack("H",f.read(2))
        self.animation      = struct.unpack("H",f.read(2))

# 44 bytes
class M2View:
    def __init__(self,f):
        oldpos = f.tell()

        indexchunk = M2Chunk(f)
        trianglechunk = M2Chunk(f)
        vertpropchunk = M2Chunk(f)
        meshchunk = M2Chunk(f)
        texturechunk = M2Chunk(f)

        f.seek(indexchunk.offset)
        self.indices = array.array('H',f.read(indexchunk.count*2))
        
        f.seek(trianglechunk.offset)
        self.triangles = []
        for i in range(trianglechunk.count / 3):
            t = struct.unpack("3H",f.read(6))
            self.triangles.append(t)

        f.seek(meshchunk.offset)
        self.meshes = []
        for i in range(meshchunk.count):
            self.meshes.append(M2Mesh(f))

        f.seek(texturechunk.offset)
        self.textures = []
        for i in range(texturechunk.count):
            self.textures.append(M2Texture(f))

        f.seek(oldpos)

# 28 bytes
class M2AnimationBlock:
    def __init__(self,f):
        self.interp_type, = struct.unpack("h",f.read(2))
        self.global_seq_id, = struct.unpack("h",f.read(2))
        interp_range = M2Chunk(f)
        timestampschunk = M2Chunk(f)
        self.valueschunk = M2Chunk(f)

        oldpos = f.tell()
        f.seek(timestampschunk.offset)
        self.timestamps = []
        for i in range(timestampschunk.count):
            t, = struct.unpack("l",f.read(4))
            self.timestamps.append(t)
        
#        print "Num timestamps:",len(self.timestamps)
#        if len(self.timestamps) > 2:
#            print "timestamp:",self.timestamps[1]
        f.seek(oldpos)

class M2VectorBlock(M2AnimationBlock):
    def __init__(self,f):
        M2AnimationBlock.__init__(self,f)
        oldpos = f.tell()
        f.seek(self.valueschunk.offset)

        self.values = []
        for i in range(self.valueschunk.count):
            t = struct.unpack("3f",f.read(12))
            self.values.append(t)

        f.seek(oldpos)

class M2QuatBlock(M2AnimationBlock):
    def __init__(self,f):
        M2AnimationBlock.__init__(self,f)
        oldpos = f.tell()
        f.seek(self.valueschunk.offset)

        def MakeFloat(v):
            f = 0
            if v > 0:
                f = v - 32767
            else:
                f = v + 32767

            return (f / 32767.0)

        self.values = []
        for i in range(self.valueschunk.count):
            t = struct.unpack("4h",f.read(8))
            quat = MakeFloat(t[3]), MakeFloat(t[0]), MakeFloat(t[1]), MakeFloat(t[2])
            self.values.append(quat)
#            print "Rot:",quat

        f.seek(oldpos)

# 112 bytes
class M2Bone:
    def __init__(self,f):
        oldpos = f.tell()
        self.index_into_F,  = struct.unpack("l",f.read(4))
        self.flags,         = struct.unpack("l",f.read(4))
        self.parent,        = struct.unpack("h",f.read(2))
        unknown             = struct.unpack("h",f.read(2))
        unknown             = struct.unpack("l",f.read(4))     # only exists in post-2.0 patches
        self.translation    = M2VectorBlock(f)
        self.rotation       = M2QuatBlock(f)
        self.scaling        = M2VectorBlock(f)
        self.pivot          = struct.unpack("3f",f.read(12))

#        print "Parent:", self.parent
#        print "Pivot:",self.pivot

# 68 bytes
class M2Animation:
    def __init__(self,f):
        self.anim_id,   = struct.unpack("l",f.read(4))
        self.start,     = struct.unpack("l",f.read(4))
        self.stop,      = struct.unpack("l",f.read(4))
        self.move_speed,= struct.unpack("f",f.read(4))
        self.looping,   = struct.unpack("l",f.read(4))
        self.flags,     = struct.unpack("l",f.read(4))
        unknown         = struct.unpack("l",f.read(4))
        unknown         = struct.unpack("l",f.read(4))
        self.play_speed,= struct.unpack("l",f.read(4))
        bbox            = struct.unpack("6f",f.read(4*6))
        radius          = struct.unpack("f",f.read(4))
        self.next_anim, = struct.unpack("h",f.read(2))
        filler          = struct.unpack("H",f.read(2))
#        print "Clicks:",(self.stop-self.start)
#        print "Speed:",self.play_speed


class M2File:
    def LoadVertices(self,f):
        f.seek(self.header.vertices.offset)
        self.verts = []
        for i in range(self.header.vertices.count):
            self.verts.append(M2Vertex(f))

    def LoadViews(self,f):
        f.seek(self.header.views.offset)
        self.views = []
        for i in range(self.header.views.count):
            self.views.append(M2View(f))

    def LoadBones(self,f):
        f.seek(self.header.bones.offset)
        self.bones = []
        for i in range(self.header.bones.count):
            self.bones.append(M2Bone(f))

    def LoadAnimations(self,f):
        f.seek(self.header.animations.offset)
        self.animations = []
        for i in range(self.header.animations.count):
            self.animations.append(M2Animation(f))

    def __init__(self,filename):
        try:
            f = open(filename)
            self.header = M2Header(f)
            self.LoadVertices(f)
            self.LoadViews(f)
            self.LoadBones(f)
            self.LoadAnimations(f)
        finally:
            f.close()


#def QuatFromVectors(v1, v2):
#    v1.normalize()
#    v2.normalize()
#    c = CrossVecs(v1,v2)
#    d = DotVecs(v1,v2)
#    if d < -0.999 and d > -1.001:
#        return Quaternion(-1,0,0,0)
#    s = math.sqrt((1.0 + d) * 2.0)
#    rs = 1.0 / s
#    q = Quaternion(s * 0.5, c.x * rs, c.y * rs, c.z * rs)
#    return q


# Triangle indices will need to be subtracted by verts_offset amount
def CreateMeshObject(model,name,verts_offset,num_verts,tris_offset,num_tris):
    mesh = Blender.Mesh.New(name)

    vslice = model.verts[verts_offset:(verts_offset + num_verts)]
    tslice = model.views[0].triangles[tris_offset:(tris_offset + num_tris)]

    # Create vertices and set normals
    mesh.verts.extend([ x.pos for x in vslice ])
    for i,v in enumerate(vslice):
        mesh.verts[i].no = Vector(v.normal)

    triuv = []
    tris = []

    # Prepare faces
    for tri in tslice:
        vindices = [ model.views[0].indices[x - verts_offset] for x in tri ]
        if vindices[2] == 0:
            vindices = vindices[1], vindices[2], vindices[0]

        triuv.append([ Vector(vslice[x].uv) for x in vindices ])
        tris.append(vindices)

    # Create faces
    face_mapping = mesh.faces.extend(tris, indexList = True)
    for i,uv in enumerate(triuv):
        if face_mapping[i] != None:
            face = mesh.faces[face_mapping[i]]
            face.uv = uv
            face.mode += Blender.Mesh.FaceModes['TEX']

    # Link object
    ob = scene.objects.new(mesh,name)
    mesh = ob.getData(mesh = True)

    # Assign weights and create vertex groups
    for boneindex,b in enumerate(model.bones):
        vlist = []
        weightbyte = 0
        for i in range(4):
            for vindex,v in enumerate(vslice):
                index = vindex + verts_offset
                if v.bindices[i] == boneindex:
                    vlist.append(vindex)
                    # NOTE: Only uses the last weight found, could cause
                    # animation artifacts
                    weightbyte = v.bweights[i]

        if len(vlist) > 0:
            name = 'bone' + str(boneindex)
            weight = weightbyte / 256.0
            mesh.addVertGroup(name)
            mesh.assignVertsToGroup(name,vlist,weight,Blender.Mesh.AssignModes.REPLACE)


    return ob

def CreateSubMesh(model,index):
    submesh = model.views[0].meshes[index]
    name = ""
    if submesh.mesh_id < 100:
        name = 'Geoset%02u' % submesh.mesh_id
    else:
        name = 'Geoset%04u' % submesh.mesh_id
    return CreateMeshObject(model,name,submesh.verts_offset,submesh.num_verts,submesh.tris_offset / 3,submesh.num_tris / 3)

def CreateArmature(model):
    global scene
    arm = Blender.Armature.New("WowArmature")
    arm_ob = scene.objects.new(arm,"WowArmature")

    arm.makeEditable()
    eblist = []

    for i,b in enumerate(model.bones):
        eb = Blender.Armature.Editbone()
        pivot = Vector(b.pivot)
        if b.parent == -1:
            eb.head = pivot
            eb.tail = eb.head + Vector(0,0.25,0)
        else:
            eb.parent = eblist[b.parent]
            eb.head = pivot
            eb.tail = pivot + Vector(0,0.25,0)
            
            # Need to check for this because blender removes zero-length bones
            newbone = eblist[b.parent].head - eb.head
            if newbone.length < 0.001:
                eblist[b.parent].tail = eb.head + Vector(0,0.1,0)
            else:
                eblist[b.parent].tail = eb.head

        arm.bones['bone' + str(i)] = eb
        eblist.append(eb)
    arm.drawType = Blender.Armature.STICK
    arm.update()
    return arm_ob

def AttachArmature():
    # Find all geoset objects
    global scene
    mesh_parts = filter(lambda x: x.name[:6] == 'Geoset',list(scene.objects))
    arm_ob = filter(lambda x: x.name[:11] == "WowArmature",list(scene.objects))

    # Attach armature to mesh parts
    for m in mesh_parts:
        mods = m.modifiers
        mod = mods.append(Blender.Modifier.Types.ARMATURE)
        mod[Blender.Modifier.Settings.OBJECT] = arm_ob[0]

    scene.update(0)


def main():
    global scene
    mesh = Blender.Mesh.New ('theMesh')

    theFile = M2File(m2file)

    #print "Client version:", header[HeaderStruct.nD]
    print "Vertices:", theFile.header.vertices.count
    #print "Triangles:", views[0][ViewStruct.nTriangles] / 3
    #print "Meshes:", views[0][ViewStruct.nSubMeshes]
    #print "Views:", header[HeaderStruct.nViews]
    #print "Textures:", header[HeaderStruct.nTextures]
    print "Bones:", theFile.header.bones.count
    print "Animations:", theFile.header.animations.count
    print "Sub-meshes:", len(theFile.views[0].meshes)
    print "Textures:",len(theFile.views[0].textures)

#    for m in theFile.views[0].meshes:
#        print m.mesh_id

#    for t in theFile.views[0].textures:
#        print t.submesh

#    numverts = theFile.header.vertices.count
#    numtris = len(theFile.views[0].triangles)
#    mesh_ob = CreateMeshObject(theFile,'theMesh',0,numverts,0,numtris)
#    mesh = mesh_ob.getData(mesh = True)

    mesh_parts = []
    for i in theFile.views[0].meshes:
        mesh_parts.append(CreateSubMesh(theFile,i))

    arm_ob = CreateArmature(theFile)
    arm = arm_ob.getData()

    bones_inv = {}
    abones = arm.bones.values()
    for b in abones:
        mat = Matrix(b.matrix['ARMATURESPACE'])
        mat.invert()
        bones_inv[b.name] = mat

    # Attach armature to mesh parts
    for m in mesh_parts:
        mods = m.modifiers
        mod = mods.append(Blender.Modifier.Types.ARMATURE)
        mod[Blender.Modifier.Settings.OBJECT] = arm_ob

    scene.update(0)

    # Set vertex groups, weights and bone bindings
#    mesh = mesh_ob.getData(mesh = True)
#    for bindex,b in enumerate(theFile.bones):
#        vindices = []
#        weight = 0
#        for i in range(4):
#            for vindex,v in enumerate(theFile.verts):
#                if v.bindices[i] == bindex:
#                    vindices.append(vindex)
#                    weightbyte = v.bweights[i]
#
#        if len(vindices) > 0:
#            name = str(bindex)
#            weight = weightbyte / 256.0
#            mesh.addVertGroup(name)
#            mesh.assignVertsToGroup(name,vindices,weight,Blender.Mesh.AssignModes.REPLACE)

    # Create actions from animations
    action = Blender.Armature.NLA.NewAction("animation")
    action.setActive(arm_ob)
    pose = arm_ob.getPose()
    pbones = pose.bones.values()

    def MultiplyQuat(q1, q2):
        w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z
        x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y
        y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x
        z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w
        return Quaternion([w,x,y,z])

    # TODO: Figure out if SetTrans or SetScale needs to transform the coordinate system
    # before applying it's transformation

    def SetQuat((pbone,ipo,quat,restquat),frame):
        preadjust = restquat
        postadjust = Quaternion(restquat)
        postadjust.inverse()
        tmp = MultiplyQuat(preadjust,Quaternion(quat))
        result = MultiplyQuat(tmp,postadjust)

        curve_w = ipo[Blender.Ipo.PO_QUATW]
        curve_x = ipo[Blender.Ipo.PO_QUATX]
        curve_y = ipo[Blender.Ipo.PO_QUATY]
        curve_z = ipo[Blender.Ipo.PO_QUATZ]

        curve_w.interpolation = Blender.IpoCurve.InterpTypes.LINEAR
        curve_x.interpolation = Blender.IpoCurve.InterpTypes.LINEAR
        curve_y.interpolation = Blender.IpoCurve.InterpTypes.LINEAR
        curve_z.interpolation = Blender.IpoCurve.InterpTypes.LINEAR

        curve_w.append((frame,result.w))
        curve_x.append((frame,result.x))
        curve_y.append((frame,result.y))
        curve_z.append((frame,result.z))


#        pbone.quat = MultiplyQuat(tmp,postadjust)
#        return (pbone,Blender.Object.Pose.ROT)
#        pbone.insertKey(arm_ob,frame,Blender.Object.Pose.ROT)

    def SetTrans((pbone,ipo,trans),frame):
        curve_x = ipo[Blender.Ipo.PO_LOCX]
        curve_y = ipo[Blender.Ipo.PO_LOCY]
        curve_z = ipo[Blender.Ipo.PO_LOCZ]

        curve_x.interpolation = Blender.IpoCurve.InterpTypes.LINEAR
        curve_y.interpolation = Blender.IpoCurve.InterpTypes.LINEAR
        curve_z.interpolation = Blender.IpoCurve.InterpTypes.LINEAR

        curve_x.append((frame,trans[0]))
        curve_y.append((frame,trans[1]))
        curve_z.append((frame,trans[2]))
#        pbone.loc = Vector(trans)
#        return (pbone,Blender.Object.Pose.LOC)
#        pbone.insertKey(arm_ob,frame,Blender.Object.Pose.LOC)

    def SetScale((pbone,ipo,scale),frame):
        curve_x = ipo[Blender.Ipo.PO_SCALEX]
        curve_y = ipo[Blender.Ipo.PO_SCALEY]
        curve_z = ipo[Blender.Ipo.PO_SCALEZ]

        curve_x.interpolation = Blender.IpoCurve.InterpTypes.LINEAR
        curve_y.interpolation = Blender.IpoCurve.InterpTypes.LINEAR
        curve_z.interpolation = Blender.IpoCurve.InterpTypes.LINEAR

        curve_x.append((frame,scale[0]))
        curve_y.append((frame,scale[1]))
        curve_z.append((frame,scale[2]))
#        pbone.size = Vector(scale)
#        return (pbone,Blender.Object.Pose.SIZE)
#        pbone.insertKey(arm_ob,frame,Blender.Object.Pose.SIZE)

    def AddKeyframes(kdict,pbone,ipo,function,animblock,restdata = None):
        for time,animdata in zip(animblock.timestamps,animblock.values):
            data = None
            if restdata != None:
                data = (pbone,ipo,animdata,restdata)
            else:
                data = (pbone,ipo,animdata)

            if kdict.has_key(time):
                kdict[time].append((function,data))
            else:
                kdict[time] = [(function,data)]

    # Set keyframes for frame 1
    for pbone in pose.bones.values():
        restbone = bone_adjust[int(pbone.name)].copy()
    #        restbone.inverse()
    #        print restbone,Quaternion([0,1,0],45)
    #        print "Result:",MultiplyQuat(Quaternion([0,1,0],45),restbone)
    #        print "Bone:",pbone.name,"Angle:",restbone.angle
    #        pbone.quat = MultiplyQuat(Quaternion([0,1,0],45),restbone)
    #        pbone.quat = Quaternion([0,1,0],45)
        preadjust = restbone
        postadjust = Quaternion(restbone)
        postadjust.inverse()
    #        tmp = MultiplyQuat(preadjust,Quaternion([0,1,0],45))
    #        pbone.quat = MultiplyQuat(tmp,postadjust)
        pbone.insertKey(arm_ob,1,Blender.Object.Pose.ROT)
        pbone.insertKey(arm_ob,1,Blender.Object.Pose.LOC)
        pbone.insertKey(arm_ob,1,Blender.Object.Pose.SIZE)

    pose.update()

    ipo_channels = action.getAllChannelIpos()

    keyframes = {}
    pbonekeys = pose.bones.keys()
    pbonekeys.sort()
    for i,bone in enumerate(theFile.bones):
        pbone = pose.bones[str(i)]
        restquat = bone_adjust[i]
        ipo = ipo_channels[str(i)]
        AddKeyframes(keyframes,pbone,ipo,SetTrans,bone.translation)
        AddKeyframes(keyframes,pbone,ipo,SetQuat,bone.rotation, restdata = Quaternion(restquat))
        AddKeyframes(keyframes,pbone,ipo,SetScale,bone.scaling)

    frames = keyframes.keys()
    frames.sort()

    print "Number of keyframes:",len(keyframes)
    counter = 0
    totaltime = 0
    curtime = time.clock()
    for frameindex,frame in enumerate(frames):
        keyslist = keyframes[frame]
        keybones = { }
        for command,data in keyslist:
#            pbone,val = command(data,frameindex+1)
            command(data,frameindex+1)
#            if keybones.has_key(pbone.name):
#                keybones[pbone.name].append(val)
#            else:
#                keybones[pbone.name] = [val]
        
#        for bone,value in keybones.iteritems():
#            print "Pose bone:",bone
#            pose.bones[bone].insertKey(arm_ob,frameindex+1,value)

        counter += 1
        if counter > 100:
            newtime = time.clock()
            dtime = newtime - curtime
            totaltime += dtime
            print "Frames loaded:",frameindex,"in",dtime,"seconds"
            counter -= 100
            curtime = newtime

#        if frameindex == 10:
#            break

    print "Total loading time:",totaltime

    pose.update()

    #    arm_ob.update()

    #print "Client version:", header[HeaderStruct.nD]
    #print "Vertices:", header[HeaderStruct.nVertices]
    #print "Triangles:", views[0][ViewStruct.nTriangles] / 3
    #print "Meshes:", views[0][ViewStruct.nSubMeshes]
    #print "Views:", header[HeaderStruct.nViews]
    #print "Textures:", header[HeaderStruct.nTextures]
    print "Bones:", theFile.header.bones.count
    print "Animations:", theFile.header.animations.count

    scene.update(0)
    Blender.Redraw()

#profile.runctx("main()",globals(),locals())
#main()

theFile = None

def LoadModel(filename):
    global theFile
    theFile = M2File(filename)

    global scene
    scene.setLayers([1])
    mesh = Blender.Mesh.New ('theMesh')

    print "Vertices:", theFile.header.vertices.count
    #print "Triangles:", views[0][ViewStruct.nTriangles] / 3
    #print "Meshes:", views[0][ViewStruct.nSubMeshes]
    #print "Views:", header[HeaderStruct.nViews]
    #print "Textures:", header[HeaderStruct.nTextures]
    print "Bones:", theFile.header.bones.count
    print "Animations:", theFile.header.animations.count
    print "Sub-meshes:", len(theFile.views[0].meshes)
    print "Textures:",len(theFile.views[0].textures)

    mesh_parts = []
    for i,mesh in enumerate(theFile.views[0].meshes):
        mesh_parts.append(CreateSubMesh(theFile,i))

def LoadArmature():
    global theFile
    scene.setLayers([1])
    CreateArmature(theFile)
    AttachArmature()



# 0 = base
# 0+id = hair
# 100+id = beards
# 400+id = arms/gloves
# 500+id = shoes
# 700+id = ears?
# 800+id = armbands
# 900+id = knees?
# 1200+id = tabard
# 1300+id = pants
# 1500+id = capes

def GetGeosets(model):
    geosets = {}

    for m in model.views[0].meshes:
        if m.mesh_id > 0:
            index = int(m.mesh_id / 100)
            if geosets.has_key(index):
                if m.mesh_id not in geosets[index]:
                    geosets[index].append(m.mesh_id)
            else:
                geosets[index] = [m.mesh_id]
                first = index * 100 + 1
                # This is not pretty, but it works. If a geoset does not
                # have a first index (401,501,xx01), it means that it's
                # possible to turn off the display of that geoset. If we add
                # that index, the geoset won't be loaded (since it doesn't
                # exist) and when attempting to set it, ShowGeoset() won't
                # find it and still hide everything else within that geoset
                # category. Problem solved!
                if first != m.mesh_id:
                    geosets[index].append(first)

    values = geosets.values()
    for val in values: val.sort()
    return geosets

def GetGeosetCategory(category):
    global scene
    obs = list(scene.objects)
    catobs = []
    for o in obs:
        name = o.name[:]
        if name[:6] == 'Geoset':
            suffix = name[6:]
            doti = suffix.rfind('.')
            if doti != -1:
                suffix = suffix[:doti]

            index = int(suffix)
            if  index != 0 and \
                index >= category * 100 and \
                index <= category * 100 + 99:
                  catobs.append(o)

    return catobs

def ShowGeoset(category,index):
    obs = GetGeosetCategory(category)
    if category != 0:
        name = 'Geoset%02u%02u' % (category,index)
    else:
        name = 'Geoset%02u' % index
    for o in obs:
#        print "Checking visibility:",o.name
        if o.name.find(name) != -1:
#            print "Found:",o.name
            o.layers = [1]
        else:
            o.layers = [2]

def RemoveHiddenGeosets():
    global scene
    obs = list(scene.objects)
    for o in obs:
        if o.name[:6] == 'Geoset' and 2 in o.layers:
            scene.objects.unlink(o)

theme = Blender.Window.Theme.Get()[0]
themecolors = theme.get(-1)
color_back = tuple([x / 255.0 for x in themecolors.textfield ])
#color_button 

EVENT_EXIT          = 1
EVENT_BACK          = 2
EVENT_UNUSED        = 3

button_height = 22
class MenuLoad:
    EVENT_LOADMODEL = EVENT_UNUSED + 1
    EVENT_SELECTFILE = EVENT_UNUSED + 2
    name_button = None
    load_button = None
    selectfile_button = None
#    filename_field = Blender.Draw.Create("/home/richard/BloodElfMale.M2")
    filename_field = Blender.Draw.Create("")
    def Draw(self):
        Blender.Draw.BeginAlign()
        self.name_button = Blender.Draw.String("",EVENT_EXIT,10,125,250,button_height,self.filename_field.val,100,"Model to load")
        self.selectfile_button = Blender.Draw.PushButton("Select file",self.EVENT_SELECTFILE,260,125,80,button_height,"Select model to load")
        Blender.Draw.EndAlign()
        self.load_button = Blender.Draw.PushButton("Load model",self.EVENT_LOADMODEL,10,100,80,button_height,"Load model")

    def ButtonEvent(self,event):
        if event == self.EVENT_SELECTFILE:
            Blender.Window.FileSelector(lambda name: self.SetFile(name))
            Blender.Draw.Redraw(1)
        elif event == self.EVENT_LOADMODEL:
            print "Loading:",self.filename_field.val
            LoadModel(self.filename_field.val)

            global active_menu,menu_main
            active_menu = menu_main

            # Need to do this once the file has been loaded
            menu_geoset.LoadGeosets()

            Blender.Redraw()

    def SetFile(self,name):
        self.filename_field.val = name

class MenuGeoset:
    EVENT_FREEZE = EVENT_UNUSED+1
    EVENT_GEOSETCHANGESTART = EVENT_UNUSED+100
    EVENT_GEOSETCHANGEEND = EVENT_UNUSED+199

    geoset_buttons = {}
    freeze_button = None
    geosets = None
    geoset_active = {}

    geoset_names = { 0 : "Hair",
            1 : "Beard",
            3 : "Earrings",
            4 : "Gloves",
            5 : "Shoes",
            7 : "Long ears",
            8 : "Sleeve length",
            9 : "Pants length",
            12 : "Tabard",
            13 : "Robe/Pants",
            15 : "Cape" }

    def Draw(self):
        self.geoset_buttons = {}
        
        # starty is too arbitrary, should be based on something sane
        starty = 40 + 13 * button_height
        for k,active in self.geoset_active.items():
            num_choices = len(self.geosets[k])

            name = ""
            if self.geoset_names.has_key(k):
                name = self.geoset_names[k]
            else:
                name = "Unknown (%u)" % k

            b = Blender.Draw.Number(name,self.EVENT_GEOSETCHANGESTART+k,10,starty,250,button_height,active,1,num_choices)
            starty -= button_height
            self.geoset_buttons[k] = b

        starty -= button_height
        self.freeze_button = Blender.Draw.PushButton("Freeze",self.EVENT_FREEZE,10,starty,80,button_height,"Remove unused parts")
        
        starty -= button_height
        glRasterPos2i(10,starty)
        Blender.Draw.Text("NOTE: Activate a 3D-view to see the changes to the model")

    def ButtonEvent(self,event):
        if event >= self.EVENT_GEOSETCHANGESTART and event < self.EVENT_GEOSETCHANGEEND:
            geoset = event - self.EVENT_GEOSETCHANGESTART
#            print "Activate geoset:",geoset,self.geoset_buttons[geoset].val
            self.geoset_active[geoset] = self.geoset_buttons[geoset].val
            ShowGeoset(geoset,self.geoset_buttons[geoset].val)
            Blender.Redraw()
        
        elif event == self.EVENT_FREEZE:
            RemoveHiddenGeosets()

    def LoadGeosets(self):
        # Only load them once
        if self.geosets != None:
            return

        global theFile
        self.geosets = GetGeosets(theFile)
        keys = self.geosets.keys()
        for g in keys:
            ShowGeoset(g,1)
            self.geoset_active[g] = 1


class MenuMain:
    EVENT_GEOSET_MENU = EVENT_UNUSED + 1
    EVENT_ANIM_MENU = EVENT_UNUSED + 2

    def Draw(self):
        Blender.Draw.BeginAlign()
        starty = 40 + 5 * button_height
        
        self.config_button = Blender.Draw.PushButton("Configure model",self.EVENT_GEOSET_MENU,10,starty,150,button_height,"Go to the mesh configuration menu")
        starty -= button_height
        
        self.anim_button = Blender.Draw.PushButton("Animations",self.EVENT_ANIM_MENU,10,starty,150,button_height,"Go to the animation import menu")
        Blender.Draw.EndAlign()

    def ButtonEvent(self,event):
        global active_menu,menu_geoset,menu_anim
        if event == self.EVENT_GEOSET_MENU:
            active_menu = menu_geoset
            Blender.Draw.Redraw(1)
        elif event == self.EVENT_ANIM_MENU:
            active_menu = menu_anim
            Blender.Draw.Redraw(1)

class MenuAnim:
    EVENT_LOAD_ARM = EVENT_UNUSED + 1
    def Draw(self):
        Blender.Draw.BeginAlign()
        starty = 40 + 5 * button_height

        self.arm_button = Blender.Draw.PushButton("Load armature",self.EVENT_LOAD_ARM,10,starty,150,button_height,"Loads the skeleton from the file")
        starty -= button_height

        Blender.Draw.EndAlign()

    def ButtonEvent(self,event):
        if event == self.EVENT_LOAD_ARM:
            LoadArmature()
            Blender.Redraw()
            

menu_load = MenuLoad()
menu_main = MenuMain()
menu_geoset = MenuGeoset()
menu_anim = MenuAnim()

active_menu = menu_load
exit_button = None
back_button = None

def gui():
    global theFile
    global load_button,exit_button,name_button
    global filename_field,selectfile_button
    r,g,b,a = color_back
    glClearColor(r,g,b,a)
    glClear(GL_COLOR_BUFFER_BIT)
    glColor3f(0,0,0)

    global active_menu
    active_menu.Draw()
   
    if active_menu == menu_main or active_menu == menu_load:
        exit_button = Blender.Draw.PushButton("Done",EVENT_EXIT,10,10,80,button_height,"Closes import plugin")
    else:
        back_button = Blender.Draw.PushButton("Back",EVENT_BACK,10,10,80,button_height,"Return to main menu")

def button_event(event):
    global active_menu,menu_main
    if event == EVENT_EXIT:
        Blender.Draw.Exit()
        Blender.Draw.Redraw(1)
    elif event == EVENT_BACK:
        active_menu = menu_main
        Blender.Draw.Redraw(1)
    else:
        active_menu.ButtonEvent(event)

Blender.Draw.Register(gui,None,button_event)
