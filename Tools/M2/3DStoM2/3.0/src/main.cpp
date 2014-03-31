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

#include "libm2\modeleditor.h"
#include "libskin\skineditor.h"

//.skin related
Triangle	*dTriangles;
int			nTriangles;
Submesh	*dSubmeshes;
int			nSubmeshes;
TexUnit	*dTextureUnits;
int				nTextureUnits;

//.m2 related
ModelVertex	*dVertices;
int			nVertices;

TextureHelper tex=TextureHelper();
std::vector<std::string>	textureNames;
ModelTextureDef	*dTextures;
int		nTextures;

int		nMaterials;
ColorHelper col=ColorHelper();
TransparencyHelper trans=TransparencyHelper();
ModelRenderFlags	*renderFlags;

int nAnimation;
ModelAnimation *dAnim;

int nBones;
ModelBoneDef *dBone;
BoneHelper *bon;

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

ModelRenderFlags setRenderFlag(Lib3dsMaterial *mat)
{
	uint16 flag=0,blend;
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
	ModelRenderFlags ret;
	ret.blend=blend;
	ret.flags=flag;
	return ret;
}

void extractMaterialData(Lib3dsFile *f)
{
	Lib3dsMaterial *mat;
	int i;
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

	col.setnColors(nMaterials);
	trans.setnTransparency(nMaterials);
	renderFlags=new ModelRenderFlags[nMaterials];

	i=0;
	//Now Load up the data from each of the materials
	if(noTextures)
	{
		col.setnSubs(0,1);
		trans.setnSubs(0,1);
		Vec3Helper *c=new Vec3Helper[1];
		ShortHelper *t=new ShortHelper[1];
		t[0].timestamp=0;
		t[0].value=32767;
		c[0].timestamp=0;
		c[0].value.x=1.0f;
		c[0].value.y=1.0f;
		c[0].value.z=1.0f;
		col.info[0][0].setData(c,t,1);
		trans.info[0][0].setData(t,1);
		renderFlags[0].blend=0;
		renderFlags[0].flags=0;
	}
	else
	{
		for (mat=f->materials; mat; mat=mat->next)
		{
			col.setnSubs(i,1);
			trans.setnSubs(i,1);
			Vec3Helper *c=new Vec3Helper[1];
			ShortHelper *t=new ShortHelper[1];
			t[0].timestamp=0;
			t[0].value=32767;
			c[0].timestamp=0;
			c[0].value.x=mat->diffuse[0];
			c[0].value.y=mat->diffuse[1];
			c[0].value.z=mat->diffuse[2];
			t[0].value=(short)(32767*mat->diffuse[3]);
			col.info[i][0].setData(c,t,1);
			t[0].value=(short)(32767*(1.0-mat->transparency));
			trans.info[i][0].setData(t,1);
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
	dTriangles=new Triangle[nTriangles];
	dSubmeshes=new Submesh[nSubmeshes];
	nTextureUnits=nSubmeshes;
	dTextureUnits=new TexUnit[nTextureUnits];

	//Load initial vertex & triangle data
	meshV=0;
	meshT=0;
	meshNum=0;
	Matrix meshMat;
	for (m=f->meshes; m; m=m->next)
	{
		printf("Processing Mesh %d\n",meshNum);

		//This mesh must have something in it

		dSubmeshes[meshNum].ID=0;
		dSubmeshes[meshNum].StartVertex=meshV;
		dSubmeshes[meshNum].nVertices=m->points;
		dSubmeshes[meshNum].StartTriangle=meshT*3;
		dSubmeshes[meshNum].nTriangles=m->faces*3;
		dSubmeshes[meshNum].RootBone=0;
		dSubmeshes[meshNum].nBones=1;
		dSubmeshes[meshNum].StartBones=0;
		dSubmeshes[meshNum].Unknown=1;
		dSubmeshes[meshNum].Floats[0]=0;
		dSubmeshes[meshNum].Floats[1]=0;
		dSubmeshes[meshNum].Floats[2]=0;
		dSubmeshes[meshNum].Position[0]=0.001168764;
		dSubmeshes[meshNum].Position[0]=5.960465E-9;
		dSubmeshes[meshNum].Position[0]=0.6214849;

		//For now we are going to fake the texture unit data
		int matNum=materialNumber(m->faceL[0].material,f);
		dTextureUnits[meshNum].Flags=0x10;
		dTextureUnits[meshNum].RenderOrder=0;
		dTextureUnits[meshNum].SubmeshIndex1=meshNum;
		dTextureUnits[meshNum].SubmeshIndex2=meshNum;
		dTextureUnits[meshNum].ColorIndex=matNum;
		dTextureUnits[meshNum].RenderFlags=matNum;
		dTextureUnits[meshNum].TexUnitNumber=0;
		dTextureUnits[meshNum].Texture=findTexture(m->faceL[0].material,f);
		dTextureUnits[meshNum].TexUnitNumer2=0;
		dTextureUnits[meshNum].Transparency=matNum;
		dTextureUnits[meshNum].TexAnim=0;
		dTextureUnits[meshNum].one=1;

		meshNum++;


		for(i=0;i<m->points;i++)
		{
			dVertices[i+meshV].pos=Vec3D(m->pointL[i].pos[0],m->pointL[i].pos[1],m->pointL[i].pos[2])*0.01*.33;
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
					dSubmeshes[meshNum].ID=0;
					dSubmeshes[meshNum].StartVertex=meshV;
					dSubmeshes[meshNum].nVertices=m->points;
					dSubmeshes[meshNum].StartTriangle=meshT*3+i*3;
					//Change count numbers
					dSubmeshes[meshNum-1].nTriangles-=m->faces*3-i*3;
					dSubmeshes[meshNum].nTriangles=m->faces*3-i*3;
					dSubmeshes[meshNum].RootBone=0;
					dSubmeshes[meshNum].nBones=1;
					dSubmeshes[meshNum].StartBones=0;
					dSubmeshes[meshNum].Unknown=1;
					dSubmeshes[meshNum].Floats[0]=0;
					dSubmeshes[meshNum].Floats[1]=0;
					dSubmeshes[meshNum].Floats[2]=0;
					dSubmeshes[meshNum].Position[0]=0.001168764;
					dSubmeshes[meshNum].Position[0]=5.960465E-9;
					dSubmeshes[meshNum].Position[0]=0.6214849;
					//For now we are going to fake the texture unit data
					dTextureUnits[meshNum].Flags=0x10;
					dTextureUnits[meshNum].ColorIndex=0;
					dTextureUnits[meshNum].RenderOrder=0;
					dTextureUnits[meshNum].SubmeshIndex1=meshNum;
					dTextureUnits[meshNum].SubmeshIndex2=meshNum;
					dTextureUnits[meshNum].RenderFlags=0;
					dTextureUnits[meshNum].TexUnitNumber=0;
					dTextureUnits[meshNum].Texture=findTexture(m->faceL[i].material,f);
					dTextureUnits[meshNum].TexUnitNumer2=0;
					dTextureUnits[meshNum].Transparency=0;
					dTextureUnits[meshNum].TexAnim=0;
					dTextureUnits[meshNum].one=1;

					meshNum++;
				}
			}
			dTriangles[(i+meshT)].Indices[0]=(uint16)m->faceL[i].points[0]+meshV;
			dTriangles[(i+meshT)].Indices[1]=(uint16)m->faceL[i].points[1]+meshV;
			dTriangles[(i+meshT)].Indices[2]=(uint16)m->faceL[i].points[2]+meshV;

		}
		meshV+=m->points;
		meshT+=m->faces;
	}
}

