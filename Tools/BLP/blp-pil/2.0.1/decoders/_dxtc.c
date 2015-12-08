/*
 * dxt1-decoder.c -- extension module to decode DXT1 compressed textures.
 * Designed for use with DdsImagePlugin.py.
 *
 * Copyright (c) 2002, Oliver Jowett <oliver@randomly.org>
 * Copyright (c) 2009, Jerome Leclanche <adys.wh@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "Python.h"

/**
 * Given the data for a single row of 4x4 blocks from a DXT1 texture, return
 * a tuple of four decoded pixel rows as strings in RGB format.
 */
static PyObject* decodeDXT1(PyObject *self, PyObject *args) {
	unsigned char *data;
	int len;
	unsigned char *outdata[4];
	int outlen;
	int i;
	int emit;
	PyObject *ret;
	
	if (!PyArg_ParseTuple(args, "s#", &data, &len)) return NULL;
	
	outlen = (len / 8) * 4 * 3;
	for (i = 0; i < 4; ++i) {
		outdata[i] = malloc(outlen);
		if (outdata[i] == NULL) {
			int j;
			for (j = 0; j < i; ++j) free(outdata[i]);
			
			return PyErr_NoMemory();
		}
	}
	
	for (i = 0, emit = 0; i < len; i += 8, emit += 4) {
		int _i, j;
		
		unsigned color0 = data[i] | data[i+1] << 8U;
		unsigned color1 = data[i+2] | data[i+3] << 8U;
		unsigned long bits = data[i+4] | data[i+5] << 8UL | data[i+6] << 16UL | data[i+7] << 24UL;
		
		unsigned b0 = (color0 & 0x1f) << 3;
		unsigned g0 = ((color0 >> 5) & 0x3f) << 2;
		unsigned r0 = ((color0 >> 11) & 0x1f) << 3;
		
		unsigned b1 = (color1 & 0x1f) << 3;
		unsigned g1 = ((color1 >> 5) & 0x3f) << 2;
		unsigned r1 = ((color1 >> 11) & 0x1f) << 3;
		
		for (j = 0; j < 4; ++j) {
			for (_i = 0; _i < 4; ++_i) {
				unsigned control = bits & 3;
				bits >>= 2;
				
				switch (control) {
					case 0:
						outdata[j][3*(emit + _i)] = r0;
						outdata[j][3*(emit + _i) + 1] = g0;
						outdata[j][3*(emit + _i) + 2] = b0;
						break;
					case 1:
						outdata[j][3*(emit + _i)] = r1;
						outdata[j][3*(emit + _i) + 1] = g1;
						outdata[j][3*(emit + _i) + 2] = b1;
						break;
					case 2:
						if (color0 > color1) {
							outdata[j][3*(emit + _i)] = (2 * r0 + r1) / 3;
							outdata[j][3*(emit + _i) + 1] = (2 * g0 + g1) / 3;
							outdata[j][3*(emit + _i) + 2] = (2 * b0 + b1) / 3;
						} else {
							outdata[j][3*(emit + _i)] = (r0 + r1) / 2;
							outdata[j][3*(emit + _i) + 1] = (g0 + g1) / 2;
							outdata[j][3*(emit + _i) + 2] = (b0 + b1) / 2;
						}
						break;
					case 3:
						if (color0 > color1) {
							outdata[j][3*(emit + _i)] = (r0 + 2 * r1) / 3;
							outdata[j][3*(emit + _i) + 1] = (g0 + 2 * g1) / 3;
							outdata[j][3*(emit + _i) + 2] = (b0 + 2 * b1) / 3;
						} else {
							outdata[j][3*(emit + _i)] = 0;
							outdata[j][3*(emit + _i) + 1] = 0;
							outdata[j][3*(emit + _i) + 2] = 0;
						}
					break;
				}
			}
		}
	}
	
	// Done. Return as strings
	ret = PyTuple_New(4);
	if (!ret) {
		for (i = 0; i < 4; ++i) free(outdata[i]);
		return NULL;
	}
	
	for (i = 0; i < 4; ++i) {
		PyObject *buf = PyString_FromStringAndSize(outdata[i], outlen);
		if (!buf) {
			int j;
			for (j = i; j < 4; ++j) free(outdata[j]);
			Py_DECREF(ret);
			return NULL;
		}
		
		PyTuple_SetItem(ret, i, buf);
	}
	
	return ret;
}

