/*
 * main.cpp
 *
 *For testing purpose only...
 *
 *  Created on: 28.06.2009
 *      Author: Bastian
 */

#include <iostream>
#include "libm2/modeleditor.h"
using namespace std;

FILE *Input,*Output;
char	*f,*o;
int FileSize,OutSize;

int main(int argc, char **argv) {
	_TF("M2Class.log");
	_Tn("main");
	printf("ParticleCloner 0.1\n");
	printf("Build: July, 19th,2009\n");
	printf("By Tigurius\n");
	if(argc<2){
		printf("Usage: ParticleCloner <Model1> <Model2>\n");
		return 0;
	}
	printf(" Loading Model %s\n",argv[1]);
	printf(" Loading Model %s\n",argv[2]);
	Input=fopen(argv[1],"rb+");
	Output=fopen(argv[2],"rb+");
	if(Input==NULL||Output==NULL)
	{
		printf(" ERROR: Could not load file");
		return 0;
	}

	fseek(Input,0,SEEK_END);
	FileSize=ftell(Input);
	fseek(Input,0,SEEK_SET);

	f=new char[FileSize];

	fread(f,FileSize,1,Input);
	fclose(Input);

	fseek(Output,0,SEEK_END);
	OutSize=ftell(Output);
	fseek(Output,0,SEEK_SET);

	o=new char[OutSize];

	fread(o,OutSize,1,Output);
	fclose(Output);

	printf("Creating ModelEditor 1\n");
	ModelEditor e=ModelEditor(f,FileSize);
	TextureHelper tex=e.getTextures();
	ParticleHelper p=e.getParticles();

	printf("Creating ModelEditor 2\n");
	ModelEditor k=ModelEditor(o,OutSize);
	TextureHelper tex2=k.getTextures();


	int*texes=new int[1];
	int ntex=1;
	texes[0]=p.particles[0].texture;
	int*newtexes=new int[1];
	for(int i=0;i<p.nParticles;i++){
		bool notinlist=true;
		for(int k=0;k<ntex;k++){
			if(texes[k]==p.particles[i].texture){
				notinlist=false;
			}
		}
		if(notinlist==true){
			int *t=texes;
			texes=new int[ntex+1];
			for(int k=0;k<ntex;k++){
				texes[k]=t[k];
			}
			texes[ntex]=p.particles[i].texture;
			ntex++;
		}
	}

	newtexes=new int[ntex];
	for(int i=0;i<ntex;i++){
		int newtex=tex2.nTextures;
		printf("texture: %d\n",texes[i]);
		TextureHelper temp=tex2;
		tex2.setnTextures(newtex+1);
		for(int k=0;k<newtex;k++){
			tex2.setTexture(temp.texdefs[k],k);
			tex2.Texnames[k]=temp.Texnames[k];
		}
		tex2.setTexture(tex.texdefs[texes[i]],newtex);
		tex2.Texnames[newtex]=tex.Texnames[texes[i]];
		printf("%s\n",tex2.Texnames[newtex]);
		newtexes[i]=newtex;

	}
	for(int i=0;i<p.nParticles;i++){
		for(int k=0;k<ntex;k++){
			if(p.particles[i].texture==texes[k]){
				p.particles[i].texture=newtexes[k];
				break;
			}
		}
	}
	k.newTextures(tex2);
	k.newTexLookup();
	k.newParticles(p);

	o=k.getFile();
	OutSize=k.GetFileSize();
	printf("Saving Changes\n");
	Output=fopen(argv[2],"wb");
	fwrite(o,OutSize,1,Output);
	fclose(Output);
	return 0;
}
