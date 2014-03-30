/*
 * modeleditor.cpp
 *
 *  Created on: 25.05.2009
 *      Author: Bastian
 */
#include "modeleditor.h"

ModelEditor::ModelEditor(char *m, int FileSize) {
	_Tn("File-Konstruktor");
	f = m;
	this->FileSize = FileSize;
	SetStructure();
}

ModelEditor::ModelEditor() {
	_Tn("()-Konstruktor");
	f = SMemNew(sizeof(ModelHeader));
	this->FileSize = sizeof(ModelHeader);
	for (int i = 0; i < FileSize; i++) {
		f[i] = 0;
	}
	SetStructure();
	header_m->id[0] = 'M';
	header_m->id[1] = 'D';
	header_m->id[2] = '2';
	header_m->id[3] = '0';
	header_m->version[0] = 0x08;
	header_m->version[1] = 0x01;
	header_m->version[2] = 0x00;
	header_m->version[3] = 0x00;
	header_m->nViews = 1;

}

ModelEditor::~ModelEditor() {

}

/**
 *This is used to  set the mainstructs of the file
 *Very usefull ;P
 *and call it as often as needed(aka f has changed!)
 **/
void ModelEditor::SetStructure() {
	_TDn("SetStructure()");
	header_m = (ModelHeader *) (f);
	if(header_m->Flags&Global_AdditionalField)
		offsetunk_m=(ofsUnk *)(f+sizeof(ModelHeader));

	globaltimestamps_m = (uint32 *) (f + header_m->ofsGlobalSequences);
	anim_m = (ModelAnimation *) (f + header_m->ofsAnimations);
	animlookup_m = (int16 *) (f + header_m->ofsAnimationLookup);
	bone_m = (ModelBoneDef *) (f + header_m->ofsBones);
	bonelookup_m = (uint16*) (f + header_m->ofsBoneLookupTable);
	keybonelookup_m = (uint16*) (f + header_m->ofsKeyBoneLookup);

	vertex_m = (ModelVertex *) (f + header_m->ofsVertices);
	renderflags_m = (ModelRenderFlags *) (f + header_m->ofsRenderFlags);
	texunitlookup_m = (int16 *) (f + header_m->ofsTexUnitLookup);
	texreplacelookup_m = (uint16 *) (f + header_m->ofsTexReplace);
	color_m = (ModelColorDef *) (f + header_m->ofsColors);
	translookup_m = (uint16 *) (f + header_m->ofsTransparencyLookup);
	trans_m = (ModelTransDef *) (f + header_m->ofsTransparency);
	texturelookup_m = (uint16 *) (f + header_m->ofsTexLookup);
	texture_m = (ModelTextureDef *) (f + header_m->ofsTextures);
	texanimlookup_m = (uint16*) (f + header_m->ofsTexAnimLookup);
	texanim_m = (ModelTexAnimDef *) (f + header_m->ofsTexAnims);

	part_m = (ModelParticleEmitterDef *) (f + header_m->ofsParticleEmitters);
	ribbon_m = (ModelRibbonEmitterDef *) (f + header_m->ofsRibbonEmitters);

	boundvertices_m = (Vec3D *) (f + header_m->ofsBoundingVertices);
	boundtriangle_m = (ModelBoundTriangle *) (f
			+ header_m->ofsBoundingTriangles);
	boundnormals_m = (Vec3D *) (f + header_m->ofsBoundingNormals);
	light_m = (ModelLightDef *) (f + header_m->ofsLights);
	cam_m = (ModelCameraDef *) (f + header_m->ofsCameras);
	camlookup_m = (uint16 *) (f + header_m->ofsCameraLookup);
	attach_m = (ModelAttachmentDef *) (f + header_m->ofsAttachments);
	attachlookup_m = (uint16*) (f + header_m->ofsAttachLookup);
	attach2_m = (ModelAttach2 *) (f + header_m->ofsAttachments_2);

}

int ModelEditor::GetFileSize() {
	return FileSize;
}

char * ModelEditor::getFile() {
	return f;
}

/**
 *This fills up the file to 0x10 bytes
 *so we get a nice filestruct :D
 **/
void ModelEditor::FillLine() {
	_TDn("FillLine()");
	int newSize = FileSize + (16 - ((FileSize) % 16));
	char *n = f;
	f=SMemNew(newSize);
	memcpy(f, n, FileSize);
	FileSize = newSize;
	SetStructure();
}

void ModelEditor::setModelName(char *name, int length) {
	int newSize = FileSize + length;
	char*n = f;
	f = new char[newSize];
	memcpy(f, n, FileSize);
	memcpy(f + FileSize, name, length);
	SetStructure();
	header_m->nameOfs = FileSize;
	header_m->nameLength = length;
	FillLine();
}

AnimationHelper<short> *ModelEditor::getTransparency() {
	_Tn("AnimationHelper<short>  *ModelEditor::getTransparency()");
	AnimationHelper<short> *ret =
			new AnimationHelper<short> [header_m->nTransparency];
	for (int i = 0; i < header_m->nTransparency; i++) {
		ret[i] = readAnimation<short> (trans_m[i].trans);
	}
	return ret;
}

int ModelEditor::getnTransparency() {
	_Tn("int ModelEditor::getnTransparency()");
	return header_m->nTransparency;
}

void ModelEditor::newTransparency(AnimationHelper<short> *help, int nTrans) {
	_Tn("void ModelEditor::newTransparency(AnimationHelper<short> *help,int nTrans)");
	int newSize = FileSize + sizeof(AnimationBlock) * nTrans;
	char *n = f;
	f = SMemNew(newSize);
	memcpy(f, n, FileSize);
	SetStructure();
	header_m->ofsTransparency = FileSize;
	header_m->nTransparency = nTrans;
	FileSize = newSize;
	FillLine();
	for (int i = 0; i < nTrans; i++) {
		writeAnimation(&(trans_m[i].trans), help[i]);
	}
}

void ModelEditor::newTransparencyLookup() {
	_Tn("newTransparencyLookup()");
	FillLine();
	int newSize = FileSize + header_m->nTransparency * sizeof(short);
	char *n = f;
	f = SMemNew(newSize);
	memcpy(f, n, FileSize);
	SetStructure();
	header_m->nTransparencyLookup = header_m->nTransparency;
	header_m->ofsTransparencyLookup = FileSize;
	printf("Setted Offsets\n");
	SetStructure();
	FileSize = newSize;
	for (int i = 0; i < header_m->nTransparencyLookup; i++) {
		translookup_m[i] = i;
	}
	FillLine();
}