/**
 * Given the data for a single row of 4x4 blocks from a DXT3 texture, return
 * a tuple of four decoded pixel rows as strings in RGBA format.
 */
static PyObject* decodeDXT3(PyObject *self, PyObject *args) {
	unsigned char *data;
	int len;
	unsigned char *outdata[4];
	int outlen;
	int i;
	int emit;
	PyObject *ret;
	
	if (!PyArg_ParseTuple(args, "s#", &data, &len)) return NULL;
	
	outlen = (len / 16) * 4 * 3;
	for (i = 0; i < 4; ++i) {
		outdata[i] = malloc(outlen);
		if (outdata[i] == NULL) {
			int j;
			for (j = 0; j < i; ++j) free(outdata[i]);
			
			return PyErr_NoMemory();
		}
	}
	
	for (i = 0, emit = 0; i < len; i += 8, emit += 4) {
		int _i, j;
		
		unsigned color0 = data[i] | data[i+1] << 8U;
		unsigned color1 = data[i+2] | data[i+3] << 8U;
		unsigned long bits = data[i+4] | data[i+5] << 8UL | data[i+6] << 16UL | data[i+7] << 24UL;
		
		unsigned b0 = (color0 & 0x1f) << 3;
		unsigned g0 = ((color0 >> 5) & 0x3f) << 2;
		unsigned r0 = ((color0 >> 11) & 0x1f) << 3;
		
		unsigned b1 = (color1 & 0x1f) << 3;
		unsigned g1 = ((color1 >> 5) & 0x3f) << 2;
		unsigned r1 = ((color1 >> 11) & 0x1f) << 3;
		
		for (j = 0; j < 4; ++j) {
			for (_i = 0; _i < 4; ++_i) {
				unsigned control = bits & 3;
				bits >>= 2;
				
				switch (control) {
					case 0:
						outdata[j][3*(emit + _i)] = r0;
						outdata[j][3*(emit + _i) + 1] = g0;
						outdata[j][3*(emit + _i) + 2] = b0;
						break;
					case 1:
						outdata[j][3*(emit + _i)] = r1;
						outdata[j][3*(emit + _i) + 1] = g1;
						outdata[j][3*(emit + _i) + 2] = b1;
						break;
					case 2:
						if (color0 > color1) {
							outdata[j][3*(emit + _i)] = (2 * r0 + r1) / 3;
							outdata[j][3*(emit + _i) + 1] = (2 * g0 + g1) / 3;
							outdata[j][3*(emit + _i) + 2] = (2 * b0 + b1) / 3;
						} else {
							outdata[j][3*(emit + _i)] = (r0 + r1) / 2;
							outdata[j][3*(emit + _i) + 1] = (g0 + g1) / 2;
							outdata[j][3*(emit + _i) + 2] = (b0 + b1) / 2;
						}
						break;
					case 3:
						if (color0 > color1) {
							outdata[j][3*(emit + _i)] = (r0 + 2 * r1) / 3;
							outdata[j][3*(emit + _i) + 1] = (g0 + 2 * g1) / 3;
							outdata[j][3*(emit + _i) + 2] = (b0 + 2 * b1) / 3;
						} else {
							outdata[j][3*(emit + _i)] = 0;
							outdata[j][3*(emit + _i) + 1] = 0;
							outdata[j][3*(emit + _i) + 2] = 0;
						}
					break;
				}
			}
		}
	}
	
	// Done. Return as strings
	ret = PyTuple_New(4);
	if (!ret) {
		for (i = 0; i < 4; ++i) free(outdata[i]);
		return NULL;
	}
	
	for (i = 0; i < 4; ++i) {
		PyObject *buf = PyString_FromStringAndSize(outdata[i], outlen);
		if (!buf) {
			int j;
			for (j = i; j < 4; ++j) free(outdata[j]);
			Py_DECREF(ret);
			return NULL;
		}
		
		PyTuple_SetItem(ret, i, buf);
	}
	
	return ret;
}

static PyMethodDef functions[] = {
	{"decodeDXT1", decodeDXT1, 1, "decodeDXT1() doc string"},
	{"decodeDXT3", decodeDXT3, 1, "decodeDXT3() doc string"},
// 	{"decodeDXT5", decodeDXT5, 1, "decodeDXT5() doc string"},
	{NULL, NULL}
};

void init_dxtc(void) {
	Py_InitModule("_dxtc", functions);
}
