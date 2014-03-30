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

struct RibbonHelper{
	int texture;
	int blend;
	float color[3];
	unsigned short opacity;
	float above,below;
	int zero,one;
};

/**
 * ToDo:Replace Quaternion with ShortVersion...
 */
struct QuatHelper{
	uint32 timestamp;
	SQuaternion value;
};

struct FloatHelper{
	uint32 timestamp;
	float value;
};

struct Vec2Helper{
	uint32 timestamp;
	Vec2D value;
};

struct Vec3Helper{
	uint32 timestamp;
	Vec3D value;
};

struct ShortHelper{
	uint32 timestamp;
	short value;
};

struct IntHelper{
	uint32 timestamp;
	uint32 value;
};

struct BoolHelper{
	uint32 timestamp;
	char value;
};

//used for fake animblocks
struct Vec2Helper_f{
	short timestamp;
	Vec2D value;
};

struct Vec3Helper_f{
	short timestamp;
	Vec3D value;
};

struct ShortHelper_f{
	short timestamp;
	short value;
};

class BoneInfo{
public:
	bool hasData[3];
	int nSubstructs[3];
	Vec3Helper *trans;
	QuatHelper *rot;
	Vec3Helper *scale;
	BoneInfo(){
		hasData[0]=false;hasData[1]=false;hasData[2]=false;
		nSubstructs[0]=0;nSubstructs[1]=0;nSubstructs[2]=0;
		rot=new QuatHelper[0];
		trans=new Vec3Helper[0];
		scale=new Vec3Helper[0];
	}
	~BoneInfo(){

	}
	void setTranslation(Vec3Helper *t,int nSubs){
		hasData[0]=true;
		nSubstructs[0]=nSubs;
		trans=t;
	}
	void setRotation(QuatHelper *r,int nSubs){
		hasData[1]=true;
		nSubstructs[1]=nSubs;
		rot=r;
	}
	void setScaling(Vec3Helper *s,int nSubs){
		hasData[2]=true;
		nSubstructs[2]=nSubs;
		scale=s;
	}

};

class ParticleInfo{
public:
	//well, the only block I've seen changing values until
	//now is [enabled], and as me ez lazy...
	//I only add the more than one anim to enabled only
	//so if you want changing things between anims
	//use more than one particle
	FloatHelper **Emissionspeed;
	int *nEmissionspeed;
	FloatHelper **SpeedVariation;
	int *nSpeedVariation;
	FloatHelper **VerticalRange;
	int *nVerticalRange;
	FloatHelper **HorizontalRange;
	int *nHorizontalRange;
	FloatHelper **Gravity;
	int *nGravity;
	FloatHelper **Lifespan;
	int *nLifespan;
	FloatHelper **EmissionRate;
	int *nEmissionRate;
	FloatHelper **EmissionAreaLength;
	int *nEmissionAreaLength;
	FloatHelper **EmissionAreaWidth;
	int *nEmissionAreaWidth;
	FloatHelper **Gravity2;
	int *nGravity2;


	/*
	 * FakeAnimBlocks don't change between animations
	 */
	Vec3Helper_f *Color;
	int nColor;
	ShortHelper_f *Opacity;
	int nOpacity;
	Vec2Helper_f *Size;
	int nSize;
	ShortHelper_f *Intensity;
	int nIntensity;
	ShortHelper_f *Unk;
	int nUnk;

	BoolHelper **Enabled;
	int *nEnabled;


	int lengthModelName;
	char *ModelName;
	int lengthParticleName;
	char *ParticleName;
	//I just include this for completness
	//now idea what it's used for <.<
	int *ref3_not_used;

	ParticleInfo(){

	}
	~ParticleInfo(){

	}

