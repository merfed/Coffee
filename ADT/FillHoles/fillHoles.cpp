#include <iostream>
using std::cout;
using std::ios;

#include <fstream>
using std::fstream;

#include <string>
using std::string;

#include <vector>
using std::vector;

typedef unsigned int UINT32;
typedef int INT32;
typedef unsigned short UINT16;
typedef short INT16;

struct SMAreaHeader {
	 /*000h*/  UINT32 pad;
	 /*004h*/  UINT32 offsInfo;		
	 /*008h*/  UINT32 offsTex;		
	 /*00Ch*/  UINT32 offsModels;		
	 /*010h*/  UINT32 offsModelsIds;		
	 /*014h*/  UINT32 offsMapObejcts;		
	 /*018h*/  UINT32 offsMapObejctsIds;		
	 /*01Ch*/  UINT32 offsDoodsDef;		
	 /*020h*/  UINT32 offsObjectsDef;	
	 /*024h*/  UINT32 pad1;	
	 /*028h*/  UINT32 pad2;		
	 /*02Ch*/  UINT32 pad3;	
	 /*030h*/  UINT32 pad4;		
	 /*034h*/  UINT32 pad5;		
	 /*038h*/  UINT32 pad6;		
	 /*03Ch*/  UINT32 pad7;	
	 /*040h*/
};
struct SMChunkInfo {
 /*000h*/  UINT32 offset;
 /*004h*/  UINT32 size;		
 /*008h*/  UINT32 flags;		
 /*00Ch*/  UINT32 asyncId;;		
 /*010h*/  		
};
struct SMChunk // 03-29-2005 By ObscuR
{
/*000h*/  UINT32 flags;
/*004h*/  UINT32 IndexX;
/*008h*/  UINT32 IndexY;
/*00Ch*/  UINT32 nLayers;
/*010h*/  UINT32 nDoodadRefs;
/*014h*/  UINT32 offsHeight;
/*018h*/  UINT32 offsNormal;
/*01Ch*/  UINT32 offsLayer;
/*020h*/  UINT32 offsRefs;
/*024h*/  UINT32 offsAlpha;
/*028h*/  UINT32 sizeAlpha;
/*02Ch*/  UINT32 offsShadow;
/*030h*/  UINT32 sizeShadow;
/*034h*/  UINT32 areaid;
/*038h*/  UINT32 nMapObjRefs;
/*03Ch*/  UINT32 holes;
/*040h*/  UINT16 unk1;
/*040h*/  UINT16 unk2;
/*044h*/  UINT32 unk3;
/*048h*/  UINT32 unk4;
/*04Ch*/  UINT32 unk5;
/*050h*/  UINT32 predTex;
/*054h*/  UINT32 noEffectDoodad;
/*058h*/  UINT32 offsSndEmitters;
/*05Ch*/  UINT32 nSndEmitters;
/*060h*/  UINT32 offsLiquid;
/*064h*/  UINT32 sizeLiquid;
/*068h*/  float  pos[3];
/*06Ch*/  
/*070h*/ 
/*074h*/  UINT32 textureId;
/*078h*/  UINT32 props;
/*07Ch*/  UINT32 effectId;
/*080h*/
};

const short OFFSET = 0x14;
const short BUFFER = 8;

SMAreaHeader header;
SMChunkInfo MCINHeaders[256];
SMChunk MCNKChunks[256];

fstream adtFile;

int main(int argc, char **argv) {
	if(argc == 1)
		return 0;

	adtFile.open(argv[1], ios::in | ios::out | ios::binary);

	adtFile.seekg(OFFSET);
	adtFile.read(reinterpret_cast<char *>(&header), sizeof(SMAreaHeader));

	adtFile.seekg(header.offsInfo + OFFSET + BUFFER);

	for(int i = 0; i < 256; i++)
		adtFile.read(reinterpret_cast<char *>(&(MCINHeaders[i])), sizeof(SMChunkInfo));

	for(int i = 0; i < 256; i++) {
		adtFile.seekg(MCINHeaders[i].offset + 8);
		adtFile.read(reinterpret_cast<char *>(&(MCNKChunks[i])), sizeof(SMChunk));
		if(MCNKChunks[i].holes != 0)
			MCNKChunks[i].holes = 0;

		adtFile.seekp(MCINHeaders[i].offset + 8);
		adtFile.write(reinterpret_cast<char *>(&(MCNKChunks[i])), sizeof(SMChunk));
	}
}
