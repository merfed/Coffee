/*
 * WDT.h
 *
 *  Created on: 23.08.2009
 *      Author: Bastian
 */

#ifndef WDT_H_
#define WDT_H_
#include "../WoWFile.h"
#include "../MVER.h"
#include "MPHD.h"
#include "MAIN.h"

class WDT:public WoWFile{
public:
	WDT();
	WDT(Buffer buff);
	~WDT();
	void Render();
	Buffer getFileBuff();

	bool hasTerrain();

	void unsetTerrain();
	void setTerrain();

	bool hasADT(int i);
	bool hasADT(int x,int y);

	void setADT(int i);
	void setADT(int x,int y);

	void unsetADT(int i);
	void unsetADT(int x,int y);

private:
	MVER mver;
	MPHD mphd;
	MAIN main;
};


#endif /* WDT_H_ */