	void setnEmissionSpeed(int n){
		_TDn("SetnEmissionSpeed");
		Emissionspeed=new FloatHelper*[n];
		nEmissionspeed=new int[n];
	}
	void setEmissionSpeed(FloatHelper *t,int nSubs,int anim){
		_TDn("setEmissionSpeed");
		nEmissionspeed[anim]=nSubs;
		Emissionspeed[anim]=t;
	}
	void setnSpeedVariation(int n){
		_TDn("setnSpeedVariation");
		nSpeedVariation=new int[n];
		SpeedVariation=new FloatHelper*[n];
	}
	void setSpeedVariation(FloatHelper *t,int nSubs,int anim){
		_TDn("setSpeedVariation");
		nSpeedVariation[anim]=nSubs;
		SpeedVariation[anim]=t;
	}
	void setnVerticalRange(int n){
		_TDn("setnVerticalRange");
		nVerticalRange=new int[n];
		VerticalRange=new FloatHelper*[n];
	}
	void setVerticalRange(FloatHelper *t,int nSubs,int anim){
		_TDn("setVerticalRange");
		nVerticalRange[anim]=nSubs;
		VerticalRange[anim]=t;
	}
	void setHorizontalRange(FloatHelper *t,int nSubs,int anim){
		_TDn("setHorizontalRange");
		nHorizontalRange[anim]=nSubs;
		HorizontalRange[anim]=t;
	}
	void setnHorizontalRange(int n){
		_TDn("setnHorizontalRange");
		nHorizontalRange=new int[n];
		HorizontalRange=new FloatHelper*[n];
	}
	void setGravity(FloatHelper *t,int nSubs,int anim){
		_TDn("setGravity");
		nGravity[anim]=nSubs;
		Gravity[anim]=t;
	}
	void setnGravity(int n){
		_TDn("setnGravity");
		nGravity=new int[n];
		Gravity=new FloatHelper*[n];
	}
	void setLifespan(FloatHelper *t,int nSubs,int anim){
		_TDn("setLifespan");
		nLifespan[anim]=nSubs;
		Lifespan[anim]=t;
	}
	void setnLifespan(int n){
		_TDn("setnLifespan");
		nLifespan=new int[n];
		Lifespan=new FloatHelper*[n];
	}

	void setEmissionRate(FloatHelper *t,int nSubs,int anim){
		_TDn("setEmissionRate");
		nEmissionRate[anim]=nSubs;
		EmissionRate[anim]=t;
	}
	void setnEmissionRate(int n){
		_TDn("setnEmissionRate");
		nEmissionRate=new int[n];
		EmissionRate=new FloatHelper*[n];
	}

	void setEmissionAreaLength(FloatHelper *t,int nSubs,int anim){
		_TDn("setEmissionAreaLength");
		nEmissionAreaLength[anim]=nSubs;
		EmissionAreaLength[anim]=t;
	}
	void setnEmissionAreaLength(int n){
		_TDn("setnEmissionAreaLength");
		nEmissionAreaLength=new int[n];
		EmissionAreaLength=new FloatHelper*[n];
	}
	void setEmissionAreaWidth(FloatHelper *t,int nSubs,int anim){
		nEmissionAreaWidth[anim]=nSubs;
		EmissionAreaWidth[anim]=t;
	}
	void setnEmissionAreaWidth(int n){
		nEmissionAreaWidth=new int[n];
		EmissionAreaWidth=new FloatHelper*[n];
	}
	void setGravity2(FloatHelper *t,int nSubs,int anim){
		nGravity2[anim]=nSubs;
		Gravity2[anim]=t;
	}
	void setnGravity2(int n){
		nGravity2=new int[n];
		Gravity2=new FloatHelper*[n];
	}

	void setColor(Vec3Helper_f *t,int nSubs){
		nColor=nSubs;
		Color=t;
	}
	void setOpacity(ShortHelper_f *t,int nSubs){
		nOpacity=nSubs;
		Opacity=t;
	}
	void setSize(Vec2Helper_f *t,int nSubs){
		nSize=nSubs;
		Size=t;
	}
	void setIntensity(ShortHelper_f *t,int nSubs){
		nIntensity=nSubs;
		Intensity=t;
	}
	void setUnk(ShortHelper_f *t,int nSubs){
		nUnk=nSubs;
		Unk=t;
	}

	void setnEnabled(int n){
		_TDn("setnEnabled");
		nEnabled=new int[n];
		Enabled=new BoolHelper*[n];
	}

	void setEnabled(BoolHelper *t,int nSubs,int anim){
		_TDn("setEnabled");
		nEnabled[anim]=nSubs;
		Enabled[anim]=t;
	}

	void setModelName(int length,char*filename){
		lengthModelName=length;
		ModelName=filename;
	}

	void setParticleName(int length,char*filename){
		lengthParticleName=length;
		ParticleName=filename;
	}

};


class ParticleHelper{
public:
	ModelParticleEmitterDef *particles;
	int nParticles;
	ParticleInfo *info;
	ParticleHelper(){
		nParticles=0;
		particles=new ModelParticleEmitterDef[0];
		info=new ParticleInfo[0];
	}
	~ParticleHelper(){

	}

