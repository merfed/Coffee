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
#ifndef MODELEDITOR_H_
#define MODELEDITOR_H_



class ModelEditor{

public:
	ModelEditor(char *m,int FileSize);
	~ModelEditor();
	int GetFileSize();
	char * getFile();
	void ResizeModel(float factor);
	void addTexture(ModelTextureDef tex,const char * texname);
	short* getTransparency(int * nTrans);
	void setTransparency(short * values);
	void addRibbon(ModelRibbonEmitterDef ribbon,RibbonHelper help);

private:
	void FillLine();
	void SetStructure();

	char *f;
	int FileSize;
	ModelHeader *header_m;
	ModelLightDef *light_m;
	ModelRibbonEmitterDef *ribbon_m;
	ModelParticleEmitterDef *part_m;
	ModelAttachmentDef *attach_m;
	ModelBoneDef *bone_m;
	ModelVertex *vertex_m;
	ModelAnimation *anim_m;
	ModelTexAnimDef *texanim_m;
	ModelRenderFlags *renderflags_m;
	ModelColorDef * color_m;
	ModelTransDef * trans_m;
	ModelTextureDef * texture_m;
	uint16 * texturelookup_m;
	ModelCameraDef * cam_m;
	ModelAttach2 * attach2_m;
	AnimationBlock * transparency_m;
};



#endif /* MODELEDITOR_H_ */
