/*
 * modeleditor.cpp
 *
 *  Created on: 25.05.2009
 *      Author: Bastian
 */
#include "modeleditor.h"

ModelEditor::ModelEditor(char *m,int FileSize){
	_Tn("File-Konstruktor");
	f=m;
	this->FileSize=FileSize;
	SetStructure();
}

ModelEditor::ModelEditor(){
	_Tn("()-Konstruktor");
	f=SMemNew(sizeof(ModelHeader));
	this->FileSize=sizeof(ModelHeader);
	for(int i=0;i<FileSize;i++){
		f[i]=0;
	}
	SetStructure();
	header_m->id[0]='M';
	header_m->id[1]='D';
	header_m->id[2]='2';
	header_m->id[3]='0';
	header_m->version[0]=0x08;
	header_m->version[1]=0x01;
	header_m->version[2]=0x00;
	header_m->version[3]=0x00;
	header_m->nViews=1;

}

ModelEditor::~ModelEditor(){

}

/**
*This is used to  set the mainstructs of the file
*Very usefull ;P
*and call it as often as needed(aka f has changed!)
**/
void ModelEditor::SetStructure(){
	_TDn("SetStructure()");
	header_m=(ModelHeader *)(f);

	globaltimestamps_m=(uint32 *)(f+header_m->ofsGlobalSequences);
	anim_m=(ModelAnimation *)(f+header_m->ofsAnimations);
	animlookup_m=(int16 *)(f+header_m->ofsAnimationLookup);
	bone_m=(ModelBoneDef *)(f+header_m->ofsBones);
	bonelookup_m=(uint16*)(f+header_m->ofsBoneLookupTable);
	keybonelookup_m=(uint16*)(f+header_m->ofsKeyBoneLookup);

	vertex_m=(ModelVertex *)(f+header_m->ofsVertices);
	renderflags_m=(ModelRenderFlags *)(f+header_m->ofsRenderFlags);
	texunitlookup_m=(int16 *)(f+header_m->ofsTexUnitLookup);
	texreplacelookup_m=(uint16 *)(f+header_m->ofsTexReplace);
	color_m=(ModelColorDef * )(f+header_m->ofsColors);
	translookup_m=(uint16 *)(f+header_m->ofsTransparencyLookup);
	trans_m=(ModelTransDef * )(f+header_m->ofsTransparency);
	texturelookup_m=(uint16 *)(f+header_m->ofsTexLookup);
	texture_m=(ModelTextureDef * )(f+header_m->ofsTextures);
	texanimlookup_m=(uint16*)(f+header_m->ofsTexAnimLookup);
	texanim_m=(ModelTexAnimDef *)(f+header_m->ofsTexAnims);

	part_m=(ModelParticleEmitterDef *)(f+header_m->ofsParticleEmitters);
	ribbon_m=(ModelRibbonEmitterDef *)(f+header_m->ofsRibbonEmitters);

	boundvertices_m=(Vec3D *)(f+header_m->ofsBoundingVertices);
	boundtriangle_m=(ModelBoundTriangle *)(f+header_m->ofsBoundingTriangles);
	boundnormals_m=(Vec3D *)(f+header_m->ofsBoundingNormals);
	light_m=(ModelLightDef *)(f+header_m->ofsLights);
	cam_m=(ModelCameraDef * )(f+header_m->ofsCameras);
	camlookup_m=(uint16 *)(f+header_m->ofsCameraLookup);
	attach_m=(ModelAttachmentDef *)(f+header_m->ofsAttachments);
	attachlookup_m=(uint16*)(f+header_m->ofsAttachLookup);
	attach2_m=(ModelAttach2 * )(f+header_m->ofsAttachments_2);

}

int ModelEditor::GetFileSize(){
	return FileSize;
}

char * ModelEditor::getFile(){
	return f;
}

/**
*This fills up the file to 0x10 bytes
*so we get a nice filestruct :D
**/
void ModelEditor::FillLine(){
	_TDn("FillLine()");
	int newSize=FileSize+(16-((FileSize)%16));
	char *n=SMemNew(newSize);
	memcpy(n,f,FileSize);
	f=n;
	FileSize=newSize;
	SetStructure();
}

void ModelEditor::setModelName(char *name,int length){
	FillLine();
}

TransparencyHelper ModelEditor::getTransparency(){
	_Tn("getTransparency()");
	TransparencyHelper ret=TransparencyHelper();
	ret.setnTransparency(header_m->nTransparency);
	for(int i=0;i<header_m->nTransparency;i++){
		//nTimes should always be nKeys...
		ret.setnSubs(i,trans_m[i].trans.nTimes);
		ret.setInterpolation(i,trans_m[i].trans.type);
		AnimSubStructure *timesub;
		AnimSubStructure *keysub;
		int offset=trans_m[i].trans.ofsTimes;
		timesub=getAnimblockData(timesub,offset);
		offset=trans_m[i].trans.ofsKeys;
		keysub=getAnimblockData(keysub,offset);
		for(int j=0;j<trans_m[i].trans.nTimes;j++){
			if(timesub[j].n!=0){
				ShortHelper *values=new ShortHelper[timesub[j].n];
				int nvals=timesub[j].n;
				short *ts=getAnimblockData(ts,keysub[j].ofs);
				uint32 *tt=getAnimblockData(tt,timesub[j].ofs);
				for(int k=0;k<timesub[j].n;k++){
					values[k].timestamp=tt[k];
					values[k].value=ts[k];
				}
				ret.info[i][j].setData(values,nvals);
			}
		}
	}
	return ret;
}

void ModelEditor::newTransparency(TransparencyHelper help){
	_Tn("New Transparency()");
	int newSize=FileSize+sizeof(AnimationBlock)*help.nTransparency;
	char *n=f;
	f=SMemNew(newSize);
	memcpy(f,n,FileSize);
	SetStructure();
	header_m->ofsTransparency=FileSize;
	header_m->nTransparency=help.nTransparency;
	FileSize=newSize;
	FillLine();
	AnimSubStructure **timesub=new AnimSubStructure*[help.nTransparency];
	AnimSubStructure **keysub=new AnimSubStructure*[help.nTransparency];
	int nPre=0;
	for(int i=0;i<help.nTransparency;i++){
		trans_m[i].trans.nKeys=help.nSubs[i];
		trans_m[i].trans.nTimes=help.nSubs[i];
		trans_m[i].trans.seq=-1;
		trans_m[i].trans.type=help.interpolation[i];
		trans_m[i].trans.ofsTimes=FileSize+nPre*sizeof(AnimSubStructure);
		trans_m[i].trans.ofsKeys=FileSize+(nPre+1)*sizeof(AnimSubStructure);
		timesub[i]=new AnimSubStructure[help.nSubs[i]];
		keysub[i]=new AnimSubStructure[help.nSubs[i]];
		for(int k=0;k<help.nSubs[i];k++){
			timesub[i][k].n=help.info[i][k].nValues;
			keysub[i][k].n=help.info[i][k].nValues;
		}
		nPre+=2*help.nSubs[i];
	}
	newSize=FileSize+2*nPre*sizeof(AnimSubStructure);
	n=f;
	f=SMemNew(newSize);
	memcpy(f,n,FileSize);
	SetStructure();
	int nSubs=0;
	for(int i=0;i<header_m->nTransparency;i++){
		memcpy(f+FileSize+nSubs*sizeof(AnimSubStructure),timesub[i],sizeof(AnimSubStructure)*help.nSubs[i]);
		memcpy(f+FileSize+(nSubs+1)*sizeof(AnimSubStructure),keysub[i],sizeof(AnimSubStructure)*help.nSubs[i]);
		timesub[i]=(AnimSubStructure *)(f+FileSize+nSubs*sizeof(AnimSubStructure));
		keysub[i]=(AnimSubStructure *)(f+FileSize+(nSubs+1)*sizeof(AnimSubStructure));
		nSubs+=2*help.nSubs[i];
	}
	int ofsSize=FileSize;
	FileSize=newSize;
	FillLine();
	nSubs=0;
	for(int i=0;i<header_m->nTransparency;i++){
		//we need to reset the pointers whenever FileSize has changed..
		for(int j=0;j<help.nSubs[i];j++){
			//n was set before.. I think xD
			timesub[i]=(AnimSubStructure *)(f+ofsSize+nSubs*sizeof(AnimSubStructure));
			timesub[i][j].ofs=FileSize;
			uint32 *time=new uint32[help.info[i][j].nValues];
			for(int k=0;k<help.info[i][j].nValues;k++){
				time[k]=help.info[i][j].values[k].timestamp;
			}
			writeAnimblockData(time,help.info[i][j].nValues);
			keysub[i]=(AnimSubStructure *)(f+ofsSize+(nSubs+1)*sizeof(AnimSubStructure));
			keysub[i][j].ofs=FileSize;
			short *val=new short[help.info[i][j].nValues];
			for(int k=0;k<help.info[i][j].nValues;k++){
				val[k]=help.info[i][j].values[k].value;
			}
			writeAnimblockData(val,help.info[i][j].nValues);
		}
		nSubs+=2*help.nSubs[i];
	}
	FillLine();
}

/**
 * Important Info:
 * if you use this the transparency must be the same
 * expect the values...
 */
void ModelEditor::setTransparency(TransparencyHelper help){
	_Tn("Set Transparency()");
	for(int i=0;i<header_m->nTransparency;i++){
		AnimSubStructure *timesub;
		AnimSubStructure *keysub;
		int offset=trans_m->trans.ofsTimes;
		timesub=getAnimblockData(timesub,offset);
		offset=trans_m->trans.ofsKeys;
		keysub=getAnimblockData(keysub,offset);
		for(int j=0;j<trans_m->trans.nTimes;j++){
		if(timesub[j].n!=0){
			short *ts=(short*)(f+keysub[j].ofs);
			uint32 *tt=(uint32 *)(f+timesub[j].ofs);
			for(int k=0;k<timesub[j].n;k++){
				tt[k]=help.info[i][j].values[k].timestamp;
				ts[k]=help.info[i][j].values[k].value;
			}
		}
		}
	}
}

void ModelEditor::newTransparencyLookup(){
	_Tn("newTransparencyLookup()");
	FillLine();
	int newSize=FileSize+header_m->nTransparency*sizeof(short);
	char *n=f;
	f=SMemNew(newSize);
	memcpy(f,n,FileSize);
	SetStructure();
	header_m->nTransparencyLookup=header_m->nTransparency;
	header_m->ofsTransparencyLookup=FileSize;
	printf("Setted Offsets\n");
	SetStructure();
	FileSize=newSize;
	for(int i=0;i<header_m->nTransparencyLookup;i++){
		translookup_m[i]=i;
	}
	FillLine();

}

ModelBoneDef* ModelEditor::getBones(){
	return bone_m;
}

uint32 *ModelEditor::getGlobalSequences(){
	return globaltimestamps_m;
}

int ModelEditor::getnGlobalSequences(){
	return header_m->nGlobalSequences;
}

void ModelEditor::newGlobalSequences(uint32 *sequ,int nSequ){
	_Tn("newGlobalSequences()");
	FillLine();
	int newSize=FileSize+sizeof(uint32)+nSequ;
	char *n=f;
	f=SMemNew(newSize);
	memcpy(f,n,FileSize);
	SetStructure();
	header_m->ofsGlobalSequences=FileSize;
	header_m->nGlobalSequences=nSequ;
	memcpy(f+FileSize,sequ,sizeof(uint32)*nSequ);
	FileSize=newSize;
	FillLine();
}

ModelVertex *ModelEditor::getVertices(){
	return vertex_m;
}

int ModelEditor::getnVertices(){
	return header_m->nVertices;
}

ModelRenderFlags *ModelEditor::getRenderflags(){
	return renderflags_m;
}
int ModelEditor::getnRenderflags(){
	return header_m->nRenderFlags;
}

void ModelEditor::newRenderflags(ModelRenderFlags *flags,int num){
	_Tn("new Renderflags");
	FillLine();
	int newSize=FileSize+sizeof(ModelRenderFlags)*num;
	char *n=f;
	f=SMemNew(newSize);
	memcpy(f,n,FileSize);
	SetStructure();
	header_m->ofsRenderFlags=FileSize;
	header_m->nRenderFlags=num;
	memcpy(f+FileSize,flags,num*sizeof(ModelRenderFlags));
	FileSize=newSize;
	FillLine();
}

ColorHelper ModelEditor::getColor(){
	_Tn("getColor()");
	ColorHelper ret=ColorHelper();
	ret.setnColors(header_m->nColors);
	for(int i=0;i<header_m->nColors;i++){
		//nTimes should always be nKeys...
		//aswell as Alphaval==Colorval?
		ret.setnSubs(i,color_m[i].color.nKeys);
		ret.setInterpolation(i,color_m[i].color.type);
		AnimSubStructure *ctimesub;
		AnimSubStructure *ckeysub;
		AnimSubStructure *atimesub;
		AnimSubStructure *akeysub;
		int offset=color_m[i].color.ofsTimes;
		ctimesub=getAnimblockData(ctimesub,offset);
		offset=color_m[i].color.ofsKeys;
		ckeysub=getAnimblockData(ckeysub,offset);
		offset=color_m[i].opacity.ofsTimes;
		atimesub=getAnimblockData(atimesub,offset);
		offset=color_m[i].opacity.ofsKeys;
		akeysub=getAnimblockData(akeysub,offset);
		for(int j=0;j<color_m[i].color.nTimes;j++){
				ShortHelper *svalues=new ShortHelper[atimesub[j].n];
				int nvals=atimesub[j].n;
				short *ts=getAnimblockData(ts,akeysub[j].ofs);
				uint32 *tt=getAnimblockData(tt,atimesub[j].ofs);
				for(int k=0;k<atimesub[j].n;k++){
					svalues[k].timestamp=tt[k];
					svalues[k].value=ts[k];
				}
				Vec3Helper *cvalues=new Vec3Helper[ctimesub[j].n];
				Vec3D *cf=getAnimblockData(cf,ckeysub[j].ofs);
				uint32 *ct=getAnimblockData(ct,ctimesub[j].ofs);
				for(int k=0;k<ctimesub[j].n;k++){
					cvalues[k].timestamp=ct[k];
					cvalues[k].value=cf[k];
				}
				ret.info[i][j].setData(cvalues,svalues,nvals);
		}
	}
	return ret;
}

