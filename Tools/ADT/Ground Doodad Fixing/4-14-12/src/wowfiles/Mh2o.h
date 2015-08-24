#ifndef _WOWFILES_MH2O_H_
#define _WOWFILES_MH2O_H_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <wowfiles/Chunk.h>

class Mh2o : public Chunk
{
  public:
  
    Mh2o();
    Mh2o(std::ifstream & adtFile, int offsetInFile);
	Mh2o(const std::vector<char> & wholeFile, int offsetInFile);
    Mh2o(const  std::string letters, int givenSize, const std::vector<char> & chunkData);
};

#endif