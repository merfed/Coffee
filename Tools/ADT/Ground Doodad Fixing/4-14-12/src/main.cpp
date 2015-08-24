#include <iostream>
#include <string>
#include <cstring>
#include <vector>

#include <wowfiles/ChunkHeaders.h>
#include <wowfiles/lichking/AdtLk.h>
#include <utilities/Utilities.h>

void get_alphamap (const McnkLk *mcnk, unsigned char *amap, int layer)
{
  const mclyEntry mcly (Utilities::get<mclyEntry> (mcnk->mcly.data, layer * 0x10));
  memset (amap, 0, 64*64);

  const bool mBigAlpha ( ( (mcnk->mcly.getRealSize()/16U) > layer+1
                         ? (mcly.ofsAlpha - mcly.ofsAlpha)
                         : (mcnk->mcal.data.size() - mcly.ofsAlpha)
                         ) == 64*64
                       );

  if (mcly.flags & 0x100)
  {
    const char* abuf (&mcnk->mcal.data[mcly.ofsAlpha]);

    if (mcly.flags & 0x200)
    {
      // 21-10-2008 by Flow
      size_t offI (0); //offset IN buffer
      size_t offO (0); //offset OUT buffer

      while (offO < 4096)
      {
        // fill or copy mode
        bool fill (abuf[offI] & 0x80);
        size_t n (abuf[offI] & 0x7F);
        ++offI;
        for (size_t k = 0; k < n; ++k)
        {
          if (offO == 4096) break;
          amap[offO] = abuf[offI];
          ++offO;
          if( !fill )
            ++offI;
        }
        if( fill ) ++offI;
      }
    }
    else if (mBigAlpha)
    {
      unsigned char *p (amap);
      for (int j (0); j < 64; ++j)
      {
        for (int i (0); i < 64; ++i)
        {
          *p++ = *abuf++;
        }
      }
      memcpy (amap + 63 * 64, amap + 62 * 64, 64);
    }
    else
    {
      unsigned char *p (amap);
      for (int j (0); j < 64; ++j)
      {
        for (int i (0); i < 32; ++i)
        {
          unsigned char c (*abuf++);
          *p++ = static_cast<unsigned char>((255*(static_cast<int>(c & 0x0f)))/0x0f);
          if(i != 31)
            *p++ = static_cast<unsigned char>((255*(static_cast<int>(c & 0xf0)))/0xf0);

          else
            *p++ = static_cast<unsigned char>((255*(static_cast<int>(c & 0x0f)))/0x0f);
        }

      }
      memcpy (amap + 63 * 64, amap + 62 * 64, 64);
    }
  }
}

void print_low_quality_map (const char* data)
{
  for (int j (0); j < 8; ++j)
  {
    for (int i (0); i < 8; ++i)
    {
      const size_t array_index ((j * 8 + i) / 4);
      const size_t bit_index (((j * 8 + i) % 4) * 2); //6 - that ones goes 6 4 2 0 and I want it 0 2 4 6

      std::cout << ((data[array_index] >> bit_index) & 3) << " ";
    }
    std::cout << "\n";
  }
}

int main (int argc, char **argv)
{
  static const size_t minimum_value_to_overwrite (120);

  std::cout << "Current minimum_value_to_overwrite : " << minimum_value_to_overwrite << std::endl;

  const std::string adtName (argv[1]);
  std::cout << "Loading: " << adtName << std::endl;

  std::vector<char> adtFile(0);
  Utilities::getWholeFile (adtName, adtFile);

  AdtLk* test (new AdtLk (adtFile, adtName));

  for (size_t mcnkIndex (0); mcnkIndex < 256; ++mcnkIndex)
  {
    //! \note This seems really wrong. Taking the reference of a
    //! return value requires the value to be on this stack page and
    //! is equal to copying the return value onto the stack and then
    //! taking a pointer to it. Or will result in undefined behavior.
    McnkLk* mcnk (&test->mcnks.at(mcnkIndex));
    const size_t nTextures (mcnk->mcly.getGivenSize() / 0x10);

    printf ("%i before:\n", mcnkIndex);
    print_low_quality_map (mcnk->mcnkHeader.groundEffectsMap);

    memset ( mcnk->mcnkHeader.groundEffectsMap
           , 0
           , sizeof (mcnk->mcnkHeader.groundEffectsMap)
           );

    for (size_t layer (1); layer < nTextures; ++layer)
    {
      unsigned char amap[64*64];

      get_alphamap (mcnk, amap, layer);

      for (size_t y (0); y < 8; ++y)
      {
        for (size_t x (0); x < 8; ++x)
        {
          size_t sum (0);
          for (size_t j (0); j < 8; ++j)
          {
            for (size_t i (0); i < 8; ++i)
            {
              sum += amap[(y * 8 + j) * 64 + (x * 8 + i)];
            }
          }

          if (sum > minimum_value_to_overwrite * 8 * 8)
          {
            const size_t array_index ((y * 8 + x) / 4);
            const size_t bit_index (((y * 8 + x) % 4) * 2); // -6

            mcnk->mcnkHeader.groundEffectsMap[array_index]
              |= ((layer & 3) << bit_index);
          }
        }
      }
    }

    printf ("%i after:\n", mcnkIndex);
    print_low_quality_map (mcnk->mcnkHeader.groundEffectsMap);
  }

  test->toFile();

  return 0;
}