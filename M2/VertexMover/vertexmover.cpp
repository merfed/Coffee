/*
 * vertexmover.cpp
 *
 *  Created on: 25.05.2009
 *      Author: Bastian
 */

#include "modelheaders.h"
FILE *Input,*Readin;

char	*f,*r;
int		FileSize,ISize;
ModelHeader *header_m;
ModelHeader *header_i;
ModelLightDef *light_m;
ModelRibbonEmitterDef *ribbon_m;
ModelParticleEmitterDef *part_m;
ModelAttachmentDef *attach_m;
ModelBoneDef *bone_m;
ModelVertex *vertex_m;
ModelVertex *vertex_i;
bool rescale;

int main(int argc, char **argv){
	printf("VertexMover by Tigurius\n");
	printf(" Loading Model %s\n",argv[1]);
	Input=fopen(argv[1],"rb+");
	printf(" Loading Model %s\n",argv[2]);
	Readin=fopen(argv[2],"rb+");
	if(Input==NULL||Readin==NULL)
	{
		printf(" ERROR: Could not load file");
		return 0;
	}
	if(argc>3)rescale=true;

	fseek(Input,0,SEEK_END);
	FileSize=ftell(Input);
	fseek(Input,0,SEEK_SET);

	f=new char[FileSize];

	fread(f,FileSize,1,Input);
	fclose(Input);


	fseek(Readin,0,SEEK_END);
	ISize=ftell(Readin);
	fseek(Readin,0,SEEK_SET);

	r=new char[ISize];

	fread(r,ISize,1,Readin);
	fclose(Readin);

	header_m=(ModelHeader *)(f);
	vertex_m=(ModelVertex *)(f+header_m->ofsVertices);
	bone_m=(ModelBoneDef *)(f+header_m->ofsBones);
	light_m=(ModelLightDef *)(f+header_m->ofsLights);
	part_m=(ModelParticleEmitterDef *)(f+header_m->ofsParticleEmitters);
	ribbon_m=(ModelRibbonEmitterDef *)(f+header_m->ofsRibbonEmitters);
	attach_m=(ModelAttachmentDef *)(f+header_m->ofsAttachments);

	header_i=(ModelHeader *)(r);
	vertex_i=(ModelVertex *)(r+header_i->ofsVertices);
	for(int i=0;i<header_m->nVertices;i++){
		vertex_m[i].pos[0]=vertex_i[i].pos[0];
		vertex_m[i].pos[1]=vertex_i[i].pos[1];
		vertex_m[i].pos[2]=vertex_i[i].pos[2];
	}
	if(rescale){
		int factor=atof(argv[3]);
		for(int i=0;i<header_m->nVertices;i++){
			vertex_m[i].pos[0]=vertex_m[i].pos[0]/factor;
			vertex_m[i].pos[1]=vertex_m[i].pos[1]/factor;
			vertex_m[i].pos[2]=vertex_m[i].pos[2]/factor;
		}
		for(int i=0;i<header_m->nBones;i++){
			bone_m[i].pivot[0]=bone_m[i].pivot[0]/factor;
			bone_m[i].pivot[1]=bone_m[i].pivot[1]/factor;
			bone_m[i].pivot[2]=bone_m[i].pivot[2]/factor;
		}
		for(int i=0;i<header_m->nLights;i++){
			light_m[i].pos[0]=light_m[i].pos[0]/factor;
			light_m[i].pos[1]=light_m[i].pos[1]/factor;
			light_m[i].pos[2]=light_m[i].pos[2]/factor;
		}
		for(int i=0;i<header_m->nAttachments;i++){
			attach_m[i].pos[0]=attach_m[i].pos[0]/factor;
			attach_m[i].pos[1]=attach_m[i].pos[1]/factor;
			attach_m[i].pos[2]=attach_m[i].pos[2]/factor;
		}
		for(int i=0;i<header_m->nRibbonEmitters;i++){
			ribbon_m[i].pos[0]=ribbon_m[i].pos[0]/factor;
			ribbon_m[i].pos[1]=ribbon_m[i].pos[1]/factor;
			ribbon_m[i].pos[2]=ribbon_m[i].pos[2]/factor;
		}
		for(int i=0;i<header_m->nParticleEmitters;i++){
			part_m[i].pos[0]=part_m[i].pos[0]/factor;
			part_m[i].pos[1]=part_m[i].pos[1]/factor;
			part_m[i].pos[2]=part_m[i].pos[2]/factor;
		}
	}
	printf("Saving Changes\n");
	Input=fopen(argv[1],"wb");
	fwrite(f,FileSize,1,Input);
	fclose(Input);
	return 0;
}
