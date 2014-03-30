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

unsigned char Flag;

void FindMCNKs()
{
	Positions=(MCIN *)(File+92);
}



void FindMCLQ()
{

	for (int i=0;i<256;i++)
	{
		MCLQ_Positions[i]=*((unsigned int *)(File+Positions[i].Offset+0x60+8))+Positions[i].Offset;
		MCSE_Positions[i]=*((unsigned int *)(File+Positions[i].Offset+0x58+8))+Positions[i].Offset;
	}
}

void FixMCLQ()
{
	float *TFloat;
	unsigned int *TInt;

	for (int i=0;i<256;i++)
	{	
		TFloat=(float *)(NewFile+MCLQ_Positions[i]+Change+8);
		*TFloat=WaterLevel;

		for(int j=0;j<81;j++)
		{
			TFloat=(float *)(NewFile+MCLQ_Positions[i]+Change+8+4+j*8);
			*TFloat=WaterLevel;
			TInt=(unsigned int *)(NewFile+MCLQ_Positions[i]+Change+8+4+j*8+4);
			*TInt=0xFF;
		}
		TFloat=(float *)(NewFile+MCLQ_Positions[i]+Change+8+652);
		*TFloat=WaterLevel;

		memset(NewFile+MCLQ_Positions[i]+Change+8+656,Flag,64);
		memset(NewFile+MCLQ_Positions[i]+Change+8+656+64,0,84);
	

		int Num;
		
		

		Positions[i].Offset=Positions[i].Offset+Change;

		TInt=(unsigned int *)(NewFile+Positions[i].Offset+8);
		*TInt=5;

		TInt=(unsigned int *)(NewFile+Positions[i].Offset+8+0x64);
		*TInt=812;

		TInt=(unsigned int *)(NewFile+Positions[i].Offset+8+0x58);
		*TInt+=812-(MCSE_Positions[i]-MCLQ_Positions[i]);


		Change+=812-(MCSE_Positions[i]-MCLQ_Positions[i]);

		if (i!=255)
			Num=MCLQ_Positions[i+1]-MCSE_Positions[i]+8;
		else
			Num=8;
		memcpy(NewFile+MCSE_Positions[i]+Change,File+MCSE_Positions[i],Num);

		
	}
}



int main(int argc, char **argv)
{
	char *replace;
	int i,len;
	if(argc<3)
	{
		printf("allwater <map filename> <water level> <optional waterflag default 20>\n");
		return 0;
	}

	if(argc>2)
		WaterLevel=atof(argv[2]);
	else
		WaterLevel=0;
	if(argc>3)
		Flag=(unsigned char)(atoi(argv[3])%256);
	else
		Flag=0x14;

	printf("Setting Water Level of %s to %.2f\n",argv[1],WaterLevel);
	Input=fopen(argv[1],"rb+");
	fseek(Input,0,SEEK_END);
	FileSize=ftell(Input);
	NewFileSize=FileSize;
	File=new char[FileSize];
	NewFile=new char[FileSize+804*256];
	fseek(Input,0,SEEK_SET);
	fread(File,1,FileSize,Input);
	fclose(Input);

	Change=0;
	
	
	FindMCNKs();
	FindMCLQ();
	FixMCLQ();
	memcpy(NewFile,File,MCLQ_Positions[0]+8);

	Input=fopen(argv[1],"wb");
	fwrite(NewFile,1,FileSize+Change,Input);
	fclose(Input);

	delete File;
	delete NewFile;
}
