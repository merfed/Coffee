#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DESCRIPTION "Fixes several offsets that are wrong after moving an ADT and can move it upwards."
#define ARGUMENTS "<map filename> [<z change>]"

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
float ZChange;

struct MCIN{
	unsigned int Offset;
	unsigned int Size;
	unsigned int Temp1;
	unsigned int Temp2;
};

unsigned int MDDF_Offset;
unsigned int MODF_Offset;

MCIN Positions[256];
unsigned int MCNK_Positions[256];

void FindMCNKs()
{
	fseek(Input,92,SEEK_SET);
	fread(Positions,sizeof(MCIN),256,Input);
}

void FindMDDFandMODF()
{
	fseek(Input,0x30,SEEK_SET);
	fread(&MDDF_Offset,sizeof(unsigned int),1,Input);
	fread(&MODF_Offset,sizeof(unsigned int),1,Input);
}

float X_Offset;
float Z_Offset;

void FixMCNKs()
{
	float Y,X;
	float oldY,oldX;
	int i;

	i=0;

	fseek(Input,Positions[i].Offset+0x68+8,SEEK_SET);
	fread(&oldY,sizeof(float),1,Input);
	fread(&oldX,sizeof(float),1,Input);

	Y=(1600.0*(32-YOffset))/3.0-100.0*(i/16)/3.0;
	X=(1600.0*(32-XOffset))/3.0-100.0*(i%16)/3.0;

	//Its Reversed Remember
	X_Offset=oldX-X;
	Z_Offset=oldY-Y;

	printf("Offsets %.2f %.2f\n",X_Offset,Z_Offset);

	for (i=0;i<256;i++)
	{	
		fseek(Input,Positions[i].Offset+0x60+8,SEEK_SET);
		fread(&MCNK_Positions[i],sizeof(unsigned int),1,Input);
		MCNK_Positions[i]=MCNK_Positions[i]+Positions[i].Offset;

		fseek(Input,Positions[i].Offset+0x68+8,SEEK_SET);
		Y=(1600.0*(32-YOffset))/3.0-100.0*(i/16)/3.0;
		X=(1600.0*(32-XOffset))/3.0-100.0*(i%16)/3.0;
		fwrite(&Y,sizeof(float),1,Input);
		fwrite(&X,sizeof(float),1,Input);
		fread(&Y,sizeof(float),1,Input);
		Y=Y+ZChange;
		fseek(Input,-sizeof(float),SEEK_CUR);
		fwrite(&Y,sizeof(float),1,Input);
	}
}

void FixMCLQ()
{
	float WaterLevel;
	unsigned int *Test;

	Test=(unsigned int *)&WaterLevel;

	for (int i=0;i<256;i++)
	{	
		fseek(Input,MCNK_Positions[i]+8,SEEK_SET);
		fread(Test,sizeof(float),1,Input);
		if((*Test)==0x4d435345)
			continue;
		WaterLevel+=ZChange;
		fseek(Input,-sizeof(float),SEEK_CUR);
		fwrite(Test,sizeof(float),1,Input);

		for(int j=0;j<82;j++)
		{
			fread(Test,sizeof(float),1,Input);
			WaterLevel+=ZChange;
			fseek(Input,-sizeof(float),SEEK_CUR);
			fwrite(Test,sizeof(float),1,Input);
			fseek(Input,sizeof(float),SEEK_CUR);
		}
	}
}

void FixDoodads()
{
	int NumDoodads;
	float Temp[3];

	fseek(Input,0x14+0x04+MDDF_Offset,SEEK_SET);
	fread(&NumDoodads,sizeof(unsigned int),1,Input);

	NumDoodads=NumDoodads/36;

	fseek(Input,0x08,SEEK_CUR);
	for (int i=0;i<NumDoodads;i++)
	{
		fread(Temp,4,3,Input);
		Temp[0]=Temp[0]+X_Offset;
		Temp[1]=Temp[1]+ZChange;
		Temp[2]=Temp[2]+Z_Offset;
		fseek(Input,-12,SEEK_CUR);
		fwrite(Temp,4,3,Input);
		fseek(Input,24,SEEK_CUR);
	}
}

void FixWMOs()
{
	int NumWMOs;
	float Temp[3];

	fseek(Input,0x14+0x04+MODF_Offset,SEEK_SET);
	fread(&NumWMOs,sizeof(unsigned int),1,Input);

	NumWMOs=NumWMOs/64;

	fseek(Input,0x08,SEEK_CUR);
	for (int i=0;i<NumWMOs;i++)
	{
		fread(Temp,4,3,Input);
		Temp[0]=Temp[0]+X_Offset;
		Temp[1]=Temp[1]+ZChange;
		Temp[2]=Temp[2]+Z_Offset;
		fseek(Input,-12,SEEK_CUR);
		fwrite(Temp,4,3,Input);

		fseek(Input,12,SEEK_CUR);

		fread(Temp,4,3,Input);	
		Temp[0]=Temp[0]+X_Offset;
		Temp[1]=Temp[1]+ZChange;
		Temp[2]=Temp[2]+Z_Offset;
		fseek(Input,-12,SEEK_CUR);
		fwrite(Temp,4,3,Input);

		fread(Temp,4,3,Input);
		Temp[0]=Temp[0]+X_Offset;
		Temp[1]=Temp[1]+ZChange;
		Temp[2]=Temp[2]+Z_Offset;
		fseek(Input,-12,SEEK_CUR);
		fwrite(Temp,4,3,Input);

		fseek(Input,16,SEEK_CUR);
	}
}


int main(int argc, char **argv)
{
	USAGE( 1, argc, argv );

	char *replace;
	int i,len;

	if(argc>2)
		ZChange=atof(argv[2]);
	else
		ZChange=0;
	printf("Fixing File %s\n",argv[1]);
	Input=fopen(argv[1],"rb+");
	
	len=strlen(argv[1]);
	for(int i=0;i<len;i++)
	{
		if(argv[1][i]==' ')
			argv[1][i]='A';
		if(argv[1][i]=='_')
			argv[1][i]=' ';
		if(argv[1][i]=='.')
			argv[1][i]=0;

	}

	sscanf(argv[1],"%*s %d %d",&XOffset,&YOffset);

	FindMCNKs();
	FindMDDFandMODF();
	FixMCNKs();
	FixMCLQ();
	FixDoodads();
	FixWMOs();
	fclose(Input);
}
