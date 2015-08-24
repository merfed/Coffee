#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <wowfiles/ChunkHeaders.h>
#include <wowfiles/Chunk.h>
#include <wowfiles/Mcnk.h>
#include <utilities/Utilities.h>

Mcnk::Mcnk() : Chunk()
{
}

Mcnk::Mcnk(std::ifstream & adtFile, int offsetInFile, const int & headerSize)
{
  letters = Utilities::getLettersFromFile(adtFile, offsetInFile);
  offsetInFile += 4;

  givenSize = Utilities::getIntFromFile(adtFile, offsetInFile);
  offsetInFile += 4;

  data = Utilities::getCharVectorFromFile(adtFile, offsetInFile, headerSize);
}

Mcnk::Mcnk(const std::vector<char> & wholeFile, int offsetInFile, const int & headerSize)
{
  letters = Utilities::getStringFromCharVector(wholeFile, offsetInFile, 4);
  offsetInFile += 4;

  givenSize = Utilities::get<int>(wholeFile, offsetInFile);
  offsetInFile += 4;

  std::vector<char>::const_iterator fileIter;
  fileIter = wholeFile.begin() + offsetInFile;

  data.assign(fileIter, fileIter + headerSize);
}

Mcnk::Mcnk(std::string letters, int givenSize, const std::vector<char> & data) : Chunk("KNCM", givenSize, data) 
{
}

void Mcnk::toFile()
{
  // TODO : something.
}