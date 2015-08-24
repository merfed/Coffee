#ifndef _WOWFILES_ALPHA_ADTALPHA_H_
#define _WOWFILES_ALPHA_ADTALPHA_H_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <wowfiles/WowChunkedFormat.h>
#include <wowfiles/Chunk.h>
#include <wowfiles/alpha/McnkAlpha.h>
#include <wowfiles/lichking/AdtLk.h>
#include <wowfiles/Mcin.h>

class AdtAlpha : public WowChunkedFormat
{
  public:

    AdtAlpha(std::string & wdtAlphaName, int offsetInFile, int adtNum);
    int getXCoord() const;
    int getYCoord() const;
    AdtLk toAdtLk() const;

    friend std::ostream & operator<<(std::ostream & os, const AdtAlpha & adtAlpha);

  private:

    std::string getAdtFileName(const std::string & wdtName) const;

    int adtNumber;
    std::string adtFileName;
    Chunk mhdr;
    Mcin mcin;
    Chunk mtex;
    Chunk mddf;
    Chunk modf;
    std::vector<McnkAlpha> mcnksAlpha;
};

#endif