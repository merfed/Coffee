#pragma once

// a lot of helpful information:
// http://madx.dk/wowdev/wiki/index.php?title=M2/WotLK
// http://wowdev.org/wiki/index.php/M2

#include "BaseTypes.h"
#include "M2Types.h"
#include "M2I.h"
#include "M2Element.h"
#include "M2Skin.h"

#define DegreesToRadians 0.0174532925f

namespace M2Lib
{


	// load, export, import merge, save: M2 file.
	class M2
	{
	public:
		static const UInt8 VersionCode = 0x10;
		static const UInt32 Signature_M2I0 = MakeFourCC('M', '2', 'I', '0');

	public:
		//
		// points to an animation located outside of the element.
		struct SExternalAnimation
		{
		public:
			UInt32 Count;	// number of timestamps or keys.
			UInt32 Offset;	// global offset in bytes from start of file to where actual animation data begins.
			//UInt32 Unknown1;
			//UInt32 Unknown2;
		};

		// named indices.
		enum EElement
		{
			EElement_Name,
			EElement_GlobalSequence,
			EElement_Animation,
			EElement_AnimationLookup,
			EElement_Bone,
			EElement_KeyBoneLookup,
			EElement_Vertex,
			EElement_Skin,
			EElement_Color,
			EElement_Texture,
			EElement_Transparency,
			EElement_TextureAnimation,
			EElement_TextureReplace,
			EElement_TextureFlags,
			EElement_SkinnedBoneLookup,
			EElement_TextureLookup,
			EElement_TextureUnitLookup,
			EElement_TransparencyLookup,
			EElement_TextureAnimationLookup,
			EElement_BoundingTriangle,
			EElement_BoundingVertex,
			EElement_BoundingNormal,
			EElement_Attachment,
			EElement_AttachmentLookup,
			EElement_Event,
			EElement_Light,
			EElement_Camera,
			EElement_CameraLookup,
			EElement_RibbonEmitter,
			EElement_ParticleEmitter,
			EElement_Unknown1,

			EElement__Count__
		};

#pragma pack(push,1)
		// memory mapped header.
		class CM2Header
		{
		public:
			struct
			{
				Char8 ID[4];	// 'MDL0'
				UInt8 Version[4];

				UInt32 nName;
				UInt32 oName;

				UInt32 Flags;
			} Description;

			struct
			{
				UInt32 nGlobalSequence;
				UInt32 oGlobalSequence;

				UInt32 nAnimation;
				UInt32 oAnimation;

				UInt32 nAnimationLookup;
				UInt32 oAnimationLookup;

				UInt32 nBone;
				UInt32 oBone;

				UInt32 nKeyBoneLookup;
				UInt32 oKeyBoneLookup;

				UInt32 nVertex;
				UInt32 oVertex;

				UInt32 nSkin;

				UInt32 nColor;
				UInt32 oColor;

				UInt32 nTexture;
				UInt32 oTexture;

				UInt32 nTransparency;
				UInt32 oTransparency;

				UInt32 nTextureAnimation;
				UInt32 oTextureAnimation;

				UInt32 nTextureReplace;
				UInt32 oTextureReplace;

				UInt32 nTextureFlags;
				UInt32 oTextureFlags;

				UInt32 nSkinnedBoneLookup;
				UInt32 oSkinnedBoneLookup;

				UInt32 nTextureLookup;
				UInt32 oTextureLookup;

				UInt32 nTextureUnitLookup;
				UInt32 oTextureUnitLookup;

				UInt32 nTransparencyLookup;
				UInt32 oTransparencyLookup;

				UInt32 nTextureAnimationLookup;
				UInt32 oTextureAnimationLookup;

				SVolume CollisionVolume;
				SVolume BoundingVolume;

				UInt32 nBoundingTriangle;
				UInt32 oBoundingTriangle;

				UInt32 nBoundingVertex;
				UInt32 oBoundingVertex;

				UInt32 nBoundingNormal;
				UInt32 oBoundingNormal;

				UInt32 nAttachment;
				UInt32 oAttachment;

				UInt32 nAttachmentLookup;
				UInt32 oAttachmentLookup;

				UInt32 nEvent;
				UInt32 oEvent;

