#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef float	Vec3D[3];
typedef float	Vec2D[2];
#include "modelheaders.h"

FILE *Input;

char	*f;
int		FileSize;
ModelHeader *header;
ModelBoneDef		*bones;
ModelRenderFlags	*rf;
ModelTextureDef		*tex;
ModelTexUnit		*texunit;
ModelView			*view;
ModelAnimation		*animations;
ModelParticleEmitterDef	*particles;
ModelRibbonEmitterDef	*ribbons;
short	*texUnitMapping;
bool changed=false;


void displayAnimBlock(AnimationBlock *block)
{
	int *Times;
	float *Keys;
	printf("Interpolation Type: %d NumRange: %d NumTimestamps: %d NumValues: %d\n",block->type,block->nRanges,block->nTimes,block->nKeys);
	Times=(int *)(f+block->ofsTimes);
	Keys=(float *)(f+block->ofsKeys);
	for(int i=0;i<block->nKeys;i++)
		printf("%d(%d)-  %.3f\n",i,Times[i],Keys[i]);
}

void displayAnimBlockFFF(AnimationBlock *block)
{
	int *Times;
	float *Keys;
	printf("Interpolation Type: %d NumRange: %d NumTimestamps: %d NumValues: %d\n",block->type,block->nRanges,block->nTimes,block->nKeys);
	Times=(int *)(f+block->ofsTimes);
	Keys=(float *)(f+block->ofsKeys);
	for(int i=0;i<block->nKeys;i++)
		printf("%d(%d)-  %8.3f %8.3f %8.3f\n",i,Times[i],Keys[i*3],Keys[i*3+1],Keys[i*3+2]);
}

void editAnimBlock(AnimationBlock *block)
{
	int i;
	float tf,*Keys;
	displayAnimBlock(block);
	Keys=(float *)(f+block->ofsKeys);
	do{
		printf("Edit which key? (-1 to return)\n");
		scanf("%d",&i);
		if((i>=0)&&(i<block->nKeys))
		{
			printf("New Value? (Cur Value %.3f)\n",Keys[i]);
			scanf("%f",&tf);
			Keys[i]=tf;
			changed=true;
		}
		else if(i!=-1)
			printf("Key %d isn't valid\n",i);
	}while(i!=-1);
}

void editAnimBlockFFF(AnimationBlock *block)
{
	int i;
	float tf,*Keys;
	displayAnimBlockFFF(block);
	Keys=(float *)(f+block->ofsKeys);
	do{
		printf("Edit which key? (-1 to return)\n");
		scanf("%d",&i);
		if((i>=0)&&(i<block->nKeys))
		{
			for(int j=0;j<3;j++)
			{
				printf("New Value %d? (Cur Value %.3f)\n",j,Keys[i*3+j]);
				scanf("%f",&tf);
				Keys[i*3+j]=tf;
			}
			changed=true;
		}
		else if(i!=-1)
			printf("Key %d isn't valid\n",i);
	}while(i!=-1);
}

