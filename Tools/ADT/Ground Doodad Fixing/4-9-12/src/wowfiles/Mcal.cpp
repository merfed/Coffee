#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <wowfiles/Chunk.h>
#include <wowfiles/Mcal.h>
#include <utilities/Utilities.h>

Mcal::Mcal() : Chunk()
{
}

Mcal::Mcal(std::ifstream & adtFile, int offsetInFile, int alphaSize) : Chunk(adtFile, offsetInFile)
{
  letters = Utilities::getLettersFromFile(adtFile, offsetInFile);
  offsetInFile += 4;

  givenSize = Utilities::getIntFromFile(adtFile, offsetInFile);
  offsetInFile += 4;

  data = Utilities::getCharVectorFromFile(adtFile, offsetInFile, alphaSize);
}

Mcal::Mcal(const std::vector<char> & wholeFile, int offsetInFile, int alphaSize)
{
  const int lettersSize (4);
  letters = Utilities::getStringFromCharVector(wholeFile, offsetInFile, lettersSize);
  offsetInFile += 4;

  givenSize = Utilities::get<int>(wholeFile, offsetInFile);
  offsetInFile += 4;

  std::vector<char>::const_iterator fileIter;
  fileIter = wholeFile.begin() + offsetInFile;

  data.assign(fileIter, fileIter + alphaSize);
}

Mcal::Mcal(std::string letters, int givenSize, const std::vector<char> & data) : Chunk("LACM", givenSize, data)
{
}