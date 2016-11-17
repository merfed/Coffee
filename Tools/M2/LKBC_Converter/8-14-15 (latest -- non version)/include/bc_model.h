/**
 * @file
 * BC M2 structures
 */

#ifndef BC_MODEL_H
#define BC_MODEL_H

#include "common.h"

typedef struct PlayAnimRecord {
	int16 ID;
	int16 flags;
} PlayAnimRecord;

typedef struct ModelHeader {
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
	uint32 nPlayableAnimationLookup;		//0x02c
	uint32 ofsPlayableAnimationLookup;		//0x030
	uint32 nBones;							//0x034
	uint32 ofsBones;						//0x038
	uint32 nKeyBoneLookup;					//0x03c
	uint32 ofsKeyBoneLookup;				//0x040

	uint32 nVertices;						//0x044
	uint32 ofsVertices;						//0x048
	uint32 nViews;							//0x04c
	uint32 ofsViews;						//0x050

	uint32 nColors;							//0x054
	uint32 ofsColors;						//0x058

	uint32 nTextures;						//0x05c
	uint32 ofsTextures;						//0x060

	uint32 nTransparency; 					//0x064
	uint32 ofsTransparency;					//0x068
	uint32 nI;   							//0x06c
	uint32 ofsI;							//0x070
	uint32 nTexAnims;						//0x074
	uint32 ofsTexAnims;						//0x078
	uint32 nTexReplace;						//0x07c
	uint32 ofsTexReplace;					//0x080

	uint32 nRenderFlags;					//0x084
	uint32 ofsRenderFlags;					//0x088
	uint32 nBoneLookupTable;				//0x08c
	uint32 ofsBoneLookupTable;				//0x090

	uint32 nTexLookup;						//0x094
	uint32 ofsTexLookup;					//0x098

	uint32 nTexUnitLookup;					//0x09c
	uint32 ofsTexUnitLookup;				//0x0a0
	uint32 nTransparencyLookup; 			//0x0a4
	uint32 ofsTransparencyLookup;			//0x0a8
	uint32 nTexAnimLookup;					//0x0ac
	uint32 ofsTexAnimLookup;				//0x0b0

	float floats[14];						//0x0b4

	uint32 nBoundingTriangles;				//0x0ec
	uint32 ofsBoundingTriangles;			//0x0f0
	uint32 nBoundingVertices;				//0x0f4
	uint32 ofsBoundingVertices;				//0x0f8
	uint32 nBoundingNormals;				//0x0fc
	uint32 ofsBoundingNormals;				//0x100

	uint32 nAttachments; 					//0x104
	uint32 ofsAttachments;					//0x108
	uint32 nAttachLookup; 					//0x10c
	uint32 ofsAttachLookup;					//0x110
	uint32 nEvents; 					//0x114
	uint32 ofsEvents;				//0x118
	uint32 nLights; 						//0x11c
	uint32 ofsLights;						//0x120
	uint32 nCameras; 						//0x124
	uint32 ofsCameras;						//0x128
	uint32 nCameraLookup;					//0x12c
	uint32 ofsCameraLookup;					//0x130
	uint32 nRibbonEmitters; 				//0x134
	uint32 ofsRibbonEmitters;				//0x138
	uint32 nParticleEmitters; 				//0x13c
	uint32 ofsParticleEmitters;				//0x140
} ModelHeader;

typedef struct ModelAnimation {
	int16 animID;
	int16 subAnimID;
	uint32 timeStart;
	uint32 timeEnd;
	float moveSpeed;
	uint32 flags;				//LoopType
	uint16 probability;
	uint16 unused;
	uint32 d1;
	uint32 d2;
	uint32 playSpeed;
	Vec3D boxA;
	Vec3D boxB;
	float rad;
	int16 NextAnimation;
	uint16 Index;
} ModelAnimation;

typedef struct PlayableAnimationLookup {
	int16 FallbackAnimationID;  //In AnimationData.DBC
	int16 Flags;
} PlayableAnimationLookup;

typedef struct AnimationBlock {
	int16 type;
	int16 seq;
	ArrayRef Ranges;  //links to i interpolations ranges of type (int,int)
	ArrayRef Times;  //links to i uint32
	ArrayRef Keys;  //links to i elements
} AnimationBlock;

