#!BPY

"""
Name: 'M2I (.m2i)...'
Blender: 248
Group: 'Export'
Tooltip: 'Save a M2I file.'
"""

# v4.0
# revision 2010.10.31

import Blender
import BinaryIO
import math
import struct

try:		import os
except:		os= False


def MakeFourCC( Value ):
	return struct.unpack( "<I", Value )[0]


Signature_M2I0 = MakeFourCC( "M2I0" )


# natural order sort functions
def try_int(s):
    "Convert to integer if possible."
    try: return int(s)
    except: return s

def natsort_key(s):
    "Used internally to get a tuple by which s is sorted."
    import re
    return map(try_int, re.findall(r'(\d+|\D+)', s))

def natcmp(a, b):
    "Natural string comparison, case sensitive."
    return cmp(natsort_key(a), natsort_key(b))

def natcasecmp(a, b):
    "Natural string comparison, ignores case."
    return natcmp(a.lower(), b.lower())

def natsort(seq, cmp=natcmp):
    "In-place natural string sort."
    seq.sort(cmp)

def natsorted(seq, cmp=natcmp):
    "Returns a copy of seq, sorted by natural string sort."
    import copy
    temp = copy.copy(seq)
    natsort(temp, cmp)
    return temp

def SortBlendWeights(A, B):
	if A[1] < B[1]:
		return 1
	elif A[1] == B[1]:
		return 0
	else:
		return -1

class CVertex:
	def __init__( self ):
		self.Index = -1
		self.Position = [0.0, 0.0, 0.0]
		self.Normal = [0.0, 0.0, 0.0]
		self.Texture = [0.0, 0.0]
		self.Bones = []	# each entry is a 3 element list, [0] is bone index, [1] is normalized float weight, [2] is normalized int weight in range 0-255
	
	def Compare( self, other ):
		return ( self.Position == other.Position ) and ( self.Normal == other.Normal ) and ( self.Texture == other.Texture ) and ( self.Bones == other.Bones )


class CSubMesh:
	def __init__( self, VertexStart ):
		self.VertexStart = VertexStart		# index offset of where this sub meshes' vertices begin in the global vertex list
		self.Triangles = []					# stores triangles in the order they are added
		self.Vertices = []					# stores vertices in the order they are added
		self.VerticesDict = {}				# stores vertices by their x position for quick lookup
	
	def AddTriangle( self, VertexA, VertexB, VertexC ):
		FinalVertexA = self.AddVertex( VertexA )
		FinalVertexB = self.AddVertex( VertexB )
		FinalVertexC = self.AddVertex( VertexC )
		self.Triangles.append( [FinalVertexA.Index, FinalVertexB.Index, FinalVertexC.Index] )
	
	def AddVertex( self, Vertex ):
		# find bucket
		if not self.VerticesDict.has_key( Vertex.Position[0] ):
			self.VerticesDict[Vertex.Position[0]] = []
		Bucket = self.VerticesDict[Vertex.Position[0]]
		
		# find existing vertex
		for i in range( 0, len( Bucket ) ):
			if Bucket[i].Compare( Vertex ):
				return Bucket[i]
		
		# add new vertex
		Vertex.Index = len( self.Vertices )
		self.Vertices.append( Vertex )
		self.VerticesDict[Vertex.Position[0]].append( Vertex )
		return Vertex


# because blender objects don't store their children...
def GetChildren( BlenderScene, BlenderObject ):
	Result = []
	for Object in BlenderScene.objects:
		Parent = Object.getParent()
		if Parent != None:
			if Parent.name == BlenderObject.name:
				Result.append( Object )
	return Result

def FindVertexTexture( InVertex, InFaces ):
	for Face in InFaces:
		iFaceVert = 0
		for Vertex in Face.verts:
			if Vertex.index == InVertex.index:
				return [Face.uv[iFaceVert].x, Face.uv[iFaceVert].y]
			iFaceVert += 1
	return [0.0, 0.0]

