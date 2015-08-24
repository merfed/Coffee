#ifndef _WOWFILES_WDL_H_
#define _WOWFILES_WDL_H_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <wowfiles/WowChunkedFormat.h>
#include <wowfiles/Chunk.h>

class Wdl : public WowChunkedFormat
{
  public:

    Wdl(const std::vector<char> & wdlFile, const std::string & wdlFileName);
    Wdl(const std::string & name
      , const Chunk & cMver
      , const Chunk & cMwmo
      , const Chunk & cMwmid
      , const Chunk & cModf
      , const Chunk & cMaof
      , const std::vector<Chunk> & cMareAndMaho  
    );
    void toFile();
    friend std::ostream & operator<<(std::ostream & os, const Wdl & wdl);

  private:
  
    std::string wdlName;
    Chunk mver;
    Chunk mwmo;
    Chunk mwid;
    Chunk modf;
    Chunk maof;
    std::vector<Chunk> mareAndMaho;
};

#endif