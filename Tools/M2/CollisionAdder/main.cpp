/*
 * main.cpp
 *
 *  Created on: 12.10.2009
 *      Author: Bastian
 */

#include "libskin/skineditor.h"
#include "libm2/modeleditor.h"

int main(int argc, char **argv) {
	printf(" Loading Model %s\n",argv[1]);
	FILE*Input=fopen(argv[1],"rb+");
	if(Input==NULL)
	{
		printf(" ERROR: Could not load file %s",argv[1]);
		return 0;
	}
	string modelname=argv[1];
	modelname = modelname.substr(0, modelname.length() - 3 );
	modelname.append("00.skin");
	printf(" Loading Skin %s\n",modelname.c_str());
	FILE*Skin=fopen(modelname.c_str(),"rb+");
	if(Skin==NULL)
	{
		printf(" ERROR: Could not load file %s",modelname);
		return 0;
	}
	char *f,*s;
	int FileSize,SkinSize;
	fseek(Input,0,SEEK_END);
	FileSize=ftell(Input);
	fseek(Input,0,SEEK_SET);

	f=new char[FileSize];

	fread(f,FileSize,1,Input);
	fclose(Input);

	fseek(Skin,0,SEEK_END);
	SkinSize=ftell(Skin);
	fseek(Skin,0,SEEK_SET);

	s=new char[FileSize];

	fread(s,SkinSize,1,Skin);
	fclose(Skin);

	ModelEditor me=ModelEditor(f,FileSize);
	SkinEditor se=SkinEditor(s,SkinSize);

	ModelVertex*vert=me.getVertices();
	int nInd=se.getnIndices();
	Index *ind=se.getIndices();
	int nTri=se.getnTriangles()/3;
	Triangle*tri=se.getTriangles();
	BoundVolumeHelper bv;
	ModelBoundTriangle *bTri=new ModelBoundTriangle[nTri];
	Vec3D*bVert=new Vec3D[nInd];
	Vec3D*bNorm=new Vec3D[nInd];
	for(int i=0;i<nInd;i++){
		bVert[i]=vert[ind[i].Vertex].pos;
		bNorm[i]=vert[ind[i].Vertex].normal;
	}
	for(int i=0;i<nTri;i++){
		bTri[i].index[0]=ind[tri[i].Indices[0]].Vertex;
		bTri[i].index[1]=ind[tri[i].Indices[1]].Vertex;
		bTri[i].index[2]=ind[tri[i].Indices[2]].Vertex;
	}
	bv.nBNormals=nInd;
	bv.BNormals=bNorm;
	bv.BVertices=bVert;
	bv.nBVertices=nInd;
	bv.nBTriangles=nTri;
	bv.BTriangles=bTri;
	me.newBounds(bv);

	f=me.getFile();
	FileSize=me.GetFileSize();
	printf("Saving Changes\n");
	Input=fopen(argv[1],"wb");
	fwrite(f,FileSize,1,Input);
	fclose(Input);
	return 0;
}
