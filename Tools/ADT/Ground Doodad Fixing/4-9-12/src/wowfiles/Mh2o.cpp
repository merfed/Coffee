#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <wowfiles/Chunk.h>
#include <wowfiles/Mh2o.h>
#include <utilities/Utilities.h>

Mh2o::Mh2o() : Chunk()
{
}

Mh2o::Mh2o(std::ifstream & adtFile, int offsetInFile)
{
  letters = Utilities::getLettersFromFile(adtFile, offsetInFile);
  offsetInFile += 4;

  givenSize = Utilities::getIntFromFile(adtFile, offsetInFile);
  offsetInFile += 4;

  const int GRETSize (1413829191);

  if (givenSize == GRETSize)
  {
    adtFile.seekg(0, std::ios::end);
    const int fileSize (adtFile.tellg());
    adtFile.seekg(offsetInFile, std::ios::beg); // TODO : check if necessary

    givenSize = fileSize - offsetInFile;
    data = Utilities::getCharVectorFromFile(adtFile, offsetInFile, givenSize);
  }
  else 
  {
    data = Utilities::getCharVectorFromFile(adtFile, offsetInFile, givenSize);
  }
}

Mh2o::Mh2o(const std::vector<char> & wholeFile, int offsetInFile) // TODO : check absolutely
{
  const int lettersSize (4);
  letters = Utilities::getStringFromCharVector(wholeFile, offsetInFile, lettersSize);
  offsetInFile += 4;

  givenSize = Utilities::get<int>(wholeFile, offsetInFile);
  offsetInFile += 4;

  const int GRETSize (1413829191);
  std::vector<char>::const_iterator fileIter;
  fileIter = wholeFile.begin() + offsetInFile;
  
  if (givenSize == GRETSize)
  {
    const int fileSize (wholeFile.size());
    givenSize = fileSize - offsetInFile;
	
    data.assign(fileIter, fileIter + givenSize);
  }
  else 
  {
    data.assign(fileIter, fileIter + givenSize);
  }
}

Mh2o::Mh2o(std::string letters, int givenSize, const std::vector<char> & data) : Chunk("O2HM", givenSize, data) 
{
}