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

const char *getExt( char *a )
{
	std::string a2 = std::string ( a );
	for( int i = strlen(a) - 1; i >= 0; i-- )
	{
		if( a[i] == '.' )
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
	
	unsigned int chunkmagic[100];
	int magix = 1;
	chunkmagic[0] = 'MVER';
	
	
	/** Root WMOs **/
	magix += 17;
	chunkmagic[1+0] = 'MOHD';
	chunkmagic[1+1] = 'MOTX';
	chunkmagic[1+2] = 'MOMT';
	chunkmagic[1+3] = 'MOGN';
	chunkmagic[1+4] = 'MOGI';
	chunkmagic[1+5] = 'MOSB';
	chunkmagic[1+6] = 'MOPV';
	chunkmagic[1+7] = 'MOPT';
	chunkmagic[1+8] = 'MOPR';
	chunkmagic[1+9] = 'MOVV';
	chunkmagic[1+10] = 'MOVB';
	chunkmagic[1+11] = 'MOLT';
	chunkmagic[1+12] = 'MODS';
	chunkmagic[1+13] = 'MODN';
	chunkmagic[1+14] = 'MODD';
	chunkmagic[1+15] = 'MFOG';
	chunkmagic[1+16] = 'MCVP';
	
	/** Group WMOs **/
	magix += 13;
	chunkmagic[1+17+0] = 'MOGP';
	chunkmagic[1+17+1] = 'MOPY';
	chunkmagic[1+17+2] = 'MOVI';
	chunkmagic[1+17+3] = 'MOVT';
	chunkmagic[1+17+4] = 'MONR';
	chunkmagic[1+17+5] = 'MOTV';
	chunkmagic[1+17+6] = 'MOBA';
	chunkmagic[1+17+7] = 'MOLR';
	chunkmagic[1+17+8] = 'MODR';
	chunkmagic[1+17+9] = 'MOBN';
	chunkmagic[1+17+10] = 'MOBR';
	chunkmagic[1+17+11] = 'MOCV';
	chunkmagic[1+17+12] = 'MLIQ';

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
		
		bool found = false;
		for( int i = 0; i < magix; i++ )
			if( magic == chunkmagic[i] )
			{
				found = true;
				break;
			}
			
		if( !found )
			printf( "%s: %x at x%x\n", getFileonly( argv[1] ), magic, position );
		
		position = position + 8 + size;
    }

	delete File;
}