	void setnParticles(int n){
		nParticles=n;
		particles=new ModelParticleEmitterDef[n];
		info=new ParticleInfo[n];
	}
};

class ColorInfo{
public:
	int nValues;
	bool hasData;
	ShortHelper *tvalues;
	Vec3Helper *colvalues;
	ColorInfo(){
		hasData=false;
		nValues=0;
		tvalues=new ShortHelper[0];
		colvalues=new Vec3Helper[0];
	}
	~ColorInfo(){

	}
	void setData(Vec3Helper*col,ShortHelper *d,int nVals){
		hasData=true;
		tvalues=d;
		colvalues=col;
		nValues=nVals;
	}
};

class CameraInfo{
public:
	int nValues[3];
	bool hasData[3];
	Vec3Helper *translationpos;
	Vec3Helper *translationtar;
	Vec3Helper *rotation;
	CameraInfo(){
		nValues[0]=0;nValues[1]=0;nValues[2]=0;
		hasData[0]=false;hasData[1]=false;hasData[2]=false;
		translationpos=new Vec3Helper[0];
		translationtar=new Vec3Helper[0];
		rotation=new Vec3Helper[0];
	}
	~CameraInfo(){

	}
	void setRotation(Vec3Helper *rot,int nVals){
		hasData[2]=true;
		nValues[2]=nVals;
		rotation=rot;
	}
	void setTranslationTarget(Vec3Helper *transtar,int nVals){
		hasData[1]=true;
		nValues[1]=nVals;
		translationtar=transtar;
	}
	void setTranslationPosition(Vec3Helper *transpos,int nVals){
		hasData[0]=true;
		nValues[0]=nVals;
		translationpos=transpos;
	}
};

class TransparencyInfo{
public:
	int nValues;
	bool hasData;
	ShortHelper *values;
	TransparencyInfo(){
		hasData=false;
		nValues=0;
		values=new ShortHelper[0];
	}
	~TransparencyInfo(){

	}
	void setData(ShortHelper *d,int nVals){
		hasData=true;
		values=d;
		nValues=nVals;
	}
};

class LightInfo{
	int nValues[7];
	bool hasData[7];
	Vec3Helper *ambientColor;
	FloatHelper *ambientIntensity;
	Vec3Helper *diffuseColor;
	FloatHelper diffuseIntensity;
	FloatHelper *attenuationStart;
	FloatHelper *attenuationEnd;
	IntHelper *enabled;
};

class TexAnimInfo{
public:
	int nValues[3];
	bool hasData[3];
	Vec3Helper *trans;
	QuatHelper *rot;
	Vec3Helper *scaling;
	TexAnimInfo(){
		hasData[0]=false;hasData[1]=false;hasData[2]=false;
		nValues[0]=0;nValues[1]=0;nValues[2]=0;
		rot=new QuatHelper[0];
		trans=new Vec3Helper[0];
		scaling=new Vec3Helper[0];
	}
	~TexAnimInfo(){

	}

	void setTranslation(Vec3Helper *d,int nVals){
		hasData[0]=true;
		nValues[0]=nVals;
		trans=d;
	}
	void setRotation(QuatHelper *d,int nVals){
		hasData[1]=true;
		rot=d;
		nValues[1]=nVals;
	}
	void setScaling(Vec3Helper *d,int nVals){
		hasData[2]=true;
		nValues[2]=nVals;
		scaling=d;
	}
};


class BoneHelper{
public:
	int nAnimations;
	bool *inAnimFile;
	BoneInfo* info;
	BoneHelper(){
		nAnimations=0;
		inAnimFile=new bool[0];
		info=new BoneInfo[nAnimations];
	}
	~BoneHelper(){

	}
	void setnAnim(int n){
		BoneInfo *temp=info;
		info=new BoneInfo[n];
		inAnimFile=new bool[n];
		if(n>=nAnimations)
		for(int i=0;i<nAnimations;i++){
			info[i]=temp[i];
		}
		nAnimations=n;
	}

};

class TextureHelper{
public:
	int nTextures;
	ModelTextureDef *texdefs;
	char **Texnames;
	TextureHelper(){
		nTextures=0;
		texdefs=new ModelTextureDef[0];
		Texnames=new char*[0];
	}
	~TextureHelper(){

	}
	void setnTextures(int n){
		nTextures=n;
		texdefs=new ModelTextureDef[n];
		Texnames=new char*[n];
	}

