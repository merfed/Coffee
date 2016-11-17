#ifndef COMMON_H
#define COMMON_H

extern char *model_name;
extern size_t name_length;
extern int classic;

typedef unsigned char uint8;
typedef char int8;
typedef unsigned short uint16;
typedef short int16;
typedef unsigned int uint32;
typedef int int32;
typedef int Range[2];
typedef short Quat[4];
typedef float QuatF[4];
typedef float Vec3D[3];
typedef float Vec2D[2];
typedef uint16 Triangle[3]; //Indices for Triangles
typedef uint16 Vertex; //The vertex in the global vertex list
typedef uint32 Property; //Bones Indices into BoneLookupTable
typedef Vec3D BigFloat[3];

typedef struct ArrayRef { //Can point to absolutely anything
	uint32 n;
	uint32 ofs;
} ArrayRef;

typedef struct ModelVertex {
	Vec3D pos;
	uint8 weights[4];
	uint8 bones[4];
	Vec3D normal;
	Vec2D texcoords;
	Vec2D texcoords2;
} ModelVertex;

typedef struct ModelTextureDef {
	uint32 type;
	uint32 flags;
	uint32 nameLen;
	uint32 nameOfs;
} ModelTextureDef;

typedef struct TexUnit {
	uint16 Flags;
	uint16 RenderOrder;
	uint16 SubmeshIndex1;
	uint16 SubmeshIndex2;
	int16 ColorIndex;
	uint16 RenderFlags;
	uint16 TexUnitNumber;
	uint16 one;
	uint16 Texture;
	uint16 TexUnitNumer2;
	uint16 Transparency;
	uint16 TexAnim;
} TexUnit;
#endif
