#ifndef _WOWFILES_CHUNKHEADERS_H_
#define _WOWFILES_CHUNKHEADERS_H_

#include <stdint.h>

struct mclyEntry
{
  uint32_t  textureID;
  uint32_t  flags;
  uint32_t  ofsAlpha;
  uint32_t  effectID;
};

struct McnkHeader
{
  int flags;
  int indexX;
  int indexY;
  int nLayers;
  int m2Number;
  int mcvtOffset;
  int mcnrOffset;
  int mclyOffset;
  int mcrfOffset;
  int mcalOffset;
  int mcalSize;
  int mcshOffset;
  int mcshSize;
  int areaId;
  int wmoNumber;
  int holes;
  char groundEffectsMap[16];
  int predTex;
  int nEffectDoodad;
  int mcseOffset;
  int nSndEmitters;
  int mclqOffset;
  int mclqSize;
  float posX;
  float posY;
  float posZ;
  int mccvOffset;
  int mclvOffset;
  int unused;
};

struct McnkAlphaHeader
{
  int flags;
  int indexX;
  int indexY;
  float unknown1;
  int nLayers;
  int unknown2;
  int mcvtOffset; 
  int mcnrOffset;
  int mclyOffset;
  int mcrfOffset;
  int mcalOffset;
  int mcalSize;
  int mcshOffset;
  int mcshSize;
  int unknown3;
  int unknown4;
  int unknown5;
  int groundEffectsMap1;
  int groundEffectsMap2;
  int groundEffectsMap3;
  int groundEffectsMap4;  
  int unknown6;
  int unknown7;
  int mcnkChunksSize;
  int unknown8;
  int mclqOffset;
  int unused1;
  int unused2;
  int unused3;
  int unused4;
  int unused5;
  int unused6;
};

struct MhdrContent // TODO : use !
{ 
  int flags;
  int mcinOffset;
  int mtexOffset;
  int mmdxOffset;
  int mmidOffset;
  int mwmoOffset;
  int mwidOffset;
  int mddfOffset;
  int modfOffset;
  int mfboOffset;
  int mh2oOffset;
  int mtxfOffset;
  int unused1;
  int unused2;
  int unused3;
  int unused4;  
};

struct MhdrAlphaContent // TODO : use !
{
  int mcinOffset;
  int mtexOffset;
  int mtexSize;
  int mddfOffset;
  int unknown1;
  int modfOffset;
  int unused1;
  int unused2;  
  int unused3;
  int unused4;
  int unused5;
  int unused6;
  int unused7;
  int unused8;
  int unused9;
  int unused10;  
};

#endif
