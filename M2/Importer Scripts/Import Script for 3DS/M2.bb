;M2 Import
;Copyright (c) 2006 r!sc
;Thank you 'ComicSansMS' at 3DBuzz Forums for helping me with this!
;Released under the GNU General Public License (http://www.gnu.org/copyleft/gpl.html)

Dim mVertexX# ( 1 )
Dim mVertexY# ( 1 )
Dim mVertexZ# ( 1 )
Dim UVu ( 1 )
Dim UVv ( 1 )
Dim mLOD( 1 )
Dim mFaces( 1 )



Function LoadM2(m2file$, dpoly = False)

;m2file$="D:\Character\Orc\Male\OrcMale.m2"
FLoad = ReadFile(m2file$)


;Start Reading M2

SeekFile ( FLoad,68 )

nVertex = ReadInt(fload)	;Read 32bit integer
ofsVertex = ReadInt(Fload)

SeekFile ( fload, 80 )

ofsLODVert = ReadInt(fload)

SeekFile ( fload, ofsVertex)


;Begin reading vertices

Dim mVertexX# ( nVertex )
Dim mVertexY# ( nVertex )
Dim mVertexZ# ( nVertex )

Dim UVu ( nVertex )
Dim UVv ( nVertex )

Repeat
	
	If loopcount = nVertex Then 
	
		Exit
	
		Else
		
			mVertexX( loopcount ) = ReadFloat( Fload )
			mVertexY( loopcount ) = ReadFloat( Fload )
			mVertexZ( loopcount ) = ReadFloat( Fload )
			
			;Hop 8 bytes
			ReadFloat(fLoad)
			ReadFloat(fLoad)
			;-----------
			
			;Hop some more
			ReadFloat(fload)
			ReadFloat(fload)
			ReadFloat(fload)
			;-----------
			
			UVu( loopcount ) = ReadFloat(fload)
			UVv( loopcount ) = ReadFloat(fload)
			
			ReadFloat(fload)
			ReadFloat(fload)
			
			loopcount = loopcount+1
		EndIf
Forever

;Finished reading vertices, read LOD

SeekFile ( fload,ofsLODVert )

nLODVert = ReadInt(fload)
Dim mLOD( nLODVert )

ofsLODVX = ReadInt(fload)


SeekFile ( fload,ofsLODVX )

loopcount=0
Repeat
	If loopcount = nLODVert Then
		
		Exit
		
		Else
		
			mLOD( loopcount ) = ReadShort(fload)
			loopcount = loopcount + 1
			
	EndIf
Forever

;Done reading LOD data, read faces

SeekFile ( fload,ofsLODVert+8 )

nFaces = ReadInt(fload)
Dim mFaces( nFaces )

ofsFaces = ReadInt(fload)

SeekFile ( fload,ofsFaces )

loopcount=0
Repeat
	If loopcount = nFaces Then
		
		Exit
		
		Else
		
			mFaces( loopcount ) = ReadShort(fload)
			loopcount = loopcount + 1
			
	EndIf
Forever

;Done reading everything :)
CloseFile(fload)

mesh = CreateMesh()

RotateEntity mesh,0,0,180

	
	surf = CreateSurface(mesh)
	
	
	For i = 0 To nFaces-1 Step 3
		


		v0 = AddVertex (surf, mVertexX#( mLOD(mFaces(i)) ), mVertexZ#( mLOD(mFaces(i)) ) * (-1), mVertexY#( mLOD(mFaces(i))), UVu( mLOD(mFaces(i))), UVv( mLOD(mFaces(i))))
		v1 = AddVertex (surf, mVertexX#( mLOD(mFaces(i+1)) ), mVertexZ#( mLOD(mFaces(i+1)) ) * (-1), mVertexY#( mLOD(mFaces(i+1))),UVu( mLOD(mFaces(i+1))), UVv( mLOD(mFaces(i+1))) )
		v2 = AddVertex (surf, mVertexX#( mLOD(mFaces(i+2)) ), mVertexZ#( mLOD(mFaces(i+2)) ) * (-1), mVertexY#( mLOD(mFaces(i+2))), UVu( mLOD(mFaces(i+2))), UVv( mLOD(mFaces(i+2))) )
		


		AddTriangle (surf, v0, v1, v2)
		If dpoly Then AddTriangle (surf, v2, v1, v0)
		
		
	Next
	
	UpdateNormals mesh
	
	
	Return mesh
	
End Function
	
	
	