void editTextureUnits()
{
	int i,j,theView;
	char t;

ViewChoose:
	printf("Choose which view (0-3):\n");
	scanf("%d",&i);
	if((i<0)||(i>3))
		goto ViewChoose;
	theView=i;
	texunit=(ModelTexUnit *)(f+view[theView].ofsTex);
	
	do
	{
		system("clear");
			printf("Texture Units\n\n");
		
		printf("Test %x %d %d\n",header->ofsTexUnitLookup,texUnitMapping[0],texUnitMapping[1]);

		for(i=0;i<view[theView].nTex;i++)		
			printf("%d - %s(%d) Mapped to %d(%d) flags %d RF=%d RFlags=%d Blend=%d\n",i,(char *)(f+tex[texunit[i].textureid].nameOfs),texunit[i].textureid,texUnitMapping[texunit[i].texunit],texunit[i].texunit,texunit[i].flags,texunit[i].flagsIndex,rf[texunit[i].flagsIndex].flags,rf[texunit[i].flagsIndex].blend);

		printf("\n t - Change Texture\n m - Change Mapping\n g - Change flag\n r - Change Render Flag Used\n f - Change Render Flag\n b - Change Render Blend\n q - Return to Main Menu\n\n");
		t=getchar();
		if(t=='r')
		{
			i=-1;

			do{
				printf("\nChange a Texture Unit Render Flag (number of tex unit or -1 to quit)\n");
				scanf ("%d",&i);
				if((i>=0)&&(i<view[theView].nTex))
				{
					printf("New Render Flag\n");
					scanf ("%d",&j);
					if((j>=0)||(j<header->nTexFlags))
					{
						texunit[i].flagsIndex=j;
						changed=true;
					}
					else
						printf("%d is an invalid render flag number\n",j);
				}
			}while(i!=-1);
		}
		else if(t=='t')
		{
			printf("Textures\n");
			for(i=0;i<header->nTextures;i++)
				printf(" %d - %s\n",i,(char *)(f+tex[i].nameOfs));

			i=-1;

			do{
				printf("\nChange a Texture Unit Texture Used (number of tex unit or -1 to quit)\n");
				scanf ("%d",&i);
				if((i>=0)&&(i<view[theView].nTex))
				{
					printf("New Texture Number\n");
					scanf ("%d",&j);
					if((j>=0)||(j<header->nTextures))
					{
						texunit[i].textureid=j;
						changed=true;
					}
					else
						printf("%d is an invalid texture number\n",j);
				}
			}while(i!=-1);
		}
		else if(t=='f')
		{
			i=-1;

			do{
				printf("\nChange a Render Flag - Flag (number of render flag or -1 to quit)\n");
				scanf ("%d",&i);
				if((i>=0)&&(i<header->nTexFlags))
				{
					printf("New Flag (1 - Unlit, 2 - Unfogged, 4 - 2 Sided, 8 ?, 16 Disable Zbuffer)\n");
					scanf ("%d",&j);
					rf[i].flags=j;
					changed=true;
				}
			}while(i!=-1);
		}
		else if(t=='g')
		{
			i=-1;

			do{
				printf("\nChange a Flag (number of texture unit or -1 to quit)\n");
				scanf ("%d",&i);
				if((i>=0)&&(i<view[theView].nTex))
				{
					printf("New Flag\n");
					scanf ("%d",&j);
					texunit[i].flags=j;
					changed=true;
				}
			}while(i!=-1);
		}
		else if(t=='m')
		{
			i=-1;

			do{
				printf("\nChange a Mapping (number of texture unit or -1 to quit)\n");
				scanf ("%d",&i);
				if((i>=0)&&(i<view[theView].nTex))
				{
					printf("New Mapping\n");
					scanf ("%d",&j);
					texunit[i].texunit=j;
					changed=true;
				}
			}while(i!=-1);
		}
		else if(t=='b')
		{
			i=-1;

			do{
				printf("\nChange a Render Flag - Blend (number of render flag or -1 to quit)\n");
				scanf ("%d",&i);
				if((i>=0)&&(i<header->nTexFlags))
				{
					printf("New Blend Mode (0 - Opaque, 1 - Alpha testing, 2 - Alpha Blending, 3 - Additive, 4 - Additive Alpha, 5 - Modulate, 6 - Deeprun Tram Glass\n");
					scanf ("%d",&j);
					rf[i].blend=j;
					changed=true;
				}
			}while(i!=-1);
		}
		else if(t=='q')
			return;
	}
	while (1);
}

void editAnimations()
{
	do{
		int i,j;
		char t;
		
		system("clear");
		printf("Animation Sequences\n");

		for(i=0;i<header->nAnimations;i++)
			printf("%d animID: %d\n",i,animations[i].animID);


		printf("\n a - Change Animation\n q - Return to Main Menu\n\n");
		t=getchar();

		if(t=='a')
		{
			i=-1;

			do{
				printf("\nChange an animID - Blend (number of animation or -1 to quit)\n");
				scanf ("%d",&i);
				if((i>=0)&&(i<header->nAnimations))
				{
					printf("New animID\n");
					scanf ("%d",&j);
					animations[i].animID=j;
					changed=true;
				}
			}while(i!=-1);
		}
		else if(t=='q')
			return;


	}while(1);
}



