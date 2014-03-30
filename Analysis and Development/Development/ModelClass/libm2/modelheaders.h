#ifndef MODELHEADERS_H
#define MODELHEADERS_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "vec3d.h"
//typedef float	Vec3D[3];
//typedef float	Vec2D[2];
typedef unsigned char uint8;
typedef char int8;
typedef unsigned __int16 uint16;
typedef __int16 int16;
typedef unsigned __int32 uint32;
typedef __int32 int32;

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
	uint32 nameOfs;
	uint32 Flags;

	uint32 nGlobalSequences;
	uint32 ofsGlobalSequences;
	uint32 nAnimations;
	uint32 ofsAnimations;
	uint32 nAnimationLookup;
	uint32 ofsAnimationLookup;
	uint32 nBones;
	uint32 ofsBones;
	uint32 nKeyBoneLookup;
	uint32 ofsKeyBoneLookup;

	uint32 nVertices;
	uint32 ofsVertices;
	uint32 nViews;

	uint32 nColors;
	uint32 ofsColors;

	uint32 nTextures;
	uint32 ofsTextures;

	uint32 nTransparency; // H
	uint32 ofsTransparency;
	uint32 nTexAnims; // J
	uint32 ofsTexAnims;
	uint32 nTexReplace;
	uint32 ofsTexReplace;

	uint32 nRenderFlags;
	uint32 ofsRenderFlags;
	uint32 nBoneLookupTable;
	uint32 ofsBoneLookupTable;

	uint32 nTexLookup;
	uint32 ofsTexLookup;

	uint32 nTexUnitLookup; // L
	uint32 ofsTexUnitLookup;
	uint32 nTransparencyLookup; // M
	uint32 ofsTransparencyLookup;
	uint32 nTexAnimLookup;
	uint32 ofsTexAnimLookup;

	//not sure about these :/
	Vec3D VertexBoxMin;//?
	Vec3D VertexBoxMax;//?
	float VertexBoxRadius;
	Vec3D BoundingBoxMin;//?
	Vec3D BoundingBoxMax;//?
	float BoundingBoxRadius;

	uint32 nBoundingTriangles;
	uint32 ofsBoundingTriangles;
	uint32 nBoundingVertices;
	uint32 ofsBoundingVertices;
	uint32 nBoundingNormals;
	uint32 ofsBoundingNormals;

	uint32 nAttachments; // O
	uint32 ofsAttachments;
	uint32 nAttachLookup; // P
	uint32 ofsAttachLookup;
	uint32 nEvents; // SoundEvents
	uint32 ofsEvents;
	uint32 nLights; // R
	uint32 ofsLights;
	uint32 nCameras; // S
	uint32 ofsCameras;
	uint32 nCameraLookup;
	uint32 ofsCameraLookup;
	uint32 nRibbonEmitters; // U
	uint32 ofsRibbonEmitters;
	uint32 nParticleEmitters; // V
	uint32 ofsParticleEmitters;

};


//only available if header->flags &0x8
//something about shadingflags
struct ofsUnk{
	int nUnk;
	int ofsnk;
};

struct ModelAnimation {
	int16 animID;
	int16 subAnimID;
	uint32 length;

	float moveSpeed;

	uint32 loopType;
	uint32 flags;
	uint32 d1;
	uint32 d2;
	uint32 playSpeed; // note: this can't be play speed because it's 0 for some models

	Vec3D boxA, boxB;
	float rad;

	int16 NextAnimation;
	int16 Index;
};

struct AnimationBlock {
	int16 type; // interpolation type (0=none, 1=linear, 2=hermite, 3=Bezier)
	int16 seq; // global sequence id or -1
	uint32 nTimes;
	uint32 ofsTimes;
	uint32 nKeys;
	uint32 ofsKeys;
};

struct FakeAnimationBlock {
	uint32 nTimes;
	uint32 ofsTimes;
	uint32 nKeys;
	uint32 ofsKeys;
};

struct AnimationBlockHeader {
	uint32 nEntrys;
	uint32 ofsEntrys;
};

struct AnimSubStructure {
	uint32 n;
	uint32 ofs;
};

struct ModelTexAnimDef {
	AnimationBlock trans, rot, scale;
};

struct ModelVertex {
	Vec3D pos;
	uint8 weights[4];
	uint8 bones[4];
	Vec3D normal;
	Vec2D texcoords;
	int unk1, unk2; // always 0,0 so this is probably unused
};

struct ModelView {
	char id[4]; // Signature
	uint32 nIndex, ofsIndex; // Vertices in this model (index into vertices[])
	uint32 nTris, ofsTris; // indices
	uint32 nProps, ofsProps; // additional vtx properties
	uint32 nSub, ofsSub; // materials/renderops/submeshes
	uint32 nTex, ofsTex; // material properties/textures
	int32 lod; // LOD bones
};

