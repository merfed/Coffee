/*
 * vertexmover.cpp
 *
 *  Created on: 25.05.2009
 *      Author: Bastian
 */

#include "modelheaders.h"
FILE *Input,*Readin;

char	*f,*r;
int		FileSize,ISize;
ModelHeader *header_m;
ModelHeader *header_i;

ModelVertex *vertex_m;
ModelVertex *vertex_i;
bool rescale;

int main(int argc, char **argv){
	printf("VertexMover by Tigurius\n");
	printf(" Loading Model %s\n",argv[1]);
	Input=fopen(argv[1],"rb+");
	printf(" Loading Model %s\n",argv[2]);
	Readin=fopen(argv[2],"rb+");
	if(Input==NULL||Readin==NULL)
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


	fseek(Readin,0,SEEK_END);
	ISize=ftell(Readin);
	fseek(Readin,0,SEEK_SET);

	r=new char[ISize];

	fread(r,ISize,1,Readin);
	fclose(Readin);

	header_m=(ModelHeader *)(f);
	vertex_m=(ModelVertex *)(f+header_m->ofsVertices);


	header_i=(ModelHeader *)(r);
	vertex_i=(ModelVertex *)(r+header_i->ofsVertices);

	for(int i=0;i<header_m->nVertices;i++){
		vertex_m[i].texcoords[0]=vertex_i[i].texcoords[0];
		vertex_m[i].texcoords[1]=vertex_i[i].texcoords[1];
	}
	printf("Saving Changes\n");
	Input=fopen(argv[1],"wb");
	fwrite(f,FileSize,1,Input);
	fclose(Input);
	return 0;
}
