#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

struct MCIN
{
	unsigned int Offset;
	unsigned int Size;
	unsigned int Temp1;
	unsigned int Temp2;
};

MCIN *Positions;

FILE *Input;
char *File;
unsigned int FileSize;

int main(int argc, char **argv)
{	
	unsigned int *TInt;
	float *TFloat;
	unsigned char *TChar;
	unsigned short *TShort;
	bool foundsomething = false;

	if( argc != 2 )
	{
		printf( "%s <map filename>\n", argv[0] );
		return 0;
	}

	Input = fopen( argv[1], "rb+" );
	fseek( Input, 0, SEEK_END );
	FileSize = ftell( Input );
	File = new char[ FileSize  ];
	fseek( Input, 0, SEEK_SET );
	fread( File, 1, FileSize, Input );
	fclose( Input );
	
	Positions = (MCIN *)( File + 0x5C );

	for( int i = 0; i < 256; i++ )
	{		
		int unkpredTex = *((unsigned int *)( File + Positions[i].Offset + 8 + 0x50 ));
		int unknoEffectDoodad = *((unsigned int *)( File + Positions[i].Offset + 8 + 0x54 ));
/*		for( int j = 0; j < nLayers; j++ )
		{
			unsigned int textureid = *((unsigned int *)( File + Positions[i].Offset + ofsMCLY + 8 + j * 0x10 + 0x00 ));
			unsigned int flags = *((unsigned int *)( File + Positions[i].Offset + ofsMCLY + 8 + j * 0x10 + 0x04 ));
			unsigned int MCAL = *((unsigned int *)( File + Positions[i].Offset + ofsMCLY + 8 + j * 0x10 + 0x08 ));
			short int effectId = *((short int *)( File + Positions[i].Offset + ofsMCLY + 8 + j * 0x10 + 0x0C ));
			unsigned short int unknown = *((unsigned short int *)( File + Positions[i].Offset + ofsMCLY + 8 + j * 0x10 + 0x0E ));
			*/
		if( unkpredTex || unknoEffectDoodad )
		{
			printf( "\t%i: %x %x\n", i, unkpredTex, unknoEffectDoodad );
			foundsomething = true;
		}
	}
	
	if(foundsomething)
		printf( "in: %s\n", argv[1] );

	delete File;
}
