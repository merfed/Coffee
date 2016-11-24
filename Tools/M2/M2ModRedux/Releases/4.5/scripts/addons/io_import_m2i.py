bl_info = {
	"name": "Import: M2 Intermediate (.m2i)",
	"author": "",
	"version": (1, 0, 0),
	"blender": (2, 5, 7),
	"api": 36302,
	"location": "File > Import > M2 Intermediate (.m2i)",
	"description": "Import files in the M2 Intermediate format (.m2i)",
	"warning": "",
	"category": "Import-Export"}

import bpy
import os
import struct

def MakeFourCC( Value ):
	return struct.unpack( "<I", Value )[0]

class EEndianness:
	Native = "@"
	Little = "<"
	Big = ">"

class CDataBinary:
	def __init__( This, File, Endianness ):
		This.File = File
		This.Endianness = Endianness
	def ReadUInt8( This ):
		return struct.unpack( This.Endianness + "B", This.File.read( 1 ) )[0]
	def ReadSInt8( This ):
		return struct.unpack( This.Endianness + "b", This.File.read( 1 ) )[0]
	def ReadUInt16( This ):
		return struct.unpack( This.Endianness + "H", This.File.read( 2 ) )[0]
	def ReadSInt16( This ):
		return struct.unpack( This.Endianness + "h", This.File.read( 2 ) )[0]
	def ReadUInt32( This ):
		return struct.unpack( This.Endianness + "I", This.File.read( 4 ) )[0]
	def ReadSInt32( This ):
		return struct.unpack( This.Endianness + "i", This.File.read( 4 ) )[0]
	def ReadFloat32( This ):
		return struct.unpack( This.Endianness + "f", This.File.read( 4 ) )[0]
	def WriteUInt8( This, Value ):
		This.File.write( struct.pack( This.Endianness + "B", Value ) )
	def WriteSInt8( This, Value ):
		This.File.write( struct.pack( This.Endianness + "B", Value ) )
	def WriteUInt16( This, Value ):
		This.File.write( struct.pack( This.Endianness + "H", Value ) )
	def WriteSInt16( This, Value ):
		This.File.write( struct.pack( This.Endianness + "h", Value ) )
	def WriteUInt32( This, Value ):
		This.File.write( struct.pack( This.Endianness + "I", Value ) )
	def WriteSInt32( This, Value ):
		This.File.write( struct.pack( This.Endianness + "i", Value ) )
	def WriteFloat32( This, Value ):
		This.File.write( struct.pack( This.Endianness + "f", Value ) )

class CMesh:
	class CVertex:
		def __init__( This ):
			This.Position = [0.0, 0.0, 0.0]
			This.BoneWeights = [0, 0, 0, 0]
			This.BoneIndices = [0, 0, 0, 0]
			This.Normal = [0.0, 0.0, 0.0]
			This.Texture = [0.0, 0.0]
	
	class CTriangle:
		def __init__( This ):
			This.A = 0
			This.B = 0
			This.C = 0
	
	def __init__( This ):
		This.ID = 0
		This.VertexList = []
		This.TriangleList = []

class CBone:
	def __init__( This ):
		This.Index = 0
		This.Parent = -1
		This.Position = [0.0, 0.0, 0.0]

class CAttachment:
	def __init__( This ):
		This.ID = 0
		This.Parent = -1
		This.Position = [0.0, 0.0, 0.0]
		This.Scale = 1.0

class CCamera:
	def __init__( This ):
		This.Type = 0
		This.FieldOfView = 0.7
		This.ClipFar = 100.0
		This.ClipNear = 0.5
		This.Position = [0.0, 0.0, 0.0]
		This.Target = [0.0, 0.0, 0.0]

