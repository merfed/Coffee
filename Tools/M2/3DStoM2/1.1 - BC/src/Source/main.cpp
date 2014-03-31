#include "vec3d.h"
#include "modeldata.h"
#include "matrix.h"

#include "lib3ds\file.h"
#include "lib3ds\material.h"
#include "lib3ds\mesh.h"
#include "lib3ds\vector.h"
#include <vector>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "m2output.h"

ModelHeader *header;
ModelView	*views;

ModelVertex	*dVertices;
int			nVertices;
uint16		*dTriangles;
int			nTriangles;
ModelGeoset	*oldSubmesh;
ModelGeoset	*dSubmeshes;
int			nSubmeshes;
ModelTexUnit	*dTextureUnits;
int				nTextureUnits;
std::vector<std::string>	textureNames;
ModelTextureDef	*dTextures;
int		nTextures;

int		nMaterials;
float	*colorData;
short	*colorTransData;
short	*transData;
uint32	*renderFlags;

float Scale;


void load3ds(Lib3dsFile *f);

void createTextures()
{
	printf("Preparing Texture Data\n");
	nTextures=textureNames.size();
	printf(" %d Texture(s)\n",nTextures);
	dTextures=new ModelTextureDef[nTextures];
	for(int i=0;i<nTextures;i++)
	{
		int length=textureNames[i].length();
		if(length==0)
		{
			textureNames[i]="DUNGEONS\\BUILDINGS\\KL_COLLIDABLEDOODADS\\TANARIS\\CAVERNSTIME\\TEXTURE\\TANARIS_SHIPWRECK_DECKPLANKING.blp";
			length=textureNames[i].length();
		}
		textureNames[i][length-3]='b';
		textureNames[i][length-2]='l';
		textureNames[i][length-1]='p';
		dTextures[i].nameLen=length+1;
		dTextures[i].type=0;
		dTextures[i].flags=3;
		printf(" %d - %s\n",i,textureNames[i].c_str());
	}
}

