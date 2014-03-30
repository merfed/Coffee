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

	if( argc != 2 )
	{
		printf( "%s <map filename>\n", argv[0] );
		return 0;
	}
	else
		printf( "%s\n", argv[1] );

	Input = fopen( argv[1], "rb+" );
	fseek( Input, 0, SEEK_END );
	FileSize = ftell( Input );
	File = new char[ FileSize  ];
	fseek( Input, 0, SEEK_SET );
	fread( File, 1, FileSize, Input );
	fclose( Input );
	
	Positions = (MCIN *)( File + 0x5C );
	
	int ofsMDDF = *((unsigned int *)( File + 0x30 ));
	char * MDDF = (char *)( File + ofsMDDF + 0x14 );
	if( *((int*)MDDF) != 'MDDF' )
	{
		printf( "This is not MDDF. Oo?" );
		return -1;
	}
	int sizeMDDF = *((int*)( MDDF + 4 ));
	for( int i = 0; i < sizeMDDF/36; i++ )
	{
		unsigned int uid = *((unsigned int *)( MDDF + 8 + i * 0x24 + 0x04 ));
	//	unsigned short scale = *((unsigned short *)( MDDF + 8 + i * 0x24 + 0x20 ));
		unsigned short flags = *((unsigned short *)( MDDF + 8 + i * 0x24 + 0x22 ));
		if( flags != 0 )
		{
		 	printf( "\t%i: %x\n", uid, flags );
		}
	}

/*	for( int i = 0; i < 256; i++ )
	{		
		int nLayers = *((unsigned int *)( File + Positions[i].Offset + 8 + 0x0C ));
		int ofsMCLY = *((unsigned int *)( File + Positions[i].Offset + 8 + 0x1C ));
		for( int j = 0; j < nLayers; j++ )
		{
			unsigned int textureid = *((unsigned int *)( File + Positions[i].Offset + ofsMCLY + 8 + j * 0x10 + 0x00 ));
			unsigned int flags = *((unsigned int *)( File + Positions[i].Offset + ofsMCLY + 8 + j * 0x10 + 0x04 ));
			unsigned int MCAL = *((unsigned int *)( File + Positions[i].Offset + ofsMCLY + 8 + j * 0x10 + 0x08 ));
			short int effectId = *((short int *)( File + Positions[i].Offset + ofsMCLY + 8 + j * 0x10 + 0x0C ));
			unsigned short int unknown = *((unsigned short int *)( File + Positions[i].Offset + ofsMCLY + 8 + j * 0x10 + 0x0E ));
			
			if( flags & 0xFFFFF000 || unknown != 0 )
			printf( "\t%i, x%x: %i\n", argv[1], textureid, flags, unknown );
		}
	}*/

	delete File;
}
