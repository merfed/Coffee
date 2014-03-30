#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

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
		printf( "%s <model filename>\n", argv[0] );
		return 0;
	}
	else

	Input = fopen( argv[1], "rb+" );
	fseek( Input, 0, SEEK_END );
	FileSize = ftell( Input );
	File = new char[ FileSize  ];
	fseek( Input, 0, SEEK_SET );
	fread( File, 1, FileSize, Input );
	fclose( Input );
	
	int nTextures = *((int *)( File + 0x50 ));
	int oTextures = *((int *)( File + 0x54 ));

    bool has = false;
    if(oTextures)
    for( int i = 0; i < nTextures; i++ )
	{
        int Type = *((unsigned int *)( File + oTextures + i*0x10 ));
        int Flags = *((unsigned int *)( File + oTextures + i*0x10 + 4 ));
        
        switch( Type )
        {
            case 0:
            case 1:
            case 2:
            case 6:
            case 8:
            case 11:
            case 12:
            case 13:
                 break;
            default:
                 printf("\t#%i: %it\n", i, Type);
                 has = true;
        }
            
        if( Flags & 0xFFFFFFFC )
        {    printf("\t#%i: %xf\n", i, Flags); has = true; }
	}
	
	if( has )
		printf( "in %s\n", argv[1] );

	delete File;
}
