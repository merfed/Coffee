#ifndef _WOWFILES_CATACLYSM_ADTCATA_H_
#define _WOWFILES_CATACLYSM_ADTCATA_H_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <wowfiles/WowChunkedFormat.h>
#include <wowfiles/Chunk.h>
#include <wowfiles/Mhdr.h>
#include <wowfiles/Mh2o.h>
#include <wowfiles/Mcnk.h>
#include <wowfiles/cataclysm/McnkCata.h>
#include <wowfiles/lichking/AdtLk.h>

class AdtCata : public WowChunkedFormat
{
  public:

    AdtCata(const std::string & adtFile);

    void toFile();
    AdtLk toAdtLk();

    friend std::ostream & operator<<(std::ostream & os, const AdtCata & adtCata);

  private:

    std::string adtName;

    Chunk terrainMver;
    Mhdr mhdr;
    Mh2o mh2o;
    std::vector<McnkCata> terrainMcnks;
    Chunk mfbo;
	  std::vector<Chunk> terrainUnknown;
};

#endif