BoneHelper ModelEditor::getBones(bool *inAnim, int nAnims) {
	_Tn("getBones");
	BoneHelper ret = BoneHelper();
	ret.setnBones(header_m->nBones);
	ret.setnAnims(nAnims);
	ret.Bones = bone_m;
	ret.inAnim = inAnim;
	printf("%d Bones\n", header_m->nBones);
	for (int i = 0; i < header_m->nBones; i++) {
		ret.rotation[i] = readAnimation<SQuaternion> (bone_m[i].rotation,
				inAnim);
		ret.translation[i] = readAnimation<Vec3D> (bone_m[i].translation,
				inAnim);
		ret.scaling[i] = readAnimation<Vec3D> (bone_m[i].scaling, inAnim);
	}
	return ret;
}
void ModelEditor::newBones(BoneHelper help) {
	_Tn("newBones");
	int newSize = FileSize + sizeof(ModelBoneDef) * help.nBones;
	char *n = f;
	f = SMemNew(newSize);
	memcpy(f, n, FileSize);
	SetStructure();
	header_m->ofsBones = FileSize;
	header_m->nBones = help.nBones;
	memcpy(f + FileSize, help.Bones, help.nBones * sizeof(ModelBoneDef));
	FileSize = newSize;
	FillLine();
	for (int i = 0; i < header_m->nBones; i++) {
		printf("Processing Bone %d\n", i);
		writeAnimation(&(bone_m[i].rotation), help.rotation[i]);
		writeAnimation(&(bone_m[i].translation), help.translation[i]);
		writeAnimation(&(bone_m[i].scaling), help.scaling[i]);
	}
}

uint32 *ModelEditor::getGlobalSequences() {
	return globaltimestamps_m;
}

int ModelEditor::getnGlobalSequences() {
	return header_m->nGlobalSequences;
}

void ModelEditor::newGlobalSequences(uint32 *sequ, int nSequ) {
	_Tn("newGlobalSequences()");
	FillLine();
	int newSize = FileSize + sizeof(uint32) + nSequ;
	char *n = f;
	f = SMemNew(newSize);
	memcpy(f, n, FileSize);
	SetStructure();
	header_m->ofsGlobalSequences = FileSize;
	header_m->nGlobalSequences = nSequ;
	memcpy(f + FileSize, sequ, sizeof(uint32) * nSequ);
	FileSize = newSize;
	FillLine();
}

ModelVertex *ModelEditor::getVertices() {
	return vertex_m;
}

int ModelEditor::getnVertices() {
	return header_m->nVertices;
}

ModelRenderFlags *ModelEditor::getRenderflags() {
	return renderflags_m;
}
int ModelEditor::getnRenderflags() {
	return header_m->nRenderFlags;
}

void ModelEditor::newRenderflags(ModelRenderFlags *flags, int num) {
	_Tn("new Renderflags");
	FillLine();
	int newSize = FileSize + sizeof(ModelRenderFlags) * num;
	char *n = f;
	f = SMemNew(newSize);
	memcpy(f, n, FileSize);
	SetStructure();
	header_m->ofsRenderFlags = FileSize;
	header_m->nRenderFlags = num;
	memcpy(f + FileSize, flags, num * sizeof(ModelRenderFlags));
	FileSize = newSize;
	FillLine();
}

ColorHelper ModelEditor::getColor() {
	_Tn("getColor()");
	ColorHelper ret = ColorHelper();
	ret.setnColors(header_m->nColors);
	for (int i = 0; i < header_m->nColors; i++) {
		ret.color[i] = readAnimation<Vec3D> (color_m[i].color);
		ret.opacity[i] = readAnimation<short> (color_m[i].opacity);
	}
	return ret;
}

void ModelEditor::newColors(ColorHelper help) {
	_Tn("newColors()");
	int newSize = FileSize + sizeof(ModelColorDef) * help.nColors;
	char *n = f;
	f = SMemNew(newSize);
	memcpy(f, n, FileSize);
	SetStructure();
	header_m->ofsColors = FileSize;
	header_m->nColors = help.nColors;
	FileSize = newSize;
	FillLine();
	for (int i = 0; i < help.nColors; i++) {
		writeAnimation(&(color_m[i].color), help.color[i]);
		writeAnimation(&(color_m[i].opacity), help.opacity[i]);
	}
	FillLine();
}

TextureHelper ModelEditor::getTextures() {
	_Tn("GetTextures");
	TextureHelper help = TextureHelper();
	help.setnTextures(header_m->nTextures);
	for (int i = 0; i < header_m->nTextures; i++) {
		help.setTexture(texture_m[i], i);
		char *name = getAnimblockData(name, texture_m[i].nameOfs);
		for (int k = 0; k < texture_m[i].nameLen; k++) {
			help.Texnames[i][k] = name[k];
		}
	}
	return help;

}

void ModelEditor::newTexAnim(TexAnimHelper help) {
	_Tn("newTexAnim");
	int newSize = FileSize + sizeof(ModelTexAnimDef) * help.nTexAnims;
	char *n = f;
	f = SMemNew(newSize);
	memcpy(f, n, FileSize);
	memcpy(f + FileSize, help.TexAnims, sizeof(ModelTexAnimDef)
			* help.nTexAnims);
	SetStructure();
	header_m->ofsTexAnims = FileSize;
	header_m->nTexAnims = help.nTexAnims;
	FileSize = newSize;
	FillLine();
	for (int i = 0; i < header_m->nTexAnims; i++) {
		writeAnimation(&(texanim_m[i].trans), help.trans[i]);
		writeAnimation(&(texanim_m[i].rot), help.rot[i]);
		writeAnimation(&(texanim_m[i].scale), help.scaling[i]);
	}
	FillLine();

}

TexAnimHelper ModelEditor::getTexAnim() {
	_Tn("getTexAnim()");
	TexAnimHelper ret = TexAnimHelper();
	ret.setnTexAnims(header_m->nTexAnims);
	ret.TexAnims = texanim_m;
	for (int i = 0; i < header_m->nTexAnims; i++) {
		ret.trans[i] = readAnimation<Vec3D> (texanim_m[i].trans);
		ret.rot[i] = readAnimation<SQuaternion> (texanim_m[i].rot);
		ret.scaling[i] = readAnimation<Vec3D> (texanim_m[i].scale);
	}
	return ret;
}


RibbonHelper ModelEditor::getRibbons() {
	_Tn("RibbonHelper ModelEditor::getRibbons()");
	RibbonHelper ret =RibbonHelper();
	ret.setnRibbons(header_m->nRibbonEmitters);
	for (int i = 0; i < header_m->nRibbonEmitters; i++) {
		ret.ribbons[i]=ribbon_m[i];
		ret.Above[i]=readAnimation<float>(ribbon_m[i].above);
		ret.Below[i]=readAnimation<float>(ribbon_m[i].below);
		ret.Color[i]=readAnimation<Vec3D>(ribbon_m[i].color);
		ret.Enabled[i]=readAnimation<bool>(ribbon_m[i].enabled);
		ret.Opacity[i]=readAnimation<short>(ribbon_m[i].opacity);
		ret.Unk1[i]=readAnimation<short>(ribbon_m[i].unk1);
	}
	return ret;
}

void ModelEditor::newRibbons(RibbonHelper help){
	_Tn("void ModelEditor::newRibbons(RibbonHelper help)");
	int newSize = FileSize + sizeof(ModelRibbonEmitterDef) * help.nRibbons;
	char *n = f;
	f = SMemNew(newSize);
	memcpy(f, n, FileSize);
	memcpy(f+FileSize,help.ribbons,sizeof(ModelRibbonEmitterDef) * help.nRibbons);
	SetStructure();
	header_m->ofsRibbonEmitters = FileSize;
	header_m->nRibbonEmitters = help.nRibbons;
	FileSize = newSize;
	FillLine();
	for (int i = 0; i < help.nRibbons; i++) {
		writeAnimation(&(ribbon_m[i].color), help.Color[i]);
		writeAnimation(&(ribbon_m[i].opacity), help.Opacity[i]);
		writeAnimation(&(ribbon_m[i].above), help.Above[i]);
		writeAnimation(&(ribbon_m[i].below), help.Below[i]);
		writeAnimation(&(ribbon_m[i].enabled), help.Enabled[i]);
		writeAnimation(&(ribbon_m[i].unk1), help.Unk1[i]);
	}
	FillLine();
}

