#!BPY
 
"""
Name: 'M2I (.m2i)...'
Blender: 248
Group: 'Import'
Tooltip: 'Load a M2I file.'
"""

#v1

import Blender
import BinaryIO
import math

try:		import os
except:		os= False


def CreateMesh( BlenderScene, InSubmeshID, InPositions, InUVs, InFaces, InVertexGroups ):
	InMeshName = "Submesh" + str(InSubmeshID)
	NewMesh = Blender.Mesh.New( InMeshName )
	
	NewMesh.verts.extend( InPositions )
	NewMesh.faces.extend( InFaces )
	NewMesh.faceUV = 1
	iFace = 0
	for face in NewMesh.faces:
		iVertex = 0
		for vert in face.verts:
			NewMesh.faces[iFace].uv[iVertex].x = InUVs[vert.index][0]
			NewMesh.faces[iFace].uv[iVertex].y = InUVs[vert.index][1]
			iVertex +=1
		iFace+= 1
	
	NewMeshObj = BlenderScene.objects.new( NewMesh )
	
	for Group, Lists in InVertexGroups.iteritems():
		GroupName = "Bone"+str(Group)
		NewMesh.addVertGroup( GroupName )
		for iVert in range( 0, len( Lists[0] ) ):
			NewMesh.assignVertsToGroup( GroupName, [Lists[0][iVert]], Lists[1][iVert], Blender.Mesh.AssignModes.REPLACE )
	
	# # removde doubles and fix normals
	# NewMesh.remDoubles( 0.0005 )
	# NewMesh.recalcNormals()
	
	# # determine if we need to make mesh double sided
	# DoubleSide = False
	# if InSubmeshID == 1302:	# skirt
		# DoubleSide = True
	# #if InSubmeshID == 1502:	# cape 0
	# #	DoubleSide = True
	# #if InSubmeshID == 1503:	# cape 1
	# #	DoubleSide = True
	# #if InSubmeshID == 1504:	# cape 2
	# #	DoubleSide = True
	# #if InSubmeshID == 1505:	# cape 3
	# #	DoubleSide = True
	# #if InSubmeshID == 1506:	# cape 4
	# #	DoubleSide = True
	# if InSubmeshID == 1002:	# shirt 0
		# DoubleSide = True
	# if InSubmeshID == 1102:	# shirt 1
		# DoubleSide = True
	# if InSubmeshID == 1202:	# tabard
		# DoubleSide = True
	# if InSubmeshID == 802:	# sleeve 0
		# DoubleSide = True
	# if InSubmeshID == 803:	# sleeve 1
		# DoubleSide = True
	# if InSubmeshID == 902:	# pant 1
		# DoubleSide = True
	# if InSubmeshID == 903:	# pant 1
		# DoubleSide = True
	
	# if DoubleSide:
		# # duplicate vertices
		# DoubleVerts = []
		# for Vert in NewMesh.verts:
			# DoubleVerts.append( [Vert.co.x, Vert.co.y, Vert.co.z] )
		# DoubleVertsLength = len( DoubleVerts )
		
		# # duplicate faces
		# DoubleFaces = []
		# for Face in NewMesh.faces:
			# DoubleFaces.append( [Face.verts[0].index + DoubleVertsLength, Face.verts[2].index + DoubleVertsLength, Face.verts[1].index + DoubleVertsLength] )
		# DoubleFacesLength = len( DoubleFaces )
		
		# # extend mesh
		# NewMesh.verts.extend( DoubleVerts )
		# NewMesh.faces.extend( DoubleFaces )
		
		# for iVertIn in range( 0, DoubleVertsLength ):
			# # copy reversed normal
			# iVertOut = iVertIn + DoubleVertsLength
			# Nor = NewMesh.verts[iVert].no
			# Nor = Blender.Mathutils.Vector( -Nor.x, -Nor.y, -Nor.z )
			# NewMesh.verts[iVert].no = Nor
			
			# # copy vertex groups
			# Influences = NewMesh.getVertexInfluences( iVertIn )
			# for Influence in Influences:
				# NewMesh.assignVertsToGroup( Influence[0], [iVertOut], Influence[1], Blender.Mesh.AssignModes.REPLACE )
		
		# # set face UV coordinates
		# for iFaceIn in range( 0, DoubleFacesLength ):
			# iFaceOut = iFaceIn + DoubleFacesLength
			# NewMesh.faces[iFaceOut].uv[0].x = NewMesh.faces[iFaceIn].uv[0].x
			# NewMesh.faces[iFaceOut].uv[0].y = NewMesh.faces[iFaceIn].uv[0].y
			# NewMesh.faces[iFaceOut].uv[1].x = NewMesh.faces[iFaceIn].uv[2].x
			# NewMesh.faces[iFaceOut].uv[1].y = NewMesh.faces[iFaceIn].uv[2].y
			# NewMesh.faces[iFaceOut].uv[2].x = NewMesh.faces[iFaceIn].uv[1].x
			# NewMesh.faces[iFaceOut].uv[2].y = NewMesh.faces[iFaceIn].uv[1].y
	
	# set smooth
	for Face in NewMesh.faces:
		Face.smooth = 1
	
	# calc normals
	NewMesh.calcNormals()
	
	return NewMeshObj