//SubBlocks
typedef struct Vec3D_SubBlock {
	Range *ranges;
	uint32 *times;
	Vec3D *keys;
} Vec3D_SubBlock;
typedef struct Quat_SubBlock {
	Range *ranges;
	uint32 *times;
	Quat *keys;
} Quat_SubBlock;
typedef struct Short_SubBlock {
	Range *ranges;
	uint32 *times;
	short *keys;
} Short_SubBlock;
typedef struct Char_SubBlock {
	Range *ranges;
	uint32 *times;
	char *keys;
} Char_SubBlock;
typedef struct Int_SubBlock {
	Range *ranges;
	uint32 *times;
	int *keys;
} Int_SubBlock;
typedef struct BigFloat_SubBlock {
	Range *ranges;
	uint32 *times;
	BigFloat *keys;
} BigFloat_SubBlock;
typedef struct Float_SubBlock {
	Range *ranges;
	uint32 *times;
	float *keys;
} Float_SubBlock;

//Bones
typedef struct ModelBoneDef {
	int32 animid;
	uint32 flags;
	int16 parent;
	uint16 geoid;
	int32 unk;

	AnimationBlock trans;
	AnimationBlock rot;
	AnimationBlock scal;

	Vec3D pivot;
} ModelBoneDef;
typedef struct CLModelBoneDef {
	int32 animid;
	uint32 flags;
	int16 parent;
	uint16 geoid;

	AnimationBlock trans;
	AnimationBlock rot;
	AnimationBlock scal;

	Vec3D pivot;
} CLModelBoneDef;
typedef struct BonesDataBlock {
	Vec3D_SubBlock trans;
	Quat_SubBlock rot;
	Vec3D_SubBlock scal;
} BonesDataBlock;

//Attachments
typedef struct Attachment {
	uint32 ID;
	uint32 bone;
	Vec3D position;
	AnimationBlock data;
} Attachment;
typedef struct AttachmentsDataBlock {
	Char_SubBlock data;
} AttachmentsDataBlock;

//Cameras
typedef struct Camera {
	uint32 Type;
	float FOV;
	float farClipping;
	float nearClipping;
	AnimationBlock transpos;
	Vec3D position;
	AnimationBlock transtar;
	Vec3D target;
	AnimationBlock scal;
} Camera;
typedef struct CamerasDataBlock {
	BigFloat_SubBlock transpos;
	BigFloat_SubBlock transtar;
	Vec3D_SubBlock scal;
} CamerasDataBlock;

//Colors
typedef struct ColorDef {
	AnimationBlock rgb;
	AnimationBlock opacity;
} ColorDef;
typedef struct ColorDataBlock {
	Vec3D_SubBlock rgb;
	Short_SubBlock opacity;
} ColorDataBlock;

//Transparency
typedef struct Transparency {
	AnimationBlock alpha;
} Transparency;
typedef struct TransparencyDataBlock {
	Short_SubBlock alpha;
} TransparencyDataBlock;

//Lights
typedef struct Light {
	uint16 ID;
	uint16 bone;
	Vec3D position;
	AnimationBlock a_color;  //Ambient
	AnimationBlock a_intensity;
	AnimationBlock d_color;  //Diffuse
	AnimationBlock d_intensity;
	AnimationBlock a_start;  //Attenuation
	AnimationBlock a_end;
	AnimationBlock unknown;
} Light;
typedef struct LightsDataBlock {
	Vec3D_SubBlock a_color;  //Ambient
	Float_SubBlock a_intensity;
	Vec3D_SubBlock d_color;  //Diffuse
	Float_SubBlock d_intensity;
	Float_SubBlock a_start;  //Attenuation
	Float_SubBlock a_end;
	Int_SubBlock unknown;
} LightsDataBlock;

struct Particle {
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
	AnimationBlock emissionspeed;//0x034
	AnimationBlock speedvariation;
	AnimationBlock verticalrange;
	AnimationBlock horizontalrange;
	AnimationBlock gravity;
	AnimationBlock lifespan;
	AnimationBlock emissionrate;
	AnimationBlock emissionarealength;
	AnimationBlock emissionareawidth;
	AnimationBlock gravity2;

	int					midPoint;//0x14C OK
	unsigned char		colors[12];//ARGB colors 0x150
	int					size[3];//0x15C
	short				tiles[10];//0x168

