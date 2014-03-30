
#include "modelheaders.h"
#include "lib3ds\mesh.h"
#include "lib3ds\file.h"

ModelHeader *header;
ModelVertex	*vertices;
ModelVertex *dVertices;//==dummyvertex
int			nVertices;
int		FileSize;
char	*m2;

void write3ds(Lib3dsFile *f)
{
	int i,meshV,meshNum;
	Lib3dsMesh *m;

	nVertices=0;
	int nSubmeshes=0;


	printf("Loading 3ds File\n");

	for (m=f->meshes; m; m=m->next)
	{
		nVertices+=m->points;
		nSubmeshes++;
	}

	//Load initial vertex & triangle data
	meshV=0;
	meshNum=0;
	for (m=f->meshes; m; m=m->next)
	{
		printf("Processing Mesh %d\n",meshNum);
		meshNum++;
		for(i=0;i<m->points;i++)
		{
			m->texelL[i][0]=vertices[i+meshV].texcoords[0];
			m->texelL[i][1]=1-vertices[i+meshV].texcoords[1];
		}
		meshV+=m->points;
	}

}

void load3ds(Lib3dsFile *f)
{
	int i,meshV,meshNum;
	Lib3dsMesh *m;
	nVertices=0;
	int nSubmeshes=0;

	printf("Loading 3ds File\n");

	for (m=f->meshes; m; m=m->next)
	{
		nVertices+=m->points;
		nSubmeshes++;
	}
	printf("Vertices: %d\n",nVertices);
	dVertices=new ModelVertex[nVertices];

	//Load initial vertex & triangle data
	meshV=0;
	meshNum=0;
	for (m=f->meshes; m; m=m->next)
	{
		printf("Processing Mesh %d\n",meshNum);
		meshNum++;
		for(i=0;i<m->points;i++)
		{
			dVertices[i+meshV].texcoords[0]=m->texelL[i][0];
			dVertices[i+meshV].texcoords[0]=m->texelL[i][1];
			dVertices[i+meshV].texcoords[1]=1-dVertices[i+meshV].texcoords[1];
		}
		meshV+=m->points;
	}

}



int main(int argc, char **argv)
{
	printf("3ds To M2 TexMover");
	Lib3dsFile *f=0;
	FILE	*M2;
	bool versa=false;
	if(argc<3)
	{
		printf(" 3dsToM2Texmover <M2 File Name> <3ds File Name> [Optional:M2to3ds: r]\n");
		return 0;
	}
	if(argc>3)versa=true;

	f=lib3ds_file_load(argv[2]);

	if(!f)
	{
		printf("ERROR: Unable to load 3ds file %s\n",argv[1]);
		return 0;
	}

	M2=fopen(argv[1],"wb");
	if(M2==NULL)
	{
		printf("ERROR: Unable to open M2 file for writing %s\n",argv[2]);
		lib3ds_file_free(f);
		return 0;
	}
	fseek(M2,0,SEEK_END);
	FileSize=ftell(M2);
	fseek(M2,0,SEEK_SET);

	m2=new char[FileSize];

	fread(m2,FileSize,1,M2);
	fclose(M2);

	header=(ModelHeader *)(m2);
	vertices=(ModelVertex *)(m2+header->ofsVertices);
	printf("M2: nVertices: %d",header->nVertices);
	if(!versa){
	load3ds(f);
	for(int i=0;i<header->nVertices;i++){
		vertices[i].texcoords[0]=dVertices[i].texcoords[0];
		vertices[i].texcoords[1]=dVertices[i].texcoords[1];
	}
	}
	else{
		write3ds(f);
	}


	lib3ds_file_free(f);
	if(!versa){
	printf("Saving Changes\n");
	M2=fopen(argv[1],"wb");
	fwrite(m2,FileSize,1,M2);
	fclose(M2);
	}

	printf("3dstoM2 UV-convertion completed\n");
}
