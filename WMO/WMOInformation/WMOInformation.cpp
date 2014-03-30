#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <string>
using namespace std;

#define DESCRIPTION "Prints information from some chunks in the WMO."
#define ARGUMENTS "<WMOFile>"

#define USAGE( minimumArguments, argc, argv ) \
	if( argc < minimumArguments + 1 ) \
	{	 \
		printf( "  %s\n", argv[0] ); \
		printf( "    " DESCRIPTION "\n\n" ); \
		printf( "    Usage: \"%s " ARGUMENTS "\"\n", argv[0] ); \
		printf( "    Built at: " __DATE__ "\n" ); \
		return -1; \
	}


typedef unsigned char UINT8;
typedef char INT8;
typedef unsigned short UINT16;
typedef short INT16;
typedef unsigned int UINT32;
typedef int INT32;

struct MOHD
{
/*000h*/  UINT32 nTextures;
/*004h*/  UINT32 nGroups;
/*008h*/  UINT32 nPortals;
/*00Ch*/  UINT32 nLights;
/*010h*/  UINT32 nModels;
/*014h*/  UINT32 nDoodads;
/*018h*/  UINT32 nSets;
/*01Ch*/  UINT8  colR;
/*01Dh*/  UINT8  colG;
/*01Eh*/  UINT8  colB;
/*01Fh*/  UINT8  colX;
/*020h*/  UINT32 wmoID;
/*024h*/  float  bb1[3];
/*030h*/  float  bb2[3];
/*03Ch*/  UINT32 nullish;
};

struct MaterialFake
{
/*000h*/  UINT32 void1[3];
/*00Ch*/  UINT32 start;
/*010h*/  UINT32 void2[2];
/*018h*/  UINT32 end;
/*020h*/  UINT32 void3[9];
/*040h*/
};

struct DoodadDefFake
{
/*000h*/  UINT32 start;
/*004h*/  UINT32 void1[9];
/*028h*/
};

FILE *Input;

char MOTX[9999];
int MOTXsize;
char MOGN[9999];
int MOGNsize;
char MODN[9999];
int MODNsize;
char MOSB[9999];
int MOSBsize;
MaterialFake mats[999];
char temp[999];
int tempint;

//MOHD Header;
/*terialFake Materials[1000];
DoodadDefFake DoodadDefs[1000];
char Textures[1000][1000];
char TexturesBlock[10000];
char Doodads[1000][1000];
char DoodadsBlock[10000];*/

unsigned int getBlock(char ID[4])
{
	int i=0;
	unsigned int size;
	char ID_[4] = {0,0,0,0};
	do {
	    fread(ID_,1,4,Input);
		fread(&size,sizeof(unsigned int),1,Input);
		printf("Block: %c%c%c%c | size: %i\n",ID_[3],ID_[2],ID_[1],ID_[0], size);
		if(!(ID[3]==ID_[3] && ID[2]==ID_[2] && ID[1]==ID_[1] && ID[0]==ID_[0]))
			fseek(Input,size,SEEK_CUR);
		i++;
	} while(i<100 && !(ID[3]==ID_[3] && ID[2]==ID_[2] && ID[1]==ID_[1] && ID[0]==ID_[0]));
//		system("pause");
	return size;
}

