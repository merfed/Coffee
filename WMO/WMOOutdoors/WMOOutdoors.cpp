#include <stdio.h>
#include <string.h>
#include <stdlib.h>

FILE *Input;

int XOffset,YOffset;
float WaterLevel;

/*
MCNK
0x14 MCVT Offset
0x18 MCNR Offset
0x1C MCLY Offset
0x20 MCRF Offset
0x24 MCAL Offset
0x2C MCSH Offset
0x58 MCSE Offset
0x60 MCLQ Offset
0x68 Z' Base Coordinate
0x6C X' Base Coordinate
0x70 Y Base Coordinate
*/

struct MCIN{
	unsigned int Offset;
	unsigned int Size;
	unsigned int Temp1;
	unsigned int Temp2;
};

unsigned int MDDF_Offset;
unsigned int MODF_Offset;

MCIN *Positions;
unsigned int MCLQ_Positions[256];
unsigned int MCSE_Positions[256];

char *File;
unsigned int FileSize;

char *NewFile;
unsigned int NewFileSize;
unsigned int Change;

void FindNextBlock()
{
	unsigned int Temp;
	char ID[4];
	fread(ID,1,4,Input);
	printf("Skipping Block %c%c%c%c\n",ID[3],ID[2],ID[1],ID[0]);
	
	fread(&Temp,sizeof(unsigned int),1,Input);
	fseek(Input,Temp,SEEK_CUR);
}

void FindMCNKs()
{
	Positions=(MCIN *)(File+92);
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
	char *replace;
	int i,len;

	if(argc<2)
	{
		printf("WMOOutdoors <filename>");
		return 0;
	}

	Input=fopen(argv[1],"rb+");
	

	Change=0;
	
	FindNextBlock();
	FindNextBlock();
	FindNextBlock();
	FindNextBlock();
	FindNextBlock();
	FixFlags();

	fclose(Input);


}
