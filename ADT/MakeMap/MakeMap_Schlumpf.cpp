#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DESCRIPTION "Combines multiple .raw maps made by Noggit into one big one."
#define ARGUMENTS "<map name> <xOff> <yOff> <tWidth> <tHeight>"

#define USAGE( minimumArguments, argc, argv ) \
	if( argc < minimumArguments + 1 ) \
	{	 \
		printf( "  %s\n", argv[0] ); \
		printf( "    " DESCRIPTION "\n\n" ); \
		printf( "    Usage: \"%s " ARGUMENTS "\"\n", argv[0] ); \
		printf( "    Built at: " __DATE__ "\n" ); \
		return -1; \
	}
	
int Width,Height;
int tWidth,tHeight;

char *image;


void ReadImage(char *fname,int x,int y)
{
	char timage[256*256*3];
	FILE *input;
	printf("Reading %s\n",fname);
	input=fopen(fname,"rb");
	if(input!=0)
	{
		fread(timage,256*3,256,input);
		fclose(input);
		for(int i=0;i<256;i++)
			memcpy(image+x*256*3+i*Width+y*256*Width,timage+256*3*(255-i),256*3);
	}
	else
	{
		for(int i=0;i<256;i++)
			memset(image+x*256*3+i*Width+y*256*Width,0,256*3);
	}
}


int main(int argc,char **argv)
{
	USAGE( 5, argc, argv );
	
	int xOff,yOff;
	char tname[256];
	if(argc<6)
		return 0;
	xOff=atoi(argv[2]);
	yOff=atoi(argv[3]);
	tWidth=atoi(argv[4]);
	tHeight=atoi(argv[5]);


	Width=tWidth*256*3;

	image=new char[256*256*3*tWidth*tHeight];

	for(int y=0;y<tHeight;y++)
		for(int x=0;x<tWidth;x++)
		{
			sprintf(tname,"%s_map_%d_%d.raw",argv[1],x+xOff,y+yOff);
			ReadImage(tname,x,y);
		}
	
	sprintf(tname,"%s_map.raw",argv[1]);
	FILE *Output;
	Output=fopen(tname,"wb");
	fwrite(image,256*256*3,tWidth*tHeight,Output);
	fclose(Output);
	return 0;
}
