	#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef float	Vec3D[3];
typedef float	Vec2D[2];
#include "modelheaders.h"

FILE *Input;

char	*f,*pfile;
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
		particles[i].ofsZero1+=header->ofsParticleEmitters;
		particles[i].ofsZero2+=header->ofsParticleEmitters;
		for(int j=0;j<10;j++)
		{
			if(particles[i].params[j].nRanges>0)
				particles[i].params[j].ofsRanges+=header->ofsParticleEmitters;
			if(particles[i].params[j].nTimes>0)
				particles[i].params[j].ofsTimes+=header->ofsParticleEmitters;
			if(particles[i].params[j].nKeys>0)
				particles[i].params[j].ofsKeys+=header->ofsParticleEmitters;
		}
		if(particles[i].unk.nRanges)
			particles[i].unk.ofsRanges+=header->ofsParticleEmitters;
		if(particles[i].unk.nTimes)
			particles[i].unk.ofsTimes+=header->ofsParticleEmitters;
		if(particles[i].unk.nKeys)
			particles[i].unk.ofsKeys+=header->ofsParticleEmitters;
	}
}


pFileHeader	*pHeader;
int main(int argc, char **argv)
{
	char t;
	int i,j;

	if(argc<2)
	{
		printf("Particle Injector V1.0 (May 29th, 2006) by John 'Cryect' Rittenhouse\n"); 
		printf(" pinjector <filename.m2> <filename.particle>\n");
		return 0;
	}

	printf("Particle Injector V1.0 (May 29th, 2006) by John 'Cryect' Rittenhouse\n"); 
	
	printf(" Loading Particle %s\n",argv[1]);

	Input=fopen(argv[2],"rb+");

	if(Input==NULL)
	{
		printf(" ERROR: Could not load file %s\n",argv[2]);
		return 0;
	}

	fseek(Input,0,SEEK_END);
	FileSize=ftell(Input);
	fseek(Input,0,SEEK_SET);

	pfile=new char[FileSize];
	
	fread(pfile,FileSize,1,Input);
	fclose(Input);

	pHeader=(pFileHeader	*)pfile;
	
	
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

	f=new char[FileSize+pHeader->sizeParticles];
	
	fread(f,FileSize,1,Input);
	fclose(Input);

	header=(ModelHeader *)f;
	

	printf(" Injecting Particles %s\n",argv[1]);
	memcpy(f+FileSize,pfile+sizeof(pFileHeader),pHeader->sizeParticles);

	header->nParticleEmitters=pHeader->numParticles;
	header->ofsParticleEmitters=FileSize;

	particles=(ModelParticleEmitterDef *)(f+header->ofsParticleEmitters);
	FixParticles();

	



	printf(" Writing Model %s\n",argv[1]);
	Input=fopen(argv[1],"wb+");
	fwrite(f,FileSize+pHeader->sizeParticles,1,Input);
	fclose(Input);

	return 0;
}

