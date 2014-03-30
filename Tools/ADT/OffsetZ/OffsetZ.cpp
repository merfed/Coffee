#include <iostream>
using std::cout;
using std::ios;

#include <fstream>
using std::fstream;

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

struct SMDoodadDef {
/*000h*/  UINT32 nameId;
/*004h*/  UINT32 uniqueId;
/*008h*/  float pos[3];
/*00Ch*/
/*010h*/
/*014h*/  float rot[3];
/*018h*/
/*01Ch*/
/*020h*/  UINT16 scale;
/*022h*/  UINT16 flags;
/*024h*/
};
struct SMMapObjDef	{
/*000h*/  UINT32 nameId;
/*004h*/  UINT32 uniqueId;
/*008h*/  float pos[3];
/*00Ch*/
/*010h*/
/*014h*/  float rot[3];
/*018h*/
/*01Ch*/
/*020h*/  float extents[6];
/*024h*/
/*028h*/
/*02Ch*/
/*030h*/
/*034h*/
/*038h*/  UINT32 flags;
/*03Ch*/  UINT16 doodadSet;
/*03Eh*/  UINT16 nameSet;
/*040h*/
};

#define OFFSET = 0x14;
#define BUFFER = 8;
#define Z_CHANGE = 10;

fstream adtFile;

SMAreaHeader header;
SMChunkInfo MCINHeader;

void offsetMDX()
{
	adtFile.seekg(header.offsDoodsDef + OFFSET + BUFFER);
	SMDoodadDef doodad;

	while((unsigned)(adtFile.tellg()) != header.offsObjectsDef + OFFSET) {
		unsigned newPos = (unsigned)(adtFile.tellg());
		adtFile.read(reinterpret_cast<char *>(&doodad), sizeof(SMDoodadDef));

		doodad.pos[1] += Z_CHANGE;

		adtFile.seekp(newPos);
		adtFile.write(reinterpret_cast<char *>(&doodad), sizeof(SMDoodadDef));
		adtFile.seekg(adtFile.tellp());
	}
}
void offsetWMO() {
	adtFile.seekg(header.offsObjectsDef + OFFSET + BUFFER);
	SMMapObjDef wmo;

	while((unsigned)(adtFile.tellg()) != MCINHeader.offset) {
		unsigned newPos = (unsigned)(adtFile.tellg());
		adtFile.read(reinterpret_cast<char *>(&wmo), sizeof(SMMapObjDef));

		wmo.pos[1] += Z_CHANGE;

		adtFile.seekp(newPos);
		adtFile.write(reinterpret_cast<char *>(&wmo), sizeof(SMMapObjDef));
		adtFile.seekg(adtFile.tellp());
	}
}

int main(int argc, char **argv) {
	if(argc == 1)
		cout << "<offsetZ> <adt files>...\n";

	for(int i = 1; i < argc; i++) {
		adtFile.open(argv[i], ios::in | ios::out | ios::binary);

		// position at start of header chunk
		adtFile.seekg(OFFSET);
		adtFile.read(reinterpret_cast<char *>(&header), sizeof(SMAreaHeader));
		adtFile.seekg(header.offsInfo + OFFSET + BUFFER);
		adtFile.read(reinterpret_cast<char *>(&MCINHeader), sizeof(SMChunkInfo));

		offsetMDX();
		offsetWMO();

		adtFile.close();
	}
}
