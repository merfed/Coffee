/**
 * @file
 * M2 structures
 */

#ifndef LK_MODEL_H
#define LK_MODEL_H
#include "common.h"

typedef struct LKModelHeader {
	char id[4];								//0x000
	uint32 version;							//0x004
	uint32 nameLength;						//0x008
	uint32 nameOfs;							//0x00c
	uint32 GlobalModelFlags;				//0x010

	uint32 nGlobalSequences;				//0x014
	uint32 ofsGlobalSequences;				//0x018
	uint32 nAnimations;						//0x01c
	uint32 ofsAnimations;					//0x020
	uint32 nAnimationLookup;				//0x024
	uint32 ofsAnimationLookup;				//0x028
	uint32 nBones;							//0x02c
	uint32 ofsBones;						//0x030
	uint32 nKeyBoneLookup;					//0x034
	uint32 ofsKeyBoneLookup;				//0x038

	uint32 nVertices;						//0x03c
	uint32 ofsVertices;						//0x040
	uint32 nViews;							//0x044

	uint32 nColors;							//0x048
	uint32 ofsColors;						//0x04c

	uint32 nTextures;						//0x050
	uint32 ofsTextures;						//0x054

	uint32 nTransparency; 					//0x058
	uint32 ofsTransparency;					//0x05c
	uint32 nTexAnims;						//0x060
	uint32 ofsTexAnims;						//0x064
	uint32 nTexReplace;						//0x068
	uint32 ofsTexReplace;					//0x06c

	uint32 nRenderFlags;					//0x070
	uint32 ofsRenderFlags;					//0x074
	uint32 nBoneLookupTable;				//0x078
	uint32 ofsBoneLookupTable;				//0x07c

	uint32 nTexLookup;						//0x080
	uint32 ofsTexLookup;					//0x084

	uint32 nTexUnitLookup;					//0x088
	uint32 ofsTexUnitLookup;				//0x08c
	uint32 nTransparencyLookup; 			//0x090
	uint32 ofsTransparencyLookup;			//0x094
	uint32 nTexAnimLookup;					//0x098
	uint32 ofsTexAnimLookup;				//0x09c

	float floats[14];						//0x0a0

	uint32 nBoundingTriangles;				//0x0d8
	uint32 ofsBoundingTriangles;			//0x0dc
	uint32 nBoundingVertices;				//0x0e0
	uint32 ofsBoundingVertices;				//0x0e4
	uint32 nBoundingNormals;				//0x0e8
	uint32 ofsBoundingNormals;				//0x0ec

	uint32 nAttachments; 					//0x0f0
	uint32 ofsAttachments;					//0x0f4
	uint32 nAttachLookup; 					//0x0f8
	uint32 ofsAttachLookup;					//0x0fc
	uint32 nEvents; 					//0x100 Events
	uint32 ofsEvents;				//0x104
	uint32 nLights; 						//0x108
	uint32 ofsLights;						//0x10c
	uint32 nCameras; 						//0x110
	uint32 ofsCameras;						//0x114
	uint32 nCameraLookup;					//0x118
	uint32 ofsCameraLookup;					//0x11c
	uint32 nRibbonEmitters; 				//0x120
	uint32 ofsRibbonEmitters;				//0x124
	uint32 nParticleEmitters; 				//0x128
	uint32 ofsParticleEmitters;				//0x12c

} LKModelHeader;

typedef struct LKModelAnimation {
	int16 animID;
	int16 subAnimID;
	uint32 length;
	float moveSpeed;
	uint32 flags;
	int16 probability;
	int16 unused;
	uint32 d1;
	uint32 d2;
	uint32 playSpeed;				//BlendTime
	Vec3D boxA;				//MinimumExtent
	Vec3D boxB;				//MaximumExtent
	float rad;				//BoundsRadius
	int16 NextAnimation;
	uint16 Index;				//aliasNext
} LKModelAnimation;

typedef struct LKAnimationBlock {
	int16 type;
	int16 seq;
	ArrayRef Times;  //links to i ArraysRefs of j uint32
	ArrayRef Keys;  //links to i ArrayRefs of j elements
} LKAnimationBlock;

