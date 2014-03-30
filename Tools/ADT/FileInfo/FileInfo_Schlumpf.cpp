#include <stdio.h>
#include <string.h>

#define DESCRIPTION "Saves info about an ADT into a txt file."
#define ARGUMENTS "<ADT filename>"

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

struct MCIN{
	unsigned int Offset;
	unsigned int Size;
	unsigned int Temp1;
	unsigned int Temp2;
};

/*
MCNK
0x14 MCVT Offset
0x18 MCNR Offset
0x1C MCLY Offset
0x20 MCRF Offset
0x24 MCAL Offset
0x2C MCSH Offset
0x52 MCSE Offset
0x60 MCLQ Offset
0x68 Z' Base Coordinate
0x6C X' Base Coordinate
0x70 Y Base Coordinate
*/
struct MCNK{
	float Z;
	float X;
	float Y;
};

struct Pos
{
	float x;
	float y;
	float z;
};

struct MCLQ
{
	int	  Enabled;
	float First;
	float Vertex[81];
	int	  Flags[81];
	float	Last;
};

MCIN Positions[256];
MCNK ADTOffsets[256];
MCLQ WaterLevels[256];
unsigned int MCNK_Positions[256];

unsigned int MTEX_Offset;
unsigned int MMDX_Offset;
unsigned int MMID_Offset;
unsigned int MWMO_Offset;
unsigned int MWID_Offset;
unsigned int MDDF_Offset;
unsigned int MODF_Offset;

int NumDoodads;
Pos *Doodads;


unsigned char *File;
unsigned int FileSize;

struct DDF
{
	unsigned int	ID;
	unsigned int	UniqueID;
	float			Pos[3];
	float			Rot[3];
	unsigned int	Scale;
};

struct WMO
{
	unsigned int	ID;
	unsigned int	UniqueID;
	float			Pos[3];
	float			Rot[3];
	float			Pos2[3];
	float			Pos3[3];
	short			Unknown1;
	short			DoodadIndex;
	unsigned int	Unknown2;
};

int NumWMOs;

WMO	*WMOs;

char *Textures;

char *MDXFiles;
int	diffMDXs;
int *MDXOffsets;

char *WMOFiles;
int	diffWMOs;
int	*WMOOffsets;

int	NumDDFs;
DDF *DDFs;

void FindMDDFandMODF()
{
	fseek(Input,0x1c,SEEK_SET);
	fread(&MTEX_Offset,sizeof(unsigned int),1,Input);
	fread(&MMDX_Offset,sizeof(unsigned int),1,Input);
	fread(&MMID_Offset,sizeof(unsigned int),1,Input);
	fread(&MWMO_Offset,sizeof(unsigned int),1,Input);
	fread(&MWID_Offset,sizeof(unsigned int),1,Input);
	fread(&MDDF_Offset,sizeof(unsigned int),1,Input);
	fread(&MODF_Offset,sizeof(unsigned int),1,Input);
}

short Map[256];

void LoadMTEX()
{
	unsigned int TexSize;
	fseek(Input,0x14+0x04+MTEX_Offset,SEEK_SET);
	fread(&TexSize,sizeof(int),1,Input);
	Textures=new char[TexSize];
	fread(Textures,sizeof(char),TexSize,Input);

	for(int i=0;i<TexSize-1;i++)
		if (Textures[i]==0)
			Textures[i]='\n';
}

void LoadMDXs()
{
	unsigned int temp;
	fseek(Input,0x14+0x04+MMDX_Offset,SEEK_SET);
	fread(&temp,sizeof(int),1,Input);
	MDXFiles=new char[temp];

	fread(MDXFiles,sizeof(char),temp,Input);

	fseek(Input,0x14+0x04+MMID_Offset,SEEK_SET);
	fread(&temp,sizeof(int),1,Input);
	diffMDXs=temp/4;
	MDXOffsets=new int[diffMDXs];
	fread(MDXOffsets,sizeof(int),diffMDXs,Input);


	fseek(Input,0x14+0x04+MDDF_Offset,SEEK_SET);
	fread(&NumDDFs,sizeof(unsigned int),1,Input);

	NumDDFs=NumDDFs/sizeof(DDF);
	DDFs=new DDF[NumDDFs];
	fread(DDFs,sizeof(DDF),NumDDFs,Input);	
}


