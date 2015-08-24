#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <wowfiles/Chunk.h>
#include <wowfiles/MainChunk.h>

MainChunk::MainChunk() : Chunk()
{
}

MainChunk::MainChunk(std::ifstream & adtFile, int offsetInFile) : Chunk(adtFile, offsetInFile)
{
}

MainChunk::MainChunk(std::string letters, int givenSize, const std::vector<char> & data) : Chunk("NIAM", givenSize, data)
{
}
