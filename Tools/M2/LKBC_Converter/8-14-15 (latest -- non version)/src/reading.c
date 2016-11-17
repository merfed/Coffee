/**
 * @file
 * File reading routines
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reading.h"
#include "common.h"
#include "colors.h"
#include "printer.h"

char* animfile_name(char *name, short AnimID, short SubAnimID) {
	char *s = malloc(name_length + 4 + 1 + 2 + strlen(".anim") + 1); //%s%04d-%02d.anim
	sprintf(s, "%s%04d-%02d.anim", name, AnimID, SubAnimID);
	return s;
}
/**
 * @param skin_files A list of pointers to skin files to read data from.
 * @param ptr A pointer to a list of skins structure where data will be written.
 * @param n The number of skin files. Between 1 and 4.
 */
int read_skins(FILE **skin_files, Skin **ptr, int n) {
	int i;
	*ptr = malloc(n * sizeof(Skin));
	for (i = 0; i < n; i++) {
		//Header
		fseek(skin_files[i], 0, SEEK_SET);
		fread(&(*ptr)[i].header, sizeof(SkinHeader), 1, skin_files[i]);

		//Indices
		if ((*ptr)[i].header.nIndices > 0) {
			(*ptr)[i].Indices = malloc(
					(*ptr)[i].header.nIndices * sizeof(Vertex));
			fseek(skin_files[i], (*ptr)[i].header.ofsIndices, SEEK_SET);
			fread((*ptr)[i].Indices, sizeof(Vertex), (*ptr)[i].header.nIndices,
					skin_files[i]);
		}

		//Triangles
		if ((*ptr)[i].header.nTriangles > 0) {
			(*ptr)[i].Triangles = malloc(
					((*ptr)[i].header.nTriangles / 3) * sizeof(Triangle));
			fseek(skin_files[i], (*ptr)[i].header.ofsTriangles, SEEK_SET);
			fread((*ptr)[i].Triangles, sizeof(Triangle),
					(*ptr)[i].header.nTriangles / 3, skin_files[i]);
		}

		//Properties
		if ((*ptr)[i].header.nProperties > 0) {
			(*ptr)[i].Properties = malloc(
					(*ptr)[i].header.nProperties * sizeof(Property));
			fseek(skin_files[i], (*ptr)[i].header.ofsProperties, SEEK_SET);
			fread((*ptr)[i].Properties, sizeof(Property),
					(*ptr)[i].header.nProperties, skin_files[i]);
		}

		//Submeshes
		if ((*ptr)[i].header.nSubmeshes > 0) {
			(*ptr)[i].Submeshes = malloc(
					(*ptr)[i].header.nSubmeshes * sizeof(LKSubmesh));
			fseek(skin_files[i], (*ptr)[i].header.ofsSubmeshes, SEEK_SET);
			fread((*ptr)[i].Submeshes, sizeof(LKSubmesh),
					(*ptr)[i].header.nSubmeshes, skin_files[i]);
		}

		//TexUnits
		if ((*ptr)[i].header.nTextureUnits > 0) {
			(*ptr)[i].TextureUnits = malloc(
					(*ptr)[i].header.nTextureUnits * sizeof(TexUnit));
			fseek(skin_files[i], (*ptr)[i].header.ofsTextureUnits, SEEK_SET);
			fread((*ptr)[i].TextureUnits, sizeof(TexUnit),
					(*ptr)[i].header.nTextureUnits, skin_files[i]);
		}
	}
	return 0;
}

void read_layer1(FILE *lk_m2_file, LKAnimationBlock *ptrBlock,
		AnimRefs *ptrAnimRefs) {
	ptrAnimRefs->times = malloc(ptrBlock->Times.n * sizeof(ArrayRef));
	fseek(lk_m2_file, ptrBlock->Times.ofs, SEEK_SET);
	fread(ptrAnimRefs->times, sizeof(ArrayRef), ptrBlock->Times.n, lk_m2_file);

	ptrAnimRefs->keys = malloc(ptrBlock->Keys.n * sizeof(ArrayRef));
	fseek(lk_m2_file, ptrBlock->Keys.ofs, SEEK_SET);
	fread(ptrAnimRefs->keys, sizeof(ArrayRef), ptrBlock->Keys.n, lk_m2_file);
}

void read_times(FILE *lk_m2_file, AnimRefs *ptrAnimRefs, uint32 **ptrTimeList,
		int j, int real_pos) {
	if (ptrAnimRefs->times[real_pos].n > 0) {
		(*ptrTimeList) = malloc(
				ptrAnimRefs->times[real_pos].n * sizeof(uint32)); //The number of elements was found previously in this function (stored in animofs)
		fseek(lk_m2_file, ptrAnimRefs->times[real_pos].ofs,
		SEEK_SET);
		fread((*ptrTimeList), sizeof(uint32), ptrAnimRefs->times[real_pos].n,
				lk_m2_file);
	}
}
void read_Vec3DAnimBlock(FILE *lk_m2_file, LKAnimationBlock *ptrBlock,
		AnimRefs *ptrAnimRefs, Vec3D_LKSubBlock **ptrDataBlock,
		LKModelAnimation *AnimList, FILE **anim_files) {
	if (ptrBlock->Times.n > 0) {
		//Layer 1
		read_layer1(lk_m2_file, ptrBlock, ptrAnimRefs);
		//Layer 2
		(*ptrDataBlock) = malloc(ptrBlock->Times.n * sizeof(Vec3D_LKSubBlock)); //Each Array_Ref leads to an array of elements (and there are Times.n of them, as seen previously)
		int j;
		for (j = 0; j < ptrBlock->Times.n; j++) {
			int real_pos = get_RealPos(j, AnimList);
			if ((AnimList[j].flags & 0x130) == 0) {		//Open .anim file
				FILE *anim_file = anim_files[real_pos];
				read_times(anim_file, ptrAnimRefs, &(*ptrDataBlock)[j].times, j,
						real_pos);
				if (ptrAnimRefs->keys[j].n > 0) {
					(*ptrDataBlock)[j].keys = malloc(
							ptrAnimRefs->keys[real_pos].n * sizeof(Vec3D));
					fseek(anim_file, ptrAnimRefs->keys[real_pos].ofs,
					SEEK_SET);
					fread((*ptrDataBlock)[j].keys, sizeof(Vec3D),
							ptrAnimRefs->keys[real_pos].n, anim_file);
				}
			} else {
				read_times(lk_m2_file, ptrAnimRefs, &(*ptrDataBlock)[j].times,
						j, real_pos);
				if (ptrAnimRefs->keys[real_pos].n > 0) {
					(*ptrDataBlock)[j].keys = malloc(
							ptrAnimRefs->keys[real_pos].n * sizeof(Vec3D));
					fseek(lk_m2_file, ptrAnimRefs->keys[real_pos].ofs,
					SEEK_SET);
					fread((*ptrDataBlock)[j].keys, sizeof(Vec3D),
							ptrAnimRefs->keys[real_pos].n, lk_m2_file);
				}
			}
		}
	}
}

