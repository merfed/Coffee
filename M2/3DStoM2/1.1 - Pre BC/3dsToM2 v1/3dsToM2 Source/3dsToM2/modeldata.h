#include <string>
#include "modelheaders.h"


#ifndef MODELDATA
#define MODELDATA


struct ModelBoneData {
	int32 animid;
	int32 flags;
	int16 parent; // parent bone index
	int16 geoid;
	AnimationBlock translation;
	AnimationBlock rotation;
	AnimationBlock scaling;
	Vec3D pivot;
};

struct ModelData {
	std::string	modelName;
	uint32	modelType;
	uint32	nGlobalSequences;
	uint32	*dGlobalSequences;
	uint32	nAnimations;
	ModelAnimation	*dAnimations;
	uint32	nC;
	uint32	*dC;
	uint32	nD;
	uint32	*dD;
	uint32	nBones;
	
};

#endif