				UInt32 nLight;
				UInt32 oLight;

				UInt32 nCamera;
				UInt32 oCamera;

				UInt32 nCameraLookup;
				UInt32 oCameraLookup;

				UInt32 nRibbonEmitter;
				UInt32 oRibbonEmitter;

				UInt32 nParticleEmitter;
				UInt32 oParticleEmitter;

				UInt32 nUnknown1;
				UInt32 oUnknown1;
			} Elements;
		};


		//
		//
		class CElement_Name
		{
		public:
			Char8* szName;

		public:
			CElement_Name()
			{
				szName = 0;
			}

			~CElement_Name()
			{
				if (szName)
				{
					delete[] szName;
				}
			}
		};


		//
		//
		class CElement_GlobalSequence
		{
		public:
			UInt32 Value;
		};

		class CElement_AnimationLookup
		{
		public:
			UInt16 AnimationID;
		};


		//
		// some sort of animation entry (without any apparent referenced animation data).
		class CElement_Animation
		{
		public:
			UInt16 AnimationID;
			UInt16 AnimationSubID;
			UInt32 Length;

			Float32 MoveSpeed;

			UInt32 Flags;

			SInt16 Rarity;

			UInt16 d1;
			UInt32 d2;
			UInt32 d3;

			UInt32 PlaySpeed;

			SVolume BoundingVolume;

			SInt16 NextAnimation;		// index to next animation with same AnimationID, -1 if there are no more.
			UInt16 TriangleIndex;				// this animation's index in the list of animations.
		};


		//
		// generic animation block header.
		class CElement_AnimationBlock
		{
		public:
			// an animation will reference several of these, and each of these in turn corresponds to a bone (my best guess).
			class CChannel
			{
			public:
				UInt32 n;	// number
				UInt32 o;	// offset
			};

			// used for times?
			struct SKey_UInt32
			{
				UInt32 Values[1];
			};

			// used for position and scale keys.
			struct SKey_Float32x3
			{
				Float32 Values[3];
			};

			// used for rotation quaternion keys.
			struct SKey_SInt16x4
			{
				UInt16 Values[4];
			};

			// used for visibility, probably interpreted as boolean.
			struct SKey_UInt16
			{
				UInt16 Values[1];
			};

			// used for light intensity, etc.
			struct SKey_Float32
			{
				Float32 Values[1];
			};

			enum EInterpolationType
				: UInt16
			{
				EInterpolationType_None = 0,
				EInterpolationType_Linear = 1,
				EInterpolationType_Hermite = 2,
			};

		public:
			EInterpolationType InterpolationType;
			SInt16 GlobalSequenceID;
			UInt32 nTimes;
			UInt32 oTimes;
			UInt32 nKeys;
			UInt32 oKeys;

		};


		//
		// a list of these makes up a skeleton.
		class CElement_Bone
		{
		public:
			enum EFlags
				: UInt32
			{
				EFlags_Transform = 8,
				EFlags_Bilboard = 512,
			};

		public:
			UInt32 BoneLookupID;		// index into BoneLookup table or -1.
			EFlags Flags;				//
			SInt16 ParentBone;			// index to parent bone or -1.
			UInt16 Unknown[3];			// ?
			CElement_AnimationBlock AnimationBlock_Position;	// Float32x3
			CElement_AnimationBlock AnimationBlock_Rotation;	// SInt16x4
			CElement_AnimationBlock AnimationBlock_Scale;		// Float32x3
			Float32 Position[3];		//
		};