typedef struct FakeAnimBlock {
	ArrayRef Times;  //links to i short (!)
	ArrayRef Keys;  //links to i elements
} FakeAnimBlock;

//SubBlocks
typedef struct Vec3D_LKSubBlock {
	uint32 *times;
	Vec3D *keys;
} Vec3D_LKSubBlock;

typedef struct Quat_LKSubBlock {
	uint32 *times;
	Quat *keys;
} Quat_LKSubBlock;

typedef struct Short_LKSubBlock {
	uint32 *times;
	short *keys;
} Short_LKSubBlock;

typedef struct Char_LKSubBlock {
	uint32 *times;
	char *keys;
} Char_LKSubBlock;

typedef struct Int_LKSubBlock {
	uint32 *times;
	int *keys;
} Int_LKSubBlock;

typedef struct BigFloat_LKSubBlock {
	uint32 *times;
	BigFloat *keys;
} BigFloat_LKSubBlock;

typedef struct Float_LKSubBlock {
	uint32 *times;
	float *keys;
} Float_LKSubBlock;

typedef struct AnimRefs {
	ArrayRef *times;
	ArrayRef *keys;
} AnimRefs;

//SubBlocks for FakeAnimBlocks
typedef struct Vec3D_FSubBlock {
	short *times;
	Vec3D *keys;
} Vec3D_FSubBlock;
typedef struct Vec2D_FSubBlock {
	short *times;
	Vec2D *keys;
} Vec2D_FSubBlock;
typedef struct Short_FSubBlock {
	short *times;
	short *keys;
} Short_FSubBlock;

//Bones
typedef struct LKModelBoneDef {
	int32 animid;  //KeyBoneID
	uint32 flags;
	int16 parent;
	uint16 geoid;  //SubmeshID
	int32 unk;

	//Offsets link to a new nX ofsX
	LKAnimationBlock trans;
	LKAnimationBlock rot;
	LKAnimationBlock scal;

	Vec3D pivot;
} LKModelBoneDef;
typedef struct RefBlock {
	AnimRefs trans;
	AnimRefs rot;
	AnimRefs scal;
} RefBlock;
typedef struct LKBonesDataBlock {
	Vec3D_LKSubBlock *trans;
	Quat_LKSubBlock *rot;
	Vec3D_LKSubBlock *scal;
} LKBonesDataBlock;

//Attachments
typedef struct LKAttachment {
	uint32 ID;
	uint32 bone;
	Vec3D position;
	LKAnimationBlock data;
} LKAttachment;
typedef struct AttachmentsRefBlock {
	AnimRefs data;
} AttachmentsRefBlock;
typedef struct LKAttachmentsDataBlock {
	Char_LKSubBlock *data;
} LKAttachmentsDataBlock;

//Cameras
typedef struct LKCamera {
	uint32 Type;
	float FOV;
	float farClipping;
	float nearClipping;
	LKAnimationBlock transpos;
	Vec3D position;
	LKAnimationBlock transtar;
	Vec3D target;
	LKAnimationBlock scal;
} LKCamera;
typedef struct CamerasRefBlock {
	AnimRefs transpos;
	AnimRefs transtar;
	AnimRefs scal;
} CamerasRefBlock;
typedef struct LKCamerasDataBlock {
	BigFloat_LKSubBlock *transpos;
	BigFloat_LKSubBlock *transtar;
	Vec3D_LKSubBlock *scal;
} LKCamerasDataBlock;

//Colors
typedef struct LKColorDef {
	LKAnimationBlock rgb;
	LKAnimationBlock opacity;
} LKColorDef;
typedef struct ColorRefBlock {
	AnimRefs rgb;
	AnimRefs opacity;
} ColorRefBlock;
typedef struct LKColorDataBlock {
	Vec3D_LKSubBlock *rgb;
	Short_LKSubBlock *opacity;
} LKColorDataBlock;

//Transparency
typedef struct LKTransparency {
	LKAnimationBlock alpha;
} LKTransparency;
typedef struct TransparencyRefBlock {
	AnimRefs alpha;
} TransparencyRefBlock;
typedef struct LKTransparencyDataBlock {
	Short_LKSubBlock *alpha;
} LKTransparencyDataBlock;

