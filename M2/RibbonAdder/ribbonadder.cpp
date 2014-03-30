#include "ribbonadder.h"




int fillLine(FILE *f)
{
	unsigned int Pos, linePos;
	char blank[16];
	memset(blank,0,16);
	Pos=ftell(f);
	linePos=Pos&0xFFFFFFF0;
	fwrite(blank,1,16-(Pos-linePos),f);
	return 16-(Pos-linePos);
}


int main(int argc, char **argv){
	if(argc<2)
	    return -2;

	FILE *Input;
	Input=fopen(argv[1],"rb+");

	// get filesize
	fseek(Input,0,SEEK_END);
	fillLine(Input);
	fseek(Input,0,SEEK_END);
	int filelength = ftell(Input);
	fseek(Input,0,SEEK_SET);
	// go to ofsAttachment
	int ofsRibbons = filelength;
	int nRibbons = 1;

	printf("How many Ribbons?: ");
		scanf("%d",&nRibbons);

	fseek(Input,0x120,SEEK_SET);
	fwrite(&nRibbons,sizeof(unsigned int),1,Input);
	fseek(Input,0x124,SEEK_SET);
	fwrite(&ofsRibbons,sizeof(unsigned int),1,Input);
	int currentoffset=ofsRibbons;
	printf("%d",nRibbons);

	__int32 reftex[nRibbons];
	__int32 refblend[nRibbons];
	float color[nRibbons][3];
	__int16 opacity[nRibbons];
	float above[nRibbons];
	float below[nRibbons];
	for(int i=0;i<nRibbons;i++){
	ribbon myribbon;
	myribbon.id = -1;

	printf("Color: \n\tr: ");
	scanf("%f",&color[i][0]);
	printf("\tg: ");
	scanf("%f",&color[i][1]);
	printf("\tb: ");
	scanf("%f",&color[i][2]);

	printf("Opacity(Full is 32767): ");
	scanf("%d",&opacity[i]);

	printf("Bone: ");
	scanf("%d",&myribbon.bone);

	printf("Resolution: ");
	scanf("%f",&myribbon.res);

	printf("Length: ");
	scanf("%f",&myribbon.length);

	printf("Emissionangle(sin val): ");
	scanf("%f",&myribbon.gravity);

	printf("Texture(numerous): ");
	scanf("%d",&reftex[i]);

	printf("Textureblendingmode: ");
	scanf("%d",&refblend[i]);

	printf("Above: ");
	scanf("%f",&above[i]);

	printf("Below: ");
	scanf("%f",&below[i]);

	printf("Position: \n\tx: ");
	scanf("%f",&myribbon.pos[0]);
	printf("\ty: ");
	scanf("%f",&myribbon.pos[1]);
	printf("\tz: ");
	scanf("%f",&myribbon.pos[2]);

	printf("Blending Modeflags: ");
	scanf("%d",&myribbon.blending1);

	printf("Blending Mode: ");
	scanf("%d",&myribbon.blending2);

	myribbon.nTextures=1;
	myribbon.ofsTextures=nRibbons*0xB0+ofsRibbons+i*ribbon_ofs_size+refTex;

	myribbon.nUnknown=1;
	myribbon.ofsUnknown=nRibbons*0xB0+ofsRibbons+i*ribbon_ofs_size+refTexBlend;

	myribbon.color.zero1 = 0;
	myribbon.color.minusone = -1;
	myribbon.color.nTime = 1;
	myribbon.color.offsettime = nRibbons*0xB0+ofsRibbons+i*ribbon_ofs_size+color_times;
	myribbon.color.nKey = 1;
	myribbon.color.offsetkey = nRibbons*0xB0+ofsRibbons+i*ribbon_ofs_size+color_keys;

	myribbon.opacity.zero1 = 0;
	myribbon.opacity.minusone = -1;
	myribbon.opacity.nTime = 1;
	myribbon.opacity.offsettime = nRibbons*0xB0+ofsRibbons+i*ribbon_ofs_size+opacity_times;
	myribbon.opacity.nKey = 1;
	myribbon.opacity.offsetkey = nRibbons*0xB0+ofsRibbons+i*ribbon_ofs_size+opacity_keys;

	myribbon.above.zero1 = 0;
	myribbon.above.minusone = -1;
	myribbon.above.nTime = 1;
	myribbon.above.offsettime = nRibbons*0xB0+ofsRibbons+i*ribbon_ofs_size+above_times;
	myribbon.above.nKey = 1;
	myribbon.above.offsetkey = nRibbons*0xB0+ofsRibbons+i*ribbon_ofs_size+above_keys;

	myribbon.below.zero1 = 0;
	myribbon.below.minusone = -1;
	myribbon.below.nTime = 1;
	myribbon.below.offsettime = nRibbons*0xB0+ofsRibbons+i*ribbon_ofs_size+below_times;
	myribbon.below.nKey = 1;
	myribbon.below.offsetkey = nRibbons*0xB0+ofsRibbons+i*ribbon_ofs_size+below_keys;

	myribbon.unk1.zero1 = 0;
	myribbon.unk1.minusone = -1;
	myribbon.unk1.nTime = 1;
	myribbon.unk1.offsettime = nRibbons*0xB0+ofsRibbons+i*ribbon_ofs_size+unk1_times;
	myribbon.unk1.nKey = 1;
	myribbon.unk1.offsetkey = nRibbons*0xB0+ofsRibbons+i*ribbon_ofs_size+unk1_keys;

	myribbon.unk2.zero1 = 0;
	myribbon.unk2.minusone = -1;
	myribbon.unk2.nTime = 1;
	myribbon.unk2.offsettime = nRibbons*0xB0+ofsRibbons+i*ribbon_ofs_size+unk2_times;
	myribbon.unk2.nKey = 1;
	myribbon.unk2.offsetkey = nRibbons*0xB0+ofsRibbons+i*ribbon_ofs_size+unk2_keys;




	fseek(Input,currentoffset,SEEK_SET);
	fwrite(&myribbon,sizeof(ribbon),1,Input);
	currentoffset+=0xB0;

	}


	fseek(Input,currentoffset,SEEK_SET);
	for(int i=0;i<nRibbons;i++){
		int null=0;
		for(int k=0;k<(ribbon_ofs_size/sizeof(int));k++){
			fwrite(&null,sizeof(int),1,Input);
		}

	}
	fseek(Input,currentoffset,SEEK_SET);
	for(int i=0;i<nRibbons;i++){
		animsub t;

		fseek(Input,currentoffset+refTex,SEEK_SET);
		fwrite(&reftex[i],sizeof(__int32),1,Input);

		fseek(Input,currentoffset+refTexBlend,SEEK_SET);
		fwrite(&refblend[i],sizeof(__int32),1,Input);

		fseek(Input,currentoffset+color_times,SEEK_SET);
		t.n=1;
		t.ofs=currentoffset+color_times_v;
		fwrite(&t,sizeof(animsub),1,Input);

		fseek(Input,currentoffset+color_keys,SEEK_SET);
		t.n=1;
		t.ofs=currentoffset+color_keys_v;
		fwrite(&t,sizeof(animsub),1,Input);

		fseek(Input,currentoffset+opacity_times,SEEK_SET);
		t.n=1;
		t.ofs=currentoffset+opacity_times_v;
		fwrite(&t,sizeof(animsub),1,Input);

		fseek(Input,currentoffset+opacity_keys,SEEK_SET);
		t.n=1;
		t.ofs=currentoffset+opacity_keys_v;
		fwrite(&t,sizeof(animsub),1,Input);

		fseek(Input,currentoffset+above_times,SEEK_SET);
		t.n=1;
		t.ofs=currentoffset+above_times_v;
		fwrite(&t,sizeof(animsub),1,Input);

		fseek(Input,currentoffset+above_keys,SEEK_SET);
		t.n=1;
		t.ofs=currentoffset+above_keys_v;
		fwrite(&t,sizeof(animsub),1,Input);

		fseek(Input,currentoffset+below_times,SEEK_SET);
		t.n=1;
		t.ofs=currentoffset+below_times_v;
		fwrite(&t,sizeof(animsub),1,Input);

		fseek(Input,currentoffset+below_keys,SEEK_SET);
		t.n=1;
		t.ofs=currentoffset+below_keys_v;
		fwrite(&t,sizeof(animsub),1,Input);

		fseek(Input,currentoffset+unk1_times,SEEK_SET);
		t.n=1;
		t.ofs=currentoffset+unk1_times_v;
		fwrite(&t,sizeof(animsub),1,Input);

		fseek(Input,currentoffset+unk1_keys,SEEK_SET);
		t.n=1;
		t.ofs=currentoffset+unk1_keys_v;
		fwrite(&t,sizeof(animsub),1,Input);

		fseek(Input,currentoffset+unk2_times,SEEK_SET);
		t.n=1;
		t.ofs=currentoffset+unk2_times_v;
		fwrite(&t,sizeof(animsub),1,Input);

		fseek(Input,currentoffset+unk2_keys,SEEK_SET);
		t.n=1;
		t.ofs=currentoffset+unk2_keys_v;
		fwrite(&t,sizeof(animsub),1,Input);


		fseek(Input,currentoffset+color_keys_v,SEEK_SET);
		for(int k=0;k<3;k++){
			fwrite(&color[i][k],sizeof(float),1,Input);
		}

		fseek(Input,currentoffset+opacity_keys_v,SEEK_SET);
		fwrite(&opacity[i],sizeof(__int16),1,Input);


		fseek(Input,currentoffset+above_keys_v,SEEK_SET);
		fwrite(&above[i],sizeof(float),1,Input);

		fseek(Input,currentoffset+below_keys_v,SEEK_SET);
		fwrite(&below[i],sizeof(float),1,Input);

		fseek(Input,currentoffset+unk2_keys_v,SEEK_SET);
		int unkint2=1;
		fwrite(&unkint2,sizeof(__int32),1,Input);

		currentoffset+=ribbon_ofs_size;
	}


	fseek(Input,0,SEEK_END);
	filelength = ftell(Input);
	fseek(Input,0,SEEK_SET);


	printf("\n\tcake time now\n\n");
//	system("pause");

	fclose(Input);
	return 0;
}
