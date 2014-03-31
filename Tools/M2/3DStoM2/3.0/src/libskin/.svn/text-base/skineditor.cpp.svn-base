

#include "skineditor.h"

SkinEditor::SkinEditor(char *m,int FileSize){
	f=m;
	this->FileSize=FileSize;
	SetStructure();
}

SkinEditor::SkinEditor(){
	f=(char*)malloc(sizeof(SkinHeader));
	FileSize=sizeof(SkinHeader);
	for(int i=0;i<FileSize;i++){
		f[i]=0;
	}
	SetStructure();
	header_s->ID[0]='S';
	header_s->ID[1]='K';
	header_s->ID[2]='I';
	header_s->ID[3]='N';
	header_s->LOD=0x15;
}

SkinEditor::~SkinEditor(){

}

int SkinEditor::GetFileSize(){
	return FileSize;
}

char *SkinEditor::getFile(){
	return f;
}


void SkinEditor::SetStructure(){
	header_s=(SkinHeader *)(f);
	index_s=(Index *)(f+header_s->ofsIndices);
	triangle_s=(Triangle *)(f+header_s->ofsTriangles);
	propertie_s=(VProp *)(f+header_s->ofsProperties);
	submesh_s=(Submesh *)(f+header_s->ofsSubmeshes);
	texunit_s=(TexUnit *)(f+header_s->ofsTextureUnits);
}

void SkinEditor::FillLine(){
	int newSize=FileSize+(16-((FileSize)%16));
	char *n=(char*)malloc(newSize);
	memcpy(n,f,FileSize);
	f=n;
	FileSize=newSize;
	SetStructure();
}

Index *SkinEditor::getIndices(){
	return index_s;
}

int SkinEditor::getnIndices(){
	return header_s->nIndices;
}

void SkinEditor::newIndices(Index *ind,int num){
	FillLine();
	int newSize=FileSize+sizeof(Index)*num;
	char *n=f;
	f=(char*)malloc(newSize);
	memcpy(f,n,FileSize);
	SetStructure();
	header_s->nIndices=num;
	header_s->ofsIndices=FileSize;
	memcpy(f+FileSize,ind,sizeof(Index)*num);
	FileSize=newSize;
	FillLine();
}

void SkinEditor::newIndices(int num){
	FillLine();
	int newSize=FileSize+sizeof(Index)*num;
	char *n=f;
	f=(char*)malloc(newSize);
	memcpy(f,n,FileSize);
	SetStructure();
	header_s->nIndices=num;
	header_s->ofsIndices=FileSize;
	SetStructure();
	for(int i=0;i<header_s->nIndices;i++){
		index_s[i].Vertex=i;
	}
	FileSize=newSize;
	FillLine();

}

Triangle *SkinEditor::getTriangles(){
	return triangle_s;
}

int SkinEditor::getnTriangles(){
	return header_s->nTriangles;
}

void SkinEditor::newTriangles(Triangle *tri,int num){
	FillLine();
	int newSize=FileSize+sizeof(Triangle)*num;
	char *n=f;
	f=(char*)malloc(newSize);
	memcpy(f,n,FileSize);
	SetStructure();
	header_s->nTriangles=num;
	header_s->ofsTriangles=FileSize;
	memcpy(f+FileSize,tri,sizeof(Triangle)*num);
	FileSize=newSize;
	FillLine();
}

VProp *SkinEditor::getProperties(){
	return propertie_s;
}

int SkinEditor::getnProperties(){
	return header_s->nProperties;
}

void SkinEditor::newProperties(VProp *pro,int num){
	FillLine();
	int newSize=FileSize+sizeof(VProp)*num;
	char *n=f;
	f=(char*)malloc(newSize);
	memcpy(f,n,FileSize);
	SetStructure();
	header_s->nProperties=num;
	header_s->ofsProperties=FileSize;
	memcpy(f+FileSize,pro,sizeof(VProp)*num);
	FileSize=newSize;
	FillLine();
}

void SkinEditor::newProperties(int num){
	FillLine();
	int newSize=FileSize+sizeof(VProp)*num;
	char *n=f;
	f=(char*)malloc(newSize);
	memcpy(f,n,FileSize);
	SetStructure();
	header_s->nProperties=num;
	header_s->ofsProperties=FileSize;
	FileSize=newSize;
	FillLine();
}

Submesh *SkinEditor::getSubmeshes(){
	return submesh_s;
}

int SkinEditor::getnSubmeshes(){
	return header_s->nSubmeshes;
}

void SkinEditor::newSubmeshes(Submesh* sub,int num){
	FillLine();
	int newSize=FileSize+sizeof(Submesh)*num;
	char *n=f;
	f=(char*)malloc(newSize);
	memcpy(f,n,FileSize);
	SetStructure();
	header_s->nSubmeshes=num;
	header_s->ofsSubmeshes=FileSize;
	memcpy(f+FileSize,sub,sizeof(Submesh)*num);
	FileSize=newSize;
	FillLine();
}


TexUnit *SkinEditor::getTexUnits(){
	return texunit_s;
}

int SkinEditor::getnTexUnits(){
	return header_s->nTextureUnits;
}

void SkinEditor::newTexUnits(TexUnit* tex, int num){
	FillLine();
	int newSize=FileSize+sizeof(TexUnit)*num;
	char *n=f;
	f=(char*)malloc(newSize);
	memcpy(f,n,FileSize);
	SetStructure();
	header_s->nTextureUnits=num;
	header_s->ofsTextureUnits=FileSize;
	memcpy(f+FileSize,tex,sizeof(TexUnit)*num);
	FileSize=newSize;
	FillLine();
}