void FindNextBlock()
{
	unsigned int size;
	char ID[4];

	ID[3]='M';ID[2]='O';ID[1]='H';ID[0]='D';size = getBlock(ID);
	    
	MOHD Header;
    fread(&Header,sizeof(MOHD),1,Input);
    
    ID[3]='M';ID[2]='O';ID[1]='T';ID[0]='X';size = getBlock(ID);

	char *TexturesBlock = new char[size];
	fread(TexturesBlock,1,size,Input);

    ID[3]='M';ID[2]='O';ID[1]='M';ID[0]='T';size = getBlock(ID);

	MaterialFake Materials[Header.nTextures];
	fread(Materials,sizeof(MaterialFake),Header.nTextures,Input);

/*	for(int i = 0;i<Header.nTextures;i++)
	{
		printf("v1: %x %x %x \tv2: %x %x \tv3: %x %x %x %x %x %x %x %x %x\n",
			Materials[i].void1[0],Materials[i].void1[1],Materials[i].void1[2],
			Materials[i].void2[0],Materials[i].void2[1],
			Materials[i].void3[0],Materials[i].void3[1],Materials[i].void3[2],
			Materials[i].void3[3],Materials[i].void3[4],Materials[i].void3[5],
			Materials[i].void3[6],Materials[i].void3[7],Materials[i].void3[8]);
	}*/

	string *Textures = new string[Header.nTextures];
	char *buffer = new char[200];
    for(int x = 0; x<Header.nTextures;x++)
	{
		int pos = 0;
		for (int c = Materials[x].start; c<Materials[x].end;c++)
		{
            buffer[pos++] = TexturesBlock[c];
		}
		Textures[x] = buffer;
	}

    ID[3]='M';ID[2]='O';ID[1]='D';ID[0]='N';size = getBlock(ID);

	int doodadsize = size;
	char *DoodadsBlock = new char[size];
	fread(DoodadsBlock,1,size,Input);

    ID[3]='M';ID[2]='O';ID[1]='D';ID[0]='D';size = getBlock(ID);

	Header.nDoodads = size / 40;
	DoodadDefFake *DoodadDefs = new DoodadDefFake[Header.nDoodads];
	fread(DoodadDefs,sizeof(DoodadDefFake),Header.nDoodads,Input);

//	char Doodads[Header.nModels][200];
	string *Doodads = new string[Header.nModels];
	int DoodadStarts[Header.nModels];

	printf("\nModels:\n");
	for(int x = 0; x<Header.nModels;x++)
	{
		Doodads[x] = DoodadsBlock;
		int c = 0;
		while(DoodadsBlock[c]!='\0')
			c++;
		while(DoodadsBlock[c]=='\0')
			c++;
		DoodadsBlock+=c;
	}
	
	for(int x = 0; x<Header.nModels;x++)
	{
		cout << x << ": " << Doodads[x] << endl;

		if(x%100==0)
			system("pause");
	}
	system("pause");

	printf("\nDoodads:\n");
	for(int j = 0;j<Header.nDoodads;j++)
	{
		cout << j << " (" << DoodadStarts[DoodadDefs[j].start] << ") " << Doodads[DoodadStarts[DoodadDefs[j].start]] << endl;

		if(j%10==0)
			system("pause");
	}
	system("pause");
	
	printf("Textures:\n");
	for(int j = 0;j<Header.nTextures;j++)
	{
		printf("%i: ",j);
		for(int i = 0;Textures[j][i] != '\0';i++)
			printf("%c",Textures[j][i]);
		printf("\n");
	}
	system("pause");
	
	
}

void FixFlags()
{
	char ID[4];
	unsigned int TInt;
	unsigned int Count;
	unsigned int Pos;
	fread(ID,1,4,Input);

	fread(&Count,sizeof(unsigned int),1,Input);
	Pos=ftell(Input);

	Count=Count/32;

	printf("Changing %d flags in Block %c%c%c%c\n",Count,ID[3],ID[2],ID[1],ID[0]);
	TInt=8;
	for (int i=0;i<Count;i++)
	{
		fseek(Input,i*32+Pos,SEEK_SET);
		fread(&TInt,sizeof(unsigned int),1,Input);
		TInt=(TInt&(0xFFFFFFFF-0x2000))|0x8;
		fseek(Input,i*32+Pos,SEEK_SET);
		if(fwrite(&TInt,sizeof(unsigned int),1,Input)==0)
			printf("Didn't write???\n");
	}
}

int main(int argc, char **argv)
{
	USAGE( 1, argc, argv );

	Input=fopen(argv[1],"rb+");


	FindNextBlock();
	
	system("pause");
}
