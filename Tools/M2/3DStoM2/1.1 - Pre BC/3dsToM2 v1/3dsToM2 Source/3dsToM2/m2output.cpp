#include "vec3d.h"
#include "modeldata.h"
#include <string>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <cstring>

ModelHeader	m2Header;

void fillLine(FILE *f)
{
	unsigned int Pos, linePos;
	char blank[16];
	memset(blank,0,16);
	Pos=ftell(f);
	linePos=Pos&0xFFFFFFF0;
	fwrite(blank,1,16-(Pos-linePos),f);
}

void writeAnimationBlockData(AnimationBlock *block, FILE *f, int *dRange, int nRange, int *dTimestamps, int nTimestamps, void *dData, int nData, int dataSize)
{
	block->ofsRanges=ftell(f);
	block->nRanges=nRange;
	if(nRange!=0)
	{
		fwrite(dRange,sizeof(int),nRange,f);
		fillLine(f);
	}
	else 
		block->ofsRanges=0;
	

	block->ofsTimes=ftell(f);
	block->nTimes=nTimestamps;
	if(nTimestamps!=0)
	{
		fwrite(dTimestamps,sizeof(int),nTimestamps,f);
		fillLine(f);
	}
	else
		block->ofsTimes=0;

	block->ofsKeys=ftell(f);
	block->nKeys=nData;
	if(nData!=0)
	{
		fwrite(dData,dataSize,nData,f);
		fillLine(f);
	}
	else
		block->ofsKeys=0;

	if(nData<=1)
		block->type=0;
	else
		block->type=1;
	block->seq=-1;
}

void initHeader()
{
	strcpy(&m2Header.id[0],"MD20");
	m2Header.version[1]=1;
	m2Header.version[2]=0;
	m2Header.version[3]=0;

	m2Header.type=0;

	m2Header.nGlobalSequences=0;
	m2Header.ofsGlobalSequences=0;

	m2Header.nTexUnitLookup=0;
	m2Header.ofsTexUnitLookup=0;

	m2Header.nTexAnims=0;
	m2Header.ofsTexAnims=0;

	for(int i=0;i<14;i++)
		m2Header.floats[i]=0;

	m2Header.nBoundingTriangles=0;
	m2Header.ofsBoundingTriangles=0;

	m2Header.nBoundingVertices=0;
	m2Header.ofsBoundingVertices=0;

	m2Header.nBoundingNormals=0;
	m2Header.ofsBoundingNormals=0;

	m2Header.nAttachments=0;
	m2Header.ofsAttachments=0;

	m2Header.nAttachments=0;
	m2Header.ofsAttachments=0;

	m2Header.nQ=0;
	m2Header.ofsQ=0;

	m2Header.nLights=0;
	m2Header.ofsLights=0;

	m2Header.nCameras=0;
	m2Header.ofsCameras=0;

	m2Header.nT=0;
	m2Header.ofsT=0;

	m2Header.nRibbonEmitters=0;
	m2Header.ofsRibbonEmitters=0;

	m2Header.nParticleEmitters=0;
	m2Header.ofsParticleEmitters=0;
}

void setupAnimations(FILE *f)
{
	ModelAnimation	anim;

	memset(&anim,0,sizeof(ModelAnimation));
	
	anim.timeEnd=3333;
	anim.flags=0x7fff;
	anim.playSpeed=150;
	anim.s[0]=0xffff;

	m2Header.nAnimations=1;
	m2Header.ofsAnimations=ftell(f);
	fwrite(&anim,sizeof(ModelAnimation),1,f);
	fillLine(f);
}

void setupBones(FILE *f)
{
	ModelBoneDef	bone;
	
	memset(&bone,0,sizeof(ModelBoneDef));

	bone.animid=-1;
	bone.parent=-1;
	bone.translation.seq=-1;
	bone.rotation.seq=-1;
	bone.scaling.seq=-1;

	m2Header.nBones=1;
	m2Header.ofsBones=ftell(f);

	fwrite(&bone,sizeof(ModelBoneDef),1,f);

	fillLine(f);
}

void initFile(FILE *f,const char *name)
{
	initHeader();

	fwrite(&m2Header,sizeof(ModelHeader),1,f);
	fillLine(f);

	m2Header.nameOfs=ftell(f);
	m2Header.nameLength=strlen(name)+1;

	fwrite(name,strlen(name)+1,1,f);
	fillLine(f);

	setupAnimations(f);

	m2Header.nC=1;
	m2Header.ofsC=ftell(f);
	fillLine(f);

	int	t[203];
	m2Header.nD=0xcb;
	m2Header.ofsD=ftell(f);
	memset(t,0,203*sizeof(int));
	fwrite(t,sizeof(int),203,f);
	fillLine(f);

	setupBones(f);

	m2Header.nF=1;
	m2Header.ofsF=ftell(f);
	t[0]=0xffff;
	fwrite(t,2,1,f);
	fillLine(f);
}

