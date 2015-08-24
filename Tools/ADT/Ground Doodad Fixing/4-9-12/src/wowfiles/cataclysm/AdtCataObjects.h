#ifndef _WOWFILES_CATACLYSM_ADTCATAOBJECTS_H_
#define _WOWFILES_CATACLYSM_ADTCATAOBJECTS_H_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <wowfiles/WowChunkedFormat.h>
#include <wowfiles/Chunk.h>
#include <wowfiles/Mcnk.h>
#include <wowfiles/cataclysm/McnkCataObjects.h>

class AdtCataObjects : public WowChunkedFormat
{
  public:

    AdtCataObjects(const std::string & adtFile);

    void toFile();

    friend std::ostream & operator<<(std::ostream & os, const AdtCataObjects & adtCataObjects);

  private:

    std::string adtName;
    Chunk objectsMver;
    Chunk mmdx;
    Chunk mmid;
    Chunk mwmo;
    Chunk mwid;
    Chunk mddf;
    Chunk modf;
    std::vector<McnkCataObjects> objectsMcnks;
	  std::vector<Chunk> objectsUnknown;
};

#endif