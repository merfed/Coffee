#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

struct MCIN
{
	unsigned int Offset;
	unsigned int Size;
	unsigned int Temp1;
	unsigned int Temp2;
};

struct MCVT
{
	float row0[9];
	float null0[8];
	float row1[9];
	float null1[8];
	float row2[9];
	float null2[8];
	float row3[9];
	float null3[8];
	float row4[9];
	float null4[8];
	float row5[9];
	float null5[8];
	float row6[9];
	float null6[8];
	float row7[9];
	float null7[8];
	float row8[9];
};

MCIN *Positions;
MCVT * HeightMap;
float * basez;

FILE *Input;
char *File;

float fabs ( float a )
{
	float temp = a * a;
	temp = sqrt( a );
	return temp;      
}

float getHeightValue2(int i)
{
	if (i<8) return *basez + HeightMap->null0[i%8];
	else if(i<16) return *basez + HeightMap->null1[i%8];
	else if(i<24) return *basez + HeightMap->null2[i%8];
	else if(i<32) return *basez + HeightMap->null3[i%8];
	else if(i<40) return *basez + HeightMap->null4[i%8];
	else if(i<48) return *basez + HeightMap->null5[i%8];
	else if(i<56) return *basez + HeightMap->null6[i%8];
	else if(i<64) return *basez + HeightMap->null7[i%8];

	return *basez;
}

float getHeightValue(int i)
{
	if (i<9) return *basez + HeightMap->row0[i%9];
	else if(i<18) return *basez + HeightMap->row1[i%9];
	else if(i<27) return *basez + HeightMap->row2[i%9];
	else if(i<36) return *basez + HeightMap->row3[i%9];
	else if(i<45) return *basez + HeightMap->row4[i%9];
	else if(i<54) return *basez + HeightMap->row5[i%9];
	else if(i<63) return *basez + HeightMap->row6[i%9];
	else if(i<72) return *basez + HeightMap->row7[i%9];
	else if(i<81) return *basez + HeightMap->row8[i%9];

	return *basez;
}

float valueormax(float value, float max)
{
	if(value < max) return value;
	else return max;
}


unsigned int FileSize;

