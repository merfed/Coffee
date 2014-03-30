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
		int oSE = *((unsigned int *)( File + Positions[i].Offset + 8 + 0x58 ));
		int nSE = *((unsigned int *)( File + Positions[i].Offset + 8 + 0x5C ));
		int sSE = *((unsigned int *)( File + Positions[i].Offset + oSE + 4 ));

		if( nSE || sSE )
		{
			printf( "\t%i: %x %x\n", i, nSE, sSE );
			foundsomething = true;
		}
	}
	
	if(foundsomething)
		printf( "in: %s\n", argv[1] );

	delete File;
}
