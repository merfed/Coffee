#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//MapMover <OldMapName> <LowerX> <LowerY> <UpperX> <UpperY> <NewMapName> <OffsetX> <OffsetY>
int main(int argc, char **argv)
{
	int x,y;
	int LowerX,LowerY,UpperX,UpperY,OffsetX,OffsetY;
	char temp[512];

	FILE *fid;
	unsigned int Writing;

	LowerX=atoi(argv[2]);
	LowerY=atoi(argv[3]);
	UpperX=atoi(argv[4]);
	UpperY=atoi(argv[5]);
	OffsetX=atoi(argv[7]);
	OffsetY=atoi(argv[8]);


	for (x=LowerX;x<=UpperX;x++)
	{
		for (y=LowerY;y<=UpperY;y++)
		{
			sprintf(temp,"cp %s_%d_%d.adt %s_%d_%d.adt",argv[1],x,y,argv[6],x+OffsetX,y+OffsetY);
			system(temp);
		}
	}
	return 0;
}