//Lights
typedef struct LKLight {
	uint16 ID;
	uint16 bone;
	Vec3D position;
	LKAnimationBlock a_color;  //Ambient
	LKAnimationBlock a_intensity;
	LKAnimationBlock d_color;  //Diffuse
	LKAnimationBlock d_intensity;
	LKAnimationBlock a_start;  //Attenuation
	LKAnimationBlock a_end;
	LKAnimationBlock unknown;
} LKLight;
typedef struct LightsRefBlock {
	AnimRefs a_color;  //Ambient
	AnimRefs a_intensity;
	AnimRefs d_color;  //Diffuse
	AnimRefs d_intensity;
	AnimRefs a_start;  //Attenuation
	AnimRefs a_end;
	AnimRefs unknown;
} LightsRefBlock;
typedef struct LKLightsDataBlock {
	Vec3D_LKSubBlock *a_color;  //Ambient
	Float_LKSubBlock *a_intensity;
	Vec3D_LKSubBlock *d_color;  //Diffuse
	Float_LKSubBlock *d_intensity;
	Float_LKSubBlock *a_start;  //Attenuation
	Float_LKSubBlock *a_end;
	Int_LKSubBlock *unknown;
} LKLightsDataBlock;

struct LKParticle {
	uint32 unknown;  //0x000
	uint32 flags;  //0x004
	Vec3D pos;  //0x008
	uint16 bone;  //0x014
	uint16 texture;  //0x016
	uint32 lenModelFilename;  //0x018
	uint32 ofsModelFilename;  //0x01C
	uint32 lenParticleFilename;  //0x020
	uint32 ofsParticleFilename;  //0x024
	char BlendingType;  //0x028
	char EmitterType;  //0x029
	short ParticleColorIndex;  //0x02A
	char ParticleType;  //0x02C
	char HeadorTail;  //0x02D
	short TextureTileRotation;  //0x02E
	short TextureRows;  //0x030
	short TextureCols;  //0x032
	LKAnimationBlock emissionspeed;
	LKAnimationBlock speedvariation;
	LKAnimationBlock verticalrange;
	LKAnimationBlock horizontalrange;
	LKAnimationBlock gravity;
	LKAnimationBlock lifespan;
	int unknown1;
	LKAnimationBlock emissionrate;
	int unknown2;
	LKAnimationBlock emissionarealength;
	LKAnimationBlock emissionareawidth;
	LKAnimationBlock gravity2;
	FakeAnimBlock color;
	FakeAnimBlock opacity;
	FakeAnimBlock size;
	int unknownfields[2];
	FakeAnimBlock intensity;
	FakeAnimBlock unknownblock;
	Vec3D unk;
	Vec3D scales;
	float slowdown;
	Vec2D unknown3;
	float rotation;
	Vec2D unknown4;
	Vec3D Rot1;
	Vec3D Rot2;
	Vec3D Trans;
	float f2[4];
	int nUnknownReference;
	int ofsUnknownReference;
	LKAnimationBlock enabled;
} LKParticle;
typedef struct ParticlesRefBlock {
	AnimRefs emissionspeed;
	AnimRefs speedvariation;
	AnimRefs verticalrange;
	AnimRefs horizontalrange;
	AnimRefs gravity;
	AnimRefs lifespan;
	AnimRefs emissionrate;
	AnimRefs emissionarealength;
	AnimRefs emissionareawidth;
	AnimRefs gravity2;
	AnimRefs enabled;
} ParticlesRefBlock;
typedef struct LKParticlesDataBlock {
	char *ModelFilename;
	char *ParticleFilename;
	Float_LKSubBlock *emissionspeed;
	Float_LKSubBlock *speedvariation;
	Float_LKSubBlock *verticalrange;
	Float_LKSubBlock *horizontalrange;
	Float_LKSubBlock *gravity;
	Float_LKSubBlock *lifespan;
	Float_LKSubBlock *emissionrate;
	Float_LKSubBlock *emissionarealength;
	Float_LKSubBlock *emissionareawidth;
	Float_LKSubBlock *gravity2;
	Vec3D_FSubBlock color;
	Short_FSubBlock opacity;
	Vec2D_FSubBlock size;
	Short_FSubBlock intensity;
	Short_FSubBlock unknownblock;
	Vec3D UnknownReference;
	Char_LKSubBlock *enabled;
} LKParticlesDataBlock;

