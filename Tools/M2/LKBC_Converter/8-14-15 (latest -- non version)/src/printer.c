/**
 * @file
 * Functions to print M2/Skin structures.
 * Primary use is debugging.
 */
#include <stdio.h>
#include "printer.h"

short tt_init = 0;
char *textypes[19];
void init_textypes() {
	textypes[0] = "Hardcoded";
	textypes[1] = "Skin -- Body and clothes";
	textypes[2] = "Object Skin -- Item, Capes";
	textypes[3] = "Weapon Blade -- Possibly Armor Reflect";
	textypes[4] = "Weapon Handle";
	textypes[5] = "Environment";
	textypes[6] = "Character Hair";
	textypes[7] = "Character Facial Hair";
	textypes[8] = "Skin Extra";
	textypes[9] = "UI Skin";
	textypes[10] = "Tauren Mane";
	textypes[11] = "Monster Skin 1 -- Skin for Creatures or GameObjects";
	textypes[12] = "Monster Skin 2 -- Skin for Creatures or GameObjects";
	textypes[13] = "Monster Skin 3 -- Skin for Creatures or GameObjects";
	textypes[14] = "Item Icon";
	textypes[15] = "Guild Background Color";
	textypes[16] = "Guild Emblem Color";
	textypes[17] = "Guild Border Color";
	textypes[18] = "Guild Emblem";
	tt_init = 1;
}
char* get_textype(int i) {
	if (i < 19) {
		if (tt_init == 0) {
			init_textypes();
		}
		return textypes[i];
	} else {
		return "Unknown";
	}
}

/**
 * Print animations of a M2/WotLK model.
 * @param model The M2/WotLK structure.
 * Extract it from a file first with reading functions.
 */
void print_anims_lk(LKM2 model) {
	printf("nAnimations: %d\n", model.header.nAnimations);
	int i;
	for (i = 0; i < model.header.nAnimations; i++) {
		printf("[Animation #%d]\n", i);
		printf("AnimID: %d\n", model.animations[i].animID);
		printf("SubAnimID: %d\n", model.animations[i].subAnimID);
		printf("Length: %d\n", model.animations[i].length);
		printf("MoveSpeed: %f\n", model.animations[i].moveSpeed);
		printf("Flags: %d\n", model.animations[i].flags);
		printf("Probability: %d\n", model.animations[i].probability);
		printf("Unused: %d\n", model.animations[i].unused);
		printf("Unknown 1: %d\n", model.animations[i].d1);
		printf("Unknown 2: %d\n", model.animations[i].d2);
		printf("playSpeed: %d\n", model.animations[i].playSpeed);
		printf("MinimumExtent: (%f,%f,%f)\n", model.animations[i].boxA[0],
				model.animations[i].boxA[1], model.animations[i].boxA[2]);
		printf("MaximumExtent: (%f,%f,%f)\n", model.animations[i].boxB[0],
				model.animations[i].boxB[1], model.animations[i].boxB[2]);
		printf("Radius: %f\n", model.animations[i].rad);
		printf("NextAnimation: %d\n", model.animations[i].NextAnimation);
		printf("Index: %d\n", model.animations[i].Index);
		printf("\n");
	}
}

/**
 * Print animations of a M2/BC model.
 * @param model The M2/BC structure.
 */
void print_anims_bc(BCM2 model) {
	printf("nAnimations: %d\n", model.header.nAnimations);
	int i;
	for (i = 0; i < model.header.nAnimations; i++) {
		printf("[Animation #%d]\n", i);
		printf("AnimID: %d\n", model.animations[i].animID);
		printf("SubAnimID: %d\n", model.animations[i].subAnimID);
		printf("timeStart: %d\n", model.animations[i].timeStart);
		printf("timeEnd: %d\n", model.animations[i].timeEnd);
		printf("MoveSpeed: %f\n", model.animations[i].moveSpeed);
		printf("Flags: %d\n", model.animations[i].flags);
		printf("Probability: %d\n", model.animations[i].probability);
		printf("Unused: %d\n", model.animations[i].unused);
		printf("Unknown 1: %d\n", model.animations[i].d1);
		printf("Unknown 2: %d\n", model.animations[i].d2);
		printf("playSpeed: %d\n", model.animations[i].playSpeed);
		printf("MinimumExtent: (%f,%f,%f)\n", model.animations[i].boxA[0],
				model.animations[i].boxA[1], model.animations[i].boxA[2]);
		printf("MaximumExtent: (%f,%f,%f)\n", model.animations[i].boxB[0],
				model.animations[i].boxB[1], model.animations[i].boxB[2]);
		printf("Radius: %f\n", model.animations[i].rad);
		printf("NextAnimation: %d\n", model.animations[i].NextAnimation);
		printf("Index: %d\n", model.animations[i].Index);
		printf("\n");
	}
}

