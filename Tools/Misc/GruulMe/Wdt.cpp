#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include "Wdt.h"
#include "Chunk.h"
#include "Utilities.h"

static const int chunkLettersAndSize = 8;

Wdt::Wdt()
{
	Chunk emptyChunk = Chunk();
    wdtName = "none";

    // WMO related
    mwmo = emptyChunk;
	modf = emptyChunk;

    // MVER
    std::vector<char> mverData = Utilities::getCharVectorFromInt(18);
    mver = Chunk("REVM", 4, mverData);

    // MPHD
    std::vector<char> mphdData (32);
    mphd = Chunk("DHPM", 32, mphdData);

    // MAIN
    std::vector<char> mainData (32768);
    main = Chunk("NIAM", 32768, mainData);
}

Wdt::Wdt(const std::string & wdtFileName) : wdtName(wdtFileName)
{
  std::ifstream wdtFile;
  wdtFile.open(wdtName.c_str(), std::ios::binary);

  int offsetInFile = 0;

  mver = Chunk(wdtFile, offsetInFile);
  offsetInFile = chunkLettersAndSize + offsetInFile + mver.getGivenSize();

  mphd = Chunk(wdtFile, offsetInFile);
  offsetInFile = chunkLettersAndSize + offsetInFile + mphd.getGivenSize();

  main = Chunk(wdtFile, offsetInFile);
  offsetInFile = chunkLettersAndSize + offsetInFile + main.getGivenSize();

  const int mphdFlags = mphd.getOffset(0);
  if (Utilities::flagsExist(mphdFlags, 0x1))
  {
    mwmo = Chunk(wdtFile, offsetInFile);
    offsetInFile = chunkLettersAndSize + offsetInFile + mwmo.getGivenSize();
  }

  if (mwmo.getGivenSize() != 0)
  {
    modf = Chunk(wdtFile, offsetInFile);
    offsetInFile = chunkLettersAndSize + offsetInFile + modf.getGivenSize();
  }

  wdtFile.close();
}

Wdt::Wdt(const std::string & name
  , const Chunk & cMver
  , const Chunk & cMphd
  , const Chunk & cMain
  , const Chunk & cMwmo
  , const Chunk & cModf
  ) : wdtName(name)
  , mver(cMver)
  , mphd(cMphd)
  , main(cMain)
  , mwmo(cMwmo)
  , modf(cModf)
{
}


std::vector<Chunk> Wdt::getFullWDT()
{
    std::vector<Chunk> v (5);
    v[0] = mver;
    v[1] = mphd;
    v[2] = main;
    v[3] = mwmo;
    v[4] = modf;
    return v;
}

// MPHD
int Wdt::getFlags()
{
	return mphd.getOffset(0);
}

void Wdt::setFlags(int flags)
{
    std::vector<char> mphdData = mphd.getData();
    mphdData = Utilities::getCharSubVector(mphdData, 4, 28);
    std::vector<char> flagsData = Utilities::getCharVectorFromInt(flags);
    flagsData.insert(flagsData.end(), mphdData.begin(), mphdData.end());
    mphd.setData(flagsData);
}

// MAIN
int Wdt::getMAINentry(const int X, const int Y)
{
    return main.getOffset((X*64 + Y) * 8);
}

void Wdt::setMAINentry(const int X, const int Y, const char flag)
{
    main.setCharInData(flag, (X*64 + Y) * 8);
}

// MWMO/MODF
std::string Wdt::getWMOpath()
{
	std::string wmoStr = "";
	if (Utilities::flagsExist(mphd.getOffset(0), 0x1))
	{
		int size = mwmo.getGivenSize();
		wmoStr = Utilities::getStringFromCharVector(mwmo.getWholeChunk(), chunkLettersAndSize, size - 1);
	}
	return wmoStr;
}

std::vector<int> Wdt::getWMOIDinfos()
{
    std::vector<char> modfData = modf.getData();
    std::vector<int> addInfos (2);
    addInfos[0] = Utilities::getIntFromCharVector(modfData, 0);
    addInfos[1] = Utilities::getIntFromCharVector(modfData, 4);
    return addInfos;
}

std::vector<float> Wdt::getWMOpos()
{
    std::vector<char> modfData = modf.getData();
    std::vector<float> pos (3);
    pos[0] = Utilities::getFloatFromCharVector(modfData, 8);
    pos[1] = Utilities::getFloatFromCharVector(modfData, 12);
    pos[2] = Utilities::getFloatFromCharVector(modfData, 16);
    return pos;
}

std::vector<float> Wdt::getWMOori()
{
    std::vector<char> modfData = modf.getData();
    std::vector<float> pos (3);
    pos[0] = Utilities::getFloatFromCharVector(modfData, 20);
    pos[1] = Utilities::getFloatFromCharVector(modfData, 24);
    pos[2] = Utilities::getFloatFromCharVector(modfData, 28);
    return pos;
}

std::vector<float> Wdt::getWMOext()
{
    std::vector<char> modfData = modf.getData();
    std::vector<float> pos (6);
    pos[0] = Utilities::getFloatFromCharVector(modfData, 32);
    pos[1] = Utilities::getFloatFromCharVector(modfData, 36);
    pos[2] = Utilities::getFloatFromCharVector(modfData, 40);
    pos[3] = Utilities::getFloatFromCharVector(modfData, 44);
    pos[4] = Utilities::getFloatFromCharVector(modfData, 48);
    pos[5] = Utilities::getFloatFromCharVector(modfData, 52);
    return pos;
}