void read_BigFloatAnimBlock(FILE *lk_m2_file, LKAnimationBlock *ptrBlock,
		AnimRefs *ptrAnimRefs, BigFloat_LKSubBlock **ptrDataBlock,
		LKModelAnimation *AnimList, FILE **anim_files) {
	if (ptrBlock->Times.n > 0) {
		//Layer 1
		read_layer1(lk_m2_file, ptrBlock, ptrAnimRefs);
		//Layer 2
		(*ptrDataBlock) = malloc(
				ptrBlock->Times.n * sizeof(BigFloat_LKSubBlock)); //Each Array_Ref leads to an array of elements (and there are Times.n of them, as seen previously)
		int j;
		for (j = 0; j < ptrBlock->Times.n; j++) {
			int real_pos = get_RealPos(j, AnimList);
			if ((AnimList[j].flags & 0x130) == 0) {		//Open .anim file
				FILE *anim_file = anim_files[real_pos];
				read_times(anim_file, ptrAnimRefs, &(*ptrDataBlock)[j].times, j,
						real_pos);
				if (ptrAnimRefs->keys[real_pos].n > 0) {
					(*ptrDataBlock)[j].keys = malloc(
							ptrAnimRefs->keys[real_pos].n * sizeof(BigFloat));
					fseek(anim_file, ptrAnimRefs->keys[real_pos].ofs,
					SEEK_SET);
					fread((*ptrDataBlock)[j].keys, sizeof(BigFloat),
							ptrAnimRefs->keys[real_pos].n, anim_file);
				}
			} else {
				read_times(lk_m2_file, ptrAnimRefs, &(*ptrDataBlock)[j].times,
						j, real_pos);
				if (ptrAnimRefs->keys[real_pos].n > 0) {
					(*ptrDataBlock)[j].keys = malloc(
							ptrAnimRefs->keys[real_pos].n * sizeof(BigFloat));
					fseek(lk_m2_file, ptrAnimRefs->keys[real_pos].ofs,
					SEEK_SET);
					fread((*ptrDataBlock)[j].keys, sizeof(BigFloat),
							ptrAnimRefs->keys[real_pos].n, lk_m2_file);
				}
			}
		}
	}
}

void read_QuatAnimBlock(FILE *lk_m2_file, LKAnimationBlock *ptrBlock,
		AnimRefs *ptrAnimRefs, Quat_LKSubBlock **ptrDataBlock,
		LKModelAnimation *AnimList, FILE **anim_files) {
	if (ptrBlock->Times.n > 0) {
		//Layer 1
		read_layer1(lk_m2_file, ptrBlock, ptrAnimRefs);
		//Layer 2
		(*ptrDataBlock) = malloc(ptrBlock->Times.n * sizeof(Quat_LKSubBlock)); //Each Array_Ref leads to an array of elements (and there are Times.n of them, as seen previously)
		int j;
		for (j = 0; j < ptrBlock->Times.n; j++) {
			int real_pos = get_RealPos(j, AnimList);
			if ((AnimList[j].flags & 0x130) == 0) {		//Open .anim file
				FILE *anim_file = anim_files[real_pos];
				read_times(anim_file, ptrAnimRefs, &(*ptrDataBlock)[j].times, j,
						real_pos);
				if (ptrAnimRefs->keys[real_pos].n > 0) {
					(*ptrDataBlock)[j].keys = malloc(
							ptrAnimRefs->keys[real_pos].n * sizeof(Quat));
					fseek(anim_file, ptrAnimRefs->keys[real_pos].ofs,
					SEEK_SET);
					fread((*ptrDataBlock)[j].keys, sizeof(Quat),
							ptrAnimRefs->keys[real_pos].n, anim_file);
				}
			} else {
				read_times(lk_m2_file, ptrAnimRefs, &(*ptrDataBlock)[j].times,
						j, real_pos);
				if (ptrAnimRefs->keys[real_pos].n > 0) {
					(*ptrDataBlock)[j].keys = malloc(
							ptrAnimRefs->keys[real_pos].n * sizeof(Quat));
					fseek(lk_m2_file, ptrAnimRefs->keys[real_pos].ofs,
					SEEK_SET);
					fread((*ptrDataBlock)[j].keys, sizeof(Quat),
							ptrAnimRefs->keys[real_pos].n, lk_m2_file);
				}
			}
		}
	}
}

void read_ShortAnimBlock(FILE *lk_m2_file, LKAnimationBlock *ptrBlock,
		AnimRefs *ptrAnimRefs, Short_LKSubBlock **ptrDataBlock,
		LKModelAnimation *AnimList, FILE **anim_files) {
	if (ptrBlock->Times.n > 0) {
		//Layer 1
		read_layer1(lk_m2_file, ptrBlock, ptrAnimRefs);
		//Layer 2
		(*ptrDataBlock) = malloc(ptrBlock->Times.n * sizeof(Short_LKSubBlock)); //Each Array_Ref leads to an array of elements (and there are Times.n of them, as seen previously)
		int j;
		for (j = 0; j < ptrBlock->Times.n; j++) {
			int real_pos = get_RealPos(j, AnimList);
			if ((AnimList[j].flags & 0x130) == 0) {	//Open .anim file
				FILE *anim_file = anim_files[real_pos];
				read_times(anim_file, ptrAnimRefs, &(*ptrDataBlock)[j].times, j,
						real_pos);
				if (ptrAnimRefs->keys[real_pos].n > 0) {
					(*ptrDataBlock)[j].keys = malloc(
							ptrAnimRefs->keys[real_pos].n * sizeof(short));
					fseek(anim_file, ptrAnimRefs->keys[real_pos].ofs,
					SEEK_SET);
					fread((*ptrDataBlock)[j].keys, sizeof(short),
							ptrAnimRefs->keys[real_pos].n, anim_file);
				}
			} else {
				read_times(lk_m2_file, ptrAnimRefs, &(*ptrDataBlock)[j].times,
						j, real_pos);
				if (ptrAnimRefs->keys[real_pos].n > 0) {
					(*ptrDataBlock)[j].keys = malloc(
							ptrAnimRefs->keys[real_pos].n * sizeof(short));
					fseek(lk_m2_file, ptrAnimRefs->keys[real_pos].ofs,
					SEEK_SET);
					fread((*ptrDataBlock)[j].keys, sizeof(short),
							ptrAnimRefs->keys[real_pos].n, lk_m2_file);
				}
			}
		}
	}
}