/**
 * Print a Skin structure.
 * @param skin
 */
void print_skin(Skin skin) {
	printf("[Skin #%d]\n", skin.header.ID);
	printf("nIndices : %d\n", skin.header.nIndices);
	printf("nTriangles : %d\n", skin.header.nTriangles);
	printf("nProperties : %d\n", skin.header.nProperties);
	printf("nSubmeshes : %d\n", skin.header.nSubmeshes);
	printf("nTextureUnits : %d\n", skin.header.nTextureUnits);
	printf("\n");
}

/**
 * Print the Views of a BC model.
 * @param model
 */
void print_views(BCM2 model) {
	int i;
	for (i = 0; i < model.header.nViews; i++) {
		int j;
		printf("[View #%d]\n", i);
		printf("nIndices : %d\n", model.views[i].header.nIndices);
		/*
		 for (j=0;j<model.views[i].header.nIndices;j++){
		 printf("[%d]%d\n ",j,model.views[i].Indices[j]);
		 }
		 */

		printf("nTriangles %d, so the real number is %d\n",
				model.views[i].header.nTriangles,
				model.views[i].header.nTriangles / 3);

		printf("nProperties : %d\n", model.views[i].header.nProperties);

		printf("nSubmeshes : %d\n", model.views[i].header.nSubmeshes);
		for (j = 0; j < model.views[i].header.nSubmeshes; j++) {
			printf("\t[Submesh #%d]\n", j);
			printf("\tID : %d\n", model.views[i].Submeshes[j].ID);
			printf("\tLevel : %d\n", model.views[i].Submeshes[j].Level);
			printf("\tStartVertex : %d\n",
					model.views[i].Submeshes[j].StartVertex);
			printf("\tnVertices : %d\n", model.views[i].Submeshes[j].nVertices);
			printf("\tnTriangles : %d\n",
					model.views[i].Submeshes[j].nTriangles);
			printf("\tnBones : %d\n", model.views[i].Submeshes[j].nBones);
			printf("\tStartBones : %d\n",
					model.views[i].Submeshes[j].StartBones);
			printf("\tRootBone : %d\n", model.views[i].Submeshes[j].RootBone);
			printf("\n");
		}

		printf("nTexUnit : %d\n", model.views[i].header.nTextureUnits);
		printf("\n");
	}
}

/**
 * Print the Bones from a BC model
 * @param model
 * @param flags What is printed depends on the three lower bits. Order : translation, rotation, scaling.
 * Example (prints translation and scaling) : 0b101, so it's 5. Values go from 0(0b000, nothing) to 7(0b111, everything).
 */