ParticleHelper ModelEditor::getParticles() {
	_Tn("ParticleHelper ModelEditor::getParticles()");
	ParticleHelper ret = ParticleHelper();
	ret.setnParticles(header_m->nParticleEmitters);
	ret.particles = part_m;
	for (int i = 0; i < header_m->nParticleEmitters; i++) {
		char *temp = new char[part_m[i].lenModelName];
		memcpy(temp, f + part_m[i].ofsModelName, part_m[i].lenModelName);
		ret.info[i].setModelName(part_m[i].lenModelName, temp);

		temp = new char[part_m[i].lenParticleName];
		memcpy(temp, f + part_m[i].ofsParticleName, part_m[i].lenParticleName);
		ret.info[i].setParticleName(part_m[i].lenParticleName, temp);
	}
	for (int i = 0; i < header_m->nParticleEmitters; i++) {

		//the float animblocks
		{
			ret.EmissionAreaLength[i] = readAnimation<float> (
					part_m[i].EmissionAreaLength);
			ret.Emissionspeed[i] = readAnimation<float> (
					part_m[i].Emissionspeed);
			ret.SpeedVariation[i] = readAnimation<float> (
					part_m[i].SpeedVariation);
			ret.VerticalRange[i] = readAnimation<float> (
					part_m[i].VerticalRange);
			ret.HorizontalRange[i] = readAnimation<float> (
					part_m[i].HorizontalRange);
			ret.Gravity[i] = readAnimation<float> (part_m[i].Gravity);
			ret.Lifespan[i] = readAnimation<float> (part_m[i].Lifespan);
			ret.EmissionRate[i] = readAnimation<float> (part_m[i].EmissionRate);
			ret.EmissionAreaWidth[i] = readAnimation<float> (
					part_m[i].EmissionAreaWidth);
			ret.Gravity2[i] = readAnimation<float> (part_m[i].Gravity2);
			ret.Enabled[i] = readAnimation<bool> (part_m[i].Enabled);
		}

		{//now the fake animblocks
			//readin colors
			{

				FakeHelper<Vec3D> *colhelp =
						new FakeHelper<Vec3D> [part_m[i].colors.nKeys];
				short *t = getAnimblockData(t, part_m[i].colors.ofsTimes);
				Vec3D *col = getAnimblockData(col, part_m[i].colors.ofsKeys);
				for (int k = 0; k < part_m[i].colors.nKeys; k++) {
					colhelp[k].timestamp = t[k];
					colhelp[k].value = col[k];
				}
				ret.info[i].setColor(colhelp, part_m[i].colors.nKeys);
			}
			//end colors

			//readin opacity
			{
				FakeHelper<short> *shelp =
						new FakeHelper<short> [part_m[i].opacity.nKeys];
				short *t = getAnimblockData(t, part_m[i].opacity.ofsTimes);
				short *s = getAnimblockData(s, part_m[i].opacity.ofsKeys);
				for (int k = 0; k < part_m[i].opacity.nKeys; k++) {
					shelp[k].timestamp = t[k];
					shelp[k].value = s[k];
				}
				ret.info[i].setOpacity(shelp, part_m[i].opacity.nKeys);
			}
			//end opacity

			//readin sizes
			{
				FakeHelper<Vec2D> *shelp =
						new FakeHelper<Vec2D> [part_m[i].sizes.nKeys];
				short *t = getAnimblockData(t, part_m[i].sizes.ofsTimes);
				Vec2D *s = getAnimblockData(s, part_m[i].sizes.ofsKeys);
				for (int k = 0; k < part_m[i].sizes.nKeys; k++) {
					shelp[k].timestamp = t[k];
					shelp[k].value = s[k];
				}
				ret.info[i].setSize(shelp, part_m[i].sizes.nKeys);
			}
			//end sizes

			//readin intensity
			{
				FakeHelper<short> *shelp =
						new FakeHelper<short> [part_m[i].intensity.nKeys];
				short *t = getAnimblockData(t, part_m[i].intensity.ofsTimes);
				short *s = getAnimblockData(s, part_m[i].intensity.ofsKeys);
				for (int k = 0; k < part_m[i].intensity.nKeys; k++) {
					shelp[k].timestamp = t[k];
					shelp[k].value = s[k];
				}
				ret.info[i].setIntensity(shelp, part_m[i].intensity.nKeys);
			}
			//end intensity

			//readin unk
			{
				FakeHelper<short> *shelp =
						new FakeHelper<short> [part_m[i].unknownblock.nKeys];
				short *t = getAnimblockData(t, part_m[i].unknownblock.ofsTimes);
				short *s = getAnimblockData(s, part_m[i].unknownblock.ofsKeys);
				for (int k = 0; k < part_m[i].unknownblock.nKeys; k++) {
					shelp[k].timestamp = t[k];
					shelp[k].value = s[k];
				}
				ret.info[i].setUnk(shelp, part_m[i].unknownblock.nKeys);
			}
			//end unk
		}

	}
	return ret;

}

