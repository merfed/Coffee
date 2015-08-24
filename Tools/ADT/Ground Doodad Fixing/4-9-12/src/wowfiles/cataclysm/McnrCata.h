#ifndef _WOWFILES_CATACLYSM_MCNRCATA_H_
#define _WOWFILES_CATACLYSM_MCNRCATA_H_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <wowfiles/Chunk.h>
#include <wowfiles/lichking/McnrLk.h>

class McnrCata : public Chunk
{
  public:
  
    McnrCata();
    McnrCata(std::ifstream & adtFile, int offsetInFile);
    McnrCata(const std::vector<char> & wholeFile, int offsetInFile);
    McnrCata(const  std::string letters, int givenSize, const std::vector<char> & chunkData);
  
    McnrLk toMcnrLk();
  
};

#endif