def Save( FileName ):
	'''
	Exports M2I
	'''
	
	# print status
	print( "\nexporting: \"" + FileName + "\"" )
	
	# start timer
	TimeStart = Blender.sys.time()
	
	# get scene
	BlenderScene = Blender.Scene.GetCurrent()
	
	# get selection
	if len( BlenderScene.objects.selected ) == 0:
		print( "export failed: there was no object selected" )
		return
	
	# get root object of model
	Root = BlenderScene.objects.selected[0]
	while Root.getParent() != None:
		Root = Root.getParent()
	
	# make sure root object is an armature
	if Root.getType() != "Armature":
		print( "export failed: root object of selection was not an armture." )
		return
	
	# get root scale, this scale is applied to all exported positional coordinates
	Scale = Root.getSize("worldspace")
	
	# gather up subsets and attachments
	RootChildren = GetChildren( BlenderScene, Root )
	OutSubsets = []
	OutAttachments = []
	OutCameraTargets = []
	for Child in RootChildren:
		if Child.getType() == "Mesh":
			if Child.name.startswith( "Subset" ) or Child.name.startswith( "Submesh" ):
				OutSubsets.append( Child )
		elif Child.getType() == "Empty":
			if Child.name.startswith( "Attachment" ):
				OutAttachments.append( Child )
			elif Child.name.startswith( "CameraTarget" ):
				OutCameraTargets.append( Child )
	
	OutCameras = []
	for Target in OutCameraTargets:
		Children = GetChildren( BlenderScene, Target )
		for Child in Children:
			if Child.getType() == "Camera":
				if Child.name.startswith( "Camera" ):
					OutCameras.append( Child )
	
	
	# open file
	file = open( FileName, 'wb' )
	endianness = BinaryIO.EEndianness_Little
	
	
	# save signature
	BinaryIO.WriteUInt32( file, endianness, Signature_M2I0 )
	
	# save version
	BinaryIO.WriteUInt16( file, endianness, 4 )
	BinaryIO.WriteUInt16( file, endianness, 0 )
	
	# save subsets
	SubMeshVertexStart = 0
	BinaryIO.WriteUInt32( file, endianness, len( OutSubsets ) )
	for Subset in OutSubsets:
		SubsetIDString = ""
		iStart = 0;
		if Subset.name.startswith( "Subset" ):
			iStart = 6;
		if Subset.name.startswith( "Submesh" ):
			iStart = 7;
		for i in range( iStart, len( Subset.name ) ):
			if Subset.name[i].isdigit():
				SubsetIDString += Subset.name[i]
			else:
				break
		
		# save ID
		SubsetID = int( SubsetIDString )
		BinaryIO.WriteUInt32( file, endianness, SubsetID )
		
		# save materials
		SubsetProperties = Subset.getAllProperties()
		MaterialCount = 0;
		for iMaterial in range( 0, 8 ):
			Found = 0
			for SubsetProperty in SubsetProperties:
				if SubsetProperty.getName() == "M2_" + str( MaterialCount ) + "_Flags1":
					MaterialCount += 1
					continue
			if Found == 0:
				break
		
		if MaterialCount == 0:
			print( "invalidly formed properties" )
			return
		
		BinaryIO.WriteUInt32( file, endianness, MaterialCount )
		for iMaterial in range( 0, MaterialCount ):
			iMaterialString = str( iMaterial )
			BinaryIO.WriteUInt16( file, endianness, Subset.getProperty( "M2_" + iMaterialString + "_Flags1" ).getData() )
			BinaryIO.WriteUInt16( file, endianness, Subset.getProperty( "M2_" + iMaterialString + "_Flags2" ).getData() )
			BinaryIO.WriteSInt16( file, endianness, Subset.getProperty( "M2_" + iMaterialString + "_iColor" ).getData() )
			BinaryIO.WriteUInt16( file, endianness, Subset.getProperty( "M2_" + iMaterialString + "_iRenderFlags" ).getData() )
			BinaryIO.WriteUInt16( file, endianness, Subset.getProperty( "M2_" + iMaterialString + "_iUnit" ).getData() )
			BinaryIO.WriteUInt16( file, endianness, Subset.getProperty( "M2_" + iMaterialString + "_iShader" ).getData() )
			BinaryIO.WriteUInt16( file, endianness, Subset.getProperty( "M2_" + iMaterialString + "_iTexture" ).getData() )
			BinaryIO.WriteUInt16( file, endianness, Subset.getProperty( "M2_" + iMaterialString + "_iTextureUnit" ).getData() )
			BinaryIO.WriteUInt16( file, endianness, Subset.getProperty( "M2_" + iMaterialString + "_iTransparency" ).getData() )
			BinaryIO.WriteUInt16( file, endianness, Subset.getProperty( "M2_" + iMaterialString + "_iTextureAnimation" ).getData() )
		
		# build vertex list for export
		OutTriangles = []
		OutVerticesDict = {}
		OutVertices = []
		
		BMesh = Subset.getData( False, True )
		
		# build sub mesh
		# make a new sub mesh builder
		SubMesh = CSubMesh( SubMeshVertexStart )
		# build faces
		for iFace in range( 0, len( BMesh.faces ) ):
			BFace = BMesh.faces[iFace]
			
			# build face vertices
			FaceVerts = []
			for iFaceVert in range( 0, len( BFace.verts ) ):
				BFaceVert = BFace.verts[iFaceVert]
				NewVertex = CVertex()
				
				# position
				NewVertex.Position = [BFaceVert.co.x, BFaceVert.co.y, BFaceVert.co.z]
				# normal
				NewVertex.Normal = [BFaceVert.no.x, BFaceVert.no.y, BFaceVert.no.z]
				# texture
				NewVertex.Texture = [BFace.uv[iFaceVert].x, BFace.uv[iFaceVert].y]
				# blend weights
				BVertexInfluences = BMesh.getVertexInfluences( BFaceVert.index )
				for BVertexInfluence in BVertexInfluences:
					if ( BVertexInfluence[1] > 0.0 ):
						BoneIndex = int( BVertexInfluence[0][4:] )
						BoneWeight = BVertexInfluence[1]	# use float for now, we will convert to int later during normalization
						NewVertex.Bones.append( [BoneIndex, BoneWeight] )
				NewVertex.Bones.sort( SortBlendWeights )
				NewVertex.Bones = NewVertex.Bones[:4]
				# normalize blend weights and convert to ints
				TotalWeight = 0.0
				for iBone in range( 0, len( NewVertex.Bones ) ):
					TotalWeight += NewVertex.Bones[iBone][1]
				if TotalWeight > 0:
					for iBone in range( 0, len( NewVertex.Bones ) ):
						NewVertex.Bones[iBone][1] = int( NewVertex.Bones[iBone][1] / TotalWeight * 255.0 )
				# add dead weights so length equals 4
				while len( NewVertex.Bones ) < 4:
					NewVertex.Bones.append( [0, 0] )
				
				FaceVerts.append( NewVertex )
			
			# add face(s) to sub mesh builder
			if len( FaceVerts ) == 3:
				# triangle
				SubMesh.AddTriangle( FaceVerts[0], FaceVerts[1], FaceVerts[2] )
			elif len( FaceVerts ) == 4:
				# quad, triangulate it
				SubMesh.AddTriangle( FaceVerts[0], FaceVerts[1], FaceVerts[2] )
				SubMesh.AddTriangle( FaceVerts[2], FaceVerts[3], FaceVerts[0] )
		
		# advance sub mesh vertex start for next sub mesh
		SubMeshVertexStart += len( SubMesh.Vertices )
		
		
		# save vertices
		BinaryIO.WriteUInt32( file, endianness, len( SubMesh.Vertices ) )
		for OutVertex in SubMesh.Vertices:
			X = OutVertex.Position[0]
			Y = OutVertex.Position[1]
			Z = OutVertex.Position[2]
			BinaryIO.WriteFloat32( file, endianness, Y )
			BinaryIO.WriteFloat32( file, endianness, -X )
			BinaryIO.WriteFloat32( file, endianness, Z )
			
			for iBone in range( 0, 4 ):
				if iBone < len( OutVertex.Bones ):
					BinaryIO.WriteUInt8( file, endianness, OutVertex.Bones[iBone][1] )
				else:
					BinaryIO.WriteUInt8( file, endianness, 0 )
			
			for iBone in range( 0, 4 ):
				if iBone < len( OutVertex.Bones ):
					BinaryIO.WriteUInt8( file, endianness, OutVertex.Bones[iBone][0] )
				else:
					BinaryIO.WriteUInt8( file, endianness, 0 )
			
			X = OutVertex.Normal[0]
			Y = OutVertex.Normal[1]
			Z = OutVertex.Normal[2]
			BinaryIO.WriteFloat32( file, endianness, Y )
			BinaryIO.WriteFloat32( file, endianness, -X )
			BinaryIO.WriteFloat32( file, endianness, Z )
			
			BinaryIO.WriteFloat32( file, endianness, OutVertex.Texture[0] )
			BinaryIO.WriteFloat32( file, endianness, 1.0 - OutVertex.Texture[1] )
		
		
		# save triangles
		BinaryIO.WriteUInt32( file, endianness, len( SubMesh.Triangles ) )
		for OutTriangle in SubMesh.Triangles:
			BinaryIO.WriteUInt16( file, endianness, OutTriangle[0] )
			BinaryIO.WriteUInt16( file, endianness, OutTriangle[1] )
			BinaryIO.WriteUInt16( file, endianness, OutTriangle[2] )
		
		
		# save unknowns
		BinaryIO.WriteUInt16( file, endianness, Subset.getProperty( "M2_Unknown1" ).getData() )
		BinaryIO.WriteUInt16( file, endianness, Subset.getProperty( "M2_Unknown2" ).getData() )
	
	
	# save bones
	Armature = Root.getData()
	BoneNames = Armature.bones.keys()
	natsort( BoneNames )
	BinaryIO.WriteUInt32( file, endianness, len( BoneNames ) )
	for BoneName in BoneNames:
		Bone = Armature.bones[BoneName]
		
		BoneID = ""
		for i in range( 4, len( Bone.name ) ):
			if Bone.name[i].isdigit():
				BoneID += Bone.name[i]
			else:
				break
		BoneID = int( BoneID )
		
		BinaryIO.WriteUInt16( file, endianness, BoneID )
		
		if Bone.hasParent():
			BinaryIO.WriteUInt16( file, endianness, int( Bone.parent.name[4:] ) )
		else:
			BinaryIO.WriteUInt16( file, endianness, 0xFFFF )
		
		X = Bone.head['ARMATURESPACE'].x * Scale[0]
		Y = Bone.head['ARMATURESPACE'].y * Scale[1]
		Z = Bone.head['ARMATURESPACE'].z * Scale[2]
		BinaryIO.WriteFloat32( file, endianness, Y )
		BinaryIO.WriteFloat32( file, endianness, -X )
		BinaryIO.WriteFloat32( file, endianness, Z )
	
	
	# save attachments
	BinaryIO.WriteUInt32( file, endianness, len( OutAttachments ) )
	for OutAttachment in OutAttachments:
		AttachmentID = ""
		for i in range( 10, len( OutAttachment.name ) ):
			if OutAttachment.name[i].isdigit():
				AttachmentID += OutAttachment.name[i]
			else:
				break
		AttachmentID = int( AttachmentID )
		
		BinaryIO.WriteUInt32( file, endianness, AttachmentID )
		
		ParentBone = 0xFFFF
		ParentBoneName = OutAttachment.getParentBoneName()
		if ParentBoneName != None:
			ParentBone = int( ParentBoneName[4:] )
		BinaryIO.WriteUInt16( file, endianness, ParentBone )
		
		Location = OutAttachment.getLocation()
		X = Location[0] * Scale[0]
		Y = Location[1] * Scale[1]
		Z = Location[2] * Scale[2]
		BinaryIO.WriteFloat32( file, endianness, Y )
		BinaryIO.WriteFloat32( file, endianness, -X )
		BinaryIO.WriteFloat32( file, endianness, Z )
	
	
	# save cameras
	BinaryIO.WriteUInt32( file, endianness, len( OutCameras ) )
	for OutCamera in OutCameras:
		Signed = False
		CameraID = ""
		for i in range( 6, len( OutCamera.name ) ):
			if OutCamera.name[i] == "-":
				if Signed:
					break
				CameraID += OutCamera.name[i]
				Signed = True
			elif OutCamera.name[i].isdigit():
				CameraID += OutCamera.name[i]
			else:
				break
		CameraID = int( CameraID )
		
		BinaryIO.WriteUInt32( file, endianness, CameraID )
		
		Camera = OutCamera.getData()
		
		BinaryIO.WriteFloat32( file, endianness, math.radians( Camera.angle ) )
		BinaryIO.WriteFloat32( file, endianness, Camera.clipEnd )
		BinaryIO.WriteFloat32( file, endianness, Camera.clipStart )
		
		Location = OutCamera.getLocation()
		X = Location[0] * Scale[0]
		Y = Location[1] * Scale[1]
		Z = Location[2] * Scale[2]
		BinaryIO.WriteFloat32( file, endianness, Y )
		BinaryIO.WriteFloat32( file, endianness, -X )
		BinaryIO.WriteFloat32( file, endianness, Z )
		
		Target = OutCamera.getTracked()
		Location = Target.getLocation()
		X = Location[0] * Scale[0]
		Y = Location[1] * Scale[1]
		Z = Location[2] * Scale[2]
		BinaryIO.WriteFloat32( file, endianness, Y )
		BinaryIO.WriteFloat32( file, endianness, -X )
		BinaryIO.WriteFloat32( file, endianness, Z )
	
	file.close()
	
	print( "export time: " + str( Blender.sys.time() - TimeStart ) )


def SaveUI( FileName ):
	Blender.Window.WaitCursor(1)
	
	if not FileName.lower().endswith('.m2i'):
		FileName += '.m2i'
	
	Save( FileName )
	
	Blender.Window.WaitCursor(0)

if __name__ == '__main__':
	Blender.Window.FileSelector( SaveUI, 'Export M2I', '*.m2i' )