void ModelEditor::newParticles(ParticleHelper help) {
	_Tn("void ModelEditor::newParticles(ParticleHelper help) ");
	int newSize = FileSize + sizeof(ModelParticleEmitterDef) * help.nParticles;
	char *n = f;
	f = SMemNew(newSize);
	memcpy(f, n, FileSize);
	SetStructure();
	header_m->ofsParticleEmitters = FileSize;
	header_m->nParticleEmitters = help.nParticles;
	memcpy(f + FileSize, help.particles, help.nParticles
			* sizeof(ModelParticleEmitterDef));
	FileSize = newSize;
	FillLine();

	for (int i = 0; i < header_m->nParticleEmitters; i++) {
		writeAnimation(&(part_m[i].Emissionspeed), help.Emissionspeed[i]);
		writeAnimation(&(part_m[i].SpeedVariation), help.SpeedVariation[i]);
		writeAnimation(&(part_m[i].VerticalRange), help.VerticalRange[i]);
		writeAnimation(&(part_m[i].HorizontalRange), help.HorizontalRange[i]);
		writeAnimation(&(part_m[i].Gravity), help.Gravity[i]);
		writeAnimation(&(part_m[i].Lifespan), help.Lifespan[i]);
		writeAnimation(&(part_m[i].EmissionRate), help.EmissionRate[i]);
		writeAnimation(&(part_m[i].EmissionAreaLength),
				help.EmissionAreaLength[i]);
		writeAnimation(&(part_m[i].EmissionAreaWidth),
				help.EmissionAreaWidth[i]);
		writeAnimation(&(part_m[i].Gravity2), help.Gravity2[i]);
		writeAnimation(&(part_m[i].Enabled), help.Enabled[i]);
	}

	//start fake color
	{
		for (int i = 0; i < header_m->nParticleEmitters; i++) {
			part_m[i].colors.nTimes = help.info[i].nColor;
			part_m[i].colors.ofsTimes = FileSize;
			short *time = new short[help.info[i].nColor];
			for (int k = 0; k < help.info[i].nColor; k++) {
				time[k] = help.info[i].Color[k].timestamp;
			}
			writeAnimblockData(time, help.info[i].nColor);
			part_m[i].colors.nKeys = help.info[i].nColor;
			part_m[i].colors.ofsKeys = FileSize;
			Vec3D *val = new Vec3D[help.info[i].nColor];
			for (int k = 0; k < help.info[i].nColor; k++) {
				val[k] = help.info[i].Color[k].value;
			}
			writeAnimblockData(val, help.info[i].nColor);
		}
		FillLine();
	}
	//end fake color

	//start fake opacity
	{
		for (int i = 0; i < header_m->nParticleEmitters; i++) {
			part_m[i].opacity.nTimes = help.info[i].nOpacity;
			part_m[i].opacity.ofsTimes = FileSize;
			short *time = new short[help.info[i].nOpacity];
			for (int k = 0; k < help.info[i].nOpacity; k++) {
				time[k] = help.info[i].Opacity[k].timestamp;
			}
			writeAnimblockData(time, help.info[i].nOpacity);
			part_m[i].opacity.nKeys = help.info[i].nOpacity;
			part_m[i].opacity.ofsKeys = FileSize;
			short *val = new short[help.info[i].nOpacity];
			for (int k = 0; k < help.info[i].nOpacity; k++) {
				val[k] = help.info[i].Opacity[k].value;
			}
			writeAnimblockData(val, help.info[i].nOpacity);
		}
		FillLine();
	}
	//end fake opacity

	//start fake sizes
	{
		for (int i = 0; i < header_m->nParticleEmitters; i++) {
			part_m[i].sizes.nTimes = help.info[i].nSize;
			part_m[i].sizes.ofsTimes = FileSize;
			short *time = new short[help.info[i].nSize];
			for (int k = 0; k < help.info[i].nSize; k++) {
				time[k] = help.info[i].Size[k].timestamp;
			}
			writeAnimblockData(time, help.info[i].nSize);
			part_m[i].sizes.nKeys = help.info[i].nSize;
			part_m[i].sizes.ofsKeys = FileSize;
			Vec2D *val = new Vec2D[help.info[i].nSize];
			for (int k = 0; k < help.info[i].nSize; k++) {
				val[k] = help.info[i].Size[k].value;
			}
			writeAnimblockData(val, help.info[i].nSize);
		}
		FillLine();
	}
	//end fake sizes

	//start fake Intensity
	{
		for (int i = 0; i < header_m->nParticleEmitters; i++) {
			part_m[i].intensity.nTimes = help.info[i].nIntensity;
			part_m[i].intensity.ofsTimes = FileSize;
			short *time = new short[help.info[i].nIntensity];
			for (int k = 0; k < help.info[i].nIntensity; k++) {
				time[k] = help.info[i].Intensity[k].timestamp;
			}
			writeAnimblockData(time, help.info[i].nIntensity);
			part_m[i].intensity.nKeys = help.info[i].nIntensity;
			part_m[i].intensity.ofsKeys = FileSize;
			short *val = new short[help.info[i].nIntensity];
			for (int k = 0; k < help.info[i].nIntensity; k++) {
				val[k] = help.info[i].Intensity[k].value;
			}
			writeAnimblockData(val, help.info[i].nIntensity);
		}
		FillLine();
	}
	//end fake Intensity

	//start fake unk
	{
		for (int i = 0; i < header_m->nParticleEmitters; i++) {
			part_m[i].unknownblock.nTimes = help.info[i].nUnk;
			part_m[i].unknownblock.ofsTimes = FileSize;
			short *time = new short[help.info[i].nUnk];
			for (int k = 0; k < help.info[i].nUnk; k++) {
				time[k] = help.info[i].Unk[k].timestamp;
			}
			writeAnimblockData(time, help.info[i].nUnk);
			part_m[i].unknownblock.nKeys = help.info[i].nUnk;
			part_m[i].unknownblock.ofsKeys = FileSize;
			short *val = new short[help.info[i].nUnk];
			for (int k = 0; k < help.info[i].nUnk; k++) {
				val[k] = help.info[i].Unk[k].value;
			}
			writeAnimblockData(val, help.info[i].nUnk);
		}
		FillLine();
	}
	//end fake unk


	//now the referenced files:
	//models
	for (int i = 0; i < help.nParticles; i++) {
		int newSize = FileSize + help.info[i].lengthModelName;
		char *n = f;
		f = SMemNew(newSize);
		memcpy(f, n, FileSize);
		SetStructure();
		part_m[i].ofsModelName = FileSize;
		memcpy(f + FileSize, help.info[i].ModelName,
				help.info[i].lengthModelName);
		FileSize = newSize;
		FillLine();
	}
	//particles
	for (int i = 0; i < help.nParticles; i++) {
		int newSize = FileSize + help.info[i].lengthParticleName;
		char *n = f;
		f = SMemNew(newSize);
		memcpy(f, n, FileSize);
		SetStructure();
		part_m[i].ofsParticleName = FileSize;
		memcpy(f + FileSize, help.info[i].ParticleName,
				help.info[i].lengthParticleName);
		FileSize = newSize;
		FillLine();
	}
}

BoundVolumeHelper ModelEditor::getBounds() {
	_Tn("BoundVolumeHelper ModelEditor::getBounds()");
	BoundVolumeHelper ret = BoundVolumeHelper();
	ret.BNormals = boundnormals_m;
	ret.nBNormals = header_m->nBoundingNormals;
	ret.BTriangles = boundtriangle_m;
	ret.nBTriangles = header_m->nBoundingTriangles;
	ret.BVertices = boundvertices_m;
	ret.nBVertices = header_m->nBoundingVertices;
	return ret;
}

void ModelEditor::newBounds(BoundVolumeHelper help) {
	_Tn("void ModelEditor::newBounds(BoundVolumeHelper help)");
	//normals
	int newSize = FileSize + sizeof(Vec3D) * help.nBNormals;
	char *n = f;
	f = new char[newSize];
	memcpy(f, n, FileSize);
	SetStructure();
	header_m->ofsBoundingNormals = FileSize;
	header_m->nBoundingNormals = help.nBNormals;
	memcpy(f + FileSize, help.BNormals, help.nBNormals * sizeof(Vec3D));
	FileSize = newSize;
	FillLine();

	//vertices
	newSize = FileSize + sizeof(Vec3D) * help.nBVertices;
	n = f;
	f = new char[newSize];
	memcpy(f, n, FileSize);
	SetStructure();
	header_m->ofsBoundingVertices = FileSize;
	header_m->nBoundingVertices = help.nBVertices;
	memcpy(f + FileSize, help.BVertices, sizeof(Vec3D) * help.nBVertices);
	FileSize = newSize;
	FillLine();

	//triangles
	newSize = FileSize + sizeof(ModelBoundTriangle) * help.nBTriangles;
	n = f;
	f = new char[newSize];
	memcpy(f, n, FileSize);
	SetStructure();
	header_m->ofsBoundingTriangles = FileSize;
	header_m->nBoundingTriangles = help.nBTriangles;
	memcpy(f + FileSize, help.BTriangles, sizeof(ModelBoundTriangle)
			* help.nBTriangles);
	FileSize = newSize;
	FillLine();

	calcBoundingBox();

}

