#ifndef _WOWFILES_MHDR_H_
#define _WOWFILES_MHDR_H_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <wowfiles/Chunk.h>

class Mhdr : public Chunk
{
  public:
  
    Mhdr();
    Mhdr(std::ifstream & adtFile, int offsetInFile);
	Mhdr(const std::vector<char> & wholeFile, int offsetInFile);
    Mhdr(const  std::string letters, int givenSize, const std::vector<char> & chunkData);

    int getFlags();

    static const int mcinOffset = 4;
    static const int mh2oOffset = 40;
    static const int mtexOffset = 8;
    static const int mmdxOffset = 12;
    static const int mmidOffset = 16;
    static const int mwmoOffset = 20;
    static const int mwidOffset = 24;
    static const int mddfOffset = 28;
    static const int modfOffset = 32;
    static const int mfboOffset = 36;
    static const int mtxfOffset = 44;
	
  /*private: // TODO : decide if I use this or not.
    
	MhdrContent mhdrContent;*/
};

#endif