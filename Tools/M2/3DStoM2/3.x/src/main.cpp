#include "matrix.h"

#include "lib3ds\lib3ds.h"


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
int nTrans=0;;
AnimationHelper<short> *trans;
ModelRenderFlags	*renderFlags;

int nAnimation;
ModelAnimation *dAnim;

BoneHelper bone;

BoundVolumeHelper bounds;

int	findTexture(const char *MaterialName, Lib3dsFile *f)
{
	int i;
	Lib3dsMaterial **mat=f->materials;

	for (int j=0;j<f->nmaterials; j++){
		if(strcmp(mat[j]->name,MaterialName)==0)
			break;
	if(mat==0)
		return 0;
	for(i=0;i<textureNames.size();i++)
		if(strcmp(textureNames[i].c_str(),mat[j]->texture1_map.name)==0)
			return i;
	}
	return 0;//default to 0 in case invalid
}

int	materialNumber(char *matName,Lib3dsFile *f)
{
	int i;
	Lib3dsMaterial **mat=f->materials;
	i=0;
	for (int j=0;j<f->nmaterials; j++)
	{
		if(strcmp(mat[j]->name,matName)==0)
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

	if(mat->is_additive)
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
	_Tn("void extractMaterialData(Lib3dsFile *f)");
	Lib3dsMaterial **mat=f->materials;
	int i;
	bool	noTextures;

	//First lets count how many materials and store the texture names
	nMaterials=f->nmaterials;
	for (int j=0;j<nMaterials;j++)
	{
		for(i=0;i<textureNames.size();i++)
			if(strcmp(textureNames[i].c_str(),mat[j]->texture1_map.name)==0)
				break;
		if(i==textureNames.size())
			textureNames.push_back(std::string(mat[j]->texture1_map.name));
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
	trans=new AnimationHelper<short>[nMaterials];
	nTrans=nMaterials;
	renderFlags=new ModelRenderFlags[nMaterials];

	//Now Load up the data from each of the materials
	if(noTextures)
	{
		col.color[0].nAnims=1;
		col.color[0].GlobalSequence=-1;
		col.color[0].Interpolation=0;
		col.color[0].helper=new AnimBlockHelper<Vec3D>[1];
		col.opacity[0].nAnims=1;
		col.opacity[0].GlobalSequence=-1;
		col.opacity[0].Interpolation=0;
		col.opacity[0].helper=new AnimBlockHelper<short>[1];

		trans[0].Interpolation=0;
		trans[0].GlobalSequence=-1;
		trans[0].nAnims=1;
		trans[0].helper=new AnimBlockHelper<short>[1];
		AnimHelper<Vec3D> *c=new AnimHelper<Vec3D>[1];
		AnimHelper<short> *t=new AnimHelper<short>[1];
		t[0].timestamp=0;
		t[0].value=32767;
		c[0].timestamp=0;
		c[0].value.x=1.0f;
		c[0].value.y=1.0f;
		c[0].value.z=1.0f;
		col.color[0].helper[0].inAnim=false;
		col.color[0].helper[0].nValues=1;
		col.color[0].helper[0].help=c;
		col.opacity[0].helper[0].inAnim=false;
		col.opacity[0].helper[0].nValues=1;
		col.opacity[0].helper[0].help=t;
		trans[0].helper[0].inAnim=false;
		trans[0].helper[0].nValues=1;
		trans[0].helper[0].help=t;
		renderFlags[0].blend=0;
		renderFlags[0].flags=0;
	}
	else
	{
		for (int j=0;j<nMaterials;j++)
		{
			col.color[j].nAnims=1;
			col.color[j].GlobalSequence=-1;
			col.color[j].Interpolation=0;
			col.color[j].helper=new AnimBlockHelper<Vec3D>[1];
			col.opacity[0].nAnims=1;
			col.opacity[0].GlobalSequence=-1;
			col.opacity[0].Interpolation=0;
			col.opacity[0].helper=new AnimBlockHelper<short>[1];

			trans[j].Interpolation=0;
			trans[j].GlobalSequence=-1;
			trans[j].nAnims=1;
			trans[j].helper=new AnimBlockHelper<short>[1];
			AnimHelper<Vec3D> *c=new AnimHelper<Vec3D>[1];
			AnimHelper<short> *t=new AnimHelper<short>[1];
			t[0].timestamp=0;
			t[0].value=32767;
			c[0].timestamp=0;
			c[0].value.x=mat[j]->diffuse[0];
			c[0].value.y=mat[j]->diffuse[1];
			c[0].value.z=mat[j]->diffuse[2];
			t[0].value=(short)(32767*mat[j]->diffuse[3]);
			col.color[0].helper[0].inAnim=false;
			col.color[0].helper[0].nValues=1;
			col.color[0].helper[0].help=c;
			col.opacity[0].helper[0].inAnim=false;
			col.opacity[0].helper[0].nValues=1;
			col.opacity[0].helper[0].help=t;
			t[0].value=(short)(32767*(1.0-mat[j]->transparency));
			trans[0].helper[0].inAnim=false;
			trans[0].helper[0].nValues=1;
			trans[0].helper[0].help=t;
			renderFlags[j]=setRenderFlag(mat[j]);
		}
	}
}

void load3ds(Lib3dsFile *f)
{
	_Tn("void load3ds(Lib3dsFile *f)");
	int i,meshV,meshT,meshNum;
	Lib3dsMesh **m=f->meshes;
	//First thing need to count all the vertices and triangles;
	nVertices=0;
	nTriangles=0;
	nSubmeshes=f->nmeshes;
	printf("Loading 3ds File\n");


	extractMaterialData(f);

	for (int j=0;j<nSubmeshes;j++)
	{
		nVertices+=m[j]->nvertices;
		nTriangles+=m[j]->nfaces;
	}
	printf("Vertices: %d Triangles: %d Submeshes: %d\n",nVertices,nTriangles,nSubmeshes);
	dVertices=new ModelVertex[nVertices];
	bounds.nBVertices=nVertices;
	Vec3D *bVertices=new Vec3D[nVertices];
	dTriangles=new Triangle[nTriangles];
	bounds.nBTriangles=nTriangles;
	ModelBoundTriangle *bTriangles=new ModelBoundTriangle[nTriangles];
	bounds.nBNormals=nTriangles;
	Vec3D *bNormals=new Vec3D[nTriangles];
	dSubmeshes=new Submesh[nSubmeshes];
	nTextureUnits=nSubmeshes;
	dTextureUnits=new TexUnit[nTextureUnits];

	//Load initial vertex & triangle data
	meshV=0;
	meshT=0;
	meshNum=0;
	Matrix meshMat;
	for (int j=0;j<nSubmeshes;j++)
	{
		printf("Processing Mesh %d\n",meshNum);

		//This mesh must have something in it

		dSubmeshes[meshNum].ID=0;
		dSubmeshes[meshNum].StartVertex=meshV;
		dSubmeshes[meshNum].nVertices=m[j]->nvertices;
		dSubmeshes[meshNum].StartTriangle=meshT*3;
		dSubmeshes[meshNum].nTriangles=m[j]->nfaces*3;
		dSubmeshes[meshNum].RootBone=0;
		dSubmeshes[meshNum].nBones=1;
		dSubmeshes[meshNum].StartBones=0;
		dSubmeshes[meshNum].Unknown=1;
		dSubmeshes[meshNum].Floats[0]=0;
		dSubmeshes[meshNum].Floats[1]=0;
		dSubmeshes[meshNum].Floats[2]=0;
		//well, I would set this values to 0 0 0
		//but cryect had set them to the followings..
		dSubmeshes[meshNum].Position[0]=0.001168764;
		dSubmeshes[meshNum].Position[0]=5.960465E-9;
		dSubmeshes[meshNum].Position[0]=0.6214849;

		//For now we are going to fake the texture unit data
		int matNum=m[j]->faces[0].material;
		dTextureUnits[meshNum].Flags=0x10;
		dTextureUnits[meshNum].RenderOrder=0;
		dTextureUnits[meshNum].SubmeshIndex1=meshNum;
		dTextureUnits[meshNum].SubmeshIndex2=meshNum;
		dTextureUnits[meshNum].ColorIndex=matNum;
		dTextureUnits[meshNum].RenderFlags=matNum;
		dTextureUnits[meshNum].TexUnitNumber=0;
		dTextureUnits[meshNum].Texture=m[j]->faces[0].material;
		dTextureUnits[meshNum].TexUnitNumer2=0;
		dTextureUnits[meshNum].Transparency=matNum;
		dTextureUnits[meshNum].TexAnim=0;
		dTextureUnits[meshNum].one=1;

		meshNum++;

		printf("Processing Vertices\n");
		for(i=0;i<m[j]->nvertices;i++)
		{
			bVertices[i+meshV]=Vec3D(m[j]->vertices[i][0],m[j]->vertices[i][1],m[j]->vertices[i][2])*0.01*.33;

			dVertices[i+meshV].pos=Vec3D(m[j]->vertices[i][0],m[j]->vertices[i][1],m[j]->vertices[i][2])*0.01*.33;
			dVertices[i+meshV].texcoords=Vec2D(m[j]->texcos[i][0],m[j]->texcos[i][1]);

			dVertices[i+meshV].texcoords[1]=1-dVertices[i+meshV].texcoords[1];
			for(int k=0;k<4;k++)
			{
				dVertices[i+meshV].bones[k]=0;
				dVertices[i+meshV].weights[k]=0;
			}
			dVertices[i+meshV].weights[0]=255;
			dVertices[i+meshV].unk1=0;
			dVertices[i+meshV].unk2=0;
			//Calculate Normal
			/*Vec3D Normal;
			Normal=Vec3D(0,0,0);
			for(int k=0;k<m[j]->nfaces;k++)
			{
				if((m[j]->vertices[i][0]==i)||(m[j]->vertices[i][1]==i)||(m[j]->vertices[i][2]==i))
					Normal+=Vec3D(normals[3*j][0],normals[3*j][1],normals[3*j][2]);
			}
			Normal.normalize();*/
			//defacto::don't use it atm, it returns strange values oO
			//dVertices[i+meshV].normal=Normal;
		}
		printf("Processing Faces\n");
		float (*normals)[3] =new float[m[j]->nfaces][3];
		lib3ds_mesh_calculate_face_normals(m[j],normals);
		for(i=0;i<m[j]->nfaces;i++){
			bNormals[i+meshT]=Vec3D(normals[i+meshT][0],normals[i+meshT][1],normals[i+meshT][2]);
		}
		for(i=0;i<m[j]->nfaces;i++)
		{
			if(i!=0)
			{
				if(m[j]->faces[i-1].material!=m[j]->faces[i].material)
				{
					//This mesh must have something in it
					dSubmeshes[meshNum].ID=0;
					dSubmeshes[meshNum].StartVertex=meshV;
					dSubmeshes[meshNum].nVertices=m[j]->nvertices;
					dSubmeshes[meshNum].StartTriangle=meshT*3+i*3;
					//Change count numbers
					dSubmeshes[meshNum-1].nTriangles-=m[j]->nfaces*3-i*3;
					dSubmeshes[meshNum].nTriangles=m[j]->nfaces*3-i*3;
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
					dTextureUnits[meshNum].Texture=m[j]->faces[i].material;
					dTextureUnits[meshNum].TexUnitNumer2=0;
					dTextureUnits[meshNum].Transparency=0;
					dTextureUnits[meshNum].TexAnim=0;
					dTextureUnits[meshNum].one=1;

					meshNum++;
				}
			}
			dTriangles[(i+meshT)].Indices[0]=m[j]->faces[i].index[0];
			dTriangles[(i+meshT)].Indices[1]=m[j]->faces[i].index[1];
			dTriangles[(i+meshT)].Indices[2]=m[j]->faces[i].index[2];

			bTriangles[i+meshT].index[0]=m[j]->faces[i].index[0];
			bTriangles[i+meshT].index[1]=m[j]->faces[i].index[1];
			bTriangles[i+meshT].index[2]=m[j]->faces[i].index[2];
		}
		meshV+=m[j]->nvertices;
		meshT+=m[j]->nfaces;
	}
	bounds.BNormals=bNormals;
	bounds.BTriangles=bTriangles;
	bounds.BVertices=bVertices;
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

	int nBones=1;

	ModelBoneDef *dBone=new ModelBoneDef[nBones];
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
	bone=BoneHelper();
	bone.setnBones(nBones);
	bone.Bones=dBone;
	bone.setnAnims(nAnimation);

	bone.rotation=new AnimationHelper<SQuaternion>[1];
	bone.rotation[0].GlobalSequence=-1;
	bone.rotation[0].Interpolation=0;
	bone.rotation[0].nAnims=0;
	bone.translation=new AnimationHelper<Vec3D>[1];
	bone.translation[0].GlobalSequence=-1;
	bone.translation[0].Interpolation=0;
	bone.translation[0].nAnims=0;
	bone.scaling=new AnimationHelper<Vec3D>[1];
	bone.scaling[0].GlobalSequence=-1;
	bone.scaling[0].Interpolation=0;
	bone.scaling[0].nAnims=0;
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

	f=lib3ds_file_open(argv[1]);

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


	bounds=BoundVolumeHelper();
	bounds.nBNormals=0;
	bounds.nBTriangles=0;
	bounds.nBVertices=0;

	load3ds(f);
	createDummyAnimation();
	convertTexture();


	printf("Creating new M2\n");
	editor_m.setModelName("Tigurius",8);
	editor_m.newVertices(dVertices,nVertices);
	editor_m.newAnimations(dAnim,nAnimation);
	editor_m.newAnimationLookup(nAnimation);
	editor_m.newBones(bone);
	editor_m.newBoneLookup();
	editor_m.newKeyBoneLookup();
	editor_m.newTextures(tex);
	editor_m.newTexLookup();
	editor_m.newTexAnimLookup();
	editor_m.newTexUnitLookup();
	editor_m.newTexReplace();
	editor_m.newRenderflags(renderFlags,nMaterials);
	editor_m.newColors(col);
	editor_m.newTransparency(trans,nTrans);
	editor_m.newTransparencyLookup();
	editor_m.newBounds(bounds);
	editor_m.ResizeModel(Scale);

	printf("Saving M2\n");
	char *output=editor_m.getFile();
	int OutputSize=editor_m.GetFileSize();
	fwrite(output,OutputSize,1,outputM2);
	fclose(outputM2);

	editor_s.newIndices(nVertices);
	editor_s.newProperties(nVertices);
	editor_s.newTriangles(dTriangles,nTriangles);
	editor_s.newTexUnits(dTextureUnits,nTextureUnits);
	editor_s.newSubmeshes(dSubmeshes,nSubmeshes);


	printf("Saving skin\n");
	output=editor_s.getFile();
	OutputSize=editor_s.GetFileSize();
	fwrite(output,OutputSize,1,outputSkin);
	fclose(outputSkin);
	lib3ds_file_free(f);

	printf("3ds to M2 conversion completed\n");
}
