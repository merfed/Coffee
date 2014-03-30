#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <cmath>

#define DESCRIPTION "Scans for a pointer in the file."
#define ARGUMENTS "<File> <Address (in hex)>"

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


int string2int(char* digit) {
	char puffer[80];
	int zahl;

	sprintf(puffer,"%s", digit);
	sscanf(puffer, "%x", &zahl);

	return zahl;
}

int main(int argc, char **argv){
	USAGE( 2, argc, argv );
	 
	Input=fopen(argv[1],"rb+");
    int search = string2int(argv[2]);

	// get filesize
	fseek(Input,0,SEEK_END);
	int filelength = ftell(Input);
	fseek(Input,0,SEEK_SET);

    int ctr = 0;
	unsigned int laststamp = 0;
    fread(&laststamp,sizeof(unsigned int),1,Input);
	do
	{
		if(laststamp == search)
		{
			printf("at 0x%x\t",ftell(Input)-4);
        	ctr++;
		}
        fread(&laststamp,sizeof(unsigned int),1,Input);
	} while (ftell(Input)<filelength);
	
	printf("\n\nfound %i offsets pointing to / containing 0x%x",ctr,search);
}
	