void read_CharAnimBlock(FILE *lk_m2_file, LKAnimationBlock *ptrBlock,
		AnimRefs *ptrAnimRefs, Char_LKSubBlock **ptrDataBlock,
		LKModelAnimation *AnimList, FILE **anim_files) {
	if (ptrBlock->Times.n > 0) {
		//Layer 1
		read_layer1(lk_m2_file, ptrBlock, ptrAnimRefs);
		//Layer 2
		(*ptrDataBlock) = malloc(ptrBlock->Times.n * sizeof(Char_LKSubBlock)); //Each Array_Ref leads to an array of elements (and there are Times.n of them, as seen previously)
		int j;
		for (j = 0; j < ptrBlock->Times.n; j++) {
			int real_pos = get_RealPos(j, AnimList);
			if ((AnimList[j].flags & 0x130) == 0) {	//Open .anim file
				FILE *anim_file = anim_files[real_pos];
				read_times(anim_file, ptrAnimRefs, &(*ptrDataBlock)[j].times, j,
						real_pos);
				if (ptrAnimRefs->keys[real_pos].n > 0) {
					(*ptrDataBlock)[j].keys = malloc(
							ptrAnimRefs->keys[real_pos].n * sizeof(char));
					fseek(anim_file, ptrAnimRefs->keys[real_pos].ofs,
					SEEK_SET);
					fread((*ptrDataBlock)[j].keys, sizeof(char),
							ptrAnimRefs->keys[real_pos].n, anim_file);
				}
			} else {
				read_times(lk_m2_file, ptrAnimRefs, &(*ptrDataBlock)[j].times,
						j, real_pos);
				if (ptrAnimRefs->keys[real_pos].n > 0) {
					(*ptrDataBlock)[j].keys = malloc(
							ptrAnimRefs->keys[real_pos].n * sizeof(char));
					fseek(lk_m2_file, ptrAnimRefs->keys[real_pos].ofs,
					SEEK_SET);
					fread((*ptrDataBlock)[j].keys, sizeof(char),
							ptrAnimRefs->keys[real_pos].n, lk_m2_file);
				}
			}
		}
	}
}

void read_IntAnimBlock(FILE *lk_m2_file, LKAnimationBlock *ptrBlock,
		AnimRefs *ptrAnimRefs, Int_LKSubBlock **ptrDataBlock,
		LKModelAnimation *AnimList, FILE **anim_files) {
	if (ptrBlock->Times.n > 0) {
		//Layer 1
		read_layer1(lk_m2_file, ptrBlock, ptrAnimRefs);
		//Layer 2
		(*ptrDataBlock) = malloc(ptrBlock->Times.n * sizeof(Int_LKSubBlock)); //Each Array_Ref leads to an array of elements (and there are Times.n of them, as seen previously)
		int j;
		for (j = 0; j < ptrBlock->Times.n; j++) {
			int real_pos = get_RealPos(j, AnimList);
			if ((AnimList[j].flags & 0x130) == 0) {	//Open .anim file
				FILE *anim_file = anim_files[real_pos];
				read_times(anim_file, ptrAnimRefs, &(*ptrDataBlock)[j].times, j,
						real_pos);
				if (ptrAnimRefs->keys[real_pos].n > 0) {
					(*ptrDataBlock)[j].keys = malloc(
							ptrAnimRefs->keys[real_pos].n * sizeof(int));
					fseek(anim_file, ptrAnimRefs->keys[real_pos].ofs,
					SEEK_SET);
					fread((*ptrDataBlock)[j].keys, sizeof(int),
							ptrAnimRefs->keys[real_pos].n, anim_file);
				}
			} else {
				read_times(lk_m2_file, ptrAnimRefs, &(*ptrDataBlock)[j].times,
						j, real_pos);
				if (ptrAnimRefs->keys[real_pos].n > 0) {
					(*ptrDataBlock)[j].keys = malloc(
							ptrAnimRefs->keys[real_pos].n * sizeof(int));
					fseek(lk_m2_file, ptrAnimRefs->keys[real_pos].ofs,
					SEEK_SET);
					fread((*ptrDataBlock)[j].keys, sizeof(int),
							ptrAnimRefs->keys[real_pos].n, lk_m2_file);
				}
			}
		}
	}
}

void read_FloatAnimBlock(FILE *lk_m2_file, LKAnimationBlock *ptrBlock,
		AnimRefs *ptrAnimRefs, Float_LKSubBlock **ptrDataBlock,
		LKModelAnimation *AnimList, FILE **anim_files) {
	if (ptrBlock->Times.n > 0) {
		//Layer 1
		read_layer1(lk_m2_file, ptrBlock, ptrAnimRefs);
		//Layer 2
		(*ptrDataBlock) = malloc(ptrBlock->Times.n * sizeof(Float_LKSubBlock)); //Each Array_Ref leads to an array of elements (and there are Times.n of them, as seen previously)
		int j;
		for (j = 0; j < ptrBlock->Times.n; j++) {
			int real_pos = get_RealPos(j, AnimList);
			if ((AnimList[j].flags & 0x130) == 0) {	//Open .anim file
				FILE *anim_file = anim_files[real_pos];
				read_times(anim_file, ptrAnimRefs, &(*ptrDataBlock)[j].times, j,
						real_pos);
				if (ptrAnimRefs->keys[real_pos].n > 0) {
					(*ptrDataBlock)[j].keys = malloc(
							ptrAnimRefs->keys[real_pos].n * sizeof(float));
					fseek(anim_file, ptrAnimRefs->keys[real_pos].ofs,
					SEEK_SET);
					fread((*ptrDataBlock)[j].keys, sizeof(float),
							ptrAnimRefs->keys[real_pos].n, anim_file);
				}
			} else {
				read_times(lk_m2_file, ptrAnimRefs, &(*ptrDataBlock)[j].times,
						j, real_pos);
				if (ptrAnimRefs->keys[real_pos].n > 0) {
					(*ptrDataBlock)[j].keys = malloc(
							ptrAnimRefs->keys[real_pos].n * sizeof(float));
					fseek(lk_m2_file, ptrAnimRefs->keys[real_pos].ofs,
					SEEK_SET);
					fread((*ptrDataBlock)[j].keys, sizeof(float),
							ptrAnimRefs->keys[real_pos].n, lk_m2_file);
				}
			}
		}
	}
}

