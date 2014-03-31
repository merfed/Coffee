#ifndef MODELHEADERS2_H
#define MODELHEADERS2_H

typedef unsigned char uint8;
typedef char int8;
typedef unsigned short uint16;
typedef short int16;
typedef unsigned int uint32;
typedef int int32;

struct AnimationBlock {
	int16 type;		// interpolation type (0=none, 1=linear, 2=hermite)
	int16 seq;		// global sequence id or -1
	uint32 nRanges;
	uint32 ofsRanges;
	uint32 nTimes;
	uint32 ofsTimes;
	uint32 nKeys;
	uint32 ofsKeys;
};

#pragma pack(push,1)

struct Vertex {
    float tu, tv;
    float x, y, z;
};

struct CharModelDetails {
	bool closeRHand;
	bool closeLHand;

	bool isChar;
	bool isMounted;

	void Reset() {
		closeRHand = false;
		closeLHand = false;
		isChar = false;
		isMounted = false;
	}
};

struct ModelHeader {
	char id[4];
	uint8 version[4];
	uint32 nameLength;
	uint32 nameOfs; // <<
	uint32 type;

	uint32 nGlobalSequences;
	uint32 ofsGlobalSequences; // <<
	uint32 nAnimations;
	uint32 ofsAnimations; // <<
	uint32 nC;
	uint32 ofsC; // <<
	uint32 nD;
	uint32 ofsD; // <<
	uint32 nBones;             // !!!!!!!!!
	uint32 ofsBones; // <<     // !!!!!!!!!
	uint32 nF;
	uint32 ofs1; // <<

	uint32 nVertices;
	uint32 ofs2; // <<
	uint32 nViews;
	uint32 ofs3; // <<

	uint32 nColors;
	uint32 ofs4; // <<

	uint32 nTextures;
	uint32 ofs5; // <<

	uint32 nTransparency; 
	uint32 ofs6; // <<
	uint32 nI;  
	uint32 ofs7; // <<
	uint32 nTexAnims;
	uint32 ofs8; // <<
	uint32 nTexReplace;
	uint32 ofs9; // <<

	uint32 nTexFlags;
	uint32 ofs10; // <<
	uint32 nY;
	uint32 ofs11; // <<

	uint32 nTexLookup;
	uint32 ofs12; // <<

	uint32 nTexUnitLookup;
	uint32 ofs13; // <<
	uint32 nTransparencyLookup;
	uint32 ofs14; // <<
	uint32 nTexAnimLookup;
	uint32 ofs15; // <<

	float floats[14];

	uint32 nBoundingTriangles;
	uint32 ofs16; // <<
	uint32 nBoundingVertices;
	uint32 ofs17; // <<
	uint32 nBoundingNormals;
	uint32 ofs18; // <<

	uint32 nAttachments;
	uint32 ofs19; // <<
	uint32 nAttachLookup;
	uint32 ofs20; // <<
	uint32 nQ;
	uint32 ofs21; // <<
	uint32 nLights;
	uint32 ofs22; // <<
	uint32 nCameras; 
	uint32 ofs23; // <<
	uint32 nT;
	uint32 ofs24; // <<
	uint32 nRibbonEmitters; 
	uint32 ofs25; // <<
	uint32 nParticleEmitters;
	uint32 ofs26; // <<

};


struct ModelGeoset1 {
	uint16 id;		// mesh part id?
	uint16 d2;		// ?
	uint16 vstart;	// first vertex
	uint16 vcount;	// num vertices
	uint16 istart;	// first index
	uint16 icount;	// num indices
	uint16 d3;		// number of bone indices
	uint16 d4;		// ? always 1 to 4
	uint16 d5;		// ?
	uint16 d6;		// root bone?
	Vec3D v;
};

struct ModelGeoset2 {
	uint16 id;		// mesh part id?
	uint16 d2;		// ?
	uint16 vstart;	// first vertex
	uint16 vcount;	// num vertices
	uint16 istart;	// first index
	uint16 icount;	// num indices
	uint16 d3;		// number of bone indices
	uint16 d4;		// ? always 1 to 4
	uint16 d5;		// ?
	uint16 d6;		// root bone?
	Vec3D v;
	float unknown[4];	// Added in WoW 2.0?
};

struct ModelBoneDef1 {
	int32 animid;
	int32 flags;
	int16 parent; // parent bone index
	int16 geoid;
	AnimationBlock translation;
	AnimationBlock rotation;
	AnimationBlock scaling;
	Vec3D pivot;
};

struct ModelBoneDef2 {
	int32 animid;
	int32 flags;
	int16 parent; // parent bone index
	int16 geoid;
	int32 unknown;  // new int added to the bone definitions.  Added in WoW 2.0
	AnimationBlock translation;
	AnimationBlock rotation;
	AnimationBlock scaling;
	Vec3D pivot;
};


// FAKES!!

struct ModelAnimation {
    char fake[68];
};

struct ModelTexAnimDef {
    char fake[84];
};

struct ModelVertex {
	Vec3D pos;
	uint8 weights[4];
	uint8 bones[4];
	Vec3D normal;
	Vec2D texcoords;
	int unk1, unk2;
};

struct ModelView {
    char fake[44];
};

struct ModelTexUnit{
    char fake[24];
};

struct ModelRenderFlags {
    char fake[4];
};

struct ModelColorDef {
    char fake[56];
};

struct ModelTransDef {
    char fake[28];
};

struct ModelTextureDef {
    char fake[16];
};

struct ModelLightDef {
    char fake[212];
};

struct ModelCameraDef {
    char fake[124];
};

struct ModelParticleParams {
    char fake[152];
};

struct ModelParticleEmitterDef {
    char fake[504];
};

struct ModelRibbonEmitterDef {
    char fake[220];
};

struct ModelBlockQ {
	char id[44];
};

struct ModelAttachmentDef {
	char fake[48];
};

#pragma pack(pop)

#endif
