/*
 * MCVT.h
 *
 *  Created on: 01.08.2009
 *      Author: Bastian
 */

#ifndef MCVT_H_
#define MCVT_H_
#include "../../Chunk.h"

class MCVT:public Chunk{

public:
	MCVT(Buffer buff);
	MCVT();
	~MCVT();

	float getValNoLOD(int x, int y);
	float getValLOD(int x, int y);
	float getVal(int index);


private:
	float heightvalues[145];

};


#endif /* MCVT_H_ */