void read_EventAnimBlock(FILE *lk_m2_file, LKEventAnimBlock *ptrBlock,
		ArrayRef **ptrAnimRefs, uint32 ***ptrDataBlock) {
	if (ptrBlock->Times.n > 0) {
		//Layer 1
		(*ptrAnimRefs) = malloc(ptrBlock->Times.n * sizeof(ArrayRef));
		fseek(lk_m2_file, ptrBlock->Times.ofs, SEEK_SET);
		fread((*ptrAnimRefs), sizeof(ArrayRef), ptrBlock->Times.n, lk_m2_file);
		//Layer 2
		(*ptrDataBlock) = malloc(ptrBlock->Times.n * sizeof(uint32*)); //Each Array_Ref leads to an array of elements (and there are Times.n of them, as seen previously)
		int j;
		for (j = 0; j < ptrBlock->Times.n; j++) {
			if ((*ptrAnimRefs)[j].n > 0) {
				(*ptrDataBlock)[j] = malloc(
						(*ptrAnimRefs)[j].n * sizeof(uint32)); //The number of elements was found previously in this function (stored in animofs)
				fseek(lk_m2_file, (*ptrAnimRefs)[j].ofs,
				SEEK_SET);
				fread((*ptrDataBlock)[j], sizeof(uint32), (*ptrAnimRefs)[j].n,
						lk_m2_file);
			}
		}
	}
}
/**
 * Read model bones with animations.
 * @param lk_m2_file The file to read data.
 * @param ptr Pointer to a M2/WotLK structure.
 * i is a bone number, j is an animation number
 */
int read_bones(FILE *lk_m2_file, LKM2 *ptr, FILE **anim_files) {
	if (ptr->header.nBones > 0) { //I think lights and other non-geometric things don't have any
		ptr->bones = malloc(ptr->header.nBones * sizeof(LKModelBoneDef));
		fseek(lk_m2_file, ptr->header.ofsBones, SEEK_SET);
		fread(ptr->bones, sizeof(LKModelBoneDef), ptr->header.nBones,
				lk_m2_file);

		/*This function is a good example of reading LK Animation Blocks and ArrayRefs.
		 * In the Block, you have what I call an ArrayRef : a number of elements and a file-relative offset to the first element.
		 * In LKAnimBlock, you have ArrayRefs of ArrayRefs. So you have to work by layers. One structure is required per layer.
		 *
		 * In the first, you store the n ArrayRefs you got by following the Block ArrayRef.
		 *
		 * In the second, you store the arrays of elements you get by following each layer 1 ArrayRef obtained in step one. So there are n of them.
		 * Remember each of these arrays of elements in the layer 2 has a custom size, given by its corresponding layer 1 ArrayRef.
		 */

		//Store animofs (layer 1)
		//This allows later to read the real animation data, by following each ArrayRef.
		ptr->animofs = malloc(ptr->header.nBones * sizeof(RefBlock));//1 LKRefBlock per bone
		//Store bones data (layer 2)
		ptr->bonesdata = malloc(ptr->header.nBones * sizeof(LKBonesDataBlock));
		int i;
		for (i = 0; i < ptr->header.nBones; i++) {
			//Translation
			read_Vec3DAnimBlock(lk_m2_file, &ptr->bones[i].trans,
					&ptr->animofs[i].trans, &ptr->bonesdata[i].trans,
					ptr->animations, anim_files);
			//Rotation
			read_QuatAnimBlock(lk_m2_file, &ptr->bones[i].rot,
					&ptr->animofs[i].rot, &ptr->bonesdata[i].rot,
					ptr->animations, anim_files);
			//Scaling
			read_Vec3DAnimBlock(lk_m2_file, &ptr->bones[i].scal,
					&ptr->animofs[i].scal, &ptr->bonesdata[i].scal,
					ptr->animations, anim_files);
		}
		return 0;
	}
	return -1;
}

int read_attachments(FILE *lk_m2_file, LKM2 *ptr, FILE **anim_files) {
	if (ptr->header.nAttachments > 0) {
		ptr->attachments = malloc(
				ptr->header.nAttachments * sizeof(LKAttachment));
		fseek(lk_m2_file, ptr->header.ofsAttachments, SEEK_SET);
		fread(ptr->attachments, sizeof(LKAttachment), ptr->header.nAttachments,
				lk_m2_file);

		ptr->attachmentsanimofs = malloc(
				ptr->header.nAttachments * sizeof(AttachmentsRefBlock));//1 LKRefBlock per bone
		ptr->attachmentsdata = malloc(
				ptr->header.nAttachments * sizeof(LKAttachmentsDataBlock));
		int i;
		for (i = 0; i < ptr->header.nAttachments; i++) {
			//Data
			read_CharAnimBlock(lk_m2_file, &ptr->attachments[i].data,
					&ptr->attachmentsanimofs[i].data,
					&ptr->attachmentsdata[i].data, ptr->animations, anim_files);
		}
		return 0;
	}
	return -1;
}

int read_lights(FILE *lk_m2_file, LKM2 *ptr, FILE **anim_files) {
	if (ptr->header.nLights > 0) {
		ptr->lights = malloc(ptr->header.nLights * sizeof(LKLight));
		fseek(lk_m2_file, ptr->header.ofsLights, SEEK_SET);
		fread(ptr->lights, sizeof(LKLight), ptr->header.nLights, lk_m2_file);

		ptr->lightsanimofs = malloc(
				ptr->header.nLights * sizeof(LightsRefBlock));//1 LKRefBlock per bone
		ptr->lightsdata = malloc(
				ptr->header.nLights * sizeof(LKLightsDataBlock));
		int i;
		for (i = 0; i < ptr->header.nLights; i++) {
			//A_color
			read_Vec3DAnimBlock(lk_m2_file, &ptr->lights[i].a_color,
					&ptr->lightsanimofs[i].a_color, &ptr->lightsdata[i].a_color,
					ptr->animations, anim_files);
			//A_intensity
			read_FloatAnimBlock(lk_m2_file, &ptr->lights[i].a_intensity,
					&ptr->lightsanimofs[i].a_intensity,
					&ptr->lightsdata[i].a_intensity, ptr->animations,
					anim_files);
			//D_color
			read_Vec3DAnimBlock(lk_m2_file, &ptr->lights[i].d_color,
					&ptr->lightsanimofs[i].d_color, &ptr->lightsdata[i].d_color,
					ptr->animations, anim_files);
			//D_intensity
			read_FloatAnimBlock(lk_m2_file, &ptr->lights[i].d_intensity,
					&ptr->lightsanimofs[i].d_intensity,
					&ptr->lightsdata[i].d_intensity, ptr->animations,
					anim_files);

			//A_start
			read_FloatAnimBlock(lk_m2_file, &ptr->lights[i].a_start,
					&ptr->lightsanimofs[i].a_start, &ptr->lightsdata[i].a_start,
					ptr->animations, anim_files);
			//A_end
			read_FloatAnimBlock(lk_m2_file, &ptr->lights[i].a_end,
					&ptr->lightsanimofs[i].a_end, &ptr->lightsdata[i].a_end,
					ptr->animations, anim_files);

			//Unknown
			read_IntAnimBlock(lk_m2_file, &ptr->lights[i].unknown,
					&ptr->lightsanimofs[i].unknown, &ptr->lightsdata[i].unknown,
					ptr->animations, anim_files);
		}
		return 0;
	}
	return -1;
}

