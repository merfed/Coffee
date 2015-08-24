#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <wowfiles/Chunk.h>
#include <wowfiles/lichking/McnrLk.h>
#include <utilities/Utilities.h>

McnrLk::McnrLk() : Chunk()
{
}

McnrLk::McnrLk(std::ifstream & adtFile, int offsetInFile)
{
  letters = Utilities::getLettersFromFile(adtFile, offsetInFile);
  offsetInFile += 4;

  const int unknownBytes (13); // TODO : move as member ? (then also change McnrAlpha)

  givenSize = Utilities::getIntFromFile(adtFile, offsetInFile);
  offsetInFile += 4;

  data = Utilities::getCharVectorFromFile(adtFile, offsetInFile, givenSize + unknownBytes);
}

McnrLk::McnrLk(const std::vector<char> & wholeFile, int offsetInFile)
{
  const int lettersSize (4);
  letters = Utilities::getStringFromCharVector(wholeFile, offsetInFile, lettersSize);
  offsetInFile += 4;

  const int unknownBytes (13);
  
  givenSize = Utilities::get<int>(wholeFile, offsetInFile);
  offsetInFile += 4;

  std::vector<char>::const_iterator fileIter;
  fileIter = wholeFile.begin() + offsetInFile;

  data.assign(fileIter, fileIter + givenSize + unknownBytes);
}

McnrLk::McnrLk(std::string letters, int givenSize, const std::vector<char> & data) : Chunk("RNCM", givenSize, data) 
{
}