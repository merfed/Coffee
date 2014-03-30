#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DESCRIPTION "Loads info from a txt file made by FileInfo back into the ADT."
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

MCIN *Positions;
MCNK ADTOffsets[256];
MCLQ WaterLevels[256];
unsigned int MCNK_Positions[256];

unsigned int *MTEX_Offset;
unsigned int *MMDX_Offset;
unsigned int *MMID_Offset;
unsigned int *MWMO_Offset;
unsigned int *MWID_Offset;
unsigned int *MDDF_Offset;
unsigned int *MODF_Offset;

int NumDoodads;
Pos *Doodads;


unsigned char *File;
unsigned int FileSize;
unsigned char *NewFile;
unsigned int NewFileSize;
unsigned int Change;

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
int MMDXSize;

char *WMOFiles;
int	diffWMOs;
int	*WMOOffsets;
int MWMOSize;

int	NumDDFs;
DDF *DDFs;

void FindOffsets()
{
	MMDX_Offset=(unsigned int*)(File+0x20);
	MMID_Offset=(unsigned int*)(File+0x24);
	MWMO_Offset=(unsigned int*)(File+0x28);
	MWID_Offset=(unsigned int*)(File+0x2c);
	MDDF_Offset=(unsigned int*)(File+0x30);
	MODF_Offset=(unsigned int*)(File+0x34);
	Positions=(MCIN *)(File+0x5c);

}

short Map[256];

void LoadChanges()
{
	unsigned char *Temp;
	unsigned int *T2,T3;
	printf("Loading MMDX Info (%x)\n",*MMDX_Offset);
	Temp=0x14+*MMDX_Offset+NewFile+Change;
	Temp[0]='X';
	Temp[1]='D';
	Temp[2]='M';
	Temp[3]='M';

	printf("Copying bytes %d\n",MMDXSize);
	
	T2=(unsigned int *)(0x14+0x04+*MMDX_Offset+NewFile+Change);
	*T2=MMDXSize;
	
	memcpy(0x14+*MMDX_Offset+NewFile+0x8+Change,MDXFiles,MMDXSize);

	T3=Change;
	Change+=MMDXSize-*((unsigned int *)(0x14+0x04+*MMDX_Offset+File));
	*MMDX_Offset+=T3;

	printf("Current Change %d\n",Change);

	printf("Loading MMID Info (%x)\n",*MMID_Offset);
	Temp=0x14+*MMID_Offset+NewFile+Change;
	Temp[0]='D';
	Temp[1]='I';
	Temp[2]='M';
	Temp[3]='M';

	T2=(unsigned int *)(0x14+*MMID_Offset+NewFile+Change+0x4);
	*T2=diffMDXs*4;
	memcpy(0x14+*MMID_Offset+NewFile+0x8+Change,MDXOffsets,*T2);

	T3=Change;
	Change+=*T2-*((unsigned int *)(0x14+0x04+*MMID_Offset+File));
	*MMID_Offset+=T3;

	printf("Current Change %d\n",Change);

	printf("Loading MWMO Info\n");
	Temp=0x14+*MWMO_Offset+NewFile+Change;
	Temp[0]='O';
	Temp[1]='M';
	Temp[2]='W';
	Temp[3]='M';

	T2=(unsigned int *)(0x14+0x04+*MWMO_Offset+NewFile+Change);
	*T2=MWMOSize;
	memcpy(0x14+*MWMO_Offset+NewFile+0x8+Change,WMOFiles,MWMOSize);

	T3=Change;
	Change+=MWMOSize-*((unsigned int *)(0x14+0x04+*MWMO_Offset+File));
	*MWMO_Offset+=T3;

	printf("Current Change %d\n",Change);

	printf("Loading MWID Info\n");
	Temp=0x14+*MWID_Offset+NewFile+Change;
	Temp[0]='D';
	Temp[1]='I';
	Temp[2]='W';
	Temp[3]='M';

	T2=(unsigned int *)(0x14+*MWID_Offset+NewFile+Change+0x4);
	*T2=diffWMOs*4;
	memcpy(0x14+*MWID_Offset+NewFile+0x8+Change,WMOOffsets,*T2);

	T3=Change;
	Change+=*T2-*((unsigned int *)(0x14+0x04+*MWID_Offset+File));
	*MWID_Offset+=T3;

	printf("Current Change %d\n",Change);

	printf("Loading MDDF Info\n");
	printf("Old Size %d\nNew Size %d\n\n",*((unsigned int *)(0x14+0x04+*MDDF_Offset+File)),NumDDFs*sizeof(DDF));
	Temp=0x14+*MDDF_Offset+NewFile+Change;
	Temp[0]='F';
	Temp[1]='D';
	Temp[2]='D';
	Temp[3]='M';

	T2=(unsigned int *)(0x14+0x04+*MDDF_Offset+NewFile+Change);
	*T2=NumDDFs*sizeof(DDF);
	memcpy(0x14+*MDDF_Offset+NewFile+0x8+Change,DDFs,*T2);

	T3=Change;
	Change+=*T2-*((unsigned int *)(0x14+0x04+*MDDF_Offset+File));
	*MDDF_Offset+=T3;

	printf("Current Change %d\n",Change);

	printf("Loading MODF Info\n");
	Temp=0x14+*MODF_Offset+NewFile+Change;
	Temp[0]='F';
	Temp[1]='D';
	Temp[2]='O';
	Temp[3]='M';

	T2=(unsigned int *)(0x14+0x04+*MODF_Offset+NewFile+Change);
	*T2=NumWMOs*sizeof(WMO);
	memcpy(0x14+*MODF_Offset+NewFile+0x8+Change,WMOs,*T2);

	T3=Change;
	Change+=*T2-*((unsigned int *)(0x14+0x04+*MODF_Offset+File));
	*MODF_Offset+=T3;

	printf("Changing offsets with a total change of %d\n",Change);
	unsigned int FirstMCNK=Positions[0].Offset;
	for (int i=0;i<256 ;i++ )
		Positions[i].Offset+=Change;
	memcpy(NewFile,File,0x14+*MMDX_Offset);
	memcpy(NewFile+Positions[0].Offset,File+FirstMCNK,FileSize-(FirstMCNK));
}


