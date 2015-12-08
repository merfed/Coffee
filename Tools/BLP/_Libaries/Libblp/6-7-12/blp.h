/*
 *  blp.h
 *  libblp
 *
 *  Created by Jean-Francois Roy on Wed Nov 19 2003.
 *  Copyright (c) 2003 MacStorm. All rights reserved.
 *
 */

#ifndef _BLP_BLP_H_
#define _BLP_BLP_H_

extern unsigned long LoadBLP(char *destBuf, char *srcBuf, long *width, long *height, unsigned long *type, unsigned long *subtype);
extern unsigned long MakeBLP(char *destBuf, char *srcBuf, long width, long height, int quality);

extern unsigned long LoadTGA(char *destbuf, char *srcbuf, long *width, long *height, long *bpp, long *type);
extern unsigned long MakeTGA(char *destBuf, char *srcBuf, long width, long height, long bpp);

extern unsigned long MakeJPG(char *destBuf, char *srcBuf, long width, long height, int quality);

extern int BLP2Everything(char *srcbuf, unsigned long srcbuflen, const char *szFileDest);

#endif
