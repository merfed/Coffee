#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <wowfiles/Chunk.h>
#include <utilities/Utilities.h>

Chunk::Chunk() : letters("NONE"), givenSize(0)
{
  std::vector<char> emptyData(0);
  data = emptyData;
}

Chunk::Chunk(std::ifstream & file, int offsetInFile) : letters("")
{
  letters = Utilities::getLettersFromFile(file, offsetInFile);
  offsetInFile += 4;

  givenSize = Utilities::getIntFromFile(file, offsetInFile);
  offsetInFile += 4;

  data = Utilities::getCharVectorFromFile(file, offsetInFile, givenSize);
}

Chunk::Chunk(const std::vector<char> & wholeFile, int offsetInFile)
{
  const int lettersSize (4);
  letters = Utilities::getStringFromCharVector(wholeFile, offsetInFile, lettersSize);
  offsetInFile += 4;

  givenSize = Utilities::get<int>(wholeFile, offsetInFile);
  offsetInFile += 4;

  std::vector<char>::const_iterator fileIter;
  fileIter = wholeFile.begin() + offsetInFile;

  data.assign(fileIter, fileIter + givenSize);
}

Chunk::Chunk(std::string let, int givSize, const std::vector<char> & da) : letters(let), givenSize(givSize), data(da)
{
}

std::vector<char> Chunk::getWholeChunk() const
{
  std::vector<char> wholeChunk (0);

  std::vector<char> let (letters.begin(), letters.end());
  wholeChunk.insert(wholeChunk.end(), let.begin(), let.end());

  std::vector<char> siz (Utilities::getCharVectorFromInt(givenSize));
  wholeChunk.insert(wholeChunk.end(), siz.begin(), siz.end());

  wholeChunk.insert(wholeChunk.end(), data.begin(), data.end());

  return wholeChunk;
}

bool Chunk::isEmpty()  const
{
  return (letters == "NONE") && (givenSize == 0) && data.size() == 0;
}

std::ostream & operator<<(std::ostream & os, const Chunk & chunk)
{
  os << "Chunk letters : " << chunk.letters << std::endl;
  os << "Chunk givenSize : " << chunk.givenSize << std::endl;
  os << "Real chunk Size : " << chunk.getRealSize() << std::endl;
  os << "------------------------------" << std::endl;
  return os;
}

int Chunk::getOffset(const int offsetInData)  const
{
  return Utilities::get<int>(data, offsetInData);
}

void Chunk::toFile(std::string & fileName)
{
  std::ofstream outputFile (fileName.c_str(), std::ios::out|std::ios::binary);
  
  if (outputFile.is_open())
  {
    if (!isEmpty())
    outputFile.write((char *)&getWholeChunk()[0], sizeof(char) * getWholeChunk().size());
  }
  
  outputFile.close();
}

int Chunk::getRealSize()  const
{
  return data.size();
}

int Chunk::getGivenSize()  const
{
  return givenSize;
}