	Vec3D unk;//0x17C OK
	Vec3D scales;//0x188 OK
	float slowdown;//0x194 OK
	//Vec2D unknown3;
	float rotation;//0x198 OK
	Vec2D unknown4;//0x19C
	Vec3D Rot1;//0x1A0
	Vec3D Rot2;//0x1AC
	Vec3D Trans;//0x1B8
	float f2[4];//0x1C4
	int nUnknownReference;//0x1D4
	int ofsUnknownReference;//0x1D8
	AnimationBlock enabled;//0x1DC OK
} Particle;
typedef struct ParticlesDataBlock {//TODO Implement it
	Vec3D_SubBlock a_color;  //Ambient
	Float_SubBlock a_intensity;
	Vec3D_SubBlock d_color;  //Diffuse
	Float_SubBlock d_intensity;
	Float_SubBlock a_start;  //Attenuation
	Float_SubBlock a_end;
	Int_SubBlock unknown;
} ParticlesDataBlock;

typedef struct ViewsHeader {
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
} ViewsHeader;

typedef struct Submesh {
	uint16 ID;
	uint16 Level; //EXPERIMENTAL : No mention of this in the Wiki, but the LK level might have always been there.
	uint16 StartVertex;
	uint16 nVertices;
	uint16 StartTriangle;
	uint16 nTriangles;
	uint16 nBones;
	uint16 StartBones;
	uint16 boneInfluences; //Amount of bones up the parent-chain affecting the submesh
	uint16 RootBone;
	Vec3D Position;
	float Floats[4];
} Submesh;
typedef struct CLSubmesh {
	uint16 ID;
	uint16 Level; //EXPERIMENTAL : No mention of this in the Wiki, but the LK level might have always been there.
	uint16 StartVertex;
	uint16 nVertices;
	uint16 StartTriangle;
	uint16 nTriangles;
	uint16 nBones;
	uint16 StartBones;
	uint16 boneInfluences; //Amount of bones up the parent-chain affecting the submesh
	uint16 RootBone;
	Vec3D Position;
} CLSubmesh;

typedef struct TextureAnimation {
	AnimationBlock trans;
	AnimationBlock rot;
	AnimationBlock scal;
} TextureAnimation;

typedef struct View { //Only present in 2.x models. Replaced by Skin files in 3.x
	ViewsHeader header;
	Vertex *Indices;
	Triangle* Triangles;
	Property *Properties;
	Submesh *Submeshes;
	TexUnit *TextureUnits;
} View;

//Events
typedef struct EventsDataBlock {
	Range *ranges;
	uint32 *times;
} EventsDataBlock;
typedef struct EventAnimBlock {
	int16 type;
	int16 seq;
	ArrayRef Ranges;  //links to i interpolations ranges of type (int,int)
	ArrayRef Times;  //links to i uint32
} EventAnimBlock;
typedef struct Event {
	char ID[4];
	uint32 data;
	uint32 bone;
	Vec3D position;
	EventAnimBlock timer;
} Event;

//FILES
/**
 * Structure of a M2/BC model
 */
typedef struct BCM2 {
	ModelHeader header;
	char *filename;
	unsigned int *globalsequences;
	ModelAnimation *animations;
	int16 *AnimLookup;
	PlayAnimRecord *PlayAnimLookup;

	ModelBoneDef *bones;
	BonesDataBlock *bonesdata;

	short *keybonelookup; //Skeletal Bone Lookup
	ModelVertex *vertices;
	View *views;

	ColorDef *colors;
	ColorDataBlock *colorsdata;

	ModelTextureDef *textures_def;
	char **texture_names;

	Transparency *transparencyrefs;
	TransparencyDataBlock *transparencydata;

	TextureAnimation *texanims;
	BonesDataBlock *texdata;

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

	Light *lights;
	LightsDataBlock *lightsdata;

	Attachment *attachments;
	AttachmentsDataBlock *attachmentsdata;
	int16 *AttachLookup;

	Event *events;
	EventsDataBlock *eventsdata;

	Camera *cameras;
	CamerasDataBlock *camerasdata;
	uint16 *CameraLookup;
/*TODO
 * RibbonEmitters, ParticleEmmiters.
 */
} BCM2;

#endif
