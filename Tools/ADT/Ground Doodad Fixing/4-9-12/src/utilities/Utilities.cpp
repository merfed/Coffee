#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <utilities/Utilities.h>

namespace Utilities
{
  void getWholeFile(const std::string & adtFileName, std::vector<char> & buffer)
  {
    std::ifstream adtFile;
    adtFile.open(adtFileName.c_str(), std::ios::binary);

    adtFile.seekg(0, std::ios::end);
    const long fileSize = adtFile.tellg();
    adtFile.seekg(0, std::ios::beg);

    char * wholeFile;
    wholeFile = new char[fileSize];
    adtFile.read(wholeFile, fileSize);

    buffer.assign(wholeFile, wholeFile + fileSize);
   
    delete [] wholeFile;
  }

  std::string getStringFromCharVector(const std::vector<char> & someData, const int start, const int stringLength)
  {
    std::string letters ("");

    std::vector<char>::const_iterator adtIter;
    for (adtIter = someData.begin() + start ; adtIter != someData.begin() + start + stringLength ; ++adtIter)
    {
      letters = letters + *adtIter;
    }
    return letters;
  }

  std::string getLettersFromFile(std::ifstream & adtFile, const int position)
  {
    std::string letters ("");

    adtFile.seekg(position, std::ios::beg);
    char lettersBuffer[4];
    adtFile.read(lettersBuffer, 4);

    int i;
    for (i = 0 ; i < sizeof(lettersBuffer) ; ++i)
    {
      letters = letters + lettersBuffer[i];
    }
    return letters;
  }

  int getIntFromFile(std::ifstream & adtFile, const int position)
  {
    int givenSize (0);

    adtFile.seekg(position, std::ios::beg);
    char sizeBuffer[4];
    adtFile.read(sizeBuffer, 4);

    memcpy(&givenSize, sizeBuffer, sizeof(givenSize));

    return givenSize;
  }

  std::vector<char> getCharVectorFromFile(std::ifstream & adtFile, const int position, const int length)
  {
    adtFile.seekg(position, std::ios::beg);

    char * dataBuffer = new char[length];
    std::vector<char> data;

    adtFile.read(dataBuffer, length);
    data.assign(dataBuffer, dataBuffer + length);

    delete[] dataBuffer;

    return data;
  }

  std::vector<char> getCharVectorFromInt(const int someValue)
  {
    std::vector<char> charVector (0);

    char size[4];
    size[0] = someValue & 0xff;
    charVector.push_back(size[0]);
    size[1] = (someValue >> 8)  & 0xff;
    charVector.push_back(size[1]);
    size[2] = (someValue >> 16) & 0xff;
    charVector.push_back(size[2]);
    size[3] = (someValue >> 24) & 0xff;
    charVector.push_back(size[3]);

    return charVector;
  }

  std::vector<char> getCharVectorFromFloat(const float someValue)
  {
    std::vector<char> charVector (0);
    unsigned char * tempStorage ((unsigned char *)&someValue);

    int i;
    for (i = 0 ; i < sizeof(someValue) ; ++i)
    {
      charVector.push_back(tempStorage[i]);
    }

    return charVector;
  }

  bool flagsExist(const int & bitmask, const int & whichFlags)
  {
    return (bitmask & whichFlags) == whichFlags;
  }

  int getAdtVersion(const std::string & adtName)
  {
    std::ifstream adtFile;
    adtFile.open(adtName.c_str(), std::ios::binary);

    const int mcinOffset (0x18);
    adtFile.seekg(mcinOffset, std::ios::beg);

    int value;
    adtFile.read((char*)&value, sizeof(int));

    adtFile.close();

    if (value == 0)
      return 4;
    else
      return 3;
  }

  int getWdtVersion(const std::string & wdtName)
  {
    std::ifstream wdtFile;
    wdtFile.open(wdtName.c_str(), std::ios::binary);

    const int mainOffset (0x98);
    wdtFile.seekg(mainOffset, std::ios::beg);

    int value;
    wdtFile.read((char*)&value, sizeof(int));

    wdtFile.close();

    if (value == 65536)
      return 0;
    else
      return 1;
  }
}
