/*
 * WDT.cpp
 *
 *  Created on: 23.08.2009
 *      Author: Bastian
 */

#include "WDT.h"

WDT::WDT(){
	mver=MVER();
	mphd=MPHD();
	main=MAIN();
}

WDT::WDT(Buffer buff){
	this->buff=buff;
	this->buff.setPosition(0);
	mver=MVER(this->buff);
	Buffer tBuff=Buffer(this->buff,mver.getRealSize());
	mphd=MPHD(tBuff);
	tBuff=Buffer(tBuff,mphd.getRealSize());
	main=MAIN(tBuff);

}

void WDT::Render(){
	this->buff=mver.getChunk();
	buff+=mphd.getChunk();
	buff+=main.getChunk();
}

Buffer WDT::getFileBuff(){
	Render();
	this->buff.setPosition(0);
	return this->buff;
}

bool WDT::hasTerrain(){
	return mphd.hasTerrain();
}

void WDT::unsetTerrain(){
	mphd.unsetTerrain();
}

void WDT::setTerrain(){
	mphd.setTerrain();
}

bool WDT::hasADT(int i){
	return main.hasADT(i);
}

bool WDT::hasADT(int x,int y){
	return main.hasADT(x*64+y);
}

void WDT::setADT(int i){
	main.setADT(i);
}

void WDT::setADT(int x,int y){
	main.setADT(x*64+y);
}

void WDT::unsetADT(int i){
	main.unsetADT(i);
}
void WDT::unsetADT(int x,int y){
	main.unsetADT(x*64+y);
}

WDT::~WDT(){

}