int read_events(FILE *lk_m2_file, LKM2 *ptr) {
	if (ptr->header.nEvents > 0) {
		ptr->events = malloc(ptr->header.nEvents * sizeof(LKEvent));
		fseek(lk_m2_file, ptr->header.ofsEvents, SEEK_SET);
		fread(ptr->events, sizeof(LKEvent), ptr->header.nEvents, lk_m2_file);

		ptr->eventsanimofs = malloc(
				ptr->header.nEvents * sizeof(EventsRefBlock));//1 LKRefBlock per bone
		ptr->eventsdata = malloc(
				ptr->header.nEvents * sizeof(LKEventsDataBlock));
		int i;
		for (i = 0; i < ptr->header.nEvents; i++) {
			//Timer
			read_EventAnimBlock(lk_m2_file, &ptr->events[i].timer,
					&ptr->eventsanimofs[i].times, &ptr->eventsdata[i].times);
		}
		return 0;
	}
	return -1;
}

/**
 * Read colors (also known as submesh animations)
 * @param lk_m2_file
 * @param ptr
 * @return
 */
int read_colors(FILE *lk_m2_file, LKM2 *ptr, FILE **anim_files) {
	if (ptr->header.nColors > 0) { //I think lights and other non-geometric things don't have any
		ptr->colors = malloc(ptr->header.nColors * sizeof(LKColorDef));
		fseek(lk_m2_file, ptr->header.ofsColors, SEEK_SET);
		fread(ptr->colors, sizeof(LKColorDef), ptr->header.nColors, lk_m2_file);

		ptr->coloranimofs = malloc(ptr->header.nColors * sizeof(ColorRefBlock)); //1 LKRefBlock per bone
		ptr->colorsdata = malloc(
				ptr->header.nColors * sizeof(LKColorDataBlock));
		int i;
		for (i = 0; i < ptr->header.nColors; i++) {
			//RGB
			read_Vec3DAnimBlock(lk_m2_file, &ptr->colors[i].rgb,
					&ptr->coloranimofs[i].rgb, &ptr->colorsdata[i].rgb,
					ptr->animations, anim_files);
			//Opacity
			read_ShortAnimBlock(lk_m2_file, &ptr->colors[i].opacity,
					&ptr->coloranimofs[i].opacity, &ptr->colorsdata[i].opacity,
					ptr->animations, anim_files);
		}
		return 0;
	}
	return -1;
}

int read_cameras(FILE *lk_m2_file, LKM2 *ptr, FILE **anim_files) {
	if (ptr->header.nCameras > 0) { //I think lights and other non-geometric things don't have any
		ptr->cameras = malloc(ptr->header.nCameras * sizeof(LKCamera));
		fseek(lk_m2_file, ptr->header.ofsCameras, SEEK_SET);
		fread(ptr->cameras, sizeof(LKCamera), ptr->header.nCameras, lk_m2_file);

		ptr->camerasanimofs = malloc(
				ptr->header.nCameras * sizeof(CamerasRefBlock)); //1 LKRefBlock per bone
		ptr->camerasdata = malloc(
				ptr->header.nCameras * sizeof(LKCamerasDataBlock));
		int i;
		for (i = 0; i < ptr->header.nCameras; i++) {
			//Translation position
			read_BigFloatAnimBlock(lk_m2_file, &ptr->cameras[i].transpos,
					&ptr->camerasanimofs[i].transpos,
					&ptr->camerasdata[i].transpos, ptr->animations, anim_files);
			//Translation target
			read_BigFloatAnimBlock(lk_m2_file, &ptr->cameras[i].transtar,
					&ptr->camerasanimofs[i].transtar,
					&ptr->camerasdata[i].transtar, ptr->animations, anim_files);
			//Scaling
			read_Vec3DAnimBlock(lk_m2_file, &ptr->cameras[i].scal,
					&ptr->camerasanimofs[i].scal, &ptr->camerasdata[i].scal,
					ptr->animations, anim_files);
		}
		return 0;
	}
	return -1;
}

int read_transparency(FILE *lk_m2_file, LKM2 *ptr, FILE **anim_files) {
	if (ptr->header.nTransparency > 0) {
		ptr->transparencyrefs = malloc(
				ptr->header.nTransparency * sizeof(LKTransparency));
		fseek(lk_m2_file, ptr->header.ofsTransparency, SEEK_SET);
		fread(ptr->transparencyrefs, sizeof(LKTransparency),
				ptr->header.nTransparency, lk_m2_file);

		ptr->transparencyanimofs = malloc(
				ptr->header.nTransparency * sizeof(TransparencyRefBlock)); //1 LKRefBlock per bone
		ptr->transparencydata = malloc(
				ptr->header.nTransparency * sizeof(LKTransparencyDataBlock));
		int i;
		for (i = 0; i < ptr->header.nTransparency; i++) {
			//Alpha
			read_ShortAnimBlock(lk_m2_file, &ptr->transparencyrefs[i].alpha,
					&ptr->transparencyanimofs[i].alpha,
					&ptr->transparencydata[i].alpha, ptr->animations,
					anim_files);
		}
		return 0;
	}
	return -1;
}

/**
 * Read texture animations.
 * @param lk_m2_file The file to read data.
 * @param ptr Pointer to a M2/WotLK structure.
 * @return 0 if successful
 */
int read_texanims(FILE *lk_m2_file, LKM2 *ptr, FILE **anim_files) {
	if (ptr->header.nTexAnims > 0) {
		ptr->texanims = malloc(
				ptr->header.nTexAnims * sizeof(LKTextureAnimation));
		fseek(lk_m2_file, ptr->header.ofsTexAnims, SEEK_SET);
		fread(ptr->texanims, sizeof(LKTextureAnimation), ptr->header.nTexAnims,
				lk_m2_file);

		ptr->texanimofs = malloc(ptr->header.nTexAnims * sizeof(RefBlock));
		ptr->texdata = malloc(ptr->header.nTexAnims * sizeof(LKBonesDataBlock));
		int i;
		for (i = 0; i < ptr->header.nTexAnims; i++) {
			//Translation
			read_Vec3DAnimBlock(lk_m2_file, &ptr->texanims[i].trans,
					&ptr->texanimofs[i].trans, &ptr->texdata[i].trans,
					ptr->animations, anim_files);
			//Rotation
			read_QuatAnimBlock(lk_m2_file, &ptr->texanims[i].rot,
					&ptr->texanimofs[i].rot, &ptr->texdata[i].rot,
					ptr->animations, anim_files);
			//Scaling
			read_Vec3DAnimBlock(lk_m2_file, &ptr->texanims[i].scal,
					&ptr->animofs[i].scal, &ptr->texdata[i].scal,
					ptr->animations, anim_files);
		}
		return 0;
	}
	return -1;
}