void print_bones_bc(BCM2 model, char flags) {
	int i;
	for (i = 0; i < model.header.nBones; i++) {
		printf("[Bone #%d]\n", i);
		int j;
		if (flags & 4) {
			printf("Translation:\n");
			printf("\tNumber of Ranges: %d\n", model.bones[i].trans.Ranges.n);
			for (j = 0; j < model.bones[i].trans.Ranges.n; j++) {
				printf("\t\t(%d, %d)\n", model.bonesdata[i].trans.ranges[j][0],
						model.bonesdata[i].trans.ranges[j][1]);
			}
			printf("\tNumber of Timestamps: %d", model.bones[i].trans.Times.n);
			printf("\t Number of Keys: %d\n", model.bones[i].trans.Keys.n);
			for (j = 0; j < model.bones[i].trans.Times.n; j++) {
				printf("\t\t%d: (%f, %f, %f)\n",
						model.bonesdata[i].trans.times[j],
						model.bonesdata[i].trans.keys[j][0],
						model.bonesdata[i].trans.keys[j][1],
						model.bonesdata[i].trans.keys[j][2]);
			}
		}

		if (flags & 2) {
			printf("Rotation:\n");
			printf("\tNumber of Ranges: %d\n", model.bones[i].rot.Ranges.n);
			for (j = 0; j < model.bones[i].rot.Ranges.n; j++) {
				printf("\t\t(%d,%d)\n", model.bonesdata[i].rot.ranges[j][0],
						model.bonesdata[i].rot.ranges[j][1]);
			}
			printf("\tNumber of Timestamps: %d", model.bones[i].rot.Times.n);
			printf("\t Number of Keys: %d\n", model.bones[i].rot.Keys.n);
			for (j = 0; j < model.bones[i].rot.Times.n; j++) {
				printf("\t\t%d: (%d, %d, %d, %d)\n",
						model.bonesdata[i].rot.times[j],
						model.bonesdata[i].rot.keys[j][0],
						model.bonesdata[i].rot.keys[j][1],
						model.bonesdata[i].rot.keys[j][2],
						model.bonesdata[i].rot.keys[j][3]);
			}
		}

		if (flags & 1) {
			printf("Scaling:\n");
			printf("\tNumber of Ranges: %d\n", model.bones[i].scal.Ranges.n);
			for (j = 0; j < model.bones[i].scal.Ranges.n; j++) {
				printf("\t\t(%d, %d)\n", model.bonesdata[i].scal.ranges[j][0],
						model.bonesdata[i].scal.ranges[j][1]);
			}
			printf("\tNumber of Timestamps: %d", model.bones[i].scal.Times.n);
			printf("\t Number of Keys: %d\n", model.bones[i].scal.Keys.n);
			for (j = 0; j < model.bones[i].scal.Times.n; j++) {
				printf("\t\t%d: (%f, %f, %f)\n",
						model.bonesdata[i].scal.times[j],
						model.bonesdata[i].scal.keys[j][0],
						model.bonesdata[i].scal.keys[j][1],
						model.bonesdata[i].scal.keys[j][2]);
			}
		}
		printf("\n");
	}
}

