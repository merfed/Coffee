#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <string>
#include <wowfiles/cataclysm/AdtCataObjects.h>
#include <wowfiles/Chunk.h>
#include <wowfiles/cataclysm/McnkCataObjects.h>
#include <utilities/Utilities.h>

AdtCataObjects::AdtCataObjects(const std::string & adtFileName) : adtName(adtFileName)
{
  std::vector<char> buffer(0);
  Utilities::getWholeFile(adtFileName, buffer);
  
  const int fileSize (buffer.size());
  int offsetInFile (0);

  int chunkName (Utilities::get<int>(buffer, offsetInFile));

  while (offsetInFile < fileSize)
  {
    switch (chunkName)
    {
      case 'MVER' :
        objectsMver = Chunk(buffer, offsetInFile);
        offsetInFile = offsetInFile + chunkLettersAndSize + objectsMver.getGivenSize();
        break;

      case 'MMDX' :
        mmdx = Chunk(buffer, offsetInFile);
        offsetInFile = offsetInFile + chunkLettersAndSize + mmdx.getGivenSize();
        break;
		
      case 'MMID' :
        mmid = Chunk(buffer, offsetInFile);
        offsetInFile = offsetInFile + chunkLettersAndSize + mmid.getGivenSize();
        break;

      case 'MWMO' :
        mwmo = Chunk(buffer, offsetInFile);
        offsetInFile = offsetInFile + chunkLettersAndSize + mwmo.getGivenSize();
        break;

      case 'MWID' :
        mwid = Chunk(buffer, offsetInFile);
        offsetInFile = offsetInFile + chunkLettersAndSize + mwid.getGivenSize();
        break;

      case 'MDDF' :
        mddf = Chunk(buffer, offsetInFile);
        offsetInFile = offsetInFile + chunkLettersAndSize + mddf.getGivenSize();
        break;	 		
		
      case 'MODF' :
        modf = Chunk(buffer, offsetInFile);
        offsetInFile = offsetInFile + chunkLettersAndSize + modf.getGivenSize();
        break;	  	

      /*case 'MCNK' :
        objectsMcnks.push_back(McnkCataObjects(buffer, offsetInFile));
        offsetInFile = offsetInFile + chunkLettersAndSize + objectsMcnks.back().getGivenSize();
        break;*/

      default :
        //std::cout << offsetInFile << std::endl;
        objectsUnknown.push_back(Chunk(buffer, offsetInFile));
        offsetInFile = offsetInFile + chunkLettersAndSize + objectsUnknown.back().getGivenSize();
    }
    
    chunkName = Utilities::get<int>(buffer, offsetInFile);
  }
}

void AdtCataObjects::toFile()
{
  // TODO.
}

std::ostream & operator<<(std::ostream & os, const AdtCataObjects & adtCataObjects)
{
  os << "------------------------------" << std::endl;
  os << adtCataObjects.adtName << std::endl;
  os << "------------------------------" << std::endl;
  os << adtCataObjects.objectsMver;
  os << adtCataObjects.mmdx;
  os << adtCataObjects.mmid;
  os << adtCataObjects.mwmo;
  os << adtCataObjects.mwid;
  os << adtCataObjects.mddf;
  os << adtCataObjects.modf;

  std::vector<McnkCataObjects>::const_iterator mcnksObjectsIter;
  int i (0);
  
  for (mcnksObjectsIter = adtCataObjects.objectsMcnks.begin() ; mcnksObjectsIter != adtCataObjects.objectsMcnks.end() ; ++mcnksObjectsIter)
  {
    os << "MCNK (objects) #" << i << " : " << std::endl;
    os << *mcnksObjectsIter;
    ++i;
  }

  std::vector<Chunk>::const_iterator objectsUnknownIter;

  for (objectsUnknownIter = adtCataObjects.objectsUnknown.begin() ; objectsUnknownIter != adtCataObjects.objectsUnknown.end() ; ++objectsUnknownIter)
  {
    os << *objectsUnknownIter;
  }    
  
  return os;
}