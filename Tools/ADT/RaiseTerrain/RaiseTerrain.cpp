#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define DESCRIPTION "Raises all terrain."
#define ARGUMENTS "<map filename> <difference>"

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

char *File;

void ProcessObjects( float diff )
{	
	float *TFloat;
	
	int MDDF_Offset = *(int *)( File + 0x38 );
	int MODF_Offset = *(int *)( File + 0x3C );
	
	int NumDDFs = *(int *)( File + 0x14 + 0x04 + MDDF_Offset );
	int NumWMOs = *(int *)( File + 0x14 + 0x04 + MODF_Offset );
	
	for( int i = 0; i < NumDDFs; i++ )
	{
		TFloat = (float *)( File + 0x14 + 0x08 + MDDF_Offset + i * 0x24 + 0x8 );
		*TFloat = *TFloat + diff; TFloat++; 
		*TFloat = *TFloat + diff; TFloat++; 
		*TFloat = *TFloat + diff;
	}
	
	for( int i = 0; i < NumWMOs; i++ )
	{
		TFloat = (float *)( File + 0x14 + 0x08 + MODF_Offset + i * 0x40 + 0x8 );
		*TFloat = *TFloat + diff; TFloat++; 
		*TFloat = *TFloat + diff; TFloat++; 
		*TFloat = *TFloat + diff;
		
		TFloat = (float *)( File + 0x14 + 0x08 + MODF_Offset + i * 0x40 + 0x20 );
		*TFloat = *TFloat + diff; TFloat++; 
		*TFloat = *TFloat + diff; TFloat++; 
		*TFloat = *TFloat + diff; TFloat++; 
		*TFloat = *TFloat + diff; TFloat++; 
		*TFloat = *TFloat + diff; TFloat++; 
		*TFloat = *TFloat + diff;
	}
}

void ProcessHeightmap( float diff )
{
	float *TFloat;
	
	MCIN *Positions = (MCIN *)( File + 0x5C );
	
	for( int i = 0; i < 256; i++ )
	{	
		TFloat = (float *)( File + Positions[i].Offset + 0x70 + 8 );
		*TFloat = *TFloat + diff;
	}
}

int main(int argc, char **argv)
{
	USAGE( 2, argc, argv );
	
	float diff;
	diff = atof(argv[2]);

	FILE *Input = fopen( argv[1], "rb+" );
	fseek( Input, 0, SEEK_END );
	unsigned int FileSize = ftell( Input );
	File = new char[ FileSize ];
	fseek( Input, 0, SEEK_SET );
	fread( File, 1, FileSize, Input );
	fclose( Input );
	
	ProcessHeightmap( diff );
	ProcessObjects( diff );

	Input = fopen( argv[1], "wb" );
	fwrite( File, 1, FileSize, Input );
	fclose( Input );

	delete File;
}