void print_cameras_bc(BCM2 model) {
	printf("Number of camera references : %d\n", model.header.nCameras);
	int i;
	for (i = 0; i < model.header.nCameras; i++) {
		printf("[Camera #%d]\n", i);
		printf("Position Vector: (%f,%f,%f)\n", model.cameras[i].position[0],
				model.cameras[i].position[1], model.cameras[i].position[2]);
		printf("TransPos:\n");
		printf("\tNumber of Ranges: %d\n", model.cameras[i].transpos.Ranges.n);
		int j;
		for (j = 0; j < model.cameras[i].transpos.Ranges.n; j++) {
			printf("\t\t(%d, %d)\n", model.camerasdata[i].transpos.ranges[j][0],
					model.camerasdata[i].transpos.ranges[j][1]);
		}
		printf("\tNumber of Timestamps: %d", model.cameras[i].transpos.Times.n);
		printf("\t Number of Keys: %d\n", model.cameras[i].transpos.Keys.n);
		for (j = 0; j < model.cameras[i].transpos.Times.n; j++) {
			printf("\t\t%d: ((%f,%f,%f),(%f,%f,%f),(%f,%f,%f)\n",
					model.camerasdata[i].transpos.times[j],
					model.camerasdata[i].transpos.keys[j][0][0],
					model.camerasdata[i].transpos.keys[j][0][1],
					model.camerasdata[i].transpos.keys[j][0][2],
					model.camerasdata[i].transpos.keys[j][1][0],
					model.camerasdata[i].transpos.keys[j][1][1],
					model.camerasdata[i].transpos.keys[j][1][2],
					model.camerasdata[i].transpos.keys[j][2][0],
					model.camerasdata[i].transpos.keys[j][2][1],
					model.camerasdata[i].transpos.keys[j][2][2]);
		}
		printf("\n");
	}
}
void print_transparency_bc(BCM2 model) {
	printf("Number of transparency references : %d\n",
			model.header.nTransparency);
	int i;
	for (i = 0; i < model.header.nTransparency; i++) {
		printf("[Transparency #%d]\n", i);
		int j;
		printf("Alpha:\n");
		printf("\tNumber of Ranges: %d\n",
				model.transparencyrefs[i].alpha.Ranges.n);
		for (j = 0; j < model.transparencyrefs[i].alpha.Ranges.n; j++) {
			printf("\t\t(%d, %d)\n",
					model.transparencydata[i].alpha.ranges[j][0],
					model.transparencydata[i].alpha.ranges[j][1]);
		}
		printf("\tNumber of Timestamps: %d",
				model.transparencyrefs[i].alpha.Times.n);
		printf("\t Number of Keys: %d\n",
				model.transparencyrefs[i].alpha.Keys.n);
		for (j = 0; j < model.transparencyrefs[i].alpha.Times.n; j++) {
			printf("\t\t%d: %d\n", model.transparencydata[i].alpha.times[j],
					model.transparencydata[i].alpha.keys[j]);
		}
		printf("\n");
	}
}
void print_transparency_lk(LKM2 model) {
	int i;
	for (i = 0; i < model.header.nTransparency; i++) {
		int j;
		printf("[Transparency #%d]\n", i);
		//Translation
		if (model.transparencyrefs[i].alpha.Keys.n > 0) {
			for (j = 0; j < model.transparencyrefs[i].alpha.Keys.n; j++) {
				printf("\t LKAnimation : %d\n", j);
				if (model.transparencyanimofs[i].alpha.keys[j].n > 0) {
					int k;
					for (k = 0;
							k < model.transparencyanimofs[i].alpha.keys[j].n;
							k++) {
						printf("\t\tLKValue : %d\n",
								model.transparencydata[i].alpha[j].keys[k]);
					}
				}
			}
		}
		printf("\n");
	}
}
void print_events_lk(LKM2 model) {
	int i;
	for (i = 0; i < model.header.nEvents; i++) {
		int j;
		printf("[Event #%d]\n", i);
		char terminatedID[5] = {0};
		terminatedID[0] = model.events[i].ID[0];
		terminatedID[1] = model.events[i].ID[1];
		terminatedID[2] = model.events[i].ID[2];
		terminatedID[3] = model.events[i].ID[3];
		printf("ID: %s\n", terminatedID);
		printf("Data: %d\n", model.events[i].data);
		printf("Bone: %d\n", model.events[i].bone);
		printf("Position Vector: (%f,%f,%f)\n", model.events[i].position[0],
				model.events[i].position[1], model.events[i].position[2]);
		printf("Type: %d\n", model.events[i].timer.type);
		printf("Seq: %d\n", model.events[i].timer.seq);
		printf("Number of times: %d\n", model.events[i].timer.Times.n);
		//Content
		if (model.events[i].timer.Times.n > 0) {
			for (j = 0; j < model.events[i].timer.Times.n; j++) {
				printf("\t Animation : %d\n", j);
				if (model.eventsanimofs[i].times[j].n > 0) {
					int k;
					for (k = 0; k < model.eventsanimofs[i].times[j].n; k++) {
						printf("\t\tOccuring Time : %d\n",
								model.eventsdata[i].times[j][k]);
					}
				}
			}
		}
		printf("\n");
	}
}