/// One material + render operation
struct ModelGeoset {
	uint16 id; // mesh part id?
	uint16 d2; // ?
	uint16 vstart; // first vertex
	uint16 vcount; // num vertices
	uint16 istart; // first index
	uint16 icount; // num indices
	uint16 d3; // number of bone indices
	uint16 d4; // ? always 1 to 4
	uint16 d5; // ?
	uint16 d6; // root bone?
	Vec3D v;
	float unknown[4]; // Added in WoW 2.0?
};

/// A texture unit (sub of material)
struct ModelTexUnit {
	// probably the texture units
	// size always >=number of materials it seems
	uint16 flags; // Flags
	uint16 order; // ?
	uint16 op; // Material this texture is part of (index into mat)
	uint16 op2; // Always same as above?
	int16 colorIndex; // color or -1
	uint16 flagsIndex; // more flags...
	uint16 texunit; // Texture unit (0 or 1)
	uint16 d4; // ? (seems to be always 1)
	uint16 textureid; // Texture id (index into global texture list)
	uint16 texunit2; // copy of texture unit value?
	uint16 transid; // transparency id (index into transparency list)
	uint16 texanimid; // texture animation id
};

// block X - render flags
struct ModelRenderFlags {
	uint16 flags;
	//unsigned char f1;
	//unsigned char f2;
	uint16 blend;
};

// block G - color defs
struct ModelColorDef {
	AnimationBlock color;
	AnimationBlock opacity;
};

// block H - transp defs
struct ModelTransDef {
	AnimationBlock trans;
};

struct ModelTextureDef {
	uint32 type;
	uint32 flags;
	uint32 nameLen;
	uint32 nameOfs;
};
struct ModelLightDef {
	int16 type;
	int16 bone;
	Vec3D pos;
	AnimationBlock ambColor;
	AnimationBlock ambIntensity;
	AnimationBlock diffcolor;
	AnimationBlock diffintensity;
	AnimationBlock attStart;
	AnimationBlock attEnd;
	AnimationBlock Enabled;
};

struct ModelCameraDef {
	int32 id;
	float fov, farclip, nearclip;
	AnimationBlock transPos;
	Vec3D pos;
	AnimationBlock transTarget;
	Vec3D target;
	AnimationBlock rot;
};

struct ModelParticleEmitterDef {
	int32 id;
	int16 flags;
	int16 flags2;
	Vec3D pos;
	int16 bone;
	int16 texture;
	int32 lenModelName;
	int32 ofsModelName;
	int32 lenParticleName;
	int32 ofsParticleName;
	int8 blend;
	int16 emittertype;
	int16 particletype;
	int8 pad;
	int16 rot;
	int16 rows;
	int16 cols;
	AnimationBlock Emissionspeed;
	AnimationBlock SpeedVariation;
	AnimationBlock VerticalRange;
	AnimationBlock HorizontalRange;
	AnimationBlock Gravity;
	AnimationBlock Lifespan;
	int32 unknown1;
	AnimationBlock EmissionRate;
	int32 unknown2;
	AnimationBlock EmissionAreaLength;
	AnimationBlock EmissionAreaWidth;
	AnimationBlock Gravity2;
	FakeAnimationBlock colors;
	FakeAnimationBlock opacity;
	FakeAnimationBlock sizes;
	int32 d[2];
	FakeAnimationBlock intensity;
	FakeAnimationBlock unknownblock;
	float unk[3];
	float scales[3];
	float slowdown;
	float unknown3[2];
	float rotation; //Sprite Rotation
	float unknown4[2];
	float Rot1[3]; //Model Rotation 1
	float Rot2[3]; //Model Rotation 2
	float Trans[3]; //Model Translation
	float f2[4];
	int nUnknownReference;
	int ofsUnknownReference;
	AnimationBlock Enabled;
};

struct ModelRibbonEmitterDef {
	int32 id;
	int32 bone;
	Vec3D pos;
	int32 nTextures;
	int32 ofsTextures;
	int32 nUnknown;
	int32 ofsUnknown;
	AnimationBlock color;
	AnimationBlock opacity;
	AnimationBlock above;
	AnimationBlock below;
	float res, length, unk;
	int16 s1, s2;
	AnimationBlock unk1;
	AnimationBlock enabled;
	int32 unknown;
};

struct ModelEvents {
	char id[4];
	int32 data;
	int32 bone;
	Vec3D pos;
	int16 type;
	int16 seq;
	uint32 nTimes;
	uint32 ofsTimes;
};

struct ModelAttachmentDef {
	int32 id;
	int32 bone;
	Vec3D pos;
	AnimationBlock Enabled;
};

struct ModelBoneDef {
	int32 KeyBoneID;
	uint32 flags;
	int16 parent; // parent bone index
	uint16 unk[3];
	AnimationBlock translation;
	AnimationBlock rotation;
	AnimationBlock scaling;
	Vec3D pivot;
};

struct ModelBoundTriangle {
	uint16 index[3];
};

#pragma pack(pop)

#endif
