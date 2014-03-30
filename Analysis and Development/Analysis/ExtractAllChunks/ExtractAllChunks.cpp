#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DESCRIPTION "Extracts all chunks of the file into seperate files."
#define ARGUMENTS "<chunked file>"

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
int chunknum;

bool FindNextBlock()
{
	unsigned int Temp;
	char ID[4];
	fread(ID,1,4,Input);
	printf("%c%c%c%c\t",ID[3],ID[2],ID[1],ID[0]);
	
	if(ftell(Input)==FileSize)
	    return false;
	    
	fread(&Temp,sizeof(unsigned int),1,Input);
	if(ftell(Input)+Temp>FileSize)
	    return false;
	    
	    // SAVE
	    
	char filename[10];
	char temp_c[Temp+1];
  	FILE* temp;
  	if(chunknum<10)
  		sprintf(filename,"00%i_%c%c%c%c", chunknum,ID[3],ID[2],ID[1],ID[0]);
  		else if(chunknum<100)
  			sprintf(filename,"0%i_%c%c%c%c", chunknum,ID[3],ID[2],ID[1],ID[0]);
  			else
				sprintf(filename,"%i_%c%c%c%c", chunknum,ID[3],ID[2],ID[1],ID[0]);
	temp = fopen(filename,"w");
	
	fread(temp_c,1,Temp,Input);
	fwrite(ID,4,1,temp);
	fwrite(&Temp,1,sizeof(unsigned int),temp);
	fwrite(temp_c,Temp,1,temp);

	fclose(temp);
	
	    // SAVE
	
	//fseek(Input,Temp,SEEK_CUR);
	if(ftell(Input)<FileSize)
	    return true;
}

int main(int argc, char **argv)
{
	USAGE( 1, argc, argv );
	
	chunknum = 1;

	Input=fopen(argv[1],"rb+");

	fseek(Input,0,SEEK_END);
	FileSize=ftell(Input);
	fseek(Input,0,SEEK_SET);

	while(FindNextBlock())
		chunknum++;
	
	printf("\nTotal chunks: %i", chunknum);

	fclose(Input);
}