void print_events_bc(BCM2 model) {
	int i;
	for (i = 0; i < model.header.nEvents; i++) {
		printf("[Event #%d]\n", i);
		char terminatedID[5] = {0};
		terminatedID[0] = model.events[i].ID[0];
		terminatedID[1] = model.events[i].ID[1];
		terminatedID[2] = model.events[i].ID[2];
		terminatedID[3] = model.events[i].ID[3];
		printf("ID: %s\n", terminatedID);
		printf("Data: %d\n", model.events[i].data);
		printf("Bone: %d\n", model.events[i].bone);
		printf("Position Vector: (%f,%f,%f)\n", model.events[i].position[0],
				model.events[i].position[1], model.events[i].position[2]);
		printf("Type: %d\n", model.events[i].timer.type);
		printf("Seq: %d\n", model.events[i].timer.seq);
		printf("Number of times: %d\n", model.events[i].timer.Times.n);
		//Content
		if (model.events[i].timer.Ranges.n > 0) {
			printf("\tRanges:\n");
			int j;
			for (j = 0; j < model.events[i].timer.Ranges.n; j++) {
				printf("\t(%d,%d)\n", model.eventsdata[i].ranges[j][0],
						model.eventsdata[i].ranges[j][1]);
			}
			printf("\n");
		}
		if (model.events[i].timer.Times.n > 0) {
			printf("\tTimes:\n");
			int j;
			for (j = 0; j < model.events[i].timer.Times.n; j++) {
				printf("\t%d\n", model.eventsdata[i].times[j]);
			}
		}
		printf("\n");
	}
}

void print_attachments_bc(BCM2 model) {
	int i;
	for (i = 0; i < model.header.nAttachments; i++) {
		printf("[Attachment #%d]\n", i);
		printf("ID: %d\n", model.attachments[i].ID);
		printf("Bone: %d\n", model.attachments[i].bone);
		printf("Position Vector: (%f,%f,%f)\n", model.attachments[i].position[0],
				model.attachments[i].position[1], model.attachments[i].position[2]);
		printf("Type: %d\n", model.attachments[i].data.type);
		printf("Seq: %d\n", model.attachments[i].data.seq);
		printf("Number of ranges: %d\n", model.attachments[i].data.Ranges.n);
		printf("Number of times & keys: %d\n", model.attachments[i].data.Times.n);
		//Data
		if (model.attachments[i].data.Ranges.n > 0) {
			printf("\tRanges:\n");
			int j;
			for (j = 0; j < model.attachments[i].data.Ranges.n; j++) {
				printf("\t(%d,%d)\n", model.attachmentsdata[i].data.ranges[j][0],
						model.attachmentsdata[i].data.ranges[j][1]);
			}
			printf("\n");
		}
		if (model.attachments[i].data.Times.n > 0) {
			printf("\tTimes and keys:\n");
			int j;
			for (j = 0; j < model.attachments[i].data.Times.n; j++) {
				printf("\t\t%d: (%d)\n",
				model.attachmentsdata[i].data.times[j], model.attachmentsdata[i].data.keys[j]);
			}
		}
		printf("\n");
	}
}

/**
 * ATM print Translation timestamps of every bone in a LK model
 * @param model
 */
void print_bones_lk(LKM2 model) {
	int i;
	for (i = 0; i < model.header.nBones; i++) {
		LKModelBoneDef lk_bone = model.bones[i];
		int j;
		printf("LKBone : %d\n", i);
		//Translation
		if (lk_bone.trans.Keys.n > 0) {
			for (j = 0; j < lk_bone.trans.Keys.n; j++) {
				printf("\t LKAnimation : %d\n", j);
				if (model.animofs[i].trans.keys[j].n > 0) {
					int k;
					for (k = 0; k < model.animofs[i].trans.keys[j].n; k++) {
						printf("\t\t%d : (%f,%f,%f)\n",
								model.bonesdata[i].trans[j].times[k],
								model.bonesdata[i].trans[j].keys[k][0],
								model.bonesdata[i].trans[j].keys[k][1],
								model.bonesdata[i].trans[j].keys[k][2]);
					}
				}
			}
		}
	}
}

/**
 * Print the Vertices.
 * @param model
 */
void print_vertices_lk(LKM2 model) {
	int i;
	for (i = 0; i < model.header.nVertices; i++) {
		printf("%d : (%f,%f,%f)\n", i, model.vertices[i].pos[0],
				model.vertices[i].pos[1], model.vertices[i].pos[2]);
	}
}

