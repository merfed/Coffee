#ifndef _WOWFILES_ALPHA_WDT_ALPHA_H_
#define _WOWFILES_ALPHA_WDT_ALPHA_H_

#include <vector>
#include <string>
#include <wowfiles/Wdt.h>
#include <wowfiles/WowChunkedFormat.h>
#include <wowfiles/Chunk.h>
#include <wowfiles/alpha/MphdAlpha.h>
#include <wowfiles/alpha/MainAlpha.h>
#include <wowfiles/alpha/Monm.h>
#include <wowfiles/alpha/AdtAlpha.h>

class WdtAlpha : public WowChunkedFormat
{
  public:

    WdtAlpha(const std::string & wdtAlphaName);
    Wdt toWdt() const;
    std::vector<int> getExistingAdtsNumbers() const;
    std::vector<int> getAdtOffsetsInMain() const; // TODO : change this

    friend std::ostream & operator<<(std::ostream & os, const WdtAlpha & wdtAlpha);

  private:

    std::string wdtName;
    Chunk mver;
    MphdAlpha mphd;
    MainAlpha main;
    Chunk mdnm;
    Monm monm;
    Chunk modf;
};

#endif