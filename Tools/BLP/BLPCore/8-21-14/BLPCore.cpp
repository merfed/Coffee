// Copyright (c) 2014 Norgannon
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
//  BLPCore.cpp
//  Blipper
//

#include "BLPCore.h"

namespace BLPCore {
    
    const int kBLPMagicSize   = sizeof(uint8_t)*4;
    
    bool CheckBLPMagic(DataChunk data)
    {
        if (data.empty()) {
            return false;
        }
        std::string magic(data.begin(), data.begin()+kBLPMagicSize);

        return magic == "BLP2";
    }
    
    bool DecodeBLPData(DataChunk file, DataChunk &decodedFile)
    {
        if (!CheckBLPMagic(file)) {
            return false;
        }
        
        BLPHeader header = GetBLPHeader(file);
        
        DataChunk pixels(file.begin()+header.mipmapOffsets[0],
                         file.begin()+header.mipmapOffsets[0]+header.mipmapLengths[0]);
        
        if (header.compressionType == BLPCompressionTypePalettized) {
            if (!DecodePalettizedBLP(pixels, header.width, header.height, header.alphaBits, header.colorPalette, decodedFile)) {
                return false;
            }
        } else if (header.compressionType == BLPCompressionTypeDXT) {
            if (!DecodeDXTBLP(pixels, header.width, header.height, header.alphaType, decodedFile)) {
                return false;
            }
        } else if (header.compressionType == BLPCompressionTypePlain) {
            decodedFile = ARGBToRGBA(pixels);
        }
        
        return true;
    }
    
    BLPHeader GetBLPHeader(DataChunk data)
    {
        return *(reinterpret_cast<BLPHeader*>(data.data()));
    }
    
    bool DecodeDXTBLP(DataChunk data, int width, int height, int alphaType, DataChunk &decodedData)
    {
        int dataSize = (width*height)*sizeof(RGBAPixel);
        squish::u8 squishData[dataSize];
        int dxtType;
        
        switch (alphaType) {
            case 0:
                dxtType = squish::kDxt1;
                break;
                
            case 1:
                dxtType = squish::kDxt3;
                break;
                
            case 7:
                dxtType = squish::kDxt5;
                break;
                
            default:
                std::cout << "Unknown DXT type, cannot continue decoding!" << std::endl;
                return false;
                break;
        }
        
        squish::DecompressImage(squishData, width, height, data.data(), dxtType);
        
        if (squishData) {
            DataChunk squishVector(squishData, squishData+dataSize);
            decodedData = squishVector;
            
            return true;
        }

        return false;
    }
    
    bool DecodePalettizedBLP(DataChunk data, int width, int height, int alphaBits, BGRAPixel *palette, DataChunk &decodedData)
    {
        DataChunk pixels;
        
        int currentpixel = 0;
        
        for (unsigned int y = 0; y < height; ++y)
        {
            for (unsigned int x = 0; x < width; ++x)
            {
                uint8_t paletteIndex = data[currentpixel];
                BGRAPixel pixel = palette[paletteIndex];
                
                if (alphaBits == 0) {
                    pixel.a = 255;
                } else if (alphaBits == 1) {
                    //TODO: Implement 1-bit alpha
                    pixel.a = 255;
                } else if (alphaBits == 4) {
                    //TODO: Implement 4-bit alpha
                    pixel.a = 255;
                } else if (alphaBits == 8) {
                    pixel.a = data[currentpixel + width * height];
                }
                
                ++currentpixel;
                
                pixels.push_back(pixel.r);
                pixels.push_back(pixel.g);
                pixels.push_back(pixel.b);
                pixels.push_back(pixel.a);
            }
        }
        
        decodedData = pixels;
        
        return true;
    }
    
    DataChunk EncodePlainBLP(DataChunk pixels, int width, int height)
    {
        DataChunk BLP;
        
        char magic[4]{'B', 'L', 'P', '2'};
        
        BLPHeader header = *new BLPHeader;
        strcpy(magic, header.magic);
        header.type = 1;
        header.compressionType = BLPCompressionTypePlain;
        header.alphaBits = 8;
        header.alphaType = 8;
        header.hasMips = 0;
        header.width = width;
        header.height = height;
        header.mipmapOffsets[0] = sizeof(header);
        header.mipmapLengths[0] = (uint32_t)pixels.size();
        
        for (int i = 1; i < 16; i++) {
            header.mipmapOffsets[i] = 0;
            header.mipmapLengths[i] = 0;
        }
        
        for (int i = 0; i < 256; i++) {
            BGRAPixel blank;
            blank.r = 0;
            blank.g = 0;
            blank.b = 0;
            blank.a = 0;
            
            header.colorPalette[i] = blank;
        }
        
        memcpy(&header, &BLP, sizeof(header));
        BLP.insert(BLP.end(), pixels.begin(), pixels.end());
        
        return BLP;
    }
    
    DataChunk ARGBToRGBA(DataChunk ARGBData)
    {
        for (int i = 0; i < ARGBData.size(); i += 4) {
            uint8_t a = ARGBData[i+0];
            uint8_t r = ARGBData[i+1];
            uint8_t g = ARGBData[i+2];
            uint8_t b = ARGBData[i+3];
            ARGBData[i+0] = r;
            ARGBData[i+1] = g;
            ARGBData[i+2] = b;
            ARGBData[i+3] = a;
        }
        
        return ARGBData;
    }
    
}