void convert3ds(FILE *outputM2, Lib3dsFile *f)
{
	load3ds(f);
	createTextures();

	printf("Writing M2 File\n");
	initFile(outputM2,"TEMP");
	printf("Writing Model Data\n");
	writeModelData(outputM2, dVertices, nVertices, dTriangles, nTriangles, dSubmeshes, nSubmeshes, dTextureUnits, nTextureUnits);
	printf("Writing Material Data\n");
	writeMaterialData(outputM2, textureNames, dTextures, nTextures, nMaterials, colorData, colorTransData, transData, renderFlags);
	printf("Finalizing Model Data\n");
	rewriteHeader(outputM2);

	/*int i,j,k;
	char *inputData;
	unsigned int fileLength;
	fseek(inputM2,0,SEEK_END);
	fileLength=ftell(inputM2);
	inputData=new char[fileLength];
	fseek(inputM2,0,SEEK_SET);
	fread(inputData,fileLength,1,inputM2);
	fwrite(inputData,fileLength,1,outputM2);

	header=(ModelHeader *)inputData;
	views=(ModelView *)(inputData+header->ofsViews);
	oldSubmesh=(ModelGeoset *)(inputData+views->ofsSub);
	load3ds(f);


	header->ofsVertices=ftell(outputM2);
	header->nVertices=nVertices*4;
	fwrite(dVertices,sizeof(ModelVertex),nVertices,outputM2);
	fwrite(dVertices,sizeof(ModelVertex),nVertices,outputM2);
	fwrite(dVertices,sizeof(ModelVertex),nVertices,outputM2);
	fwrite(dVertices,sizeof(ModelVertex),nVertices,outputM2);
	fillLine(outputM2);

	header->ofsTexLookup=ftell(outputM2);
	header->nTexLookup=textureNames.size();
	for(i=0;i<textureNames.size();i++)
		fwrite(&i,sizeof(uint16),1,outputM2);
	fillLine(outputM2);

	//Write out texture data
	createTextures();
	for(i=0;i<nTextures;i++)
	{
		dTextures[i].nameOfs=ftell(outputM2);
		fwrite(textureNames[i].c_str(),textureNames[i].length()+1,1,outputM2);
		fillLine(outputM2);
	}

	header->nTextures=nTextures;
	header->ofsTextures=ftell(outputM2);
	fwrite(dTextures,sizeof(ModelTextureDef),nTextures,outputM2);
	fillLine(outputM2);

	//Output Color data
	header->nColors=nMaterials;
	header->ofsColors=ftell(outputM2);
	
	AnimationBlock *colorBlocks;
	colorBlocks=new AnimationBlock[2*nMaterials];
	fwrite(colorBlocks,sizeof(AnimationBlock),2*nMaterials,outputM2);
	fillLine(outputM2);

	j=0;

	for(i=0;i<nMaterials;i++)
	{
		writeAnimationBlockData(&colorBlocks[i*2], outputM2, 0, 0, &j, 1, &colorData[3*i], 1, 3*sizeof(float));
		writeAnimationBlockData(&colorBlocks[i*2+1], outputM2, 0, 0, &j, 1, &colorTransData[i], 1, sizeof(short));
	}
	fseek(outputM2,header->ofsColors,SEEK_SET);
	fwrite(colorBlocks,sizeof(AnimationBlock),2*nMaterials,outputM2);
	delete colorBlocks;
	fseek(outputM2,0,SEEK_END);

	//Output Transparency
	AnimationBlock *transBlocks;

	header->nTransparency=nMaterials;
	header->ofsTransparency=ftell(outputM2);
	
	transBlocks=new AnimationBlock[nMaterials];
	fwrite(transBlocks,sizeof(AnimationBlock),nMaterials,outputM2);
	fillLine(outputM2);

	for(i=0;i<nMaterials;i++)
		writeAnimationBlockData(&transBlocks[i], outputM2, 0, 0, &j, 1, &transData[i], 1, sizeof(short));
	
	fseek(outputM2,header->ofsTransparency,SEEK_SET);
	fwrite(transBlocks,sizeof(AnimationBlock),nMaterials,outputM2);
	delete transBlocks;
	fseek(outputM2,0,SEEK_END);

	header->nTransparencyLookup=nMaterials;
	header->ofsTransparencyLookup=ftell(outputM2);
	for(i=0;i<textureNames.size();i++)
		fwrite(&i,sizeof(uint16),1,outputM2);
	fillLine(outputM2);

	//Output Render Flags
	header->nTexFlags=nMaterials;
	header->ofsTexFlags=ftell(outputM2);

	fwrite(renderFlags,sizeof(uint32),nMaterials,outputM2);
	fillLine(outputM2);
	
	//Redo View Data
	for(i=0;i<header->nViews;i++)
	{
		views[i].ofsIndex=ftell(outputM2);
		views[i].nIndex=nVertices;
		for(j=0;j<nVertices;j++)
		{
			k=j+i*nVertices;
			fwrite(&k,sizeof(uint16),1,outputM2);
		}
		fillLine(outputM2);

		views[i].ofsTris=ftell(outputM2);
		views[i].nTris=nTriangles*3;
		fwrite(dTriangles,sizeof(uint16)*3,nTriangles,outputM2);
		fillLine(outputM2);

		views[i].ofsProps=ftell(outputM2);
		views[i].nProps=nVertices;
		k=0;
		for(j=0;j<nVertices;j++)
			fwrite(&k,sizeof(int),1,outputM2);
		fillLine(outputM2);

		views[i].ofsSub=ftell(outputM2);
		views[i].nSub=nSubmeshes;
		fwrite(dSubmeshes,sizeof(ModelGeoset),nSubmeshes,outputM2);
		fillLine(outputM2);

		views[i].ofsTex=ftell(outputM2);
		views[i].nTex=nTextureUnits;
		fwrite(dTextureUnits,sizeof(ModelTexUnit),nTextureUnits,outputM2);
		fillLine(outputM2);
	}



	fseek(outputM2,0,SEEK_SET);
	fwrite(header,sizeof(ModelHeader),1,outputM2);
	fseek(outputM2,header->ofsViews,SEEK_SET);
	fwrite(views,sizeof(ModelView),header->nViews,outputM2);*/
}

int	findTexture(const char *MaterialName, Lib3dsFile *f)
{
	int i;
	Lib3dsMaterial *mat;
	for (mat=f->materials; mat; mat=mat->next)
		if(strcmp(mat->name,MaterialName)==0)
			break;
	if(mat==0)
		return 0;
	for(i=0;i<textureNames.size();i++)
		if(strcmp(textureNames[i].c_str(),mat->texture1_map.name)==0)
			return i;
	return 0;//default to 0 in case invalid
}

int	materialNumber(const char *matName,Lib3dsFile *f)
{
	int i;
	Lib3dsMaterial *mat;
	i=0;
	for (mat=f->materials; mat; mat=mat->next)
	{
		if(strcmp(mat->name,matName)==0)
			return i;
		i++;
	}
	return 0;
}


uint32 setRenderFlag(Lib3dsMaterial *mat)
{
	uint32 flag=0,blend;
	if(mat->self_illum)
		flag+=1;
	if(mat->use_falloff)
		flag+=2;
	if(mat->two_sided)
		flag+=4;

	if(mat->additive)
		blend=3;
	else
		blend=0;
	return (blend<<16)+flag;
}

