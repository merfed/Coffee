/*
 * MCVT.cpp
 *
 *  Created on: 01.08.2009
 *      Author: Bastian
 */
#include "MCVT.h"

MCVT::MCVT(Buffer buff){
		_Tn("buff-Konstruktor");
		this->buff=buff;
		this->buff.setPosition(magic);
		this->buff >> Magic >> ChunkSize;
		this->buff.setPosition(data);
		for(int i=0;i<145;i++){
			this->buff>>heightvalues[i];
		}
}

MCVT::MCVT(){
	buff=Buffer(data+145*sizeof(int));
	char m[]={'T','V','C','M'};
	Magic=MagicToInt(m);
	ChunkSize=0;
}

float MCVT::getValLOD(int x,int y){
	int add = (y+1)*9;
	return heightvalues[y*8 + x + add];
}

float MCVT::getValNoLOD(int x,int y){
	int add = y*8;
	return heightvalues[y*9 + x + add];
}

float MCVT::getVal(int index){
	return heightvalues[index];
}

MCVT::~MCVT(){

}
