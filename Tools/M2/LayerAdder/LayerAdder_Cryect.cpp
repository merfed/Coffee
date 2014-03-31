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
ModelRenderFlags	*rf;
ModelTextureDef		*tex;
ModelTexUnit		*texunit[4];
ModelView			*view;
short	*texUnitMapping;
bool changed=false;

char textureType[255];
void setTextureType(int i)
{
	if(i==0)
		strcpy(textureType,"Hard Coded");
	else if(i==1)
		strcpy(textureType,"Player Skin");
	else if(i==2)
		strcpy(textureType,"Cape");
	else if(i==6)
		strcpy(textureType,"Hair");
	else if(i==8)
		strcpy(textureType,"Tauren Fur");
	else if(i==11)
		strcpy(textureType,"Creature Skin 1");
	else if(i==12)
		strcpy(textureType,"Creature Skin 2");
	else if(i==13)
		strcpy(textureType,"Creature Skin 3");
	else
		strcpy(textureType,"Unknown");
}

int main(int argc, char **argv)
{
	char t;
	int i,j;

	if(argc<2)
	{
		printf("M2Modder V1.0 (April 7th, 2006) by John 'Cryect' Rittenhouse\n"); 
		printf(" m2modder <filename.m2>\n");
		return 0;
	}

	printf("M2Modder V1.0 (April 7th, 2006) by John 'Cryect' Rittenhouse\n"); 
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

	f=new char[FileSize*2];
	
	fread(f,FileSize,1,Input);
	fclose(Input);

	header=(ModelHeader *)f;
	rf=(ModelRenderFlags *)(f+header->ofsTexFlags);
	tex=(ModelTextureDef *)(f+header->ofsTextures);
	view= (ModelView*)(f+ header->ofsViews);
	for(i=0;i<4;i++)
		texunit[i]=(ModelTexUnit *)(f+view[i].ofsTex);
	texUnitMapping=(short *)(f+header->ofsTexUnitLookup);


	
	for(i=0;i<header->nTextures;i++)
	{
		setTextureType(tex[i].type);
		
		printf("%d Type: %s (%d) Unknown: 0x%04x Flags: 0x%04x\n",i,textureType,tex[i].type,tex[i].flags>>16,tex[i].flags&0xffff);
		if(tex[i].type==0)
			printf("\tTexture: %s\n",(char *)(f+tex[i].nameOfs));
	}

	int texBase,texSecond,layerRF;
	printf("Add Second Layer to Base Texture #: ");
	scanf ("%d",&texBase);
	printf("Second Layer Texture #: ");
	scanf ("%d",&texSecond);

	for(i=0;i<header->nTexFlags;i++)
		printf("Render Flag: %d Flag: %d Blend: %d\n",i,rf[i].flags,rf[i].blend);
	printf("\nUse which Render Flag for Second Layer: ");
	scanf ("%d",&layerRF);

	printf("Second Layer's Settings (-1 Environment Mapped, 1 Second Layer): ");
	scanf("%hd",&texUnitMapping[1]);
	header->nTexUnitLookup=2;

	int FileOffset=FileSize,newTUNum;
	ModelTexUnit *TempTU;

	for(i=0;i<4;i++)
	{
		newTUNum=view[i].nTex;
		view[i].ofsTex=FileOffset;
		memcpy(f+FileOffset,texunit[i],newTUNum*sizeof(ModelTexUnit));
		FileOffset+=newTUNum*sizeof(ModelTexUnit);
		for(j=0;j<view[i].nTex;j++)
			if(texunit[i][j].textureid==texBase)
			{
				memcpy(f+FileOffset,&texunit[i][j],sizeof(ModelTexUnit));
				TempTU=(ModelTexUnit *)(f+FileOffset);
				TempTU->texunit=1;
				TempTU->texunit2=1;
				TempTU->textureid=texSecond;
				TempTU->flagsIndex=layerRF;
				FileOffset+=sizeof(ModelTexUnit);
				newTUNum++;
			}
		view[i].nTex=newTUNum;
	}
	

	printf("Saving Changes\n");
	Input=fopen(argv[1],"wb");
	fwrite(f,FileOffset,1,Input);
	fclose(Input);


	return 0;
}

