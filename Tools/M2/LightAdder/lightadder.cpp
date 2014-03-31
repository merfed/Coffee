//============================================================================
// Name        : lightadder.cpp
// Author      : Tigurius
// Version     :
// Copyright   : GPL
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "lightadder.h"

int fillLine(FILE *f)
{
	unsigned int Pos, linePos;
	char blank[16];
	memset(blank,0,16);
	Pos=ftell(f);
	linePos=Pos&0xFFFFFFF0;
	fwrite(blank,1,16-(Pos-linePos),f);
	return 16-(Pos-linePos);
}

FILE *Input;
char *File;

uint32 FileSize;

int main(int argc, char **argv) {

	unsigned int *TInt;
	float *TFloat;
	light *TLight;


	int n=0;

	if( argc < 2 )
	{
		printf( "Lightadder <filename>\n" );
		return 0;
	}

	Input = fopen( argv[1], "ab+" );
	fseek( Input, 0, SEEK_END );
	fillLine(Input);
	fseek( Input, 0, SEEK_END );
	FileSize = ftell( Input );

	printf("How many Lights?\t");
	scanf("%d",&n);
	File = new char[ FileSize + n*sizeof(light)+(16-( n*sizeof(light))%16)+n*light_end ];           // And lightz
	fseek( Input, 0, SEEK_SET );
	fread( File, 1, FileSize, Input );
	fclose( Input );

	TInt=(unsigned int *)(File+nLight);
	*TInt=n;
	TInt=(unsigned int *)( File + ofsLight );
	*TInt=FileSize;

	unsigned int lightofs=FileSize+n*sizeof(light)+(16-(n*sizeof(light))%16);

	float acolor[n][3],aintensity[n],dcolor[n][3],
	dintensity[n],astart[n],aend[n];
	for(int i=0;i<n;i++){

		light mylight;

		printf("Type(0 direct, 1 point):\t");
		scanf("%d",&mylight.type);
		printf("Bone:\t");
		scanf("%d",&mylight.bone);
		printf("Position:\t");
		printf("x:\t");
		scanf("%f",&mylight.pos[0]);
		printf("y:\t");
		scanf("%f",&mylight.pos[1]);
		printf("z:\t");
		scanf("%f",&mylight.pos[2]);
		printf("Ambient:");
		printf("r:\t");
		scanf("%f",&acolor[i][0]);
		printf("g:\t");
		scanf("%f",&acolor[i][1]);
		printf("b:\t");
		scanf("%f",&acolor[i][2]);
		printf("intensity:\t");
		scanf("%f",&aintensity[i]);
		printf("Diffuse:");
		printf("r:\t");
		scanf("%f",&dcolor[i][0]);
		printf("g:\t");
		scanf("%f",&dcolor[i][1]);
		printf("b:\t");
		scanf("%f",&dcolor[i][2]);
		printf("intensity:\t");
		scanf("%f",&dintensity[i]);
		printf("AttenuationStart: ");
		scanf("%f",&astart[i]);
		printf("AttenuationEnd: ");
		scanf("%f",&aend[i]);

		mylight.acolor.nKey=1;
		mylight.acolor.minusone=-1;
		mylight.acolor.offsetkey=lightofs+i*light_end+acolor_key;
		mylight.acolor.nTime=1;
		mylight.acolor.zero=0;
		mylight.acolor.offsettime=lightofs+i*light_end+acolor_time;

		mylight.aintensity.nKey=1;
		mylight.aintensity.minusone=-1;
		mylight.aintensity.offsetkey=lightofs+i*light_end+aintensity_key;
		mylight.aintensity.nTime=1;
		mylight.aintensity.zero=0;
		mylight.aintensity.offsettime=lightofs+i*light_end+aintensity_time;

		mylight.dcolor.nKey=1;
		mylight.dcolor.minusone=-1;
		mylight.dcolor.offsetkey=lightofs+i*light_end+dcolor_key;
		mylight.dcolor.nTime=1;
		mylight.dcolor.zero=0;
		mylight.dcolor.offsettime=lightofs+i*light_end+dcolor_time;

		mylight.dintensity.nKey=1;
		mylight.dintensity.minusone=-1;
		mylight.dintensity.offsetkey=lightofs+i*light_end+dintensity_key;
		mylight.dintensity.nTime=1;
		mylight.dintensity.zero=0;
		mylight.dintensity.offsettime=lightofs+i*light_end+dintensity_time;

		mylight.astart.nKey=1;
		mylight.astart.minusone=-1;
		mylight.astart.offsetkey=lightofs+i*light_end+astart_key;
		mylight.astart.nTime=1;
		mylight.astart.zero=0;
		mylight.astart.offsettime=lightofs+i*light_end+astart_time;

		mylight.aend.nKey=1;
		mylight.aend.minusone=-1;
		mylight.aend.offsetkey=lightofs+i*light_end+aend_key;
		mylight.aend.nTime=1;
		mylight.aend.zero=0;
		mylight.aend.offsettime=lightofs+i*light_end+aend_time;


		TLight=(light *)(File+FileSize+i*sizeof(light));
		*TLight=mylight;
	}
	for(int i=0;i<n;i++){
		unsigned int offsetofs=lightofs+i*light_end;

		TInt=(unsigned int *)(File+offsetofs+acolor_time);
		*TInt=1;TInt++;
		*TInt=offsetofs+acolor_time_v;
		TInt=(unsigned int *)(File+offsetofs+acolor_key);
		*TInt=1;TInt++;
		*TInt=offsetofs+acolor_key_v;
		TFloat=(float *)(File+offsetofs+acolor_key_v);
		*TFloat=acolor[i][0];TFloat++;
		*TFloat=acolor[i][1];TFloat++;
		*TFloat=acolor[i][2];TFloat++;

		TInt=(unsigned int *)(File+offsetofs+aintensity_time);
		*TInt=1;TInt++;
		*TInt=offsetofs+aintensity_time_v;
		TInt=(unsigned int *)(File+offsetofs+aintensity_key);
		*TInt=1;TInt++;
		*TInt=offsetofs+aintensity_key_v;
		TFloat=(float *)(File+offsetofs+aintensity_key_v);
		*TFloat=aintensity[i];TFloat++;

		TInt=(unsigned int *)(File+offsetofs+dcolor_time);
		*TInt=1;TInt++;
		*TInt=offsetofs+dcolor_time_v;
		TInt=(unsigned int *)(File+offsetofs+dcolor_key);
		*TInt=1;TInt++;
		*TInt=offsetofs+dcolor_key_v;
		TFloat=(float *)(File+offsetofs+dcolor_key_v);
		*TFloat=dcolor[i][0];TFloat++;
		*TFloat=dcolor[i][1];TFloat++;
		*TFloat=dcolor[i][2];TFloat++;

		TInt=(unsigned int *)(File+offsetofs+dintensity_time);
		*TInt=1;TInt++;
		*TInt=offsetofs+dintensity_time_v;
		TInt=(unsigned int *)(File+offsetofs+dintensity_key);
		*TInt=1;TInt++;
		*TInt=offsetofs+dintensity_key_v;
		TFloat=(float *)(File+offsetofs+dintensity_key_v);
		*TFloat=aintensity[i];TFloat++;

		TInt=(unsigned int *)(File+offsetofs+astart_time);
		*TInt=1;TInt++;
		*TInt=offsetofs+astart_time_v;
		TInt=(unsigned int *)(File+offsetofs+astart_key);
		*TInt=1;TInt++;
		*TInt=offsetofs+astart_key_v;
		TFloat=(float *)(File+offsetofs+astart_key_v);
		*TFloat=astart[i];TFloat++;

		TInt=(unsigned int *)(File+offsetofs+aend_time);
		*TInt=1;TInt++;
		*TInt=offsetofs+aend_time_v;
		TInt=(unsigned int *)(File+offsetofs+aend_key);
		*TInt=1;TInt++;
		*TInt=offsetofs+aend_key_v;
		TFloat=(float *)(File+offsetofs+aend_key_v);
		*TFloat=aend[i];TFloat++;

		TInt=(unsigned int *)(File+offsetofs+unk1_time);
		*TInt=1;TInt++;
		*TInt=offsetofs+unk1_time_v;
		TInt=(unsigned int *)(File+offsetofs+unk1_key);
		*TInt=1;TInt++;
		*TInt=offsetofs+unk1_key_v;
		TFloat=(float *)(File+offsetofs+unk1_key_v);
		*TFloat=1;TFloat++;
	}



	Input = fopen( argv[1], "wb" );
	fwrite( File, 1, FileSize + n*sizeof(light)+(16-( n*sizeof(light))%16)+n*light_end, Input );
	fclose( Input );

	delete File;

	return 0;
}