void extractMaterialData(Lib3dsFile *f)
{
	Lib3dsMaterial *mat;
	int i,j;
	short Temp;
	bool	noTextures;

	//First lets count how many materials and store the texture names
	nMaterials=0;
	for (mat=f->materials; mat; mat=mat->next)
	{
		for(i=0;i<textureNames.size();i++)
			if(strcmp(textureNames[i].c_str(),mat->texture1_map.name)==0)
				break;
		if(i==textureNames.size())
			textureNames.push_back(std::string(mat->texture1_map.name));
		nMaterials++;
	}

	if(nMaterials==0)
	{
		nMaterials=1;
		noTextures=true;
		textureNames.push_back(std::string(""));
	}
	else
		noTextures=false;

	colorData=new float[3*nMaterials];
	colorTransData=new short[nMaterials];
	transData=new short[nMaterials];
	renderFlags=new uint32[nMaterials];

	i=0;
	//Now Load up the data from each of the materials
	if(noTextures)
	{
		colorData[0]=1.0f;
		colorData[1]=1.0f;
		colorData[2]=1.0f;
		colorTransData[0]=32767;
		transData[0]=32767;
		renderFlags[0]=0;
	}
	else
	{
		for (mat=f->materials; mat; mat=mat->next)
		{
			memcpy(&colorData[i*3],mat->diffuse,sizeof(float)*3);
			colorTransData[i]=(short)(32767*mat->diffuse[3]);
			transData[i]=(short)(32767*(1.0-mat->transparency));
			renderFlags[i]=setRenderFlag(mat);
			i++;
		}
	}
}


