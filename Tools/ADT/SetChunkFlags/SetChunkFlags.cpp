#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DESCRIPTION "Sets the MCNK flags for all chunks in an ADT."
#define ARGUMENTS "<map filename> [<flag = 8>]\n      magma = 16, ocean = 8, river = 4"

#define USAGE( minimumArguments, argc, argv ) \
	if( argc < minimumArguments + 1 ) \
	{	 \
		printf( "  %s\n", argv[0] ); \
		printf( "    " DESCRIPTION "\n\n" ); \
		printf( "    Usage: \"%s " ARGUMENTS "\"\n", argv[0] ); \
		printf( "    Built at: " __DATE__ "\n" ); \
		return -1; \
	}
	
struct MCIN{
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
	
	int flag = 8;

	if( argc > 2 )
	flag = atoi(argv[2]);

	Input = fopen( argv[1], "rb+" );
	fseek( Input, 0, SEEK_END );
	FileSize = ftell( Input );
	File = new char[ FileSize ];
	fseek( Input, 0, SEEK_SET );
	fread( File, 1, FileSize, Input );
	fclose( Input );
	
	Positions = (MCIN *)( File+92 );

	unsigned int *TInt;

	for( int i = 0; i < 256; i++ )
	{		
		TInt = (unsigned int *)( File + Positions[i].Offset + 8 );

		// magma = 16
		// ocean = 8
		// river = 4

		// mask = 00011 = 3

		*TInt=*TInt & 3;
		*TInt=*TInt | flag;
		*TInt=*TInt | 1;
	}

	Input = fopen( argv[1], "wb" );
	fwrite( File, 1, FileSize, Input );
	fclose( Input );

	delete File;
}