def DoImport( FileName ):
	MeshList = []
	BoneList = []
	AttachmentList = []
	CameraList = []
	
	# open stream
	File = open( FileName, "rb" )
	DataBinary = CDataBinary( File, EEndianness.Little )
	
	# load header
	SignatureIn = DataBinary.ReadUInt32()
	if SignatureIn != MakeFourCC( b"M2I0" ):
		return
	VersionMajor = DataBinary.ReadUInt16()
	VersionMinor = DataBinary.ReadUInt16()
	
	# load mesh list
	MeshCount = DataBinary.ReadUInt32()
	for i in range( 0, MeshCount ):
		Mesh = CMesh()
		Mesh.ID = DataBinary.ReadUInt32()
		VertexCount = DataBinary.ReadUInt32()
		for j in range( 0, VertexCount ):
			Vertex = CMesh.CVertex()
			Vertex.Position[0] = DataBinary.ReadFloat32()
			Vertex.Position[1] = DataBinary.ReadFloat32()
			Vertex.Position[2] = DataBinary.ReadFloat32()
			Vertex.BoneWeights[0] = DataBinary.ReadUInt8()
			Vertex.BoneWeights[1] = DataBinary.ReadUInt8()
			Vertex.BoneWeights[2] = DataBinary.ReadUInt8()
			Vertex.BoneWeights[3] = DataBinary.ReadUInt8()
			Vertex.BoneIndices[0] = DataBinary.ReadUInt8()
			Vertex.BoneIndices[1] = DataBinary.ReadUInt8()
			Vertex.BoneIndices[2] = DataBinary.ReadUInt8()
			Vertex.BoneIndices[3] = DataBinary.ReadUInt8()
			Vertex.Normal[0] = DataBinary.ReadFloat32()
			Vertex.Normal[1] = DataBinary.ReadFloat32()
			Vertex.Normal[2] = DataBinary.ReadFloat32()
			Vertex.Texture[0] = DataBinary.ReadFloat32()
			Vertex.Texture[1] = DataBinary.ReadFloat32()
			Mesh.VertexList.append( Vertex )
		TriangleCount = DataBinary.ReadUInt32()
		for j in range( 0, TriangleCount ):
			Triangle = CMesh.CTriangle()
			Triangle.A = DataBinary.ReadUInt16()
			Triangle.B = DataBinary.ReadUInt16()
			Triangle.C = DataBinary.ReadUInt16()
			Mesh.TriangleList.append( Triangle )
		MeshList.append( Mesh )
	
	# load bone list
	BoneCount = DataBinary.ReadUInt32()
	for i in range( 0, BoneCount ):
		Bone = CBone()
		Bone.Index = DataBinary.ReadUInt16()
		Bone.Parent = DataBinary.ReadSInt16()
		Bone.Position[0] = DataBinary.ReadFloat32()
		Bone.Position[1] = DataBinary.ReadFloat32()
		Bone.Position[2] = DataBinary.ReadFloat32()
		BoneList.append( Bone )
	
	# load attachment list
	AttachmentCount = DataBinary.ReadUInt32()
	for i in range( 0, AttachmentCount ):
		Attachment = CAttachment()
		Attachment.ID = DataBinary.ReadUInt32()
		Attachment.Parent = DataBinary.ReadSInt16()
		Attachment.Position[0] = DataBinary.ReadFloat32()
		Attachment.Position[1] = DataBinary.ReadFloat32()
		Attachment.Position[2] = DataBinary.ReadFloat32()
		Attachment.Scale = DataBinary.ReadFloat32()
		AttachmentList.append( Attachment )
	
	# load camera list
	CameraCount = DataBinary.ReadUInt32()
	for i in range( 0, CameraCount ):
		Camera = CCamera()
		Camera.Type = DataBinary.ReadSInt32()
		Camera.FieldOfView = DataBinary.ReadFloat32()
		Camera.ClipFar = DataBinary.ReadFloat32()
		Camera.ClipNear = DataBinary.ReadFloat32()
		Camera.Position[0] = DataBinary.ReadFloat32()
		Camera.Position[1] = DataBinary.ReadFloat32()
		Camera.Position[2] = DataBinary.ReadFloat32()
		Camera.Target[0] = DataBinary.ReadFloat32()
		Camera.Target[1] = DataBinary.ReadFloat32()
		Camera.Target[2] = DataBinary.ReadFloat32()
		CameraList.append( Camera )
	
	# close stream
	File.close()
	
	#deselect all objects.
	bpy.ops.object.select_all( action='DESELECT' )
	
	# instantiate armature
	bpy.ops.object.add( type='ARMATURE', enter_editmode=True, location=(0.0, 0.0, 0.0) )
	BArmature = bpy.context.object
	for Bone in BoneList: # add bones to armature.
		BEditBone = BArmature.data.edit_bones.new( "Bone"+str(Bone.Index) )
		BEditBone.head.x = -Bone.Position[1]
		BEditBone.head.y = Bone.Position[0]
		BEditBone.head.z = Bone.Position[2]
		BEditBone.tail.x = BEditBone.head.x
		BEditBone.tail.y = BEditBone.head.y + 0.1
		BEditBone.tail.z = BEditBone.head.z
	for Bone in BoneList: # link children to parents
		if Bone.Parent >= 0:
			BEditBone = BArmature.data.edit_bones["Bone"+str(Bone.Index)]
			BEditBone.parent = BArmature.data.edit_bones["Bone"+str(Bone.Parent)]
	bpy.context.scene.update() # update scene.
	bpy.ops.object.mode_set( mode='OBJECT' ) # return to object mode.
	
	# instantiate attachments
	for Attachment in AttachmentList:
		bpy.ops.object.add( type='EMPTY', location=(0.0, 0.0, 0.0) )
		BAttachment = bpy.context.object
		BAttachment.name = "Attachment" + str( Attachment.ID )
		BBone = BArmature.data.bones["Bone"+str(Attachment.Parent)]
		BAttachment.location.x = -Attachment.Position[1] - BBone.head_local[0]
		BAttachment.location.y = Attachment.Position[0] - BBone.head_local[1] - 0.1
		BAttachment.location.z = Attachment.Position[2] - BBone.head_local[2]
		if Attachment.Parent >= 0:
			BAttachment.parent = BArmature
			BAttachment.parent_bone = "Bone"+str(Attachment.Parent)
			BAttachment.parent_type = 'BONE'
			#BAttachment.location.x -= -Attachment.Position[1]
			#BAttachment.location.y -= Attachment.Position[0]
			#BAttachment.location.z -= Attachment.Position[2]
			BAttachment.empty_draw_size = 0.1
	
	# instantiate meshes
	for Mesh in MeshList:
		bpy.ops.object.add( type='MESH', location=(0.0, 0.0, 0.0) )
		BMesh = bpy.context.object
		BMesh.name = "SubMesh" + str( Mesh.ID )
		BMeshData = BMesh.data
		BMeshData.name = BMesh.name
		BMeshData.vertices.add( len( Mesh.VertexList ) ) # add vertices to mesh data.
		for i, Vertex in enumerate( Mesh.VertexList ):
			BVertex = BMeshData.vertices[i]
			BVertex.co.x = -Vertex.Position[1]
			BVertex.co.y = Vertex.Position[0]
			BVertex.co.z = Vertex.Position[2]
			#BVertex.normal.x = -Vertex.Normal[1] # we don't need to import normals because they will be calculated automatically by Blender
			#BVertex.normal.y = Vertex.Normal[0]
			#BVertex.normal.z = Vertex.Normal[2]
		BMeshData.faces.add( len( Mesh.TriangleList ) ) # add triangles to mesh data.
		BMeshTextureFaceLayer = BMeshData.uv_textures.new( name="UVMap" )
		for i, Triangle in enumerate( Mesh.TriangleList ):
			BFace = BMeshData.faces[i]
			BFace.vertices = [Triangle.A, Triangle.B, Triangle.C]	# reverse the wind order so normals point out.
			BMeshTextureFace = BMeshTextureFaceLayer.data[i]
			BMeshTextureFace.uv1[0] = Mesh.VertexList[Triangle.A].Texture[0]
			BMeshTextureFace.uv1[1] = 1.0 - Mesh.VertexList[Triangle.A].Texture[1]
			BMeshTextureFace.uv2[0] = Mesh.VertexList[Triangle.B].Texture[0]
			BMeshTextureFace.uv2[1] = 1.0 - Mesh.VertexList[Triangle.B].Texture[1]
			BMeshTextureFace.uv3[0] = Mesh.VertexList[Triangle.C].Texture[0]
			BMeshTextureFace.uv3[1] = 1.0 - Mesh.VertexList[Triangle.C].Texture[1]
			BFace.use_smooth = True
		for Bone in BoneList:
			BVertexGroup = BMesh.vertex_groups.new( "Bone" + str( Bone.Index ) )
		for i, Vertex in enumerate( Mesh.VertexList ):
			BVertex = BMeshData.vertices[i]
			for j in range( 0, 4 ):
				if Vertex.BoneWeights[j] > 0:
					BVertexGroup = BMesh.vertex_groups["Bone"+str(Vertex.BoneIndices[j])]
					BVertexGroup.add( [i], float(Vertex.BoneWeights[j])/255.0, 'ADD' )
		BMeshData.update()
		BArmatureModifier = BMesh.modifiers.new( "Armature", 'ARMATURE' )
		BArmatureModifier.object = BArmature
		BArmatureModifier.use_bone_envelopes = False
		BArmatureModifier.use_vertex_groups = True
		BMesh.parent = BArmature
		BMesh.select = False
	
	BArmature.select = True
	bpy.context.scene.objects.active = BArmature
	
	print( "M2I imported successfully: " + FileName )