void printBoneTree(ModelBoneDef *B, int id)
{
	if(B->parent!=-1)
	{
		printBoneTree(&bones[B->parent],B->parent);
		printf("->");
	}
	printf("%d",id);
}

void editBone(int i)
{
	do{
		int j;
		float f;
		char t;
		system("clear");
		printf("Editing Bone %d\n",i);
		printf(" ");
		printBoneTree(&bones[i],i);
		printf("\n Pivot Point %8.3f %8.3f %8.3f\n",bones[i].pivot[0],bones[i].pivot[1],bones[i].pivot[2]);
		printf(" Flags 0x%08x\n",bones[i].flags);

		printf("\n t - Edit Translation\n r - Edit Rotation\n s - Edit Scaling\n f - Edit Flags\n p - Edit Pivot Point\n q - Return to Main Menu\n\n");
		t=getchar();

		if(t=='t')
			editAnimBlockFFF(&bones[i].translation);
		else if(t=='r')
			editAnimBlockFFF(&bones[i].rotation);
		else if(t=='s')
			editAnimBlockFFF(&bones[i].scaling);
		else if(t=='f')
		{
				printf("\nCurrent Flags 0x%08x\n",bones[i].flags);
				printf("New Flag Value\n");
				printf(" 8 - Billboarded\n");
				printf(" 200 - Transformed\n");
				scanf ("%x",&j);
				bones[i].flags=j;
				changed=true;
		}
		else if(t=='p')
		{
			printf("\nCurrent Pivot Point %8.3f %8.3f %8.3f\n",bones[i].pivot[0],bones[i].pivot[1],bones[i].pivot[2]);
			printf("New Pivot X\n");
			scanf ("%f",&bones[i].pivot[0]);
			printf("New Pivot Y\n");
			scanf ("%f",&bones[i].pivot[1]);
			printf("New Pivot Z\n");
			scanf ("%f",&bones[i].pivot[2]);
			changed=true;
		}
		else if(t=='q')
			return;


	}while(1);
}

void editBones()
{
	do{
		int i,j;
		char t;
		
		system("clear");
		printf("Bones\n");

		for(i=0;i<header->nBones;i++)
		{
			printBoneTree(&bones[i],i);
			printf("\n");
		}


		printf("\n e - Edit Bone\n q - Return to Main Menu\n\n");
		t=getchar();

		if(t=='e')
		{
			i=-1;

			do{
				printf("\nChange a Bone (number of bone or -1 to quit)\n");
				scanf ("%d",&i);
				if((i>=0)&&(i<header->nBones))
					editBone(i);
			}while(i!=-1);
		}
		else if(t=='q')
			return;


	}while(1);
}

char textureType[255];
void setTextureType(int i)
{
	if(i==0)
		strcpy(textureType,"Hard Coded");
	else if(i==1)
		strcpy(textureType,"Player Skin");
	else if(i==2)
		strcpy(textureType,"Cape");
	else if(i==6)
		strcpy(textureType,"Hair");
	else if(i==8)
		strcpy(textureType,"Tauren Fur");
	else if(i==11)
		strcpy(textureType,"Creature Skin 1");
	else if(i==12)
		strcpy(textureType,"Creature Skin 2");
	else if(i==13)
		strcpy(textureType,"Creature Skin 3");
	else
		strcpy(textureType,"Unknown");
}

