#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DESCRIPTION "Removes all holes in the terrain."
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



void FixHoles()
{
	unsigned int *TFloat;
	unsigned int *TInt;
	for (int i=0;i<256;i++)
	{
		TInt=(unsigned int *)(File+Positions[i].Offset+0x8+0x3c);
		if(*TInt!=0)
		{
			printf("Found Holes in Chunk %d and patching",i,*TInt);
			*TInt=0;
			fseek(Input,Positions[i].Offset+0x8+0x3c,SEEK_SET);
			fwrite(TInt,sizeof(unsigned int),1,Input);
		}
	}
}



int main(int argc, char **argv)
{
	USAGE( 1, argc, argv );
	
	char *replace;
	int i,len;

	printf("Patchingg Holes from File %s\n",argv[1]);
	Input=fopen(argv[1],"rb+");
	fseek(Input,0,SEEK_END);
	FileSize=ftell(Input);
	NewFileSize=FileSize;
	File=new char[FileSize];
	fseek(Input,0,SEEK_SET);
	fread(File,1,FileSize,Input);
	

	Change=0;
	

	FindMCNKs();
	FixHoles();

	fclose(Input);


	delete File;
}
