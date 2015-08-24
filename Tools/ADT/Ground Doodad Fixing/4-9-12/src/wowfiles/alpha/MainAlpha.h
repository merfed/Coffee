#ifndef _WOWFILES_ALPHA_MAINALPHA_H_
#define _WOWFILES_ALPHA_MAINALPHA_H_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <wowfiles/Chunk.h>
#include <wowfiles/MainChunk.h>

class MainAlpha : public Chunk
{
  public:
  
  MainAlpha();
  MainAlpha(std::ifstream & wdtAlphaFile, int offsetInFile);
  MainAlpha(const  std::string letters, int givenSize, const std::vector<char> & chunkData);
  
  std::vector<int> getMhdrOffsets() const;
  MainChunk toMain() const;
  friend std::ostream & operator<<(std::ostream & os, const MainAlpha & main);
};

#endif