void writeModelData(FILE *f,ModelVertex *vertices,int nVertices, uint16	*triangles, int nTriangles, ModelGeoset	*submeshes, int nSubmeshes, ModelTexUnit *texUnits, int nTexUnits)
{
	ModelView	views[4];
	int i,j,k;

	m2Header.ofsVertices=ftell(f);
	m2Header.nVertices=nVertices*4;
	fwrite(vertices,sizeof(ModelVertex),nVertices,f);
	fwrite(vertices,sizeof(ModelVertex),nVertices,f);
	fwrite(vertices,sizeof(ModelVertex),nVertices,f);
	fwrite(vertices,sizeof(ModelVertex),nVertices,f);
	fillLine(f);

	views[0].lod=0x100;
	views[1].lod=0x4b;
	views[2].lod=0x35;
	views[3].lod=0x15;

	m2Header.ofsViews=ftell(f);
	m2Header.nViews=4;

	fwrite(views, sizeof(ModelView), 4, f);
	fillLine(f);

	for(i=0;i<4;i++)
	{
		views[i].ofsIndex=ftell(f);
		views[i].nIndex=nVertices;
		for(j=0;j<nVertices;j++)
		{
			k=j+i*nVertices;
			fwrite(&k,sizeof(uint16),1,f);
		}
		fillLine(f);

		views[i].ofsTris=ftell(f);
		views[i].nTris=nTriangles*3;
		fwrite(triangles,sizeof(uint16)*3,nTriangles,f);
		fillLine(f);

		views[i].ofsProps=ftell(f);
		views[i].nProps=nVertices;
		k=0;
		for(j=0;j<nVertices;j++)
			fwrite(&k,sizeof(int),1,f);
		fillLine(f);

		views[i].ofsSub=ftell(f);
		views[i].nSub=nSubmeshes;
		fwrite(submeshes,sizeof(ModelGeoset),nSubmeshes,f);
		fillLine(f);

		views[i].ofsTex=ftell(f);
		views[i].nTex=nTexUnits;
		fwrite(texUnits,sizeof(ModelTexUnit),nTexUnits,f);
		fillLine(f);
	}

	fseek(f,m2Header.ofsViews,SEEK_SET);
	fwrite(views,sizeof(ModelView),4,f);
	fseek(f,0,SEEK_END);
}

void writeMaterialData(FILE *f, std::vector<std::string> texNames, ModelTextureDef *textures, int nTextures, int nMaterials, float *colorData, short *colorTransData, short *transData, uint32 *renderFlags)
{
	int i,j;
	
	//Color Data 
	m2Header.nColors = nMaterials;
	m2Header.ofsColors = ftell(f);

	AnimationBlock *colorBlocks;
	colorBlocks=new AnimationBlock[2*nMaterials];
	fwrite(colorBlocks,sizeof(AnimationBlock),2*nMaterials,f);
	fillLine(f);

	j=0;

	for(i=0;i<nMaterials;i++)
	{
		writeAnimationBlockData(&colorBlocks[i*2], f, 0, 0, &j, 1, &colorData[3*i], 1, 3*sizeof(float));
		writeAnimationBlockData(&colorBlocks[i*2+1], f, 0, 0, &j, 1, &colorTransData[i], 1, sizeof(short));
	}
	fseek(f,m2Header.ofsColors,SEEK_SET);

	fwrite(colorBlocks,sizeof(AnimationBlock),2*nMaterials,f);
	delete colorBlocks;
	fseek(f,0,SEEK_END);

	//Texture Data
	m2Header.ofsTextures=ftell(f);
	m2Header.nTextures=nTextures;

	fwrite(textures, sizeof(ModelTextureDef), nTextures, f);

	for(i=0;i<nTextures;i++)
	{
		textures[i].nameOfs=ftell(f);
		fwrite(texNames[i].c_str(),textures[i].nameLen,1,f);
		fillLine(f);
	}

	fseek(f,m2Header.ofsTextures,SEEK_SET);
	fwrite(textures, sizeof(ModelTextureDef), nTextures, f);
	fseek(f,0,SEEK_END);

	//transparency block
	AnimationBlock *transBlocks;

	m2Header.nTransparency=nMaterials;
	m2Header.ofsTransparency=ftell(f);
	
	transBlocks=new AnimationBlock[nMaterials];
	fwrite(transBlocks,sizeof(AnimationBlock),nMaterials,f);
	fillLine(f);

	for(i=0;i<nMaterials;i++)
		writeAnimationBlockData(&transBlocks[i], f, 0, 0, &j, 1, &transData[i], 1, sizeof(short));
	
	fseek(f,m2Header.ofsTransparency,SEEK_SET);
	fwrite(transBlocks,sizeof(AnimationBlock),nMaterials,f);
	delete transBlocks;
	fseek(f,0,SEEK_END);
	
	//K block now need to understand this one better
	m2Header.nTexReplace=1;
	m2Header.ofsTexReplace=ftell(f);

	fillLine(f);

	//YBlock
	m2Header.nY=4;
	m2Header.ofsY=ftell(f);

	fillLine(f);

	//Render Flags now
	m2Header.nTexFlags=nMaterials;
	m2Header.ofsTexFlags=ftell(f);

	fwrite(renderFlags,sizeof(int),nMaterials,f);
	fillLine(f);


	//Texture Lookup Table
	m2Header.ofsTexLookup=ftell(f);
	m2Header.nTexLookup=nTextures;
	for(i=0;i<nTextures;i++)
		fwrite(&i,sizeof(uint16),1,f);
	fillLine(f);
}

void writeMisc(FILE *f)
{
	int i;

	m2Header.nTexUnitLookup=1;
	m2Header.ofsTexUnitLookup=ftell(f);
	fillLine(f);

	m2Header.nTransparencyLookup=m2Header.nTransparency;
	m2Header.ofsTransparencyLookup=ftell(f);
	for(i=0;i<m2Header.nTransparency;i++)
		fwrite(&i,sizeof(uint16),1,f);
	fillLine(f);

	m2Header.nTexAnimLookup=1;
	m2Header.ofsTexAnimLookup=ftell(f);
	i=0xffff;
	fwrite(&i,2,1,f);
	fillLine(f);
}

void rewriteHeader(FILE *f)
{
	writeMisc(f);
	fseek(f,0,SEEK_SET);
	fwrite(&m2Header,sizeof(ModelHeader),1,f);
}