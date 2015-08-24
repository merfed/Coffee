#ifndef _WOWFILES_MCNK_H_
#define _WOWFILES_MCNK_H_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <wowfiles/ChunkHeaders.h>
#include <wowfiles/Chunk.h>

/*

Note : global idea is that Mcnk.data only contains header information, to avoid too much data duplication. 
Headers have different sizes/structures (Cata ones can be empty), so they're defined in derived classes only.

*/

class Mcnk : public Chunk
{
  public:
  
    Mcnk();
    Mcnk(std::ifstream & adtFile, int offsetInFile, const int & headerSize);
    Mcnk(const std::vector<char> & wholeFile, int offsetInFile, const int & headerSize);
    Mcnk(const std::string letters, int givenSize, const std::vector<char> & chunkData);
		
    void toFile();
};

#endif