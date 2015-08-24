#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <wowfiles/Chunk.h>
#include <wowfiles/Mcin.h>
#include <utilities/Utilities.h>

Mcin::Mcin() : Chunk()
{
}

Mcin::Mcin(std::ifstream & adtFile, int offsetInFile) : Chunk(adtFile, offsetInFile)
{
}

Mcin::Mcin(const std::vector<char> & wholeFile, int offsetInFile) : Chunk(wholeFile, offsetInFile)
{
}

Mcin::Mcin(std::string letters, int givenSize, const std::vector<char> & data) : Chunk("NICM", givenSize, data)
{
}

std::vector<int> Mcin::getMcnkOffsets() const
{
  std::vector<int> mcnkOffsets (256);
  const int otherMcinDataSize (16);
  int currentMcinOffset (0);
  int mcnkNumber;
  
  for (mcnkNumber = 0 ; mcnkNumber < 256 ; ++mcnkNumber)
  {
    mcnkOffsets[mcnkNumber] = Utilities::get<int>(data, currentMcinOffset);
    currentMcinOffset = currentMcinOffset + otherMcinDataSize;
  }

  return mcnkOffsets;
}

std::ostream & operator<<(std::ostream & os, const Mcin & mcin)
{
  os << "Chunk letters : " << mcin.letters << std::endl;
  os << "Chunk givenSize : " << mcin.givenSize << std::endl;
  os << "MCNK Offsets in MCIN : " << std::endl;

  std::vector<int> mcnkOffsets (mcin.getMcnkOffsets());

  std::vector<int>::iterator mcnkOffsetsIter;
  int i = 1;

  for (mcnkOffsetsIter = mcnkOffsets.begin() ; mcnkOffsetsIter != mcnkOffsets.end() ; ++mcnkOffsetsIter)
  {
    if (i % 16 == 0)
      os << *mcnkOffsetsIter << std::endl;
    else
      os << *mcnkOffsetsIter << "\t";
    ++i;
  }

  os << "------------------------------" << std::endl;
  return os;
}