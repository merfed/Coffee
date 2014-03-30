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
ModelBoneDef1 *bones1;
ModelBoneDef2 *bones2;
ModelView *view;


char          *bet;
char          *end;
char          *bet2;
char          *end2;

bool convbase1(int argc, char **argv)
{
	{		// Import Data.
	// between header and bones.
    bet = (char *)(f+324);
    
    // bones.
    bones1=(ModelBoneDef1 *)(f+header->ofsBones);
    bones2=(ModelBoneDef2 *)(f+header->ofsBones); // filling up with crap ...
    
    // save the shit after the bones.
    end = (char *)(f+header->ofsBones+(header->nBones*104));
  	}
	
	{		// Convert Bonestruct.
	for(int i=0; i<header->nBones;i++)
	{
		for(int j=0;j<12;j++)
	        bones2[i].fake1[j] = bones1[i].fake1[j];
		for(int k=0;k<92;k++)
        	bones2[i].fake2[k] = bones1[i].fake2[k];
        bones2[i].unknown = 0;
	}
	
	}

	printf("\n\t(i) Processing Model %s\n",argv[1]);

	{		// Header
		header->version[0]=4;
		header->version[1]=1;
		header->version[2]=0;
		header->version[3]=0;
	}

	printf("\t(D) header.version=%i%i%i%i\n", header->version[0], header->version[1], header->version[2], header->version[3]);
	
	{		// OffsetFix
		int plusofsafterbones = header->nBones*4;
		
		if(header->ofs1 > header->ofsBones) header->ofs1 += plusofsafterbones;
		if(header->ofs2 > header->ofsBones) header->ofs2 += plusofsafterbones;
		if(header->ofs3 > header->ofsBones) header->ofs3 += plusofsafterbones;
		if(header->ofs4 > header->ofsBones) header->ofs4 += plusofsafterbones;
		if(header->ofs5 > header->ofsBones) header->ofs5 += plusofsafterbones;
		if(header->ofs6 > header->ofsBones) header->ofs6 += plusofsafterbones;
		if(header->ofs7 > header->ofsBones) header->ofs7 += plusofsafterbones;
		if(header->ofs8 > header->ofsBones) header->ofs8 += plusofsafterbones;
		if(header->ofs9 > header->ofsBones) header->ofs9 += plusofsafterbones;
		if(header->ofs10 > header->ofsBones) header->ofs10 = plusofsafterbones;
		if(header->ofs11 > header->ofsBones) header->ofs11 += plusofsafterbones;
		if(header->ofs12 > header->ofsBones) header->ofs12 += plusofsafterbones;
		if(header->ofs13 > header->ofsBones) header->ofs13 += plusofsafterbones;
		if(header->ofs14 > header->ofsBones) header->ofs14 += plusofsafterbones;
		if(header->ofs15 > header->ofsBones) header->ofs15 += plusofsafterbones;
		if(header->ofs16 > header->ofsBones) header->ofs16 += plusofsafterbones;
		if(header->ofs17 > header->ofsBones) header->ofs17 += plusofsafterbones;
		if(header->ofs18 > header->ofsBones) header->ofs18 += plusofsafterbones;
		if(header->ofs19 > header->ofsBones) header->ofs19 += plusofsafterbones;
		if(header->ofs20 > header->ofsBones) header->ofs20 += plusofsafterbones;
		if(header->ofs21 > header->ofsBones) header->ofs21 += plusofsafterbones;
		if(header->ofs22 > header->ofsBones) header->ofs22 += plusofsafterbones;
		if(header->ofs23 > header->ofsBones) header->ofs23 += plusofsafterbones;
		if(header->ofs24 > header->ofsBones) header->ofs24 += plusofsafterbones;
		if(header->ofs25 > header->ofsBones) header->ofs25 += plusofsafterbones;
		if(header->ofs26 > header->ofsBones) header->ofs26 += plusofsafterbones;

		int plusofsafterviews = header->nViews*16;
		
		if(header->ofs4 > header->ofs3) header->ofs4 += plusofsafterviews;
		if(header->ofs5 > header->ofs3) header->ofs5 += plusofsafterviews;
		if(header->ofs6 > header->ofs3) header->ofs6 += plusofsafterviews;
		if(header->ofs7 > header->ofs3) header->ofs7 += plusofsafterviews;
		if(header->ofs8 > header->ofs3) header->ofs8 += plusofsafterviews;
		if(header->ofs9 > header->ofs3) header->ofs9 += plusofsafterviews;
		if(header->ofs10 > header->ofs3) header->ofs10 += plusofsafterviews;
		if(header->ofs11 > header->ofs3) header->ofs11 += plusofsafterviews;
		if(header->ofs12 > header->ofs3) header->ofs12 += plusofsafterviews;
		if(header->ofs13 > header->ofs3) header->ofs13 += plusofsafterviews;
		if(header->ofs14 > header->ofs3) header->ofs14 += plusofsafterviews;
		if(header->ofs15 > header->ofs3) header->ofs15 += plusofsafterviews;
		if(header->ofs16 > header->ofs3) header->ofs16 += plusofsafterviews;
		if(header->ofs17 > header->ofs3) header->ofs17 += plusofsafterviews;
		if(header->ofs18 > header->ofs3) header->ofs18 += plusofsafterviews;
		if(header->ofs19 > header->ofs3) header->ofs19 += plusofsafterviews;
		if(header->ofs20 > header->ofs3) header->ofs20 += plusofsafterviews;
		if(header->ofs21 > header->ofs3) header->ofs21 += plusofsafterviews;
		if(header->ofs22 > header->ofs3) header->ofs22 += plusofsafterviews;
		if(header->ofs23 > header->ofs3) header->ofs23 += plusofsafterviews;
		if(header->ofs24 > header->ofs3) header->ofs24 += plusofsafterviews;
		if(header->ofs25 > header->ofs3) header->ofs25 += plusofsafterviews;
		if(header->ofs26 > header->ofs3) header->ofs26 += plusofsafterviews;
	}

	printf("\n\t(i) Saving Model %s\n",argv[1]);
	
	{		// Save Model.

		Input=fopen(argv[1],"rb+");

		if(Input==NULL)
		{
			printf("\t(!) ERROR: Could not load file %s\n",argv[1]);
			return false;
		}

		fseek(Input,0,SEEK_END);
		FileSize=ftell(Input);
		fseek(Input,0,SEEK_SET);


		printf("\tHeader");
		fwrite(header,324,1,Input);
		printf("\tbet");
		fwrite(bet,(header->ofsBones-324),1,Input);
		printf("\tbones2");
		fwrite(bones2,(header->nBones*108),1,Input);
		printf("\tend");
		fwrite(end,(FileSize-(header->nBones*104)),1,Input);
	//	fwrite(end,(FileSize-pos),1,Input);
		
		/*
		fwrite(header,324,1,Input);
		fwrite(bet,(header->ofsBones-324),1,Input);
		fwrite(bones2,(header->nBones*108),1,Input);
		fwrite(end,(FileSize-pos),1,Input);*/
	//	fwrite(end,(FileSize-(header->nBones*104)),1,Input);

	//	view= (ModelView*)(f+ header->ofs3); // ofsViews


		fclose(Input);	
	}

	return true;
}