		//
		//
		class CElement_BoneLookup
		{
		public:
			enum EBoneLookup
				: SInt16
			{
				EBoneLookup_ArmL,
				EBoneLookup_ArmR,
				EBoneLookup_ShoulderL,
				EBoneLookup_ShoulderR,
				EBoneLookup_SpineLow,
				EBoneLookup_Waist,
				EBoneLookup_Head,
				EBoneLookup_Jaw,
				EBoneLookup_TriangleIndexFingerR,
				EBoneLookup_MiddleFingerR,
				EBoneLookup_PinkyFingerR,
				EBoneLookup_RingFingerR,
				EBoneLookup_ThumbR,
				EBoneLookup_TriangleIndexFingerL,
				EBoneLookup_MiddleFingerL,
				EBoneLookup_PinkyFingerL,
				EBoneLookup_RingFingerL,
				EBoneLookup_ThumbL,
				EBoneLookup_Event_BTH,
				EBoneLookup_Event_CSR,
				EBoneLookup_Event_CSL,
				EBoneLookup__Breath,
				EBoneLookup__Name,
				EBoneLookup__NameMount,
				EBoneLookup_Event_CHD,
				EBoneLookup_Event_CCH,
				EBoneLookup_Root,
				EBoneLookup_Wheel1,
				EBoneLookup_Wheel2,
				EBoneLookup_Wheel3,
				EBoneLookup_Wheel4,
				EBoneLookup_Wheel5,
				EBoneLookup_Wheel6,
				EBoneLookup_Wheel7,
				EBoneLookup_Wheel8,
			};

		public:
			EBoneLookup BoneLookup;
		};


		//
		//
		class CElement_Vertex
		{
		public:
			Float32 Position[3];
			UInt8 BoneWeights[4];
			UInt8 BoneIndices[4];
			Float32 Normal[3];
			Float32 Texture[2];
			Float32 Padding[2];
		};


		//
		//
		class CElement_Color
		{
		public:
			CElement_AnimationBlock AnimationBlock_Color;		// Float32x3
			CElement_AnimationBlock AnimationBlock_Opacity;		// UInt16
		};


		//
		//
		class CElement_Texture
		{
		public:
			UInt32 Type;
			UInt32 Flags;
			UInt32 nTexturePath;	// length of name in characters.
			UInt32 oTexturePath;	// position of name in file.
		};


		//
		//
		class CElement_Transparency
		{
		public:
			CElement_AnimationBlock AnimationBlock_Transparency;	// UInt16

		};


		//
		// texture coordinate animation block.
		class CElement_UVAnimation
		{
		public:
			CElement_AnimationBlock AnimationBlock_Position;	// Float32x3
			CElement_AnimationBlock AnimationBlock_Rotation;	// SInt16x4
			CElement_AnimationBlock AnimationBlock_Scale;		// Float32x3
		};


		//
		//
		class CElement_TextureReplace
		{
		public:
			enum ETexture
				: SInt16
			{
				ETexture_Final_Hardcoded,
				ETexture_Body_Clothes,
				ETexture_Unk1,
				ETexture_Unk2,
				ETexture_Unk3,
				ETexture_Unk4,
				ETexture_Hair_Beard,
				ETexture_Unk5,
				ETexture_TaurenFur,
				ETexture_Unk6,
				ETexture_Unk7,
				ETexture_CreatureSkin1,
				ETexture_CreatureSkin2,
				ETexture_CreatureSkin3,
			};

		public:
			SInt16 TextureID;
		};


		//
		//
		class CElement_TextureFlag
		{
		public:
			enum EFlags
				: UInt16
			{
				EFlags_Unlit = 0x01,
				EFlags_Unfogged = 0x02,
				EFlags_TwoSided = 0x04,
				EFlags_Billboard = 0x08,
				EFlags_NoZBuffer = 0x10,
			};

			enum EBlend
			{
				EBlend_Opaque,
				EBlend_Mod,
				EBlend_Decal,
				EBlend_Add,
				EBlend_Mod2x,
				EBlend_Fade,
				EBlend_Unknown7,
			};

		public:
			EFlags Flags;
			EBlend Blend;
		};


		//
		// each skin file has one or more bone partitions. each bone partition references into a subset of this skinned bone lookup array.
		class CElement_PartitionedBoneLookup
		{
		public:
			UInt16 BoneIndex;			// index into the model's bone list.
		};


		//
		//
		class CElement_TextureLookup
		{
		public:
			UInt16 TextureIndex;
		};


		//
		//
		class CElement_TextureUnits
		{
		public:
			UInt16 Unit;
		};


		//
		//
		class CElement_TransparencyLookup
		{
		public:
			UInt16 TransparencyLookup;
		};


		//
		//
		class CElement_UVAnimationLookup
		{
		public:
			UInt16 TextureIndex;
		};


