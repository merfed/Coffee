#include "ijg.h"


METHODDEF(void)
init_source (j_decompress_ptr cinfo)
{
  //my_src_ptr src = (my_src_ptr) cinfo->src;

  //  src->start_of_buf = TRUE;
}



METHODDEF(void)
init_destination (j_compress_ptr cinfo)
{
  my_dest_ptr dest = (my_dest_ptr) cinfo->dest;

  /* Allocate the output buffer --- it will be released when done with image */
  //dest->buffer = (JOCTET *)
  //   (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_IMAGE,
  //				  dest->destbufsize  * SIZEOF(JOCTET));

  dest->buffer = dest->destbuf;

  dest->pub.next_output_byte = dest->buffer;
  dest->pub.free_in_buffer = dest->destbufsize;
}


METHODDEF(boolean)
empty_output_buffer (j_compress_ptr cinfo)
{
  my_dest_ptr dest = (my_dest_ptr) cinfo->dest;

//  if (JFWRITE(dest->outfile, dest->buffer, OUTPUT_BUF_SIZE) !=
//      (size_t) OUTPUT_BUF_SIZE)
//    ERREXIT(cinfo, JERR_FILE_WRITE);

  dest->pub.next_output_byte = dest->buffer;
  dest->pub.free_in_buffer = dest->destbufsize;

  return TRUE;
}


METHODDEF(void)
term_destination (j_compress_ptr cinfo)
{
	/* no work necessary here */

/*  my_dest_ptr dest = (my_dest_ptr) cinfo->dest;
  size_t datacount = OUTPUT_BUF_SIZE - dest->pub.free_in_buffer;

  // Write any data remaining in the buffer 
  if (datacount > 0) {
    if (JFWRITE(dest->outfile, dest->buffer, datacount) != datacount)
      ERREXIT(cinfo, JERR_FILE_WRITE);
  }
  fflush(dest->outfile);
  // Make sure we wrote the output file OK 
  if (ferror(dest->outfile))
    ERREXIT(cinfo, JERR_FILE_WRITE);*/
}



METHODDEF(boolean)
fill_input_buffer (j_decompress_ptr cinfo)
{
  my_src_ptr src = (my_src_ptr) cinfo->src;
  size_t nbytes;

  // nbytes = JFREAD(src->infile, src->buffer, INPUT_BUF_SIZE);

  nbytes = src->srcbufsize;
  src->buffer = src->srcbuf;

/*
  if (nbytes <= 0) {
    if (src->start_of_file)	
      ERREXIT(cinfo, JERR_INPUT_EMPTY);
    WARNMS(cinfo, JWRN_JPEG_EOF);
    // Insert a fake EOI marker 
    src->buffer[0] = (JOCTET) 0xFF;
    src->buffer[1] = (JOCTET) JPEG_EOI;
    nbytes = 2;
  }
*/

  src->pub.next_input_byte = src->buffer;
  src->pub.bytes_in_buffer = nbytes;

//  src->start_of_buf = FALSE;

  return TRUE;
}



METHODDEF(void)
skip_input_data (j_decompress_ptr cinfo, long num_bytes)
{
  my_src_ptr src = (my_src_ptr) cinfo->src;

  if (num_bytes > 0)
  {
    while (num_bytes > (long) src->pub.bytes_in_buffer)
	{
      num_bytes -= (long) src->pub.bytes_in_buffer;
      (void) fill_input_buffer(cinfo);
    }
    src->pub.next_input_byte += (size_t) num_bytes;
    src->pub.bytes_in_buffer -= (size_t) num_bytes;
  }
}


METHODDEF(void)
term_source (j_decompress_ptr cinfo)
{
  /* no work necessary here */
}




GLOBAL(void)
jpeg_memory_dest (j_compress_ptr cinfo, unsigned char * outbuf, unsigned long outbuflen)
{
  my_dest_ptr dest;

  if (cinfo->dest == NULL) {
    cinfo->dest = (struct jpeg_destination_mgr *)
      (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
				  sizeof(my_destination_mgr));
  }

  dest = (my_dest_ptr) cinfo->dest;
  dest->pub.init_destination = init_destination;
  dest->pub.empty_output_buffer = empty_output_buffer;
  dest->pub.term_destination = term_destination;
  dest->destbuf = outbuf;
  dest->destbufsize = outbuflen;
}



GLOBAL(void)
jpeg_memory_src (j_decompress_ptr cinfo, unsigned char * inbuf, unsigned long inbuflen)
{
  my_src_ptr src;

  if (cinfo->src == NULL)
  {	
    cinfo->src = (struct jpeg_source_mgr *)
      (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
				  sizeof(my_source_mgr));

    src = (my_src_ptr) cinfo->src;

    src->buffer = (JOCTET *)
      (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
				  inbuflen * sizeof(JOCTET));

  }

  src = (my_src_ptr) cinfo->src;
  src->pub.init_source = init_source;
  src->pub.fill_input_buffer = fill_input_buffer;
  src->pub.skip_input_data = skip_input_data;
  src->pub.resync_to_restart = jpeg_resync_to_restart;
  src->pub.term_source = term_source;
  src->srcbuf = inbuf;
  src->srcbufsize = inbuflen;
  src->pub.bytes_in_buffer = 0;
  src->pub.next_input_byte = NULL;
}





