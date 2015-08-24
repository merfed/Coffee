#ifndef _WOWFILES_CATACLYSM_MCNKCATATEXTURES_H_
#define _WOWFILES_CATACLYSM_MCNKCATATEXTURES_H_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <wowfiles/Chunk.h>
#include <wowfiles/Mcnk.h>

class McnkCataTextures : public Mcnk
{
  public:

    McnkCataTextures(std::ifstream & file, int offsetInFile);
    McnkCataTextures(std::string letters, int givenSize, const std::vector<char> &data);

	  void toFile();
	
    friend std::ostream & operator<<(std::ostream & os, const McnkCataTextures & mcnkCataTextures);

  private:

    Chunk mcly;
    Chunk mcsh;
    Chunk mcal;
    Chunk mcmt;
	  std::vector<Chunk> texturesMcnkUnknown;
};

#endif