#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <string>

FILE *Input;
char *File;
unsigned int FileSize;

void printflags(int a)
{
    for(int i = 31; i>=0;i--)
    {
         if(a & (1<<i))
              printf("0x%x ", 1<<i);
    } 
}

const char *getFileonly( char *a )
{
	std::string a2 = std::string ( a );
	for( int i = strlen(a) - 1; i >= 0; i-- )
	{
		if( a[i] == '\\' || a[i] == '/' )
			return a2.substr( i + 1 ).c_str();
	}
	return a2.c_str();
}

int main(int argc, char **argv)
{	
	unsigned int *TInt;
	float *TFloat;
	unsigned char *TChar;
	unsigned short *TShort;

	if( argc != 2 )
	{
		printf( "%s <model filename>\n", argv[0] );
		return 0;
	}
	
	unsigned int chunkmagic = 'MWMO';

	Input = fopen( argv[1], "rb+" );
	fseek( Input, 0, SEEK_END );
	FileSize = ftell( Input );
	File = new char[ FileSize  ];
	fseek( Input, 0, SEEK_SET );
	fread( File, 1, FileSize, Input );
	fclose( Input );
	
	int position = 0;
	while( position < FileSize )
	{
		unsigned int magic = *((unsigned int*)( File + position ));
		unsigned int size = *((unsigned int*)( File + position + 4 ));
		
		if( magic == chunkmagic )
		{
			printf( "%s: %x at x%x\n", getFileonly( argv[1] ), magic, position );
		} 
		
		position = position + 8 + size;
    }

	delete File;
}
