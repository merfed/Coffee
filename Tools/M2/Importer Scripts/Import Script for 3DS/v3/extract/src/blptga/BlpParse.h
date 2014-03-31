#ifndef __BLP_BASE_H__
#define __BLP_BASE_H__

// shamelessly stolen from linghuye

#include "dds.h"
#include <cmath>
#include <cstring>

typedef struct tagBLPHEADER
{
	char ident[4];
	int compress, nummipmaps, width, height, pictype, picsubtype;
	int mipmapOffsets[16], mipmapLengths[16];
}	BLP_HEADER;

struct PA_PIXEL
{
	unsigned char p1;
	unsigned char p2;
};

struct P_PIXEL
{
	unsigned char i;
};

struct BLP2_HEADER
{
	char ident[4];
	int  compress;
	char flags[4];
	int width;
	int height;
	int mipmapOffsets[16];
	int mipmapLengths[16];
};

struct RGBA_PIXEL
{
	unsigned char R, G, B, A;
};

enum { PALETTE_ALPHA = 0x800 };

bool DecodeDDSFormat(int nFormat, unsigned char* src, int width, int height, unsigned char *dest)
{
	if(nFormat == DDS_RGB)
	{
		for(int y = 0; y < height; y++)
		{
			for(int x = 0; x < width; x++)
			{
				*dest++ = *src++;
				*dest++ = *src++;
				*dest++ = *src++;
				*dest++ = 255;
			}
		}
	}
	else if(nFormat == DDS_RGBA)
	{
		memcpy(dest, src, width*height*4);
	}
	else // DXT
	{
		int cx = (width < 4)? width : 4;
		int cy = (height< 4)? height: 4;

		for(int y = 0; y < height; y += cy)
		{
			for(int x = 0; x < width; x += cx)
			{
				unsigned long long alpha = 0;
				unsigned int a0 = 0, a1 = 0;

				if(nFormat == DDS_DXT3)
				{
					alpha = *(unsigned long long*)src;
					src += 8;
				}
				else if(nFormat == DDS_DXT5)
				{
					alpha = (*(unsigned long long*)src) >> 16;
					a0 = src[0];
					a1 = src[1];
					src += 8;
				}

				unsigned int c0 = *(unsigned short*)(src + 0);
				unsigned int c1 = *(unsigned short*)(src + 2);
				src += 4;

				DDS_COLOR color[4];
				color[0].b = (unsigned char)((c0 >> 11) & 0x1f) << 3;
				color[0].g = (unsigned char)((c0 >> 5) & 0x3f) << 2;
				color[0].r = (unsigned char)(c0 & 0x1f) << 3;
				color[1].b = (unsigned char)((c1 >> 11) & 0x1f) << 3;
				color[1].g = (unsigned char)((c1 >> 5) & 0x3f) << 2;
				color[1].r = (unsigned char)(c1 & 0x1f) << 3;

				if(c0 > c1)
				{
					color[2].r = (color[0].r * 2 + color[1].r) / 3;
					color[2].g = (color[0].g * 2 + color[1].g) / 3;
					color[2].b = (color[0].b * 2 + color[1].b) / 3;
					color[3].r = (color[0].r + color[1].r * 2) / 3;
					color[3].g = (color[0].g + color[1].g * 2) / 3;
					color[3].b = (color[0].b + color[1].b * 2) / 3;
				}
				else
				{
					color[2].r = (color[0].r + color[1].r) / 2;
					color[2].g = (color[0].g + color[1].g) / 2;
					color[2].b = (color[0].b + color[1].b) / 2;
					color[3].r = 0;
					color[3].g = 0;
					color[3].b = 0;
				}

				for(int i = 0; i < cy; i++)
				{
					unsigned int index = *src++;
					unsigned char* tdata = dest + (width * (y + i) + x) * 4;
					for(int j = 0; j < cx; j++)
					{
						*tdata++ = color[index & 0x03].r;
						*tdata++ = color[index & 0x03].g;
						*tdata++ = color[index & 0x03].b;
						if(nFormat == DDS_DXT1)
						{
							*tdata++ = ((index & 0x03) == 3 && c0 <= c1) ? 0 : 255;
						}
						else if(nFormat == DDS_DXT3)
						{
							*tdata++ = (unsigned char)(alpha & 0x0f) << 4;
							alpha >>= 4;
						}
						else if(nFormat == DDS_DXT5)
						{
							unsigned int a = (unsigned char)alpha & 0x07;
							if(a == 0)
							{
								*tdata++ = (unsigned char)a0;
							}
							else if(a == 1)
							{
								*tdata++ = (unsigned char)a1;
							}
							else if(a0 > a1)
							{
								*tdata++ = (unsigned char)((8 - a) * a0 + (a - 1) * a1) / 7;
							}
							else if(a > 5)
							{
								*tdata++ = (a == 6) ? 0 : 255;
							}
							else
							{
								*tdata++ = (unsigned char)((6 - a) * a0 + (a - 1) * a1) / 5;
							}
							alpha >>= 3;
						}
						else
						{
							*tdata++ = 255;
						}
						index >>= 2;
					}
				}
			}
		}
	}
	return true;
}

#endif//__BLP_BASE_H__

