#ifndef _WOWFILES_LICHKING_MCNRLK_H_
#define _WOWFILES_LICHKING_MCNRLK_H_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <wowfiles/Chunk.h>

class McnrLk : public Chunk
{
  public:
  
  McnrLk();
  McnrLk(std::ifstream & adtFile, int offsetInFile);
  McnrLk(const std::vector<char> & wholeFile, int offsetInFile);
  McnrLk(const  std::string letters, int givenSize, const std::vector<char> & chunkData);
};

#endif