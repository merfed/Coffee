/*
 * modeleditor.cpp
 *
 *  Created on: 25.05.2009
 *      Author: Bastian
 */
#include "modeleditor.h"

ModelEditor::ModelEditor(char *m,int FileSize){
	f=m;
	this->FileSize=FileSize;
	SetStructure();
}

ModelEditor::~ModelEditor(){

}

void ModelEditor::SetStructure(){
	header_m=(ModelHeader *)(f);
	vertex_m=(ModelVertex *)(f+header_m->ofsVertices);
	bone_m=(ModelBoneDef *)(f+header_m->ofsBones);
	light_m=(ModelLightDef *)(f+header_m->ofsLights);
	part_m=(ModelParticleEmitterDef *)(f+header_m->ofsParticleEmitters);
	ribbon_m=(ModelRibbonEmitterDef *)(f+header_m->ofsRibbonEmitters);
	attach_m=(ModelAttachmentDef *)(f+header_m->ofsAttachments);
	anim_m=(ModelAnimation *)(f+header_m->ofsAnimations);
	texanim_m=(ModelTexAnimDef *)(f+header_m->ofsTexAnims);
	renderflags_m=(ModelRenderFlags *)(f+header_m->ofsRenderFlags);
	color_m=(ModelColorDef * )(f+header_m->ofsColors);
	trans_m=(ModelTransDef * )(f+header_m->ofsTransparency);
	texture_m=(ModelTextureDef * )(f+header_m->ofsTextures);
	texturelookup_m=(uint16 *)(f+header_m->ofsTexLookup);
	cam_m=(ModelCameraDef * )(f+header_m->ofsCameras);
	attach2_m=(ModelAttach2 * )(f+header_m->ofsAttachments_2);
	transparency_m=(AnimationBlock *)(f+header_m->ofsTransparency);

}

int ModelEditor::GetFileSize(){
	return FileSize;
}

char * ModelEditor::getFile(){
	return f;
}

void ModelEditor::FillLine(){
	int newSize=FileSize+(16-((FileSize)%16));
	char *n=new char[newSize];
	memcpy(n,f,FileSize);
	f=n;
	FileSize=newSize;
}

short * ModelEditor::getTransparency(int * nTrans){
	nTrans[0]=header_m->nTransparency;
	short * transval=new short[header_m->nTransparency];
	for(int i=0;i<header_m->nTransparency;i++){
		AnimSubStructure * sub=(AnimSubStructure *)(f+transparency_m->ofsKeys);
		transval[i]=((short *)(f+sub->ofs))[0];
	}
	return transval;
}

void ModelEditor::setTransparency(short * values){
	for(int i=0;i<header_m->nTransparency;i++){
		AnimSubStructure * sub=(AnimSubStructure *)(f+transparency_m->ofsKeys);
		short * temp=(short *)(f+sub->ofs);
		temp[0]=values[i];
	}

}

void ModelEditor::addTexture(ModelTextureDef tex,const char * texname){
	FillLine();
	//let's calc the needed space
	int newSize=FileSize+
				sizeof(ModelTextureDef)*(header_m->nTextures+1)+
				//(16-((sizeof(ModelTextureDef)*(header_m->nTextures+1))%16))+ //we don't need this as (sizeof(ModelTextureDef)==0x10
				tex.nameLen+
				(16-(tex.nameLen)%16)+
				sizeof(uint16)*(header_m->nTexLookup+1)+
				(16-((sizeof(uint16)*(header_m->nTexLookup+1))%16));
	char *n=f;
	f=new char[newSize];
	memcpy(f,n,FileSize);

	memcpy(f+FileSize,(f+header_m->ofsTextures),sizeof(ModelTextureDef)*header_m->nTextures);
	memcpy(f+FileSize+
			sizeof(ModelTextureDef)*(header_m->nTextures+1),texname,tex.nameLen);
	memcpy(f+FileSize+
			sizeof(ModelTextureDef)*(header_m->nTextures+1)+
			tex.nameLen+
			(16-(sizeof(tex.nameLen))%16),(f+header_m->ofsTexLookup),sizeof(uint16)*header_m->nTexLookup);
	header_m=(ModelHeader *)(f);


	header_m->ofsTextures=FileSize;
	texture_m=(ModelTextureDef * )(f+header_m->ofsTextures);
	texture_m[header_m->nTextures].flags=tex.flags;
	texture_m[header_m->nTextures].type=tex.type;
	texture_m[header_m->nTextures].nameLen=tex.nameLen;
	texture_m[header_m->nTextures].nameOfs=FileSize+sizeof(ModelTextureDef)*(header_m->nTextures+1);


	header_m->ofsTexLookup=FileSize+
				sizeof(ModelTextureDef)*(header_m->nTextures+1)+
				tex.nameLen+
				(16-(tex.nameLen)%16);
	texturelookup_m=(uint16 *)(f+header_m->ofsTexLookup);
	texturelookup_m[header_m->nTexLookup]=header_m->nTextures;

	header_m->nTextures++;
	header_m->nTexLookup++;

	FileSize=newSize;
	FillLine();
	SetStructure();
}

