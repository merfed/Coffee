#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "Chunk.h"
#include "Utilities.h"

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

Chunk::Chunk(std::string let, int givSize, const std::vector<char> & da) : letters(let), givenSize(givSize), data(da)
{
}

std::vector<char> Chunk::getWholeChunk() const
{
  std::vector<char> wholeChunk(0);

  std::vector<char> let(letters.begin(), letters.end());
  wholeChunk.insert(wholeChunk.end(), let.begin(), let.end());

  std::vector<char> siz = Utilities::getCharVectorFromInt(givenSize);
  wholeChunk.insert(wholeChunk.end(), siz.begin(), siz.end());

  wholeChunk.insert(wholeChunk.end(), data.begin(), data.end());

  return wholeChunk;
}

std::vector<char> Chunk::getData()
{
    return data;
}

void Chunk::setData(const std::vector<char> dataToSet)
{
    data = dataToSet;
}

void Chunk::setCharInData(char c, int offset)
{
    data[offset] = c;
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
  return Utilities::getIntFromCharVector(data, offsetInData);
}

int Chunk::getRealSize()  const
{
  return data.size();
}

int Chunk::getGivenSize()  const
{
  return givenSize;
}