LightHelper ModelEditor::getLights() {
	_Tn("LightHelper ModelEditor::getLights() ");
	LightHelper ret=LightHelper();
	ret.setnLights(header_m->nLights);
	for(int i=0;i<header_m->nLights;i++){
		ret.AmbientColor[i]=readAnimation<Vec3D>(light_m[i].ambColor);
		ret.AmbientIntensity[i]=readAnimation<float>(light_m[i].ambIntensity);
		ret.DiffuseColor[i]=readAnimation<Vec3D>(light_m[i].diffcolor);
		ret.DiffuseIntensity[i]=readAnimation<float>(light_m[i].diffintensity);
		ret.AttenuationEnd[i]=readAnimation<float>(light_m[i].attEnd);
		ret.AttenuationStart[i]=readAnimation<float>(light_m[i].attStart);
		ret.Enabled[i]=readAnimation<bool>(light_m[i].Enabled);
	}
	return ret;

}

void ModelEditor::newLights(LightHelper help){
	_Tn("void ModelEditor::newLights(LightHelper help)");
	int newSize = FileSize + sizeof(ModelLightDef) * help.nLights;
	char *n = f;
	f = SMemNew(newSize);
	memcpy(f, n, FileSize);
	memcpy(f+FileSize,help.lights,sizeof(ModelLightDef) * help.nLights);
	SetStructure();
	header_m->ofsLights = FileSize;
	header_m->nLights = help.nLights;
	FileSize = newSize;
	FillLine();
	for (int i = 0; i < help.nLights; i++) {
		writeAnimation(&(light_m[i].ambColor), help.AmbientColor[i]);
		writeAnimation(&(light_m[i].ambIntensity), help.AmbientIntensity[i]);
		writeAnimation(&(light_m[i].diffcolor), help.DiffuseColor[i]);
		writeAnimation(&(light_m[i].diffintensity), help.DiffuseIntensity[i]);
		writeAnimation(&(light_m[i].attEnd), help.AttenuationEnd[i]);
		writeAnimation(&(light_m[i].attStart), help.AttenuationStart[i]);
		writeAnimation(&(light_m[i].Enabled), help.Enabled[i]);
	}
	FillLine();
}

CamHelper ModelEditor::getCameras() {
	_Tn("getCameras");
	CamHelper ret = CamHelper();
	ret.setnCameras(header_m->nCameras);

	for (int i = 0; i < header_m->nCameras; i++) {
		ret.camera[i] = cam_m[i];
		ret.rotation[i] = readAnimation<Vec3D> (cam_m[i].rot);
		ret.translationtar[i] = readAnimation<Vec3D> (cam_m[i].transTarget);
		ret.translationpos[i] = readAnimation<Vec3D> (cam_m[i].transPos);
	}
	return ret;
}

void ModelEditor::newCameras(CamHelper help) {
	_Tn("newCameras");
	int newSize = FileSize + sizeof(ModelCameraDef) * help.nCameras;
	char *n = f;
	f = SMemNew(newSize);
	memcpy(f, n, FileSize);
	memcpy(f + FileSize, help.camera, sizeof(ModelCameraDef) * help.nCameras);
	SetStructure();
	header_m->ofsCameras = FileSize;
	header_m->nCameras = help.nCameras;
	FileSize = newSize;
	FillLine();
	for (int i = 0; i < header_m->nCameras; i++) {
		writeAnimation(&(cam_m[i].rot), help.rotation[i]);
		writeAnimation(&(cam_m[i].transTarget), help.translationtar[i]);
		writeAnimation(&(cam_m[i].transPos), help.translationpos[i]);
	}
	FillLine();
	//I only know about two different cameras...
	int nCamLookup = 2;
	newSize = FileSize + sizeof(short) * nCamLookup;
	n = f;
	f = SMemNew(newSize);
	memcpy(f, n, FileSize);
	SetStructure();
	header_m->ofsCameraLookup = FileSize;
	header_m->nCameraLookup = nCamLookup;
	SetStructure();
	for (int i = 0; i < header_m->nCameraLookup; i++) {
		for (int j = 0; j < header_m->nCameras; j++) {
			if (cam_m[j].id == i) {
				camlookup_m[i] = j;
				break;
			} else
				camlookup_m[i] = -1;
		}
	}
	FileSize = newSize;
	FillLine();
}

ModelAttachmentDef *ModelEditor::getAttachments() {
	return attach_m;

}
int ModelEditor::getnAttachments() {
	return header_m->nAttachments;
}
void ModelEditor::newAttachments(ModelAttachmentDef*attachments, int num) {
	_Tn("newAttachments");
	FillLine();
	int newSize = FileSize + sizeof(ModelAttachmentDef) * num;
	char *n = f;
	f = SMemNew(newSize);
	memcpy(f, n, FileSize);
	SetStructure();
	header_m->nAttachments = num;
	header_m->ofsAttachments = FileSize;
	FileSize = newSize;
	memcpy(f + header_m->ofsAttachments, attachments, num
			* sizeof(ModelAttachmentDef));
	SetStructure();
	for (int i = 0; i < header_m->nAttachments; i++) {
		//I null this because I saw no difference,
		//and without animblocks it is much easier
		attach_m[i].unk.nKeys = 0;
		attach_m[i].unk.nTimes = 0;
		attach_m[i].unk.ofsKeys = 0;
		attach_m[i].unk.ofsTimes = 0;
	}
	FillLine();
	//Now we create the lookup!

	//40 should be enough atm
	//maybe we need later more?
	int nLook = 40;
	newSize = FileSize + nLook * sizeof(short);
	n = f;
	f = SMemNew(newSize);
	memcpy(f, n, FileSize);
	SetStructure();
	header_m->nAttachLookup = nLook;
	header_m->ofsAttachLookup = FileSize;
	FileSize = newSize;
	SetStructure();
	for (int i = 0; i < header_m->nAttachLookup; i++) {
		for (int j = 0; j < header_m->nAttachments; j++) {
			if (attach_m[j].id = i) {
				attachlookup_m[i] = j;
				break;
			} else {
				attachlookup_m[i] = -1;
			}
		}
	}
	FillLine();

}
ModelAttach2 *ModelEditor::getAttachment2() {
	return attach2_m;
}