void ModelEditor::addRibbon(ModelRibbonEmitterDef ribbon,RibbonHelper help){
	FillLine();
	int newSize=FileSize+sizeof(ModelRibbonEmitterDef)*(header_m->nRibbonEmitters+1)+ribbon_ofs_size;
	int ofs=FileSize+sizeof(ModelRibbonEmitterDef)*(header_m->nRibbonEmitters+1);

	char *n=f;
	f=new char[newSize];
	memcpy(f,n,FileSize);
	memcpy(f,(f+header_m->ofsRibbonEmitters),sizeof(ModelRibbonEmitterDef)*(header_m->nRibbonEmitters));

	header_m=(ModelHeader *)(f);

	header_m->ofsRibbonEmitters=FileSize;
	ribbon_m=(ModelRibbonEmitterDef *)(f+header_m->ofsRibbonEmitters);

	ribbon_m[header_m->nRibbonEmitters].id=ribbon.id;
	ribbon_m[header_m->nRibbonEmitters].bone=ribbon.bone;
	ribbon_m[header_m->nRibbonEmitters].length=ribbon.length;
	ribbon_m[header_m->nRibbonEmitters].pos[0]=ribbon.pos[0];
	ribbon_m[header_m->nRibbonEmitters].pos[1]=ribbon.pos[1];
	ribbon_m[header_m->nRibbonEmitters].pos[2]=ribbon.pos[2];
	ribbon_m[header_m->nRibbonEmitters].res=ribbon.res;
	ribbon_m[header_m->nRibbonEmitters].s1=ribbon.s1;
	ribbon_m[header_m->nRibbonEmitters].s2=ribbon.s2;
	ribbon_m[header_m->nRibbonEmitters].unk=ribbon.unk;
	ribbon_m[header_m->nRibbonEmitters].unknown=ribbon.unknown;
	//now we calc the ofsets
	ribbon_m[header_m->nRibbonEmitters].nTextures=1;
	ribbon_m[header_m->nRibbonEmitters].ofsTextures=ofs;
	ribbon_m[header_m->nRibbonEmitters].nUnknown=1;
	ribbon_m[header_m->nRibbonEmitters].ofsUnknown=ofs+0x10;

	ribbon_m[header_m->nRibbonEmitters].color.seq=-1;
	ribbon_m[header_m->nRibbonEmitters].color.nKeys=1;
	ribbon_m[header_m->nRibbonEmitters].color.nTimes=1;
	ribbon_m[header_m->nRibbonEmitters].color.ofsTimes=ofs+color_times;
	ribbon_m[header_m->nRibbonEmitters].color.ofsKeys=ofs+color_keys;

	ribbon_m[header_m->nRibbonEmitters].opacity.seq=-1;
	ribbon_m[header_m->nRibbonEmitters].opacity.nKeys=1;
	ribbon_m[header_m->nRibbonEmitters].opacity.nTimes=1;
	ribbon_m[header_m->nRibbonEmitters].opacity.ofsTimes=ofs+opacity_times;
	ribbon_m[header_m->nRibbonEmitters].opacity.ofsKeys=ofs+opacity_keys;

	ribbon_m[header_m->nRibbonEmitters].above.seq=-1;
	ribbon_m[header_m->nRibbonEmitters].above.nKeys=1;
	ribbon_m[header_m->nRibbonEmitters].above.nTimes=1;
	ribbon_m[header_m->nRibbonEmitters].above.ofsKeys=ofs+above_keys;
	ribbon_m[header_m->nRibbonEmitters].above.ofsTimes=ofs+above_times;

	ribbon_m[header_m->nRibbonEmitters].below.seq=-1;
	ribbon_m[header_m->nRibbonEmitters].below.nKeys=1;
	ribbon_m[header_m->nRibbonEmitters].below.nTimes=1;
	ribbon_m[header_m->nRibbonEmitters].below.ofsKeys=ofs+below_keys;
	ribbon_m[header_m->nRibbonEmitters].below.ofsTimes=ofs+below_times;

	ribbon_m[header_m->nRibbonEmitters].unk1.seq=-1;
	ribbon_m[header_m->nRibbonEmitters].unk1.nKeys=1;
	ribbon_m[header_m->nRibbonEmitters].unk1.nTimes=1;
	ribbon_m[header_m->nRibbonEmitters].unk1.ofsKeys=ofs+unk1_keys;
	ribbon_m[header_m->nRibbonEmitters].unk1.ofsTimes=ofs+unk1_times;

	ribbon_m[header_m->nRibbonEmitters].unk2.seq=-1;
	ribbon_m[header_m->nRibbonEmitters].unk2.nKeys=1;
	ribbon_m[header_m->nRibbonEmitters].unk2.nTimes=1;
	ribbon_m[header_m->nRibbonEmitters].unk2.ofsKeys=ofs+unk2_keys;
	ribbon_m[header_m->nRibbonEmitters].unk2.ofsTimes=ofs+unk2_times;

	header_m->nRibbonEmitters++;


	float *TFloat;
	int *TInt;
	short *TShort;

	/*
	 * refTex - unk2_keys
	 */
	TInt=(int *)(f+ofs+refTex);
	*TInt=help.texture;

	TInt=(int *)(f+ofs+refBlend);
	*TInt=help.blend;

	TInt=(int *)(f+ofs+color_times);
	*TInt=1;TInt++;*TInt=ofs+color_times_v;

	TInt=(int *)(f+ofs+color_keys);
	*TInt=1;TInt++;*TInt=ofs+color_keys_v;

	TInt=(int *)(f+ofs+opacity_times);
	*TInt=1;TInt++;*TInt=ofs+opacity_times_v;

	TInt=(int *)(f+ofs+opacity_keys);
	*TInt=1;TInt++;*TInt=ofs+opacity_keys_v;

	TInt=(int *)(f+ofs+above_times);
	*TInt=1;TInt++;*TInt=ofs+above_times_v;

	TInt=(int *)(f+ofs+above_keys);
	*TInt=1;TInt++;*TInt=ofs+above_keys_v;

	TInt=(int *)(f+ofs+below_times);
	*TInt=1;TInt++;*TInt=ofs+below_times_v;

	TInt=(int *)(f+ofs+below_keys);
	*TInt=1;TInt++;*TInt=ofs+below_keys_v;

	TInt=(int *)(f+ofs+unk1_times);
	*TInt=1;TInt++;*TInt=ofs+unk1_times_v;

	TInt=(int *)(f+ofs+unk1_keys);
	*TInt=1;TInt++;*TInt=ofs+unk1_keys_v;

	TInt=(int *)(f+ofs+unk2_times);
	*TInt=1;TInt++;*TInt=ofs+unk2_times_v;

	TInt=(int *)(f+ofs+unk2_keys);
	*TInt=1;TInt++;*TInt=ofs+unk2_keys_v;

	/*
	 * Now we add the values
	 */
	TFloat=(float *)(f+ofs+color_keys_v);
	*TFloat=help.color[0];TFloat++;
	*TFloat=help.color[1];TFloat++;
	*TFloat=help.color[2];TFloat++;

	TShort=(short *)(f+ofs+opacity_keys_v);
	*TShort=help.opacity;

	TFloat=(float *)(f+ofs+above_keys_v);
	*TFloat=help.above;

	TFloat=(float *)(f+ofs+below_keys_v);
	*TFloat=help.below;

	TInt=(int *)(f+ofs+unk1_keys_v);
	*TInt=help.zero;

	TInt=(int *)(f+ofs+unk2_keys_v);
	*TInt=help.one;


	FileSize=newSize;
	FillLine();
	SetStructure();
}