	void addTexture(ModelTextureDef tex){
		ModelTextureDef *temp=texdefs;
		char **ctemp=Texnames;
		texdefs=new ModelTextureDef[nTextures+1];
		Texnames=new char*[nTextures+1];
		for(int i=0;i<nTextures;i++){
			texdefs[i]=temp[i];
			Texnames[i]=ctemp[i];
		}
		texdefs[nTextures]=tex;
		Texnames[nTextures]=new char[tex.nameLen];
		nTextures++;
	}

	void addTexture(ModelTextureDef tex,char*texname){
		ModelTextureDef *temp=texdefs;
		char **ctemp=Texnames;
		texdefs=new ModelTextureDef[nTextures+1];
		Texnames=new char*[nTextures+1];
		for(int i=0;i<nTextures;i++){
			texdefs[i]=temp[i];
			Texnames[i]=ctemp[i];
		}
		texdefs[nTextures]=tex;
		Texnames[nTextures]=texname;
		nTextures++;
	}

	void setTexture(ModelTextureDef tex,int n){
		texdefs[n]=tex;
		Texnames[n]=new char[texdefs[n].nameLen];
	}
};

class TransparencyHelper{
public:
	int nTransparency;
	int *nSubs;
	TransparencyInfo **info;
	int *interpolation;

	TransparencyHelper(){
		nTransparency=0;
		info=new TransparencyInfo*[0];
		nSubs=new int[0];
	}
	~TransparencyHelper(){

	}
	void setnTransparency(int n){
		info=new TransparencyInfo*[n];
		nSubs=new int[n];
		interpolation=new int[n];
		for(int i=0;i<n;i++){
			nSubs[i]=0;
			interpolation[i]=0;
		}
		nTransparency=n;
	}

	void setInterpolation(int transparency,int type){
		interpolation[transparency]=type;
	}

	void setnSubs(int transparency,int n){
		nSubs[transparency]=n;
		info[transparency]=new TransparencyInfo[n];
	}
};

class ColorHelper{
public:
	int nColors;
	int *nSubs;
	ColorInfo **info;
	int *interpolation;

	ColorHelper(){
		nColors=0;
		info=new ColorInfo*[0];
		nSubs=new int[0];
	}
	~ColorHelper(){

	}
	void setnColors(int n){
		info=new ColorInfo*[n];
		nSubs=new int[n];
		interpolation=new int[n];
		for(int i=0;i<n;i++){
			nSubs[i]=0;
			interpolation[i]=0;
		}
		nColors=n;
	}

	void setInterpolation(int color,int type){
		interpolation[color]=type;
	}

	void setnSubs(int color,int n){
		nSubs[color]=n;
		info[color]=new ColorInfo[n];
	}
};

class TexAnimHelper{
public:
	int nTexAnims;
	int *nSubs;
	TexAnimInfo **info;
	ModelTexAnimDef *TexAnims;
	TexAnimHelper(){
		nTexAnims=0;
		nSubs=new int[0];
		info=new TexAnimInfo*[0];
		TexAnims=new ModelTexAnimDef[0];
	}
	~TexAnimHelper(){

	}

	void setnTexAnims(int n){
		nTexAnims=n;
		info=new TexAnimInfo*[n];
		nSubs=new int[n];
		TexAnims=new ModelTexAnimDef[n];
	}
	void setnSubs(int texanim,int num){
		nSubs[texanim]=num;
		info[texanim]=new TexAnimInfo[num];
	}
};

class LightHelper{
	int nLights;
	int *nSubs;

};

class CamHelper{
public:
	int nCameras;
	int *nSubs;
	CameraInfo **info;
	ModelCameraDef *camera;
	CamHelper(){
		nCameras=0;
		nSubs=new int[0];
		camera=new ModelCameraDef[0];
	}
	~CamHelper(){

	}
	void setnCameras(int n){
		info=new CameraInfo*[n];
		nSubs=new int[n];
		for(int i=0;i<n;i++){
			nSubs[i]=0;
		}
		nCameras=n;
	}

	void setnSubs(int camera,int n){
		nSubs[camera]=n;
		info[camera]=new CameraInfo[n];
	}
};

#endif /* MODELHELPSTRUCTS_H_ */
