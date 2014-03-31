#pragma once

// a lot of helpful information:
// http://madx.dk/wowdev/wiki/index.php?title=M2/WotLK
// http://wowdev.org/wiki/index.php/M2

#include "BaseTypes.h"
#include "EError.h"
#include "CM2I.h"
#include "CM2Element.h"
#include "CM2Skin.h"
#include "M2Types.h"



namespace M2Lib
{




// load, export, import merge, save: M2 file.
class CM2
{
public:
	// named indices.
	enum EElement
	{
		EElement_Name,
		EElement_GlobalSequences,
		EElement_Animations,
		EElement_AnimationsLookup,
		EElement_Bones,
		EElement_KeyBonesLookup,
		EElement_Vertices,
		EElement_Skins,
		EElement_Colors,
		EElement_Textures,
		EElement_Transparencies,
		EElement_TextureAnimations,
		EElement_TextureReplace,
		EElement_TextureFlags,
		EElement_BonesLookup,
		EElement_TexturesLookup,
		EElement_TextureUnitsLookup,
		EElement_TransparenciesLookup,
		EElement_TextureAnimationsLookup,
		EElement_BoundingTriangles,
		EElement_BoundingVertices,
		EElement_BoundingNormals,
		EElement_Attachments,
		EElement_AttachmentsLookup,
		EElement_Events,
		EElement_Lights,
		EElement_Cameras,
		EElement_CamerasLookup,
		EElement_RibbonEmitters,
		EElement_ParticleEmitters,
		EElement_Unknown1,

		EElement__Count__
	};

	#pragma pack(push,1)
	// memory mapped header.
	class CM2Header
	{
	public:
		Char8 ID[4];	// 'MDL0'
		UInt8 Version[4];

		UInt32 nName;
		UInt32 oName;

		UInt32 Type;

		UInt32 nGlobalSequences;
		UInt32 oGlobalSequences;

		UInt32 nAnimations;
		UInt32 oAnimations;

		UInt32 nAnimationsLookup;
		UInt32 oAnimationsLookup;

		UInt32 nBones;
		UInt32 oBones;

		UInt32 nKeyBonesLookup;
		UInt32 oKeyBonesLookup;

		UInt32 nVertices;
		UInt32 oVertices;

		UInt32 nViews;

		UInt32 nColors;
		UInt32 oColors;

		UInt32 nTextures;
		UInt32 oTextures;

		UInt32 nTransparencies;
		UInt32 oTransparencies;

		UInt32 nTextureAnimations;
		UInt32 oTextureAnimations;

		UInt32 nTextureReplace;
		UInt32 oTextureReplace;

		UInt32 nTextureFlags;
		UInt32 oTextureFlags;

		UInt32 nBonesLookup;
		UInt32 oBonesLookup;

		UInt32 nTexturesLookup;
		UInt32 oTexturesLookup;

		UInt32 nTextureUnitsLookup;
		UInt32 oTextureUnitsLookup;

		UInt32 nTransparenciesLookup;
		UInt32 oTransparenciesLookup;

		UInt32 nTextureAnimationsLookup;
		UInt32 oTextureAnimationsLookup;

		Float32 UnknownFloats[14];

		UInt32 nBoundingTriangles;
		UInt32 oBoundingTriangles;

		UInt32 nBoundingVertices;
		UInt32 oBoundingVertices;

		UInt32 nBoundingNormals;
		UInt32 oBoundingNormals;

		UInt32 nAttachments;
		UInt32 oAttachments;

		UInt32 nAttachmentsLookup;
		UInt32 oAttachmentsLookup;

		UInt32 bEvents;
		UInt32 oEvents;

		UInt32 nLights;
		UInt32 oLights;

		UInt32 nCameras;
		UInt32 oCameras;

		UInt32 nCamerasLookup;
		UInt32 oCamersaLookup;

		UInt32 nRibbonEmitters;
		UInt32 oRibbonEmitters;

		UInt32 nParticleEmitters;
		UInt32 oParticleEmitters;

		UInt32 nUnknown1;
		UInt32 oUnknown1;
	};

	// some sort of animation entry (without any apparent referenced animation data).
	class CAnimation
	{
	public:
		UInt32 AnimationID;
		UInt32 AnimationSubID;
		UInt32 Length;

		Float32 MoveSpeed;

		UInt32 LoopType;			// ? 1 for non-looping
		UInt32 Flags;				// ? Flags
		UInt32 Unknown0;			// ?
		UInt32 Unknown1;			// ? 0
		UInt32 PlaySpeed;

		Float32 BoundingBox[2][3];	//
		Float32 BoundingRadius;		//

		UInt16 NextAnimation;		// index to next animation with same AnimationID, -1 if there are no more.
		UInt16 Index;				// this animation's index in the list of animations.
	};

	// generic animation block header.
	class CAnimationBlock
	{
	public:
		class CIndirectPair
		{
		public:
			UInt32 n;	// number
			UInt32 o;	// offset
		};

