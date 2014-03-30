#ifndef DATAEXCHANGE_H
#define DATAEXCHANGE_H

#include "M2.h"

#include <list>

// Temporary structs used to pass data to Maya

struct DETexture
{
	std::string filename;
};

struct DEMesh
{	
	unsigned int nOriginalVertices;
	M2Vertex* originalVertices;

	unsigned int nIndices;
	uint16* indices;

	std::list<DETexture> textures;
};

#endif