void ModelEditor::newColors(ColorHelper help){
	_Tn("newColors()");
	int newSize=FileSize+sizeof(ModelColorDef)*help.nColors;
	char *n=f;
	f=SMemNew(newSize);
	memcpy(f,n,FileSize);
	SetStructure();
	header_m->ofsColors=FileSize;
	header_m->nColors=help.nColors;
	FileSize=newSize;
	FillLine();
	AnimSubStructure **ctimesub=new AnimSubStructure*[help.nColors];
	AnimSubStructure **ckeysub=new AnimSubStructure*[help.nColors];
	AnimSubStructure **atimesub=new AnimSubStructure*[help.nColors];
	AnimSubStructure **akeysub=new AnimSubStructure*[help.nColors];
	int nPre=0;
	for(int i=0;i<help.nColors;i++){
		color_m[i].color.nKeys=help.nSubs[i];
		color_m[i].color.nTimes=help.nSubs[i];
		color_m[i].color.seq=-1;
		color_m[i].color.type=help.interpolation[i];
		color_m[i].color.ofsTimes=FileSize+nPre*sizeof(AnimSubStructure);
		color_m[i].color.ofsKeys=FileSize+(nPre+1)*sizeof(AnimSubStructure);
		ctimesub[i]=new AnimSubStructure[help.nSubs[i]];
		ckeysub[i]=new AnimSubStructure[help.nSubs[i]];
		color_m[i].opacity.nKeys=help.nSubs[i];
		color_m[i].opacity.nTimes=help.nSubs[i];
		color_m[i].opacity.seq=-1;
		color_m[i].opacity.type=help.interpolation[i];
		color_m[i].opacity.ofsTimes=FileSize+(nPre+2)*sizeof(AnimSubStructure);
		color_m[i].opacity.ofsKeys=FileSize+(nPre+3)*sizeof(AnimSubStructure);
		atimesub[i]=new AnimSubStructure[help.nSubs[i]];
		akeysub[i]=new AnimSubStructure[help.nSubs[i]];
		for(int k=0;k<help.nSubs[i];k++){
			ctimesub[i][k].n=help.info[i][k].nValues;
			ckeysub[i][k].n=help.info[i][k].nValues;
			atimesub[i][k].n=help.info[i][k].nValues;
			akeysub[i][k].n=help.info[i][k].nValues;
		}
		nPre+=4*help.nSubs[i];
	}
	/**
	 * 4* oO have to check if I can remove this
	 * in transparency too!!
	 */
	newSize=FileSize+4*nPre*sizeof(AnimSubStructure);
	n=f;
	f=SMemNew(newSize);
	memcpy(f,n,FileSize);
	SetStructure();
	int nSubs=0;
	for(int i=0;i<header_m->nColors;i++){
		memcpy(f+FileSize+nSubs*sizeof(AnimSubStructure),ctimesub[i],sizeof(AnimSubStructure)*help.nSubs[i]);
		memcpy(f+FileSize+(nSubs+1)*sizeof(AnimSubStructure),ckeysub[i],sizeof(AnimSubStructure)*help.nSubs[i]);
		memcpy(f+FileSize+(nSubs+2)*sizeof(AnimSubStructure),atimesub[i],sizeof(AnimSubStructure)*help.nSubs[i]);
		memcpy(f+FileSize+(nSubs+3)*sizeof(AnimSubStructure),akeysub[i],sizeof(AnimSubStructure)*help.nSubs[i]);
		ctimesub[i]=(AnimSubStructure *)(f+FileSize+nSubs*sizeof(AnimSubStructure));
		ckeysub[i]=(AnimSubStructure *)(f+FileSize+(nSubs+1)*sizeof(AnimSubStructure));
		atimesub[i]=(AnimSubStructure *)(f+FileSize+nSubs*sizeof(AnimSubStructure));
		akeysub[i]=(AnimSubStructure *)(f+FileSize+(nSubs+1)*sizeof(AnimSubStructure));
		nSubs+=4*help.nSubs[i];
	}
	int ofsSize=FileSize;
	FileSize=newSize;
	FillLine();
	nSubs=0;
	for(int i=0;i<header_m->nColors;i++){
		//we need to reset the pointers whenever FileSize has changed..
		for(int j=0;j<help.nSubs[i];j++){
			//n was set before.. I think xD
			ctimesub[i]=(AnimSubStructure *)(f+ofsSize+nSubs*sizeof(AnimSubStructure));
			ctimesub[i][j].ofs=FileSize;
			uint32 *ctime=new uint32[help.info[i][j].nValues];
			for(int k=0;k<help.info[i][j].nValues;k++){
				ctime[k]=help.info[i][j].colvalues[k].timestamp;
			}
			writeAnimblockData(ctime,help.info[i][j].nValues);
			ckeysub[i]=(AnimSubStructure *)(f+ofsSize+(nSubs+1)*sizeof(AnimSubStructure));
			ckeysub[i][j].ofs=FileSize;
			Vec3D *cval=new Vec3D[help.info[i][j].nValues];
			for(int k=0;k<help.info[i][j].nValues;k++){
				cval[k]=help.info[i][j].colvalues[k].value;
			}
			writeAnimblockData(cval,help.info[i][j].nValues);

			atimesub[i]=(AnimSubStructure *)(f+ofsSize+(nSubs+2)*sizeof(AnimSubStructure));
			atimesub[i][j].ofs=FileSize;
			uint32 *atime=new uint32[help.info[i][j].nValues];
			for(int k=0;k<help.info[i][j].nValues;k++){
				atime[k]=help.info[i][j].tvalues[k].timestamp;
			}
			writeAnimblockData(atime,help.info[i][j].nValues);
			akeysub[i]=(AnimSubStructure *)(f+ofsSize+(nSubs+3)*sizeof(AnimSubStructure));
			akeysub[i][j].ofs=FileSize;
			short *aval=new short[help.info[i][j].nValues];
			for(int k=0;k<help.info[i][j].nValues;k++){
				aval[k]=help.info[i][j].tvalues[k].value;
			}
			writeAnimblockData(aval,help.info[i][j].nValues);
		}
		nSubs+=4*help.nSubs[i];
	}
	FillLine();
}


TextureHelper ModelEditor::getTextures(){
	_Tn("GetTextures");
	TextureHelper help=TextureHelper();
	help.setnTextures(header_m->nTextures);
	for(int i=0;i<header_m->nTextures;i++){
		help.setTexture(texture_m[i],i);
		char *name=getAnimblockData(name,texture_m[i].nameOfs);
		for(int k=0;k<texture_m[i].nameLen;k++){
			help.Texnames[i][k]=name[k];
		}
	}
	return help;

}

void ModelEditor::newTexAnim(TexAnimHelper help){
	_Tn("newTexAnim");
	int newSize=FileSize+sizeof(ModelTexAnimDef)*help.nTexAnims;
	char *n=f;
	f=SMemNew(newSize);
	memcpy(f,n,FileSize);
	memcpy(f+FileSize,help.TexAnims,sizeof(ModelTexAnimDef)*help.nTexAnims);
	SetStructure();
	header_m->ofsTexAnims=FileSize;
	header_m->nTexAnims=help.nTexAnims;
	FileSize=newSize;
	FillLine();
	AnimSubStructure **transtimesub=new AnimSubStructure*[help.nTexAnims];
	AnimSubStructure **transkeysub=new AnimSubStructure*[help.nTexAnims];
	AnimSubStructure **rottimesub=new AnimSubStructure*[help.nTexAnims];
	AnimSubStructure **rotkeysub=new AnimSubStructure*[help.nTexAnims];
	AnimSubStructure **scaletimesub=new AnimSubStructure*[help.nTexAnims];
	AnimSubStructure **scalekeysub=new AnimSubStructure*[help.nTexAnims];
	int nPre=0;
	for(int i=0;i<help.nTexAnims;i++){
		texanim_m[i].trans.nKeys=help.nSubs[i];
		texanim_m[i].trans.nTimes=help.nSubs[i];
		texanim_m[i].trans.ofsTimes=FileSize+nPre*sizeof(AnimSubStructure);
		texanim_m[i].trans.ofsKeys=FileSize+(nPre+1)*sizeof(AnimSubStructure);
		transtimesub[i]=new AnimSubStructure[help.nSubs[i]];
		transkeysub[i]=new AnimSubStructure[help.nSubs[i]];
		for(int k=0;k<help.nSubs[i];k++){
			transtimesub[i][k].n=help.info[i][k].nValues[0];
			transkeysub[i][k].n=help.info[i][k].nValues[0];
		}
		nPre+=2*help.nSubs[i];
	}
	for(int i=0;i<help.nTexAnims;i++){
		texanim_m[i].rot.nKeys=help.nSubs[i];
		texanim_m[i].rot.nTimes=help.nSubs[i];
		texanim_m[i].rot.ofsTimes=FileSize+nPre*sizeof(AnimSubStructure);
		texanim_m[i].rot.ofsKeys=FileSize+(nPre+1)*sizeof(AnimSubStructure);
		rottimesub[i]=new AnimSubStructure[help.nSubs[i]];
		rotkeysub[i]=new AnimSubStructure[help.nSubs[i]];
		for(int k=0;k<help.nSubs[i];k++){
			rottimesub[i][k].n=help.info[i][k].nValues[1];
			rotkeysub[i][k].n=help.info[i][k].nValues[1];
		}
		nPre+=2*help.nSubs[i];
	}
	for(int i=0;i<help.nTexAnims;i++){
		texanim_m[i].scale.nKeys=help.nSubs[i];
		texanim_m[i].scale.nTimes=help.nSubs[i];
		texanim_m[i].scale.ofsTimes=FileSize+nPre*sizeof(AnimSubStructure);
		texanim_m[i].scale.ofsKeys=FileSize+(nPre+1)*sizeof(AnimSubStructure);
		scaletimesub[i]=new AnimSubStructure[help.nSubs[i]];
		scalekeysub[i]=new AnimSubStructure[help.nSubs[i]];
		for(int k=0;k<help.nSubs[i];k++){
			scaletimesub[i][k].n=help.info[i][k].nValues[2];
			scalekeysub[i][k].n=help.info[i][k].nValues[2];
		}
		nPre+=2*help.nSubs[i];
	}
	newSize=FileSize+6*nPre*sizeof(AnimSubStructure);
	n=f;
	f=SMemNew(newSize);
	memcpy(f,n,FileSize);
	SetStructure();
	int nSubs=0;
	for(int i=0;i<header_m->nTexAnims;i++){
		memcpy(f+FileSize+nSubs*sizeof(AnimSubStructure),transtimesub[i],sizeof(AnimSubStructure)*help.nSubs[i]);
		memcpy(f+FileSize+(nSubs+1)*sizeof(AnimSubStructure),transkeysub[i],sizeof(AnimSubStructure)*help.nSubs[i]);
		transtimesub[i]=(AnimSubStructure *)(f+FileSize+nSubs*sizeof(AnimSubStructure));
		transkeysub[i]=(AnimSubStructure *)(f+FileSize+(nSubs+1)*sizeof(AnimSubStructure));
		nSubs+=2*help.nSubs[i];
	}
	for(int i=0;i<header_m->nTexAnims;i++){
		memcpy(f+FileSize+nSubs*sizeof(AnimSubStructure),rottimesub[i],sizeof(AnimSubStructure)*help.nSubs[i]);
		memcpy(f+FileSize+(nSubs+1)*sizeof(AnimSubStructure),rotkeysub[i],sizeof(AnimSubStructure)*help.nSubs[i]);
		rottimesub[i]=(AnimSubStructure *)(f+FileSize+nSubs*sizeof(AnimSubStructure));
		rotkeysub[i]=(AnimSubStructure *)(f+FileSize+(nSubs+1)*sizeof(AnimSubStructure));
		nSubs+=2*help.nSubs[i];
	}
	for(int i=0;i<header_m->nTexAnims;i++){
		memcpy(f+FileSize+nSubs*sizeof(AnimSubStructure),scaletimesub[i],sizeof(AnimSubStructure)*help.nSubs[i]);
		memcpy(f+FileSize+(nSubs+1)*sizeof(AnimSubStructure),scalekeysub[i],sizeof(AnimSubStructure)*help.nSubs[i]);
		scaletimesub[i]=(AnimSubStructure *)(f+FileSize+nSubs*sizeof(AnimSubStructure));
		scalekeysub[i]=(AnimSubStructure *)(f+FileSize+(nSubs+1)*sizeof(AnimSubStructure));
		nSubs+=2*help.nSubs[i];
	}
	int ofsSize=FileSize;
	FileSize=newSize;
	FillLine();
	nSubs=0;
	for(int i=0;i<header_m->nTexAnims;i++){
		//we need to reset the pointers whenever FileSize has changed..
		for(int j=0;j<help.nSubs[i];j++){
			//n was set before.. I think xD
			transtimesub[i]=(AnimSubStructure *)(f+ofsSize+nSubs*sizeof(AnimSubStructure));
			transtimesub[i][j].ofs=FileSize;
			uint32 *time=new uint32[help.info[i][j].nValues[0]];
			for(int k=0;k<help.info[i][j].nValues[0];k++){
				time[k]=help.info[i][j].trans[k].timestamp;
			}
			writeAnimblockData(time,help.info[i][j].nValues[0]);

			transkeysub[i]=(AnimSubStructure *)(f+ofsSize+(nSubs+1)*sizeof(AnimSubStructure));
			transkeysub[i][j].ofs=FileSize;
			Vec3D *val=new Vec3D[help.info[i][j].nValues[0]];
			for(int k=0;k<help.info[i][j].nValues[0];k++){
				val[k]=help.info[i][j].trans[k].value;
			}
			writeAnimblockData(val,help.info[i][j].nValues[0]);
		}
		nSubs+=2*help.nSubs[i];
	}
	for(int i=0;i<header_m->nTexAnims;i++){
		//we need to reset the pointers whenever FileSize has changed..
		for(int j=0;j<help.nSubs[i];j++){
			//n was set before.. I think xD
			rottimesub[i]=(AnimSubStructure *)(f+ofsSize+nSubs*sizeof(AnimSubStructure));
			rottimesub[i][j].ofs=FileSize;
			uint32 *time=new uint32[help.info[i][j].nValues[1]];
			for(int k=0;k<help.info[i][j].nValues[1];k++){
				time[k]=help.info[i][j].rot[k].timestamp;
			}
			writeAnimblockData(time,help.info[i][j].nValues[1]);

			rotkeysub[i]=(AnimSubStructure *)(f+ofsSize+(nSubs+1)*sizeof(AnimSubStructure));
			rotkeysub[i][j].ofs=FileSize;
			SQuaternion *val=new SQuaternion[help.info[i][j].nValues[1]];
			for(int k=0;k<help.info[i][j].nValues[1];k++){
				val[k]=help.info[i][j].rot[k].value;
			}
			writeAnimblockData(val,help.info[i][j].nValues[1]);
		}
		nSubs+=2*help.nSubs[i];
	}
	for(int i=0;i<header_m->nTexAnims;i++){
		//we need to reset the pointers whenever FileSize has changed..
		for(int j=0;j<help.nSubs[i];j++){
			//n was set before.. I think xD
			scaletimesub[i]=(AnimSubStructure *)(f+ofsSize+nSubs*sizeof(AnimSubStructure));
			scaletimesub[i][j].ofs=FileSize;
			uint32 *time=new uint32[help.info[i][j].nValues[2]];
			for(int k=0;k<help.info[i][j].nValues[2];k++){
				time[k]=help.info[i][j].scaling[k].timestamp;
			}
			writeAnimblockData(time,help.info[i][j].nValues[2]);

			scalekeysub[i]=(AnimSubStructure *)(f+ofsSize+(nSubs+1)*sizeof(AnimSubStructure));
			scalekeysub[i][j].ofs=FileSize;
			Vec3D *val=new Vec3D[help.info[i][j].nValues[2]];
			for(int k=0;k<help.info[i][j].nValues[2];k++){
				val[k]=help.info[i][j].scaling[k].value;
			}
			writeAnimblockData(val,help.info[i][j].nValues[2]);
		}
		nSubs+=2*help.nSubs[i];
	}
	FillLine();

}


TexAnimHelper ModelEditor::getTexAnim(){
	_Tn("getTexAnim()");
	TexAnimHelper ret=TexAnimHelper();
	ret.setnTexAnims(header_m->nTexAnims);
	ret.TexAnims=texanim_m;
	for(int i=0;i<header_m->nTexAnims;i++){
		//nTimes should always be nKeys...
		int nSubstructs=0;
		//it's much easier to handle if we use the greatest one...
		//aka the one which is not null :D
		if(texanim_m[i].trans.nKeys>nSubstructs)nSubstructs=texanim_m[i].trans.nKeys;
		if(texanim_m[i].scale.nKeys>nSubstructs)nSubstructs=texanim_m[i].scale.nKeys;
		if(texanim_m[i].rot.nKeys>nSubstructs)nSubstructs=texanim_m[i].rot.nKeys;
		ret.setnSubs(i,nSubstructs);
		AnimSubStructure *transtimesub;
		AnimSubStructure *transkeysub;
		int offset=texanim_m[i].trans.ofsTimes;
		transtimesub=getAnimblockData(transtimesub,offset);
		offset=texanim_m[i].trans.ofsKeys;
		transkeysub=getAnimblockData(transkeysub,offset);
		for(int j=0;j<texanim_m[i].trans.nKeys;j++){
				Vec3Helper *values=new Vec3Helper[transtimesub[j].n];
				int nvals=transtimesub[j].n;
				Vec3D *ts=getAnimblockData(ts,transkeysub[j].ofs);
				uint32 *tt=getAnimblockData(tt,transtimesub[j].ofs);
				for(int k=0;k<transtimesub[j].n;k++){
					values[k].timestamp=tt[k];
					values[k].value=ts[k];
				}
				ret.info[i][j].setTranslation(values,nvals);
		}
		AnimSubStructure *rottimesub;
		AnimSubStructure *rotkeysub;
		offset=texanim_m[i].rot.ofsTimes;
		rottimesub=getAnimblockData(rottimesub,offset);
		offset=texanim_m[i].rot.ofsKeys;
		rotkeysub=getAnimblockData(rotkeysub,offset);
		for(int j=0;j<texanim_m[i].rot.nKeys;j++){
				QuatHelper *values=new QuatHelper[rottimesub[j].n];
				int nvals=rottimesub[j].n;
				SQuaternion *ts=getAnimblockData(ts,rotkeysub[j].ofs);
				uint32 *tt=getAnimblockData(tt,rottimesub[j].ofs);
				for(int k=0;k<rottimesub[j].n;k++){
					values[k].timestamp=tt[k];
					values[k].value=ts[k];
				}
				ret.info[i][j].setRotation(values,nvals);
		}
		AnimSubStructure *scaletimesub;
		AnimSubStructure *scalekeysub;
		offset=texanim_m[i].scale.ofsTimes;
		scaletimesub=getAnimblockData(scaletimesub,offset);
		offset=texanim_m[i].scale.ofsKeys;
		scalekeysub=getAnimblockData(scalekeysub,offset);
		for(int j=0;j<texanim_m[i].scale.nKeys;j++){
				Vec3Helper *values=new Vec3Helper[scaletimesub[j].n];
				int nvals=scaletimesub[j].n;
				Vec3D *ts=getAnimblockData(ts,scalekeysub[j].ofs);
				uint32 *tt=getAnimblockData(tt,scaletimesub[j].ofs);
				for(int k=0;k<scaletimesub[j].n;k++){
					values[k].timestamp=tt[k];
					values[k].value=ts[k];
				}
				ret.info[i][j].setScaling(values,nvals);
		}
	}
	return ret;
}

