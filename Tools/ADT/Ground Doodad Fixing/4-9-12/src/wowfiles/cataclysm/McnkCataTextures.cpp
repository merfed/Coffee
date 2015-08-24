#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <wowfiles/Chunk.h>
#include <wowfiles/Mcnk.h>
#include <wowfiles/cataclysm/McnkCataTextures.h>
#include <utilities/Utilities.h>

McnkCataTextures::McnkCataTextures(std::ifstream & adtFile, int offsetInFile) : Mcnk(adtFile, offsetInFile, 128)
{
  const int absoluteMcnkEnd = offsetInFile + chunkLettersAndSize + givenSize;

  offsetInFile = chunkLettersAndSize + offsetInFile;
  
  int chunkName (Utilities::getIntFromFile(adtFile, offsetInFile));

  while (offsetInFile < absoluteMcnkEnd)
  {
    switch (chunkName)
    {
      case 'MCLY' :
        mcly = Chunk(adtFile, offsetInFile);
        offsetInFile = offsetInFile + chunkLettersAndSize + mcly.getGivenSize();
        break;  

      case 'MCSH' :
        mcsh = Chunk(adtFile, offsetInFile);
        offsetInFile = offsetInFile + chunkLettersAndSize + mcsh.getGivenSize();
        break; 

      case 'MCAL' :
        mcal = Chunk(adtFile, offsetInFile);
        offsetInFile = offsetInFile + chunkLettersAndSize + mcal.getGivenSize();
        break;  		

      case 'MCMT' :
        mcmt = Chunk(adtFile, offsetInFile);
        offsetInFile = offsetInFile + chunkLettersAndSize + mcmt.getGivenSize();
        break;  

      default :
        texturesMcnkUnknown.push_back(Chunk(adtFile, offsetInFile));
        offsetInFile = offsetInFile + chunkLettersAndSize + texturesMcnkUnknown.back().getGivenSize();
    }
	
    chunkName = Utilities::getIntFromFile(adtFile, offsetInFile);
  }	
}

void McnkCataTextures::toFile()
{
  // TODO
}

std::ostream & operator<<(std::ostream & os, const McnkCataTextures & mcnkCataTextures)
{
  os << "Chunk letters : " << mcnkCataTextures.letters << std::endl;
  os << "Chunk givenSize : " << mcnkCataTextures.givenSize << std::endl;

  os << "------------------------------" << std::endl;

  os << mcnkCataTextures.mcly;
  os << mcnkCataTextures.mcsh;
  os << mcnkCataTextures.mcal;
  os << mcnkCataTextures.mcmt;

  os << "Hi, in here we're unknown !" << std::endl;
  os << std::endl;

  std::vector<Chunk>::const_iterator unknownIter;

  for (unknownIter = mcnkCataTextures.texturesMcnkUnknown.begin() ; unknownIter != mcnkCataTextures.texturesMcnkUnknown.end() ; ++unknownIter)
  {
    os << *unknownIter;
  }  
  
  os << "------------------------------" << std::endl;
  return os;
}
