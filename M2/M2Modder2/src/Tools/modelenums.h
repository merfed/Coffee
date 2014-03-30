/*
 * modelenums.h
 *
 *  Created on: 11.06.2009
 *      Author: Bastian
 */

#ifndef MODELENUMS_H_
#define MODELENUMS_H_

enum eRibbonOffsets{
	refTex=0x0,
	refBlend=0x10,
	color_times=0x20,
	color_keys=0x30,
	opacity_times=0x40,
	opacity_keys=0x50,
	above_times=0x60,
	above_keys=0x70,
	below_times=0x80,
	below_keys=0x90,
	unk1_times=0xA0,
	unk1_keys=0xB0,
	unk2_times=0xC0,
	unk2_keys=0xD0,
	color_times_v=0xE0,
	color_keys_v=0xF0,
	opacity_times_v=0x100,
	opacity_keys_v=0x110,
	above_times_v=0x120,
	above_keys_v=0x130,
	below_times_v=0x140,
	below_keys_v=0x150,
	unk1_times_v=0x160,
	unk1_keys_v=0x170,
	unk2_times_v=0x180,
	unk2_keys_v=0x190,
	ribbon_ofs_size=0x1A0
};

#endif /* MODELENUMS_H_ */