void ModelEditor::ResizeModel(float factor){

		for(int i=0;i<header_m->nVertices;i++){
			vertex_m[i].pos[0]*=factor;
			vertex_m[i].pos[1]*=factor;
			vertex_m[i].pos[2]*=factor;
		}
		for(int i=0;i<header_m->nBones;i++){
			bone_m[i].pivot[0]*=factor;
			bone_m[i].pivot[1]*=factor;
			bone_m[i].pivot[2]*=factor;
		}
		for(int i=0;i<header_m->nLights;i++){
			light_m[i].pos[0]*=factor;
			light_m[i].pos[1]*=factor;
			light_m[i].pos[2]*=factor;
		}
		for(int i=0;i<header_m->nAttachments;i++){
			attach_m[i].pos[0]*=factor;
			attach_m[i].pos[1]*=factor;
			attach_m[i].pos[2]*=factor;
		}
		for(int i=0;i<header_m->nRibbonEmitters;i++){
			ribbon_m[i].pos[0]*=factor;
			ribbon_m[i].pos[1]*=factor;
			ribbon_m[i].pos[2]*=factor;
		}
		for(int i=0;i<header_m->nParticleEmitters;i++){
			part_m[i].pos[0]*=factor;
			part_m[i].pos[1]*=factor;
			part_m[i].pos[2]*=factor;
		}

}
