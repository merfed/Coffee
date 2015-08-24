#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <wowfiles/Chunk.h>
#include <wowfiles/cataclysm/McnrCata.h>
#include <wowfiles/lichking/McnrLk.h>
#include <utilities/Utilities.h>

McnrCata::McnrCata() : Chunk()
{
}

McnrCata::McnrCata(std::ifstream & adtFile, int offsetInFile) : Chunk(adtFile, offsetInFile)
{
}

McnrCata::McnrCata(const std::vector<char> & wholeFile, int offsetInFile) : Chunk(wholeFile, offsetInFile)
{
}

McnrCata::McnrCata(std::string letters, int givenSize, const std::vector<char> & data) : Chunk("RNCM", givenSize, data) 
{
}

McnrLk McnrCata::toMcnrLk()
{
  const int unknownBytes (13);

  return McnrLk("RNCM", givenSize - unknownBytes, data);
}