typedef struct SkinHeader {
	uint32 ID;
	uint32 nIndices;
	uint32 ofsIndices;
	uint32 nTriangles;
	uint32 ofsTriangles;
	uint32 nProperties;
	uint32 ofsProperties;
	uint32 nSubmeshes;
	uint32 ofsSubmeshes;
	uint32 nTextureUnits;
	uint32 ofsTextureUnits;
	uint32 LOD;
} SkinHeader;

typedef struct LKSubmesh {
	uint16 ID;
	uint16 Level; //level*(2^16)+StartThing
	uint16 StartVertex;
	uint16 nVertices;
	uint16 StartTriangle;
	uint16 nTriangles;
	uint16 nBones;
	uint16 StartBones;
	uint16 boneInfluences;
	uint16 RootBone;
	Vec3D CenterMass;
	Vec3D CenterBoundingBox;
	float Radius;
} LKSubmesh;

typedef struct LKTextureAnimation {
	LKAnimationBlock trans;
	LKAnimationBlock rot;
	LKAnimationBlock scal;
} LKTextureAnimation;

//Events
typedef struct EventsRefBlock {
	ArrayRef *times;
} EventsRefBlock;
typedef struct LKEventsDataBlock {
	uint32 **times;
} LKEventsDataBlock;
typedef struct LKEventAnimBlock {
	int16 type;
	int16 seq;
	ArrayRef Times;  //links to i ArraysRefs of j uint32
} LKEventAnimBlock;
typedef struct LKEvent {
	char ID[4];
	uint32 data;
	uint32 bone;
	Vec3D position;
	LKEventAnimBlock timer;
} LKEvent;

//FILES
/**
 * Structure of a M2/WotLK model
 */
typedef struct LKM2 {
	LKModelHeader header;
	char *filename;
	unsigned int *globalsequences;
	LKModelAnimation *animations;
	int16 *AnimLookup;

	LKModelBoneDef *bones;
	RefBlock *animofs; //bones layer 1
	LKBonesDataBlock *bonesdata; //bones layer 2

	short *keybonelookup;
	ModelVertex *vertices;

	LKColorDef *colors;
	ColorRefBlock *coloranimofs;
	LKColorDataBlock *colorsdata;

	ModelTextureDef *textures_def;
	char **texture_names;

	LKTransparency *transparencyrefs;
	TransparencyRefBlock *transparencyanimofs;
	LKTransparencyDataBlock *transparencydata;

	LKTextureAnimation *texanims;
	RefBlock *texanimofs;
	LKBonesDataBlock *texdata; //bones layer 2

	short *TexReplace;
	int *renderflags;
	int16 *BoneLookupTable;
	short *TexLookupTable;
	short *TexUnit;
	short *TransparencyLookup;
	short *TexAnimLookup;
	Triangle *BoundingTriangles;
	Vec3D *BoundingVertices;
	Vec3D *BoundingNormals;

	LKLight *lights;
	LightsRefBlock *lightsanimofs;
	LKLightsDataBlock *lightsdata;

	LKAttachment *attachments;
	AttachmentsRefBlock *attachmentsanimofs;
	LKAttachmentsDataBlock *attachmentsdata;
	int16 *AttachLookup;

	LKEvent *events;
	EventsRefBlock *eventsanimofs;
	LKEventsDataBlock *eventsdata;

	LKCamera *cameras;
	CamerasRefBlock *camerasanimofs;
	LKCamerasDataBlock *camerasdata;
	uint16 *CameraLookup;
/*TODO
 * RibbonEmitters, ParticleEmmiters.
 */
} LKM2;

/**
 * Structure of Skin/WotLK resources
 */
typedef struct Skin {
	SkinHeader header;
	Vertex *Indices;
	Triangle* Triangles;
	Property *Properties;
	LKSubmesh *Submeshes;
	TexUnit *TextureUnits;
} Skin;
#endif
