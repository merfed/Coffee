#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <wowfiles/Wdt.h>
#include <wowfiles/Wdl.h>
#include <wowfiles/alpha/WdtAlpha.h>
#include <wowfiles/lichking/AdtLk.h>
#include <wowfiles/cataclysm/AdtCata.h>
#include <wowfiles/cataclysm/AdtCataTextures.h>
#include <wowfiles/cataclysm/AdtCataObjects.h>
#include <utilities/Utilities.h>
#include "wowfiles/ChunkHeaders.h"

bool mBigAlpha;

int get_uint2 (char* array, size_t index)
{
    const size_t array_index (index / 4);
    const size_t bit_index (6 - (index % 4) * 2);

    const char byte (array[array_index]);
    const char bit_shifted (byte >> bit_index);
    return (bit_shifted & 3);
}
void set_uint2 (char* array, size_t index, unsigned int value)
{
    const size_t array_index (index / 4);
    const size_t bit_index (6 - (index % 4) * 2);

    array[array_index] &= ~(3 << bit_index);
    array[array_index] |= ((value & 3) << bit_index);
}

void getAlphamap(McnkLk *mcnk, unsigned char *amap, int layer)
{
    mclyEntry mcly = Utilities::get<mclyEntry>(mcnk->mcly.data,layer*16U);
    memset(amap,0,64*64);
    if(!mBigAlpha)
        mBigAlpha = ( (mcnk->mcly.getRealSize()/16U) > layer+1 ? (mcly.ofsAlpha - mcly.ofsAlpha) : (mcnk->mcal.data.size() - mcly.ofsAlpha)) == 64*64;

    if( mcly.flags & 0x100 )
    {
        if( mcly.flags & 0x200 )
        {  // compressed

            // 21-10-2008 by Flow
            unsigned offI = 0; //offset IN buffer
            unsigned offO = 0; //offset OUT buffer
            char* buffIn = &mcnk->mcal.data[mcly.ofsAlpha]; // pointer to data in adt file

            while( offO < 4096 )
            {
                // fill or copy mode
                bool fill = buffIn[offI] & 0x80;
                unsigned n = buffIn[offI] & 0x7F;
                offI++;
                for( unsigned k = 0; k < n; ++k )
                {
                    if (offO == 4096) break;
                    amap[offO] = buffIn[offI];
                    offO++;
                    if( !fill )
                        offI++;
                }
                if( fill ) offI++;
            }
        }
        else if(mBigAlpha){
            // not compressed
            unsigned char *p = amap;
            char *abuf = &mcnk->mcal.data[mcly.ofsAlpha];
            for (int j=0; j<64; ++j) {
                for (int i=0; i<64; ++i) {
                    *p++ = *abuf++;
                }

            }

            memcpy(amap+63*64,amap+62*64,64);
        }
        else
        {

            // not compressed
            char *abuf = &mcnk->mcal.data[mcly.ofsAlpha];
            unsigned char *p = amap;
            for (int j=0; j<64; ++j) {
                for (int i=0; i<32; ++i) {
                    unsigned char c = *abuf++;
                    *p++ = static_cast<unsigned char>((255*(static_cast<int>(c & 0x0f)))/0x0f);
                    if(i != 31)
                        *p++ = static_cast<unsigned char>((255*(static_cast<int>(c & 0xf0)))/0xf0);

                    else
                        *p++ = static_cast<unsigned char>((255*(static_cast<int>(c & 0x0f)))/0x0f);
                }

            }
            memcpy(amap+63*64,amap+62*64,64);
        }
    }
}



int main (int argc, char **argv)
{
    std::vector<char> adtFile(0);
    std::string adtName (argv[1]);

    std::cout << "Loading: " << adtName << std::endl;

    Utilities::getWholeFile(adtName, adtFile);

    AdtLk *test = new AdtLk(adtFile, adtName);

    unsigned int realyLow[8*8];
    unsigned int alphaMapAverage[3][8*8];
    unsigned int sum;



    for(int mcnkIndex = 0; mcnkIndex < 256; ++mcnkIndex)
    {
        McnkLk *mcnk = &test->mcnks.at(mcnkIndex);
        size_t nTextures = mcnk->mcly.getGivenSize() / 16U;
        printf ("%i Befor:\n", mcnkIndex);
        for (int j (0); j < 8; ++j)
        {
            for (int i (0); i < 8; ++i)
            {
                printf ("%i ", get_uint2(mcnk->mcnkHeader.groundEffectsMap, j * 8 + i));
            }
            printf ("\n");
        }

        for( int layer = 0; layer < nTextures; layer++ )
        {
            unsigned char amap[64*64];

            getAlphamap(mcnk,amap,layer);

            for (int i=0; i<64; ++i)
            {
                for (int cx=0; cx<8; ++cx)
                    for (int cy=0; cy<8; ++cy)
                        sum += amap[64*i+(cx*8+cy)];
                alphaMapAverage[layer][i] = sum/64;
                sum = 0;

            }

            for( int i = 0; i < 64; i++ )
                if(alphaMapAverage[layer][i] > 128)
                    realyLow[i] = layer;
                else if(layer == 0)
                    realyLow[i] = 0; //just to ensure

        }

        memset (mcnk->mcnkHeader.groundEffectsMap, 0, sizeof (mcnk->mcnkHeader.groundEffectsMap));

        for (size_t j (0); j < 8 * 8; ++j)
            set_uint2 (mcnk->mcnkHeader.groundEffectsMap, j, realyLow[j]);

        printf ("%i After:\n",mcnkIndex);
        for (int j (0); j < 8; ++j)
        {
            for (int i (0); i < 8; ++i)
            {
                printf ("%i ", get_uint2(mcnk->mcnkHeader.groundEffectsMap, j * 8 + i));
            }
            printf ("\n");
        }

    }

    test->toFile();

    return 0;
}