/**
 * At the moment, this function is used to debug the converter by checking data on genuine BC models.
 * @param bc_m2_file
 * @param ptr
 * @return
 */
int read_model_bc(FILE *bc_m2_file, BCM2 *ptr) {
	//Header
	fseek(bc_m2_file, 0, SEEK_SET);
	fread(&ptr->header, sizeof(ModelHeader), 1, bc_m2_file);

	/*
	 //Animations
	 ptr->animations = malloc(ptr->header.nAnimations * sizeof(ModelAnimation));
	 fseek(bc_m2_file, ptr->header.ofsAnimations, SEEK_SET);
	 fread(ptr->animations, sizeof(ModelAnimation), ptr->header.nAnimations,
	 bc_m2_file);

	 //Bones
	 if (ptr->header.nBones > 0) { //I think lights and other non-geometric things don't have any
	 ptr->bones = malloc(ptr->header.nBones * sizeof(ModelBoneDef));
	 ptr->bonesdata = malloc(ptr->header.nBones * sizeof(BonesDataBlock));
	 fseek(bc_m2_file, ptr->header.ofsBones, SEEK_SET);
	 fread(ptr->bones, sizeof(ModelBoneDef), ptr->header.nBones, bc_m2_file);
	 int i;
	 for (i = 0; i < ptr->header.nBones; i++) {
	 //Translation
	 if (ptr->bones[i].trans.Ranges.n > 0) {
	 ptr->bonesdata[i].trans.ranges = malloc(
	 ptr->bones[i].trans.Ranges.n * sizeof(Range));
	 fseek(bc_m2_file, ptr->bones[i].trans.Ranges.ofs,
	 SEEK_SET);
	 fread(ptr->bonesdata[i].trans.ranges, sizeof(Range),
	 ptr->bones[i].trans.Ranges.n, bc_m2_file);
	 }
	 if (ptr->bones[i].trans.Times.n > 0) {
	 ptr->bonesdata[i].trans.times = malloc(
	 ptr->bones[i].trans.Times.n * sizeof(uint32));
	 fseek(bc_m2_file, ptr->bones[i].trans.Times.ofs,
	 SEEK_SET);
	 fread(ptr->bonesdata[i].trans.times, sizeof(uint32),
	 ptr->bones[i].trans.Times.n, bc_m2_file);
	 }
	 if (ptr->bones[i].trans.Keys.n > 0) {
	 ptr->bonesdata[i].trans.keys = malloc(
	 ptr->bones[i].trans.Keys.n * sizeof(Vec3D));
	 fseek(bc_m2_file, ptr->bones[i].trans.Keys.ofs,
	 SEEK_SET);
	 fread(ptr->bonesdata[i].trans.keys, sizeof(Vec3D),
	 ptr->bones[i].trans.Keys.n, bc_m2_file);
	 }
	 //Rotation
	 if (ptr->bones[i].rot.Ranges.n > 0) {
	 ptr->bonesdata[i].rot.ranges = malloc(
	 ptr->bones[i].rot.Ranges.n * sizeof(Range));
	 fseek(bc_m2_file, ptr->bones[i].rot.Ranges.ofs,
	 SEEK_SET);
	 fread(ptr->bonesdata[i].rot.ranges, sizeof(Range),
	 ptr->bones[i].rot.Ranges.n, bc_m2_file);
	 }
	 if (ptr->bones[i].rot.Times.n > 0) {
	 ptr->bonesdata[i].rot.times = malloc(
	 ptr->bones[i].rot.Times.n * sizeof(uint32));
	 fseek(bc_m2_file, ptr->bones[i].rot.Times.ofs,
	 SEEK_SET);
	 fread(ptr->bonesdata[i].rot.times, sizeof(uint32),
	 ptr->bones[i].rot.Times.n, bc_m2_file);
	 }
	 if (ptr->bones[i].rot.Keys.n > 0) {
	 ptr->bonesdata[i].rot.keys = malloc(
	 ptr->bones[i].rot.Keys.n * sizeof(Quat));
	 fseek(bc_m2_file, ptr->bones[i].rot.Keys.ofs,
	 SEEK_SET);
	 fread(ptr->bonesdata[i].rot.keys, sizeof(Quat),
	 ptr->bones[i].rot.Keys.n, bc_m2_file);
	 }
	 //Scaling
	 if (ptr->bones[i].scal.Ranges.n > 0) {
	 ptr->bonesdata[i].scal.ranges = malloc(
	 ptr->bones[i].scal.Ranges.n * sizeof(Range));
	 fseek(bc_m2_file, ptr->bones[i].scal.Ranges.ofs,
	 SEEK_SET);
	 fread(ptr->bonesdata[i].scal.ranges, sizeof(Range),
	 ptr->bones[i].scal.Ranges.n, bc_m2_file);
	 }
	 if (ptr->bones[i].scal.Times.n > 0) {
	 ptr->bonesdata[i].scal.times = malloc(
	 ptr->bones[i].scal.Times.n * sizeof(uint32));
	 fseek(bc_m2_file, ptr->bones[i].scal.Times.ofs,
	 SEEK_SET);
	 fread(ptr->bonesdata[i].scal.times, sizeof(uint32),
	 ptr->bones[i].scal.Times.n, bc_m2_file);
	 }
	 if (ptr->bones[i].scal.Keys.n > 0) {
	 ptr->bonesdata[i].scal.keys = malloc(
	 ptr->bones[i].scal.Keys.n * sizeof(Vec3D));
	 fseek(bc_m2_file, ptr->bones[i].scal.Keys.ofs,
	 SEEK_SET);
	 fread(ptr->bonesdata[i].scal.keys, sizeof(Vec3D),
	 ptr->bones[i].scal.Keys.n, bc_m2_file);
	 }
	 }
	 }
	 */
	//Events
	if (ptr->header.nEvents > 0) { //I think lights and other non-geometric things don't have any
		ptr->events = malloc(ptr->header.nEvents * sizeof(Event));
		ptr->eventsdata = malloc(ptr->header.nEvents * sizeof(EventsDataBlock));
		fseek(bc_m2_file, ptr->header.ofsEvents, SEEK_SET);
		fread(ptr->events, sizeof(Event), ptr->header.nEvents, bc_m2_file);
		int i;
		for (i = 0; i < ptr->header.nEvents; i++) {
			//Content
			if (ptr->events[i].timer.Ranges.n > 0) {
				ptr->eventsdata[i].ranges = malloc(
						ptr->events[i].timer.Ranges.n * sizeof(Range));
				fseek(bc_m2_file, ptr->events[i].timer.Ranges.ofs,
				SEEK_SET);
				fread(ptr->eventsdata[i].ranges, sizeof(Range),
						ptr->events[i].timer.Ranges.n, bc_m2_file);
			}
			if (ptr->events[i].timer.Times.n > 0) {
				ptr->eventsdata[i].times = malloc(
						ptr->events[i].timer.Times.n * sizeof(uint32));
				fseek(bc_m2_file, ptr->events[i].timer.Times.ofs,
				SEEK_SET);
				fread(ptr->eventsdata[i].times, sizeof(uint32),
						ptr->events[i].timer.Times.n, bc_m2_file);
			}
		}
	}

	//Attachments
	if (ptr->header.nAttachments > 0) { //I think lights and other non-geometric things don't have any
		ptr->attachments = malloc(
				ptr->header.nAttachments * sizeof(Attachment));
		ptr->attachmentsdata = malloc(
				ptr->header.nAttachments * sizeof(AttachmentsDataBlock));
		fseek(bc_m2_file, ptr->header.ofsAttachments, SEEK_SET);
		fread(ptr->attachments, sizeof(Attachment), ptr->header.nAttachments,
				bc_m2_file);
		int i;
		for (i = 0; i < ptr->header.nAttachments; i++) {
			//Data
			if (ptr->attachments[i].data.Ranges.n > 0) {
				ptr->attachmentsdata[i].data.ranges = malloc(
						ptr->attachments[i].data.Ranges.n * sizeof(Range));
				fseek(bc_m2_file, ptr->attachments[i].data.Ranges.ofs,
				SEEK_SET);
				fread(ptr->attachmentsdata[i].data.ranges, sizeof(Range),
						ptr->attachments[i].data.Ranges.n, bc_m2_file);
			}
			if (ptr->attachments[i].data.Times.n > 0) {
				ptr->attachmentsdata[i].data.times = malloc(
						ptr->attachments[i].data.Times.n * sizeof(uint32));
				fseek(bc_m2_file, ptr->attachments[i].data.Times.ofs,
				SEEK_SET);
				fread(ptr->attachmentsdata[i].data.times, sizeof(uint32),
						ptr->attachments[i].data.Times.n, bc_m2_file);
			}
			if (ptr->attachments[i].data.Keys.n > 0) {
				ptr->attachmentsdata[i].data.keys = malloc(
						ptr->attachments[i].data.Keys.n * sizeof(int));
				fseek(bc_m2_file, ptr->attachments[i].data.Keys.ofs,
				SEEK_SET);
				fread(ptr->attachmentsdata[i].data.keys, sizeof(int),
						ptr->attachments[i].data.Keys.n, bc_m2_file);
			}
		}
	}
	return 0;
}

