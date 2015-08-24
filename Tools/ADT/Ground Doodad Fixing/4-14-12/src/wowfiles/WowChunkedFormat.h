#ifndef _WOWFILES_WOWCHUNKEDFORMAT_H_
#define _WOWFILES_WOWCHUNKEDFORMAT_H_

#include <vector>
#include <iostream>
#include <fstream>
#include <string>

class WowChunkedFormat
{
  public:
    
    static const int chunkLettersAndSize = 8;
    static const int mcnkTerrainHeaderSize = 128;
};

#endif