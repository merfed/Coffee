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

class ModelEditor {

public:
	ModelEditor(char *m, int FileSize);
	ModelEditor();
	~ModelEditor();
	int GetFileSize();
	char * getFile();
	void ResizeModel(float factor);
	void ResizeModel(float x,float y,float z);
	void setModelName(char *name, int length);
	//the following voids are used to add additional things
	void addTexture(ModelTextureDef tex, const char * texname);
	void addVertices(ModelVertex * vert, int nvert);
	void addAnimations(ModelAnimation *anim, int nanim);
	void addAnimationLookup(int nanim);
	void addRibbon(ModelRibbonEmitterDef ribbon, RibbonHelper help);
	//and these are used to add new things by replacing the old ones(if there where some...)
	void newGlobalSequences(uint32 *sequ, int nSequ);
	void newTransparency(AnimationHelper<short> *help, int nTrans);
	void newTransparencyLookup();
	void newVertices(ModelVertex *vert, int nvert);
	void newRenderflags(ModelRenderFlags *flags, int num);
	void newColors(ColorHelper help);
	void newAnimations(ModelAnimation *anim, int nanim);
	void newAnimationLookup(int nanim);
	void newBones(BoneHelper help);
	void newKeyBoneLookup();
	void newBoneLookup();
	void newTextures(TextureHelper help);
	void newTexLookup();
	void newTexUnitLookup();
	void newTexReplace();
	void newTexAnim(TexAnimHelper help);
	void newTexAnimLookup();
	void newBounds(BoundVolumeHelper help);
	void newLights(LightHelper help);
	void newAttachments(ModelAttachmentDef*attachments, int num);
	void newCameras(CamHelper help);
	void newRibbons(RibbonHelper help);
	void newParticles(ParticleHelper help);
	//get Classes
	uint32 *getGlobalSequences();
	int getnGlobalSequences();
	ModelAnimation* getAnimations();
	int getnAnimations();
	BoneHelper getBones(bool *inAnim, int nAnims);
	ModelVertex *getVertices();
	int getnVertices();
	ModelRenderFlags *getRenderflags();
	int getnRenderflags();
	ColorHelper getColor();
	AnimationHelper<short> *getTransparency();
	int getnTransparency();
	TextureHelper getTextures();
	TexAnimHelper getTexAnim();
	RibbonHelper getRibbons();
	ParticleHelper getParticles();
	BoundVolumeHelper getBounds();
	LightHelper getLights();
	CamHelper getCameras();
	int getnAttachments();
	ModelAttachmentDef *getAttachments();
	ModelEvents *getEvents();

	//helpers
	void calcVertexBox();
	void calcBoundingBox();

private:
	void FillLine();
	void SetStructure();
	int writeSubStructs(BoneHelper help);
	template<class T>
	T* getAnimblockData(T *data, int nValues);
	template<class T>
	int writeAnimblockData(T *data, int nValues);
	template<class T>
	AnimationHelper<T> readAnimation(AnimationBlock ablock);
	template<class T>
	AnimationHelper<T> readAnimation(AnimationBlock ablock, bool*inAnim);
	template<class T>
	void writeAnimation(AnimationBlock *ablock, AnimationHelper<T> help);

	char *f;
	int FileSize;
	ModelHeader *header_m;
	ofsUnk *offsetunk_m;
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
	ModelEvents * events_m;

};

#endif /* MODELEDITOR_H_ */