		//
		//
		class CElement_BoundingTriangle
		{
		public:
			UInt16 Indices[3];
		};


		//
		//
		class CElement_BoundingVertices
		{
		public:
			Float32 Position[3];
		};


		//
		//
		class CElement_BoundingNormals
		{
		public:
			Float32 Normal[3];
		};


		//
		// an attachment to a bone defines where items and effects will anchor to the rig.
		class CElement_Attachment
		{
		public:
			enum EID
				: UInt32
			{
				EID_WristL = 0,		// (shield)
				EID_PalmR = 1,
				EID_PalmL = 2,
				EID_ElbowR = 3,
				EID_ElbowL = 4,
				EID_ShoulderR = 5,
				EID_ShoulderL = 6,
				EID_KneeR = 7,
				EID_KneeL = 8,
				EID_Unknown9 = 9,
				EID_Unknown10 = 10,
				EID_Helmet = 11,
				EID_Back = 12,
				EID_Unknown13 = 13,
				EID_Unknown14 = 14,
				EID_Bust1 = 15,
				EID_Bust2 = 16,
				EID_Face = 17,
				EID_AboveCharacter = 18,
				EID_Ground = 19,
				EID_TopOfHead = 20,
				EID_PalmL2 = 21,
				EID_PalmR2 = 22,
				EID_Unknown23 = 23,
				EID_Unknown24 = 24,
				EID_Unknown25 = 25,
				EID_BackSheathR = 26,
				EID_BackSheathL = 27,
				EID_BackSheathM = 28,
				EID_Belly = 29,
				EID_BackL = 30,
				EID_BackR = 31,
				EID_HipSheathL = 32,
				EID_HipSheathR = 33,
				EID_Bust3 = 34,
				EID_PalmR3 = 35,
			};

		public:
			UInt32 ID;					//
			UInt32 ParentBone;			// parent bone.
			Float32 Position[3];		// position relative to parent bone.
			CElement_AnimationBlock AnimationBlock_Visibility;	// UInt16.
		};


		//
		//
		class CElement_AttachmentLookup
		{
		public:
			enum EAttachmentLookup
			{
				EAttachmentLookup_Item_PalmR,
				EAttachmentLookup_Item_PalmL,
				EAttachmentLookup_Item_ElbowR,
				EAttachmentLookup_Item_ElbowL,
				EAttachmentLookup_Item_ShoulderR,
				EAttachmentLookup_Item_ShoulderL,
				EAttachmentLookup_Item_KneeR,
				EAttachmentLookup_Item_KneeL,
				EAttachmentLookup_Item_HipR,
				EAttachmentLookup_Item_HipL,
				EAttachmentLookup_Item_Helmet,
				EAttachmentLookup_Item_Back,
				EAttachmentLookup_Item_ShoulderExtR,
				EAttachmentLookup_Item_ShoulderExtL,
				EAttachmentLookup_Hit_Front,
				EAttachmentLookup_Hit_Back,
				EAttachmentLookup_Spell_Mouth,
				EAttachmentLookup_Spell_NearHead,
				EAttachmentLookup_Spell_Base,
				EAttachmentLookup_Spell_Head,
				EAttachmentLookup_Spell_Precast1L,
				EAttachmentLookup_Spell_Precast1R,
				EAttachmentLookup_Spell_Precast2L,
				EAttachmentLookup_Spell_Precast2R,
				EAttachmentLookup_Spell_Precast3,
				EAttachmentLookup_Sheath_UpBackR,
				EAttachmentLookup_Sheath_UpBackL,
				EAttachmentLookup_Sheath_CenterBack,
				EAttachmentLookup_Sheath_HipBelly,
				EAttachmentLookup_Sheath_UpDownBack,
				EAttachmentLookup_Sheath_UnknownBack,
				EAttachmentLookup_Sheath_HipL,
				EAttachmentLookup_Sheath_HipR,
				EAttachmentLookup_Sheath_Chest,
				EAttachmentLookup_Unknown_PalmR1,
				EAttachmentLookup_Unknown_PalmR2,
			};

		public:
			EAttachmentLookup AttachmentLookup;

		};