void ModelEditor::addTexture(ModelTextureDef tex, const char * texname) {
	_Tn("addTexture");
	FillLine();
	//let's calc the needed space
	int newSize = FileSize + sizeof(ModelTextureDef)
			* (header_m->nTextures + 1) +
	//(16-((sizeof(ModelTextureDef)*(header_m->nTextures+1))%16))+ //we don't need this as (sizeof(ModelTextureDef)==0x10
			tex.nameLen + (16 - (tex.nameLen) % 16) + sizeof(uint16)
			* (header_m->nTexLookup + 1) + (16 - ((sizeof(uint16)
			* (header_m->nTexLookup + 1)) % 16));
	char *n = f;
	f = SMemNew(newSize);
	memcpy(f, n, FileSize);
	SetStructure();
	memcpy(f + FileSize, (f + header_m->ofsTextures), sizeof(ModelTextureDef)
			* header_m->nTextures);
	memcpy(f + FileSize + sizeof(ModelTextureDef) * (header_m->nTextures + 1),
			texname, tex.nameLen);
	memcpy(f + FileSize + sizeof(ModelTextureDef) * (header_m->nTextures + 1)
			+ tex.nameLen + (16 - (sizeof(tex.nameLen)) % 16), (f
			+ header_m->ofsTexLookup), sizeof(short) * header_m->nTexLookup);
	header_m = (ModelHeader *) (f);

	header_m->ofsTextures = FileSize;
	texture_m = (ModelTextureDef *) (f + header_m->ofsTextures);
	texture_m[header_m->nTextures].flags = tex.flags;
	texture_m[header_m->nTextures].type = tex.type;
	texture_m[header_m->nTextures].nameLen = tex.nameLen;
	texture_m[header_m->nTextures].nameOfs = FileSize + sizeof(ModelTextureDef)
			* (header_m->nTextures + 1);

	header_m->ofsTexLookup = FileSize + sizeof(ModelTextureDef)
			* (header_m->nTextures + 1) + tex.nameLen + (16 - (tex.nameLen)
			% 16);
	texturelookup_m = (uint16 *) (f + header_m->ofsTexLookup);
	texturelookup_m[header_m->nTexLookup] = header_m->nTextures;

	header_m->nTextures++;
	header_m->nTexLookup++;

	FileSize = newSize;
	FillLine();
	SetStructure();
}

void ModelEditor::newTextures(TextureHelper help) {
	_Tn("newTextures");
	FillLine();
	int newSize = FileSize + help.nTextures * sizeof(ModelTextureDef);
	for (int i = 0; i < help.nTextures; i++) {
		help.texdefs[i].nameOfs = newSize;
		newSize += help.texdefs[i].nameLen + (16 - ((help.texdefs[i].nameLen)
				% 16));
	}
	char *n = f;
	f = SMemNew(newSize);
	memcpy(f, n, FileSize);
	SetStructure();
	header_m->ofsTextures = FileSize;
	header_m->nTextures = help.nTextures;
	texture_m = (ModelTextureDef *) (f + header_m->ofsTextures);
	for (int i = 0; i < help.nTextures; i++) {
		texture_m[i] = help.texdefs[i];
		char *texname = (char *) (f + texture_m[i].nameOfs);
		for (int k = 0; k < texture_m[i].nameLen; k++) {
			texname[k] = help.Texnames[i][k];
		}
	}
	FileSize = newSize;
	FillLine();
}

void ModelEditor::addVertices(ModelVertex * vert, int nvert) {
	_Tn("addVertices");
	FillLine();
	int newSize = FileSize + sizeof(ModelVertex)
			* (nvert + header_m->nVertices);
	char *newc = SMemNew(newSize);
	memcpy(newc, f, FileSize);
	f = newc;
	SetStructure();
	memcpy(f, (f + header_m->ofsVertices), sizeof(ModelVertex)
			* header_m->nVertices);
	memcpy(f + FileSize, vert, nvert * sizeof(ModelVertex));
	header_m->ofsVertices = FileSize;
	header_m->nVertices += nvert;
	FileSize = newSize;
	FillLine();
}

void ModelEditor::newVertices(ModelVertex * vert, int nvert) {
	_Tn("newVertices");
	FillLine();
	int newSize = FileSize + sizeof(ModelVertex) * (nvert);
	char *newc = SMemNew(newSize);
	memcpy(newc, f, FileSize);
	f = newc;
	SetStructure();
	memcpy(f + FileSize, vert, nvert * sizeof(ModelVertex));
	header_m->ofsVertices = FileSize;
	header_m->nVertices = nvert;
	FileSize = newSize;
	FillLine();
	SetStructure();
}

ModelAnimation* ModelEditor::getAnimations() {
	return anim_m;
}

int ModelEditor::getnAnimations() {
	return header_m->nAnimations;
}

/**
 *Note: This will not add any animated bones!
 *only Animationsdef will be added!
 **/
void ModelEditor::addAnimations(ModelAnimation *anim, int nanim) {
	_Tn("addAnimations");
	FillLine();
	int newSize = FileSize + sizeof(ModelAnimation) * (header_m->nAnimations
			+ nanim);
	char *n = f;
	f = SMemNew(newSize);
	memcpy(f, n, FileSize);
	SetStructure();
	memcpy(f + FileSize, (f + header_m->ofsAnimations), sizeof(ModelAnimation)
			* (header_m->nAnimations));
	memcpy(f + FileSize + sizeof(ModelAnimation) * header_m->nAnimations, anim,
			nanim * sizeof(ModelAnimation));
	header_m->ofsAnimations = FileSize;
	header_m->nAnimations += nanim;
	FileSize = newSize;
	FillLine();
	SetStructure();
}

/**
 *Note: This will not add any animated bones!
 *only Animationsdef will be added!
 **/
void ModelEditor::newAnimations(ModelAnimation *anim, int nanim) {
	_Tn("newAnimations");
	FillLine();
	int newSize = FileSize + sizeof(ModelAnimation) * (nanim);
	char *n = f;
	f = SMemNew(newSize);
	memcpy(f, n, FileSize);
	SetStructure();
	memcpy(f + FileSize, anim, nanim * sizeof(ModelAnimation));
	header_m->ofsAnimations = FileSize;
	header_m->nAnimations = nanim;
	FileSize = newSize;
	FillLine();
	SetStructure();
}

/**
 *in fact..this is bullshit oO
 *
 *it should be more like:
 *read in the animations and check for which animation they are...
 **/
void ModelEditor::addAnimationLookup(int nanim) {
	_Tn("addAnimLookup");
	FillLine();
	int newSize = FileSize + sizeof(uint16) * (header_m->nAnimationLookup
			+ nanim);
	char *n = f;
	f = SMemNew(newSize);
	memcpy(f, n, FileSize);
	SetStructure();
	memcpy(f + FileSize, (f + header_m->ofsAnimationLookup), sizeof(uint16)
			* (header_m->nAnimationLookup));
	header_m->ofsAnimationLookup = FileSize;
	animlookup_m = (int16*) (f + header_m->ofsAnimationLookup);
	for (int i = 0; i < nanim; i++) {
		//not really...
		animlookup_m[i + header_m->nAnimationLookup] = i
				+ header_m->nAnimationLookup;
	}
	header_m->nAnimationLookup += nanim;
	FileSize = newSize;
	FillLine();
	SetStructure();
}

void ModelEditor::newAnimationLookup(int nanim) {
	_Tn("newAnimationLookup");
	FillLine();
	int newSize = FileSize + sizeof(uint16) * (nanim);
	char *n = f;
	f = SMemNew(newSize);
	memcpy(f, n, FileSize);
	SetStructure();
	header_m->ofsAnimationLookup = FileSize;
	header_m->nAnimationLookup = nanim;
	animlookup_m = (int16*) (f + header_m->ofsAnimationLookup);
	for (int i = 0; i < nanim; i++) {
		for (int j = 0; j < header_m->nAnimations; j++) {
			if (anim_m[j].animID == i) {
				animlookup_m[i] = (int16) j;
				break;
			} else {
				animlookup_m[i] = -1;
			}
		}
	}
	FileSize = newSize;
	FillLine();
}