		struct SKeyFloat3
		{
			Float32 Values[3];
		};

		struct SKeyFloat4
		{
			Float32 Values[4];
		};

	public:
		enum EInterpolationType
			: UInt16
		{
			EInterpolationType_None = 0,
			EInterpolationType_Linear = 1,
			EInterpolationType_Hermite = 2,
		};

	public:
		EInterpolationType InterpolationType;
		UInt16 GlobalSequenceID;
		UInt32 nTimes;		
		UInt32 oTimes;
		UInt32 nKeys;
		UInt32 oKeys;
	};

	// a list of these makes up a skeleton.
	class CBone
	{
	public:
		enum EFlag
			: UInt32
		{
			EFlag_Transform = 8,
			EFlag_Bilboard = 512,
		};

	public:
		UInt32 BoneLookupID;		// index into BoneLookup table or -1.
		EFlag Flags;				// 
		SInt16 ParentBone;			// index to parent bone or -1.
		UInt16 Unknown[3];			// ?
		CAnimationBlock AnimationBlock_Position;	// each key is a (Flaot32, Float32, Float32)
		CAnimationBlock AnimationBlock_Rotation;	// each key is a (Flaot32, Float32, Float32, Float32) or (SInt16, SInt16, SInt16, SInt16)
		CAnimationBlock AnimationBlock_Scale;		// each key is a (Flaot32, Float32, Float32)
		Float32 Position[3];			// 
	};

	//// vertices are in this format. defined externally because it's used elsewhere.
	//typedef M2Lib::CVertex CVertex;

	// 
	class CColor
	{
	public:
		CAnimationBlock AnimationBlock_Color;
		CAnimationBlock AnimationBlock_Opacity;
	};

	// 
	class CTexture
	{
	public:
		UInt32 Type;
		UInt32 Flags;
		UInt32 nName;	// length of name in characters.
		UInt32 oName;	// position of name in file.
	};

	//
	class CRenderFlag
	{
	public:
		UInt16 A;
		UInt16 B;
	};

	//
	class CTransparency
	{
	public:
		CAnimationBlock AnimationBlock_Transparency;

	};

	// an attachment to a bone defines where items and effects will anchor to the rig.
	class CAttachment
	{
	public:
		UInt32 ID;					// 
		SInt16 ParentBone;			// parent bone.
		SInt16 Padding;				// filler?
		Float32 Position[3];		// position relative to parent bone.
		CAnimationBlock AnimationBlock_Animation;	// some sort of animation data.
	};

	// events define sounds that are played during animation.
	class CEvent
	{
	public:
		class CTimeLine
		{
		public:
			UInt32 nTimeStamps;
			UInt32 oTimeStamps;
		};

	public:
		Char8 ID[4];			// this event's ID.
		UInt32 Data;			// database id of sound to play from SoundEntries.dbc.
		UInt32 ParentBone;		// parent bone.
		Float32 Position[3];	// position relative to parent bone.

		UInt16 InterpolationType;	// 
		UInt16 GlobalSequence;		//
		UInt32 nTimeLines;	//
		UInt32 oTimeLines;	//
	};

	//
	class CLight
	{
	public:
		UInt16 Type;
		UInt16 ParentBone;
		Float32 Position[3];
		CAnimationBlock AnimationBlock_AmbientColor;
		CAnimationBlock AnimationBlock_AmbientIntensity;
		CAnimationBlock AnimationBlock_DiffuseColor;
		CAnimationBlock AnimationBlock_DiffuseIntensity;
		CAnimationBlock AnimationBlock_AttenuationStart;
		CAnimationBlock AnimationBlock_AttenuationEnd;
		CAnimationBlock AnimationBlock_Unknown;
	};

	typedef M2Lib::CVertex CVertex;

	//
	class CCamera
	{
	public:
		SInt32 Type;
		Float32 FieldOfView;
		Float32 ClipFar;
		Float32 ClipNear;
		CAnimationBlock AnimationBlock_Position;
		Float32 Position[3];
		CAnimationBlock AnimationBlock_Target;
		Float32 Target[3];
		CAnimationBlock AnimationBlock_Roll;
	};

	//
	class CRibbonEmitter
	{
	public:
		UInt32 ID;
		UInt32 ParentBone;
		Float32 Position[3];
		UInt32 nTextures;
		UInt32 oTextures;
		UInt32 nBlend;		// ?
		UInt32 oBlend;		// ?
		CAnimationBlock AnimationBlock_Color;
		CAnimationBlock AnimationBlock_Opacity;
		CAnimationBlock AnimationBlock_Above;	// position of point A of ribbon leading edge.
		CAnimationBlock AnimationBlock_Below;	// position of point B of ribbon leading edge.
		Float32 Resolution;
		Float32 Length;
		Float32 EmissionAngle;					// use arcsin(val) to get the angle in degree.
		UInt16 iRenderFlags[2];	// ?
		CAnimationBlock AnimationBlock_Unknown1;
		CAnimationBlock AnimationBlock_Unknown2;
	};

