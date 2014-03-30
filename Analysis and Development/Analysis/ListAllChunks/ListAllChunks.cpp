#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DESCRIPTION "Counts all main chunks in the file."
#define ARGUMENTS "<File>"

#define USAGE( minimumArguments, argc, argv ) \
	if( argc < minimumArguments + 1 ) \
	{	 \
		printf( "  %s\n", argv[0] ); \
		printf( "    " DESCRIPTION "\n\n" ); \
		printf( "    Usage: \"%s " ARGUMENTS "\"\n", argv[0] ); \
		printf( "    Built at: " __DATE__ "\n" ); \
		return -1; \
	}


FILE *Input;
int FileSize;

bool FindNextBlock()
{
	unsigned int Temp;
	char ID[4];
	fread(ID,1,4,Input);
	printf("%c%c%c%c\t",ID[3],ID[2],ID[1],ID[0]);
	
	fread(&Temp,sizeof(unsigned int),1,Input);
	if(ftell(Input)+Temp>FileSize)
	    return false;
	fseek(Input,Temp,SEEK_CUR);
	if(ftell(Input)<FileSize)
	    return true;
}

int main(int argc, char **argv)
{
	USAGE( 1, argc, argv );
	
	int i = 0;

	Input=fopen(argv[1],"rb+");

	fseek(Input,0,SEEK_END);
	FileSize=ftell(Input);

	while(FindNextBlock())
		i++;
	
	printf("\nTotal chunks: %i", i);

	fclose(Input);
}