void load3ds(Lib3dsFile *f)
{
	int i,j,meshV,meshT,meshNum;
	Lib3dsMesh *m;
	Lib3dsMaterial *mat;
	//First thing need to count all the vertices and triangles;
	nVertices=0;
	nTriangles=0;
	nSubmeshes=0;
	printf("Loading 3ds File\n");
	

	extractMaterialData(f);

	for (m=f->meshes; m; m=m->next)
	{
		nVertices+=m->points;
		nTriangles+=m->faces;
		for(i=1;i<m->faces;i++)
			if(strcmp(m->faceL[i-1].material,m->faceL[i].material)!=0)
				nSubmeshes++;
		nSubmeshes++;
	}
	printf("Vertices: %d Triangles: %d Submeshes: %d\n",nVertices,nTriangles,nSubmeshes);
	dVertices=new ModelVertex[nVertices];
	dTriangles=new uint16[nTriangles*3];
	dSubmeshes=new ModelGeoset[nSubmeshes];
	nTextureUnits=nSubmeshes;
	dTextureUnits=new ModelTexUnit[nTextureUnits];

	//Load initial vertex & triangle data
	meshV=0;
	meshT=0;
	meshNum=0;
	Matrix meshMat;
	for (m=f->meshes; m; m=m->next)
	{
		printf("Processing Mesh %d\n",meshNum);
		
		//This mesh must have something in it
		
		dSubmeshes[meshNum].id=0;
		dSubmeshes[meshNum].vstart=meshV;
		dSubmeshes[meshNum].vcount=m->points;
		dSubmeshes[meshNum].istart=meshT*3;
		dSubmeshes[meshNum].icount=m->faces*3;
		dSubmeshes[meshNum].d2=0;
		dSubmeshes[meshNum].d3=1;
		dSubmeshes[meshNum].d4=0;
		dSubmeshes[meshNum].d5=1;
		dSubmeshes[meshNum].d6=0;
		dSubmeshes[meshNum].v.x=0.001168764;
		dSubmeshes[meshNum].v.y=5.960465E-9;
		dSubmeshes[meshNum].v.z=0.6214849;
		
		//For now we are going to fake the texture unit data
		int matNum=materialNumber(m->faceL[0].material,f);
		dTextureUnits[meshNum].flags=0;
		dTextureUnits[meshNum].order=0;
		dTextureUnits[meshNum].op=meshNum;
		dTextureUnits[meshNum].op2=meshNum;
		dTextureUnits[meshNum].colorIndex=matNum;
		dTextureUnits[meshNum].flagsIndex=matNum;
		dTextureUnits[meshNum].texunit=0;
		dTextureUnits[meshNum].textureid=findTexture(m->faceL[0].material,f);
		dTextureUnits[meshNum].texunit2=0;
		dTextureUnits[meshNum].transid=matNum;
		dTextureUnits[meshNum].texanimid=0;
		dTextureUnits[meshNum].d4=1;
		
		meshNum++;

		
		for(i=0;i<m->points;i++)
		{
			dVertices[i+meshV].pos=Vec3D(m->pointL[i].pos[0],m->pointL[i].pos[1],m->pointL[i].pos[2])*0.01*.33*Scale;
			dVertices[i+meshV].texcoords=Vec2D(m->texelL[i][0],m->texelL[i][1]);

			dVertices[i+meshV].texcoords[1]=1-dVertices[i+meshV].texcoords[1];
			for(j=0;j<4;j++)
			{
				dVertices[i+meshV].bones[j]=0;
				dVertices[i+meshV].weights[j]=0;
			}
			dVertices[i+meshV].weights[0]=255;
			dVertices[i+meshV].unk1=0;
			dVertices[i+meshV].unk2=0;
			//Calculate Normal
			Vec3D Normal;
			Normal=Vec3D(0,0,0);
			for(j=0;j<m->faces;j++)
			{
				if((m->faceL[j].points[0]==i)||(m->faceL[j].points[2]==i)||(m->faceL[j].points[2]==i))
					Normal+=Vec3D(m->faceL[j].normal[0],m->faceL[j].normal[1],m->faceL[j].normal[2]);
			}
			Normal.normalize();
			dVertices[i+meshV].normal=Normal;
		}
		for(i=0;i<m->faces;i++)
		{
			if(i!=0)
			{
				if(strcmp(m->faceL[i-1].material,m->faceL[i].material)!=0)
				{
					//This mesh must have something in it
					dSubmeshes[meshNum].id=0;
					dSubmeshes[meshNum].vstart=meshV;
					dSubmeshes[meshNum].vcount=m->points;
					dSubmeshes[meshNum].istart=meshT*3+i*3;
					//Change count numbers
					dSubmeshes[meshNum-1].icount-=m->faces*3-i*3;
					dSubmeshes[meshNum].icount=m->faces*3-i*3;
					dSubmeshes[meshNum].d2=0;
					dSubmeshes[meshNum].d3=1;
					dSubmeshes[meshNum].d4=0;
					dSubmeshes[meshNum].d5=1;
					dSubmeshes[meshNum].d6=0;
					dSubmeshes[meshNum].v.x=0.001168764;
					dSubmeshes[meshNum].v.y=5.960465E-9;
					dSubmeshes[meshNum].v.z=0.6214849;
					//For now we are going to fake the texture unit data
					dTextureUnits[meshNum].flags=0;
					dTextureUnits[meshNum].colorIndex=0;
					dTextureUnits[meshNum].order=0;
					dTextureUnits[meshNum].op=meshNum;
					dTextureUnits[meshNum].op2=meshNum;
					dTextureUnits[meshNum].colorIndex=0;
					dTextureUnits[meshNum].flagsIndex=0;
					dTextureUnits[meshNum].texunit=0;
					dTextureUnits[meshNum].textureid=findTexture(m->faceL[i].material,f);
					dTextureUnits[meshNum].texunit2=0;
					dTextureUnits[meshNum].transid=0;
					dTextureUnits[meshNum].texanimid=0;
					dTextureUnits[meshNum].d4=1;
					
					meshNum++;
				}
			}
			dTriangles[(i+meshT)*3]=(uint16)m->faceL[i].points[0]+meshV;
			dTriangles[(i+meshT)*3+1]=(uint16)m->faceL[i].points[1]+meshV;
			dTriangles[(i+meshT)*3+2]=(uint16)m->faceL[i].points[2]+meshV;
			
		}
		meshV+=m->points;
		meshT+=m->faces;
	}
}

int main(int argc, char **argv)
{
	printf("3ds To M2 Converter v2.0 (July 29th, 2007)\n");
	printf(" (Original Version July 28rd, 2006)\n");
	printf("-by John \"Cryect\" Rittenhouse\n");
	printf("-fixed by schlumpf\n");
	Lib3dsFile *f=0;
	FILE	*inputM2,*outputM2;

	if(argc<3)
	{
		printf(" 3dsToM2 <3ds File Name> <M2 File Name> [<Optional Scale>]\n");
		return 0;
	}
	
	f=lib3ds_file_load(argv[1]);

	if(!f)
	{
		printf("ERROR: Unable to load 3ds file %s\n",argv[1]);
		return 0;
	}

	outputM2=fopen(argv[2],"wb");
	if(!outputM2)
	{
		printf("ERROR: Unable to open M2 file for writing %s\n",argv[2]);
		lib3ds_file_free(f);
		return 0;
	}
	
	if(argc>3)
		Scale=atof(argv[3]);
	else
		Scale=1;

	convert3ds(outputM2, f);
	fclose(outputM2);
	lib3ds_file_free(f);

	printf("3ds to M2 conversion completed\n");
}

