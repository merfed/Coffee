#ifndef _WOWFILES_MAIN_H_
#define _WOWFILES_MAIN_H_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <wowfiles/Chunk.h>

class MainChunk : public Chunk
{
  public:
  
  MainChunk();
  MainChunk(std::ifstream & adtFile, int offsetInFile);
  MainChunk(const  std::string letters, int givenSize, const std::vector<char> & chunkData);
};

#endif