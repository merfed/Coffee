#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <wowfiles/Chunk.h>
#include <wowfiles/alpha/Monm.h>

Monm::Monm() : Chunk()
{
}

Monm::Monm(std::ifstream & wdtAlphaFile, int offsetInFile) : Chunk(wdtAlphaFile, offsetInFile)
{
}

Monm::Monm(std::string letters, int givenSize, const std::vector<char> & data) : Chunk("MNOM", givenSize, data) 
{
}

Chunk Monm::toMwmo() const
{
  Chunk mwmoLk ("OMWM", givenSize, data);
  return mwmoLk;
}