int main(int argc, char **argv)
{
	int depth = 1;
	
	unsigned int *TInt;
	unsigned char *TChar;
	unsigned short *TShort;

	if( argc < 2 )
	{
		printf( "AllOcean3.0 <map filename> [<depthfactor>]\n" );
		return 0;
	}
	
	if( argc == 3 )
	depth = atoi(argv[2]);

	Input = fopen( argv[1], "rb+" );
	fseek( Input, 0, SEEK_END );
	FileSize = ftell( Input );
	File = new char[ FileSize + 0xC00 + 0x81 * 256 + 0x8 ];           // And new chunk.
	fseek( Input, 0, SEEK_SET );
	fread( File, 1, FileSize, Input );
	fclose( Input );
	
	Positions = (MCIN *)( File + 0x5C );
	
	// header content is at 0x14, water is at header+0x28
	TInt = (unsigned int *)( File + 0x14 + 0x28 );
	*TInt = FileSize - 0x14;
	
	// write chunk:
	TChar = (unsigned char *)( File + FileSize ); // magix
	*TChar = 'O'; TChar++; *TChar = '2'; TChar++;
	*TChar = 'H'; TChar++; *TChar = 'M';
	
	TInt = (unsigned int *)( File + FileSize + 0x4 ); // size
	*TInt = 0xC00 + 0x81 * 256;
	

	for( int i = 0; i < 256; i++ )
	{		
		// delete MCLQ
		TInt = (unsigned int *)( File + Positions[i].Offset + 0x08 );
		*TInt = *TInt & ~(28); // b11100 = x1C = 28
        TInt = (unsigned int *)( File + Positions[i].Offset + 0x68 );
		*TInt = 0;
		TInt = (unsigned int *)( File + Positions[i].Offset + 0x6C );
		*TInt = 0;
		
		int address = 0xC00 + i * 0x81;
		
		// the info - array
		
		TInt = (unsigned int *)( File + FileSize + 0x8 + i*0xC );
		/* 0x000 */ *TInt = address; TInt++;
		/* 0x004 */ *TInt = 1; TInt++;
		/* 0x008 */ *TInt = address + 0x18;
		
		// the data
		
		TShort = (unsigned short *)( File + FileSize + 0x8 + address );
		/* 0xC00 */ *TShort = 2; TShort++; 
		/* 0xC02 */ *TShort = 2;
		TInt = (unsigned int *)( File + FileSize + 0x8 + address + 0x4 );
		/* 0xC04 */ *TInt = 0; TInt++; 
		/* 0xC08 */ *TInt = 0; TInt++;
		TChar = (unsigned char *)( File + FileSize + 0x8 + address + 0xC );
		/* 0xC0C */ *TChar = 0; TChar++; *TChar = 0; TChar++;
		/* 0xC0E */ *TChar = 8; TChar++; *TChar = 8;
		TInt = (unsigned int *)( File + FileSize + 0x8 + address + 0x10 );
		/* 0xC10 */ *TInt = address + 0x28; TInt++;
		/* 0xC14 */ *TInt = address + 0x30; TInt++;
		/* 0xC18 */ *TInt = 0; TInt++;
		/* 0xC1C */ *TInt = 0; TInt++;
		/* 0xC20 */ *TInt = 0; TInt++;
		/* 0xC24 */ *TInt = 0; TInt++;
		/* 0xC28 */ *TInt = 0; TInt++;
		/* 0xC2C */ *TInt = 0;
		
		// and the information we gain from seeing the heightmap.

		HeightMap = (MCVT *)( File + Positions[i].Offset + 0x8 + 128 + 0x8 );
		basez = (float*)( File + Positions[i].Offset + 0x78 );
		
		TChar = (unsigned char *)( File + FileSize + 0x8 + address + 0x30 );
		unsigned char * mask_byte = (unsigned char*)( File + FileSize + 0x8 + address + 0x18 );
		unsigned char * mask_byte2 = (unsigned char*)( File + FileSize + 0x8 + address + 0x28 );
		
		for( int j = 0; j < 9*9; j++)
		{
			int baselevel = 0;
			float trans = baselevel - getHeightValue(j);
			
			if( trans < 0 ) trans = 0;
			else if(trans > 255) trans = 255;
			
			/* 0xC30 + j */ *TChar = (unsigned char)trans;
			TChar++;     
		}
		
		for( int j = 0; j < 8; j++)
		{
			for( int k = 0; k < 8; k++)
			{
				int baselevel = 0;
				float trans = baselevel - getHeightValue2(j*8+k);
				
			//	*   *   // j*8 + k, j*8 + 1 + k 
		    //    +     // j*8 + k
			//	*   *   // (j+1)*8 + k, (j+1)*8 + 1 + k
                

				if((baselevel - getHeightValue2(j*8+k)) >= baselevel 
                 || (baselevel - getHeightValue(j*8+k)) >= baselevel
                 || (baselevel - getHeightValue(j*8+k+1)) >= baselevel
                 || (baselevel - getHeightValue((j+1)*8+k)) >= baselevel
                 || (baselevel - getHeightValue((j+1)*8+k+1)) >= baselevel  )	
				{		
					*mask_byte = *mask_byte | ( 1 << (k % 8) );
					*mask_byte2 = *mask_byte2 | ( 1 << (k % 8) );
				}
			}
			mask_byte++;
			mask_byte2++;
		}
	}


	Input = fopen( argv[1], "wb" );
	fwrite( File, 1, FileSize + 0xC00 + 0x81 * 256 + 0x8, Input );
	fclose( Input );

	delete File;
}