/**
 * Print the Vertices.
 * @param model
 */
void print_vertices_bc(BCM2 model) {
	int i;
	for (i = 0; i < model.header.nVertices; i++) {
		printf("%d : (%f,%f,%f)\n", i, model.vertices[i].pos[0],
				model.vertices[i].pos[1], model.vertices[i].pos[2]);
	}
}

void print_animlookup_bc(BCM2 model) {
	printf("[Animation Lookup]\n");
	printf("PhysicalID \t-\t AnimID\n");
	int i;
	for (i = 0; i < model.header.nAnimationLookup; i++) {
		printf("%d \t\t|\t %d\n", model.AnimLookup[i], i);
	}
}
void print_playanimlookup_bc(BCM2 model) {
	printf("[Playable Animation Lookup]\n");
	printf("AnimID,Flags \t-\t In game ID\n");
	int i;
	for (i = 0; i < model.header.nPlayableAnimationLookup; i++) {
		printf("%d,%d \t\t|\t %d\n", model.PlayAnimLookup[i].ID,
				model.PlayAnimLookup[i].flags, i);
	}
}

void print_attachlookup_bc(BCM2 model) {
	printf("[Attachment Lookup]\n");
	printf("PhysicalID \t-\t AttachID\n");
	int i;
	for (i = 0; i < model.header.nAttachLookup; i++) {
		printf("%d \t\t|\t %d\n", model.AttachLookup[i], i);
	}
}
void print_texnames_bc(BCM2 model) {
	int i;
	for (i = 0; i < model.header.nTextures; i++) {
		printf("[Texture #%d]\n", i);
		if (model.textures_def[i].type == 0) {
			printf("\t%s\n", model.texture_names[i]);
		} else {
			printf(
					"\tDefined by DBC files CharSections.dbc, CreatureDisplayInfo.dbc or ItemDisplayInfo.dbc.\n");
			printf("\tType : %s.\n", get_textype(model.textures_def[i].type));
		}
	}
	printf("\n");
}
void print_texnames_lk(LKM2 model) {
	int i;
	for (i = 0; i < model.header.nTextures; i++) {
		printf("[Texture #%d]\n", i);
		if (model.textures_def[i].type == 0) {
			printf("\t%s\n", model.texture_names[i]);
		} else {
			printf(
					"\tDefined by DBC files CharSections.dbc, CreatureDisplayInfo.dbc or ItemDisplayInfo.dbc.\n");
			printf("\tType : %s\n", get_textype(model.textures_def[i].type));
		}
	}
}

