/*
 * MODF.h
 *
 *  Created on: 23.08.2009
 *      Author: Bastian
 */

#ifndef MODF_H_
#define MODF_H_
#include "../Chunk.h"
#include "mapheaders.h"

class MODF:public Chunk{
public:
	MODF();
	MODF(Buffer buff);
	~MODF();
	void Render();
	Buffer getChunk();
private:
	MODF_Entry *entries;
	int nEntries;
};


#endif /* MODF_H_ */
