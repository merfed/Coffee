/*
 * modelhelpstructs.h
 *
 *  Created on: 11.06.2009
 *      Author: Bastian
 */

#ifndef MODELHELPSTRUCTS_H_
#define MODELHELPSTRUCTS_H_

struct RibbonHelper{
	int texture;
	int blend;
	float color[3];
	unsigned short opacity;
	float above,below;
	int zero,one;
};
#endif /* MODELHELPSTRUCTS_H_ */