METHODDEF(void)
my_error_exit (j_common_ptr cinfo)
{
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  (*cinfo->err->output_message) (cinfo);

  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
}


GLOBAL(unsigned long)
IJGRead (unsigned char *buf, unsigned long buflen, long *w, long *h, long *bpp, unsigned char *destbuf)
{
  struct jpeg_decompress_struct cinfo;
  struct my_error_mgr jerr;
  JSAMPARRAY buffer;
  int row_stride;
  long ss;

  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = my_error_exit;
  if (setjmp(jerr.setjmp_buffer))
  {
    jpeg_destroy_decompress(&cinfo);
	  if (w != NULL) *w = 0;
	  if (h != NULL) *h = 0;
	  if (bpp != NULL) *bpp = 0;
    return 0;
  }
  jpeg_create_decompress(&cinfo);

  jpeg_memory_src(&cinfo, buf, buflen);

  (void) jpeg_read_header(&cinfo, TRUE);
  (void) jpeg_start_decompress(&cinfo);

  row_stride = cinfo.output_width * cinfo.output_components;

  if(w!=NULL) *w = cinfo.output_width;
  if(h!=NULL) *h = cinfo.output_height;
  if(bpp!=NULL) *bpp = cinfo.output_components*8;
  ss = cinfo.output_width * cinfo.output_height * cinfo.output_components;

  if(destbuf==NULL)
  {
	  jpeg_destroy_decompress(&cinfo);
	  return ss;
  }

  buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

  while (cinfo.output_scanline < cinfo.output_height)
  {
	(void) jpeg_read_scanlines(&cinfo, &destbuf, 1);
	destbuf+=row_stride;
  }

  (void) jpeg_finish_decompress(&cinfo);

  jpeg_destroy_decompress(&cinfo);

  return ss;
}

GLOBAL(unsigned long)
IJGWrite (unsigned char *buf, unsigned long buflen, unsigned char *destbuf, unsigned long destbuflen, int quality, long image_width, long image_height, long image_planes)
{
	//int bwasempty=0;
	unsigned char *tdestbuf=destbuf;
	struct jpeg_compress_struct cinfo;
	struct my_error_mgr jerr;
	unsigned char * row_pointer[1];
	int row_stride;
	long nbyteswritten = 800;
	
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	if (setjmp(jerr.setjmp_buffer))
	{
		jpeg_destroy_compress(&cinfo);
		return 0;
	}
	jpeg_create_compress(&cinfo);
	
	if (destbuf==NULL)
	{
		tdestbuf=(unsigned char*)malloc((buflen<10000 ? 10000 : buflen));
		jpeg_memory_dest(&cinfo, tdestbuf, buflen);
		
		cinfo.image_width = image_width;
		cinfo.image_height = image_height;
		cinfo.input_components = image_planes;
		
		if(image_planes==3)
			cinfo.in_color_space = JCS_RGB;
		else
			cinfo.in_color_space = JCS_UNKNOWN;
		
		jpeg_set_defaults(&cinfo);
		jpeg_set_quality(&cinfo, quality, TRUE);
		jpeg_start_compress(&cinfo, TRUE);
		row_stride = image_width * image_planes;
		
		while (cinfo.next_scanline < cinfo.image_height)
		{
			row_pointer[0] = & buf[cinfo.next_scanline * row_stride];
			(void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
		}
		
		nbyteswritten = buflen - cinfo.dest->free_in_buffer;
		jpeg_finish_compress(&cinfo);
		jpeg_destroy_compress(&cinfo);
		//free(tdestbuf);
		return nbyteswritten+3;
	}
	else
	{
		jpeg_memory_dest(&cinfo, destbuf, destbuflen);
		
		cinfo.image_width = image_width;
		cinfo.image_height = image_height;
		cinfo.input_components = image_planes;
		
		if(image_planes==3)
			cinfo.in_color_space = JCS_RGB;
		else
			cinfo.in_color_space = JCS_UNKNOWN;
		
		jpeg_set_defaults(&cinfo);
		jpeg_set_quality(&cinfo, quality, TRUE);
		jpeg_start_compress(&cinfo, TRUE);
		row_stride = image_width * image_planes;
		
		while (cinfo.next_scanline < cinfo.image_height)
		{
			row_pointer[0] = & buf[cinfo.next_scanline * row_stride];
			(void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
		}
		
		nbyteswritten = destbuflen - cinfo.dest->free_in_buffer;
		jpeg_finish_compress(&cinfo);
		jpeg_destroy_compress(&cinfo);
		return nbyteswritten;
	}
}