void print_boneheaders_lk(LKM2 model) {
	int i;
	for (i = 0; i < model.header.nBones; i++) {
		printf("[Bone #%d]\n", i);
		printf("ID: %d\n", model.bones[i].animid);
		printf("Flags: %d\n", model.bones[i].flags);
		printf("ParentBone: %d\n", model.bones[i].parent);
		printf("SubmeshID: %d\n", model.bones[i].geoid);
		printf("Unknown: %d\n", model.bones[i].unk);

		printf("[Translation]\n");
		printf("\tType: %d\n", model.bones[i].trans.type);
		printf("\tSeq: %d\n", model.bones[i].trans.seq);
		printf("\tNumber of times: %d\n", model.bones[i].trans.Times.n);
		printf("\tOffset of times: %d\n", model.bones[i].trans.Times.ofs);
		printf("\tNumber of keys: %d\n", model.bones[i].trans.Keys.n);
		printf("\tOffset of keys: %d\n", model.bones[i].trans.Keys.ofs);

		printf("[Rotation]\n");
		printf("\tType: %d\n", model.bones[i].rot.type);
		printf("\tSeq: %d\n", model.bones[i].rot.seq);
		printf("\tNumber of times: %d\n", model.bones[i].rot.Times.n);
		printf("\tOffset of times: %d\n", model.bones[i].rot.Times.ofs);
		printf("\tNumber of keys: %d\n", model.bones[i].rot.Keys.n);
		printf("\tOffset of keys: %d\n", model.bones[i].rot.Keys.ofs);

		printf("[Scaling]\n");
		printf("\tType: %d\n", model.bones[i].scal.type);
		printf("\tSeq: %d\n", model.bones[i].scal.seq);
		printf("\tNumber of times: %d\n", model.bones[i].scal.Times.n);
		printf("\tOffset of times: %d\n", model.bones[i].scal.Times.ofs);
		printf("\tNumber of keys: %d\n", model.bones[i].scal.Keys.n);
		printf("\tOffset of keys: %d\n", model.bones[i].scal.Keys.ofs);

		printf("Pivot Vector: (%f,%f,%f)\n", model.bones[i].pivot[0],
				model.bones[i].pivot[1], model.bones[i].pivot[2]);
		printf("\n");
	}
}
void print_boneheaders_bc(BCM2 model) {
	int i;
	for (i = 0; i < model.header.nBones; i++) {
		printf("[Bone #%d]\n", i);
		printf("ID: %d\n", model.bones[i].animid);
		printf("Flags: %d\n", model.bones[i].flags);
		printf("ParentBone: %d\n", model.bones[i].parent);
		printf("SubmeshID: %d\n", model.bones[i].geoid);
		printf("Unknown: %d\n", model.bones[i].unk);

		printf("[Translation]\n");
		printf("\tType: %d\n", model.bones[i].trans.type);
		printf("\tSeq: %d\n", model.bones[i].trans.seq);
		printf("\tNumber of ranges: %d\n", model.bones[i].trans.Ranges.n);
		printf("\tOffset of ranges: %d\n", model.bones[i].trans.Ranges.ofs);
		printf("\tNumber of times: %d\n", model.bones[i].trans.Times.n);
		printf("\tOffset of times: %d\n", model.bones[i].trans.Times.ofs);
		printf("\tNumber of keys: %d\n", model.bones[i].trans.Keys.n);
		printf("\tOffset of keys: %d\n", model.bones[i].trans.Keys.ofs);

		printf("[Rotation]\n");
		printf("\tType: %d\n", model.bones[i].rot.type);
		printf("\tSeq: %d\n", model.bones[i].rot.seq);
		printf("\tNumber of ranges: %d\n", model.bones[i].rot.Ranges.n);
		printf("\tOffset of ranges: %d\n", model.bones[i].rot.Ranges.ofs);
		printf("\tNumber of times: %d\n", model.bones[i].rot.Times.n);
		printf("\tOffset of times: %d\n", model.bones[i].rot.Times.ofs);
		printf("\tNumber of keys: %d\n", model.bones[i].rot.Keys.n);
		printf("\tOffset of keys: %d\n", model.bones[i].rot.Keys.ofs);

		printf("[Scaling]\n");
		printf("\tType: %d\n", model.bones[i].scal.type);
		printf("\tSeq: %d\n", model.bones[i].scal.seq);
		printf("\tNumber of ranges: %d\n", model.bones[i].scal.Ranges.n);
		printf("\tOffset of ranges: %d\n", model.bones[i].scal.Ranges.ofs);
		printf("\tNumber of times: %d\n", model.bones[i].scal.Times.n);
		printf("\tOffset of times: %d\n", model.bones[i].scal.Times.ofs);
		printf("\tNumber of keys: %d\n", model.bones[i].scal.Keys.n);
		printf("\tOffset of keys: %d\n", model.bones[i].scal.Keys.ofs);

		printf("Pivot Vector: (%f,%f,%f)\n", model.bones[i].pivot[0],
				model.bones[i].pivot[1], model.bones[i].pivot[2]);
		printf("\n");
	}
}

/**
 * Print binary for any datatype
 * @param size
 * @param ptr
 * @author Enno Groper
 */
//assumes little endian
void printBits(size_t const size, void const * const ptr) {
	unsigned char *b = (unsigned char*) ptr;
	unsigned char byte;
	int i, j;
	for (i = size - 1; i >= 0; i--) {
		for (j = 7; j >= 0; j--) {
			byte = b[i] & (1 << j);
			byte >>= j;
			printf("%u", byte);
		}
	}
	puts("");
}
