#ifndef _WOWFILES_ALPHA_MVCTALPHA_H_
#define _WOWFILES_ALPHA_MVCTALPHA_H_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <wowfiles/Chunk.h>

class McvtAlpha : public Chunk
{
  public:
  
    McvtAlpha();
    McvtAlpha(std::ifstream & wdtAlphaFile, int offsetInFile);
    McvtAlpha(const  std::string letters, int givenSize, const std::vector<char> & chunkData);
    
    Chunk toMcvt() const;
};

#endif