class M2IImporter(bpy.types.Operator):
	'''Import a M2 Intermediate file'''
	bl_idname = "import.m2i"
	bl_label = "Import M2I"
	
	filepath = bpy.props.StringProperty(name="File Path", description="Filepath used for importing the M2I file", maxlen= 1024, default= "")
	check_existing = bpy.props.BoolProperty(name="Check Existing", description="Check and warn on overwriting existing files", default=True, options={'HIDDEN'})
	
	def execute( self, context ):
		FilePath = self.properties.filepath
		if not FilePath.lower().endswith(".m2i"):
			FilePath += ".m2i"
		DoImport( FilePath )
		return {'FINISHED'}
	
	def invoke(self, context, event):
		WindowManager = context.window_manager
		WindowManager.fileselect_add( self )
		return {'RUNNING_MODAL'}


def menu_func(self, context):
	default_path = os.path.splitext(bpy.data.filepath)[0] + ".m2i"
	self.layout.operator(M2IImporter.bl_idname, text="M2 Intermediate (.m2i)").filepath = default_path


def register():
	bpy.utils.register_module(__name__)
	bpy.types.INFO_MT_file_import.prepend(menu_func)


def unregister():
	bpy.utils.unregister_module(__name__)
	bpy.types.INFO_MT_file_import.remove(menu_func)


if __name__ == "__main__":
	register()