void editTextures()
{
	do{
		int i,j;
		char t;
		
		system("clear");
		printf("Textures\n");

		for(i=0;i<header->nTextures;i++)
		{
			setTextureType(tex[i].type);
			
			printf("%d Type: %s (%d) Unknown: 0x%04x Flags: 0x%04x\n",i,textureType,tex[i].type,tex[i].flags>>16,tex[i].flags&0xffff);
			if(tex[i].type==0)
				printf("\tTexture: %s\n",(char *)(f+tex[i].nameOfs));
		}


		printf("\n t - Change Texture Type\n n - Change Texture File Name\n f - Change Texture Flags\n q - Return to Main Menu\n");
		t=getchar();

		if(t=='t')
		{
			i=-1;

			do{
				printf("\nChange a texture type (number of texture or -1 to quit)\n\n");
				scanf ("%d",&i);
				if((i>=0)&&(i<header->nTextures))
				{
					printf("New texture type (currently %d)\n",tex[i].type);
					printf("0 - Hard Coded\n");	
					printf("1 - Player Skin\n");
					printf("2 - Cape\n");
					printf("6 - Hair\n");
					printf("8 - Tauren Fur\n");
					printf("11 - Creature Skin 1\n");
					printf("12 - Creature Skin 2\n");
					printf("13 - Creature Skin 3\n");
					scanf ("%d",&j);
					tex[i].type=j;
					changed=true;
				}
			}while(i!=-1);
		}
		if(t=='n')
		{
			i=-1;

			do{
				printf("\nChange a texture file name (number of texture or -1 to quit)\n");
				scanf ("%d",&i);
				if((i>=0)&&(i<header->nTextures))
				{
					char newName[255];
					printf("New texture file name (currently %s max new length 15)\n",(char *)(f+tex[i].nameOfs));
					scanf ("%s",newName);
					if(strlen(newName)<16)
					{
						strcpy((char *)(f+tex[i].nameOfs),newName);
						tex[i].nameLen=strlen(newName)+1;
						changed=true;
					}
					else
						printf("Length of %s too long (%d characters while max of 15)\n",newName,strlen(newName));
					
				}
			}while(i!=-1);
		}
		if(t=='f')
		{
			i=-1;

			do{
				printf("\nChange a texture flag (number of texture or -1 to quit)\n");
				scanf ("%d",&i);
				if((i>=0)&&(i<header->nTextures))
				{
					printf("New texture type (currently 0x%04x)\n",tex[i].flags&0xffff);
					printf("1 - Texture Wrap X\n");	
					printf("2 - Texture Wrap Y\n");
					scanf ("%d",&j);
					tex[i].flags=(tex[i].flags&0xffff0000)|(j&(0xffff));
					changed=true;
				}
			}while(i!=-1);
		}
		else if(t=='q')
			return;


	}while(1);
}


