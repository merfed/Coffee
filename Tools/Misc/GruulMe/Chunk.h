#ifndef CHUNK_H_
#define CHUNK_H_

#include <vector>
#include <iostream>
#include <fstream>
#include <string>

class Chunk
{
        public:

                Chunk();
                Chunk(std::ifstream & file, int offsetInFile);
                Chunk(std::string letters, int givenSize, const std::vector<char> & chunkData);

                int getGivenSize() const;
                int getRealSize() const;
                bool isEmpty() const;
                std::vector<char> getWholeChunk() const;
                int getOffset(const int offsetInData) const;

                std::vector<char> getData();
                void setData(const std::vector<char> dataToSet);
                void setCharInData(char c, int offset); // use with caution !

                friend std::ostream & operator<<(std::ostream & os, const Chunk & chunk);

        protected:

                int getChunkSizeFromFullData(const std::vector<char> & fullData, int position) const;

                std::string letters;
                int givenSize;
                std::vector<char> data;
};

#endif