void convertTexture(){
	if(nTextures!=0){
	tex.setnTextures(nTextures);
	for(int i=0;i<nTextures;i++){
		tex.setTexture(dTextures[i],i);
		tex.Texnames[i]=(char *)textureNames[i].c_str();
	}
	}
	else{
		int j=0;
		printf("How many Textures do you want?\n");
		scanf("%d",&j);
		tex.setnTextures(j);
		for(int i=0;i<j;i++){
			printf("Processing Texture: %d\n",i);
			ModelTextureDef texa;
			printf("Please enter the texflag:\n");
			scanf("%d",&texa.flags);
			printf("Please enter the texture type:\n");
			scanf("%d",&texa.type);
			if(texa.type==0){
				char newName[255];
				printf("Please insert the new texname:\n");
				scanf ("%s",newName);
				texa.nameLen=strlen(newName)+1;
				tex.setTexture(texa,i);
				for(int k=0;k<texa.nameLen;k++){
					tex.Texnames[i][k]=newName[k];
				}
			}
			else{
				texa.nameLen=1;
				tex.setTexture(texa,i);
			}
		}
	}
}

void createDummyAnimation(){
	nAnimation=1;
	dAnim=new ModelAnimation[nAnimation];
	dAnim[0].Index=0;
	dAnim[0].NextAnimation=-1;
	dAnim[0].animID=0;
	dAnim[0].boxA.x=0;
	dAnim[0].boxA.y=0;
	dAnim[0].boxA.z=0;
	dAnim[0].boxB.x=0;
	dAnim[0].boxB.y=0;
	dAnim[0].boxB.z=0;
	dAnim[0].d1=0;
	dAnim[0].d2=0;
	dAnim[0].flags=0x7FFF;
	dAnim[0].length=0;
	dAnim[0].playSpeed=0;
	dAnim[0].loopType=32;
	dAnim[0].moveSpeed=0;
	dAnim[0].subAnimID=0;
	dAnim[0].rad=0;

	nBones=1;
	dBone=new ModelBoneDef[nBones];
	dBone[0].animid=-1;
	dBone[0].flags=0;
	dBone[0].parent=-1;
	dBone[0].pivot.x=0;
	dBone[0].pivot.y=0;
	dBone[0].pivot.z=0;
	dBone[0].geoid=0;
	dBone[0].unk=0;
	dBone[0].rotation.seq=-1;
	dBone[0].rotation.type=0;
	dBone[0].scaling.seq=-1;
	dBone[0].scaling.type=0;
	dBone[0].translation.seq=-1;
	dBone[0].translation.type=0;
	bon=new BoneHelper[nBones];
	bon[0].setnAnim(1);
	QuatHelper *q=new QuatHelper[1];
	Vec3Helper *v=new Vec3Helper[1];
	q[0].timestamp=0;
	q[0].value.w=0;
	q[0].value.x=0;
	q[0].value.y=0;
	q[0].value.z=0;
	v[0].timestamp=0;
	v[0].value.x=0;
	v[0].value.y=0;
	v[0].value.z=0;
	bon[0].info[0].setRotation(q,1);
	bon[0].info[0].setScaling(v,1);
	bon[0].info[0].setTranslation(v,1);
}