void editParticles()
{
	do{
		int i,j;
		char t;
		
		system("clear");
		printf("Textures\n");

		for(i=0;i<header->nParticleEmitters;i++)
		{
			setTextureType(tex[i].type);
			
			printf("%d Flags: %02x Bone: %d Pos: %.2f %.2f %.2f Texture: %d Blend: %d Type: %d\n",i,particles[i].flags,particles[i].bone,particles[i].pos[0],particles[i].pos[1],particles[i].pos[2],particles[i].texture,particles[i].blend,particles[i].type);
			printf("\tColors %08x %08x %08x\n",particles[i].p.colors[0],particles[i].p.colors[1],particles[i].p.colors[2]);
			printf("\tSizes %.2f %.2f %.2f\n",particles[i].p.sizes[0],particles[i].p.sizes[1],particles[i].p.sizes[2]);
			printf("\tSlowdown %.2f Rotation %.2f\n",particles[i].p.slowdown,particles[i].p.rotation);
		}


		printf("\n f - Change Flags\n n - Change Bones\n p - Change Position\n r - Change Rotation\n t - Change Texture\n c - Change Colors\n b - Change Blending\n y - Change Emitter Type\n s - Change Sizes\n a - Edit Animation Block\n q - Return to Main Menu\n");
		t=getchar();
		if(t=='p')
		{
			i=-1;

			do{
				printf("\nChange a postion (number of particle or -1 to quit)\n");
				scanf ("%d",&i);
				if((i>=0)&&(i<header->nParticleEmitters))
				{
					float Pos[3];
					printf("New Position (format \"x y z\")\n");
					scanf ("%f %f %f",&Pos[0],&Pos[1],&Pos[2]);
					for(j=0;j<3;j++)
						particles[i].pos[j]=Pos[j];
					changed=true;
				}
			}while(i!=-1);
		}
		else if(t=='s')
		{
			i=-1;

			do{
				printf("\nChange the sizes (number of particle or -1 to quit)\n");
				scanf ("%d",&i);
				if((i>=0)&&(i<header->nParticleEmitters))
				{
					float Size[3];
					printf("New Sizes (format \"b m e\")\n");
					scanf ("%f %f %f",&Size[0],&Size[1],&Size[2]);
					for(j=0;j<3;j++)
						particles[i].p.sizes[j]=Size[j];
					changed=true;
				}
			}while(i!=-1);
		}
		else if(t=='a')
		{
			i=-1;
			printf("Animation Block Types\n");
			printf("0 - Emission Speed\n");
			printf("1 - Speed Variation (range 0 to 1)\n");
			printf("2 - Rotation\\Tumble Spread (range 0 to pi)\n");
			printf("3 - ? (range: 0 to 2pi)\n");
			printf("4 - Gravity\n");
			printf("5 - Lifespan\n");
			printf("6 - Emission Rate\n");
			printf("7 - Emission Area Length\n");
			printf("8 - Emission Area Width\n");
			printf("9 - Gravity? (much stronger)\n");

			do{
				printf("Edit Animation Block (number of particle or -1 to quit)\n");
				scanf ("%d",&i);
				if((i>=0)&&(i<header->nParticleEmitters))
				{
					float Pos[3];
					printf("Which Block(0-9)\n");
					scanf ("%d",&j);
					editAnimBlock(&(particles[i].params[j]));
				}
			}while(i!=-1);
		}
		if(t=='b')
		{
			i=-1;

			do{
				printf("\nChange a blend mode (number of particle or -1 to quit)\n");
				scanf ("%d",&i);
				if((i>=0)&&(i<header->nParticleEmitters))
				{
					printf("New Blend Mode (0 - Opaque, 1 - Alpha testing, 2 - Alpha Blending, 3 - Additive, 4 - Additive Alpha, 5 - Modulate, 6 - Deeprun Tram Glass\n");
					scanf ("%d",&j);
					particles[i].blend=j;
					changed=true;
				}
			}while(i!=-1);
		}
		if(t=='f')
		{
			i=-1;

			do{
				printf("\nChange flags (number of particle or -1 to quit)\n");
				scanf ("%d",&i);
				if((i>=0)&&(i<header->nParticleEmitters))
				{				
					printf("New Flag\n");
					scanf ("%x",&j);
					particles[i].flags=j;
					changed=true;
				}
			}while(i!=-1);
		}
		if(t=='r')
		{
			i=-1;

			do{
				printf("\nChange flags (number of particle or -1 to quit)\n");
				scanf ("%d",&i);
				if((i>=0)&&(i<header->nParticleEmitters))
				{				
					printf("New Rotation\n");
					scanf ("%f",&(particles[i].p.rotation));
					changed=true;
				}
			}while(i!=-1);
		}
		if(t=='n')
		{
			i=-1;

			do{
				printf("\nChange Bones (number of particle or -1 to quit)\n");
				scanf ("%d",&i);
				if((i>=0)&&(i<header->nParticleEmitters))
				{				
					printf("New Bone\n");
					scanf ("%d",&j);
					particles[i].bone=j;
					changed=true;
				}
			}while(i!=-1);
		}
		if(t=='y')
		{
			i=-1;

			do{
				printf("\nChange a emitter type (number of particle or -1 to quit)\n");
				scanf ("%d",&i);
				if((i>=0)&&(i<header->nParticleEmitters))
				{
					printf("New Emitter Type (1 - Plane 2 - Sphere 3 - Unknown\n");
					scanf ("%d",&j);
					particles[i].type=j;
					changed=true;
				}
			}while(i!=-1);
		}
		if(t=='t')
		{
			printf("Textures\n");
			for(i=0;i<header->nTextures;i++)
				printf(" %d - %s\n",i,(char *)(f+tex[i].nameOfs));

			i=-1;

			do{
				printf("\nChange a texture (number of particle or -1 to quit)\n");
				scanf ("%d",&i);
				if((i>=0)&&(i<header->nParticleEmitters))
				{
					printf("New Texture\n");

					scanf ("%d",&j);
					particles[i].texture=j;
					changed=true;
				}
			}while(i!=-1);
		}
		if(t=='c')
		{
			i=-1;

			do{
				printf("\nChange particle colors (number of particle or -1 to quit)\n");
				scanf ("%d",&i);
				if((i>=0)&&(i<header->nParticleEmitters))
				{
					printf("Current Colors %08x %08x %08x\n",particles[i].p.colors[0],particles[i].p.colors[1],particles[i].p.colors[2]);

					printf("New Start Color\n");
					scanf ("%x",&j);
					particles[i].p.colors[0]=j;

					printf("New Middle Color\n");
					scanf ("%x",&j);
					particles[i].p.colors[1]=j;

					printf("New End Color\n");
					scanf ("%x",&j);
					particles[i].p.colors[2]=j;

					changed=true;
				}
			}while(i!=-1);
		}
		if(t=='q')
			return;


	}while(1);
}



