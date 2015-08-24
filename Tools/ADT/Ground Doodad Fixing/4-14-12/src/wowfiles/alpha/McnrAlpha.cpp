#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <wowfiles/Chunk.h>
#include <wowfiles/alpha/McnrAlpha.h>
#include <wowfiles/lichking/McnrLk.h>
#include <utilities/Utilities.h>

McnrAlpha::McnrAlpha() : Chunk()
{
}

McnrAlpha::McnrAlpha(std::ifstream & adtFile, int offsetInFile) : Chunk(adtFile, offsetInFile)
{
}

McnrAlpha::McnrAlpha(std::string letters, int givenSize, const std::vector<char> & data) : Chunk("RNCM", givenSize, data) 
{
}

McnrLk McnrAlpha::toMcnrLk() const
{
  std::vector<char> cMcnrData (0);

  const int outerNormalsSequence = 9*3;
  const int innerNormalsSequence = 8*3;

  const int innerDataStart = outerNormalsSequence*9;

  int i;

  for (i = 0 ; i < 9 ; ++i)
  {
    cMcnrData.insert(cMcnrData.end()
      , data.begin() + i*outerNormalsSequence
      , data.begin() + i*outerNormalsSequence + outerNormalsSequence
      );

    if (i == 8) break; 

    cMcnrData.insert(cMcnrData.end()
      , data.begin() + innerDataStart+(i*innerNormalsSequence)
      , data.begin() + innerDataStart+(i*innerNormalsSequence) + innerNormalsSequence
      );
  }

  const int unknownBytes (13);
  std::vector<char> unknownData (unknownBytes);

  cMcnrData.insert(cMcnrData.end(), unknownData.begin(), unknownData.end());

  McnrLk mcnrLk ("RNCM", givenSize - unknownBytes, cMcnrData);

  return mcnrLk;
}