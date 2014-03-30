//============================================================================
// Name        : copygeoset.cpp
// Author      : Tigurius
// Version     :
// Copyright   : GPL
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "modelheaders.h"
#include "skin.h"
using namespace std;

FILE *Input,*Readin,*Skin,*InSkin;

char	*f,*r,*s,*j;
int		FileSize,ISize,SSize,FSkin,NewSize,NewSkinSize;
ModelHeader *header_m;
ModelHeader *header_i;
SkinHeader *header_s;
SkinHeader *header_n;
ModelVertex *vertex_m;
ModelVertex *vertex_i;
ModelVertex *vertex_new;
Submesh *submesh_s;
Submesh *submesh_n;
Triangle *tri_s;
Triangle *tri_n;
short *vert_s;
short *vert_n;
VProp *prop_n;
TexUnit *tex;
short *texlookup;

void DisplaySubmeshInfo(){
	for(int i=0;i<header_s->nSubmeshes;i++){
		printf("Submesh: %d\n",i);
		printf("nVertex:\t%d\n",submesh_s[i].nVertices);
		printf("StartVertex:\t%d\n",submesh_s[i].StartVertex);
	}
}

int getNearestVertex(ModelVertex v){
	float distance=999;
	int returnv=0;
	for(int i=0;i<header_m->nVertices;i++){
		float newdistance=0;
		if((v.pos[0]-vertex_m[i].pos[0])<0){
			newdistance+=(-1)*(v.pos[0]-vertex_m[i].pos[0]);
		}
		else
			newdistance+=(v.pos[0]-vertex_m[i].pos[0]);

		if((v.pos[1]-vertex_m[i].pos[1])<0){
			newdistance+=(-1)*(v.pos[1]-vertex_m[i].pos[1]);
		}
		else
			newdistance+=(v.pos[1]-vertex_m[i].pos[1]);

		if((v.pos[2]-vertex_m[i].pos[2])<0){
			newdistance+=(-1)*(v.pos[2]-vertex_m[i].pos[2]);
		}
		else
			newdistance+=(v.pos[2]-vertex_m[i].pos[2]);

		if(newdistance<distance){
			distance=newdistance;
			returnv=i;
		}
	}
	return returnv;
}

