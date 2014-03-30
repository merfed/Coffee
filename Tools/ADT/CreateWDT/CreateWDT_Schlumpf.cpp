#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DESCRIPTION "Makes a WDT file telling that the specified ADT tiles are present."
#define ARGUMENTS "<MapName> <LowerX> <LowerY> <UpperX> <UpperY>"

#define USAGE( minimumArguments, argc, argv ) \
	if( argc < minimumArguments + 1 ) \
	{	 \
		printf( "  %s\n", argv[0] ); \
		printf( "    " DESCRIPTION "\n\n" ); \
		printf( "    Usage: \"%s " ARGUMENTS "\"\n", argv[0] ); \
		printf( "    Built at: " __DATE__ "\n" ); \
		return -1; \
	}

int main(int argc, char **argv)
{
	USAGE( 5, argc, argv );
	
	int x,y;
	int LowerX,LowerY,UpperX,UpperY,OffsetX,OffsetY;
	char temp[512];

	FILE *fid;
	unsigned int Writing;

	LowerX=atoi(argv[2]);
	LowerY=atoi(argv[3]);
	UpperX=atoi(argv[4]);
	UpperY=atoi(argv[5]);

//	printf("Range (%d,%d) to (%d,%d)\n",LowerX,LowerY,UpperX,UpperY);
	
	sprintf(temp,"%s.wdt",argv[1]);
	fid=fopen(temp,"wb");
	Writing=0x4d564552;//MVER
	fwrite(&Writing,4,1,fid);
	Writing=0x04;
	fwrite(&Writing,4,1,fid);
	Writing=0x12;
	fwrite(&Writing,4,1,fid);
	
	Writing=0x4d504844;//MPHD
	fwrite(&Writing,4,1,fid);
	Writing=0x20;
	fwrite(&Writing,4,1,fid);
	Writing=0x0;
	for(int i=0;i<8;i++)
		fwrite(&Writing,4,1,fid);

	Writing=0x4d41494e;//MAIN
	fwrite(&Writing,4,1,fid);
	Writing=0x8000;
	fwrite(&Writing,4,1,fid);

	Writing=0;
	for(int i=0;i<4096;i++)
	{
		x=(i%64);
		y=i/64;
		if ((x>=LowerX) && (x<=UpperX) && (y>=LowerY) && (y<=UpperY))
		{
//			printf("(%d,%d)\n",x,y);
			Writing=1;
		}
		fwrite(&Writing,4,1,fid);
		Writing=0;
		fwrite(&Writing,4,1,fid);
	}

	Writing=0x4d574d4f; //MWMO
	fclose(fid);
	return 0;
}