def CreateArmature( BlenderScene, InArmatureName, InBones ):
	NewArmature = Blender.Armature.New( InArmatureName )
	NewArmatureObj = BlenderScene.objects.new( NewArmature )
	NewArmatureObj.setName( InArmatureName )
	
	NewArmature.makeEditable()
	
	for Bone in InBones:
		NewEditBone = Blender.Armature.Editbone()
		NewEditBone.head = Blender.Mathutils.Vector( Bone[2], Bone[3], Bone[4] )
		NewEditBone.tail = Blender.Mathutils.Vector( Bone[2], Bone[3] + 0.1, Bone[4] )
		NewArmature.bones["Bone"+str(Bone[0])] = NewEditBone
	
	for Bone in InBones:
		if Bone[1] >= 0:
			NewArmature.bones["Bone"+str(Bone[0])].parent = NewArmature.bones["Bone"+str(Bone[1])]
			#NewArmature.bones["Bone"+str(Bone[1])].tail = NewArmature.bones["Bone"+str(Bone[0])].head
	
	NewArmature.update()
	
	return NewArmatureObj

def CreateAttachments( BlenderScene, InAttachments, InArmature ):
	for Attachment in InAttachments:
		NewEmptyObj = BlenderScene.objects.new( "Empty" )
		NewEmptyObj.setName( "Attachment"+str(Attachment[0]) )
		NewEmptyObj.setSize( 0.1, 0.1, 0.1 )
		NewEmptyObj.setLocation( Attachment[2], Attachment[3], Attachment[4] )
		if Attachment[1] >= 0:
			InArmature.makeParentBone( [NewEmptyObj], "Bone"+str(Attachment[1]) )

def CreateCamera( BlenderScene, InCamera ):
	NewCamera = Blender.Camera.New( "persp", "Camera"+str(InCamera[0]) )
	
	NewCamera.angle = math.degrees( InCamera[1] )
	NewCamera.clipEnd = InCamera[2]
	NewCamera.clipStart = InCamera[3]
	
	NewCameraObj = BlenderScene.objects.new( NewCamera )
	NewCameraObj.setName( "Camera"+str(InCamera[0]) )
	NewCameraObj.setLocation( InCamera[4], InCamera[5], InCamera[6] )
	
	NewEmptyObj = BlenderScene.objects.new( "Empty" )
	NewEmptyObj.setName( "CameraTarget"+str(InCamera[0]) )
	NewEmptyObj.setLocation( InCamera[7], InCamera[8], InCamera[9] )
	
	NewCameraObj.makeTrack( NewEmptyObj )
	
	return NewCameraObj, NewEmptyObj


