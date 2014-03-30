/*
 * MODF.cpp
 *
 *  Created on: 23.08.2009
 *      Author: Bastian
 */
#include "MODF.h"

MODF::MODF(){
	buff=Buffer(data);
	char m[]={'F','D','O','M'};
	Magic=MagicToInt(m);
	ChunkSize=0;
}

MODF::MODF(Buffer buff){
	this->buff=buff;
	this->buff.setPosition(magic);
	this->buff>>Magic>>ChunkSize;
	nEntries=ChunkSize/sizeof(MODF_Entry);
	entries=new MODF_Entry[nEntries];
	for(int i=0;i<nEntries;i++){
		this->buff>>entries[i];
	}
}

void MODF::Render(){
	ChunkSize=nEntries*sizeof(MODF_Entry);
	buff=Buffer(data+ChunkSize);
	buff.setPosition(magic);
	buff<<Magic;
	buff<<ChunkSize;
	buff.setPosition(data);
	for(int i=0;i<nEntries;i++){
		this->buff<<entries[i];
	}
}

Buffer MODF::getChunk(){
	Render();
	this->buff.setPosition(magic);
	return this->buff;
}

MODF::~MODF(){

}
