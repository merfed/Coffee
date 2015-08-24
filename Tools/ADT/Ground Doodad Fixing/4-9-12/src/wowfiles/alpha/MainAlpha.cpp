#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <wowfiles/Chunk.h>
#include <wowfiles/alpha/MainAlpha.h>
#include <wowfiles/MainChunk.h>
#include <utilities/Utilities.h>

MainAlpha::MainAlpha() : Chunk()
{
}

MainAlpha::MainAlpha(std::ifstream & wdtAlphaFile, int offsetInFile) : Chunk(wdtAlphaFile, offsetInFile)
{
}

MainAlpha::MainAlpha(std::string letters, int givenSize, const std::vector<char> & data) : Chunk("NIAM", givenSize, data) 
{
}

std::vector<int> MainAlpha::getMhdrOffsets() const
{
  std::vector<int> mhdrOffsets (4096);
  const int otherMainDataSize (16);
  int currentMainOffset (0);
  int mhdrNumber;

  for (mhdrNumber = 0 ; mhdrNumber < 4096 ; ++mhdrNumber)
  {
    mhdrOffsets[mhdrNumber] = Utilities::get<int>(data, currentMainOffset);
    currentMainOffset = currentMainOffset + otherMainDataSize;
  }

  return mhdrOffsets;
}

MainChunk MainAlpha::toMain() const
{
  const int mainLkDataSize (32768);
  std::vector<char> mainLkData(mainLkDataSize);

  int i;
  int j (0);

  // Note : If there's a value on alpha every 16 bytes, every 8 bytes on lk = 1
  for (i = 0 ; i < 65536 ; i = i+16)
  {
    if (Utilities::get<int>(data, i) != 0)
    {
      mainLkData[j] = 1;
    }
    j = j+8;
  }

  MainChunk mainLk = MainChunk("NIAM", mainLkDataSize, mainLkData);
  return mainLk;
}

std::ostream & operator<<(std::ostream & os, const MainAlpha & main)
{
  os << "Chunk letters : " << main.letters << std::endl;
  os << "Chunk givenSize : " << main.givenSize << std::endl;
  os << "MHDR Offsets in MAIN : " << std::endl;

  std::vector<int> mhdrOffsets = main.getMhdrOffsets();

  std::vector<int>::iterator mhdrOffsetsIter;
  int i (1);

  for (mhdrOffsetsIter = mhdrOffsets.begin() ; mhdrOffsetsIter != mhdrOffsets.end() ; ++mhdrOffsetsIter)
  {
    if (i % 16 == 0)
      os << *mhdrOffsetsIter << std::endl;
    else
      os << *mhdrOffsetsIter << "\t";
    ++i;
  }

  os << "------------------------------" << std::endl;
  return os;
}
