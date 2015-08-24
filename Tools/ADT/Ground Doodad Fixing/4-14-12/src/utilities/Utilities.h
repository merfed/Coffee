#ifndef _UTILITIES_UTILITIES_H_
#define _UTILITIES_UTILITIES_H_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

namespace Utilities
{
  template<typename T> T get (const std::vector<char>& data, std::size_t offset)
  {
    return T (*reinterpret_cast<const T*> (&data[offset]));
  }

  void getWholeFile(const std::string & adtFileName, std::vector<char> & buffer);

  std::string getStringFromCharVector(const std::vector<char> & someData, const int start, const int stringLength);

  std::string getLettersFromFile(std::ifstream & adtFile, const int position);
  int getIntFromFile(std::ifstream & adtFile, const int position);
  std::vector<char> getCharVectorFromFile(std::ifstream & adtFile, const int position, const int length);

  std::vector<char> getCharVectorFromInt(const int someValue);
  std::vector<char> getCharVectorFromFloat(const float someValue);

  bool flagsExist(const int & bitmask, const int & whichFlags);

  int getAdtVersion(const std::string & adtName);
  int getWdtVersion(const std::string & wdtName);
};

#endif