#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

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
	
	int nAnimations = *((int *)( File + 0x1C ));
	int oAnimations = *((int *)( File + 0x20 ));

    bool has = false;
    if(oAnimations)
    for( int i = 0; i < nAnimations; i++ )
	{
        short AnimationID = *((unsigned short *)( File + oAnimations + i*0x40 ));
        int Flags = *((unsigned int *)( File + oAnimations + i*0x40 + 0xC ));
        int Flags2 = *((unsigned int *)( File + oAnimations + i*0x40 + 0x10 ));
        int Unknown1 = *((unsigned int *)( File + oAnimations + i*0x40 + 0x14 ));
        int Unknown2 = *((unsigned int *)( File + oAnimations + i*0x40 + 0x18 ));
            
        if( ! (Flags & 0xFFFFFFFE) )
        {    printf("\t#%i (%i): 0x%x", i, AnimationID, Flags); has = true;
    //printflags(Flags & 0xFFFFFFFF);
    printf("\n"); }
	}
	
	if( has )
		printf( "in %s\n", argv[1] );

	delete File;
}