def Load( FileName ):
	'''
	imports M2I
	'''
	
	# print status
	print( "\nimporting: \"" + FileName + "\"" )
	
	# start timer
	TimeStart = Blender.sys.time()
	
	# deselect all
	BlenderScene = Blender.Scene.GetCurrent()
	BlenderScene.objects.selected = []
	
	# open file
	file = open( FileName, "rb" )
	endianness = BinaryIO.EEndianness_Little
	
	# load
	InSignature = BinaryIO.ReadUInt32( file, endianness )
	if InSignature != 0 and InSignature != 1:
		print( "\terror: bad signature" )
		return;
	
	# load subsets
	NewMeshes = []	# need to make a list of all the meshes we make so we can make armature parent later
	InSubsetsCount = BinaryIO.ReadUInt32( file, endianness )
	for iSubset in range( 0, InSubsetsCount ):
		InSubsetID = BinaryIO.ReadUInt32( file, endianness )
		
		# load root bone
		InRootBone = BinaryIO.ReadUInt16( file, endianness )
		
		# load materials
		InMaterials = []
		InMaterialCount = 1
		if InSignature == 1:
			InMaterialCount = BinaryIO.ReadUInt32( file, endianness )
		for iMaterial in range( 0, InMaterialCount ):
			InMaterial = []
			InMaterial.append( BinaryIO.ReadUInt16( file, endianness ) )	# material flags 1 "Flags1"
			InMaterial.append( BinaryIO.ReadUInt16( file, endianness ) )	# material flags 2 "Flags2"
			InMaterial.append( BinaryIO.ReadSInt16( file, endianness ) )	# material color index "iColor"
			InMaterial.append( BinaryIO.ReadUInt16( file, endianness ) )	# material render flags index "iRenderFlags"
			InMaterial.append( BinaryIO.ReadUInt16( file, endianness ) )	# material texture unit 1 index "iTexUnit1"
			InMaterial.append( BinaryIO.ReadUInt16( file, endianness ) )	# material shader "Shader"
			InMaterial.append( BinaryIO.ReadUInt16( file, endianness ) )	# material texture index "iTex"
			InMaterial.append( BinaryIO.ReadUInt16( file, endianness ) )	# material texture unit 2 index "iTextUnit2"
			InMaterial.append( BinaryIO.ReadUInt16( file, endianness ) )	# material transparency index "iTrans"
			InMaterial.append( BinaryIO.ReadUInt16( file, endianness ) )	# material texture animation index "iTexAnim"
			InMaterials.append( InMaterial )
		
		# load vertices
		iVertex = 0
		InPositions = []
		InTextures = []
		InVertexGroups = {}
		InSubsetVertexCount = BinaryIO.ReadUInt32( file, endianness )
		for  iSubsetVertex in range( 0, InSubsetVertexCount ):
			InVertexPosition = []
			X = BinaryIO.ReadFloat32( file, endianness )
			Y = BinaryIO.ReadFloat32( file, endianness )
			Z = BinaryIO.ReadFloat32( file, endianness )
			InVertexPosition.append( -Y )
			InVertexPosition.append( X )
			InVertexPosition.append( Z )
			InPositions.append( InVertexPosition )
			
			InVertexBoneWeights = []
			InVertexBoneWeights.append( BinaryIO.ReadUInt8( file, endianness ) )
			InVertexBoneWeights.append( BinaryIO.ReadUInt8( file, endianness ) )
			InVertexBoneWeights.append( BinaryIO.ReadUInt8( file, endianness ) )
			InVertexBoneWeights.append( BinaryIO.ReadUInt8( file, endianness ) )
			
			InVertexBones = []
			InVertexBones.append( BinaryIO.ReadUInt8( file, endianness ) )
			InVertexBones.append( BinaryIO.ReadUInt8( file, endianness ) )
			InVertexBones.append( BinaryIO.ReadUInt8( file, endianness ) )
			InVertexBones.append( BinaryIO.ReadUInt8( file, endianness ) )
			
			for i in range( 0, 4 ):
				if InVertexBones[i] != 0:
					if ( InVertexBones[i] in InVertexGroups ) == False:
						InVertexGroups[InVertexBones[i]] = [[],[]]
					InVertexGroups[InVertexBones[i]][0].append( iVertex )
					InVertexGroups[InVertexBones[i]][1].append( InVertexBoneWeights[i] / 256.0 )
			
			InVertexNormal = []
			X = BinaryIO.ReadFloat32( file, endianness )
			Y = BinaryIO.ReadFloat32( file, endianness )
			Z = BinaryIO.ReadFloat32( file, endianness )
			InVertexNormal.append( -Y )
			InVertexNormal.append( X )
			InVertexNormal.append( Z )
			
			InVertexTexture = []
			InVertexTexture.append( BinaryIO.ReadFloat32( file, endianness ) )
			InVertexTexture.append( 1.0 - BinaryIO.ReadFloat32( file, endianness ) )
			InTextures.append( InVertexTexture )
			
			iVertex += 1
		
		# load triangles
		InTriangles = []
		InSubsetTriangleCount = BinaryIO.ReadUInt32( file, endianness )
		for iSubsetTriangle in range( 0, InSubsetTriangleCount ):
			InTriangle = []
			InTriangle.append( BinaryIO.ReadUInt16( file, endianness ) )
			InTriangle.append( BinaryIO.ReadUInt16( file, endianness ) )
			InTriangle.append( BinaryIO.ReadUInt16( file, endianness ) )
			InTriangles.append( InTriangle )
		
		# create the mesh
		NewMesh = CreateMesh( BlenderScene, InSubsetID, InPositions, InTextures, InTriangles, InVertexGroups )
		
		# add properties to new mesh
		NewMesh.addProperty( "M2_RootBone", InRootBone, "INT" )
		for iMaterial in range( 0, len( InMaterials ) ):
			InMaterial = InMaterials[iMaterial]
			iMaterialString = str( iMaterial )
			NewMesh.addProperty( "M2_" + iMaterialString + "_Flags1",					InMaterial[0], "INT" )
			NewMesh.addProperty( "M2_" + iMaterialString + "_Flags2",					InMaterial[1], "INT" )
			NewMesh.addProperty( "M2_" + iMaterialString + "_iColor",					InMaterial[2], "INT" )
			NewMesh.addProperty( "M2_" + iMaterialString + "_iRenderFlags",				InMaterial[3], "INT" )
			NewMesh.addProperty( "M2_" + iMaterialString + "_iUnit",					InMaterial[4], "INT" )
			NewMesh.addProperty( "M2_" + iMaterialString + "_iShader",					InMaterial[5], "INT" )
			NewMesh.addProperty( "M2_" + iMaterialString + "_iTexture",					InMaterial[6], "INT" )
			NewMesh.addProperty( "M2_" + iMaterialString + "_iTextureUnit",				InMaterial[7], "INT" )
			NewMesh.addProperty( "M2_" + iMaterialString + "_iTransparency",			InMaterial[8], "INT" )
			NewMesh.addProperty( "M2_" + iMaterialString + "_iTextureAnimation",		InMaterial[9], "INT" )
		
		NewMeshes.append( NewMesh )
		BlenderScene.update( 1 )
	
	# load bones
	InBones = []
	InBonesCount = BinaryIO.ReadUInt32( file, endianness )
	for iBone in range( 0, InBonesCount ):
		InBone = []
		InBone.append( BinaryIO.ReadUInt16( file, endianness ) )	# this bone's index ID
		InBone.append( BinaryIO.ReadSInt16( file, endianness ) )	# parent bone's index ID
		X = BinaryIO.ReadFloat32( file, endianness )
		Y = BinaryIO.ReadFloat32( file, endianness )
		Z = BinaryIO.ReadFloat32( file, endianness )
		InBone.append( -Y )
		InBone.append( X )
		InBone.append( Z )
		InBones.append( InBone )
	Root = CreateArmature( BlenderScene, "ArmatureRoot", InBones )
	
	# parent meshes to armature
	BlenderScene.update( 1 )
	Root.makeParentDeform( NewMeshes )
	BlenderScene.update( 1 )
	
	# load attachments
	InAttachments = []
	InAttachmentsCount = BinaryIO.ReadUInt32( file, endianness )
	for iAttachment in range( 0, InAttachmentsCount ):
		InAttachment = []
		
		InAttachment.append( BinaryIO.ReadUInt32( file, endianness ) )	# this attachment's index ID
		InAttachment.append( BinaryIO.ReadSInt16( file, endianness ) )	# parent bone's index ID
		
		X = BinaryIO.ReadFloat32( file, endianness )
		Y = BinaryIO.ReadFloat32( file, endianness )
		Z = BinaryIO.ReadFloat32( file, endianness )
		InAttachment.append( -Y )
		InAttachment.append( X )
		InAttachment.append( Z )
		
		InAttachments.append( InAttachment )
	CreateAttachments( BlenderScene, InAttachments, Root )
	BlenderScene.update( 1 )
	
	# load cameras
	InCameras = []
	InCamerasCount = BinaryIO.ReadUInt32( file, endianness )
	for iCamera in range( 0, InCamerasCount ):
		InCamera = []
		
		InCamera.append( BinaryIO.ReadSInt32( file, endianness ) )	# type
		InCamera.append( BinaryIO.ReadFloat32( file, endianness ) )	# field of view
		InCamera.append( BinaryIO.ReadFloat32( file, endianness ) )	# clip far
		InCamera.append( BinaryIO.ReadFloat32( file, endianness ) )	# clip near
		X = BinaryIO.ReadFloat32( file, endianness )
		Y = BinaryIO.ReadFloat32( file, endianness )
		Z = BinaryIO.ReadFloat32( file, endianness )
		InCamera.append( -Y )	# position x
		InCamera.append( X )	# position y
		InCamera.append( Z )	# position z
		
		X = BinaryIO.ReadFloat32( file, endianness )
		Y = BinaryIO.ReadFloat32( file, endianness )
		Z = BinaryIO.ReadFloat32( file, endianness )
		InCamera.append( -Y )	# target x
		InCamera.append( X )	# target y
		InCamera.append( Z )	# target z
		
		NewCamera, NewTarget = CreateCamera( BlenderScene, InCamera )
		
		Root.makeParent( [NewTarget] )
		NewTarget.makeParent( [NewCamera] )
		
		BlenderScene.update( 1 )
	
	file.close()
	
	BlenderScene.update( 1 )
	
	print( "import time: " + str( Blender.sys.time() - TimeStart ) )


def LoadUI( FileName ):
	Blender.Window.WaitCursor(1)
	
	if not FileName.lower().endswith(".m2i"):
		FileName += ".m2i"
		
	Load( FileName )
	
	Blender.Window.WaitCursor(0)


if __name__ == '__main__':
	Blender.Window.FileSelector( LoadUI, "Import M2I", "*.m2i" )