void CopySubmesh(){
	printf("Resize?(1/0)\t");
	float factor=1;
	int k=0;
	scanf("%d",&k);
	if(k==1){
		printf("Insert Factor:\t");
		scanf("%f",&factor);
	}




	printf("Which Submesh(0-%d)?\t",header_s->nSubmeshes-1);
	int n;
	scanf("%d",&n);

	k=0;
	int abone=0;
	int nbone=1;
	bool mbone=false;
	uint8 boneindices[submesh_s[n].nVertices][4];
	//uint8 vprops[submesh_s[n].nVertices][4];
	uint8 boneweights[submesh_s[n].nVertices][4];
	printf("\nAttach to Skeleton?( 0 = no{geoset will be attached to rootbone}\n 1 = attach to bone\n 2 = attach to model{unstable})\t");
	scanf("%d",&k);
	if(k==0){
		for(int i=submesh_s[n].StartVertex;i<submesh_s[n].StartVertex+submesh_s[n].nVertices;i++){
			boneindices[i-submesh_s[n].StartVertex][0]=0;
			boneindices[i-submesh_s[n].StartVertex][1]=0;
			boneindices[i-submesh_s[n].StartVertex][2]=0;
			boneindices[i-submesh_s[n].StartVertex][3]=0;

			boneweights[i-submesh_s[n].StartVertex][0]=-1;
			boneweights[i-submesh_s[n].StartVertex][1]=0;
			boneweights[i-submesh_s[n].StartVertex][2]=0;
			boneweights[i-submesh_s[n].StartVertex][3]=0;
		}
	}
	else if(k==1){
		printf("Attach to which bone?\t");
		scanf("%d",&abone);
		for(int i=submesh_s[n].StartVertex;i<submesh_s[n].StartVertex+submesh_s[n].nVertices;i++){
			boneindices[i-submesh_s[n].StartVertex][0]=abone;
			boneindices[i-submesh_s[n].StartVertex][1]=0;
			boneindices[i-submesh_s[n].StartVertex][2]=0;
			boneindices[i-submesh_s[n].StartVertex][3]=0;

			boneweights[i-submesh_s[n].StartVertex][0]=-1;
			boneweights[i-submesh_s[n].StartVertex][1]=0;
			boneweights[i-submesh_s[n].StartVertex][2]=0;
			boneweights[i-submesh_s[n].StartVertex][3]=0;
		}
	}
	else if(k==2){
		mbone=true;
		for(int i=submesh_s[n].StartVertex;i<submesh_s[n].StartVertex+submesh_s[n].nVertices;i++){
			int v=getNearestVertex(vertex_i[i]);
			boneindices[i-submesh_s[n].StartVertex][0]=vertex_m[v].bones[0];
			boneindices[i-submesh_s[n].StartVertex][1]=vertex_m[v].bones[1];
			boneindices[i-submesh_s[n].StartVertex][2]=vertex_m[v].bones[2];
			boneindices[i-submesh_s[n].StartVertex][3]=vertex_m[v].bones[3];


			boneweights[i-submesh_s[n].StartVertex][0]=vertex_m[v].weights[0];
			boneweights[i-submesh_s[n].StartVertex][1]=vertex_m[v].weights[1];
			boneweights[i-submesh_s[n].StartVertex][2]=vertex_m[v].weights[2];
			boneweights[i-submesh_s[n].StartVertex][3]=vertex_m[v].weights[3];

		}
	}

	int texunitnumber=header_m->nTexUnitLookup;
	NewSize=FileSize+(header_m->nVertices+submesh_s[n].nVertices)*sizeof(ModelVertex)+(header_m->nTexUnitLookup+1)*sizeof(short);
	char *copy=new char[NewSize];
	ModelVertex *tmp=(ModelVertex *)(copy+FileSize);
	short *newtexlook=(short *)(copy+FileSize+(header_m->nVertices+submesh_s[n].nVertices)*sizeof(ModelVertex));
	for(int i=0;i<header_m->nTexUnitLookup;i++){
		newtexlook[i]=texlookup[i];
	}
	newtexlook[header_m->nTexUnitLookup]=-1;
	for(int i=0;i<header_m->nVertices;i++){
		tmp[i]=vertex_m[i];
	}
	for(int i=0;i<submesh_s[n].nVertices;i++){
		//readin vertices and rescales them and if attaching to
		//model attach
		tmp[i+header_m->nVertices]=vertex_i[vert_s[submesh_s[n].StartVertex+i]];
		tmp[i+header_m->nVertices].pos[0]*=factor;
		tmp[i+header_m->nVertices].pos[1]*=factor;
		tmp[i+header_m->nVertices].pos[2]*=factor;
		tmp[i+header_m->nVertices].bones[0]=abone;

		tmp[i+header_m->nVertices].bones[0]=boneindices[i][0];
		tmp[i+header_m->nVertices].bones[1]=boneindices[i][1];
		tmp[i+header_m->nVertices].bones[2]=boneindices[i][2];
		tmp[i+header_m->nVertices].bones[3]=boneindices[i][3];

		tmp[i+header_m->nVertices].weights[0]=boneweights[i][0];
		tmp[i+header_m->nVertices].weights[1]=boneweights[i][1];
		tmp[i+header_m->nVertices].weights[2]=boneweights[i][2];
		tmp[i+header_m->nVertices].weights[3]=boneweights[i][3];


	}
	char *temp=f;
	f=new char[NewSize];
	header_m=(ModelHeader *)f;
	//this writes the additional data into m2 and changes the offsets etc.
	memcpy(f,temp,FileSize);
	memcpy(f+FileSize,tmp,(header_m->nVertices+submesh_s[n].nVertices)*sizeof(ModelVertex));
	memcpy(f+FileSize+(header_m->nVertices+submesh_s[n].nVertices)*sizeof(ModelVertex),newtexlook,(header_m->nTexUnitLookup+1)*sizeof(short));
	printf("Copyied Vertices!\n");
	header_m->nVertices=header_m->nVertices+submesh_s[n].nVertices;
	header_m->ofsVertices=FileSize;
	vertex_m=(ModelVertex *)(f+header_m->ofsVertices);
	header_m->nTexUnitLookup=header_m->nTexUnitLookup+1;
	header_m->ofsTexUnitLookup=FileSize+(header_m->nVertices)*sizeof(ModelVertex);

	//now the new *.skin file <.<
	printf("Creating new skin...\n");
	NewSkinSize=FSkin+(submesh_s[n].nVertices*sizeof(short))+
	(header_n->nTriangles+submesh_s[n].nTriangles)/3*sizeof(Triangle)+
	sizeof(Submesh)+sizeof(TexUnit)+
	(submesh_s[n].nVertices*sizeof(uint32));

	char *newskin=new char[NewSkinSize];

	SkinHeader * oldh=header_n;

	//the following parts just rebuild the *.skin
	printf("Writing new header...\n");
	memcpy(newskin+0,(j+0),sizeof(SkinHeader));
	header_n=(SkinHeader *)newskin;

	printf("Writing new Vertices...\n");
	memcpy((newskin+sizeof(SkinHeader)),(j+oldh->ofsIndices),sizeof(short)*oldh->nIndices);
	vert_n=(short *)(newskin+sizeof(SkinHeader));
	header_n->ofsIndices=sizeof(SkinHeader);

	printf("Writing new Properties...\n");
	memcpy(newskin+sizeof(SkinHeader)+(header_n->nIndices+submesh_s[n].nVertices)*sizeof(short),(j+oldh->ofsProperties),oldh->nProperties*sizeof(int));
	prop_n=(VProp *)(newskin+sizeof(SkinHeader)+(oldh->nIndices+submesh_s[n].nVertices)*sizeof(short));
	header_n->ofsProperties=sizeof(SkinHeader)+(oldh->nIndices+submesh_s[n].nVertices)*sizeof(short);

	printf("Writing new Triangles...\n");
	memcpy(newskin+sizeof(SkinHeader)+
			(oldh->nIndices+submesh_s[n].nVertices)*sizeof(short)+
			(oldh->nProperties+submesh_s[n].nVertices)*sizeof(int),(j+oldh->ofsTriangles),(oldh->nTriangles)/3*sizeof(Triangle));
	tri_n=(Triangle *)(newskin+sizeof(SkinHeader)+
			(oldh->nIndices+submesh_s[n].nVertices)*sizeof(short)+
			(oldh->nProperties+submesh_s[n].nVertices)*sizeof(int));
	header_n->ofsTriangles=sizeof(SkinHeader)+
	(oldh->nIndices+submesh_s[n].nVertices)*sizeof(short)+
	(oldh->nProperties+submesh_s[n].nVertices)*sizeof(int);

	printf("Writing new Submeshes...\n");
	memcpy(newskin+sizeof(SkinHeader)+
			(oldh->nIndices+submesh_s[n].nVertices)*sizeof(short)+
			(oldh->nProperties+submesh_s[n].nVertices)*sizeof(int)+
			((oldh->nTriangles+submesh_s[n].nTriangles)/3)*sizeof(Triangle),
			(j+oldh->ofsSubmeshes),oldh->nSubmeshes*sizeof(Submesh));
	submesh_n=(Submesh *)(newskin+sizeof(SkinHeader)+
			(oldh->nIndices+submesh_s[n].nVertices)*sizeof(short)+
			(oldh->nProperties+submesh_s[n].nVertices)*sizeof(int)+
			(oldh->nTriangles+submesh_s[n].nTriangles)/3*sizeof(Triangle));
	header_n->ofsSubmeshes=sizeof(SkinHeader)+
	(oldh->nIndices+submesh_s[n].nVertices)*sizeof(short)+
	(oldh->nProperties+submesh_s[n].nVertices)*sizeof(int)+
	(oldh->nTriangles+submesh_s[n].nTriangles)/3*sizeof(Triangle);

	printf("Writing new TexUnits...\n");
	memcpy(newskin+sizeof(SkinHeader)+
			(oldh->nIndices+submesh_s[n].nVertices)*sizeof(short)+
			(oldh->nProperties+submesh_s[n].nVertices)*sizeof(int)+
			(oldh->nTriangles+submesh_s[n].nTriangles)/3*sizeof(Triangle)+
			(oldh->nSubmeshes+1)*sizeof(Submesh),
			(j+oldh->ofsTextureUnits),sizeof(TexUnit)*oldh->nTextureUnits);
	tex=(TexUnit *)(newskin+sizeof(SkinHeader)+
			(oldh->nIndices+submesh_s[n].nVertices)*sizeof(short)+
			(oldh->nProperties+submesh_s[n].nVertices)*sizeof(int)+
			(oldh->nTriangles+submesh_s[n].nTriangles)/3*sizeof(Triangle)+
			(oldh->nSubmeshes+1)*sizeof(Submesh));
	header_n->ofsTextureUnits=sizeof(SkinHeader)+
	(oldh->nIndices+submesh_s[n].nVertices)*sizeof(short)+
	(oldh->nProperties+submesh_s[n].nVertices)*sizeof(int)+
	(oldh->nTriangles+submesh_s[n].nTriangles)/3*sizeof(Triangle)+
	(oldh->nSubmeshes+1)*sizeof(Submesh);

	printf("Create new Indices...\n");
	for(int i=oldh->nIndices;i<oldh->nIndices+submesh_s[n].nVertices;i++){
		//creating the indices
		vert_n[i]=i;
	}
	header_n->nIndices=oldh->nIndices+submesh_s[n].nVertices;

	printf("Copying new Triangles...(n=%d)\n",(header_n->nTriangles+submesh_s[n].nTriangles)/3);
	for(int i=(oldh->nTriangles/3);i<(oldh->nTriangles+submesh_s[n].nTriangles)/3;i++){
		//this calculates the triangles new to fit into the new model
		int triangle=(i-(oldh->nTriangles/3))+submesh_s[n].StartTriangle/3;
		tri_n[i]=tri_s[triangle];
		tri_n[i].Indices[0]-=submesh_s[n].StartVertex;
		tri_n[i].Indices[1]-=submesh_s[n].StartVertex;
		tri_n[i].Indices[2]-=submesh_s[n].StartVertex;
		tri_n[i].Indices[0]+=oldh->nIndices;
		tri_n[i].Indices[1]+=oldh->nIndices;
		tri_n[i].Indices[2]+=oldh->nIndices;
	}
	header_n->nTriangles=oldh->nTriangles+submesh_s[n].nTriangles;

	printf("Create new Properties...\n");
	for(int i=oldh->nIndices;i<oldh->nIndices+submesh_s[n].nVertices;i++){
		prop_n[i].Properties[0]=abone;
		if(mbone){
			int v=getNearestVertex(vertex_i[i]);
			printf("Nearest V: %d\n",v);
			prop_n[i].Properties[0]=prop_n[v].Properties[0];
			prop_n[i].Properties[1]=prop_n[v].Properties[1];
			prop_n[i].Properties[2]=prop_n[v].Properties[2];
			prop_n[i].Properties[3]=prop_n[v].Properties[3];
			printf("%d/n",prop_n[i].Properties[0]);
		}
	}
	header_n->nProperties=oldh->nProperties+submesh_s[n].nVertices;

	printf("Creating Submesh...\n");
	submesh_n[oldh->nSubmeshes].StartTriangle=oldh->nTriangles;
	submesh_n[oldh->nSubmeshes].StartVertex=oldh->nIndices;
	submesh_n[oldh->nSubmeshes].nTriangles=submesh_s[n].nTriangles;
	submesh_n[oldh->nSubmeshes].nVertices=submesh_s[n].nVertices;
	submesh_n[oldh->nSubmeshes].ID=0; //0=StandardID
	submesh_n[oldh->nSubmeshes].Position[0]=submesh_s[n].Position[0];
	submesh_n[oldh->nSubmeshes].Position[1]=submesh_s[n].Position[1];
	submesh_n[oldh->nSubmeshes].Position[2]=submesh_s[n].Position[2];
	submesh_n[oldh->nSubmeshes].Floats[0]=submesh_s[n].Floats[0];
	submesh_n[oldh->nSubmeshes].Floats[1]=submesh_s[n].Floats[1];
	submesh_n[oldh->nSubmeshes].Floats[2]=submesh_s[n].Floats[2];
	submesh_n[oldh->nSubmeshes].Floats[3]=submesh_s[n].Floats[3];
	submesh_n[oldh->nSubmeshes].Unknown=submesh_s[n].Unknown;
	submesh_n[oldh->nSubmeshes].nBones=submesh_n[oldh->nSubmeshes-1].nBones;//nBones referenced in BoneLookupTable
	submesh_n[oldh->nSubmeshes].StartBones=submesh_n[oldh->nSubmeshes-1].StartBones;//StartingBone in BoneLookupTable set to same to avoid glitches!
	submesh_n[oldh->nSubmeshes].RootBone=abone;
	header_n->nSubmeshes=oldh->nSubmeshes+1;
	printf("Creating TexUnit...\n");
	tex[oldh->nTextureUnits].ColorIndex=-1;
	tex[oldh->nTextureUnits].Flags=16;
	tex[oldh->nTextureUnits].RenderFlags=0;
	tex[oldh->nTextureUnits].RenderFlags=0;
	tex[oldh->nTextureUnits].SubmeshIndex1=oldh->nSubmeshes;
	tex[oldh->nTextureUnits].SubmeshIndex2=oldh->nSubmeshes;
	tex[oldh->nTextureUnits].TexAnim=0;
	tex[oldh->nTextureUnits].one=1;
	tex[oldh->nTextureUnits].Texture=0;
	tex[oldh->nTextureUnits].Transparency=0;
	tex[oldh->nTextureUnits].TexUnitNumber=texunitnumber;
	tex[oldh->nTextureUnits].TexUnitNumber=texunitnumber;
	header_n->nTextureUnits=oldh->nTextureUnits+1;

	j=newskin;
}

