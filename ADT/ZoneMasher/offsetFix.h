#ifndef __OFFSET_FIX
#define __OFFSET_FIX

#include <fstream>

typedef unsigned long UInt32;

struct Offsets{ //A tidy little struct that contains offsets
    int x, y;
    float xf, zf;
    float zOff;
    float wdtXOff, wdtYOff, wdtZOff;
};

struct MCIN{
    UInt32 offset, size, temp1, temp2;
};

struct OffsetFixData{
    Offsets offset;
    UInt32 offsetMDDF, offsetMODF;
    UInt32 positionsMCNK[256];
    MCIN positions[256];
};

void findMCNKs      (std::fstream&, OffsetFixData&);
void findMDDFandMODF(std::fstream&, OffsetFixData&);
void fixMCNKs       (std::fstream&, OffsetFixData&);
void fixDoodads     (std::fstream&, OffsetFixData&);
void fixWMOs        (std::fstream&, OffsetFixData&);

#endif
