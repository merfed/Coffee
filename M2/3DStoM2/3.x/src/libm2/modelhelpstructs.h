/*
 * modelhelpstructs.h
 *
 *  Created on: 11.06.2009
 *      Author: Bastian
 */

#ifndef MODELHELPSTRUCTS_H_
#define MODELHELPSTRUCTS_H_
#include "vec3d.h"
#include "quaternion.h"
#include "trace.h"

template<class T>
struct AnimHelper {
	uint32 timestamp;
	T value;
};

template<class T>
struct AnimBlockHelper {
	uint32 nValues;
	AnimHelper<T> *help;
	/*
	 * this is for bones&&*.anim-files only
	 */
	bool inAnim;
	uint32 ofsTime;
	uint32 ofsKey;

};

template<class T>
struct AnimationHelper {
	int16 Interpolation;
	int16 GlobalSequence;
	uint32 nAnims;
	AnimBlockHelper<T> *helper;
};

//used for fake animblocks

template<class T>
struct FakeHelper {
	short timestamp;
	T value;
};

class ParticleInfo {
public:
	/*
	 * FakeAnimBlocks don't change between animations
	 */
	FakeHelper<Vec3D> *Color;
	int nColor;
	FakeHelper<short> *Opacity;
	int nOpacity;
	FakeHelper<Vec2D> *Size;
	int nSize;
	FakeHelper<short> *Intensity;
	int nIntensity;
	FakeHelper<short> *Unk;
	int nUnk;

	int lengthModelName;
	char *ModelName;
	int lengthParticleName;
	char *ParticleName;
	//I just include this for completness
	//now idea what it's used for <.<
	int *ref3_not_used;

	ParticleInfo() {

	}
	~ParticleInfo() {

	}

	void setColor(FakeHelper<Vec3D> *t, int nSubs) {
		nColor = nSubs;
		Color = t;
	}
	void setOpacity(FakeHelper<short> *t, int nSubs) {
		nOpacity = nSubs;
		Opacity = t;
	}
	void setSize(FakeHelper<Vec2D> *t, int nSubs) {
		nSize = nSubs;
		Size = t;
	}
	void setIntensity(FakeHelper<short> *t, int nSubs) {
		nIntensity = nSubs;
		Intensity = t;
	}
	void setUnk(FakeHelper<short> *t, int nSubs) {
		nUnk = nSubs;
		Unk = t;
	}

	void setModelName(int length, char*filename) {
		lengthModelName = length;
		ModelName = filename;
	}

	void setParticleName(int length, char*filename) {
		lengthParticleName = length;
		ParticleName = filename;
	}

};

class RibbonHelper {
public:
	int nRibbons;
	ModelRibbonEmitterDef *ribbons;
	AnimationHelper<Vec3D> *Color;
	AnimationHelper<short> *Opacity;
	AnimationHelper<float> *Above;
	AnimationHelper<float> *Below;
	AnimationHelper<short> *Unk1;
	AnimationHelper<bool> *Enabled;

	int **TexRefs;
	int **unkRefs;

	RibbonHelper() {

	}

	~RibbonHelper() {

	}

	void setnRibbons(int n) {
		nRibbons = n;
		ribbons = new ModelRibbonEmitterDef[n];
		Color = new AnimationHelper<Vec3D> [n];
		Opacity = new AnimationHelper<short> [n];
		Above = new AnimationHelper<float> [n];
		Below = new AnimationHelper<float> [n];
		Unk1 = new AnimationHelper<short> [n];
		Enabled = new AnimationHelper<bool> [n];
		TexRefs=new int*[n];
		unkRefs=new int*[n];
	}

};

class ParticleHelper {
public:
	AnimationHelper<float> *Emissionspeed;
	AnimationHelper<float> *SpeedVariation;
	AnimationHelper<float> *VerticalRange;
	AnimationHelper<float> *HorizontalRange;
	AnimationHelper<float> *Gravity;
	AnimationHelper<float> *Lifespan;
	AnimationHelper<float> *EmissionRate;
	AnimationHelper<float> *EmissionAreaLength;
	AnimationHelper<float> *EmissionAreaWidth;
	AnimationHelper<float> *Gravity2;
	AnimationHelper<bool> *Enabled;

	ModelParticleEmitterDef *particles;
	int nParticles;
	ParticleInfo *info;
	ParticleHelper() {
		nParticles = 0;
		particles = new ModelParticleEmitterDef[0];
	}
	~ParticleHelper() {

	}

	void setnParticles(int n) {
		nParticles = n;
		Emissionspeed = new AnimationHelper<float> [n];
		SpeedVariation = new AnimationHelper<float> [n];
		VerticalRange = new AnimationHelper<float> [n];
		HorizontalRange = new AnimationHelper<float> [n];
		Gravity = new AnimationHelper<float> [n];
		Lifespan = new AnimationHelper<float> [n];
		EmissionRate = new AnimationHelper<float> [n];
		EmissionAreaLength = new AnimationHelper<float> [n];
		EmissionAreaWidth = new AnimationHelper<float> [n];
		Gravity2 = new AnimationHelper<float> [n];
		Enabled = new AnimationHelper<bool> [n];
		particles = new ModelParticleEmitterDef[n];
		info = new ParticleInfo[n];
	}
};

class BoneHelper {
public:
	bool *inAnim;
	ModelBoneDef *Bones;
	int nBones;
	AnimationHelper<Vec3D> *translation;
	AnimationHelper<SQuaternion> *rotation;
	AnimationHelper<Vec3D> *scaling;