//we need an alternative for this but well xD
//it's the easiest one :D
void ModelEditor::newBoneLookup() {
	_Tn("newBoneLookup");
	int newSize = FileSize + header_m->nBones * sizeof(short);
	char *n = f;
	f = SMemNew(newSize);
	memcpy(f, n, FileSize);
	SetStructure();
	header_m->nBoneLookupTable = header_m->nBones;
	header_m->ofsBoneLookupTable = FileSize;
	FileSize = newSize;
	SetStructure();
	for (int i = 0; i < header_m->nBoneLookupTable; i++) {
		bonelookup_m[i] = i;
	}
	FillLine();
}

void ModelEditor::newTexLookup() {
	_Tn("newTexLookup");
	int newSize = FileSize + header_m->nTextures * sizeof(short);
	char *n = f;
	f = SMemNew(newSize);
	memcpy(f, n, FileSize);
	SetStructure();
	header_m->nTexLookup = header_m->nTextures;
	header_m->ofsTexLookup = FileSize;
	FileSize = newSize;
	SetStructure();
	for (int i = 0; i < header_m->nTexLookup; i++) {
		texturelookup_m[i] = i;
	}
	FillLine();
}

void ModelEditor::newTexUnitLookup() {
	_Tn("newTexUnitLookup");
	int newSize = FileSize + sizeof(short);
	char *n = f;
	f = SMemNew(newSize);
	memcpy(f, n, FileSize);
	SetStructure();
	header_m->nTexUnitLookup = 1;
	header_m->ofsTexUnitLookup = FileSize;
	FileSize = newSize;
	SetStructure();
	texunitlookup_m[0] = 0;
	FillLine();
}

void ModelEditor::newTexReplace() {
	_Tn("newTexReplace");
	int newSize = FileSize + sizeof(short);
	char *n = f;
	f = SMemNew(newSize);
	memcpy(f, n, FileSize);
	SetStructure();
	header_m->nTexReplace = 1;
	header_m->ofsTexReplace = FileSize;
	FileSize = newSize;
	SetStructure();
	texreplacelookup_m[0] = 0;
	FillLine();
}

void ModelEditor::newTexAnimLookup() {
	_Tn("newTexAnimLookup");
	if (header_m->nTexAnims != 0) {
		int newSize = FileSize + header_m->nTexAnims * sizeof(short);
		char *n = f;
		f = SMemNew(newSize);
		memcpy(f, n, FileSize);
		SetStructure();
		header_m->nTexAnimLookup = header_m->nTextures;
		header_m->ofsTexAnimLookup = FileSize;
		FileSize = newSize;
		SetStructure();
		for (int i = 0; i < header_m->nTexLookup; i++) {
			texanimlookup_m[i] = i;
		}
		FillLine();
	} else {
		int newSize = FileSize + sizeof(short);
		char *n = f;
		f = SMemNew(newSize);
		memcpy(f, n, FileSize);
		SetStructure();
		header_m->nTexAnimLookup = 1;
		header_m->ofsTexAnimLookup = FileSize;
		FileSize = newSize;
		SetStructure();
		texanimlookup_m[0] = -1;
		FillLine();
	}
}

/**
 * Just some dummy to have the keylookup in the file...
 */
void ModelEditor::newKeyBoneLookup() {
	_Tn("newKeyBoneLookup");
	int newSize = FileSize + sizeof(short);
	char *n = f;
	f = SMemNew(newSize);
	memcpy(f, n, FileSize);
	SetStructure();
	header_m->nKeyBoneLookup = 1;
	header_m->ofsKeyBoneLookup = FileSize;
	FileSize = newSize;
	SetStructure();
	keybonelookup_m[0] = -1;
	FillLine();
}

template<class T>
void ModelEditor::writeAnimation(AnimationBlock *ablock,
		AnimationHelper<T> help) {
	_TDn("void ModelEditor::writeAnimation(AnimationBlock *ablock,AnimationHelper<T>help)");
	ablock->nKeys = help.nAnims;
	ablock->nTimes = help.nAnims;
	ablock->seq = help.GlobalSequence;
	ablock->type = help.Interpolation;
	if (help.nAnims != 0) {
		ablock->ofsTimes = FileSize;
		ablock->ofsKeys = FileSize + sizeof(AnimSubStructure);
		AnimSubStructure *timesub = new AnimSubStructure[help.nAnims];
		AnimSubStructure *keysub = new AnimSubStructure[help.nAnims];
		for (int k = 0; k < help.nAnims; k++) {
			timesub[k].n = help.helper[k].nValues;
			keysub[k].n = help.helper[k].nValues;
		}
		int newSize = FileSize + 2 * sizeof(AnimSubStructure)*help.nAnims;
		char *n = f;
		f = SMemNew(newSize);
		memcpy(f, n, FileSize);
		SetStructure();
		memcpy(f + FileSize, timesub, sizeof(AnimSubStructure) * help.nAnims);
		memcpy(f + FileSize + sizeof(AnimSubStructure)*help.nAnims, keysub,
				sizeof(AnimSubStructure) * help.nAnims);
		int ofsSize = FileSize;
		FileSize = newSize;
		FillLine();
		//we need to reset the pointers whenever FileSize has changed..
		for (int j = 0; j < help.nAnims; j++) {
			//n was set before.. I think xD
			if (help.helper[j].nValues != 0 && !help.helper[j].inAnim) {
				timesub = (AnimSubStructure *) (f + ofsSize);
				timesub[j].ofs = FileSize;
				uint32 *time = new uint32[help.helper[j].nValues];
				for (int k = 0; k < help.helper[j].nValues; k++) {
					time[k] = help.helper[j].help[k].timestamp;
				}
				writeAnimblockData(time, help.helper[j].nValues);
				keysub = (AnimSubStructure *) (f + ofsSize
						+ sizeof(AnimSubStructure)*help.nAnims);
				keysub[j].ofs = FileSize;
				T *val = new T[help.helper[j].nValues];
				for (int k = 0; k < help.helper[j].nValues; k++) {
					val[k] = help.helper[j].help[k].value;
				}
				writeAnimblockData(val, help.helper[j].nValues);
			} else {
				timesub = (AnimSubStructure *) (f + ofsSize);
				timesub[j].ofs = help.helper[j].ofsTime;
				keysub = (AnimSubStructure *) (f + ofsSize
						+ sizeof(AnimSubStructure)*help.nAnims);
				keysub[j].ofs = help.helper[j].ofsKey;
			}
		}
		FillLine();
	} else {
		ablock->ofsTimes = 0;
		ablock->ofsKeys = 0;
	}
}

