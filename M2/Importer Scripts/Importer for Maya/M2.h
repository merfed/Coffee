#ifndef M2_H
#define M2_H

//typedef char byte;
typedef unsigned char uint8;
typedef unsigned int uint32;
typedef signed int int32;
typedef unsigned short uint16;

// Model header
struct M2Header
{
	char   magic[4];
	uint8  version[4];									
	uint32 nameLen;
	uint32 offsetName;
	uint32 type;
	uint32 nGlobalSequences;
	uint32 offsetGlobalSequences;
	uint32 nAnimations;
	uint32 offsetAnimations;
	uint32 nC;
	uint32 offsetC;
	uint32 nD;
	uint32 offsetD;
	uint32 nBones;
	uint32 offsetBones;
	uint32 nF;
	uint32 offsetF;
	uint32 nVertices;
	uint32 offsetVertices;
	uint32 nViews;
	uint32 offsetViews;
	uint32 nColors;
	uint32 offsetColors;
	uint32 nTextures;
	uint32 offsetTextures;
	uint32 nTransparency;
	uint32 offsetTransparency;
	uint32 nI;
	uint32 offsetI;
	uint32 nTexAnims;
	uint32 offsetTexAnims;
	uint32 nTexReplace;
	uint32 offsetTexReplace;
	uint32 nTexFlags;
	uint32 offsetTexFlags;
	uint32 nBoneGroups;
	uint32 offsetBoneGroups;
	uint32 nTexLookup;
	uint32 offsetTexLookup;
	uint32 nTexUnitLookup;
	uint32 offsetTexUnitLookup;
	uint32 nTransparencyLookup;
	uint32 offsetTransparencyLookup;
	uint32 nTexAnimLookup;
	uint32 offsetTexAnimLookup;
	float unknown14floats[14];
	uint32 nBoundingTriangles;
	uint32 offsetBoundingTriangles;
	uint32 nBoundingVertices;
	uint32 offsetBoundingVertices;
	uint32 nBoundingNormals;
	uint32 offsetBoundingNormals;
	uint32 nAttachments;
	uint32 offsetAttachments;
	uint32 nAttachLookup;
	uint32 offsetAttachLookup;
	uint32 nQ;
	uint32 offsetQ;
	uint32 nLights;
	uint32 offsetLights;
	uint32 nCameras;
	uint32 offsetCameras;
	uint32 nT;
	uint32 offsetT;
	uint32 nRibbonEmitters;
	uint32 offsetRibbonEmitters;
	uint32 nParticleEmitters;
	uint32 offsetParticleEmitters;
};

// Model vertex
struct M2Vertex
{
	float pos[3];
	uint8 bw[4];
	uint8 bi[4];
	float normal[3];
	float uv[2];
	uint32 unknown[2];
};

// LOD level?
struct M2View
{
	uint32 nIndex;
	uint32 offsetIndex;
	uint32 nTris;
	uint32 offsetTris;
	uint32 nProps;
	uint32 offsetProps;
	uint32 nSub;
	uint32 offsetSub;
	uint32 nTex;
	uint32 offsetTex;
	int32	 lod;
};

// Piece of a mesh (meshes are cut to pieces 
// that have single material on them)
struct M2Geoset 
{
	uint16 id;
	uint16 d2;
	uint16 vStart;
	uint16 vCount;
	uint16 iStart;
	uint16 iCount;
	uint16 nBones;
	uint16 d4;
	uint16 d5;
	uint16 d6;
	float v[3];
};

// Texture info
struct M2Texture
{
	uint32 type;
	uint32 flags;
	uint32 nameLen;
	uint32 offsetName;
};

#endif