	BoneHelper() {
		nBones = 0;
		Bones = new ModelBoneDef[nBones];
		inAnim = new bool[0];
	}
	~BoneHelper() {

	}
	void setnAnims(int n) {
		inAnim = new bool[n];
	}
	void setnBones(int n) {
		nBones = n;
		Bones = new ModelBoneDef[nBones];
		translation = new AnimationHelper<Vec3D> [n];
		rotation = new AnimationHelper<SQuaternion> [n];
		scaling = new AnimationHelper<Vec3D> [n];
	}

};

class TextureHelper {
public:
	int nTextures;
	ModelTextureDef *texdefs;
	char **Texnames;
	TextureHelper() {
		nTextures = 0;
		texdefs = new ModelTextureDef[0];
		Texnames = new char*[0];
	}
	~TextureHelper() {

	}
	void setnTextures(int n) {
		nTextures = n;
		texdefs = new ModelTextureDef[n];
		Texnames = new char*[n];
	}

	void addTexture(ModelTextureDef tex) {
		ModelTextureDef *temp = texdefs;
		char **ctemp = Texnames;
		texdefs = new ModelTextureDef[nTextures + 1];
		Texnames = new char*[nTextures + 1];
		for (int i = 0; i < nTextures; i++) {
			texdefs[i] = temp[i];
			Texnames[i] = ctemp[i];
		}
		texdefs[nTextures] = tex;
		Texnames[nTextures] = new char[tex.nameLen];
		nTextures++;
	}

	void addTexture(ModelTextureDef tex, char*texname) {
		ModelTextureDef *temp = texdefs;
		char **ctemp = Texnames;
		texdefs = new ModelTextureDef[nTextures + 1];
		Texnames = new char*[nTextures + 1];
		for (int i = 0; i < nTextures; i++) {
			texdefs[i] = temp[i];
			Texnames[i] = ctemp[i];
		}
		texdefs[nTextures] = tex;
		Texnames[nTextures] = texname;
		nTextures++;
	}

	void setTexture(ModelTextureDef tex, int n) {
		texdefs[n] = tex;
		Texnames[n] = new char[texdefs[n].nameLen];
	}
};

class ColorHelper {
public:
	int nColors;
	AnimationHelper<Vec3D> *color;
	AnimationHelper<short> *opacity;

	ColorHelper() {

	}
	~ColorHelper() {

	}
	void setnColors(int n) {
		color = new AnimationHelper<Vec3D> [n];
		opacity = new AnimationHelper<short> [n];
		nColors = n;
	}
};

class TexAnimHelper {
public:
	int nTexAnims;
	AnimationHelper<Vec3D> *trans;
	AnimationHelper<SQuaternion> *rot;
	AnimationHelper<Vec3D> *scaling;
	ModelTexAnimDef *TexAnims;
	TexAnimHelper() {
		nTexAnims = 0;
		TexAnims = new ModelTexAnimDef[0];
	}
	~TexAnimHelper() {

	}

	void setnTexAnims(int n) {
		nTexAnims = n;
		trans = new AnimationHelper<Vec3D> [n];
		rot = new AnimationHelper<SQuaternion> [n];
		scaling = new AnimationHelper<Vec3D> [n];
		TexAnims = new ModelTexAnimDef[n];
	}
};

class LightHelper {
public:
	int nLights;
	ModelLightDef *lights;
	AnimationHelper<Vec3D> *AmbientColor;
	AnimationHelper<float> *AmbientIntensity;
	AnimationHelper<Vec3D> *DiffuseColor;
	AnimationHelper<float> *DiffuseIntensity;
	AnimationHelper<float> *AttenuationStart;
	AnimationHelper<float> *AttenuationEnd;
	AnimationHelper<bool> *Enabled;

	LightHelper() {

	}

	~LightHelper() {

	}

	void setnLights(int n) {
		nLights = n;
		lights = new ModelLightDef[n];
		AmbientColor = new AnimationHelper<Vec3D> [n];
		AmbientIntensity = new AnimationHelper<float> [n];
		DiffuseColor = new AnimationHelper<Vec3D> [n];
		DiffuseIntensity = new AnimationHelper<float> [n];
		AttenuationStart = new AnimationHelper<float> [n];
		AttenuationEnd = new AnimationHelper<float> [n];
		Enabled = new AnimationHelper<bool> [n];
	}

};

class CamHelper {
public:
	int nCameras;

	AnimationHelper<Vec3D> *translationpos;
	AnimationHelper<Vec3D> *translationtar;
	AnimationHelper<Vec3D> *rotation;
	ModelCameraDef *camera;
	CamHelper() {
		nCameras = 0;
		camera = new ModelCameraDef[0];
	}
	~CamHelper() {

	}
	void setnCameras(int n) {
		translationpos = new AnimationHelper<Vec3D> [n];
		translationtar = new AnimationHelper<Vec3D> [n];
		rotation = new AnimationHelper<Vec3D> [n];
		camera = new ModelCameraDef[n];
		nCameras = n;
	}

};

class BoundVolumeHelper {
public:
	int nBTriangles;
	ModelBoundTriangle* BTriangles;
	int nBVertices;
	Vec3D* BVertices;
	int nBNormals;
	Vec3D* BNormals;
	BoundVolumeHelper() {

	}
	~BoundVolumeHelper() {

	}
};

#endif /* MODELHELPSTRUCTS_H_ */