		//
		// events define sounds that are played during animation.
		class CElement_Event
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
			UInt32 SoundID;			// database id of sound to play from SoundEntries.dbc.
			UInt32 ParentBone;		// parent bone.
			Float32 Position[3];	// position relative to parent bone.
			UInt16 InterpolationType;
			SInt16 GlobalSequenceID;
			UInt32 nTimeLines;
			UInt32 oTimeLines;
			//CElement_AnimationBlock AnimationBlock_Visibility;	// SKey_UInt16
		};


		//
		//
		class CElement_Light
		{
		public:
			UInt16 Type;
			UInt16 ParentBone;
			Float32 Position[3];
			CElement_AnimationBlock AnimationBlock_AmbientColor;		// Float32x3
			CElement_AnimationBlock AnimationBlock_AmbientIntensity;	// Float32
			CElement_AnimationBlock AnimationBlock_DiffuseColor;		// Float32x3
			CElement_AnimationBlock AnimationBlock_DiffuseIntensity;	// Float32
			CElement_AnimationBlock AnimationBlock_AttenuationStart;	// Float32
			CElement_AnimationBlock AnimationBlock_AttenuationEnd;		// Float32
			CElement_AnimationBlock AnimationBlock_Visibility;			// UInt16
		};


		//
		//
		class CElement_Camera
		{
		public:
			SInt32 Type;
			//Float32 FieldOfView;	// v3
			Float32 ClipFar;
			Float32 ClipNear;
			CElement_AnimationBlock AnimationBlock_Position;		// Float32x3
			Float32 Position[3];
			CElement_AnimationBlock AnimationBlock_Target;			// Float32x3
			Float32 Target[3];
			CElement_AnimationBlock AnimationBlock_Roll;			// Float32
			CElement_AnimationBlock AnimationBlock_FieldOfView;		// Float32	// v4
		};


		//
		//
		class CElement_CameraLookup
		{
		public:
			UInt16 TriangleIndex;
		};


		//
		//
		class CElement_RibbonEmitter
		{
		public:
			SInt32 ID;
			SInt32 ParentBone;
			Float32 Position[3];
			UInt32 nTexture;
			UInt32 oTexture;
			UInt32 nRenderFlag;
			UInt32 oRenderFlag;
			CElement_AnimationBlock AnimationBlock_Color;		// Float32x3
			CElement_AnimationBlock AnimationBlock_Opacity;		// UInt16
			CElement_AnimationBlock AnimationBlock_Above;		// Float32, position of point A of ribbon leading edge.
			CElement_AnimationBlock AnimationBlock_Below;		// Float32, position of point B of ribbon leading edge.
			Float32 Resolution;
			Float32 Length;
			Float32 EmissionAngle;		// use arcsin(val) to get the angle in degree.
			UInt16 s1;		// ?
			UInt16 s2;		// ?
			CElement_AnimationBlock AnimationBlock_Unknown1;	// SInt32
			CElement_AnimationBlock AnimationBlock_Visibility;	// SInt16
		};


		//
		//
		class CElement_ParticleEmitter
		{
		public:
			UInt32 ID;				// always -1?
			UInt32 Flags;
			Float32 Position[3];	// position relative to parent bone.
			UInt16 ParentBone;
			UInt16 Texture;
			UInt32 nFileNameModel;	//
			UInt32 oFileNameModel;	// name of model to spawn *.mdx.
			UInt32 nUnknown;
			UInt32 oUnknown;
			UInt16 BlendMode;
			UInt16 EmitterType;
			UInt16 ParticleType;
			UInt16 Rotation;
			UInt16 Columns;
			UInt16 Rows;

			CElement_AnimationBlock AnimationBlock_EmitSpeed;		// Float32
			CElement_AnimationBlock AnimationBlock_SpeedVariance;	// Float32
			CElement_AnimationBlock AnimationBlock_Spread;			// Float32
			CElement_AnimationBlock AnimationBlock_Unknown;			// Float32
			CElement_AnimationBlock AnimationBlock_Gravity;			// Float32
			CElement_AnimationBlock AnimationBlock_Lifespan;		// Float32
			CElement_AnimationBlock AnimationBlock_EmitRate;		// Float32
			CElement_AnimationBlock AnimationBlock_EmitLength;		// Float32
			CElement_AnimationBlock AnimationBlock_EmitWidth;		// Float32
			CElement_AnimationBlock AnimationBlock_GravityStrong;	// Float32