ModelRibbonEmitterDef *ModelEditor::getRibbons(){
	return ribbon_m;

}
RibbonHelper *ModelEditor::getRibbonData(){
	_Tn("getRibbonData");
	RibbonHelper *ret=new RibbonHelper[header_m->nRibbonEmitters];
	for(int i=0;i<header_m->nRibbonEmitters;i++){
		//we use 0 because Ribbons normaly don't have animated things
		ret[i].zero=0;
		ret[i].one=1;
		int *ofs=getAnimblockData(ofs,ribbon_m[i].above.ofsKeys);
		float *TFloat=getAnimblockData(TFloat,ofs[0]);
		ret[i].above=TFloat[0];
		ofs=getAnimblockData(ofs,ribbon_m[i].below.ofsKeys);
		TFloat=getAnimblockData(TFloat,ofs[0]);
		ret[i].below=TFloat[0];
		ofs=getAnimblockData(ofs,ribbon_m[i].color.ofsKeys);
		for(int k=0;k<3;k++){
			TFloat=getAnimblockData(TFloat,ofs[k]);
			ret[i].color[k]=TFloat[0];
		}

		int *TInt=getAnimblockData(TInt,ribbon_m[i].ofsTextures);
		ret[i].texture=TInt[0];
		TInt=getAnimblockData(TInt,ribbon_m[i].ofsUnknown);
		ret[i].blend=TInt[0];
		ofs=getAnimblockData(ofs,ribbon_m[i].opacity.ofsKeys);
		short *TShort=getAnimblockData(TShort,ofs[0]);
		ret[i].opacity=TShort[0];
	}
	return ret;
}

ParticleHelper ModelEditor::getParticles(){
	_Tn("getParticles()");
	ParticleHelper ret=ParticleHelper();
	ret.setnParticles(header_m->nParticleEmitters);
	ret.particles=part_m;
	for(int i=0;i<header_m->nParticleEmitters;i++){
		char *temp=new char[part_m[i].lenModelName];
		memcpy(temp,f+part_m[i].ofsModelName,part_m[i].lenModelName);
		ret.info[i].setModelName(part_m[i].lenModelName,temp);

		temp=new char[part_m[i].lenParticleName];
		memcpy(temp,f+part_m[i].ofsParticleName,part_m[i].lenParticleName);
		ret.info[i].setParticleName(part_m[i].lenParticleName,temp);
	}
	for(int i=0;i<header_m->nParticleEmitters;i++){

		//the float animblocks
		{
		AnimSubStructure *timesub;
		AnimSubStructure *keysub;

		//readin Emissionspeed
		int offset=part_m[i].Emissionspeed.ofsTimes;
		timesub=getAnimblockData(timesub,offset);
		offset=part_m[i].Emissionspeed.ofsKeys;
		keysub=getAnimblockData(keysub,offset);
		ret.info[i].setnEmissionSpeed(part_m[i].Emissionspeed.nKeys);
		for(int j=0;j<part_m[i].Emissionspeed.nKeys;j++){
				FloatHelper *fhelp=new FloatHelper[timesub[j].n];
				float *f=getAnimblockData(f,keysub[j].ofs);
				uint32 *t=getAnimblockData(t,timesub[j].ofs);
				for(int k=0;k<timesub[j].n;k++){
					fhelp[k].timestamp=t[k];
					fhelp[k].value=f[k];
				}
				ret.info[i].setEmissionSpeed(fhelp,timesub[j].n,j);
		}
		//end Emissionspeed

		//readin speedvariation
		offset=part_m[i].SpeedVariation.ofsTimes;
		timesub=getAnimblockData(timesub,offset);
		offset=part_m[i].SpeedVariation.ofsKeys;
		keysub=getAnimblockData(keysub,offset);
		ret.info[i].setnSpeedVariation(part_m[i].SpeedVariation.nKeys);
		for(int j=0;j<part_m[i].SpeedVariation.nKeys;j++){

				FloatHelper *fhelp=new FloatHelper[timesub[j].n];
				float *f=getAnimblockData(f,keysub[j].ofs);
				uint32 *t=getAnimblockData(t,timesub[j].ofs);
				for(int k=0;k<timesub[j].n;k++){
					fhelp[k].timestamp=t[k];
					fhelp[k].value=f[k];
				}
				ret.info[i].setSpeedVariation(fhelp,timesub[j].n,j);

		}
		//end speed variation

		//readin varticalrange
		offset=part_m[i].VerticalRange.ofsTimes;
		timesub=getAnimblockData(timesub,offset);
		offset=part_m[i].VerticalRange.ofsKeys;
		keysub=getAnimblockData(keysub,offset);
		ret.info[i].setnVerticalRange(part_m[i].VerticalRange.nKeys);
		for(int j=0;j<part_m[i].VerticalRange.nKeys;j++){
				FloatHelper *fhelp=new FloatHelper[timesub[j].n];
				float *f=getAnimblockData(f,keysub[j].ofs);
				uint32 *t=getAnimblockData(t,timesub[j].ofs);
				for(int k=0;k<timesub[j].n;k++){
					fhelp[k].timestamp=t[k];
					fhelp[k].value=f[k];
				}
				ret.info[i].setVerticalRange(fhelp,timesub[j].n,j);

		}
		//end vertical range

		//readin HorizontalRange
		offset=part_m[i].HorizontalRange.ofsTimes;
		timesub=getAnimblockData(timesub,offset);
		offset=part_m[i].HorizontalRange.ofsKeys;
		keysub=getAnimblockData(keysub,offset);
		ret.info[i].setnHorizontalRange(part_m[i].HorizontalRange.nKeys);
		for(int j=0;j<part_m[i].HorizontalRange.nKeys;j++){
				FloatHelper *fhelp=new FloatHelper[timesub[j].n];
				float *f=getAnimblockData(f,keysub[j].ofs);
				uint32 *t=getAnimblockData(t,timesub[j].ofs);
				for(int k=0;k<timesub[j].n;k++){
					fhelp[k].timestamp=t[k];
					fhelp[k].value=f[k];
				}
				ret.info[i].setHorizontalRange(fhelp,timesub[j].n,j);

		}
		//end horizontal range

		//readin gravity
		offset=part_m[i].Gravity.ofsTimes;
		timesub=getAnimblockData(timesub,offset);
		offset=part_m[i].Gravity.ofsKeys;
		keysub=getAnimblockData(keysub,offset);
		ret.info[i].setnGravity(part_m[i].Gravity.nKeys);
		for(int j=0;j<part_m[i].Gravity.nKeys;j++){
				FloatHelper *fhelp=new FloatHelper[timesub[j].n];
				float *f=getAnimblockData(f,keysub[j].ofs);
				uint32 *t=getAnimblockData(t,timesub[j].ofs);
				for(int k=0;k<timesub[j].n;k++){
					fhelp[k].timestamp=t[k];
					fhelp[k].value=f[k];
				}
				ret.info[i].setGravity(fhelp,timesub[j].n,j);

		}
		//end gravity

		//readin lifespan
		offset=part_m[i].Lifespan.ofsTimes;
		timesub=getAnimblockData(timesub,offset);
		offset=part_m[i].Lifespan.ofsKeys;
		keysub=getAnimblockData(keysub,offset);
		ret.info[i].setnLifespan(part_m[i].Lifespan.nKeys);
		for(int j=0;j<part_m[i].Lifespan.nKeys;j++){
				FloatHelper *fhelp=new FloatHelper[timesub[j].n];
				float *f=getAnimblockData(f,keysub[j].ofs);
				uint32 *t=getAnimblockData(t,timesub[j].ofs);
				for(int k=0;k<timesub[j].n;k++){
					fhelp[k].timestamp=t[k];
					fhelp[k].value=f[k];
				}
				ret.info[i].setLifespan(fhelp,timesub[j].n,j);

		}
		//end lifespan

		//readin EmissionRate
		offset=part_m[i].EmissionRate.ofsTimes;
		timesub=getAnimblockData(timesub,offset);
		offset=part_m[i].EmissionRate.ofsKeys;
		keysub=getAnimblockData(keysub,offset);
		ret.info[i].setnEmissionRate(part_m[i].EmissionRate.nKeys);
		for(int j=0;j<part_m[i].EmissionRate.nKeys;j++){
				FloatHelper *fhelp=new FloatHelper[timesub[j].n];
				float *f=getAnimblockData(f,keysub[j].ofs);
				uint32 *t=getAnimblockData(t,timesub[j].ofs);
				for(int k=0;k<timesub[j].n;k++){
					fhelp[k].timestamp=t[k];
					fhelp[k].value=f[k];
				}
				ret.info[i].setEmissionRate(fhelp,timesub[j].n,j);
		}
		//end EmissionRate

		//readin EmissionAreaLength
		offset=part_m[i].EmissionAreaLength.ofsTimes;
		timesub=getAnimblockData(timesub,offset);
		offset=part_m[i].EmissionAreaLength.ofsKeys;
		keysub=getAnimblockData(keysub,offset);
		ret.info[i].setnEmissionAreaLength(part_m[i].EmissionAreaLength.nKeys);
		for(int j=0;j<part_m[i].EmissionAreaLength.nKeys;j++){
				FloatHelper *fhelp=new FloatHelper[timesub[j].n];
				float *f=getAnimblockData(f,keysub[j].ofs);
				uint32 *t=getAnimblockData(t,timesub[j].ofs);
				for(int k=0;k<timesub[j].n;k++){
					fhelp[k].timestamp=t[k];
					fhelp[k].value=f[k];
				}
				ret.info[i].setEmissionAreaLength(fhelp,timesub[j].n,j);

		}
		//end EmissionAreaLength

		//readin EmissionAreaWidth
		offset=part_m[i].EmissionAreaWidth.ofsTimes;
		timesub=getAnimblockData(timesub,offset);
		offset=part_m[i].EmissionAreaWidth.ofsKeys;
		keysub=getAnimblockData(keysub,offset);
		ret.info[i].setnEmissionAreaWidth(part_m[i].EmissionAreaWidth.nKeys);
		for(int j=0;j<part_m[i].EmissionAreaWidth.nKeys;j++){
				FloatHelper *fhelp=new FloatHelper[timesub[j].n];
				float *f=getAnimblockData(f,keysub[j].ofs);
				uint32 *t=getAnimblockData(t,timesub[j].ofs);
				for(int k=0;k<timesub[j].n;k++){
					fhelp[k].timestamp=t[k];
					fhelp[k].value=f[k];
				}
				ret.info[i].setEmissionAreaWidth(fhelp,timesub[j].n,j);

		}
		//end EmissionAreaWidth

		//readin Gravity2
		offset=part_m[i].Gravity2.ofsTimes;
		timesub=getAnimblockData(timesub,offset);
		offset=part_m[i].Gravity2.ofsKeys;
		keysub=getAnimblockData(keysub,offset);
		ret.info[i].setnGravity2(part_m[i].Gravity2.nKeys);
		for(int j=0;j<part_m[i].Gravity2.nKeys;j++){
				FloatHelper *fhelp=new FloatHelper[timesub[j].n];
				float *fl=getAnimblockData(fl,keysub[j].ofs);
				uint32 *t=getAnimblockData(t,timesub[j].ofs);
				for(int k=0;k<timesub[j].n;k++){
					fhelp[k].timestamp=t[k];
					fhelp[k].value=fl[k];
				}
				ret.info[i].setGravity2(fhelp,timesub[j].n,j);

		}
		//end Gravity2

		//and nonetheless the enabled one
		offset=part_m[i].Enabled.ofsTimes;
		timesub=getAnimblockData(timesub,offset);
		offset=part_m[i].Enabled.ofsKeys;
		keysub=getAnimblockData(keysub,offset);
		ret.info[i].setnEnabled(part_m[i].Enabled.nTimes);
		for(int j=0;j<part_m[i].Enabled.nKeys;j++){
				BoolHelper *bhelp=new BoolHelper[timesub[j].n];
				char *b=getAnimblockData(b,keysub[j].ofs);
				uint32 *t=getAnimblockData(t,timesub[j].ofs);
				for(int k=0;k<timesub[j].n;k++){
					bhelp[k].timestamp=t[k];
					bhelp[k].value=b[k];
				}
				ret.info[i].setEnabled(bhelp,timesub[j].n,j);
		}
		//end enabled

		}


		{//now the fake animblocks
			//readin colors
			{

			Vec3Helper_f *colhelp=new Vec3Helper_f[part_m[i].colors.nKeys];
			short *t=getAnimblockData(t,part_m[i].colors.ofsTimes);
			Vec3D *col=getAnimblockData(col,part_m[i].colors.ofsKeys);
			for(int k=0;k<part_m[i].colors.nKeys;k++){
				colhelp[k].timestamp=t[k];
				colhelp[k].value=col[k];
			}
			ret.info[i].setColor(colhelp,part_m[i].colors.nKeys);
			}
			//end colors

			//readin opacity
			{
			ShortHelper_f *shelp=new ShortHelper_f[part_m[i].opacity.nKeys];
			short *t=getAnimblockData(t,part_m[i].opacity.ofsTimes);
			short *s=getAnimblockData(s,part_m[i].opacity.ofsKeys);
			for(int k=0;k<part_m[i].opacity.nKeys;k++){
				shelp[k].timestamp=t[k];
				shelp[k].value=s[k];
			}
			ret.info[i].setOpacity(shelp,part_m[i].opacity.nKeys);
			}
			//end opacity

			//readin sizes
			{
			Vec2Helper_f *shelp=new Vec2Helper_f[part_m[i].sizes.nKeys];
			short *t=getAnimblockData(t,part_m[i].sizes.ofsTimes);
			Vec2D *s=getAnimblockData(s,part_m[i].sizes.ofsKeys);
			for(int k=0;k<part_m[i].sizes.nKeys;k++){
				shelp[k].timestamp=t[k];
				shelp[k].value=s[k];
			}
			ret.info[i].setSize(shelp,part_m[i].sizes.nKeys);
			}
			//end sizes

			//readin intensity
			{
			ShortHelper_f *shelp=new ShortHelper_f[part_m[i].intensity.nKeys];
			short *t=getAnimblockData(t,part_m[i].intensity.ofsTimes);
			short *s=getAnimblockData(s,part_m[i].intensity.ofsKeys);
			for(int k=0;k<part_m[i].intensity.nKeys;k++){
				shelp[k].timestamp=t[k];
				shelp[k].value=s[k];
			}
			ret.info[i].setIntensity(shelp,part_m[i].intensity.nKeys);
			}
			//end intensity

			//readin unk
			{
			ShortHelper_f *shelp=new ShortHelper_f[part_m[i].unknownblock.nKeys];
			short *t=getAnimblockData(t,part_m[i].unknownblock.ofsTimes);
			short *s=getAnimblockData(s,part_m[i].unknownblock.ofsKeys);
			for(int k=0;k<part_m[i].unknownblock.nKeys;k++){
				shelp[k].timestamp=t[k];
				shelp[k].value=s[k];
			}
			ret.info[i].setUnk(shelp,part_m[i].unknownblock.nKeys);
			}
			//end unk
		}

	}
	return ret;

}