template<class T>
AnimationHelper<T> ModelEditor::readAnimation(AnimationBlock ablock) {
	_TDn("AnimationHelper<T> ModelEditor::readAnimation(AnimationBlock ablock)");
	AnimationHelper<T> ret;
	//nTimes should always be nKeys...
	ret.nAnims = ablock.nTimes;
	ret.GlobalSequence = ablock.seq;
	ret.Interpolation = ablock.type;
	ret.helper = new AnimBlockHelper<T> [ret.nAnims];
	AnimSubStructure *timesub;
	AnimSubStructure *keysub;
	int offset = ablock.ofsTimes;
	timesub = getAnimblockData(timesub, offset);
	offset = ablock.ofsKeys;
	keysub = getAnimblockData(keysub, offset);
	for (int j = 0; j < ablock.nTimes; j++) {
		if (timesub[j].n != 0) {
			AnimHelper<T> *values = new AnimHelper<T> [timesub[j].n];
			T *ts = getAnimblockData(ts, keysub[j].ofs);
			uint32 *tt = getAnimblockData(tt, timesub[j].ofs);
			for (int k = 0; k < timesub[j].n; k++) {
				values[k].timestamp = tt[k];
				values[k].value = ts[k];
			}
			ret.helper[j].nValues = timesub[j].n;
			ret.helper[j].help = values;
		} else {
			ret.helper[j].help=new AnimHelper<T>[0];
			ret.helper[j].nValues = timesub[j].n;
		}
		//just to make sure...
		ret.helper[j].inAnim = false;
		ret.helper[j].ofsKey = 0;
		ret.helper[j].ofsTime = 0;
	}
	return ret;
}

template<class T>
AnimationHelper<T> ModelEditor::readAnimation(AnimationBlock ablock,
		bool*inAnim) {
	AnimationHelper<T> ret;
	//nTimes should always be nKeys...
	ret.nAnims = ablock.nTimes;
	ret.GlobalSequence = ablock.seq;
	ret.Interpolation = ablock.type;
	ret.helper = new AnimBlockHelper<T> [ret.nAnims];
	AnimSubStructure *timesub;
	AnimSubStructure *keysub;
	int offset = ablock.ofsTimes;
	timesub = getAnimblockData(timesub, offset);
	offset = ablock.ofsKeys;
	keysub = getAnimblockData(keysub, offset);
	for (int j = 0; j < ablock.nTimes; j++) {
		if (timesub[j].n != 0 && !inAnim[j]) {
			AnimHelper<T> *values = new AnimHelper<T> [timesub[j].n];
			T *ts = getAnimblockData(ts, keysub[j].ofs);
			uint32 *tt = getAnimblockData(tt, timesub[j].ofs);
			for (int k = 0; k < timesub[j].n; k++) {
				values[k].timestamp = tt[k];
				values[k].value = ts[k];
			}
			ret.helper[j].nValues = timesub[j].n;
			ret.helper[j].help = values;
		} else {
			if (inAnim[j])
				ret.helper[j].inAnim = true;
			else
				ret.helper[j].inAnim = false;
			ret.helper[j].nValues = timesub[j].n;
			ret.helper[j].ofsKey = keysub[j].ofs;
			ret.helper[j].ofsTime = timesub[j].ofs;
		}

	}
	return ret;
}

/**
 * I abuse this often
 * so don't care if it's used for something else :O
 */
template<class T>
T* ModelEditor::getAnimblockData(T *data, int Offset) {
	_TDn("getAnimBlockData");
	data = (T*) (f + Offset);
	return data;
}

/**
 *This writes the data of an animblock of bones to the file
 *nTimestamps==nKeys!!!
 **/
template<class T>
int ModelEditor::writeAnimblockData(T *data, int nValues) {
	_TDn("writeAnimblockData");
	int newSize = FileSize + sizeof(T) * nValues;
	char *n = f;
	f = SMemNew(newSize);
	memcpy(f, n, FileSize);
	SetStructure();
	memcpy(f + FileSize, data, sizeof(T) * nValues);
	FileSize = newSize;
	FillLine();
	return 0;
}

void ModelEditor::calcVertexBox(){
	float max=-99999;
	float may=-99999;
	float maz=-99999;
	float mix=99999;
	float miy=99999;
	float miz=99999;
	for(int i=0;i<header_m->nVertices;i++){
		if(vertex_m[i].pos[0]>max)max=vertex_m[i].pos[0];
		if(vertex_m[i].pos[1]>may)may=vertex_m[i].pos[1];
		if(vertex_m[i].pos[2]>maz)maz=vertex_m[i].pos[2];
		if(vertex_m[i].pos[0]<mix)mix=vertex_m[i].pos[0];
		if(vertex_m[i].pos[1]<miy)miy=vertex_m[i].pos[1];
		if(vertex_m[i].pos[2]<miz)miz=vertex_m[i].pos[2];
	}
	Vec3D maxBox=Vec3D(max,may,maz);
	Vec3D minBox=Vec3D(mix,miy,miz);
	float radius=(maxBox.length()+minBox.length())/2;
	header_m->VertexBoxMax=maxBox;
	header_m->VertexBoxMin=minBox;
	header_m->VertexBoxRadius=radius;
}

void ModelEditor::calcBoundingBox(){
	float max=-99999;
	float may=-99999;
	float maz=-99999;
	float mix=99999;
	float miy=99999;
	float miz=99999;
	for(int i=0;i<header_m->nBoundingVertices;i++){
		if(boundvertices_m[i].x>max)max=boundvertices_m[i].x;
		if(boundvertices_m[i].y>may)may=boundvertices_m[i].y;
		if(boundvertices_m[i].z>maz)maz=boundvertices_m[i].z;
		if(boundvertices_m[i].x<mix)mix=boundvertices_m[i].x;
		if(boundvertices_m[i].y<miy)miy=boundvertices_m[i].y;
		if(boundvertices_m[i].z<miz)miz=boundvertices_m[i].z;
	}
	Vec3D maxBox=Vec3D(max,may,maz);
	Vec3D minBox=Vec3D(mix,miy,miz);
	float radius=(maxBox.length()+minBox.length())/2;
	header_m->BoundingBoxMax=maxBox;
	header_m->BoundingBoxMin=minBox;
	header_m->BoundingBoxRadius=radius;
}

void ModelEditor::ResizeModel(float factor) {

	for (int i = 0; i < header_m->nVertices; i++) {
		vertex_m[i].pos[0] *= factor;
		vertex_m[i].pos[1] *= factor;
		vertex_m[i].pos[2] *= factor;
	}
	for (int i = 0; i < header_m->nBones; i++) {
		bone_m[i].pivot[0] *= factor;
		bone_m[i].pivot[1] *= factor;
		bone_m[i].pivot[2] *= factor;
	}
	for (int i = 0; i < header_m->nLights; i++) {
		light_m[i].pos[0] *= factor;
		light_m[i].pos[1] *= factor;
		light_m[i].pos[2] *= factor;
	}
	for (int i = 0; i < header_m->nAttachments; i++) {
		attach_m[i].pos[0] *= factor;
		attach_m[i].pos[1] *= factor;
		attach_m[i].pos[2] *= factor;
	}
	for (int i = 0; i < header_m->nRibbonEmitters; i++) {
		ribbon_m[i].pos[0] *= factor;
		ribbon_m[i].pos[1] *= factor;
		ribbon_m[i].pos[2] *= factor;
	}
	for (int i = 0; i < header_m->nParticleEmitters; i++) {
		part_m[i].pos[0] *= factor;
		part_m[i].pos[1] *= factor;
		part_m[i].pos[2] *= factor;
	}
	for(int i=0;i<header_m->nBoundingVertices;i++){
		boundvertices_m[i].x*= factor;
		boundvertices_m[i].y*= factor;
		boundvertices_m[i].z*= factor;
	}
	calcVertexBox();
	calcBoundingBox();

}