			struct
			{
				Float32 Mid;
				UInt32 Colors[3];
				Float32 Sizes[3];
				UInt16 Indices[10];
				Float32 Unknown[3];
				Float32 Scales[3];
				Float32 Slowdown;
				Float32 Rotation;
				Float32 f2[16];
			} Attributes;

			CElement_AnimationBlock AnimationBlock_Visibility;		// UInt16
		};
#pragma pack(pop)


	public:
		Char16 _FileName[1024];	// needed to create skin file names so we can load/save skins.

		CM2Header Header;		// used for loading and saving. not used when editing.
		M2Element Elements[EElement__Count__];
		M2Skin* Skins[4];

		UInt32 m_OriginalSize;	// size in bytes of the original M2 file when loaded.

		UInt8* RawData;			// the entire file read into memory, so we can access animations.

		bool ImportedM2I;

		M2Element Element_NewVertex;	//
		M2Element& GetElement_Vertex()
		{
			if (Element_NewVertex.Count)
			{
				return Element_NewVertex;
			}
			else
			{
				return Elements[EElement_Vertex];
			}
		}

	public:
		M2()
		{
			Skins[0] = 0;
			Skins[1] = 0;
			Skins[2] = 0;
			Skins[3] = 0;
			m_OriginalSize = 0;
			RawData = 0;
			ImportedM2I = false;
		}

		~M2()
		{
		}


	public:
		// loads an M2 from a file.
		M2Lib::EError Load(const Char16* FileName);
		// saves this M2 to a file.
		M2Lib::EError Save(const Char16* FileName);

		// exports the loaded M2 as an M2I file.
		M2Lib::EError ExportM2Intermediate(Char16* FileName);
		// imports an M2I file and merges it with already loaded M2.
		M2Lib::EError ImportM2Intermediate(Char16* FileName, bool IgnoreBones, bool IgnoreAttachments, bool IgnoreCameras);

		// prints diagnostic information.
		void PrintInfo();

		// szResult is a buffer large enough to store result, which on windows means it should be at least MAX_PATH long, which is 260.
		bool GetFileSkin(Char16* SkinFileNameResultBuffer, const Char16* M2FileName, UInt32 SkinTriangleIndex);


	public:
		// utilities and tests

		// averages normals of duplicate vertices within submeshes.
		void FixSeamsSubMesh(Float32 PositionalTolerance, Float32 AngularTolerance);
		// averages normals of duplicate vertices between body submeshes.
		void FixSeamsBody(Float32 PositionalTolerance, Float32 AngularTolerance);
		// closes small gaps between clothing and body and copies normals from body to clothing.
		void FixSeamsClothing(Float32 PositionalTolerance, Float32 AngularTolerance);

		// makes sure per vertex bone weigts sum up to 0 or 255.
		void NormalizeBoneWeights();
		// scales all positional coordinates of vertices, bones, attachments, events, lights, cameras, ribbon emitters, particle emitters.
		void Scale(Float32 Scale);
		// mirrors the portrait camera accross the Y axis
		void MirrorCamera();

		UInt32 AddTexture(const Char8* szTextureSource, UInt16 Type, UInt16 Flags);
		UInt32 AddTextureFlags(UInt16 Blend, UInt32 Flags);

		void MakeShiny(const Char8* szTextureSource);

	public:
		// post load header
		void m_LoadElements_CopyHeaderToElements();
		void m_LoadElements_FindSizes(UInt32 FileSize);

		// pre save header
		void m_SaveElements_FindOffsets();
		//void m_FixAnimationOffsets( SInt32 OffsetDelta, SInt32 OffsetDeltaEnd, UInt32 OriginalEnd, CAnimationBlock& AnimationBlock, SInt32 iElement, UInt32 NewElementOffset );
		void m_FixAnimationOffsets(UInt32 OriginalEnd, SInt32 OffsetDelta, CElement_AnimationBlock& AnimationBlock, SInt32 iElement, bool Post);
		void m_SaveElements_CopyElementsToHeader();

	};


}