void ModelEditor::newParticles(ParticleHelper help){
	_Tn("New Particles");
	int newSize=FileSize+sizeof(ModelParticleEmitterDef)*help.nParticles;
	char *n=f;
	f=SMemNew(newSize);
	memcpy(f,n,FileSize);
	SetStructure();
	header_m->ofsParticleEmitters=FileSize;
	header_m->nParticleEmitters=help.nParticles;
	memcpy(f+FileSize,help.particles,help.nParticles*sizeof(ModelParticleEmitterDef));
	FileSize=newSize;
	FillLine();

	//start emissionspeed
	{
	AnimSubStructure **timesub=new AnimSubStructure*[help.nParticles];
	AnimSubStructure **keysub=new AnimSubStructure*[help.nParticles];
	int nPre=0;
	for(int i=0;i<help.nParticles;i++){
		part_m[i].Emissionspeed.ofsTimes=FileSize+nPre*sizeof(AnimSubStructure);
		part_m[i].Emissionspeed.ofsKeys=FileSize+(nPre+1)*sizeof(AnimSubStructure);
		timesub[i]=new AnimSubStructure[sizeof(help.info[i].nEmissionspeed)/sizeof(int)];
		keysub[i]=new AnimSubStructure[sizeof(help.info[i].nEmissionspeed)/sizeof(int)];
		for(int k=0;k<(sizeof(help.info[i].nEmissionspeed)/sizeof(int));k++){
			timesub[i][k].n=help.info[i].nEmissionspeed[k];
			keysub[i][k].n=help.info[i].nEmissionspeed[k];
		}
		nPre+=2*sizeof(help.info[i].nEmissionspeed)/sizeof(int);
	}
	newSize=FileSize+2*nPre*sizeof(AnimSubStructure);
	n=f;
	f=SMemNew(newSize);
	memcpy(f,n,FileSize);
	SetStructure();
	int nSubs=0;
	for(int i=0;i<header_m->nParticleEmitters;i++){
		memcpy(f+FileSize+nSubs*sizeof(AnimSubStructure),timesub[i],sizeof(AnimSubStructure)*(sizeof(help.info[i].nEmissionspeed)/sizeof(int)));
		memcpy(f+FileSize+(nSubs+1)*sizeof(AnimSubStructure),keysub[i],sizeof(AnimSubStructure)*(sizeof(help.info[i].nEmissionspeed)/sizeof(int)));
		timesub[i]=(AnimSubStructure *)(f+FileSize+nSubs*sizeof(AnimSubStructure));
		keysub[i]=(AnimSubStructure *)(f+FileSize+(nSubs+1)*sizeof(AnimSubStructure));
		nSubs+=2*sizeof(help.info[i].nEmissionspeed)/sizeof(int);
	}
	int ofsSize=FileSize;
	FileSize=newSize;
	FillLine();
	nSubs=0;
	for(int i=0;i<header_m->nParticleEmitters;i++){
		//we need to reset the pointers whenever FileSize has changed..
		for(int j=0;j<sizeof(help.info[i].nEmissionspeed)/sizeof(int);j++){
			//n was set before.. I think xD
			timesub[i]=(AnimSubStructure *)(f+ofsSize+nSubs*sizeof(AnimSubStructure));
			timesub[i][j].ofs=FileSize;
			uint32 *time=new uint32[help.info[i].nEmissionspeed[j]];
			for(int k=0;k<help.info[i].nEmissionspeed[j];k++){
				time[k]=help.info[i].Emissionspeed[j][k].timestamp;
			}
			writeAnimblockData(time,help.info[i].nEmissionspeed[j]);
			keysub[i]=(AnimSubStructure *)(f+ofsSize+(nSubs+1)*sizeof(AnimSubStructure));
			keysub[i][j].ofs=FileSize;
			float *val=new float[help.info[i].nEmissionspeed[j]];
			for(int k=0;k<help.info[i].nEmissionspeed[j];k++){
				val[k]=help.info[i].Emissionspeed[j][k].value;
			}
			writeAnimblockData(val,help.info[i].nEmissionspeed[j]);
		}
		nSubs+=2*sizeof(help.info[i].nEmissionspeed)/sizeof(int);
	}
	FillLine();
	}
	//end emissionspeed

	//start speed var
	{
	AnimSubStructure **timesub=new AnimSubStructure*[help.nParticles];
	AnimSubStructure **keysub=new AnimSubStructure*[help.nParticles];
	int nPre=0;
	for(int i=0;i<help.nParticles;i++){
		part_m[i].SpeedVariation.ofsTimes=FileSize+nPre*sizeof(AnimSubStructure);
		part_m[i].SpeedVariation.ofsKeys=FileSize+(nPre+1)*sizeof(AnimSubStructure);
		timesub[i]=new AnimSubStructure[sizeof(help.info[i].nSpeedVariation)/sizeof(int)];
		keysub[i]=new AnimSubStructure[sizeof(help.info[i].nSpeedVariation)/sizeof(int)];
		for(int k=0;k<(sizeof(help.info[i].nSpeedVariation)/sizeof(int));k++){
			timesub[i][k].n=help.info[i].nSpeedVariation[k];
			keysub[i][k].n=help.info[i].nSpeedVariation[k];
		}
		nPre+=2*sizeof(help.info[i].nSpeedVariation)/sizeof(int);
	}
	newSize=FileSize+2*nPre*sizeof(AnimSubStructure);
	n=f;
	f=SMemNew(newSize);
	memcpy(f,n,FileSize);
	SetStructure();
	int nSubs=0;
	for(int i=0;i<header_m->nParticleEmitters;i++){
		memcpy(f+FileSize+nSubs*sizeof(AnimSubStructure),timesub[i],sizeof(AnimSubStructure)*(sizeof(help.info[i].nSpeedVariation)/sizeof(int)));
		memcpy(f+FileSize+(nSubs+1)*sizeof(AnimSubStructure),keysub[i],sizeof(AnimSubStructure)*(sizeof(help.info[i].nSpeedVariation)/sizeof(int)));
		timesub[i]=(AnimSubStructure *)(f+FileSize+nSubs*sizeof(AnimSubStructure));
		keysub[i]=(AnimSubStructure *)(f+FileSize+(nSubs+1)*sizeof(AnimSubStructure));
		nSubs+=2*sizeof(help.info[i].nSpeedVariation)/sizeof(int);
	}
	int ofsSize=FileSize;
	FileSize=newSize;
	FillLine();
	nSubs=0;
	for(int i=0;i<header_m->nParticleEmitters;i++){
		//we need to reset the pointers whenever FileSize has changed..
		for(int j=0;j<sizeof(help.info[i].nSpeedVariation)/sizeof(int);j++){
			//n was set before.. I think xD
			timesub[i]=(AnimSubStructure *)(f+ofsSize+nSubs*sizeof(AnimSubStructure));
			timesub[i][j].ofs=FileSize;
			uint32 *time=new uint32[help.info[i].nSpeedVariation[j]];
			for(int k=0;k<help.info[i].nSpeedVariation[j];k++){
				time[k]=help.info[i].SpeedVariation[j][k].timestamp;
			}
			writeAnimblockData(time,help.info[i].nSpeedVariation[j]);
			keysub[i]=(AnimSubStructure *)(f+ofsSize+(nSubs+1)*sizeof(AnimSubStructure));
			keysub[i][j].ofs=FileSize;
			float *val=new float[help.info[i].nSpeedVariation[j]];
			for(int k=0;k<help.info[i].nSpeedVariation[j];k++){
				val[k]=help.info[i].SpeedVariation[j][k].value;
			}
			writeAnimblockData(val,help.info[i].nSpeedVariation[j]);
		}
		nSubs+=2*sizeof(help.info[i].nSpeedVariation)/sizeof(int);
	}
	FillLine();
	}
	//end speed var

	//start VerticalRange
	{
	AnimSubStructure **timesub=new AnimSubStructure*[help.nParticles];
	AnimSubStructure **keysub=new AnimSubStructure*[help.nParticles];
	int nPre=0;
	for(int i=0;i<help.nParticles;i++){
		part_m[i].VerticalRange.ofsTimes=FileSize+nPre*sizeof(AnimSubStructure);
		part_m[i].VerticalRange.ofsKeys=FileSize+(nPre+1)*sizeof(AnimSubStructure);
		timesub[i]=new AnimSubStructure[sizeof(help.info[i].nVerticalRange)/sizeof(int)];
		keysub[i]=new AnimSubStructure[sizeof(help.info[i].nVerticalRange)/sizeof(int)];
		for(int k=0;k<(sizeof(help.info[i].nVerticalRange)/sizeof(int));k++){
			timesub[i][k].n=help.info[i].nVerticalRange[k];
			keysub[i][k].n=help.info[i].nVerticalRange[k];
		}
		nPre+=2*sizeof(help.info[i].nVerticalRange)/sizeof(int);
	}
	newSize=FileSize+2*nPre*sizeof(AnimSubStructure);
	n=f;
	f=SMemNew(newSize);
	memcpy(f,n,FileSize);
	SetStructure();
	int nSubs=0;
	for(int i=0;i<header_m->nParticleEmitters;i++){
		memcpy(f+FileSize+nSubs*sizeof(AnimSubStructure),timesub[i],sizeof(AnimSubStructure)*(sizeof(help.info[i].nVerticalRange)/sizeof(int)));
		memcpy(f+FileSize+(nSubs+1)*sizeof(AnimSubStructure),keysub[i],sizeof(AnimSubStructure)*(sizeof(help.info[i].nVerticalRange)/sizeof(int)));
		timesub[i]=(AnimSubStructure *)(f+FileSize+nSubs*sizeof(AnimSubStructure));
		keysub[i]=(AnimSubStructure *)(f+FileSize+(nSubs+1)*sizeof(AnimSubStructure));
		nSubs+=2*sizeof(help.info[i].nVerticalRange)/sizeof(int);
	}
	int ofsSize=FileSize;
	FileSize=newSize;
	FillLine();
	nSubs=0;
	for(int i=0;i<header_m->nParticleEmitters;i++){
		//we need to reset the pointers whenever FileSize has changed..
		for(int j=0;j<sizeof(help.info[i].nVerticalRange)/sizeof(int);j++){
			//n was set before.. I think xD
			timesub[i]=(AnimSubStructure *)(f+ofsSize+nSubs*sizeof(AnimSubStructure));
			timesub[i][j].ofs=FileSize;
			uint32 *time=new uint32[help.info[i].nVerticalRange[j]];
			for(int k=0;k<help.info[i].nVerticalRange[j];k++){
				time[k]=help.info[i].VerticalRange[j][k].timestamp;
			}
			writeAnimblockData(time,help.info[i].nVerticalRange[j]);
			keysub[i]=(AnimSubStructure *)(f+ofsSize+(nSubs+1)*sizeof(AnimSubStructure));
			keysub[i][j].ofs=FileSize;
			float *val=new float[help.info[i].nVerticalRange[j]];
			for(int k=0;k<help.info[i].nVerticalRange[j];k++){
				val[k]=help.info[i].VerticalRange[j][k].value;
			}
			writeAnimblockData(val,help.info[i].nVerticalRange[j]);
		}
		nSubs+=2*sizeof(help.info[i].nVerticalRange)/sizeof(int);
	}
	FillLine();
	}
	//end VerticalRange

	//start HorizontalRange
	{
	AnimSubStructure **timesub=new AnimSubStructure*[help.nParticles];
	AnimSubStructure **keysub=new AnimSubStructure*[help.nParticles];
	int nPre=0;
	for(int i=0;i<help.nParticles;i++){
		part_m[i].HorizontalRange.ofsTimes=FileSize+nPre*sizeof(AnimSubStructure);
		part_m[i].HorizontalRange.ofsKeys=FileSize+(nPre+1)*sizeof(AnimSubStructure);
		timesub[i]=new AnimSubStructure[sizeof(help.info[i].nHorizontalRange)/sizeof(int)];
		keysub[i]=new AnimSubStructure[sizeof(help.info[i].nHorizontalRange)/sizeof(int)];
		for(int k=0;k<(sizeof(help.info[i].nHorizontalRange)/sizeof(int));k++){
			timesub[i][k].n=help.info[i].nHorizontalRange[k];
			keysub[i][k].n=help.info[i].nHorizontalRange[k];
		}
		nPre+=2*sizeof(help.info[i].nHorizontalRange)/sizeof(int);
	}
	newSize=FileSize+2*nPre*sizeof(AnimSubStructure);
	n=f;
	f=SMemNew(newSize);
	memcpy(f,n,FileSize);
	SetStructure();
	int nSubs=0;
	for(int i=0;i<header_m->nParticleEmitters;i++){
		memcpy(f+FileSize+nSubs*sizeof(AnimSubStructure),timesub[i],sizeof(AnimSubStructure)*(sizeof(help.info[i].nHorizontalRange)/sizeof(int)));
		memcpy(f+FileSize+(nSubs+1)*sizeof(AnimSubStructure),keysub[i],sizeof(AnimSubStructure)*(sizeof(help.info[i].nHorizontalRange)/sizeof(int)));
		timesub[i]=(AnimSubStructure *)(f+FileSize+nSubs*sizeof(AnimSubStructure));
		keysub[i]=(AnimSubStructure *)(f+FileSize+(nSubs+1)*sizeof(AnimSubStructure));
		nSubs+=2*sizeof(help.info[i].nHorizontalRange)/sizeof(int);
	}
	int ofsSize=FileSize;
	FileSize=newSize;
	FillLine();
	nSubs=0;
	for(int i=0;i<header_m->nParticleEmitters;i++){
		//we need to reset the pointers whenever FileSize has changed..
		for(int j=0;j<sizeof(help.info[i].nHorizontalRange)/sizeof(int);j++){
			//n was set before.. I think xD
			timesub[i]=(AnimSubStructure *)(f+ofsSize+nSubs*sizeof(AnimSubStructure));
			timesub[i][j].ofs=FileSize;
			uint32 *time=new uint32[help.info[i].nHorizontalRange[j]];
			for(int k=0;k<help.info[i].nHorizontalRange[j];k++){
				time[k]=help.info[i].HorizontalRange[j][k].timestamp;
			}
			writeAnimblockData(time,help.info[i].nHorizontalRange[j]);
			keysub[i]=(AnimSubStructure *)(f+ofsSize+(nSubs+1)*sizeof(AnimSubStructure));
			keysub[i][j].ofs=FileSize;
			float *val=new float[help.info[i].nHorizontalRange[j]];
			for(int k=0;k<help.info[i].nHorizontalRange[j];k++){
				val[k]=help.info[i].HorizontalRange[j][k].value;
			}
			writeAnimblockData(val,help.info[i].nHorizontalRange[j]);
		}
		nSubs+=2*sizeof(help.info[i].nHorizontalRange)/sizeof(int);
	}
	FillLine();
	}
	//end HorizontalRange

	//start Gravity
	{
	AnimSubStructure **timesub=new AnimSubStructure*[help.nParticles];
	AnimSubStructure **keysub=new AnimSubStructure*[help.nParticles];
	int nPre=0;
	for(int i=0;i<help.nParticles;i++){
		part_m[i].Gravity.ofsTimes=FileSize+nPre*sizeof(AnimSubStructure);
		part_m[i].Gravity.ofsKeys=FileSize+(nPre+1)*sizeof(AnimSubStructure);
		timesub[i]=new AnimSubStructure[sizeof(help.info[i].nGravity)/sizeof(int)];
		keysub[i]=new AnimSubStructure[sizeof(help.info[i].nGravity)/sizeof(int)];
		for(int k=0;k<(sizeof(help.info[i].nGravity)/sizeof(int));k++){
			timesub[i][k].n=help.info[i].nGravity[k];
			keysub[i][k].n=help.info[i].nGravity[k];
		}
		nPre+=2*sizeof(help.info[i].nGravity)/sizeof(int);
	}
	newSize=FileSize+2*nPre*sizeof(AnimSubStructure);
	n=f;
	f=SMemNew(newSize);
	memcpy(f,n,FileSize);
	SetStructure();
	int nSubs=0;
	for(int i=0;i<header_m->nParticleEmitters;i++){
		memcpy(f+FileSize+nSubs*sizeof(AnimSubStructure),timesub[i],sizeof(AnimSubStructure)*(sizeof(help.info[i].nGravity)/sizeof(int)));
		memcpy(f+FileSize+(nSubs+1)*sizeof(AnimSubStructure),keysub[i],sizeof(AnimSubStructure)*(sizeof(help.info[i].nGravity)/sizeof(int)));
		timesub[i]=(AnimSubStructure *)(f+FileSize+nSubs*sizeof(AnimSubStructure));
		keysub[i]=(AnimSubStructure *)(f+FileSize+(nSubs+1)*sizeof(AnimSubStructure));
		nSubs+=2*sizeof(help.info[i].nGravity)/sizeof(int);
	}
	int ofsSize=FileSize;
	FileSize=newSize;
	FillLine();
	nSubs=0;
	for(int i=0;i<header_m->nParticleEmitters;i++){
		//we need to reset the pointers whenever FileSize has changed..
		for(int j=0;j<sizeof(help.info[i].nGravity)/sizeof(int);j++){
			//n was set before.. I think xD
			timesub[i]=(AnimSubStructure *)(f+ofsSize+nSubs*sizeof(AnimSubStructure));
			timesub[i][j].ofs=FileSize;
			uint32 *time=new uint32[help.info[i].nGravity[j]];
			for(int k=0;k<help.info[i].nGravity[j];k++){
				time[k]=help.info[i].Gravity[j][k].timestamp;
			}
			writeAnimblockData(time,help.info[i].nGravity[j]);
			keysub[i]=(AnimSubStructure *)(f+ofsSize+(nSubs+1)*sizeof(AnimSubStructure));
			keysub[i][j].ofs=FileSize;
			float *val=new float[help.info[i].nGravity[j]];
			for(int k=0;k<help.info[i].nGravity[j];k++){
				val[k]=help.info[i].Gravity[j][k].value;
			}
			writeAnimblockData(val,help.info[i].nGravity[j]);
		}
		nSubs+=2*sizeof(help.info[i].nGravity)/sizeof(int);
	}
	FillLine();
	}
	//end Gravity

	//start Lifespan
	{
	AnimSubStructure **timesub=new AnimSubStructure*[help.nParticles];
	AnimSubStructure **keysub=new AnimSubStructure*[help.nParticles];
	int nPre=0;
	for(int i=0;i<help.nParticles;i++){
		part_m[i].Lifespan.ofsTimes=FileSize+nPre*sizeof(AnimSubStructure);
		part_m[i].Lifespan.ofsKeys=FileSize+(nPre+1)*sizeof(AnimSubStructure);
		timesub[i]=new AnimSubStructure[sizeof(help.info[i].nLifespan)/sizeof(int)];
		keysub[i]=new AnimSubStructure[sizeof(help.info[i].nLifespan)/sizeof(int)];
		for(int k=0;k<(sizeof(help.info[i].nLifespan)/sizeof(int));k++){
			timesub[i][k].n=help.info[i].nLifespan[k];
			keysub[i][k].n=help.info[i].nLifespan[k];
		}
		nPre+=2*sizeof(help.info[i].nLifespan)/sizeof(int);
	}
	newSize=FileSize+2*nPre*sizeof(AnimSubStructure);
	n=f;
	f=SMemNew(newSize);
	memcpy(f,n,FileSize);
	SetStructure();
	int nSubs=0;
	for(int i=0;i<header_m->nParticleEmitters;i++){
		memcpy(f+FileSize+nSubs*sizeof(AnimSubStructure),timesub[i],sizeof(AnimSubStructure)*(sizeof(help.info[i].nLifespan)/sizeof(int)));
		memcpy(f+FileSize+(nSubs+1)*sizeof(AnimSubStructure),keysub[i],sizeof(AnimSubStructure)*(sizeof(help.info[i].nLifespan)/sizeof(int)));
		timesub[i]=(AnimSubStructure *)(f+FileSize+nSubs*sizeof(AnimSubStructure));
		keysub[i]=(AnimSubStructure *)(f+FileSize+(nSubs+1)*sizeof(AnimSubStructure));
		nSubs+=2*sizeof(help.info[i].nLifespan)/sizeof(int);
	}
	int ofsSize=FileSize;
	FileSize=newSize;
	FillLine();
	nSubs=0;
	for(int i=0;i<header_m->nParticleEmitters;i++){
		//we need to reset the pointers whenever FileSize has changed..
		for(int j=0;j<sizeof(help.info[i].nLifespan)/sizeof(int);j++){
			//n was set before.. I think xD
			timesub[i]=(AnimSubStructure *)(f+ofsSize+nSubs*sizeof(AnimSubStructure));
			timesub[i][j].ofs=FileSize;
			uint32 *time=new uint32[help.info[i].nLifespan[j]];
			for(int k=0;k<help.info[i].nLifespan[j];k++){
				time[k]=help.info[i].Lifespan[j][k].timestamp;
			}
			writeAnimblockData(time,help.info[i].nLifespan[j]);
			keysub[i]=(AnimSubStructure *)(f+ofsSize+(nSubs+1)*sizeof(AnimSubStructure));
			keysub[i][j].ofs=FileSize;
			float *val=new float[help.info[i].nLifespan[j]];
			for(int k=0;k<help.info[i].nLifespan[j];k++){
				val[k]=help.info[i].Lifespan[j][k].value;
			}
			writeAnimblockData(val,help.info[i].nLifespan[j]);
		}
		nSubs+=2*sizeof(help.info[i].nLifespan)/sizeof(int);
	}
	FillLine();
	}
	//end Lifespan

	//start EmissionRate
	{
	AnimSubStructure **timesub=new AnimSubStructure*[help.nParticles];
	AnimSubStructure **keysub=new AnimSubStructure*[help.nParticles];
	int nPre=0;
	for(int i=0;i<help.nParticles;i++){
		part_m[i].EmissionRate.ofsTimes=FileSize+nPre*sizeof(AnimSubStructure);
		part_m[i].EmissionRate.ofsKeys=FileSize+(nPre+1)*sizeof(AnimSubStructure);
		timesub[i]=new AnimSubStructure[sizeof(help.info[i].nEmissionRate)/sizeof(int)];
		keysub[i]=new AnimSubStructure[sizeof(help.info[i].nEmissionRate)/sizeof(int)];
		for(int k=0;k<(sizeof(help.info[i].nEmissionRate)/sizeof(int));k++){
			timesub[i][k].n=help.info[i].nEmissionRate[k];
			keysub[i][k].n=help.info[i].nEmissionRate[k];
		}
		nPre+=2*sizeof(help.info[i].nEmissionRate)/sizeof(int);
	}
	newSize=FileSize+2*nPre*sizeof(AnimSubStructure);
	n=f;
	f=SMemNew(newSize);
	memcpy(f,n,FileSize);
	SetStructure();
	int nSubs=0;
	for(int i=0;i<header_m->nParticleEmitters;i++){
		memcpy(f+FileSize+nSubs*sizeof(AnimSubStructure),timesub[i],sizeof(AnimSubStructure)*(sizeof(help.info[i].nEmissionRate)/sizeof(int)));
		memcpy(f+FileSize+(nSubs+1)*sizeof(AnimSubStructure),keysub[i],sizeof(AnimSubStructure)*(sizeof(help.info[i].nEmissionRate)/sizeof(int)));
		timesub[i]=(AnimSubStructure *)(f+FileSize+nSubs*sizeof(AnimSubStructure));
		keysub[i]=(AnimSubStructure *)(f+FileSize+(nSubs+1)*sizeof(AnimSubStructure));
		nSubs+=2*sizeof(help.info[i].nEmissionRate)/sizeof(int);
	}
	int ofsSize=FileSize;
	FileSize=newSize;
	FillLine();
	nSubs=0;
	for(int i=0;i<header_m->nParticleEmitters;i++){
		//we need to reset the pointers whenever FileSize has changed..
		for(int j=0;j<sizeof(help.info[i].nEmissionRate)/sizeof(int);j++){
			//n was set before.. I think xD
			timesub[i]=(AnimSubStructure *)(f+ofsSize+nSubs*sizeof(AnimSubStructure));
			timesub[i][j].ofs=FileSize;
			uint32 *time=new uint32[help.info[i].nEmissionRate[j]];
			for(int k=0;k<help.info[i].nEmissionRate[j];k++){
				time[k]=help.info[i].EmissionRate[j][k].timestamp;
			}
			writeAnimblockData(time,help.info[i].nEmissionRate[j]);
			keysub[i]=(AnimSubStructure *)(f+ofsSize+(nSubs+1)*sizeof(AnimSubStructure));
			keysub[i][j].ofs=FileSize;
			float *val=new float[help.info[i].nEmissionRate[j]];
			for(int k=0;k<help.info[i].nEmissionRate[j];k++){
				val[k]=help.info[i].EmissionRate[j][k].value;
			}
			writeAnimblockData(val,help.info[i].nEmissionRate[j]);
		}
		nSubs+=2*sizeof(help.info[i].nEmissionRate)/sizeof(int);
	}
	FillLine();
	}
	//end EmissionRate

	//start EmissionAreaLength
	{
	AnimSubStructure **timesub=new AnimSubStructure*[help.nParticles];
	AnimSubStructure **keysub=new AnimSubStructure*[help.nParticles];
	int nPre=0;
	for(int i=0;i<help.nParticles;i++){
		part_m[i].EmissionAreaLength.ofsTimes=FileSize+nPre*sizeof(AnimSubStructure);
		part_m[i].EmissionAreaLength.ofsKeys=FileSize+(nPre+1)*sizeof(AnimSubStructure);
		timesub[i]=new AnimSubStructure[sizeof(help.info[i].nEmissionAreaLength)/sizeof(int)];
		keysub[i]=new AnimSubStructure[sizeof(help.info[i].nEmissionAreaLength)/sizeof(int)];
		for(int k=0;k<(sizeof(help.info[i].nEmissionAreaLength)/sizeof(int));k++){
			timesub[i][k].n=help.info[i].nEmissionAreaLength[k];
			keysub[i][k].n=help.info[i].nEmissionAreaLength[k];
		}
		nPre+=2*sizeof(help.info[i].nEmissionAreaLength)/sizeof(int);
	}
	newSize=FileSize+2*nPre*sizeof(AnimSubStructure);
	n=f;
	f=SMemNew(newSize);
	memcpy(f,n,FileSize);
	SetStructure();
	int nSubs=0;
	for(int i=0;i<header_m->nParticleEmitters;i++){
		memcpy(f+FileSize+nSubs*sizeof(AnimSubStructure),timesub[i],sizeof(AnimSubStructure)*(sizeof(help.info[i].nEmissionAreaLength)/sizeof(int)));
		memcpy(f+FileSize+(nSubs+1)*sizeof(AnimSubStructure),keysub[i],sizeof(AnimSubStructure)*(sizeof(help.info[i].nEmissionAreaLength)/sizeof(int)));
		timesub[i]=(AnimSubStructure *)(f+FileSize+nSubs*sizeof(AnimSubStructure));
		keysub[i]=(AnimSubStructure *)(f+FileSize+(nSubs+1)*sizeof(AnimSubStructure));
		nSubs+=2*sizeof(help.info[i].nEmissionAreaLength)/sizeof(int);
	}
	int ofsSize=FileSize;
	FileSize=newSize;
	FillLine();
	nSubs=0;
	for(int i=0;i<header_m->nParticleEmitters;i++){
		//we need to reset the pointers whenever FileSize has changed..
		for(int j=0;j<sizeof(help.info[i].nEmissionAreaLength)/sizeof(int);j++){
			//n was set before.. I think xD
			timesub[i]=(AnimSubStructure *)(f+ofsSize+nSubs*sizeof(AnimSubStructure));
			timesub[i][j].ofs=FileSize;
			uint32 *time=new uint32[help.info[i].nEmissionAreaLength[j]];
			for(int k=0;k<help.info[i].nEmissionAreaLength[j];k++){
				time[k]=help.info[i].EmissionAreaLength[j][k].timestamp;
			}
			writeAnimblockData(time,help.info[i].nEmissionAreaLength[j]);
			keysub[i]=(AnimSubStructure *)(f+ofsSize+(nSubs+1)*sizeof(AnimSubStructure));
			keysub[i][j].ofs=FileSize;
			float *val=new float[help.info[i].nEmissionAreaLength[j]];
			for(int k=0;k<help.info[i].nEmissionAreaLength[j];k++){
				val[k]=help.info[i].EmissionAreaLength[j][k].value;
			}
			writeAnimblockData(val,help.info[i].nEmissionAreaLength[j]);
		}
		nSubs+=2*sizeof(help.info[i].nEmissionAreaLength)/sizeof(int);
	}
	FillLine();
	}
	//end EmissionAreaLength


	//start EmissionAreaWidth
	{
	AnimSubStructure **timesub=new AnimSubStructure*[help.nParticles];
	AnimSubStructure **keysub=new AnimSubStructure*[help.nParticles];
	int nPre=0;
	for(int i=0;i<help.nParticles;i++){
		part_m[i].EmissionAreaWidth.ofsTimes=FileSize+nPre*sizeof(AnimSubStructure);
		part_m[i].EmissionAreaWidth.ofsKeys=FileSize+(nPre+1)*sizeof(AnimSubStructure);
		timesub[i]=new AnimSubStructure[sizeof(help.info[i].nEmissionAreaWidth)/sizeof(int)];
		keysub[i]=new AnimSubStructure[sizeof(help.info[i].nEmissionAreaWidth)/sizeof(int)];
		for(int k=0;k<(sizeof(help.info[i].nEmissionAreaWidth)/sizeof(int));k++){
			timesub[i][k].n=help.info[i].nEmissionAreaWidth[k];
			keysub[i][k].n=help.info[i].nEmissionAreaWidth[k];
		}
		nPre+=2*sizeof(help.info[i].nEmissionAreaWidth)/sizeof(int);
	}
	newSize=FileSize+2*nPre*sizeof(AnimSubStructure);
	n=f;
	f=SMemNew(newSize);
	memcpy(f,n,FileSize);
	SetStructure();
	int nSubs=0;
	for(int i=0;i<header_m->nParticleEmitters;i++){
		memcpy(f+FileSize+nSubs*sizeof(AnimSubStructure),timesub[i],sizeof(AnimSubStructure)*(sizeof(help.info[i].nEmissionAreaWidth)/sizeof(int)));
		memcpy(f+FileSize+(nSubs+1)*sizeof(AnimSubStructure),keysub[i],sizeof(AnimSubStructure)*(sizeof(help.info[i].nEmissionAreaWidth)/sizeof(int)));
		timesub[i]=(AnimSubStructure *)(f+FileSize+nSubs*sizeof(AnimSubStructure));
		keysub[i]=(AnimSubStructure *)(f+FileSize+(nSubs+1)*sizeof(AnimSubStructure));
		nSubs+=2*sizeof(help.info[i].nEmissionAreaWidth)/sizeof(int);
	}
	int ofsSize=FileSize;
	FileSize=newSize;
	FillLine();
	nSubs=0;
	for(int i=0;i<header_m->nParticleEmitters;i++){
		//we need to reset the pointers whenever FileSize has changed..
		for(int j=0;j<sizeof(help.info[i].nEmissionAreaWidth)/sizeof(int);j++){
			//n was set before.. I think xD
			timesub[i]=(AnimSubStructure *)(f+ofsSize+nSubs*sizeof(AnimSubStructure));
			timesub[i][j].ofs=FileSize;
			uint32 *time=new uint32[help.info[i].nEmissionAreaWidth[j]];
			for(int k=0;k<help.info[i].nEmissionAreaWidth[j];k++){
				time[k]=help.info[i].EmissionAreaWidth[j][k].timestamp;
			}
			writeAnimblockData(time,help.info[i].nEmissionAreaWidth[j]);
			keysub[i]=(AnimSubStructure *)(f+ofsSize+(nSubs+1)*sizeof(AnimSubStructure));
			keysub[i][j].ofs=FileSize;
			float *val=new float[help.info[i].nEmissionAreaWidth[j]];
			for(int k=0;k<help.info[i].nEmissionAreaWidth[j];k++){
				val[k]=help.info[i].EmissionAreaWidth[j][k].value;
			}
			writeAnimblockData(val,help.info[i].nEmissionAreaWidth[j]);
		}
		nSubs+=2*sizeof(help.info[i].nEmissionAreaWidth)/sizeof(int);
	}
	FillLine();
	}
	//end EmissionAreaWidth

	//start Gravity2
	{
	AnimSubStructure **timesub=new AnimSubStructure*[help.nParticles];
	AnimSubStructure **keysub=new AnimSubStructure*[help.nParticles];
	int nPre=0;
	for(int i=0;i<help.nParticles;i++){
		part_m[i].Gravity2.ofsTimes=FileSize+nPre*sizeof(AnimSubStructure);
		part_m[i].Gravity2.ofsKeys=FileSize+(nPre+1)*sizeof(AnimSubStructure);
		timesub[i]=new AnimSubStructure[sizeof(help.info[i].nGravity2)/sizeof(int)];
		keysub[i]=new AnimSubStructure[sizeof(help.info[i].nGravity2)/sizeof(int)];
		for(int k=0;k<(sizeof(help.info[i].nGravity2)/sizeof(int));k++){
			timesub[i][k].n=help.info[i].nGravity2[k];
			keysub[i][k].n=help.info[i].nGravity2[k];
		}
		nPre+=2*sizeof(help.info[i].nGravity2)/sizeof(int);
	}
	newSize=FileSize+2*nPre*sizeof(AnimSubStructure);
	n=f;
	f=SMemNew(newSize);
	memcpy(f,n,FileSize);
	SetStructure();
	int nSubs=0;
	for(int i=0;i<header_m->nParticleEmitters;i++){
		memcpy(f+FileSize+nSubs*sizeof(AnimSubStructure),timesub[i],sizeof(AnimSubStructure)*(sizeof(help.info[i].nGravity2)/sizeof(int)));
		memcpy(f+FileSize+(nSubs+1)*sizeof(AnimSubStructure),keysub[i],sizeof(AnimSubStructure)*(sizeof(help.info[i].nGravity2)/sizeof(int)));
		timesub[i]=(AnimSubStructure *)(f+FileSize+nSubs*sizeof(AnimSubStructure));
		keysub[i]=(AnimSubStructure *)(f+FileSize+(nSubs+1)*sizeof(AnimSubStructure));
		nSubs+=2*sizeof(help.info[i].nGravity2)/sizeof(int);
	}
	int ofsSize=FileSize;
	FileSize=newSize;
	FillLine();
	nSubs=0;
	for(int i=0;i<header_m->nParticleEmitters;i++){
		//we need to reset the pointers whenever FileSize has changed..
		for(int j=0;j<sizeof(help.info[i].nGravity2)/sizeof(int);j++){
			//n was set before.. I think xD
			timesub[i]=(AnimSubStructure *)(f+ofsSize+nSubs*sizeof(AnimSubStructure));
			timesub[i][j].ofs=FileSize;
			uint32 *time=new uint32[help.info[i].nGravity2[j]];
			for(int k=0;k<help.info[i].nGravity2[j];k++){
				time[k]=help.info[i].Gravity2[j][k].timestamp;
			}
			writeAnimblockData(time,help.info[i].nGravity2[j]);
			keysub[i]=(AnimSubStructure *)(f+ofsSize+(nSubs+1)*sizeof(AnimSubStructure));
			keysub[i][j].ofs=FileSize;
			float *val=new float[help.info[i].nGravity2[j]];
			for(int k=0;k<help.info[i].nGravity2[j];k++){
				val[k]=help.info[i].Gravity2[j][k].value;
			}
			writeAnimblockData(val,help.info[i].nGravity2[j]);
		}
		nSubs+=2*sizeof(help.info[i].nGravity2)/sizeof(int);
	}
	FillLine();
	}
	//end Gravity2

	//start fake color
	{
	for(int i=0;i<header_m->nParticleEmitters;i++){
		part_m[i].colors.nTimes=help.info[i].nColor;
		part_m[i].colors.ofsTimes=FileSize;
		short *time=new short[help.info[i].nColor];
		for(int k=0;k<help.info[i].nColor;k++){
			time[k]=help.info[i].Color[k].timestamp;
		}
		writeAnimblockData(time,help.info[i].nColor);
		part_m[i].colors.nKeys=help.info[i].nColor;
		part_m[i].colors.ofsKeys=FileSize;
		Vec3D *val=new Vec3D[help.info[i].nColor];
		for(int k=0;k<help.info[i].nColor;k++){
			val[k]=help.info[i].Color[k].value;
		}
		writeAnimblockData(val,help.info[i].nColor);
	}
	FillLine();
	}
	//end fake color

	//start fake opacity
	{
	for(int i=0;i<header_m->nParticleEmitters;i++){
		part_m[i].opacity.nTimes=help.info[i].nOpacity;
		part_m[i].opacity.ofsTimes=FileSize;
		short *time=new short[help.info[i].nOpacity];
		for(int k=0;k<help.info[i].nOpacity;k++){
			time[k]=help.info[i].Opacity[k].timestamp;
		}
		writeAnimblockData(time,help.info[i].nOpacity);
		part_m[i].opacity.nKeys=help.info[i].nOpacity;
		part_m[i].opacity.ofsKeys=FileSize;
		short *val=new short[help.info[i].nOpacity];
		for(int k=0;k<help.info[i].nOpacity;k++){
			val[k]=help.info[i].Opacity[k].value;
		}
		writeAnimblockData(val,help.info[i].nOpacity);
	}
	FillLine();
	}
	//end fake opacity

	//start fake sizes
	{
	for(int i=0;i<header_m->nParticleEmitters;i++){
		part_m[i].sizes.nTimes=help.info[i].nSize;
		part_m[i].sizes.ofsTimes=FileSize;
		short *time=new short[help.info[i].nSize];
		for(int k=0;k<help.info[i].nSize;k++){
			time[k]=help.info[i].Size[k].timestamp;
		}
		writeAnimblockData(time,help.info[i].nSize);
		part_m[i].sizes.nKeys=help.info[i].nSize;
		part_m[i].sizes.ofsKeys=FileSize;
		Vec2D *val=new Vec2D[help.info[i].nSize];
		for(int k=0;k<help.info[i].nSize;k++){
			val[k]=help.info[i].Size[k].value;
		}
		writeAnimblockData(val,help.info[i].nSize);
	}
	FillLine();
	}
	//end fake sizes

	//start fake Intensity
	{
	for(int i=0;i<header_m->nParticleEmitters;i++){
		part_m[i].intensity.nTimes=help.info[i].nIntensity;
		part_m[i].intensity.ofsTimes=FileSize;
		short *time=new short[help.info[i].nIntensity];
		for(int k=0;k<help.info[i].nIntensity;k++){
			time[k]=help.info[i].Intensity[k].timestamp;
		}
		writeAnimblockData(time,help.info[i].nIntensity);
		part_m[i].intensity.nKeys=help.info[i].nIntensity;
		part_m[i].intensity.ofsKeys=FileSize;
		short *val=new short[help.info[i].nIntensity];
		for(int k=0;k<help.info[i].nIntensity;k++){
			val[k]=help.info[i].Intensity[k].value;
		}
		writeAnimblockData(val,help.info[i].nIntensity);
	}
	FillLine();
	}
	//end fake Intensity

	//start fake unk
	{
	for(int i=0;i<header_m->nParticleEmitters;i++){
		part_m[i].unknownblock.nTimes=help.info[i].nUnk;
		part_m[i].unknownblock.ofsTimes=FileSize;
		short *time=new short[help.info[i].nUnk];
		for(int k=0;k<help.info[i].nUnk;k++){
			time[k]=help.info[i].Unk[k].timestamp;
		}
		writeAnimblockData(time,help.info[i].nUnk);
		part_m[i].unknownblock.nKeys=help.info[i].nUnk;
		part_m[i].unknownblock.ofsKeys=FileSize;
		short *val=new short[help.info[i].nUnk];
		for(int k=0;k<help.info[i].nUnk;k++){
			val[k]=help.info[i].Unk[k].value;
		}
		writeAnimblockData(val,help.info[i].nUnk);
	}
	FillLine();
	}
	//end fake unk

	//start nEnabled
	{
	AnimSubStructure **timesub=new AnimSubStructure*[help.nParticles];
	AnimSubStructure **keysub=new AnimSubStructure*[help.nParticles];
	int nPre=0;
	for(int i=0;i<help.nParticles;i++){
		part_m[i].Enabled.ofsTimes=FileSize+nPre*sizeof(AnimSubStructure);
		part_m[i].Enabled.ofsKeys=FileSize+(nPre+1)*sizeof(AnimSubStructure);
		timesub[i]=new AnimSubStructure[sizeof(help.info[i].nEnabled)/sizeof(int)];
		keysub[i]=new AnimSubStructure[sizeof(help.info[i].nEnabled)/sizeof(int)];
		for(int k=0;k<(sizeof(help.info[i].nEnabled)/sizeof(int));k++){
			timesub[i][k].n=help.info[i].nEnabled[k];
			keysub[i][k].n=help.info[i].nEnabled[k];
		}
		nPre+=2*sizeof(help.info[i].nEnabled)/sizeof(int);
	}
	newSize=FileSize+2*nPre*sizeof(AnimSubStructure);
	n=f;
	f=SMemNew(newSize);
	memcpy(f,n,FileSize);
	SetStructure();
	int nSubs=0;
	for(int i=0;i<header_m->nParticleEmitters;i++){
		memcpy(f+FileSize+nSubs*sizeof(AnimSubStructure),timesub[i],sizeof(AnimSubStructure)*(sizeof(help.info[i].nEnabled)/sizeof(int)));
		memcpy(f+FileSize+(nSubs+1)*sizeof(AnimSubStructure),keysub[i],sizeof(AnimSubStructure)*(sizeof(help.info[i].nEnabled)/sizeof(int)));
		timesub[i]=(AnimSubStructure *)(f+FileSize+nSubs*sizeof(AnimSubStructure));
		keysub[i]=(AnimSubStructure *)(f+FileSize+(nSubs+1)*sizeof(AnimSubStructure));
		nSubs+=2*sizeof(help.info[i].nEnabled)/sizeof(int);
	}
	int ofsSize=FileSize;
	FileSize=newSize;
	FillLine();
	nSubs=0;
	for(int i=0;i<header_m->nParticleEmitters;i++){
		//we need to reset the pointers whenever FileSize has changed..
		for(int j=0;j<sizeof(help.info[i].nEnabled)/sizeof(int);j++){
			//n was set before.. I think xD
			timesub[i]=(AnimSubStructure *)(f+ofsSize+nSubs*sizeof(AnimSubStructure));
			timesub[i][j].ofs=FileSize;
			uint32 *time=new uint32[help.info[i].nEnabled[j]];
			for(int k=0;k<help.info[i].nEnabled[j];k++){
				time[k]=help.info[i].Enabled[j][k].timestamp;
			}
			writeAnimblockData(time,help.info[i].nEnabled[j]);
			keysub[i]=(AnimSubStructure *)(f+ofsSize+(nSubs+1)*sizeof(AnimSubStructure));
			keysub[i][j].ofs=FileSize;
			char *val=new char[help.info[i].nEnabled[j]];
			for(int k=0;k<help.info[i].nEnabled[j];k++){
				val[k]=help.info[i].Enabled[j][k].value;
			}
			writeAnimblockData(val,help.info[i].nEnabled[j]);
		}
		nSubs+=2*sizeof(help.info[i].nEnabled)/sizeof(int);
	}
	FillLine();
	}
	//end nEnabled

	//now the referenced files:
	//models
	for(int i=0;i<help.nParticles;i++){
		int newSize=FileSize+help.info[i].lengthModelName;
		char *n=f;
		f=SMemNew(newSize);
		memcpy(f,n,FileSize);
		SetStructure();
		part_m[i].ofsModelName=FileSize;
		memcpy(f+FileSize,help.info[i].ModelName,help.info[i].lengthModelName);
		FileSize=newSize;
		FillLine();
	}
	//particles
	for(int i=0;i<help.nParticles;i++){
		int newSize=FileSize+help.info[i].lengthParticleName;
		char *n=f;
		f=SMemNew(newSize);
		memcpy(f,n,FileSize);
		SetStructure();
		part_m[i].ofsParticleName=FileSize;
		memcpy(f+FileSize,help.info[i].ParticleName,help.info[i].lengthParticleName);
		FileSize=newSize;
		FillLine();
	}
}

