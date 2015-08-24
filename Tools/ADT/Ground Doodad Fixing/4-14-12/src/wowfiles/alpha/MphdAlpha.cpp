#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <wowfiles/Chunk.h>
#include <wowfiles/alpha/MphdAlpha.h>
#include <utilities/Utilities.h>

MphdAlpha::MphdAlpha() : Chunk()
{
}

MphdAlpha::MphdAlpha(std::ifstream & wdtAlphaFile, int offsetInFile) : Chunk(wdtAlphaFile, offsetInFile)
{
}

MphdAlpha::MphdAlpha(std::string letters, int givenSize, const std::vector<char> & data) : Chunk("DHPM", givenSize, data) 
{
}

bool MphdAlpha::isWmoBased() const
{
  const int isWmoBasedOffset (8);
  return Utilities::get<int>(data, isWmoBasedOffset) == 2;
}

Mphd MphdAlpha::toMphd() const
{
  std::vector<char> mphdLkData (32);

  // Note : I don't think other flags are necessary for alpha to lk... ?
  if (isWmoBased())
    mphdLkData[0] = 1;

  Mphd mphdLk ("DHPM", 32, mphdLkData);
  return mphdLk;
}