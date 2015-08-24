#ifndef _WOWFILES_ALPHA_MPHDALPHA_H_
#define _WOWFILES_ALPHA_MPHDALPHA_H_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <wowfiles/Chunk.h>
#include <wowfiles/Mphd.h>

class MphdAlpha : public Chunk
{
  public:
  
    MphdAlpha();
    MphdAlpha(std::ifstream & wdtAlphaFile, int offsetInFile);
    MphdAlpha(const  std::string letters, int givenSize, const std::vector<char> & chunkData);
  
    bool isWmoBased() const;
    Mphd toMphd() const;
};

#endif