Vec3D *ModelEditor::getBoundingVertices(){
	return boundvertices_m;

}
ModelBoundTriangle *ModelEditor::getBoundingTriangles(){
	return boundtriangle_m;

}
Vec3D *ModelEditor::getBoundingNormals(){
	return boundnormals_m;

}
ModelLightDef *ModelEditor::getLights(){
	return light_m;

}
LightHelper *ModelEditor::getLightData(){
	return NULL;
}
CamHelper ModelEditor::getCameras(){
	_Tn("getCameras");
	CamHelper ret=CamHelper();
	ret.setnCameras(header_m->nCameras);
	ret.camera=cam_m;
	for(int i=0;i<header_m->nCameras;i++){
		//nTimes should always be nKeys...
		ret.setnSubs(i,cam_m[i].rot.nKeys);
		AnimSubStructure *tartimesub;
		AnimSubStructure *tarkeysub;
		int offset=cam_m[i].transTarget.ofsTimes;
		tartimesub=getAnimblockData(tartimesub,offset);
		offset=cam_m[i].transTarget.ofsKeys;
		tarkeysub=getAnimblockData(tarkeysub,offset);
		for(int j=0;j<cam_m[i].transTarget.nKeys;j++){
				Vec3Helper *values=new Vec3Helper[tartimesub[j].n];
				int nvals=tartimesub[j].n;
				Vec3D *ts=getAnimblockData(ts,tarkeysub[j].ofs);
				uint32 *tt=getAnimblockData(tt,tartimesub[j].ofs);
				for(int k=0;k<tartimesub[j].n;k++){
					values[k].timestamp=tt[k];
					values[k].value=ts[k];
				}
				ret.info[i][j].setTranslationTarget(values,nvals);

		}
		AnimSubStructure *postimesub;
		AnimSubStructure *poskeysub;
		offset=cam_m[i].transPos.ofsTimes;
		postimesub=getAnimblockData(postimesub,offset);
		offset=cam_m[i].transTarget.ofsKeys;
		poskeysub=getAnimblockData(poskeysub,offset);
		for(int j=0;j<cam_m[i].transPos.nKeys;j++){
				Vec3Helper *values=new Vec3Helper[postimesub[j].n];
				int nvals=postimesub[j].n;
				Vec3D *ts=getAnimblockData(ts,poskeysub[j].ofs);
				uint32 *tt=getAnimblockData(tt,postimesub[j].ofs);
				for(int k=0;k<postimesub[j].n;k++){
					values[k].timestamp=tt[k];
					values[k].value=ts[k];
				}
				ret.info[i][j].setTranslationPosition(values,nvals);
		}
		AnimSubStructure *rottimesub;
		AnimSubStructure *rotkeysub;
		offset=cam_m[i].rot.ofsTimes;
		rottimesub=getAnimblockData(rottimesub,offset);
		offset=cam_m[i].rot.ofsKeys;
		rotkeysub=getAnimblockData(rotkeysub,offset);
		for(int j=0;j<cam_m[i].rot.nKeys;j++){
				Vec3Helper *values=new Vec3Helper[rottimesub[j].n];
				int nvals=rottimesub[j].n;
				Vec3D *ts=getAnimblockData(ts,rotkeysub[j].ofs);
				uint32 *tt=getAnimblockData(tt,rottimesub[j].ofs);
				for(int k=0;k<rottimesub[j].n;k++){
					values[k].timestamp=tt[k];
					values[k].value=ts[k];
				}
				ret.info[i][j].setRotation(values,nvals);
		}
	}
	return ret;
}

