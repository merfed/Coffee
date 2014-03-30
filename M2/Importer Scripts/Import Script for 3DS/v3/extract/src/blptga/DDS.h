#ifndef __DDS_H__
#define __DDS_H__

#pragma once

struct DDS_COLORKEY
{
	unsigned int dwColorSpaceLowValue;
	unsigned int dwColorSpaceHighValue;
};

struct DDS_HEADER
{
	unsigned int magic;
	unsigned int dwSize;
	unsigned int dwFlags;
	unsigned int dwHeight;
	unsigned int dwWidth;
	long lPitch;
	unsigned int dwDepth;
	unsigned int dwMipMapCount;
	unsigned int dwAlphaBitDepth;
	unsigned int dwReserved;
	void *lpSurface;
	DDS_COLORKEY ddckCKDestOverlay;
	DDS_COLORKEY ddckCKDestBlt;
	DDS_COLORKEY ddckCKSrcOverlay;
	DDS_COLORKEY ddckCKSrcBlt;
	unsigned int dwPFSize;
	unsigned int dwPFFlags;
	unsigned int dwFourCC;
	unsigned int dwRGBBitCount;
	unsigned int dwRBitMask;
	unsigned int dwGBitMask;
	unsigned int dwBBitMask;
	unsigned int dwRGBAlphaBitMask;
	unsigned int dwCaps;
	unsigned int dwCaps2;
	unsigned int dwCaps3;
	unsigned int dwVolumeDepth;
	unsigned int dwTextureStage;
};

struct DDS_COLOR
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

enum
{
	DDS_ERROR = -1,
	DDS_RGB,
	DDS_RGBA,
	DDS_DXT1,
	DDS_DXT2,
	DDS_DXT3,
	DDS_DXT4,
	DDS_DXT5
};

enum
{
	DDPF_ALPHAPIXELS = 0x01,
	DDPF_FOURCC = 0x04,
	DDPF_RGB = 0x40,
	DDPF_RGBA = 0x41
};

#endif//__DDS_H__
