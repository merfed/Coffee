#ifndef _WOWFILES_CATACLYSM_MCNKCATAOBJECTS_H_
#define _WOWFILES_CATACLYSM_MCNKCATAOBJECTS_H_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <wowfiles/Chunk.h>
#include <wowfiles/Mcnk.h>

class McnkCataObjects : public Mcnk
{
  public:

    McnkCataObjects(std::ifstream & file, int offsetInFile);
    McnkCataObjects(const std::vector<char> & wholeFile, int offsetInFile);
    McnkCataObjects(std::string letters, int givenSize, const std::vector<char> &data);

	  void toFile();
	
	  friend std::ostream & operator<<(std::ostream & os, const McnkCataObjects & mcnkCataObjects);
	
  private:

    Chunk mcrd;
    Chunk mcrw;
	  std::vector<Chunk> objectsMcnkUnknown;
};

#endif