void ModelEditor::newCameras(CamHelper help){
	_Tn("newCameras");
	int newSize=FileSize+sizeof(ModelCameraDef)*help.nCameras;
	char *n=f;
	f=SMemNew(newSize);
	memcpy(f,n,FileSize);
	memcpy(f+FileSize,help.camera,sizeof(ModelCameraDef)*help.nCameras);
	SetStructure();
	header_m->ofsCameras=FileSize;
	header_m->nCameras=help.nCameras;
	FileSize=newSize;
	FillLine();
	AnimSubStructure **tartimesub=new AnimSubStructure*[help.nCameras];
	AnimSubStructure **tarkeysub=new AnimSubStructure*[help.nCameras];
	AnimSubStructure **postimesub=new AnimSubStructure*[help.nCameras];
	AnimSubStructure **poskeysub=new AnimSubStructure*[help.nCameras];
	AnimSubStructure **rottimesub=new AnimSubStructure*[help.nCameras];
	AnimSubStructure **rotkeysub=new AnimSubStructure*[help.nCameras];
	int nPre=0;
	for(int i=0;i<help.nCameras;i++){
		cam_m[i].transTarget.nKeys=help.nSubs[i];
		cam_m[i].transTarget.nTimes=help.nSubs[i];
		cam_m[i].transTarget.ofsTimes=FileSize+nPre*sizeof(AnimSubStructure);
		cam_m[i].transTarget.ofsKeys=FileSize+(nPre+1)*sizeof(AnimSubStructure);
		tartimesub[i]=new AnimSubStructure[help.nSubs[i]];
		tarkeysub[i]=new AnimSubStructure[help.nSubs[i]];
		for(int k=0;k<help.nSubs[i];k++){
			tartimesub[i][k].n=help.info[i][k].nValues[1];
			tarkeysub[i][k].n=help.info[i][k].nValues[1];
		}
		nPre+=2*help.nSubs[i];
	}
	for(int i=0;i<help.nCameras;i++){
		cam_m[i].transPos.nKeys=help.nSubs[i];
		cam_m[i].transPos.nTimes=help.nSubs[i];
		cam_m[i].transPos.ofsTimes=FileSize+nPre*sizeof(AnimSubStructure);
		cam_m[i].transPos.ofsKeys=FileSize+(nPre+1)*sizeof(AnimSubStructure);
		postimesub[i]=new AnimSubStructure[help.nSubs[i]];
		poskeysub[i]=new AnimSubStructure[help.nSubs[i]];
		for(int k=0;k<help.nSubs[i];k++){
			postimesub[i][k].n=help.info[i][k].nValues[2];
			poskeysub[i][k].n=help.info[i][k].nValues[2];
		}
		nPre+=2*help.nSubs[i];
	}
	for(int i=0;i<help.nCameras;i++){
		cam_m[i].rot.nKeys=help.nSubs[i];
		cam_m[i].rot.nTimes=help.nSubs[i];
		cam_m[i].rot.ofsTimes=FileSize+nPre*sizeof(AnimSubStructure);
		cam_m[i].rot.ofsKeys=FileSize+(nPre+1)*sizeof(AnimSubStructure);
		rottimesub[i]=new AnimSubStructure[help.nSubs[i]];
		rotkeysub[i]=new AnimSubStructure[help.nSubs[i]];
		for(int k=0;k<help.nSubs[i];k++){
			rottimesub[i][k].n=help.info[i][k].nValues[0];
			rotkeysub[i][k].n=help.info[i][k].nValues[0];
		}
		nPre+=2*help.nSubs[i];
	}
	newSize=FileSize+6*nPre*sizeof(AnimSubStructure);
	n=f;
	f=SMemNew(newSize);
	memcpy(f,n,FileSize);
	SetStructure();
	int nSubs=0;
	for(int i=0;i<header_m->nCameras;i++){
		memcpy(f+FileSize+nSubs*sizeof(AnimSubStructure),tartimesub[i],sizeof(AnimSubStructure)*help.nSubs[i]);
		memcpy(f+FileSize+(nSubs+1)*sizeof(AnimSubStructure),tarkeysub[i],sizeof(AnimSubStructure)*help.nSubs[i]);
		tartimesub[i]=(AnimSubStructure *)(f+FileSize+nSubs*sizeof(AnimSubStructure));
		tarkeysub[i]=(AnimSubStructure *)(f+FileSize+(nSubs+1)*sizeof(AnimSubStructure));
		nSubs+=2*help.nSubs[i];
	}
	for(int i=0;i<header_m->nCameras;i++){
		memcpy(f+FileSize+nSubs*sizeof(AnimSubStructure),postimesub[i],sizeof(AnimSubStructure)*help.nSubs[i]);
		memcpy(f+FileSize+(nSubs+1)*sizeof(AnimSubStructure),poskeysub[i],sizeof(AnimSubStructure)*help.nSubs[i]);
		postimesub[i]=(AnimSubStructure *)(f+FileSize+nSubs*sizeof(AnimSubStructure));
		poskeysub[i]=(AnimSubStructure *)(f+FileSize+(nSubs+1)*sizeof(AnimSubStructure));
		nSubs+=2*help.nSubs[i];
	}
	for(int i=0;i<header_m->nCameras;i++){
		memcpy(f+FileSize+nSubs*sizeof(AnimSubStructure),rottimesub[i],sizeof(AnimSubStructure)*help.nSubs[i]);
		memcpy(f+FileSize+(nSubs+1)*sizeof(AnimSubStructure),rotkeysub[i],sizeof(AnimSubStructure)*help.nSubs[i]);
		rottimesub[i]=(AnimSubStructure *)(f+FileSize+nSubs*sizeof(AnimSubStructure));
		rotkeysub[i]=(AnimSubStructure *)(f+FileSize+(nSubs+1)*sizeof(AnimSubStructure));
		nSubs+=2*help.nSubs[i];
	}
	int ofsSize=FileSize;
	FileSize=newSize;
	FillLine();
	nSubs=0;
	for(int i=0;i<header_m->nCameras;i++){
		//we need to reset the pointers whenever FileSize has changed..
		for(int j=0;j<help.nSubs[i];j++){
			//n was set before.. I think xD
			tartimesub[i]=(AnimSubStructure *)(f+ofsSize+nSubs*sizeof(AnimSubStructure));
			tartimesub[i][j].ofs=FileSize;
			uint32 *time=new uint32[help.info[i][j].nValues[1]];
			for(int k=0;k<help.info[i][j].nValues[1];k++){
				time[k]=help.info[i][j].translationtar[k].timestamp;
			}
			writeAnimblockData(time,help.info[i][j].nValues[1]);
			tarkeysub[i]=(AnimSubStructure *)(f+ofsSize+(nSubs+1)*sizeof(AnimSubStructure));
			tarkeysub[i][j].ofs=FileSize;
			Vec3D *val=new Vec3D[help.info[i][j].nValues[1]];
			for(int k=0;k<help.info[i][j].nValues[1];k++){
				val[k]=help.info[i][j].translationtar[k].value;
			}
			writeAnimblockData(val,help.info[i][j].nValues[1]);
		}
		nSubs+=2*help.nSubs[i];
	}
	for(int i=0;i<header_m->nCameras;i++){
		//we need to reset the pointers whenever FileSize has changed..
		for(int j=0;j<help.nSubs[i];j++){
			//n was set before.. I think xD
			postimesub[i]=(AnimSubStructure *)(f+ofsSize+nSubs*sizeof(AnimSubStructure));
			postimesub[i][j].ofs=FileSize;
			uint32 *time=new uint32[help.info[i][j].nValues[2]];
			for(int k=0;k<help.info[i][j].nValues[2];k++){
				time[k]=help.info[i][j].translationpos[k].timestamp;
			}
			writeAnimblockData(time,help.info[i][j].nValues[2]);
			poskeysub[i]=(AnimSubStructure *)(f+ofsSize+(nSubs+1)*sizeof(AnimSubStructure));
			poskeysub[i][j].ofs=FileSize;
			Vec3D *val=new Vec3D[help.info[i][j].nValues[2]];
			for(int k=0;k<help.info[i][j].nValues[2];k++){
				val[k]=help.info[i][j].translationpos[k].value;
			}
			writeAnimblockData(val,help.info[i][j].nValues[2]);
		}
		nSubs+=2*help.nSubs[i];
	}
	for(int i=0;i<header_m->nCameras;i++){
		//we need to reset the pointers whenever FileSize has changed..
		for(int j=0;j<help.nSubs[i];j++){
			//n was set before.. I think xD
			rottimesub[i]=(AnimSubStructure *)(f+ofsSize+nSubs*sizeof(AnimSubStructure));
			rottimesub[i][j].ofs=FileSize;
			uint32 *time=new uint32[help.info[i][j].nValues[0]];
			for(int k=0;k<help.info[i][j].nValues[0];k++){
				time[k]=help.info[i][j].rotation[k].timestamp;
			}
			writeAnimblockData(time,help.info[i][j].nValues[0]);
			rotkeysub[i]=(AnimSubStructure *)(f+ofsSize+(nSubs+1)*sizeof(AnimSubStructure));
			rotkeysub[i][j].ofs=FileSize;
			Vec3D *val=new Vec3D[help.info[i][j].nValues[0]];
			for(int k=0;k<help.info[i][j].nValues[0];k++){
				val[k]=help.info[i][j].rotation[k].value;
			}
			writeAnimblockData(val,help.info[i][j].nValues[0]);
		}
		nSubs+=2*help.nSubs[i];
	}
	FillLine();
	//I only know about two different cameras...
	int nCamLookup=2;
	newSize=FileSize+sizeof(short)*nCamLookup;
	n=f;
	f=SMemNew(newSize);
	memcpy(f,n,FileSize);
	SetStructure();
	header_m->ofsCameraLookup=FileSize;
	header_m->nCameraLookup=nCamLookup;
	SetStructure();
	for(int i=0;i<header_m->nCameraLookup;i++){
		for(int j=0;j<header_m->nCameras;j++){
			if(cam_m[j].id==i){
				camlookup_m[i]=j;break;
			}
			else camlookup_m[i]=-1;
		}
	}
	FileSize=newSize;
	FillLine();
}


