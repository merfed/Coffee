#include <stdio.h>
#include <string.h>

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

char *ReplacingMe;
char *TheReplacement;

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
	unsigned int temp,temp2;
	temp2=0x14+0x04+MMDX_Offset;

	
	fseek(Input,0x14+0x04+MMDX_Offset,SEEK_SET);
	fread(&temp,sizeof(int),1,Input);
	printf("Position %d Size %d\n",temp2,temp);
	MDXFiles=new char[temp];

	fread(MDXFiles,sizeof(char),temp,Input);

	fseek(Input,0x14+0x04+MMID_Offset,SEEK_SET);
	fread(&temp,sizeof(int),1,Input);
	diffMDXs=temp/4;
	MDXOffsets=new int[diffMDXs];
	fread(MDXOffsets,sizeof(int),diffMDXs,Input);

	for (int i=0;i<diffMDXs ;i++ )
	{
		if(strcmp(ReplacingMe,MDXFiles+MDXOffsets[i])==0)
		{
			printf("Replacing %i (%s) with %s length %d\n",i,MDXFiles+MDXOffsets[i],TheReplacement,strlen(ReplacingMe));
			memcpy(MDXFiles+MDXOffsets[i],TheReplacement,strlen(TheReplacement)+1);
		}
	}

	fseek(Input,temp2,SEEK_SET);
	fread(&temp,sizeof(int),1,Input);
	fseek(Input,0,SEEK_CUR);
	fwrite(MDXFiles,1,temp,Input);
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


void LoadMCLQ()
{
	float WaterLevel;
	unsigned int *Test;
	char Temp[8];

	Test=(unsigned int *)&WaterLevel;

	for (int i=0;i<256;i++)
	{	
		fseek(Input,MCNK_Positions[i],SEEK_SET);
		fread(Temp,1,8,Input);
		fread(Test,sizeof(float),1,Input);
		if((*Test)==0x4d435345)
		{
			WaterLevels[i].Enabled=0;
			continue;
		}
		WaterLevels[i].Enabled=1;

		WaterLevels[i].First=WaterLevel;
		

		for(int j=0;j<81;j++)
		{
			fread(&(WaterLevels[i].Vertex[j]),sizeof(float),1,Input);
			fread(&(WaterLevels[i].Flags[j]),sizeof(float),1,Input);
		}
		fread(&(WaterLevels[i].Last),sizeof(float),1,Input);
	}
}

int main(int argc, char **argv)
{
	int i;
	char *replace;
	printf("Replacing in File %s\n",argv[1]);
	ReplacingMe=argv[2];
	TheReplacement=argv[3];
	if(strlen(TheReplacement)<=strlen(ReplacingMe))
		printf("Replacing %s with %s\n",ReplacingMe,TheReplacement);
	else
	{
		printf("The replacement string is too long\n");
		return 0;
	}

	Input=fopen(argv[1],"rb+");
	FindMDDFandMODF();
//	LoadMTEX();
	LoadMDXs();
//	LoadWMOs();
	fclose(Input);
}