bool convbase2(int argc, char **argv)
{
	{		// Import Data.
    // between header and bones.
    bet = (char *)(f+324);

    // bones.
    bones2=(ModelBoneDef2 *)(f+header->ofsBones);
    bones1=(ModelBoneDef1 *)(f+header->ofsBones); // filling up with crap ...

    // save the shit after the bones.
    end = (char *)(f+header->ofsBones+(header->nBones*108));
	}

	{		// Convert Bonestruct.
	for(int i=0; i<header->nBones;i++)
	{
		for(int j=0;j<12;j++)
	        bones2[i].fake1[j] = bones1[i].fake1[j];
		for(int k=0;k<92;k++)
        	bones2[i].fake2[k] = bones1[i].fake2[k];
	}
	}

	printf("\n\t(i) Processing Model %s\n",argv[1]);

	{		// Header
		header->version[0]=0;
		header->version[1]=1;
		header->version[2]=0;
		header->version[3]=0;
	}

	printf("\t(D) header.version=%i%i%i%i\n", header->version[0], header->version[1], header->version[2], header->version[3]);

	{		// OffsetFix
		int plusofsafterbones = header->nBones*4;

		if(header->ofs1 > header->ofsBones) header->ofs1 -= plusofsafterbones;
		if(header->ofs2 > header->ofsBones) header->ofs2 -= plusofsafterbones;
		if(header->ofs3 > header->ofsBones) header->ofs3 -= plusofsafterbones;
		if(header->ofs4 > header->ofsBones) header->ofs4 -= plusofsafterbones;
		if(header->ofs5 > header->ofsBones) header->ofs5 -= plusofsafterbones;
		if(header->ofs6 > header->ofsBones) header->ofs6 -= plusofsafterbones;
		if(header->ofs7 > header->ofsBones) header->ofs7 -= plusofsafterbones;
		if(header->ofs8 > header->ofsBones) header->ofs8 -= plusofsafterbones;
		if(header->ofs9 > header->ofsBones) header->ofs9 -= plusofsafterbones;
		if(header->ofs10 > header->ofsBones) header->ofs10 -= plusofsafterbones;
		if(header->ofs11 > header->ofsBones) header->ofs11 -= plusofsafterbones;
		if(header->ofs12 > header->ofsBones) header->ofs12 -= plusofsafterbones;
		if(header->ofs13 > header->ofsBones) header->ofs13 -= plusofsafterbones;
		if(header->ofs14 > header->ofsBones) header->ofs14 -= plusofsafterbones;
		if(header->ofs15 > header->ofsBones) header->ofs15 -= plusofsafterbones;
		if(header->ofs16 > header->ofsBones) header->ofs16 -= plusofsafterbones;
		if(header->ofs17 > header->ofsBones) header->ofs17 -= plusofsafterbones;
		if(header->ofs18 > header->ofsBones) header->ofs18 -= plusofsafterbones;
		if(header->ofs19 > header->ofsBones) header->ofs19 -= plusofsafterbones;
		if(header->ofs20 > header->ofsBones) header->ofs20 -= plusofsafterbones;
		if(header->ofs21 > header->ofsBones) header->ofs21 -= plusofsafterbones;
		if(header->ofs22 > header->ofsBones) header->ofs22 -= plusofsafterbones;
		if(header->ofs23 > header->ofsBones) header->ofs23 -= plusofsafterbones;
		if(header->ofs24 > header->ofsBones) header->ofs24 -= plusofsafterbones;
		if(header->ofs25 > header->ofsBones) header->ofs25 -= plusofsafterbones;
		if(header->ofs26 > header->ofsBones) header->ofs26 -= plusofsafterbones;

		int plusofsafterviews = header->nViews*16;
		
		if(header->ofs4 > header->ofs3) header->ofs4 -= plusofsafterviews;
		if(header->ofs5 > header->ofs3) header->ofs5 -= plusofsafterviews;
		if(header->ofs6 > header->ofs3) header->ofs6 -= plusofsafterviews;
		if(header->ofs7 > header->ofs3) header->ofs7 -= plusofsafterviews;
		if(header->ofs8 > header->ofs3) header->ofs8 -= plusofsafterviews;
		if(header->ofs9 > header->ofs3) header->ofs9 -= plusofsafterviews;
		if(header->ofs10 > header->ofs3) header->ofs10 -= plusofsafterviews;
		if(header->ofs11 > header->ofs3) header->ofs11 -= plusofsafterviews;
		if(header->ofs12 > header->ofs3) header->ofs12 -= plusofsafterviews;
		if(header->ofs13 > header->ofs3) header->ofs13 -= plusofsafterviews;
		if(header->ofs14 > header->ofs3) header->ofs14 -= plusofsafterviews;
		if(header->ofs15 > header->ofs3) header->ofs15 -= plusofsafterviews;
		if(header->ofs16 > header->ofs3) header->ofs16 -= plusofsafterviews;
		if(header->ofs17 > header->ofs3) header->ofs17 -= plusofsafterviews;
		if(header->ofs18 > header->ofs3) header->ofs18 -= plusofsafterviews;
		if(header->ofs19 > header->ofs3) header->ofs19 -= plusofsafterviews;
		if(header->ofs20 > header->ofs3) header->ofs20 -= plusofsafterviews;
		if(header->ofs21 > header->ofs3) header->ofs21 -= plusofsafterviews;
		if(header->ofs22 > header->ofs3) header->ofs22 -= plusofsafterviews;
		if(header->ofs23 > header->ofs3) header->ofs23 -= plusofsafterviews;
		if(header->ofs24 > header->ofs3) header->ofs24 -= plusofsafterviews;
		if(header->ofs25 > header->ofs3) header->ofs25 -= plusofsafterviews;
		if(header->ofs26 > header->ofs3) header->ofs26 -= plusofsafterviews;
	}

	printf("\n\t(i) Saving Model %s\n",argv[1]);

	{		// Save Model.
	Input=fopen(argv[1],"rb+");

	if(Input==NULL)
	{
		printf("\t(!) ERROR: Could not load file %s\n",argv[1]);
		return false;
	}

	fseek(Input,0,SEEK_END);
	FileSize=ftell(Input);
	fseek(Input,0,SEEK_SET);

	printf("\tHeader");
	fwrite(header,324,1,Input);
	printf("\tbet");
	fwrite(bet,(header->ofsBones-324),1,Input);
	printf("\tbones1");
	fwrite(bones1,(header->nBones*104),1,Input);
	printf("\tend");
	fwrite(end,(FileSize-(header->nBones*108)),1,Input);
//	fwrite(end,(FileSize-pos),1,Input);

//	view= (ModelView*)(f+ header->ofs3); // ofsViews


	fclose(Input);
	}

	return true;
}

int main(int argc, char **argv)
{
	char t;
	int i,j;
	
	printf("\n\t(#) M2Conv - by schlumpf_\n");

	if(argc<2)
	{
		printf("\t(!) Syntax: m2conv <filename.m2>\n");
		return 0;
	}

	printf("\n\t(i) Loading Model %s\n",argv[1]);
	
	// read start
	
	Input=fopen(argv[1],"rb+");

	if(Input==NULL)
	{
		printf("\t(!) ERROR: Could not load file %s\n",argv[1]);
		return 0;
	}

	fseek(Input,0,SEEK_END);
	FileSize=ftell(Input);
	fseek(Input,0,SEEK_SET);


	f=new char[FileSize];
	
	fread(f,FileSize,1,Input);
	
	header=(ModelHeader *)f;
	
	fclose(Input);
	
	// end read.
	
	printf("\n\t(i) header.version=%i%i%i%i\n\n", header->version[0], header->version[1], header->version[2], header->version[3]);
	if(header->version[0]==4)
	{
		printf("\n\t(i) having 2.x M2. convert to 1.x M2\n");
		convbase2(argc, argv);
	}
	else
	{
		printf("\n\t(i) having 1.x M2. convert to 2.x M2\n");
		convbase1(argc, argv);
	}
}