ModelAttachmentDef *ModelEditor::getAttachments(){
	return 	attach_m;

}
int ModelEditor::getnAttachments(){
	return header_m->nAttachments;
}
void ModelEditor::newAttachments(ModelAttachmentDef*attachments,int num){
	_Tn("newAttachments");
	FillLine();
	int newSize=FileSize+sizeof(ModelAttachmentDef)*num;
	char *n=f;
	f=SMemNew(newSize);
	memcpy(f,n,FileSize);
	SetStructure();
	header_m->nAttachments=num;
	header_m->ofsAttachments=FileSize;
	FileSize=newSize;
	memcpy(f+header_m->ofsAttachments,attachments,num*sizeof(ModelAttachmentDef));
	SetStructure();
	for(int i=0;i<header_m->nAttachments;i++){
		//I null this because I saw no difference,
		//and without animblocks it is much easier
		attach_m[i].unk.nKeys=0;
		attach_m[i].unk.nTimes=0;
		attach_m[i].unk.ofsKeys=0;
		attach_m[i].unk.ofsTimes=0;
	}
	FillLine();
	//Now we create the lookup!

	//40 should be enough atm
	//maybe we need later more?
	int nLook=40;
	newSize=FileSize+nLook*sizeof(short);
	n=f;
	f=SMemNew(newSize);
	memcpy(f,n,FileSize);
	SetStructure();
	header_m->nAttachLookup=nLook;
	header_m->ofsAttachLookup=FileSize;
	FileSize=newSize;
	SetStructure();
	for(int i=0;i<header_m->nAttachLookup;i++){
		for(int j=0;j<header_m->nAttachments;j++){
			if(attach_m[j].id=i){
				attachlookup_m[i]=j;
				break;
			}
			else{
				attachlookup_m[i]=-1;
			}
		}
	}
	FillLine();

}
ModelAttach2 *ModelEditor::getAttachment2(){
	return 	attach2_m;
}


BoneHelper* ModelEditor::getBoneData(){
	_Tn("getBoneData");
	BoneHelper *help=new BoneHelper[header_m->nBones];
	for(int i=0;i<header_m->nBones;i++){
		AnimSubStructure *tr_timesub=(AnimSubStructure*)(f+bone_m[i].translation.ofsTimes);
		AnimSubStructure *tr_keysub=(AnimSubStructure*)(f+bone_m[i].translation.ofsKeys);
		AnimSubStructure *ro_timesub=(AnimSubStructure*)(f+bone_m[i].rotation.ofsTimes);
		AnimSubStructure *ro_keysub=(AnimSubStructure*)(f+bone_m[i].rotation.ofsKeys);
		AnimSubStructure *sc_timesub=(AnimSubStructure*)(f+bone_m[i].scaling.ofsTimes);
		AnimSubStructure *sc_keysub=(AnimSubStructure*)(f+bone_m[i].scaling.ofsKeys);

		help[i].setnAnim(header_m->nAnimations);
		for(int j=0;j<header_m->nAnimations;j++){
				Vec3Helper* vHelp=new Vec3Helper[tr_timesub[j].n];
				short *timestamp;Vec3D *value;
				timestamp=getAnimblockData(timestamp,tr_timesub[j].ofs);
				value=getAnimblockData(value,tr_keysub[j].ofs);
				for(int k=0;k<tr_timesub[j].n;k++){
					vHelp[k].timestamp=timestamp[k];
					vHelp[k].value=value[k];
				}
				help[i].info[j].setTranslation(vHelp,tr_timesub[j].n);

				QuatHelper* qHelp=new QuatHelper[ro_timesub[j].n];
				SQuaternion *qvalue;
				timestamp=getAnimblockData(timestamp,ro_timesub[j].ofs);
				qvalue=getAnimblockData(qvalue,ro_keysub[j].ofs);
				for(int k=0;k<ro_timesub[j].n;k++){
					qHelp[k].timestamp=timestamp[k];
					qHelp[k].value=qvalue[k];
				}
				help[i].info[j].setRotation(qHelp,ro_timesub[j].n);

				vHelp=new Vec3Helper[sc_timesub[j].n];
				timestamp=getAnimblockData(timestamp,sc_timesub[j].ofs);
				value=getAnimblockData(value,sc_keysub[j].ofs);
				for(int k=0;k<sc_timesub[j].n;k++){
					vHelp[k].timestamp=timestamp[k];
					vHelp[k].value=value[k];
				}
				help[i].info[j].setScaling(vHelp,sc_timesub[j].n);


		}
	}

	return help;
}

int ModelEditor::getnBones(){
	return header_m->nBones;
}


