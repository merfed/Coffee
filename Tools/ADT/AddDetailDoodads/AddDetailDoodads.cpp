#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <string>
using namespace std;

#define DESCRIPTION "Adds detail doodads per texture filename."
#define ARGUMENTS "<map filename>"

#define USAGE( minimumArguments, argc, argv ) \
	if( argc < minimumArguments + 1 ) \
	{	 \
		printf( "  %s\n", argv[0] ); \
		printf( "    " DESCRIPTION "\n\n" ); \
		printf( "    Usage: \"%s " ARGUMENTS "\"\n", argv[0] ); \
		printf( "    Built at: " __DATE__ "\n" ); \
		return -1; \
	}
	

struct MCIN{
	unsigned int Offset;
	unsigned int void1;
	unsigned int void2;
	unsigned int void3;
};

struct MCLY 
{
	unsigned int textureId;
 	unsigned int void1;
  	unsigned int void2;
  	unsigned int effectId;

};

FILE *Input;

char *File;
char *MTEX;
unsigned int FileSize;

char *NewFile;
unsigned int NewFileSize;
unsigned int Change;

MCIN *Positions;
MCLY Layer;

int DoodadForTexture[100];
unsigned int MCLQ_Positions[256];
unsigned int MCSE_Positions[256];

void FindMCNKs()
{

	Positions=(MCIN *)(File+92);
	
	for(int i =0;i<100;i++)
	{
        DoodadForTexture[i] = -1;
	}
}

void FindTextures()
{
	int nTex=0;
	fseek(Input,0x1060,SEEK_SET);
	fread(&nTex,sizeof(unsigned int),1,Input);
	char text[nTex];
	fseek(Input,0x1064,SEEK_SET);
	fread(text,sizeof(char),nTex,Input);

	int i = 0;
	int j = 0;
	while(i<nTex)
	{
		printf("please assign doodads to: \n\t");
		while(text[i]!='\0')
		{
			printf("%c",text[i]);
			i++;
		}
		printf("\n (-1: no change to existing doodads)\n");
        int newd;
		cin >> newd;
		DoodadForTexture[j] = newd;
		j++;
		i++;
	}
}


void addDoodads()
{
	unsigned int *TFloat;
	unsigned int *TInt=0;
	unsigned int T=0;
	unsigned int nlayers;
	for (int i=0;i<256;i++)
	{
		fseek(Input,Positions[i].Offset+0x8+0x1C,SEEK_SET);
		fread(&T,sizeof(unsigned int),1,Input);
		T=T+Positions[i].Offset;
		fseek(Input,Positions[i].Offset+0x8+0x0C,SEEK_SET);
		fread(&nlayers,sizeof(unsigned int),1,Input);

		printf("found chunk %d with %d layers (@0x%x)\n",i,nlayers,T);
	//	Layers=(MCLY *)(TInt+8);
		for (int j = 0;j < nlayers;j++)
		{
			fseek(Input,T+8+(sizeof(struct MCLY))*j,SEEK_SET);
			fread(&Layer,sizeof(struct MCLY),1,Input);
		//	printf("%d> texture %d, doodads %d\n",j,Layer.textureId,Layer.effectId);
			
			if (DoodadForTexture[Layer.textureId]!=-1)
			{
				Layer.effectId = DoodadForTexture[Layer.textureId];
			}
			
			fseek(Input,T+8+(sizeof(struct MCLY))*j,SEEK_SET);
			fwrite(&Layer,0x10,1,Input);

		}
	//	if(i%10==0)
		//	system("pause");
	}
}

int main(int argc, char **argv)
{
	USAGE( 1, argc, argv );

	char *replace;
	int i,len;

	printf("Adding doodads to layers in file %s\n\n",argv[1]);
	Input=fopen(argv[1],"rb+");
	fseek(Input,0,SEEK_END);
	FileSize=ftell(Input);
	NewFileSize=FileSize;
	File=new char[FileSize];
	fseek(Input,0,SEEK_SET);
	fread(File,1,FileSize,Input);


	Change=0;


	FindMCNKs();
	FindTextures();
	addDoodads();

	fclose(Input);


	delete File;
}