int main(int argc, char **argv) {
	printf("CopyGeoset by Tigurius\n");
	if(argc<1){
		printf("Usage: copygeoset <ModeltoInsertGS> <ModelContainingGS>");
		return 0;
	}
	printf(" Loading Model %s\n",argv[1]);
	Input=fopen(argv[1],"rb+");
	string modelname=argv[1];
	modelname = modelname.substr(0, modelname.length() - 3 );
	modelname.append("00.skin");
	printf(" Loading Model %s\n",argv[2]);
	Readin=fopen(argv[2],"rb+");
	string geoname=argv[2];
	geoname = geoname.substr(0, geoname.length() - 3 );
	geoname.append("00.skin");
	printf(" Loading Skin %s\n",modelname.c_str());
	InSkin=fopen(modelname.c_str(),"rb+");
	printf(" Loading Skin %s\n",geoname.c_str());
	Skin=fopen(geoname.c_str(),"rb+");

	if(Input==NULL||Readin==NULL||Skin==NULL||InSkin==NULL)
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

	fseek(InSkin,0,SEEK_END);
	FSkin=ftell(InSkin);
	fseek(InSkin,0,SEEK_SET);

	j=new char[FSkin];

	fread(j,FSkin,1,InSkin);
	fclose(InSkin);

	fseek(Skin,0,SEEK_END);
	SSize=ftell(Skin);
	fseek(Skin,0,SEEK_SET);

	s=new char[SSize];

	fread(s,SSize,1,Skin);
	fclose(Skin);

	header_m=(ModelHeader *)(f);
	vertex_m=(ModelVertex *)(f+header_m->ofsVertices);
	texlookup=(short *)(f+header_m->ofsTexUnitLookup);
	header_i=(ModelHeader *)(r);
	vertex_i=(ModelVertex *)(r+header_i->ofsVertices);
	header_s=(SkinHeader *)(s);
	submesh_s=(Submesh *)(s+header_s->ofsSubmeshes);
	tri_s=(Triangle *)(s+header_s->ofsTriangles);
	vert_s=(short *)(s+header_s->ofsIndices);

	header_n=(SkinHeader *)(j);
	submesh_n=(Submesh *)(j+header_n->ofsSubmeshes);
	tri_n=(Triangle *)(j+header_n->ofsTriangles);
	vert_n=(short *)(j+header_n->ofsIndices);
	prop_n=(VProp *)(j+header_n->ofsProperties);
	tex=(TexUnit *)(j+header_n->ofsTextureUnits);
	system("cls");
	do{
	char k;
	printf("\n\tCopy Geosets!");
	printf("\n d - Display Submeshinfo\n e - Extract Submesh\n q - Quit\n");
	k=getchar();
	if(k=='d')DisplaySubmeshInfo();
	else if(k=='e')
		CopySubmesh();
	else if(k=='q')
		break;
	else if(k=='x') return 0;

	}while(1);

	printf("Saving Changes\n");
	Input=fopen(argv[1],"wb");
	fwrite(f,NewSize,1,Input);
	fclose(Input);

	InSkin=fopen(modelname.c_str(),"wb");
	fwrite(j,NewSkinSize,1,InSkin);
	fclose(InSkin);

	return 0;

}
