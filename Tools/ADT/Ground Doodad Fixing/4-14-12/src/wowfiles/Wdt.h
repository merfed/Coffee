#ifndef _WOWFILES_WDT_H_
#define _WOWFILES_WDT_H_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <wowfiles/WowChunkedFormat.h>
#include <wowfiles/Chunk.h>
#include <wowfiles/Mphd.h>

class Wdt : public WowChunkedFormat
{
  public:

    Wdt(const std::vector<char> & wdtFile, const std::string & wdtFileName);
    Wdt(const std::string & name
      , const Chunk & cMver
      , const Mphd & cMphd
      , const Chunk & cMain
      , const Chunk & cMwmo
      , const Chunk & cModf
    );
    void toFile();
    friend std::ostream & operator<<(std::ostream & os, const Wdt & wdt);

  private:

    std::string wdtName;
    Chunk mver;
    Mphd mphd;
    Chunk main;
    Chunk mwmo;
    Chunk modf;
};

#endif