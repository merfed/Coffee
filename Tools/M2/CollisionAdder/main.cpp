/*
 * main.cpp
 *
 *  Created on: 12.10.2009
 *      Author: Bastian
 */

 /*
  * Small changes on bounding box normals : Mjollnà 2011-10-25
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
	std::string modelname=argv[1]; // std:: added
	modelname = modelname.substr(0, modelname.length() - 3 );
	modelname.append("00.skin");
	printf(" Loading Skin %s\n",modelname.c_str()); // .c_str() added.
	FILE*Skin=fopen(modelname.c_str(),"rb+");
	if(Skin==NULL)
	{
		printf(" ERROR: Could not load file %s",modelname.c_str());
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
		// bNorm[i]=vert[ind[i].Vertex].normal;
	}
	for(int i=0;i<nTri;i++){
		bTri[i].index[0]=ind[tri[i].Indices[0]].Vertex;
		bTri[i].index[1]=ind[tri[i].Indices[1]].Vertex;
		bTri[i].index[2]=ind[tri[i].Indices[2]].Vertex;
		
		// Mjo : We add 1 surface normal to every triangle
		Vec3D U=vert[ind[tri[i].Indices[1]].Vertex].pos-vert[ind[tri[i].Indices[0]].Vertex].pos; 
		//std::cout << U.x << " " << U.y << " " << U.z << std::endl;
		Vec3D V=vert[ind[tri[i].Indices[2]].Vertex].pos-vert[ind[tri[i].Indices[0]].Vertex].pos;			
		//std::cout << V.x << " " << V.y << " " << V.z << std::endl;		
		bNorm[i]=U%V;
		bNorm[i].normalize();
		//std::cout << bNorm[i].x << " " << bNorm[i].y << " " << bNorm[i].z << std::endl;
	}
	bv.nBNormals=nTri; // Mjo : One normal per triangle (was : nInd)
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
