#include "vec3d.h"
#include "modelheaders.h"

#include <vector>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstring>

char *f1,*f2;

char *readFile(FILE *f)
{
	int fileLength;
	char *inputData;
	fseek(f,0,SEEK_END);
	fileLength=ftell(f);
	inputData=new char[fileLength];
	fseek(f,0,SEEK_SET);
	fread(inputData,fileLength,1,f);
	return inputData;
}
int byteWrong;
bool compareData(char *d1,char *d2,int length)
{
	for(int i=0;i<length;i++)
		if(d1[i]!=d2[i])
		{
			byteWrong=i;
			return false;
		}
	return true;
}

void viewCompare(char *d1, char *d2)
{
	ModelView *v1,*v2;
	
	v1=(ModelView *)d1;
	v2=(ModelView *)d2;
	
	for(int i=0;i<4;i++)
	{
		if(!compareData(f1+v1[i].ofsIndex,f2+v2[i].ofsIndex,v1[i].nIndex*2))
		{
			printf("Not the same in Indices\n");
			return;
		}
		if(!compareData(f1+v1[i].ofsProps,f2+v2[i].ofsProps,v1[i].nProps*4))
		{
			printf("Not the same in Properties\n");
			return;
		}
		if(!compareData(f1+v1[i].ofsTris,f2+v2[i].ofsTris,v1[i].nTris*2))
		{
			printf("Not the same in Triangles for view %d at byte %d\n",i,byteWrong);
			return;
		}
		if(!compareData(f1+v1[i].ofsSub,f2+v2[i].ofsSub,v1[i].nSub*32))
		{
			printf("Not the same in Subsets for view %d at byte %d\n",i,byteWrong);
			return;
		}
		if(!compareData(f1+v1[i].ofsTex,f2+v2[i].ofsTex,v1[i].nTex*24))
		{
			printf("Not the same in Subsets for view %d at byte %d\n",i,byteWrong);
			return;
		}
		if(v1[i].lod!=v2[i].lod)
		{
			printf("LOD not the same for %d\n",i);
		}
	}
}

void ablockCompare(char *d1, char *d2,int num,int size)
{
	AnimationBlock	*a1,*a2;
	a1=(AnimationBlock *)d1;
	a2=(AnimationBlock *)d2;
	for(int i=0;i<num;i++)
	{
		if(a1[i].seq!=a2[i].seq)
		{
			printf("Animation Block Sequence %d not the same at byte %d\n",i,byteWrong);
			return;
		}
		if(a1[i].type!=a2[i].type)
		{
			printf("Animation Block Sequence %d not the same at byte %d\n",i,byteWrong);
			return;
		}
		if(!compareData(f1+a1[i].ofsRanges,f2+a2[i].ofsRanges,4*a1[i].nRanges))
		{
			printf("Animation Block Range %d not the same at byte %d\n",i,byteWrong);
			return;
		}
		if(!compareData(f1+a1[i].ofsTimes,f2+a2[i].ofsTimes,4*a1[i].nTimes))
		{
			printf("Animation Block Times %d not the same at byte %d\n",i,byteWrong);
			return;
		}
		if(!compareData(f1+a1[i].ofsKeys,f2+a2[i].ofsKeys,size*a1[i].nKeys))
		{
			printf("Animation Block Keys %d not the same at byte %d\n",i,byteWrong);
			return;
		}
	}
}

void compareBones(char *d1, char *d2,int num)
{
	ModelBoneDef *b1,*b2;
	b1=(ModelBoneDef *)d1;
	b2=(ModelBoneDef *)d2;
	for(int i=0;i<num;i++)
	{
		if(b1[i].animid!=b2[i].animid)
		{
			printf("Bone anim id not the same\n");
			return;
		}
		if(b1[i].flags!=b2[i].flags)
		{
			printf("Bone flags not the same\n");
			return;
		}
		if(b1[i].geoid!=b2[i].geoid)
		{
			printf("Bone id not the same\n");
			return;
		}
		if(b1[i].parent !=b2[i].parent)
		{
			printf("Bone parent not the same\n");
			return;
		}
		if(!compareData((char *)&b1[i].pivot.x,(char *)&b2[i].pivot.x,12))
		{
			printf("Bone Pivot not the same\n");
			return;
		}
		ablockCompare((char *)&(b1[i].rotation),(char *)&(b2[i].rotation),1,16);
		ablockCompare((char *)&(b1[i].translation),(char *)&(b2[i].translation),1,16);
		ablockCompare((char *)&b1[i].scaling,(char *)&b2[i].scaling,1,16);
	}
}

void compareData()
{
	ModelHeader *h1,*h2;
	h1=(ModelHeader *)f1;
	h2=(ModelHeader *)f2;

	viewCompare(f1+h1->ofsViews,f2+h2->ofsViews);

	if(!compareData(f1+h1->ofsC,f2+h2->ofsC,h1->nC*2))
	{
		printf("C Block data doesn't match at byte %d\n",byteWrong);
	}
	if(!compareData(f1+h1->ofsD,f2+h2->ofsD,h1->nD*4))
	{
		printf("D Block data doesn't match at byte %d\n",byteWrong);
	}
	if(!compareData(f1+h1->ofsF,f2+h2->ofsF,h1->nF*2))
	{
		printf("F Block data doesn't match at byte %d\n",byteWrong);
	}
	if(!compareData(f1+h1->ofsAnimations,f2+h2->ofsAnimations,h1->nAnimations*68))
	{
		printf("Animation data doesn't match at byte %d\n",byteWrong);
	}
	if(!compareData(f1+h1->ofsVertices,f2+h2->ofsVertices,h1->nVertices*48))
	{
		printf("Vertex data doesn't match at byte %d\n",byteWrong);
	}
	compareBones(f1+h1->ofsBones,f2+h2->ofsBones,h1->nBones);
}

int main(int argc, char **argv)
{
	FILE	*file1,*file2;
	
	file1=fopen(argv[1],"rb");
	file2=fopen(argv[2],"rb");

	f1=readFile(file1);
	f2=readFile(file2);	

	compareData();

	fclose(file1);
	fclose(file2);
}