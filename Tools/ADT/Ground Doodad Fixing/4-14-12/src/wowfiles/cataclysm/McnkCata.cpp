#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <wowfiles/Chunk.h>
#include <wowfiles/Mcnk.h>
#include <wowfiles/cataclysm/McnkCata.h>
#include <wowfiles/cataclysm/McnrCata.h>
#include <wowfiles/lichking/McnkLk.h>
#include <utilities/Utilities.h>

McnkCata::McnkCata(std::ifstream & adtFile, int offsetInFile) : Mcnk(adtFile, offsetInFile, 128)
{
  const int headerStartOffset (offsetInFile + chunkLettersAndSize);
  const int absoluteMcnkEnd = offsetInFile + chunkLettersAndSize + givenSize;

  offsetInFile = chunkLettersAndSize + offsetInFile;

  getHeaderFromFile(adtFile, offsetInFile, mcnkTerrainHeaderSize);

  offsetInFile = headerStartOffset + mcnkTerrainHeaderSize;
  
  int chunkName (Utilities::getIntFromFile(adtFile, offsetInFile));

  while (offsetInFile < absoluteMcnkEnd)
  {
    switch (chunkName)
    {
      case 'MCVT' :
        mcvt = Chunk(adtFile, offsetInFile);
        offsetInFile = offsetInFile + chunkLettersAndSize + mcvt.getGivenSize();
        break;  

      case 'MCCV' :
        mccv = Chunk(adtFile, offsetInFile);
        offsetInFile = offsetInFile + chunkLettersAndSize + mccv.getGivenSize();
        break;  		

      case 'MCLV' :
        mclv = Chunk(adtFile, offsetInFile);
        offsetInFile = offsetInFile + chunkLettersAndSize + mclv.getGivenSize();
        break;  

      case 'MCNR' :
        mcnr = McnrCata(adtFile, offsetInFile);
        offsetInFile = offsetInFile + chunkLettersAndSize + mcnr.getGivenSize();
        break;  		

      case 'MCLQ' :
        mclq = Chunk(adtFile, offsetInFile);
        offsetInFile = offsetInFile + chunkLettersAndSize + mclq.getGivenSize();
        break;  
		
      case 'MCSE' :
        mcse = Chunk(adtFile, offsetInFile);
        offsetInFile = offsetInFile + chunkLettersAndSize + mcse.getGivenSize();
        break;  	
		
      default :
        terrainMcnkUnknown.push_back(Chunk(adtFile, offsetInFile));
        offsetInFile = offsetInFile + chunkLettersAndSize + terrainMcnkUnknown.back().getGivenSize();
    }
	
    chunkName = Utilities::getIntFromFile(adtFile, offsetInFile);
  }		
}

void McnkCata::toFile()
{
  // TODO
}

McnkLk McnkCata::toMcnkLk() // TODO : probably shouldn't be here.
{
  std::vector<char> emptyData (0); 
  Chunk (emptyChunk); 

  McnrLk cMcnr(mcnr.toMcnrLk());

  Chunk (cMccv);
  if (!mccv.isEmpty())
    cMccv = mccv;

  Chunk cMcly ("YLCM", 0, emptyData);
  Chunk cMcrf ("FRCM", 0, emptyData);
  Chunk cMcsh ("HSCM", 0, emptyData);
  Mcal (cMcal);

  McnkLk mcnkLk = McnkLk(mcnkHeader, mcvt, cMccv, cMcnr, cMcly, cMcrf, cMcsh, cMcal, emptyChunk, emptyChunk);
  return mcnkLk;
}

void McnkCata::getHeaderFromFile(std::ifstream & adtFile, const int position, const int length)
{
  adtFile.seekg(position, std::ios::beg);
  char * dataBuffer = new char[length];

  adtFile.read(dataBuffer, length);

  mcnkHeader = *reinterpret_cast<McnkHeader*>(dataBuffer);

  delete[] dataBuffer;
}

std::ostream & operator<<(std::ostream & os, const McnkCata & mcnkCata)
{
  os << "Chunk letters : " << mcnkCata.letters << std::endl;
  os << "Chunk givenSize : " << mcnkCata.givenSize << std::endl;

  os << "------------------------------" << std::endl;

  os << mcnkCata.mcvt;
  os << mcnkCata.mccv;
  os << mcnkCata.mclv;
  os << mcnkCata.mcnr;
  os << mcnkCata.mclq;
  os << mcnkCata.mcse;

  os << "Hi, in here we're unknown !" << std::endl;
  os << std::endl;

  std::vector<Chunk>::const_iterator unknownIter;

  for (unknownIter = mcnkCata.terrainMcnkUnknown.begin() ; unknownIter != mcnkCata.terrainMcnkUnknown.end() ; ++unknownIter)
  {
    os << *unknownIter;
  }
  
  os << "------------------------------" << std::endl;
  return os;
}
