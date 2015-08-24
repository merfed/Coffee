#ifndef _WOWFILES_MCAL_H_
#define _WOWFILES_MCAL_H_

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <wowfiles/Chunk.h>

class Mcal : public Chunk
{
  public:

    Mcal();
    Mcal(std::ifstream & adtFile, int offsetInFile, int alphaSize);
	Mcal(const std::vector<char> & wholeFile, int offsetInFile, int alphaSize);
    Mcal(std::string letters, int givenSize, const std::vector<char> & chunkData);
};

#endif