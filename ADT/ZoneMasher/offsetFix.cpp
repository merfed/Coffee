#include <iomanip>
#include <fstream>
#include <iostream>
#include "offsetFix.h"

void findMCNKs      (std::fstream& zoneFile, OffsetFixData& offData){
    zoneFile.seekg(92, std::ios::beg);
    zoneFile.read((char*)(offData.positions), sizeof(MCIN)*256);
}

void findMDDFandMODF(std::fstream& zoneFile, OffsetFixData& offData){
    zoneFile.seekg(0x30, std::ios::beg);
    zoneFile.read((char*)(&(offData.offsetMDDF)), sizeof(UInt32)*1);
    zoneFile.seekg(0x30+4, std::ios::beg);
    zoneFile.read((char*)(&(offData.offsetMODF)), sizeof(UInt32)*1);
}

void fixMCNKs       (std::fstream& zoneFile, OffsetFixData& offData){
    float x, y;
    float oldX, oldY;
    zoneFile.seekg(offData.positions[0].offset+0x68+8,std::ios::beg);
    zoneFile.read((char*)(&oldY), sizeof(float)*1);
    zoneFile.seekg(offData.positions[0].offset+0x68+12,std::ios::beg);
    zoneFile.read((char*)(&oldX), sizeof(float)*1);

    y = (1600.0*(32-offData.offset.y))/3.0;
    x = (1600.0*(32-offData.offset.x))/3.0;

    offData.offset.xf = oldX - x;
    offData.offset.zf = oldY - y;

    for(int i=0; i < 256; i++)
    {
        zoneFile.seekg(offData.positions[i].offset+0x60+8, std::ios::beg);
        zoneFile.read((char*)(&(offData.positionsMCNK[i])), sizeof(UInt32)*1);
        offData.positionsMCNK[i] = offData.positionsMCNK[i] + offData.positions[i].offset;
        zoneFile.seekp(offData.positions[i].offset+0x68+8, std::ios::beg);
        y = (1600.0*(32-offData.offset.y))/3.0-100.0*(i/16)/3.0;
        x = (1600.0*(32-offData.offset.x))/3.0-100.0*(i%16)/3.0;
        zoneFile.write((char*)&y, sizeof(float)*1);
        zoneFile.seekp(offData.positions[i].offset+0x68+8+4, std::ios::beg);
        zoneFile.write((char*)&x, sizeof(float)*1);
        zoneFile.seekg(offData.positions[i].offset+0x68+8+8, std::ios::beg);
        zoneFile.read ((char*)&y, sizeof(float)*1);
        y += offData.offset.zOff;
        zoneFile.seekp(offData.positions[i].offset+0x68+8+8, std::ios::beg);
        zoneFile.write((char*)&y, sizeof(float)*1);
    }
}

void fixDoodads     (std::fstream& zoneFile, OffsetFixData& offData){
    int numDoodads;
    float temp[3];

    zoneFile.seekg(0x14+0x04+offData.offsetMDDF, std::ios::beg);
    zoneFile.read((char*)&numDoodads, sizeof(UInt32)*1);

    numDoodads/=36;

    zoneFile.seekg(0x08, std::ios::cur);
    for(int i=0; i < numDoodads; i++)
    {
        zoneFile.read((char*)temp, 4*3);
        temp[0] += offData.offset.xf;
        temp[2] += offData.offset.zf;
        zoneFile.seekp(-12, std::ios::cur);
        zoneFile.write((char*)temp, 4*3);
        zoneFile.seekg(24, std::ios::cur);
    }
}

void fixWMOs (std::fstream& zoneFile, OffsetFixData& offData){
    int numWMOs;
    float temp[3];

    zoneFile.seekg(0x14+0x04+offData.offsetMODF, std::ios::beg);
    zoneFile.read ((char*)&numWMOs, sizeof(UInt32)*1);

    numWMOs/=64;

    zoneFile.seekg(0x14+0x04+offData.offsetMODF+0x08+4, std::ios::beg);
    for(int i=0; i < numWMOs; i++)
    {
        zoneFile.read((char*)temp, 4*3);
        temp[0] += offData.offset.xf+offData.offset.wdtXOff;
        temp[1] += offData.offset.zOff+offData.offset.wdtZOff;
        temp[2] += offData.offset.zf+offData.offset.wdtYOff;
        zoneFile.seekp(0x14+0x04+offData.offsetMODF+0x08+4+i*64, std::ios::beg);
        zoneFile.write((char*)temp, 4*3);

        zoneFile.seekg(0x14+0x04+offData.offsetMODF+0x08+4+i*64+24, std::ios::beg);

        zoneFile.read((char*)temp, 4*3);
        temp[0] += offData.offset.xf+offData.offset.wdtXOff;
        temp[1] += offData.offset.zOff+offData.offset.wdtZOff;
        temp[2] += offData.offset.zf+offData.offset.wdtYOff;
        zoneFile.seekp(0x14+0x04+offData.offsetMODF+0x08+4+i*64+24, std::ios::beg);
        zoneFile.write((char*)temp, 4*3);

        zoneFile.seekg(0x14+0x04+offData.offsetMODF+0x08+4+i*64+36, std::ios::beg);

        zoneFile.read((char*)temp, 4*3);
        temp[0] += offData.offset.xf+offData.offset.wdtXOff;
        temp[1] += offData.offset.zOff+offData.offset.wdtZOff;
        temp[2] += offData.offset.zf+offData.offset.wdtYOff;
        zoneFile.seekp(0x14+0x04+offData.offsetMODF+0x08+4+i*64+36, std::ios::beg);
        zoneFile.write((char*)temp, 4*3);

        zoneFile.seekg(0x14+0x04+offData.offsetMODF+0x08+4+i*64+64, std::ios::beg);
    }
}
