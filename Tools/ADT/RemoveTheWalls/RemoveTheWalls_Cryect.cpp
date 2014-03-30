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


void FindMCNKs()
{
	Positions=(MCIN *)(File+92);
}



void FixFlags()
{
	unsigned int *TFloat;
	unsigned int *TInt;
	for (int i=0;i<256;i++)
	{
		TInt=(unsigned int *)(File+Positions[i].Offset+8);
		if(*TInt&0x02)
		{
			printf("Found Invisible Wall Chunk %d (Flag %x)\n",i,*TInt);
			*TInt=*TInt^0x02;
			fseek(Input,Positions[i].Offset+8,SEEK_SET);
			fwrite(TInt,sizeof(unsigned int),1,Input);
		}
	}
}



int main(int argc, char **argv)
{
	char *replace;
	int i,len;

	printf("Removing Walls from File %s\n",argv[1]);
	Input=fopen(argv[1],"rb+");
	fseek(Input,0,SEEK_END);
	FileSize=ftell(Input);
	NewFileSize=FileSize;
	File=new char[FileSize];
	fseek(Input,0,SEEK_SET);
	fread(File,1,FileSize,Input);
	

	Change=0;
	

	FindMCNKs();
	FixFlags();

	fclose(Input);


	delete File;
}
