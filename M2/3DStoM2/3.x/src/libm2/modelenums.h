/*
 * modelenums.h
 *
 *  Created on: 11.06.2009
 *      Author: Bastian
 */

#ifndef MODELENUMS_H_
#define MODELENUMS_H_

enum GlobalModelFlags{
	Global_Tilt_X	=	0x1,
	Global_Tilt_Y	=	0x2,
	Global_unk1	=	0x4,
	Global_AdditionalField	=	0x8,
	Global_unk2	=	0x10
};

enum AnimationFlags{
	Anim_Every_Sequence	=	0x20//One thing I saw in the source is that "-1 animationblocks" in bones wont get parsed if 0x20 is not set.
};

enum BoneFlags{
	Bone_Billboarded	=	0x8,
	Bone_Transformed	=	0x200
};

enum RenderFlags{
	Render_Unlit	=	0x1,
	Render_Unfogged	=	0x2,
	Render_Two_Sided	=	0x4,
	Render_Billboarded	=	0x8, //???
	Render_Disable_Z_Buff	=	0x10 //???
};

enum BlendModes{
	Blend_Opaque	=	0,
	Blend_Mod	=	1,
	Blend_Decal	=	2,
	Blend_Add	=	3,
	Blend_Mod2x	=	4,
	Blend_Fade	=	5,
	Blend_Deeprun	=	6
};

enum LightTypes{
	Light_Directional	=	0,
	Light_Point	=	1
};

enum CameraTypes{
	Camera_Nothing	=	-1,
	Camera_Potrait	=	0,
	Camera_CharInfo	=	1
};

enum ParticleFlags{
	Particle_Do_Not_Trail	=	0x10,
	Particle_Do_Not_Billboard	=	0x1000
};

#endif /* MODELENUMS_H_ */
