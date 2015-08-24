#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <wowfiles/Wdl.h>
#include <wowfiles/Chunk.h>
#include <utilities/Utilities.h>

Wdl::Wdl(const std::vector<char> & wdlFile, const std::string & wdlFileName) : wdlName(wdlFileName)
{
  const int fileSize (wdlFile.size());
  
  int offsetInFile (0);

  mver = Chunk(wdlFile, offsetInFile);
  offsetInFile = chunkLettersAndSize + offsetInFile + mver.getGivenSize();

  mwmo = Chunk(wdlFile, offsetInFile);
  offsetInFile = chunkLettersAndSize + offsetInFile + mwmo.getGivenSize();

  mwid = Chunk(wdlFile, offsetInFile);
  offsetInFile = chunkLettersAndSize + offsetInFile + mwid.getGivenSize();

  modf = Chunk(wdlFile, offsetInFile);
  offsetInFile = chunkLettersAndSize + offsetInFile + modf.getGivenSize();

  maof = Chunk(wdlFile, offsetInFile);
  offsetInFile = chunkLettersAndSize + offsetInFile + maof.getGivenSize();  

  while (offsetInFile < fileSize)
  {
    mareAndMaho.push_back(Chunk(wdlFile, offsetInFile));
	offsetInFile = chunkLettersAndSize + offsetInFile + mareAndMaho.back().getGivenSize();
  }
}

Wdl::Wdl(const std::string & name // TODO : that one is untested but should work
      , const Chunk & cMver
      , const Chunk & cMwmo
      , const Chunk & cMwid
      , const Chunk & cModf
      , const Chunk & cMaof
      , const std::vector<Chunk> & cMareAndMaho
  ) : wdlName(name)
  , mwmo(cMwmo)
  , mwid(cMwid)
  , modf(cModf)
  , maof(cMaof)
  , mareAndMaho(cMareAndMaho)
{
}

void Wdl::toFile()
{
  std::vector<char> wholeWdl(mver.getWholeChunk());

  std::vector<char> tempData(mwmo.getWholeChunk());
  wholeWdl.insert(wholeWdl.end(), tempData.begin(), tempData.end());

  tempData = mwid.getWholeChunk();
  wholeWdl.insert(wholeWdl.end(), tempData.begin(), tempData.end());

  tempData = modf.getWholeChunk();
  wholeWdl.insert(wholeWdl.end(), tempData.begin(), tempData.end());

  tempData = maof.getWholeChunk();
  wholeWdl.insert(wholeWdl.end(), tempData.begin(), tempData.end());

  int currentMareAndMaho;

  for (currentMareAndMaho = 0 ; currentMareAndMaho < mareAndMaho.size() ; ++currentMareAndMaho)
  {
    tempData =  mareAndMaho[currentMareAndMaho].getWholeChunk();
    wholeWdl.insert(wholeWdl.end(), tempData.begin(), tempData.end());
  }

  std::string fileName (wdlName);
  fileName.append("_new");
  std::ofstream outputFile (fileName.c_str(), std::ios::out|std::ios::binary);

  if (outputFile.is_open())
    outputFile.write((char *)&wholeWdl[0], sizeof(char) * wholeWdl.size());

  outputFile.close();
}

std::ostream & operator<<(std::ostream & os, const Wdl & wdl)
{
  os << wdl.wdlName << std::endl;
  os << "------------------------------" << std::endl;
  os << wdl.mver;
  os << wdl.mwmo;
  os << wdl.mwid;
  os << wdl.modf;
  os << wdl.maof;

  std::vector<Chunk>::const_iterator mareAndMahoIter;

  for (mareAndMahoIter = wdl.mareAndMaho.begin() ; mareAndMahoIter != wdl.mareAndMaho.end() ; ++mareAndMahoIter)
  {
    os << *mareAndMahoIter;
  }
  
  return os;
}
