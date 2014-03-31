#include "Plugin.h"
#include "ImportException.h"

#include <maya/MFloatArray.h>
#include <maya/MIntArray.h>
#include <maya/MFloatPointArray.h>
#include <maya/MFnMesh.h>
#include <maya/MFnDependencyNode.h>

#include <iostream>

using namespace std;

// Print some of the known fields of the header
void printHeader(const M2Header& header)
{	
	cout << "==File Header==" << endl;

	cout << "Magic: " << header.magic[0] << header.magic[1] << header.magic[2] << header.magic[3] << endl;		
	cout << "Version: " << header.version[0] << header.version[1] << header.version[2] << header.version[3] << endl;		
	cout << "Global Sequences: " << header.nGlobalSequences << endl;
	cout << "Animations: " << header.nAnimations << endl;
	cout << "Bones: " << header.nBones << endl;
	cout << "Vertices: " << header.nVertices << endl;
	cout << "Views: " << header.nViews << endl;
	cout << "Textures: " << header.nTextures << endl;
	cout << "Bone Groups: " << header.nBoneGroups << endl;
	cout << "Texture Lookups:" << header.nTexLookup << endl;
	cout << "Bounding Triangles: " << header.nBoundingTriangles << endl;
	cout << "Bounding Vertices: " << header.nBoundingVertices << endl;
	cout << "Bounding Normals: " << header.nBoundingNormals << endl;	
}

void printVertex(const M2Vertex& v)
{
	cout << "=Read Vertex=" << endl;
	cout << "Pos: (" << v.pos[0] << ", " << v.pos[1] << ", " << v.pos[2] << ")" << endl;
	cout << "BW:  (" << (int)v.bw[0] << ", " << (int)v.bw[1] << ", " << (int)v.bw[2] << ", " << (int)v.bw[3] << ")" << endl;
	cout << "BI:  (" << (int)v.bi[0] << ", " << (int)v.bi[1] << ", " << (int)v.bi[2] << ", " << (int)v.bi[3] << ")" << endl;
	cout << "Nor: (" << v.normal[0] << ", " << v.normal[1] << ", " << v.normal[2] << ")" << endl;
	cout << "UV:  (" << v.uv[0] << ", " << v.uv[1] << ")" << endl;	
}

void printView(const M2View* view)
{	
	cout << "nTris: " << view->nTris << endl;
	cout << "nSub:  " << view->nSub << endl;
	cout << "LOD:   " << view->lod << endl;
}

void Plugin::importAll()
{
	// Read header
	M2Header header;

	if(m_verbose) cout << "Reading header..." << endl;
	memcpy(&header, m_file, sizeof(header));

	if(m_verbose) printHeader(header);

	DEMesh tmpMesh;

	// Read all vertices	
	tmpMesh.nOriginalVertices = header.nVertices;
	tmpMesh.originalVertices = new M2Vertex [header.nVertices];
	memcpy(tmpMesh.originalVertices, m_file + header.offsetVertices, sizeof(M2Vertex) * header.nVertices);

	// Print views
	if(m_verbose)
	{
		for(int i = 0; i < header.nViews; i++)
		{
			M2View* view = (M2View*)(m_file + header.offsetViews + i * sizeof(M2View));

			cout << "View " << i << ":" << endl;
			printView(view);
		}	
	}

	// Just use the first 'view' (is this best or worst quality LOD?)		
	M2View* view = (M2View*)(m_file + header.offsetViews);	

	uint16* indexLookup = (uint16*)(m_file + view->offsetIndex);
	uint16* triangles = (uint16*)(m_file + view->offsetTris);

	tmpMesh.nIndices = view->nTris;
	tmpMesh.indices = new uint16 [tmpMesh.nIndices];
	for(unsigned int i = 0; i < tmpMesh.nIndices; i++)
	{
		tmpMesh.indices[i] = indexLookup[triangles[i]];
	}

	// Read textures
/*
	if(header.nTextures)
	{
		M2Texture* m2Textures = (M2Texture*)(m_file + header.offsetTextures);

		for(uint32 i = 0; i < header.nTextures; i++)
		{
			char buffer[256];

			if(m2Textures[i].type == 0)
			{
				strncpy(buffer, (const char*)(m_file + m2Textures[i].offsetName), m2Textures[i].nameLen);
				buffer[m2Textures[i].nameLen] = '\0';

				DETexture texture;
				texture.filename = buffer;

				tmpMesh.textures.push_back(texture);

				if(m_verbose) cout << "Found texture '" << texture.filename << "'" << endl;
			}
		}
	}
*/

	// Send the data to Maya
	sendDataToMaya(tmpMesh, view);
}

void Plugin::sendDataToMaya(const DEMesh& mesh, const M2View* view)
{
	for(int iGeoset = 0; iGeoset < view->nSub; iGeoset++)
	{
		M2Geoset* geoset = (M2Geoset*)(m_file + view->offsetSub + iGeoset * sizeof(M2Geoset));

		// Only grab the vertices belonging to this geoset	
		MFloatPointArray vertexArray;
		MFloatArray uArray, vArray;
		for(int i = 0; i < geoset->vCount; i++)
		{
			int vi = geoset->vStart + i;
			vertexArray.append(MFloatPoint(	mesh.originalVertices[vi].pos[0], 
											mesh.originalVertices[vi].pos[2],
											-mesh.originalVertices[vi].pos[1]));
			uArray.append(mesh.originalVertices[vi].uv[0]);
			vArray.append(mesh.originalVertices[vi].uv[1]);
		}

		MIntArray polyCounts, polyConnects;
		MIntArray uvIds;

		int nPolys = geoset->iCount / 3;

		// Grab indices belonging to this geoset
		for(int i = 0, t = 0, uvId = 0; i < nPolys; i++)
		{
			polyCounts.append(3);

			for(int j = 0; j < 3; j++, t++, uvId++)
			{
				int ii = geoset->iStart + t;
				// We're referencing the local vertex array now, not the global one
				int index = mesh.indices[ii] - geoset->vStart;

				polyConnects.append(index);
				uvIds.append(index);
			}
		}

		// Create a mesh in maya
		MStatus status;
		MFnMesh fnMesh;

		if(m_verbose) cout << "Creating mesh..." << endl;
		MObject mayaMesh = fnMesh.create(geoset->vCount, nPolys, vertexArray, polyCounts, polyConnects, MObject::kNullObj, &status);
		if(status != MStatus::kSuccess) throw new ImportException("Failed to create mesh object.");

		char buffer[32];
		sprintf(buffer, "Geoset %d", geoset->id);
		if(m_verbose) cout << "Renaming mesh to '" << buffer << "'" << endl;

		MFnDependencyNode depfn(mayaMesh);
		depfn.setName(buffer);

		if(m_verbose) cout << "Setting UVs..." << endl;
		fnMesh.setUVs(uArray, vArray);
		fnMesh.assignUVs(polyCounts, uvIds);
	}
}