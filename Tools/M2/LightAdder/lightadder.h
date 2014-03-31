/*
 * lightadder.h
 *
 *  Created on: 21.04.2009
 *      Author: Bastian
 */
#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <stdlib.h>

#ifndef LIGHTADDER_H_
#define LIGHTADDER_H_


#endif /* LIGHTADDER_H_ */

typedef unsigned __int8 uint8;
typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
typedef __int32 int32;
typedef __int8 int8;
typedef __int16 int16;
const uint32 nLight=0x108;
const uint32 ofsLight=0x10C;

struct animfake
{
	__int16 zero;
	__int16 minusone;
	uint32 nTime;
	uint32 offsettime;
	uint32 nKey;
	uint32 offsetkey;
};

struct animsub{
	uint32 nVals;
	uint32 ofsVals;
	uint32 null1;
	uint32 null2;
};

struct light{
	uint16 type;
	int16 bone;
	float pos[3];
	animfake acolor;
	animfake aintensity;
	animfake dcolor;
	animfake dintensity;
	animfake astart;
	animfake aend;
	animfake unk1;
};

enum eOfsLight{
	acolor_time=0x0,
	acolor_key=0x10,
	aintensity_time=0x20,
	aintensity_key=0x30,
	dcolor_time=0x40,
	dcolor_key=0x50,
	dintensity_time=0x60,
	dintensity_key=0x70,
	astart_time=0x80,
	astart_key=0x90,
	aend_time=0xA0,
	aend_key=0xB0,
	unk1_time=0xC0,
	unk1_key=0xD0,
	acolor_time_v=0xE0,
	acolor_key_v=0xF0,
	aintensity_time_v=0x100,
	aintensity_key_v=0x110,
	dcolor_time_v=0x120,
	dcolor_key_v=0x130,
	dintensity_time_v=0x140,
	dintensity_key_v=0x150,
	astart_time_v=0x160,
	astart_key_v=0x170,
	aend_time_v=0x180,
	aend_key_v=0x190,
	unk1_time_v=0x1A0,
	unk1_key_v=0x1B0,
	light_end=0x1C0
};
