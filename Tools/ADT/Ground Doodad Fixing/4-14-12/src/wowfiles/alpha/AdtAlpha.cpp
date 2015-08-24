#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <wowfiles/Chunk.h>
#include <wowfiles/alpha/AdtAlpha.h>
#include <wowfiles/lichking/AdtLk.h>
#include <wowfiles/alpha/McnkAlpha.h>
#include <wowfiles/Mcin.h>

AdtAlpha::AdtAlpha(std::string & wdtAlphaName, int offsetInFile, int adtNum) : adtNumber(adtNum)
{
  std::ifstream wdtAlphaFile;
  wdtAlphaFile.open(wdtAlphaName.c_str(), std::ios::binary);

  adtFileName = getAdtFileName(wdtAlphaName);
  
  const int mcinOffset (0x0);
  const int mtexOffset (0x4);
  const int mddfOffset (0x0C);
  const int modfOffset (0x14);
  
  mhdr = Chunk(wdtAlphaFile, offsetInFile);

  const int MhdrStartOffset (offsetInFile + chunkLettersAndSize);

  offsetInFile = MhdrStartOffset + mhdr.getOffset(mcinOffset);
  mcin = Mcin(wdtAlphaFile, offsetInFile);

  offsetInFile = MhdrStartOffset + mhdr.getOffset(mtexOffset);
  mtex = Chunk(wdtAlphaFile, offsetInFile);

  offsetInFile = MhdrStartOffset + mhdr.getOffset(mddfOffset);
  mddf = Chunk(wdtAlphaFile, offsetInFile);

  offsetInFile = MhdrStartOffset + mhdr.getOffset(modfOffset);
  modf = Chunk(wdtAlphaFile, offsetInFile);

  std::vector<int> mcnkOffsets (mcin.getMcnkOffsets());
  int currentMcnk;

  for (currentMcnk = 0 ; currentMcnk < 256 ; ++currentMcnk)
  {
    offsetInFile = mcnkOffsets[currentMcnk];
    mcnksAlpha.push_back(McnkAlpha(wdtAlphaFile, offsetInFile, mcnkTerrainHeaderSize, adtNumber));
  }
}

std::string AdtAlpha::getAdtFileName(const std::string & wdtName) const
{
  std::string adtFileName (wdtName);
  adtFileName = adtFileName.substr(0, adtFileName.size() - 4);
  adtFileName.append("_");

  std::stringstream tempStream;
  
  tempStream << getXCoord();
  adtFileName.append(tempStream.str());

  adtFileName.append("_");
  tempStream.str("");

  tempStream << getYCoord();
  adtFileName.append(tempStream.str());
  
  adtFileName.append(".adt");

  return adtFileName;
}

int AdtAlpha::getXCoord() const
{
  return adtNumber % 64;
}

int AdtAlpha::getYCoord() const
{
  return adtNumber / 64;
}

AdtLk AdtAlpha::toAdtLk() const
{  
  std::string cName (adtFileName);

  std::vector<char> mverData;
  mverData.push_back(0x12);
  mverData.push_back(0x0);
  mverData.push_back(0x0);
  mverData.push_back(0x0);
  Chunk cMver ("REVM", 4, mverData);

  Mh2o (cMh2o);
  
  std::vector<char> emptyData(0);

  Chunk cMmdx ("XDMM", 0, emptyData); // TODO : fill emptiness for objects
  Chunk cMmid ("DIMM", 0, emptyData);
  Chunk cMwmo ("OMWM", 0, emptyData);
  Chunk cMwid ("DIWM", 0, emptyData);
  Chunk cMddf ("FDDM", 0, emptyData);
  Chunk cModf ("FDOM", 0, emptyData);
  
  std::vector<McnkLk> cMcnks;
  int currentMcnk;

  for (currentMcnk = 0 ; currentMcnk < 256 ; ++currentMcnk)
  {
    cMcnks.push_back(mcnksAlpha[currentMcnk].toMcnkLk());
  }

  Chunk (cMfbo);
  Chunk (cMtxf);

  const int mhdrFlags = 0;

  AdtLk adtLk(cName, cMver, mhdrFlags, cMh2o, mtex, cMmdx, cMmid, cMwmo, cMwid, cMddf, cModf, cMcnks, cMfbo, cMtxf);
  return adtLk;
}

std::ostream & operator<<(std::ostream & os, const AdtAlpha & adtAlpha)
{
  os << "ADT #" << adtAlpha.adtNumber << std::endl;
  os << "------------------------------" << std::endl;
  os << adtAlpha.mhdr;
  os << adtAlpha.mcin;
  os << adtAlpha.mtex;
  os << adtAlpha.mddf;
  os << adtAlpha.modf;

  std::vector<McnkAlpha>::const_iterator mcnksIter;
  int i = 0;

  for (mcnksIter = adtAlpha.mcnksAlpha.begin() ; mcnksIter != adtAlpha.mcnksAlpha.end() ; ++mcnksIter)
  {
    os << "MCNK #" << i << " : " << std::endl;
    os << *mcnksIter;
    ++i;
  }

  return os;
}