int main(int argc, char **argv)
{
	_TF("3dstom2.log");
	_Tn("main");
	printf("3ds To M2 Converter v3.0 (July 6th, 2009)\n");
	printf("v2.0 (July 29th, 2007)\n");
	printf(" (Original Version July 28rd, 2006)\n");
	printf("-by John \"Cryect\" Rittenhouse\n");
	printf("-fixed by schlumpf(v2.0)\n");
	printf("-updatet and completely remodeled by Tigurius[Taliis]\n");
	Lib3dsFile *f=0;
	FILE	*outputM2,*outputSkin;
	ModelEditor editor_m=ModelEditor();
	SkinEditor editor_s=SkinEditor();

	if(argc<3)
	{
		printf(" 3dsToM2 <3ds File Name> <M2 File Name> <Skin File Name>[<Optional Scale>]\n");
		return 0;
	}

	f=lib3ds_file_load(argv[1]);

	if(!f)
	{
		printf("ERROR: Unable to load 3ds file %s\n",argv[1]);
		return 0;
	}

	outputM2=fopen(argv[2],"wb+");
	outputSkin=fopen(argv[3],"wb+");
	if(!outputM2||!outputSkin)
	{
		printf("ERROR: Unable to open M2- or Skin-file for writing %s\n",argv[2]);
		lib3ds_file_free(f);
		return 0;
	}
	float Scale=1;
	if(argc>4)
		Scale=atof(argv[4]);



	load3ds(f);

	convertTexture();
	createDummyAnimation();

	printf("Creating new M2\n");
	editor_m.setModelName("Tigurius",8);
	editor_m.newVertices(dVertices,nVertices);
	editor_m.newAnimations(dAnim,nAnimation);
	editor_m.newAnimationLookup(nAnimation);
	editor_m.newBones(dBone,nBones,bon);
	editor_m.newBoneLookup();
	editor_m.newKeyBoneLookup();
	editor_m.newTextures(tex);
	editor_m.newTexLookup();
	editor_m.newTexAnimLookup();
	editor_m.newTexUnitLookup();
	editor_m.newTexReplace();
	editor_m.newRenderflags(renderFlags,nMaterials);
	editor_m.newColors(col);
	editor_m.newTransparency(trans);
	editor_m.newTransparencyLookup();
	editor_m.ResizeModel(Scale);

	printf("Saving M2\n");
	char *output=editor_m.getFile();
	int OutputSize=editor_m.GetFileSize();
	fwrite(output,OutputSize,1,outputM2);
	fclose(outputM2);

	printf("Creating new Skin\n");
	editor_s.newIndices(nVertices);
	printf("Created new Vertices\n");
	editor_s.newProperties(nVertices);
	printf("Created new Properties\n");
	editor_s.newTriangles(dTriangles,nTriangles);
	printf("Created new Triangles\n");
	editor_s.newTexUnits(dTextureUnits,nTextureUnits);
	printf("Created new TexUnits\n");
	editor_s.newSubmeshes(dSubmeshes,nSubmeshes);
	printf("Created new Submeshes\n");


	printf("Saving skin\n");
	output=editor_s.getFile();
	OutputSize=editor_s.GetFileSize();
	fwrite(output,OutputSize,1,outputSkin);
	fclose(outputSkin);
	lib3ds_file_free(f);

	printf("3ds to M2 conversion completed\n");
}
