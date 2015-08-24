#ifndef _WOWFILES_ALPHA_MCNRALPHA_H_
#define _WOWFILES_ALPHA_MCNRALPHA_H_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <wowfiles/lichking/McnrLk.h>
#include <wowfiles/Chunk.h>

class McnrAlpha : public Chunk
{
  public:
  
  McnrAlpha();
  McnrAlpha(std::ifstream & adtFile, int offsetInFile);
  McnrAlpha(const  std::string letters, int givenSize, const std::vector<char> & chunkData);

  McnrLk toMcnrLk() const;
};

#endif