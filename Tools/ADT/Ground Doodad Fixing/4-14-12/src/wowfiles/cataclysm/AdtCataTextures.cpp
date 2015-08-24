#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <string>
#include <wowfiles/cataclysm/AdtCataTextures.h>
#include <wowfiles/Chunk.h>
#include <wowfiles/cataclysm/McnkCataTextures.h>
#include <utilities/Utilities.h>

AdtCataTextures::AdtCataTextures(const std::string & adtFileName) : adtName(adtFileName)
{
  std::ifstream adtFile;
  adtFile.open(adtFileName.c_str(), std::ios::binary);

  adtFile.seekg(0, std::ios::end);
  const int fileSize = adtFile.tellg();
  adtFile.seekg(0, std::ios::beg);
  
  int offsetInFile (0);

  int chunkName (Utilities::getIntFromFile(adtFile, offsetInFile));

  while (offsetInFile < fileSize)
  {
    switch (chunkName)
    {
      case 'MVER' :
        texturesMver = Chunk(adtFile, offsetInFile);
        offsetInFile = offsetInFile + chunkLettersAndSize + texturesMver.getGivenSize();
        break;

      case 'MAMP' :
        mamp = Chunk(adtFile, offsetInFile);
        offsetInFile = offsetInFile + chunkLettersAndSize + mamp.getGivenSize();
        break;

      case 'MTEX' :
        mtex = Chunk(adtFile, offsetInFile);
        offsetInFile = offsetInFile + chunkLettersAndSize + mtex.getGivenSize();
        break;		
		
      case 'MCNK' :
        texturesMcnks.push_back(McnkCataTextures(adtFile, offsetInFile));
        offsetInFile = offsetInFile + chunkLettersAndSize + texturesMcnks.back().getGivenSize(); 
        break;

      case 'MTXF' :
        mtxf = Chunk(adtFile, offsetInFile);
        offsetInFile = offsetInFile + chunkLettersAndSize + mtxf.getGivenSize();
        break;	

      case 'MTXP' :
        mtxp = Chunk(adtFile, offsetInFile);
        offsetInFile = offsetInFile + chunkLettersAndSize + mtxp.getGivenSize();
        break;
		
      default :
        texturesUnknown.push_back(Chunk(adtFile, offsetInFile));
        offsetInFile = offsetInFile + chunkLettersAndSize + texturesUnknown.back().getGivenSize();
    }
	
    chunkName = Utilities::getIntFromFile(adtFile, offsetInFile);
  }	
}

void AdtCataTextures::toFile()
{
  // TODO.
}

std::ostream & operator<<(std::ostream & os, const AdtCataTextures & adtCataTextures)
{
  os << "------------------------------" << std::endl;
  os << adtCataTextures.adtName << std::endl;
  os << "------------------------------" << std::endl;
  os << adtCataTextures.texturesMver;
  os << adtCataTextures.mamp;
  os << adtCataTextures.mtex;

  std::vector<McnkCataTextures>::const_iterator mcnksTexturesIter;
  int i (0);

  for (mcnksTexturesIter = adtCataTextures.texturesMcnks.begin() ; mcnksTexturesIter != adtCataTextures.texturesMcnks.end() ; ++mcnksTexturesIter)
  {
    os << "MCNK (textures) #" << i << " : " << std::endl;
    os << *mcnksTexturesIter;
    ++i;
  }

  os << adtCataTextures.mtxf;
  os << adtCataTextures.mtxp;

  std::vector<Chunk>::const_iterator texturesUnknownIter;

  for (texturesUnknownIter = adtCataTextures.texturesUnknown.begin() ; texturesUnknownIter != adtCataTextures.texturesUnknown.end() ; ++texturesUnknownIter)
  {
    os << *texturesUnknownIter;
  }     
  
  return os;
}