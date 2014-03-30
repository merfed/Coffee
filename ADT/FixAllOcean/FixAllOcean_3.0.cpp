#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DESCRIPTION "Deletes all MCLQ chunks."
#define ARGUMENTS "<map filename>"

#define USAGE( minimumArguments, argc, argv ) \
	if( argc < minimumArguments + 1 ) \
	{	 \
		printf( "  %s\n", argv[0] ); \
		printf( "    " DESCRIPTION "\n\n" ); \
		printf( "    Usage: \"%s " ARGUMENTS "\"\n", argv[0] ); \
		printf( "    Built at: " __DATE__ "\n" ); \
		return -1; \
	}
	
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
	USAGE( 1, argc, argv );
	
	int depth = 1;
	
	unsigned int *TInt;
	unsigned char *TChar;
	unsigned short *TShort;
	
	if( argc == 3 )
	depth = atoi(argv[2]);

	Input = fopen( argv[1], "rb+" );
	fseek( Input, 0, SEEK_END );
	FileSize = ftell( Input );
	File = new char[ FileSize ];           // And new chunk.
	fseek( Input, 0, SEEK_SET );
	fread( File, 1, FileSize, Input );
	fclose( Input );
	
	Positions = (MCIN *)( File + 0x5C );	

	for( int i = 0; i < 256; i++ )
	{		
		// delete MCLQ
		TInt = (unsigned int *)( File + Positions[i].Offset + 0x08 );
		*TInt = *TInt & ~(28); // b11100 = x1C = 28
	}


	Input = fopen( argv[1], "wb" );
	fwrite( File, 1, FileSize, Input );
	fclose( Input );

	delete File;
}