int main(int argc, char **argv)
{
	USAGE( 1, argc, argv );
	
	int i,StringOffset;
	char *replace;
	int		TempInt,TotalLength;
	char TempStr[512];
	char TempList[512][256];
	char FName[256];

	strcpy(FName,argv[1]);
	
	replace=strstr(FName,"adt");
	replace[0]='t';
	replace[1]='x';
	replace[2]='t';

	FILE *Output;
	Output=fopen(FName,"rt");
	printf("Loading Info From File %s\n",argv[1]);
	fscanf(Output,"MMDX %d",&diffMDXs);	
	TotalLength=0;
	MDXOffsets=new int[diffMDXs];
	for(i=0;i<diffMDXs;i++)
	{
		fgets(TempStr,512,Output);
		if(strlen(TempStr)<3)
		{
			i--;
			continue;
		}
		printf("Read %s",TempStr);
		for(int j=0;j<512;j++)
		{
			if(TempStr[j]=='\t')
			{
				TempInt=atoi(TempStr);
				TempStr[j]=0;
				StringOffset=j+1;
			}
			else if(TempStr[j]=='\n')
			{
				TempStr[j]=0;
				break;
			}
		}

		printf("Loaded %d %s\n",TempInt,TempStr+StringOffset);
		strcpy(TempList[TempInt],TempStr+StringOffset);
		TotalLength+=strlen(TempStr+StringOffset)+1;
	}

	MMDXSize=TotalLength;
	
	MDXFiles=new char[TotalLength];
	TempInt=0;
	for(i=0;i<diffMDXs;i++)
	{
		MDXOffsets[i]=TempInt;
		strcpy(MDXFiles+TempInt,TempList[i]);
		TempInt+=strlen(TempList[i])+1;
	}

	fgets(TempStr,512,Output);
	while(strlen(TempStr)<3)
	{
		fgets(TempStr,512,Output);
		if(feof(Output))
		{
			printf("Error!\n");
			return 0;
		}
	}

	sscanf(TempStr,"MWMO %d",&diffWMOs);	
	printf("Read %d\n",diffWMOs);
	TotalLength=0;
	WMOOffsets=new int[diffWMOs];
	for(i=0;i<diffWMOs;i++)
	{
		fgets(TempStr,512,Output);
		if(strlen(TempStr)<3)
		{
			i--;
			continue;
		}
		printf("Read %s",TempStr);
		for(int j=0;j<512;j++)
		{
			if(TempStr[j]=='\t')
			{
				TempInt=atoi(TempStr);
				TempStr[j]=0;
				StringOffset=j+1;
			}
			else if(TempStr[j]=='\n')
			{
				TempStr[j]=0;
				break;
			}
		}

		printf("Loaded %d %s\n",TempInt,TempStr+StringOffset);
		strcpy(TempList[TempInt],TempStr+StringOffset);
		TotalLength+=strlen(TempStr+StringOffset)+1;
	}

	
	MWMOSize=TotalLength;
	WMOFiles=new char[TotalLength];
	TempInt=0;
	for(i=0;i<diffWMOs;i++)
	{
		WMOOffsets[i]=TempInt;
		strcpy(WMOFiles+TempInt,TempList[i]);
		TempInt+=strlen(TempList[i])+1;
	}

	fgets(TempStr,512,Output);
	while(strlen(TempStr)<3)
	{
		fgets(TempStr,512,Output);
		if(feof(Output))
		{
			printf("Error!\n");
			return 0;
		}
	}

	sscanf(TempStr,"MDDF %d",&NumDDFs);
	printf("There is %d doodads\n",NumDDFs);
	DDFs=new DDF[NumDDFs];
	for(i=0;i<NumDDFs;i++)
		fscanf(Output,"%d %d %f %f %f %f %f %f %d",&DDFs[i].ID,&DDFs[i].UniqueID,&DDFs[i].Pos[0],&DDFs[i].Pos[1],&DDFs[i].Pos[2],&DDFs[i].Rot[0],&DDFs[i].Rot[1],&DDFs[i].Rot[2],&DDFs[i].Scale);

	fgets(TempStr,512,Output);
	while(strlen(TempStr)<3)
	{
		fgets(TempStr,512,Output);
		if(feof(Output))
		{
			printf("Error!\n");
			return 0;
		}
	}
	sscanf(TempStr,"MODF %d",&NumWMOs);
	printf("There is %d WMOs\n",NumWMOs);
	WMOs=new WMO[NumWMOs];
	for(i=0;i<NumWMOs;i++)
	{
		fscanf(Output,"%d %d %f %f %f %f %f %f %f %f %f %f %f %f %d",&(WMOs[i].ID),&(WMOs[i].UniqueID),&(WMOs[i].Pos[0]),&(WMOs[i].Pos[1]),&(WMOs[i].Pos[2]),&WMOs[i].Rot[0],&WMOs[i].Rot[1],&WMOs[i].Rot[2],&WMOs[i].Pos2[0],&WMOs[i].Pos2[1],&WMOs[i].Pos2[2],&WMOs[i].Pos3[0],&WMOs[i].Pos3[1],&WMOs[i].Pos3[2],&WMOs[i].DoodadIndex);
		WMOs[i].Unknown1=0;
		WMOs[i].Unknown2=0;
	}
	fclose(Output);

	replace[0]='a';
	replace[1]='d';
	replace[2]='t';

	printf("Loading Info Into File %s\n",FName);
	Input=fopen(FName,"rb");
	fseek(Input,0,SEEK_END);
	FileSize=ftell(Input);

	File=new unsigned char[FileSize];
	fseek(Input,0,SEEK_SET);
	fread(File,1,FileSize,Input);
	fclose(Input);
	NewFileSize=FileSize;
	NewFile=new unsigned char[2*FileSize];
	Change=0;

	FindOffsets();
	LoadChanges();

	Output=fopen(FName,"wb");
	fwrite(NewFile,1,FileSize+Change,Output);
	fclose(Output);

	delete NewFile;
	delete File;
}
