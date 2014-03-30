#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef float	Vec3D[3];
typedef float	Vec2D[2];
#include "modelheaders.h"

FILE *Input;

char	*f;
int		FileSize;
ModelHeader *header;
ModelParticleEmitterDef	*particles;

struct pFileHeader
{
	int		numParticles;
	int		sizeParticles;
	char	description[248];
};

void FixParticles()
{
	for(int i=0;i<header->nParticleEmitters;i++)
	{
		particles[i].ofsZero1-=header->ofsParticleEmitters;
		if(particles[i].ofsZero1<0)
			printf("Oh oh Particle %d ofsZero1 is negative now :(\n",i);
		particles[i].ofsZero2-=header->ofsParticleEmitters;
		if(particles[i].ofsZero2<0)
			printf("Oh oh Particle %d ofsZero2 is negative now :(\n",i);
		for(int j=0;j<10;j++)
		{
			if(particles[i].params[j].nRanges>0)
			{
				particles[i].params[j].ofsRanges-=header->ofsParticleEmitters;
				if(particles[i].params[j].ofsRanges<0)
					printf("Oh oh Particle %d Animblock %d ofsRanges is negative now :(\n",i,j);
			}
			if(particles[i].params[j].nTimes>0)
			{
				particles[i].params[j].ofsTimes-=header->ofsParticleEmitters;
				if(particles[i].params[j].ofsTimes<0)
					printf("Oh oh Particle %d Animblock %d ofsTimes is negative now :(\n",i,j);
			}
			if(particles[i].params[j].nKeys>0)
			{
				particles[i].params[j].ofsKeys-=header->ofsParticleEmitters;
				if(particles[i].params[j].ofsKeys<0)
					printf("Oh oh Particle %d Animblock %d ofsKeys is negative now :(\n",i,j);
			}
		}
		if(particles[i].unk.nRanges>0)
		{
			particles[i].unk.ofsRanges-=header->ofsParticleEmitters;
			if(particles[i].unk.ofsRanges<0)
				printf("Oh oh Particle %d Unknown Animblock ofsRanges is negative now :(\n",i);
		}
		if(particles[i].unk.nTimes>0)
		{
			particles[i].unk.ofsTimes-=header->ofsParticleEmitters;
			if(particles[i].unk.ofsTimes<0)
				printf("Oh oh Particle %d Unknown Animblock ofsTimes is negative now :(\n",i);
		}
		if(particles[i].unk.nKeys>0)
		{
			particles[i].unk.ofsKeys-=header->ofsParticleEmitters;
			if(particles[i].unk.ofsKeys<0)
				printf("Oh oh Particle %d Unknown Animblock ofsKeys is negative now :(\n",i);
		}
	}
}


int main(int argc, char **argv)
{
	char t;
	int i,j;

	if(argc<2)
	{
		printf("Particle Extracter V1.0 (May 29th, 2006) by John 'Cryect' Rittenhouse\n"); 
		printf(" pextracter <filename.m2>\n");
		return 0;
	}

	printf("pextracter V1.0 (May 29th, 2006) by John 'Cryect' Rittenhouse\n"); 
	printf(" Loading Model %s\n",argv[1]);
	Input=fopen(argv[1],"rb+");

	if(Input==NULL)
	{
		printf(" ERROR: Could not load file %s\n",argv[1]);
		return 0;
	}

	fseek(Input,0,SEEK_END);
	FileSize=ftell(Input);
	fseek(Input,0,SEEK_SET);

	f=new char[FileSize];
	
	fread(f,FileSize,1,Input);
	fclose(Input);

	header=(ModelHeader *)f;
	particles=(ModelParticleEmitterDef *)(f+header->ofsParticleEmitters);

	FixParticles();

	pFileHeader	pHeader;

	pHeader.numParticles=header->nParticleEmitters;
	pHeader.sizeParticles=FileSize-header->ofsParticleEmitters;
	printf("Particle File Description: ");
	memset(pHeader.description,0,248);
	gets(pHeader.description);

	char fname[256];
	printf("Particle File Name: ");
	gets(fname);

	Input=fopen(fname,"wb+");
	fwrite(&pHeader,sizeof(pFileHeader),1,Input);
	fwrite(f+header->ofsParticleEmitters,pHeader.sizeParticles,1,Input);
	fclose(Input);

	return 0;
}

