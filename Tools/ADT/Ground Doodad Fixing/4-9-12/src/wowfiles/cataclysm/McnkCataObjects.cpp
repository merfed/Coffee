#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <wowfiles/Chunk.h>
#include <wowfiles/Mcnk.h>
#include <wowfiles/cataclysm/McnkCataObjects.h>
#include <utilities/Utilities.h>

McnkCataObjects::McnkCataObjects(std::ifstream & adtFile, int offsetInFile) : Mcnk(adtFile, offsetInFile, 128)
{
  const int absoluteMcnkEnd = offsetInFile + chunkLettersAndSize + givenSize;

  offsetInFile = chunkLettersAndSize + offsetInFile;
  
  int chunkName (Utilities::getIntFromFile(adtFile, offsetInFile));

  /*while (offsetInFile < absoluteMcnkEnd)
  {
    switch (chunkName)
    {
      case 'MCRD' :
        mcrd = Chunk(adtFile, offsetInFile);
        offsetInFile = offsetInFile + chunkLettersAndSize + mcrd.getGivenSize();
        break;  

      case 'MCRW' :
        mcrw = Chunk(adtFile, offsetInFile);
        offsetInFile = offsetInFile + chunkLettersAndSize + mcrw.getGivenSize();
        break;

      default :
        objectsMcnkUnknown.push_back(Chunk(adtFile, offsetInFile));
        offsetInFile = offsetInFile + chunkLettersAndSize + objectsMcnkUnknown.back().getGivenSize();
    }
	
    chunkName = Utilities::getIntFromFile(adtFile, offsetInFile);
  }*/
}

McnkCataObjects::McnkCataObjects(const std::vector<char> & wholeFile, int offsetInFile) : Mcnk(wholeFile, offsetInFile, 128)
{
  // TODO.
}

void McnkCataObjects::toFile()
{
  // TODO
}

std::ostream & operator<<(std::ostream & os, const McnkCataObjects & mcnkCataObjects)
{
  os << "Chunk letters : " << mcnkCataObjects.letters << std::endl;
  os << "Chunk givenSize : " << mcnkCataObjects.givenSize << std::endl;

  os << "------------------------------" << std::endl;

  os << mcnkCataObjects.mcrd;
  os << mcnkCataObjects.mcrw;

  os << "Hi, in here we're unknown !" << std::endl;
  os << std::endl;

  std::vector<Chunk>::const_iterator unknownIter;

  for (unknownIter = mcnkCataObjects.objectsMcnkUnknown.begin() ; unknownIter != mcnkCataObjects.objectsMcnkUnknown.end() ; ++unknownIter)
  {
    os << *unknownIter;
  }  
  
  os << "------------------------------" << std::endl;
  return os;
}
