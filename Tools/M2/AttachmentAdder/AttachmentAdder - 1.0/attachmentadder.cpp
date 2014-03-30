#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <stdlib.h>


struct animfake
{
	__int16 zero1;
	__int16 minusone;
	unsigned __int32 one1;
	unsigned __int32 offsettovoid1;
	unsigned __int32 one2;
	unsigned __int32 offsettovoid2;
};

struct attachment
{
__int32 ID;     		// ID From ID Table
__int32	Bone;		// The bone
float	pos[3];	    // Position
animfake fake1;       // AnimFake
};


int main(int argc, char **argv){
	if(argc<2)
	    return -2;

	FILE *Input;
	Input=fopen(argv[1],"rb+");

	// get filesize
	fseek(Input,0,SEEK_END);
	int filelength = ftell(Input);
	fseek(Input,0,SEEK_SET);

	// go to ofsAttachment
	int ofsAttachments = filelength;
	int nAttachments = 1;
	printf("How many Attachments?: ");
		scanf("%d",&nAttachments);

	fseek(Input,0xF0,SEEK_SET);
	fwrite(&nAttachments,sizeof(unsigned int),1,Input);
	fseek(Input,0xF4,SEEK_SET);
	fwrite(&ofsAttachments,sizeof(unsigned int),1,Input);
	int currentoffset=ofsAttachments;
	printf("%d",nAttachments);
	for(int i=0;i<nAttachments;i++){
	attachment myattach;
	myattach.ID = -1;

	printf("Type(0 for mountpoints): ");
	scanf("%f",&myattach.ID);

	printf("Bone: ");
	scanf("%f",&myattach.Bone);

	printf("Position: \n\tx: ");
	scanf("%f",&myattach.pos[0]);
	printf("\ty: ");
	scanf("%f",&myattach.pos[1]);
	printf("\tz: ");
	scanf("%f",&myattach.pos[2]);

/*	short zero1;
	short minusone;
	int zero2;
	int zero3;
	int one1;
	int offsettovoid1;
	int one2;
	int offsettovoid2;*/

	myattach.fake1.zero1 = 0;
	myattach.fake1.minusone = -1;
	myattach.fake1.one1 = 0;
	myattach.fake1.offsettovoid1 = 0;
	myattach.fake1.one2 = 0;
	myattach.fake1.offsettovoid2 = 0;


	fseek(Input,currentoffset,SEEK_SET);
	fwrite(&myattach,sizeof(attachment),1,Input);
	currentoffset+=0x28;
	}
	int zero = 0;
	for(int i = 0; i < 6; i++)
	{
		fwrite(&zero,sizeof(int),1,Input);
	}

	fseek(Input,0,SEEK_END);
	filelength = ftell(Input);
	fseek(Input,0,SEEK_SET);

	int ofsAttachLookup = filelength;
	int nAttachLookup = nAttachments;

	fseek(Input,0xF8,SEEK_SET);
	fwrite(&nAttachLookup,sizeof(unsigned int),1,Input);
	fseek(Input,0xFC,SEEK_SET);
	fwrite(&ofsAttachLookup,sizeof(unsigned int),1,Input);

	fseek(Input,ofsAttachLookup,SEEK_SET);
	for(int i=0;i<nAttachLookup;i++){
	short temp = i;
	fwrite(&temp,sizeof(short),1,Input);
	}

	printf("\n\tcake time now\n\n");
//	system("pause");

	fclose(Input);
	return 0;
}
