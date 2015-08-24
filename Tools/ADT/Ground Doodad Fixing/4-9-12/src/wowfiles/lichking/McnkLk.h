#ifndef _WOWFILES_LICHKING_MCNKLK_H_
#define _WOWFILES_LICHKING_MCNKLK_H_

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <wowfiles/Chunk.h>
#include <wowfiles/ChunkHeaders.h>
#include <wowfiles/Mcal.h>
#include <wowfiles/lichking/McnrLk.h>
#include <wowfiles/WowChunkedFormat.h>
#include <utilities/Utilities.h>
#include <wowfiles/Mcnk.h>

class McnkLk : public Mcnk
{
  public:

    McnkLk(std::ifstream & file, int offsetInFile, const int headerSize);
	McnkLk(const std::vector<char> & wholeFile, int offsetInFile, const int & headerSize);
    McnkLk(std::string letters, int givenSize, const std::vector<char> &data);
    McnkLk(const McnkHeader & cMcnkHeader
      , const Chunk & cMcvt
      , const Chunk & cMccv
      , const McnrLk & cMcnr
      , const Chunk & cMcly
      , const Chunk & cMcrf
      , const Chunk & cMcsh
      , const Mcal & cMcal
      , const Chunk & cMclq
      , const Chunk & cMcse
      );

    void toFile(std::ofstream & adtFile, std::string & adtFileName);
    void toFile();
    std::vector<char> getWholeChunk() const;
	
    friend std::ostream & operator<<(std::ostream & os, const McnkLk & mcnkLk);

  //private:

    void getHeaderFromFile(std::ifstream & adtFile, const int position, const int length); // TODO : get rid of this when possible

    McnkHeader mcnkHeader;
    Chunk mcvt;
    Chunk mccv;
    McnrLk mcnr;
    Chunk mcly;
    Chunk mcrf;
    Chunk mcsh;
    Mcal mcal;
    Chunk mclq;
    Chunk mcse;
};

#endif
