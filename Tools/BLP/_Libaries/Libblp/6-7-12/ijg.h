/********************************************************************************
    Warcraft 3 Viewer - Utility to view models and textures from Warcraft 3
    Copyright (C) 2002  David GRIMBICHLER (theprophet@wanadoo.Fr)
	http://www.xeberon.net

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
********************************************************************************/

#ifndef _IJG_H_
#define _IJG_H_

#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>
#include "jpeglib.h"
//#include "jinclude.h"
#include "jpeglib.h"
#include "jerror.h"

//#pragma comment(lib, "jpeg.lib")


typedef struct
{
  struct jpeg_source_mgr pub;
  unsigned char * srcbuf;
  unsigned long srcbufsize;
  JOCTET * buffer;
} my_source_mgr;

typedef my_source_mgr * my_src_ptr;


//#define OUTPUT_BUF_SIZE 4096

typedef struct
{
  struct jpeg_destination_mgr pub;
  unsigned char * destbuf;
  unsigned long destbufsize;
  JOCTET * buffer;
//  unsigned long numblocks;
} my_destination_mgr;

typedef my_destination_mgr * my_dest_ptr;



struct my_error_mgr
{
  struct jpeg_error_mgr pub;
  jmp_buf setjmp_buffer;
};

typedef struct my_error_mgr * my_error_ptr;


METHODDEF(void) init_source (j_decompress_ptr cinfo);
METHODDEF(boolean) fill_input_buffer (j_decompress_ptr cinfo);
METHODDEF(void) skip_input_data (j_decompress_ptr cinfo, long num_bytes);
METHODDEF(void) term_source (j_decompress_ptr cinfo);
GLOBAL(void) jpeg_memory_src (j_decompress_ptr cinfo, unsigned char * inbuf, unsigned long inbuflen);
METHODDEF(void) my_error_exit (j_common_ptr cinfo);
GLOBAL(unsigned long) IJGRead (unsigned char *buf, unsigned long buflen, long *w, long *h, long *bpp, unsigned char *destbuf);


METHODDEF(void) init_destination (j_compress_ptr cinfo);
METHODDEF(boolean) empty_output_buffer (j_compress_ptr cinfo);
METHODDEF(void) term_destination (j_compress_ptr cinfo);
GLOBAL(void) jpeg_memory_dest (j_compress_ptr cinfo, unsigned char * outbuf, unsigned long outbuflen);
GLOBAL(unsigned long) IJGWrite (unsigned char *buf, unsigned long buflen, unsigned char *destbuf, unsigned long destbuflen, int quality, long image_width, long image_height, long image_planes);



#else
#error "ijg.h already included"
#endif//_IJG_H_