/**
 * Each substructure in the M2 has a variable size except the header.
 * Memory for each substructure must therefore be allocated at reading depending on header values.
 * @param lk_m2_file The file to read data.
 * @param ptr Pointer to a M2/WotLK structure.
 */
int read_model(FILE *lk_m2_file, LKM2 *ptr) {
	//Header
	fseek(lk_m2_file, 0, SEEK_SET);
	fread(&ptr->header, sizeof(LKModelHeader), 1, lk_m2_file);
	char real_id[5];
	real_id[0] = ptr->header.id[0];
	real_id[1] = ptr->header.id[1];
	real_id[2] = ptr->header.id[2];
	real_id[3] = ptr->header.id[3];
	real_id[4] = '\0';
	if (strcmp("MD20", real_id)) {
		fprintf(stderr, "This is not an M2 file.\n");
		exit(EXIT_FAILURE);
	}
	if (ptr->header.version != 264) {
		fprintf(stderr, "Incorrect model version (%d).\n", ptr->header.version);
		fprintf(stderr, "A WotLK model (264) is expected.\n");
		exit(EXIT_FAILURE);
	}

	//Name
	ptr->filename = malloc(ptr->header.nameLength);
	fseek(lk_m2_file, ptr->header.nameOfs, SEEK_SET);
	fread(ptr->filename, sizeof(char), ptr->header.nameLength, lk_m2_file);

	//Global Sequences
	ptr->globalsequences = malloc(
			ptr->header.nGlobalSequences * sizeof(unsigned int));
	if (ptr->header.nGlobalSequences > 0) {
		fseek(lk_m2_file, ptr->header.ofsGlobalSequences, SEEK_SET);
		fread(ptr->globalsequences, sizeof(unsigned int),
				ptr->header.nGlobalSequences, lk_m2_file);
	}

	//Animations
	ptr->animations = malloc(
			ptr->header.nAnimations * sizeof(LKModelAnimation));
	fseek(lk_m2_file, ptr->header.ofsAnimations, SEEK_SET);
	fread(ptr->animations, sizeof(LKModelAnimation), ptr->header.nAnimations,
			lk_m2_file);

	//Animation Files
	FILE **anim_files;
	anim_files = malloc(ptr->header.nAnimations * sizeof(FILE *));
	int i;
	for (i = 0; i < ptr->header.nAnimations; i++) {
		if (((ptr->animations[i].flags & 0x40) == 0)
				&& ((ptr->animations[i].flags & 0x130) == 0)) { //If anim[i] is not an alias and is not stored in the model
			printf("\t%s\n",
					animfile_name(model_name, ptr->animations[i].animID,
							ptr->animations[i].subAnimID));
			anim_files[i] = (FILE *) fcaseopen(
					animfile_name(model_name, ptr->animations[i].animID,
							ptr->animations[i].subAnimID), "r+b");
			if (anim_files[i] == NULL) {
				fprintf(stderr, KRED "[Error] " RESET
				"[Anim #%d, ID%d, Flags %d] %s file not found.\n", i,
						ptr->animations[i].animID, ptr->animations[i].flags,
						animfile_name(model_name, ptr->animations[i].animID,
								ptr->animations[i].subAnimID));
				fprintf(stderr, "[aliasNext %d]\n", ptr->animations[i].Index);
				exit(EXIT_FAILURE);
			}
		}
	}

	//Animations Lookup Table
	ptr->AnimLookup = malloc(ptr->header.nAnimationLookup * sizeof(short));
	fseek(lk_m2_file, ptr->header.ofsAnimationLookup, SEEK_SET);
	fread(ptr->AnimLookup, sizeof(short), ptr->header.nAnimationLookup,
			lk_m2_file);

	//Bones
	read_bones(lk_m2_file, ptr, anim_files);

	//Skeleton Bone Lookup
	ptr->keybonelookup = malloc(ptr->header.nKeyBoneLookup * sizeof(short));
	fseek(lk_m2_file, ptr->header.ofsKeyBoneLookup, SEEK_SET);
	fread(ptr->keybonelookup, sizeof(short), ptr->header.nKeyBoneLookup,
			lk_m2_file);

	//Vertices
	ptr->vertices = malloc(ptr->header.nVertices * sizeof(ModelVertex));
	fseek(lk_m2_file, ptr->header.ofsVertices, SEEK_SET);
	fread(ptr->vertices, sizeof(ModelVertex), ptr->header.nVertices,
			lk_m2_file);

	//Colors
	read_colors(lk_m2_file, ptr, anim_files);

	//Textures Definition
	if (ptr->header.nTextures > 0) {
		ptr->textures_def = malloc(
				ptr->header.nTextures * sizeof(ModelTextureDef));
		fseek(lk_m2_file, ptr->header.ofsTextures, SEEK_SET);
		fread(ptr->textures_def, sizeof(ModelTextureDef), ptr->header.nTextures,
				lk_m2_file);
		//textures names
		ptr->texture_names = malloc(ptr->header.nTextures * sizeof(char *));
		int i;
		for (i = 0; i < ptr->header.nTextures; i++) {
			if (ptr->textures_def[i].type == 0) { //Filename is referenced in the m2 only when the type is 0
				if (ptr->textures_def[i].nameLen >= 256) {
					fprintf(stderr,
							"nameLen too large : %d\nPlease report this issue.",
							ptr->textures_def[i].nameLen);
					return -1;
				}
				ptr->texture_names[i] = malloc(ptr->textures_def[i].nameLen);
				fseek(lk_m2_file, ptr->textures_def[i].nameOfs,
				SEEK_SET);
				fread(ptr->texture_names[i], sizeof(char),
						ptr->textures_def[i].nameLen, lk_m2_file);
			}
		}
	}

	//Transparency
	read_transparency(lk_m2_file, ptr, anim_files);

	//TexReplace
	ptr->TexReplace = malloc(ptr->header.nTexReplace * sizeof(short));
	fseek(lk_m2_file, ptr->header.ofsTexReplace, SEEK_SET);
	fread(ptr->TexReplace, sizeof(short), ptr->header.nTexReplace, lk_m2_file);

	//Render Flags
	ptr->renderflags = malloc(ptr->header.nRenderFlags * sizeof(int));
	fseek(lk_m2_file, ptr->header.ofsRenderFlags, SEEK_SET);
	fread(ptr->renderflags, sizeof(int), ptr->header.nRenderFlags, lk_m2_file);

	//Bone Lookup Table
	ptr->BoneLookupTable = malloc(ptr->header.nBoneLookupTable * sizeof(int16));
	fseek(lk_m2_file, ptr->header.ofsBoneLookupTable, SEEK_SET);
	fread(ptr->BoneLookupTable, sizeof(int16), ptr->header.nBoneLookupTable,
			lk_m2_file);

	//Texture Lookup Table
	ptr->TexLookupTable = malloc(ptr->header.nTexLookup * sizeof(short));
	fseek(lk_m2_file, ptr->header.ofsTexLookup, SEEK_SET);
	fread(ptr->TexLookupTable, sizeof(short), ptr->header.nTexLookup,
			lk_m2_file);

	//TexUnit
	ptr->TexUnit = malloc(ptr->header.nTexUnitLookup * sizeof(short));
	fseek(lk_m2_file, ptr->header.ofsTexUnitLookup, SEEK_SET);
	fread(ptr->TexUnit, sizeof(short), ptr->header.nTexUnitLookup, lk_m2_file);

	//TransLookup
	ptr->TransparencyLookup = malloc(
			ptr->header.nTransparencyLookup * sizeof(short));
	fseek(lk_m2_file, ptr->header.ofsTransparencyLookup, SEEK_SET);
	fread(ptr->TransparencyLookup, sizeof(short),
			ptr->header.nTransparencyLookup, lk_m2_file);

	//TexAnimLookup
	ptr->TexAnimLookup = malloc(ptr->header.nTexAnimLookup * sizeof(short));
	fseek(lk_m2_file, ptr->header.ofsTexAnimLookup, SEEK_SET);
	fread(ptr->TexAnimLookup, sizeof(short), ptr->header.nTexAnimLookup,
			lk_m2_file);

	//BoundingTriangles
	ptr->BoundingTriangles = malloc(
			ptr->header.nBoundingTriangles / 3 * sizeof(Triangle));
	fseek(lk_m2_file, ptr->header.ofsBoundingTriangles, SEEK_SET);
	fread(ptr->BoundingTriangles, sizeof(Triangle),
			ptr->header.nBoundingTriangles / 3, lk_m2_file);

	//BoundingVertices
	ptr->BoundingVertices = malloc(
			ptr->header.nBoundingVertices * sizeof(Vec3D));
	fseek(lk_m2_file, ptr->header.ofsBoundingVertices, SEEK_SET);
	fread(ptr->BoundingVertices, sizeof(Vec3D), ptr->header.nBoundingVertices,
			lk_m2_file);

	//BoundingNormals
	ptr->BoundingNormals = malloc(ptr->header.nBoundingNormals * sizeof(Vec3D));
	fseek(lk_m2_file, ptr->header.ofsBoundingNormals, SEEK_SET);
	fread(ptr->BoundingNormals, sizeof(Vec3D), ptr->header.nBoundingNormals,
			lk_m2_file);

	//Attachments
	read_attachments(lk_m2_file, ptr, anim_files);

	//Attachment Lookup Table
	ptr->AttachLookup = malloc(ptr->header.nAttachLookup * sizeof(short));
	fseek(lk_m2_file, ptr->header.ofsAttachLookup, SEEK_SET);
	fread(ptr->AttachLookup, sizeof(short), ptr->header.nAttachLookup,
			lk_m2_file);

	//Events
	read_events(lk_m2_file, ptr);

	//Lights
	read_lights(lk_m2_file, ptr, anim_files);

	//Cameras
	read_cameras(lk_m2_file, ptr, anim_files);

	//Cameras Lookup
	ptr->CameraLookup = malloc(ptr->header.nCameraLookup * sizeof(short));
	fseek(lk_m2_file, ptr->header.ofsCameraLookup, SEEK_SET);
	fread(ptr->CameraLookup, sizeof(short), ptr->header.nCameraLookup,
			lk_m2_file);

	//TexAnims
	read_texanims(lk_m2_file, ptr, anim_files);
	/*TODO
	 Ribbons;
	 Particles
	 */
	return 0;
}
