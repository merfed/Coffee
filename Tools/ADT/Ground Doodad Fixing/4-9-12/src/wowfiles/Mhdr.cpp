#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <wowfiles/Chunk.h>
#include <wowfiles/Mhdr.h>

Mhdr::Mhdr() : Chunk()
{
}

Mhdr::Mhdr(std::ifstream & adtFile, int offsetInFile) : Chunk(adtFile, offsetInFile)
{
}

Mhdr::Mhdr(const std::vector<char> & wholeFile, int offsetInFile) : Chunk(wholeFile, offsetInFile)
{
}

Mhdr::Mhdr(std::string letters, int givenSize, const std::vector<char> & data) : Chunk("RDHM", givenSize, data) 
{
}

int Mhdr::getFlags()
{
  return getOffset(0);
}