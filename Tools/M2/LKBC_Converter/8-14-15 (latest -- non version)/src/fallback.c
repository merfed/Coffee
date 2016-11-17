/**
 * @file
 */
#include <stdio.h>
#include "fallback.h"

int init = 0;
short Fallback[512] = {0};

void init_Fallback() {//Extracted from AnimationData.dbc (TBC 2.4.3)
	init = 1;
	Fallback[0] = 147;
	Fallback[6] = 1;
	Fallback[8] = 25;
	Fallback[9] = 8;
	Fallback[10] = 9;
	Fallback[13] = 4;
	Fallback[17] = 16;
	Fallback[18] = 17;
	Fallback[19] = 18;
	Fallback[20] = 8;
	Fallback[21] = 8;
	Fallback[22] = 8;
	Fallback[23] = 8;
	Fallback[24] = 8;
	Fallback[26] = 25;
	Fallback[27] = 25;
	Fallback[28] = 25;
	Fallback[30] = 8;
	Fallback[32] = 16;
	Fallback[33] = 32;
	Fallback[36] = 8;
	Fallback[42] = 4;
	Fallback[45] = 42;
	Fallback[51] = 52;
	Fallback[52] = 31;
	Fallback[53] = 54;
	Fallback[54] = 33;
	Fallback[55] = 16;
	Fallback[57] = 17;
	Fallback[58] = 18;
	Fallback[59] = 87;
	Fallback[71] = 100;
	Fallback[85] = 17;
	Fallback[86] = 19;
	Fallback[87] = 88;
	Fallback[88] = 16;
	Fallback[95] = 16;
	Fallback[97] = 96;
	Fallback[98] = 96;
	Fallback[100] = 99;
	Fallback[101] = 99;
	Fallback[107] = 16;
	Fallback[115] = 114;
	Fallback[116] = 114;
	Fallback[117] = 87;
	Fallback[118] = 57;
	Fallback[119] = 4;
	Fallback[123] = 128;
	Fallback[124] = 52;
	Fallback[125] = 31;
	Fallback[129] = 128;
	Fallback[131] = 1;
	Fallback[132] = 131;
	Fallback[135] = 42;
	Fallback[136] = 62;
	Fallback[137] = 14;
	Fallback[138] = 63;
	Fallback[141] = 115;
	Fallback[143] = 5;
	//Fallback[146] = 148;//Loop
	Fallback[146] = 0;//FIXME
	Fallback[147] = 146;
	Fallback[148] = 146;
	Fallback[149] = 148;
	Fallback[150] = 148;
	Fallback[151] = 150;
	Fallback[152] = 150;
	Fallback[187] = 5;
	Fallback[188] = 50;
	Fallback[189] = 50;
	Fallback[196] = 1;
	Fallback[197] = 69;
	Fallback[199] = 61;
	Fallback[203] = 115;
	Fallback[208] = 60;
	Fallback[209] = 84;
	Fallback[210] = 113;
	Fallback[211] = 69;
	Fallback[212] = 16;
	Fallback[223] = 119;
	Fallback[224] = 127;
}

int get_RealPos(int pos, LKModelAnimation *animations) {
	if((animations[pos].flags & 0x40) == 0){
		return pos;
	}
	return get_RealPos(animations[pos].Index, animations);
}

short get_RealID(short ID, BCM2 model) {
	if (init == 0) {
		init_Fallback();
	}
	if (ID >= 226) {
		fprintf(stderr, "Error : Invalid ID (>226) in get_RealID().");
		return 0; //Not a Burning Crusade ID
	}
	if (ID < model.header.nAnimationLookup && (model.AnimLookup[ID] > -1)) {
		return ID;
	}
	return get_RealID(Fallback[ID], model);
}
