#ifndef _WOWFILES_CHUNK_H_
#define _WOWFILES_CHUNK_H_

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <wowfiles/WowChunkedFormat.h>

class Chunk : public WowChunkedFormat
{
  public:

    Chunk();
    Chunk(std::ifstream & file, int offsetInFile);
    Chunk(const std::vector<char> & wholeFile, int offsetInFile);
    Chunk(std::string letters, int givenSize, const std::vector<char> & chunkData);

    int getGivenSize() const;
    int getRealSize() const;
    bool isEmpty() const;
    std::vector<char> getWholeChunk() const;
    int getOffset(const int offsetInData) const;
    void toFile(std::string & fileName);

    friend std::ostream & operator<<(std::ostream & os, const Chunk & chunk);

  //protected:

    int getChunkSizeFromFullData(const std::vector<char> & fullData, int position) const;

    std::string letters;
    int givenSize;
    std::vector<char> data;
};

#endif
