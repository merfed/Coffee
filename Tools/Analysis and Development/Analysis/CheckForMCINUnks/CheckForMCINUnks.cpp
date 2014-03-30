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
		if( Positions[i].Temp1 || Positions[i].Temp2 )
		{
			printf( "\t%i: %i, %i\n", i, Positions[i].Temp1, Positions[i].Temp2 );
			foundsomething = true;
		}
	}
	
	if(foundsomething)
		printf( "in: %s\n", argv[1] );

	delete File;
}
