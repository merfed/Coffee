/*
 * modeleditor.h
 *
 *  Created on: 25.05.2009
 *      Author: Bastian
 *
 *  Important Info:
 *  This misses a rendering function so it will blow up your FileSize!
 */
#include "modelheaders.h"
#include "modelenums.h"
#include "modelhelpstructs.h"
#include "SMem.hpp"
#include "trace.h"
#ifndef MODELEDITOR_H_
#define MODELEDITOR_H_



class ModelEditor{

public:
	ModelEditor(char *m,int FileSize);
	ModelEditor();
	~ModelEditor();
	int GetFileSize();
	char * getFile();
	void ResizeModel(float factor);
	void setModelName(char *name,int length);
	void addTexture(ModelTextureDef tex,const char * texname);
	void setTransparency(TransparencyHelper help);
	//the following voids are used to add additional things
	void addVertices(ModelVertex * vert,int nvert);
	void addAnimations(ModelAnimation *anim,int nanim);
	void addAnimationLookup(int nanim);
	void addRibbon(ModelRibbonEmitterDef ribbon,RibbonHelper help);
	//and these are used to add new things by replacing the old ones(if there where some...)
	void newGlobalSequences(uint32 *sequ,int nSequ);
	void newTransparency(TransparencyHelper help);
	void newTransparencyLookup();
	void newVertices(ModelVertex *vert,int nvert);
	void newRenderflags(ModelRenderFlags *flags,int num);
	void newColors(ColorHelper help);
	void newAnimations(ModelAnimation *anim,int nanim);
	void newAnimationLookup(int nanim);
	void newBones(ModelBoneDef *bones,int nBones,BoneHelper *help);
	void newKeyBoneLookup();
	void newBoneLookup();
	void newTextures(TextureHelper help);
	void newTexLookup();
	void newTexUnitLookup();
	void newTexReplace();
	void newTexAnim(TexAnimHelper help);
	void newTexAnimLookup();
	void newAttachments(ModelAttachmentDef*attachments,int num);
	void newCameras(CamHelper help);
	void newRibbons(ModelRibbonEmitterDef *ribbon,RibbonHelper* help,int nribbon);

	//get Classes
	uint32 *getGlobalSequences();
	int getnGlobalSequences();
	ModelAnimation* getAnimations();
	int getnAnimations();
	ModelBoneDef* getBones();
	BoneHelper* getBoneData();
	int getnBones();
	ModelVertex *getVertices();
	int getnVertices();
	ModelRenderFlags *getRenderflags();
	int getnRenderflags();
	ColorHelper getColor();
	TransparencyHelper getTransparency();
	TextureHelper getTextures();
	TexAnimHelper getTexAnim();
	ModelRibbonEmitterDef *getRibbons();
	RibbonHelper *getRibbonData();
	ModelParticleEmitterDef *getParticles();
	ParticleHelper *getParticleData();
	Vec3D *getBoundingVertices();
	ModelBoundTriangle *getBoundingTriangles();
	Vec3D *getBoundingNormals();
	ModelLightDef *getLights();
	LightHelper *getLightData();
	CamHelper getCameras();
	int getnAttachments();
	ModelAttachmentDef *getAttachments();
	ModelAttach2 *getAttachment2();

private:
	void FillLine();
	void SetStructure();
	int writeSubStructs(BoneHelper help);
	template<class T>
	T* getAnimblockData(T *data,int nValues);
	template<class T>
	int writeAnimblockData(T *data,int nValues);

	char *f;
	int FileSize;
	ModelHeader *header_m;
	uint32 *globaltimestamps_m;
	ModelAnimation *anim_m;
	int16 *animlookup_m;
	ModelBoneDef *bone_m;
	uint16 *bonelookup_m;
	uint16 *keybonelookup_m;

	ModelVertex *vertex_m;
	ModelRenderFlags *renderflags_m;
	int16 *texunitlookup_m;
	uint16 *texreplacelookup_m;
	ModelColorDef * color_m;
	uint16 *translookup_m;
	ModelTransDef * trans_m;
	uint16 * texturelookup_m;
	ModelTextureDef * texture_m;
	uint16 *texanimlookup_m;
	ModelTexAnimDef *texanim_m;

	ModelRibbonEmitterDef *ribbon_m;
	ModelParticleEmitterDef *part_m;

	Vec3D * boundvertices_m;
	ModelBoundTriangle * boundtriangle_m;
	Vec3D *boundnormals_m;
	ModelLightDef *light_m;
	ModelCameraDef * cam_m;
	uint16 *camlookup_m;
	ModelAttachmentDef *attach_m;
	uint16 *attachlookup_m;
	ModelAttach2 * attach2_m;

};



#endif /* MODELEDITOR_H_ */