void editRibbons()
{

	do{
		char t;
		system("clear");
		printf("Ribbons\n");
		for(int i=0;i<header->nRibbonEmitters;i++)
			printf("%d Bone: %2d Pos: %6.2f %6.2f %6.2f\n",i,ribbons[i].bone,ribbons[i].pos[0],ribbons[i].pos[1],ribbons[i].pos[2]);
	
		printf("\n q - Return to Main Menu\n");
		t=getchar();
		if(t=='q')
			return;
	}	while(1);
}


int main(int argc, char **argv)
{
	char t;
	int i,j;

	if(argc<2)
	{
		printf("M2Modder V1.0 (April 7th, 2006) by John 'Cryect' Rittenhouse\n"); 
		printf(" m2modder <filename.m2>\n");
		return 0;
	}

	printf("M2Modder V1.0 (April 7th, 2006) by John 'Cryect' Rittenhouse\n"); 
	printf(" Loading Model %s\n",argv[1]);
	Input=fopen(argv[1],"rb+");

	if(Input==NULL)
	{
		printf(" ERROR: Could not load file %s\n",argv[1]);
		return 0;
	}

	fseek(Input,0,SEEK_END);
	FileSize=ftell(Input);
	fseek(Input,0,SEEK_SET);

	f=new char[FileSize];
	
	fread(f,FileSize,1,Input);
	fclose(Input);

	header=(ModelHeader *)f;
	rf=(ModelRenderFlags *)(f+header->ofsTexFlags);
	tex=(ModelTextureDef *)(f+header->ofsTextures);
	view= (ModelView*)(f+ header->ofsViews);
	texunit=(ModelTexUnit *)(f+view->ofsTex);
	animations=(ModelAnimation *)(f+header->ofsAnimations);
	particles=(ModelParticleEmitterDef *)(f+header->ofsParticleEmitters);
	bones=(ModelBoneDef *)(f+header->ofsBones);
	ribbons=(ModelRibbonEmitterDef *)(f+header->ofsRibbonEmitters);
	texUnitMapping=(short *)(f+header->ofsTexUnitLookup);


	
	do{
		system("clear");
		printf("M2Modder V1.0 (April 7th, 2006) by John 'Cryect' Rittenhouse\n"); 
		printf(" Currently Editing %s ",argv[1]);

		if(changed)
			printf("which has changed\n");
		else
			printf("which has not changed\n");

		printf("\n t - Edit Textures Definations\n u - Edit Texture Units\n b - Edit Bones\n a - Edit Animation IDs(a)\n p - Edit Particles\n r - Edit Ribbons\n q - Quit\n x - Quit Without Saving\n\n");
		t=getchar();

		if(t=='t')
			editTextures();
		else if(t=='u')
			editTextureUnits();
		else if(t=='b')
			editBones();
		else if(t=='a')
			editAnimations();
		else if(t=='p')
			editParticles();
		else if(t=='r')
			editRibbons();
		else if(t=='q')
			break;
		else if(t=='x')
			return 0;

	}while(1);
	

	if(changed)
	{
		printf("Saving Changes\n");
		Input=fopen(argv[1],"wb");
		fwrite(f,FileSize,1,Input);
		fclose(Input);
	}

	return 0;
}