void ModelEditor::addTexture(ModelTextureDef tex,const char * texname){
	_Tn("addTexture");
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
	f=SMemNew(newSize);
	memcpy(f,n,FileSize);
	SetStructure();
	memcpy(f+FileSize,(f+header_m->ofsTextures),sizeof(ModelTextureDef)*header_m->nTextures);
	memcpy(f+FileSize+
			sizeof(ModelTextureDef)*(header_m->nTextures+1),texname,tex.nameLen);
	memcpy(f+FileSize+
			sizeof(ModelTextureDef)*(header_m->nTextures+1)+
			tex.nameLen+
			(16-(sizeof(tex.nameLen))%16),(f+header_m->ofsTexLookup),sizeof(short)*header_m->nTexLookup);
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

void ModelEditor::newTextures(TextureHelper help){
	_Tn("newTextures");
	FillLine();
	int newSize=FileSize+help.nTextures*sizeof(ModelTextureDef);
	for(int i=0;i<help.nTextures;i++){
		help.texdefs[i].nameOfs=newSize;
		newSize+=help.texdefs[i].nameLen
				+(16-((help.texdefs[i].nameLen)%16));
	}
	char *n=f;
	f=SMemNew(newSize);
	memcpy(f,n,FileSize);
	SetStructure();
	header_m->ofsTextures=FileSize;
	header_m->nTextures=help.nTextures;
	texture_m=(ModelTextureDef *)(f+header_m->ofsTextures);
	for(int i=0;i<help.nTextures;i++){
		texture_m[i]=help.texdefs[i];
		char *texname=(char *)(f+texture_m[i].nameOfs);
		for(int k=0;k<texture_m[i].nameLen;k++){
			texname[k]=help.Texnames[i][k];
		}
	}
	FileSize=newSize;
	FillLine();
}

void ModelEditor::addVertices(ModelVertex * vert, int nvert){
	_Tn("addVertices");
	FillLine();
	int newSize=FileSize+sizeof(ModelVertex)*(nvert+header_m->nVertices);
	char *newc=SMemNew(newSize);
	memcpy(newc,f,FileSize);
	f=newc;
	SetStructure();
	memcpy(f,(f+header_m->ofsVertices),sizeof(ModelVertex)*header_m->nVertices);
	memcpy(f+FileSize,vert,nvert*sizeof(ModelVertex));
	header_m->ofsVertices=FileSize;
	header_m->nVertices+=nvert;
	FileSize=newSize;
	FillLine();
}

void ModelEditor::newVertices(ModelVertex * vert, int nvert){
	_Tn("newVertices");
	FillLine();
	int newSize=FileSize+sizeof(ModelVertex)*(nvert);
	char *newc=SMemNew(newSize);
	memcpy(newc,f,FileSize);
	f=newc;
	SetStructure();
	memcpy(f+FileSize,vert,nvert*sizeof(ModelVertex));
	header_m->ofsVertices=FileSize;
	header_m->nVertices=nvert;
	FileSize=newSize;
	FillLine();
	SetStructure();
}

ModelAnimation* ModelEditor::getAnimations(){
	return anim_m;
}

int ModelEditor::getnAnimations(){
	return header_m->nAnimations;
}

/**
*Note: This will not add any animated bones!
*only Animationsdef will be added!
**/
void ModelEditor::addAnimations(ModelAnimation *anim,int nanim){
	_Tn("addAnimations");
	FillLine();
	int newSize=FileSize+sizeof(ModelAnimation)*(header_m->nAnimations+nanim);
	char *n=f;
	f=SMemNew(newSize);
	memcpy(f,n,FileSize);
	SetStructure();
	memcpy(f+FileSize,(f+header_m->ofsAnimations),sizeof(ModelAnimation)*(header_m->nAnimations));
	memcpy(f+FileSize+sizeof(ModelAnimation)*header_m->nAnimations,anim,nanim*sizeof(ModelAnimation));
	header_m->ofsAnimations=FileSize;
	header_m->nAnimations+=nanim;
	FileSize=newSize;
	FillLine();
	SetStructure();
}

/**
*Note: This will not add any animated bones!
*only Animationsdef will be added!
**/
void ModelEditor::newAnimations(ModelAnimation *anim,int nanim){
	_Tn("newAnimations");
	FillLine();
	int newSize=FileSize+sizeof(ModelAnimation)*(nanim);
	char *n=f;
	f=SMemNew(newSize);
	memcpy(f,n,FileSize);
	SetStructure();
	memcpy(f+FileSize,anim,nanim*sizeof(ModelAnimation));
	header_m->ofsAnimations=FileSize;
	header_m->nAnimations=nanim;
	FileSize=newSize;
	FillLine();
	SetStructure();
}

/**
*in fact..this is bullshit oO
*
*it should be more like:
*read in the animations and check for which animation they are...
**/
void ModelEditor::addAnimationLookup(int nanim){
	_Tn("addAnimLookup");
	FillLine();
	int newSize=FileSize+sizeof(uint16)*(header_m->nAnimationLookup+nanim);
	char *n=f;
	f=SMemNew(newSize);
	memcpy(f,n,FileSize);
	SetStructure();
	memcpy(f+FileSize,(f+header_m->ofsAnimationLookup),sizeof(uint16)*(header_m->nAnimationLookup));
	header_m->ofsAnimationLookup=FileSize;
	animlookup_m=(int16*)(f+header_m->ofsAnimationLookup);
	for(int i=0;i<nanim;i++){
		//not really...
		animlookup_m[i+header_m->nAnimationLookup]=i+header_m->nAnimationLookup;
	}
	header_m->nAnimationLookup+=nanim;
	FileSize=newSize;
	FillLine();
	SetStructure();
}

void ModelEditor::newAnimationLookup(int nanim){
	_Tn("newAnimationLookup");
	FillLine();
	int newSize=FileSize+sizeof(uint16)*(nanim);
	char *n=f;
	f=SMemNew(newSize);
	memcpy(f,n,FileSize);
	SetStructure();
	header_m->ofsAnimationLookup=FileSize;
	header_m->nAnimationLookup=nanim;
	animlookup_m=(int16*)(f+header_m->ofsAnimationLookup);
	for(int i=0;i<nanim;i++){
		for(int j=0;j<header_m->nAnimations;j++){
			if(anim_m[j].animID==i){
				animlookup_m[i]=(int16)j;
				break;
			}
			else{
				animlookup_m[i]=-1;
			}
		}
	}
	FileSize=newSize;
	FillLine();
}

void ModelEditor::newBones(ModelBoneDef *bones,int nBones,BoneHelper *help){
	_Tn("newBones");
	FillLine();
	char *n=f;
	int newSize=FileSize+sizeof(ModelBoneDef)*nBones;
	f=SMemNew(newSize);
	memcpy(f,n,FileSize);
	SetStructure();
	memcpy(f+FileSize,bones,sizeof(ModelBoneDef)*nBones);
	header_m->nBones=nBones;
	header_m->ofsBones=FileSize;
	SetStructure();
	FileSize=newSize;
	FillLine();
	for(int i=0;i<nBones;i++){
		printf("Processing Bone: %d\n",i);
		bone_m[i].translation.nTimes=help[i].nAnimations;
		bone_m[i].translation.ofsTimes=FileSize;
		bone_m[i].translation.nKeys=help[i].nAnimations;
		bone_m[i].translation.ofsKeys=FileSize
			+sizeof(AnimSubStructure)*help[i].nAnimations;
		bone_m[i].rotation.nTimes=help[i].nAnimations;
		bone_m[i].rotation.ofsTimes=FileSize
			+2*sizeof(AnimSubStructure)*help[i].nAnimations;
		bone_m[i].rotation.nKeys=help[i].nAnimations;
		bone_m[i].rotation.ofsKeys=FileSize
			+3*sizeof(AnimSubStructure)*help[i].nAnimations;
		bone_m[i].scaling.nTimes=help[i].nAnimations;
		bone_m[i].scaling.ofsTimes=FileSize
			+4*sizeof(AnimSubStructure)*help[i].nAnimations;
		bone_m[i].scaling.nKeys=help[i].nAnimations;
		bone_m[i].scaling.ofsKeys=FileSize
			+5*sizeof(AnimSubStructure)*help[i].nAnimations;
		writeSubStructs(help[i]);
	}
}
//we need an alternative for this but well xD
//it's the easiest one :D
void ModelEditor::newBoneLookup(){
	_Tn("newBoneLookup");
	int newSize=FileSize+header_m->nBones*sizeof(short);
	char *n=f;
	f=SMemNew(newSize);
	memcpy(f,n,FileSize);
	SetStructure();
	header_m->nBoneLookupTable=header_m->nBones;
	header_m->ofsBoneLookupTable=FileSize;
	FileSize=newSize;
	SetStructure();
	for(int i=0;i<header_m->nBoneLookupTable;i++){
		bonelookup_m[i]=i;
	}
	FillLine();
}

void ModelEditor::newTexLookup(){
	_Tn("newTexLookup");
	int newSize=FileSize+header_m->nTextures*sizeof(short);
	char *n=f;
	f=SMemNew(newSize);
	memcpy(f,n,FileSize);
	SetStructure();
	header_m->nTexLookup=header_m->nTextures;
	header_m->ofsTexLookup=FileSize;
	FileSize=newSize;
	SetStructure();
	for(int i=0;i<header_m->nTexLookup;i++){
		texturelookup_m[i]=i;
	}
	FillLine();
}

void ModelEditor::newTexUnitLookup(){
	_Tn("newTexUnitLookup");
	int newSize=FileSize+sizeof(short);
	char *n=f;
	f=SMemNew(newSize);
	memcpy(f,n,FileSize);
	SetStructure();
	header_m->nTexUnitLookup=1;
	header_m->ofsTexUnitLookup=FileSize;
	FileSize=newSize;
	SetStructure();
	texunitlookup_m[0]=0;
	FillLine();
}

void ModelEditor::newTexReplace(){
	_Tn("newTexReplace");
	int newSize=FileSize+sizeof(short);
	char *n=f;
	f=SMemNew(newSize);
	memcpy(f,n,FileSize);
	SetStructure();
	header_m->nTexReplace=1;
	header_m->ofsTexReplace=FileSize;
	FileSize=newSize;
	SetStructure();
	texreplacelookup_m[0]=0;
	FillLine();
}

void ModelEditor::newTexAnimLookup(){
	_Tn("newTexAnimLookup");
	if(header_m->nTexAnims!=0){
	int newSize=FileSize+header_m->nTexAnims*sizeof(short);
	char *n=f;
	f=SMemNew(newSize);
	memcpy(f,n,FileSize);
	SetStructure();
	header_m->nTexAnimLookup=header_m->nTextures;
	header_m->ofsTexAnimLookup=FileSize;
	FileSize=newSize;
	SetStructure();
	for(int i=0;i<header_m->nTexLookup;i++){
		texanimlookup_m[i]=i;
	}
	FillLine();
	}
	else{
		int newSize=FileSize+sizeof(short);
		char *n=f;
		f=SMemNew(newSize);
		memcpy(f,n,FileSize);
		SetStructure();
		header_m->nTexAnimLookup=1;
		header_m->ofsTexAnimLookup=FileSize;
		FileSize=newSize;
		SetStructure();
		texanimlookup_m[0]=-1;
		FillLine();
	}
}

/**
 * Just some dummy to have the keylookup in the file...
 */
void ModelEditor::newKeyBoneLookup(){
	_Tn("newKeyBoneLookup");
	int newSize=FileSize+sizeof(short);
	char *n=f;
	f=SMemNew(newSize);
	memcpy(f,n,FileSize);
	SetStructure();
	header_m->nKeyBoneLookup=1;
	header_m->ofsKeyBoneLookup=FileSize;
	FileSize=newSize;
	SetStructure();
	keybonelookup_m[0]=-1;
	FillLine();
}

/**
 * I abuse this often
 * so don't care if it's used for something else :O
 */
template<class T>
T* ModelEditor::getAnimblockData(T *data,int Offset){
	_TDn("getAnimBlockData");
	data=(T*)(f+Offset);
	return data;
}

/**
*This writes the data of an animblock of bones to the file
*nTimestamps==nKeys!!!
**/
template<class T>
int ModelEditor::writeAnimblockData(T *data,int nValues){
	_TDn("writeAnimblockData");
	int newSize=FileSize+sizeof(T)*nValues;
	char *n=f;
	f=SMemNew(newSize);
	memcpy(f,n,FileSize);
	SetStructure();
	memcpy(f+FileSize,data,sizeof(T)*nValues);
	FileSize=newSize;
	FillLine();
	return 0;
}

int ModelEditor::writeSubStructs(BoneHelper help){
	_TDn("writeSubStructs");
	FillLine();
	char *n=f;
	int newSize=FileSize+sizeof(AnimSubStructure)*help.nAnimations*6;
	f=SMemNew(newSize);
	memcpy(f,n,FileSize);
	SetStructure();
	AnimSubStructure *transtime;
	AnimSubStructure *transkeys;
	AnimSubStructure *rottime;
	AnimSubStructure *rotkeys;
	AnimSubStructure *scaletime;
	AnimSubStructure *scalekeys;
	int ofsSize=FileSize;
	FileSize=newSize;
	for(int i=0;i<help.nAnimations;i++){
		//translation
		if(help.info[i].hasData[0]){

			transtime=(AnimSubStructure *)(f+ofsSize);
			transtime[i].n=help.info[i].nSubstructs[0];
			transtime[i].ofs=FileSize;
			//let's create *Data out of struct <.<
			uint32 *timestamps=new uint32[help.info[i].nSubstructs[0]];
			for(int j=0;j<help.info[i].nSubstructs[0];j++){
				timestamps[j]=help.info[i].trans[j].timestamp;
			}
			writeAnimblockData(timestamps,help.info[i].nSubstructs[0]);

			transkeys=(AnimSubStructure *)(f+ofsSize
									+sizeof(AnimSubStructure)*help.nAnimations);
			transkeys[i].n=help.info[i].nSubstructs[0];
			transkeys[i].ofs=FileSize;
			Vec3D *keys=new Vec3D[help.info[i].nSubstructs[0]];
			for(int j=0;j<help.info[i].nSubstructs[0];j++){
				keys[j]=help.info[i].trans[j].value;
			}
			writeAnimblockData(keys,help.info[i].nSubstructs[0]);
		}
		//rotation
		if(help.info[i].hasData[1]){

			rottime=(AnimSubStructure *)(f+ofsSize
									+2*sizeof(AnimSubStructure)*help.nAnimations);
			rottime[i].n=help.info[i].nSubstructs[1];
			rottime[i].ofs=FileSize;
			uint32 *timestamps=new uint32[help.info[i].nSubstructs[1]];
			for(int j=0;j<help.info[i].nSubstructs[1];j++){
				timestamps[j]=help.info[i].rot[j].timestamp;
			}
			writeAnimblockData(timestamps,help.info[i].nSubstructs[1]);

			rotkeys=(AnimSubStructure *)(f+ofsSize
									+3*sizeof(AnimSubStructure)*help.nAnimations);
			rotkeys[i].n=help.info[i].nSubstructs[1];
			rotkeys[i].ofs=FileSize;
			SQuaternion *keys=new SQuaternion[help.info[i].nSubstructs[1]];
			for(int j=0;j<help.info[i].nSubstructs[1];j++){
				keys[j]=help.info[i].rot[j].value;
			}
			writeAnimblockData(keys,help.info[i].nSubstructs[1]);
		}
		//scaling
		if(help.info[i].hasData[2]){

			scaletime=(AnimSubStructure *)(f+ofsSize
									+4*sizeof(AnimSubStructure)*help.nAnimations);
			scaletime[i].n=help.info[i].nSubstructs[2];
			scaletime[i].ofs=FileSize;
			uint32 *timestamps=new uint32[help.info[i].nSubstructs[2]];
			for(int j=0;j<help.info[i].nSubstructs[2];j++){
				timestamps[j]=help.info[i].scale[j].timestamp;
			}

			writeAnimblockData(timestamps,help.info[i].nSubstructs[2]);

			scalekeys=(AnimSubStructure *)(f+ofsSize
									+5*sizeof(AnimSubStructure)*help.nAnimations);
			scalekeys[i].n=help.info[i].nSubstructs[2];
			scalekeys[i].ofs=FileSize;
			Vec3D *keys=new Vec3D[help.info[i].nSubstructs[2]];
			for(int j=0;j<help.info[i].nSubstructs[2];j++){
				keys[j]=help.info[i].scale[j].value;
			}
			writeAnimblockData(keys,help.info[i].nSubstructs[2]);

		}
	}
	return 0;
}

void ModelEditor::addRibbon(ModelRibbonEmitterDef ribbon,RibbonHelper help){
	_TDn("addRibbon");
	FillLine();
	int newSize=FileSize+sizeof(ModelRibbonEmitterDef)*(header_m->nRibbonEmitters+1)+ribbon_ofs_size;
	int ofs=FileSize+sizeof(ModelRibbonEmitterDef)*(header_m->nRibbonEmitters+1);

	char *n=f;
	f=SMemNew(newSize);
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