std::vector<short> Wdt::getWMOaddInfos()
{
    std::vector<char> modfData = modf.getData();
    std::vector<short> addInfos (3);
    addInfos[0] = Utilities::getShortFromCharVector(modfData, 56);
    addInfos[1] = Utilities::getShortFromCharVector(modfData, 58);
    addInfos[2] = Utilities::getShortFromCharVector(modfData, 60);
    return addInfos;
}

Chunk Wdt::buildMWMO(std::string n)
{
    std::vector<char> data (n.begin(), n.end());
    data.push_back(0x00);
    Chunk c ("OMWM", data.size(), data);
    return c;
}

Chunk Wdt::buildMODF(
        int id,
        int uid,
        float posX,
        float posY,
        float posZ,
        float oriA,
        float oriB,
        float oriC,
        float upe1,
        float upe2,
        float upe3,
        float loe1,
        float loe2,
        float loe3,
        short flags,
        short dds,
        short ns)
{
    std::vector<char> data (0);
    std::vector<char> Vid = Utilities::getCharVectorFromInt(id);
    std::vector<char> Vuid = Utilities::getCharVectorFromInt(uid);
    std::vector<char> VposX = Utilities::getCharVectorFromFloat(posX);
    std::vector<char> VposY = Utilities::getCharVectorFromFloat(posY);
    std::vector<char> VposZ = Utilities::getCharVectorFromFloat(posZ);
    std::vector<char> VoriA = Utilities::getCharVectorFromFloat(oriA);
    std::vector<char> VoriB = Utilities::getCharVectorFromFloat(oriB);
    std::vector<char> VoriC = Utilities::getCharVectorFromFloat(oriC);
    std::vector<char> Vupe1 = Utilities::getCharVectorFromFloat(upe1);
    std::vector<char> Vupe2 = Utilities::getCharVectorFromFloat(upe2);
    std::vector<char> Vupe3 = Utilities::getCharVectorFromFloat(upe3);
    std::vector<char> Vloe1 = Utilities::getCharVectorFromFloat(loe1);
    std::vector<char> Vloe2 = Utilities::getCharVectorFromFloat(loe2);
    std::vector<char> Vloe3 = Utilities::getCharVectorFromFloat(loe3);
    std::vector<char> Vflags = Utilities::getCharVectorFromShort(flags);
    std::vector<char> Vdds = Utilities::getCharVectorFromShort(dds);
    std::vector<char> Vns = Utilities::getCharVectorFromShort(ns);
    data.insert(data.end(), Vid.begin(), Vid.end());
    data.insert(data.end(), Vuid.begin(), Vuid.end());
    data.insert(data.end(), VposX.begin(), VposX.end());
    data.insert(data.end(), VposY.begin(), VposY.end());
    data.insert(data.end(), VposZ.begin(), VposZ.end());
    data.insert(data.end(), VoriA.begin(), VoriA.end());
    data.insert(data.end(), VoriB.begin(), VoriB.end());
    data.insert(data.end(), VoriC.begin(), VoriC.end());
    data.insert(data.end(), Vupe1.begin(), Vupe1.end());
    data.insert(data.end(), Vupe2.begin(), Vupe2.end());
    data.insert(data.end(), Vupe3.begin(), Vupe3.end());
    data.insert(data.end(), Vloe1.begin(), Vloe1.end());
    data.insert(data.end(), Vloe2.begin(), Vloe2.end());
    data.insert(data.end(), Vloe3.begin(), Vloe3.end());
    data.insert(data.end(), Vflags.begin(), Vflags.end());
    data.insert(data.end(), Vdds.begin(), Vdds.end());
    data.insert(data.end(), Vns.begin(), Vns.end());
    data.push_back(0x00);
    data.push_back(0x00); // padding
    Chunk c ("FDOM", data.size(), data);
    return c;
}


// other methods
void Wdt::toFile()
{
  std::string fileName = wdtName;
  std::ofstream outputFile(fileName.c_str(), std::ios::out|std::ios::binary);
  if (outputFile.is_open())
  {
    outputFile.write((char *)&mver.getWholeChunk()[0], sizeof(char) * mver.getWholeChunk().size());
    outputFile.write((char *)&mphd.getWholeChunk()[0], sizeof(char) * mphd.getWholeChunk().size());
    outputFile.write((char *)&main.getWholeChunk()[0], sizeof(char) * main.getWholeChunk().size());

    if (!mwmo.isEmpty())
      outputFile.write((char *)&mwmo.getWholeChunk()[0], sizeof(char) * mwmo.getWholeChunk().size());

    if (!modf.isEmpty())
      outputFile.write((char *)&modf.getWholeChunk()[0], sizeof(char) * modf.getWholeChunk().size());
  }

  outputFile.close();
}

std::ostream & operator<<(std::ostream & os, const Wdt & wdt)
{
  os << wdt.wdtName << std::endl;
  os << "------------------------------" << std::endl;
  os << wdt.mver;
  os << wdt.mphd;
  os << wdt.main;
  os << wdt.mwmo;
  os << wdt.modf;

  return os;
}