	//
	class CParticleEmitter
	{
	public:
		//
		class CParameters
		{
		public:
			Float32 Mid;
			UInt32 Colors[3];
			Float32 Sizes[3];
			SInt16 Unknown0[10];
			Float32 Unknown1[3];
			Float32 Scales[3];
			Float32 Dampening;	// rate of slow down.
			Float32 Rotation;	// sprite rotation.
			Float32 Unknown2;
			Float32 Rotation1[3];	// model rotation 1
			Float32 Rotation2[3];	// model rotation 2
			Float32 Translation[3];	// model translation
			Float32 Unknown3[6];
		};

	public:
		UInt32 ID;		// always -1?
		UInt32 Flags;
		Float32 Position[3];	// position relative to parent bone.
		UInt16 ParentBone;
		UInt16 Texture;
		UInt32 nFileNameModel;	// 
		UInt32 oFileNameModel;	// name of model to spawn *.mdx.
		UInt32 nZero2;
		UInt32 oZero2;
		UInt16 Blend;
		UInt16 Type;
		UInt16 S1;
		UInt16 S2;
		UInt16 Columns;
		UInt16 Rows;
		CAnimationBlock AnimationBlock_Blocks[10];
		CParameters Parameters;
		CAnimationBlock AnimationBlock_Enable;
	};
	#pragma pack(pop)


	
public:
	Char8 m_szFile[1024];	// needed to create skin file names so we can load/save skins.

	CM2Header Header;		// used for loading and saving. not used when editing.
	CM2Element Elements[EElement__Count__];
	CM2Skin* Skins[4];

	UInt32 m_OriginalSize;	// size in bytes of the original M2 file when loaded.


public:
	// M2I import

	// M2I file signature, first 4 bytes of file.
	static const UInt32 M2ISignature = 0x00000000;


public:
	CM2()
	{
		Skins[0] = 0;
		Skins[1] = 0;
		Skins[2] = 0;
		Skins[3] = 0;
		m_OriginalSize = 0;
	}

	~CM2()
	{
	}


public:
	M2Lib::EError Load( Char8* szFile );
	M2Lib::EError Save( Char8* szFile );

	M2Lib::EError ExportM2Intermediate( Char8* szFile );
	M2Lib::EError ImportM2Intermediate( Char8* szFile, bool IgnoreExtras, Float32 SubmeshPositionalTolerance, Float32 SubmeshAngularTolerance, Float32 BodyPositionalTolerance, Float32 BodyAngularTolerance, Float32 ClothingPositionalTolerance, Float32 ClothingAngularTolerance );

	void PrintInfo();

	// szResult is a buffer large enough to store result, which on windows means it should be at least MAX_PATH long, which is 260.
	bool GetFileSkin( Char8* szResult, Char8* szFileM2, UInt32 SkinIndex );


public:
	// utilities and tests

	// builds vertex properties for each skin. these vertex properties bone indexes into the skin's bone lookup list, which is a slice of the global bone lookup list in the M2.
	void BuildVertexBoneIndices();
	// build max bones for each skin.
	void BuildMaxBones();
	// averages normals of duplicate vertices to smooth seams between subsets.
	//void FixNormals();
	//static void m_SmoothNormals( std::vector< M2Lib::CM2Skin::CSubset* >& SubMesh, M2Lib::CVertex* Vertices, UInt16* Indices );

	// averages normals of duplicate vertices within submeshes.
	void FixSeamsSubmesh( Float32 PositionalTolerance, Float32 AngularTolerance );
	// averages normals of duplicate vertices between body submeshes.
	void FixSeamsBody( Float32 PositionalTolerance, Float32 AngularTolerance );
	// closes small gaps between clothing and body and copies normals from body to clothing.
	void FixSeamsClothing( Float32 PositionalTolerance, Float32 AngularTolerance );

	// makes sure per vertex bone weigts sum up to 0 or 255.
	void NormalizeBoneWeights();
	// scales all positional coordinates of vertices, bones, attachments, events, lights, cameras, ribbon emitters, particle emitters.
	void Scale( Float32 Scale );
	// experimental.
	void ScaleAnimation( M2Lib::CM2Element* pElement, CAnimationBlock* pAnimationBlock, Float32 Scale );
	// mirrors the portrait camera accross the Y axis
	void MirrorCamera();


public:
	// post load header
	void m_LoadElements_CopyHeaderToElements();
	void m_LoadElements_FindSizes( UInt32 FileSize );

	// pre save header
	void m_SaveElements_FindOffsets();
	//void m_FixAnimationOffsets( SInt32 OffsetDelta, SInt32 OffsetDeltaEnd, UInt32 OriginalEnd, CAnimationBlock& AnimationBlock, SInt32 iElement, UInt32 NewElementOffset );
	void m_FixAnimationOffsets( UInt32 OriginalEnd, SInt32 OffsetDelta, CAnimationBlock& AnimationBlock, SInt32 iElement, bool Post );
	void m_SaveElements_CopyElementsToHeader();

};




}
