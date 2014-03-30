/*
 * WoWFile.cpp
 *
 *  Created on: 05.08.2009
 *      Author: Bastian
 */

#include "WoWFile.h"

WoWFile::WoWFile(){
	_Tn("WoWFile::WoWFile()");

}

WoWFile::WoWFile(Buffer buff){
	_Tn("WoWFile::WoWFile(Buffer buff)");
	this->buff=buff;
	this->buff.setPosition(0);
	mver=MVER(this->buff);
}

Buffer WoWFile::getFileBuff(){
	Render();
	this->buff.setPosition(0);
	return this->buff;
}

void WoWFile::Render(){
	_Tn("void WoWFile::Render()");
	buff=Buffer();
	buff+=mver.getChunk();

}

WoWFile::~WoWFile(){
	_Tn("WoWFile::~WoWFile()");

}