void LoadWMOs()
{
	unsigned int temp;
	fseek(Input,0x14+0x04+MWMO_Offset,SEEK_SET);
	fread(&temp,sizeof(int),1,Input);
	WMOFiles=new char[temp];

	fread(WMOFiles,sizeof(char),temp,Input);

	fseek(Input,0x14+0x04+MWID_Offset,SEEK_SET);
	fread(&temp,sizeof(int),1,Input);
	diffWMOs=temp/4;
	WMOOffsets=new int[diffWMOs];
	fread(WMOOffsets,sizeof(int),diffWMOs,Input);


	fseek(Input,0x14+0x04+MODF_Offset,SEEK_SET);
	fread(&NumWMOs,sizeof(unsigned int),1,Input);

	NumWMOs=NumWMOs/64;
	WMOs=new WMO[NumWMOs];
	fread(WMOs,sizeof(WMO),NumWMOs,Input);	
}

int main(int argc, char **argv)
{
	USAGE( 1, argc, argv );

	int i;
	char *replace;
	printf("Extracting Info From File %s\n",argv[1]);
	Input=fopen(argv[1],"rb");
	if(Input==0)
	{
		printf("ERROR: Could not open %s\n",argv[1]);
		return 0;
	}
	printf("Finding MDDF's & MODF's\n");
	FindMDDFandMODF();
	printf("Loading MTEX\n");
	LoadMTEX();
	printf("Loading MDDX\n");
	LoadMDXs();
	printf("Loading WMOs\n");
	LoadWMOs();
	fclose(Input);
	printf("Closing File\n");

	replace=strstr(argv[1],"adt");
	replace[0]='t';
	replace[1]='x';
	replace[2]='t';

	FILE *Output;
	Output=fopen(argv[1],"wt");
	if(Output==0)
	{
		printf("ERROR: Couldn't open %s for saving\n",argv[1]);
		return 0;
	}
	printf("Writing Info To File %s\n",argv[1]);
	fprintf(Output,"MMDX\t%d\n",diffMDXs);	
	for(i=0;i<diffMDXs;i++)
		fprintf(Output,"%d\t%s\n",i,MDXFiles+MDXOffsets[i]);

	fprintf(Output,"\nMWMO\t%d\n",diffWMOs);	
	for(i=0;i<diffWMOs;i++)
		fprintf(Output,"%d\t%s\n",i,WMOFiles+WMOOffsets[i]);

	fprintf(Output,"\nMDDF\t%d\n",NumDDFs);
	for(i=0;i<NumDDFs;i++)
		fprintf(Output,"%d\t%d\t%f\t%f\t%f\t%f\t%f\t%f\t%d\n",DDFs[i].ID,DDFs[i].UniqueID,DDFs[i].Pos[0],DDFs[i].Pos[1],DDFs[i].Pos[2],DDFs[i].Rot[0],DDFs[i].Rot[1],DDFs[i].Rot[2],DDFs[i].Scale);

	fprintf(Output,"\nMODF\t%d\n",NumWMOs);
	for(i=0;i<NumWMOs;i++)
		fprintf(Output,"%d\t%d\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%d\n",WMOs[i].ID,WMOs[i].UniqueID,WMOs[i].Pos[0],WMOs[i].Pos[1],WMOs[i].Pos[2],WMOs[i].Rot[0],WMOs[i].Rot[1],WMOs[i].Rot[2],WMOs[i].Pos2[0],WMOs[i].Pos2[1],WMOs[i].Pos2[2],WMOs[i].Pos3[0],WMOs[i].Pos3[1],WMOs[i].Pos3[2],WMOs[i].DoodadIndex);
	fclose(Output);
}
