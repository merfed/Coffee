#include <iostream>
using std::cout;
using std::ios;

#include <fstream>
using std::ifstream;
using std::ofstream;

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


ifstream adtFile;
ofstream outFile;

SMAreaHeader header;
SMChunkInfo MCINHeader;
vector<string> mdxNames;
vector<string> wmoNames;

void loadMDXNames() {
	adtFile.seekg(header.offsModels + 0x14 + 8);

	string currentModelname;

	outFile << "***************************************\n"
			<< "MMDX\n"
			<< "***************************************";

	char c;
	while((unsigned)(adtFile.tellg()) != header.offsModelsIds + 0x14) {
		adtFile.read(&c, 1);

		if(c != 0)
			currentModelname += c;
		else {
			outFile << "\n\n" << mdxNames.size() << '\t' << currentModelname;
			mdxNames.push_back(currentModelname);
			currentModelname.clear();
		}
	}
}
void loadWMONames() {
	adtFile.seekg(header.offsMapObejcts + 0x14 + 8);

	string currentWMOname;

	outFile << "\n\n\n***************************************\n"
			<< "MWMO\n"
			<< "***************************************";

	char c;
	while((unsigned)(adtFile.tellg()) != header.offsMapObejctsIds + 0x14) {
		adtFile.read(&c, 1);

		if(c != 0)
			currentWMOname += c;
		else {
			outFile << "\n\n" << wmoNames.size() << '\t' << currentWMOname;
			wmoNames.push_back(currentWMOname);
			currentWMOname.clear();
		}
	}
}
void printTexInfo() {
	adtFile.seekg(header.offsTex + 0x14 + 8);
	outFile << "\n\n\n***************************************\n"
			<< "MTEX\n"
			<< "***************************************";

	string currentTexname;

	char c;
	while((unsigned)(adtFile.tellg()) != header.offsModels + 0x14) {
		adtFile.read(&c, 1);

		if(c != 0)
			currentTexname += c;
		else {
			outFile << "\n\n" << currentTexname;
			currentTexname.clear();
		}
	}
}
void printMDXInfo() {
	adtFile.seekg(header.offsDoodsDef + 0x14 + 8);
	SMDoodadDef doodad;

	outFile << "\n\n\n***************************************\n"
			<< "MDDF\n"
			<< "***************************************\n";

	while((unsigned)(adtFile.tellg()) != header.offsObjectsDef + 0x14) {
		adtFile.read(reinterpret_cast<char *>(&doodad), sizeof(SMDoodadDef));

		outFile << "\nModelID: " << mdxNames.at(doodad.nameId) << "(" << doodad.nameId << ")"
			<<  "\n\nUniqueID: " << doodad.uniqueId
			<<  "\nPosition: "
			<<  "\n     X: " << doodad.pos[0]
			<<  "\n     Y: " << doodad.pos[1]
			<<  "\n     Z: " << doodad.pos[2]
			<<  "\nRotation: "
			<<  "\n     X: " << doodad.rot[0]
			<<  "\n     Y: " << doodad.rot[1]
			<<  "\n     Z: " << doodad.rot[2]
			<<  "\nFlags: " << doodad.flags
			<<  "\nScale: " << doodad.scale
			<<  "\n\n";
	}
}
void printWMOInfo() {
	adtFile.seekg(header.offsObjectsDef + 0x14 + 8);
	SMMapObjDef wmo;

	outFile << "\n\n\n***************************************\n"
			<< "MODF\n"
			<< "***************************************\n";

	while((unsigned)(adtFile.tellg()) != MCINHeader.offset) {
		adtFile.read(reinterpret_cast<char *>(&wmo), sizeof(SMMapObjDef));

		outFile << "\nWMOID: " << wmoNames.at(wmo.nameId) << "(" << wmo.nameId << ")"
			<<  "\n\nUniqueID: " << wmo.uniqueId
			<<  "\nPosition: "
			<<  "\n     X: " << wmo.pos[0]
			<<  "\n     Y: " << wmo.pos[1]
			<<  "\n     Z: " << wmo.pos[2]
			<<  "\nRotation: "
			<<  "\n     X: " << wmo.rot[0]
			<<  "\n     Y: " << wmo.rot[1]
			<<  "\n     Z: " << wmo.rot[2]
			<<  "\nFlags: " << wmo.flags
			<< "\nDoodadSet: " << wmo.doodadSet
			<< "\nNameSet: " << wmo.nameSet
			<< "\n\n";
	}
}

int main(int argc, char **argv) {
	if(argc == 1)
		return 0;

	adtFile.open(argv[1], ios::in | ios::binary);

	char *replace;
	replace = strstr(argv[1], "adt");
	replace[0] = 't';
	replace[1] = 'x';
	replace[2] = 't';

	outFile.open(argv[1], ios::out);

	// position at start of header chunk
	adtFile.seekg(0x14);
	adtFile.read(reinterpret_cast<char *>(&header), sizeof(SMAreaHeader));

	adtFile.seekg(header.offsInfo + 0x14 + 8);
	adtFile.read(reinterpret_cast<char*>(&MCINHeader), sizeof(SMChunkInfo));

	loadMDXNames();
	loadWMONames();
	printTexInfo();
	printMDXInfo();
	printWMOInfo();
}