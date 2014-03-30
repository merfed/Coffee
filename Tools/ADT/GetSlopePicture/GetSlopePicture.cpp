#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DESCRIPTION "[NOT STABLE] Tries to make a pgm-image from the slopes in the terrain."
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
unsigned int MCVT_Positions[256];
float heights[256][145];
float heights2[256][9][9];
float bigarray[256*9][256*9];
char bigarrayc[256*9][256*9];

char *File;
unsigned int FileSize;

char *NewFile;
unsigned int NewFileSize;
unsigned int Change;

unsigned char Flag;

void FindPositions()
{
	Positions=(MCIN *)(File+92);
	
	for (int i=0;i<256;i++)
	{
		MCVT_Positions[i]=*((unsigned int *)(File+Positions[i].Offset+0x14+8))+Positions[i].Offset;
	}
}

void getMCVT()
{
	for (int i=0;i<256;i++)
	{
		for(int j=0;j<145;j++)
		{
			float* temp = (float *)(File+MCVT_Positions[i]+j*(sizeof(float)));
            heights[i][j] = *temp;
		}
	}
}

void getarray2()
{
	for(int ch = 0; ch < 256; ch++)
	{
		printf("ch: %i\n",ch);
		
		int y = 0, x = 0, z = 0;

		while(z<145)
		{
			for(int t=0;t<9;t++)
			{
				heights2[ch][y][x++]=heights[ch][z];
			//	printf("%f |", heights[ch][z]);
			}
			z+=8;
			y++;
			x=0;
		}
	}
}

void getbigarray()
{
	for(int a=0;a<256;a++)
	{
        for(int b=0;b<256;b++)
		{
			for(int i = 0; i<9;i++)
			{
				for(int j = 0; j<9;j++)
				{
					bigarray[(a+i)][(b+j)] = heights2[a][i][j];
					bigarrayc[(a+i)][(b+j)] = (int)(heights2[a][i][j]);
				}
			}
		}
	}
}

int main(int argc, char **argv)
{
	USAGE( 1, argc, argv );
	
	char *replace;
	int i,len;
		Input=fopen(argv[1],"rb+");
	fseek(Input,0,SEEK_END);
	FileSize=ftell(Input);
	NewFileSize=FileSize;
	File=new char[FileSize];
	NewFile=new char[FileSize+804*256];
	fseek(Input,0,SEEK_SET);
	fread(File,1,FileSize,Input);
	fclose(Input);


printf("findpos();\n");
	FindPositions();
printf("getMCVT();\n");
    getMCVT();
printf("getarray2();\n");
    getarray2();
printf("getbigarray();\n\n");
    getbigarray();
    

printf("gen header\n");
    char file2[18];
    sprintf(file2, "P5\n2304 2304\n255\n");
printf("open file\n");
    Input=fopen("map.pgm","wb");
printf("write header\n");
	fwrite(file2,1,18,Input);
printf("write bigarray\n");
	for(int i = 0; i<2304;i++)
	{
		for(int j = 0; j<2304;j++)
		{
			fwrite(&bigarrayc[i][j],sizeof(char),1,Input);
		}
	}
	fclose(Input);
}
