bl_info = {
	"name": "Export: M2 Intermediate (.m2i)",
	"author": "",
	"version": (1, 0, 0),
	"blender": (2, 5, 7),
	"api": 36302,
	"location": "File > Export > M2 Intermediate (.m2i)",
	"description": "Export files in the M2 Intermediate format (.m2i)",
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
			This.Index = 0
		
		def Compare( A, B ):
			return ( A.Position == B.Position ) and ( A.Normal == B.Normal ) and ( A.Texture == B.Texture )
	
	class CTriangle:
		def __init__( This ):
			This.A = 0
			This.B = 0
			This.C = 0
	
	def __init__( This ):
		This.ID = 0
		This.VertexList = []
		This.TriangleList = []
		This.VertexDict = {}
	
	def AddTriangle( This, VertexA, VertexB, VertexC ):
		Triangle = CMesh.CTriangle()
		Triangle.A = This.AddVertex( VertexA )
		Triangle.B = This.AddVertex( VertexB )
		Triangle.C = This.AddVertex( VertexC )
		This.TriangleList.append( Triangle )
	
	def AddVertex( This, Vertex ):
		# find existing vertex
		if not Vertex.Position[0] in This.VertexDict:
			This.VertexDict[Vertex.Position[0]] = []
		Bucket = This.VertexDict[Vertex.Position[0]]
		for i in range( 0, len( Bucket ) ):
			if Bucket[i].Compare( Vertex ):
				return Bucket[i].Index
		# add new vertex
		Vertex.Index = len( This.VertexList )
		This.VertexList.append( Vertex )
		This.VertexDict[Vertex.Position[0]].append( Vertex )
		return Vertex.Index

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

# sorts a list of vertex influences from heaviest to lightest
def VertexInfluenceSortKey( A ):
	return -A[1]

def DoExport( FileName ):
	MeshList = []
	BoneList = []
	AttachmentList = []
	CameraList = []
	
	# find root object
	if len( bpy.context.selected_objects ) == 0:
		print( "no selection." )
		return
	BArmature = bpy.context.selected_objects[0]
	while BArmature.parent != None:
		BArmature = BArmature.parent
	if BArmature.type != 'ARMATURE':
		print( "root is not armature." )
		return
	
	# gather Blender objects
	BMeshList = []
	BEmptyList = []
	BCameraList = []
	for BObject in BArmature.children:
		if BObject.type == 'MESH' and BObject.name.startswith( "SubMesh" ):
			BMeshList.append( BObject )
		elif BObject.type == 'EMPTY' and BObject.name.startswith( "Attachment" ):
			BEmptyList.append( BObject )
		elif BObject.type == 'CAMERA' and BObject.name.startswith( "Camera" ):
			BCameraList.append( BObject )
	
	# extract meshes
	for BMesh in BMeshList:
		ActiveUV = BMesh.data.uv_textures.active
		Mesh = CMesh()
		Mesh.ID = int(BMesh.name[7:].split('.')[0])
		for iFace, BFace in enumerate( BMesh.data.faces ):
			FaceUVs = []
			if ActiveUV != None:
				for UV in ActiveUV.data[iFace].uv:
					FaceUVs.append( [UV[0], UV[1]] )
			# build vertex list for this face
			FaceVertexList = []
			for iFaceVertex, VertexIndex in enumerate( BFace.vertices ):
				BVertex = BMesh.data.vertices[VertexIndex]
				Vertex = CMesh.CVertex()
				# position
				Vertex.Position[0] = BVertex.co.y
				Vertex.Position[1] = -BVertex.co.x
				Vertex.Position[2] = BVertex.co.z
				# normal
				if BFace.use_smooth:
					Vertex.Normal[0] = BVertex.normal.y
					Vertex.Normal[1] = -BVertex.normal.x
					Vertex.Normal[2] = BVertex.normal.z
				else:
					Vertex.Normal[0] = BFace.normal.y
					Vertex.Normal[1] = -BFace.normal.x
					Vertex.Normal[2] = BFace.normal.z
				# texture
				Vertex.Texture[0] = FaceUVs[iFaceVertex][0]
				Vertex.Texture[1] = 1.0-FaceUVs[iFaceVertex][1]
				#
				VertexInfluences = []
				for BVertexGroupElement in BVertex.groups:
					if BVertexGroupElement.weight > 0.0:
						VertexInfluences.append( [BMesh.vertex_groups[BVertexGroupElement.group].name, BVertexGroupElement.weight] )
				VertexInfluences.sort( key=VertexInfluenceSortKey )
				VertexInfluences = VertexInfluences[:4]
				WeightSum = 0.0
				for VertexInfluence in VertexInfluences:
					WeightSum += VertexInfluence[1]
				if WeightSum > 0.0:
					for iBone, VertexInfluence in enumerate( VertexInfluences ):
						Vertex.BoneWeights[iBone] = int( VertexInfluence[1] / WeightSum * 255.0 )
						Vertex.BoneIndices[iBone] = int( VertexInfluence[0][4:].split('.')[0] )
				WeightSum = Vertex.BoneWeights[0] + Vertex.BoneWeights[1] + Vertex.BoneWeights[2] + Vertex.BoneWeights[3]
				if WeightSum > 0:
					while WeightSum < 255:
						for iBone in range( 0, 4 ):
							if Vertex.BoneWeights[iBone] > 0 and Vertex.BoneWeights[iBone] < 255:
								Vertex.BoneWeights[iBone] += 1
								WeightSum += 1
								break
				FaceVertexList.append( Vertex )
			# add triangle to mesh
			if len( FaceVertexList ) == 3:
				Mesh.AddTriangle( FaceVertexList[0], FaceVertexList[1], FaceVertexList[2] )
			elif len( FaceVertexList ) == 4:
				Mesh.AddTriangle( FaceVertexList[0], FaceVertexList[1], FaceVertexList[2] )
				Mesh.AddTriangle( FaceVertexList[2], FaceVertexList[3], FaceVertexList[0] )
		MeshList.append( Mesh )
	
	# extract bones
	BoneMap = {}
	bpy.ops.object.select_all( action='DESELECT' )
	#bpy.ops.object.select_name( name=BArmature.name, extend=False )
	BArmature.select = True
	bpy.context.scene.objects.active = BArmature
	bpy.ops.object.mode_set( mode='EDIT', toggle=False )
	for BBone in BArmature.data.edit_bones:
		Bone = CBone()
		if not BBone.name.startswith( "Bone" ):
			print( "bone \"" + BBone.name + "\" is not named properly. proper convention is \"Bone[index]\". model may have been corrupted by user error." )
			return
		if BBone.parent != None:
			if not BBone.parent.name.startswith( "Bone" ):
				print( "bone \"" + BBone.parent.name + "\" is not named properly. proper convention is \"Bone[index]\". model may have been corrupted by user error." )
				return
		Bone.Index = int(BBone.name[4:].split('.')[0])
		if BBone.parent != None:
			Bone.Parent = int(BBone.parent.name[4:].split('.')[0])
		Bone.Position[0] = BBone.head.y
		Bone.Position[1] = -BBone.head.x
		Bone.Position[2] = BBone.head.z
		BoneList.append( Bone )
		BoneMap[BBone.name] = BBone
	bpy.ops.object.mode_set( mode='OBJECT', toggle=False )
	
	# extract attachments
	for BEmpty in BEmptyList:
		Attachment = CAttachment()
		Attachment.ID = int(BEmpty.name[10:].split('.')[0])
		Attachment.Parent = int(BEmpty.parent_bone[4:].split('.')[0])
		BBone = BArmature.data.bones[BEmpty.parent_bone]
		Attachment.Position[0] = BEmpty.location.y + BBone.head_local[1] + 0.1
		Attachment.Position[1] = -BEmpty.location.x - BBone.head_local[0]
		Attachment.Position[2] = BEmpty.location.z + BBone.head_local[2]
		Attachment.Scale = 1.0
		AttachmentList.append( Attachment )
	
	# extract cameras
	
	
	
	# open stream
	File = open( FileName, "wb" )
	DataBinary = CDataBinary( File, EEndianness.Little )
	
	# save header
	DataBinary.WriteUInt32( MakeFourCC( b"M2I0" ) )
	DataBinary.WriteUInt16( 4 )
	DataBinary.WriteUInt16( 5 )
	
	# save mesh list
	DataBinary.WriteUInt32( len( MeshList ) )
	for Mesh in MeshList:
		DataBinary.WriteUInt32( Mesh.ID )
		DataBinary.WriteUInt32( len( Mesh.VertexList ) )
		for Vertex in Mesh.VertexList:
			DataBinary.WriteFloat32( Vertex.Position[0] )
			DataBinary.WriteFloat32( Vertex.Position[1] )
			DataBinary.WriteFloat32( Vertex.Position[2] )
			DataBinary.WriteUInt8( Vertex.BoneWeights[0] )
			DataBinary.WriteUInt8( Vertex.BoneWeights[1] )
			DataBinary.WriteUInt8( Vertex.BoneWeights[2] )
			DataBinary.WriteUInt8( Vertex.BoneWeights[3] )
			DataBinary.WriteUInt8( Vertex.BoneIndices[0] )
			DataBinary.WriteUInt8( Vertex.BoneIndices[1] )
			DataBinary.WriteUInt8( Vertex.BoneIndices[2] )
			DataBinary.WriteUInt8( Vertex.BoneIndices[3] )
			DataBinary.WriteFloat32( Vertex.Normal[0] )
			DataBinary.WriteFloat32( Vertex.Normal[1] )
			DataBinary.WriteFloat32( Vertex.Normal[2] )
			DataBinary.WriteFloat32( Vertex.Texture[0] )
			DataBinary.WriteFloat32( Vertex.Texture[1] )
		DataBinary.WriteUInt32( len( Mesh.TriangleList ) )
		for Triangle in Mesh.TriangleList:
			DataBinary.WriteUInt16( Triangle.A )
			DataBinary.WriteUInt16( Triangle.B )
			DataBinary.WriteUInt16( Triangle.C )
	
	# save bone list
	DataBinary.WriteUInt32( len( BoneList ) )
	for Bone in BoneList:
		DataBinary.WriteUInt16( Bone.Index )
		DataBinary.WriteSInt16( Bone.Parent )
		DataBinary.WriteFloat32( Bone.Position[0] )
		DataBinary.WriteFloat32( Bone.Position[1] )
		DataBinary.WriteFloat32( Bone.Position[2] )
	
	# save attachment list
	DataBinary.WriteUInt32( len( AttachmentList ) )
	for Attachment in AttachmentList:
		DataBinary.WriteUInt32( Attachment.ID )
		DataBinary.WriteSInt16( Attachment.Parent )
		DataBinary.WriteFloat32( Attachment.Position[0] )
		DataBinary.WriteFloat32( Attachment.Position[1] )
		DataBinary.WriteFloat32( Attachment.Position[2] )
		DataBinary.WriteFloat32( Attachment.Scale )
		
	# save camera list
	DataBinary.WriteUInt32( len( CameraList ) )
	for Camera in CameraList:
		DataBinary.WriteSInt32( Camera.Type )
		DataBinary.WriteFloat32( Camera.FieldOfView )
		DataBinary.WriteFloat32( Camera.ClipFar )
		DataBinary.WriteFloat32( Camera.ClipNear )
		DataBinary.WriteFloat32( Camera.Position[0] )
		DataBinary.WriteFloat32( Camera.Position[1] )
		DataBinary.WriteFloat32( Camera.Position[2] )
		DataBinary.WriteFloat32( Camera.Target[0] )
		DataBinary.WriteFloat32( Camera.Target[1] )
		DataBinary.WriteFloat32( Camera.Target[2] )
	
	# close stream
	File.close()
	
	print( "M2I exported successfully: " + FileName )

class M2IExporter(bpy.types.Operator):
	'''Export a M2 Intermediate file'''
	bl_idname = "export.m2i"
	bl_label = "Export M2I"
	
	filepath = bpy.props.StringProperty(name="File Path", description="Filepath used for exporting the M2I file", maxlen= 1024, default= "")
	check_existing = bpy.props.BoolProperty(name="Check Existing", description="Check and warn on overwriting existing files", default=True, options={'HIDDEN'})
	
	def execute( self, context ):
		FilePath = self.properties.filepath
		if not FilePath.lower().endswith(".m2i"):
			FilePath += ".m2i"
		DoExport( FilePath )
		return {'FINISHED'}
	
	def invoke(self, context, event):
		WindowManager = context.window_manager
		WindowManager.fileselect_add( self )
		return {'RUNNING_MODAL'}


def menu_func(self, context):
	default_path = os.path.splitext(bpy.data.filepath)[0] + ".m2i"
	self.layout.operator(M2IExporter.bl_idname, text="M2 Intermediate (.m2i)").filepath = default_path


def register():
	bpy.utils.register_module(__name__)
	bpy.types.INFO_MT_file_export.prepend(menu_func)


def unregister():
	bpy.utils.unregister_module(__name__)
	bpy.types.INFO_MT_file_export.remove(menu_func)


if __name__ == "__main__":
	register()
