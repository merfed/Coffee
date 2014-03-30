#include <iostream>
using std::cout;
using std::cin;
using std::ios;
using std::ostream;
using std::showbase;
using std::noshowbase;
using std::hex;
using std::dec;

#include <fstream>
using std::fstream;

#include <windows.h>

struct WMOHeader {
	unsigned int	nTextures;
	unsigned int	nGroups;
	unsigned int	nPortals;
	unsigned int	nLights;
	unsigned int	nModels;
	unsigned int	nDoodads;
	unsigned int	nSets;
	float			ambColor[6];
	float			pos[2];
	unsigned int	wmoID;
};

struct SMOLight // 04-29-2005 By ObscuR
{
/*000h*/  UINT8 LightType; 	
/*001h*/  UINT8 type;
/*002h*/  UINT8 useAtten;
/*003h*/  UINT8 pad;
/*004h*/  UINT8 color[4];  
/*008h*/  float position[3];
/*014h*/  float intensity;
/*018h*/  float attenStart;
/*01Ch*/  float attenEnd;
/*020h*/  float unk1;
/*024h*/  float unk2;
/*028h*/  float unk3;
/*02Ch*/  float unk4;
/*030h*/  
};

struct SMODoodad // 03-29-2005 By ObscuR
{
/*000h*/  UINT32 nameIndex;
/*004h*/  float pos[3];
/*010h*/  float rot[4];
/*020h*/  float scale;
/*024h*/  UINT8 color[4];
/*028h*/
};

void changeLights();

fstream wmoFile;
// I could just use chars, couldn't I? >_<
UINT8 color[4];

bool isRoot = false;

int main(int argc, char **argv) {
	if(argc == 1) {
		cout << "WMOColorChanger <WMO Files...>\n";
		return 0;
	}
	char headerName[5];
	unsigned temp = -1;

	// color is written in files as BGRA
	while(temp < 0 || temp > 255) {
		cout << "Color value for red (0-255)? ";
		cin >> temp;
		cout << '\n';
	}
	color[2] = temp;

	temp = -1;
	while(temp < 0 || temp > 255) {
		cout << "Color value for green (0-255)? ";
		cin >> temp;
		cout << '\n';
	}
	color[1] = temp;

	temp = -1;
	while(temp < 0 || temp > 255) {
		cout << "Color value for blue (0-255)? ";
		cin >> temp;
		cout << '\n';
	}
	color[0] = temp;

	temp = -1;
	while(temp < 0 || temp > 255) {
		cout << "Alpha value (0-255)? ";
		cin >> temp;
		cout << '\n';
	}
	color[3] = temp;

	for(int i = 1; i < argc; i++) {
		wmoFile.open(argv[i], ios::in | ios::out | ios::binary);
		// Group or Root?
		wmoFile.seekg(0xC);

		wmoFile.read(headerName, 4);
		headerName[4] = '\0';

		if(!strcmp(headerName, "DHOM")) {
			cout << "Editing lights in a Root WMO...\n\n";
			isRoot = true;
		}
		else {
			cout << "Editing lights in a Group WMO...\n\n";
			isRoot = false;
		}

		changeLights();	

		wmoFile.close();
	}
}

void changeLights() {

	// read header from WMO file
	WMOHeader header;
	if(isRoot) {
		wmoFile.seekg(0x14);
		wmoFile.read(reinterpret_cast<char *>(&header), sizeof(WMOHeader));
	}

	char headerName[5];
	headerName[4] = '\0';

	unsigned offset;
	unsigned eof;

	wmoFile.seekg(0, ios::end);
	eof = wmoFile.tellg();

	wmoFile.seekg((isRoot ? 0x54 : 0x58));

	while((unsigned)(wmoFile.tellg()) != eof) {
		wmoFile.read(headerName, 4);
		cout << "Header " << headerName << " found\n";
		unsigned pos = wmoFile.tellg();

		if(!strcmp(headerName, "DDOM")) {
			unsigned size;
			SMODoodad model;
			cout << "\tWMO Model chunk!\n";

			wmoFile.read(reinterpret_cast<char *>(&size), sizeof(unsigned));
			size /= sizeof(SMODoodad);

			cout << "\t Found " << size << " models.\n";

			for(short i = 0; i < size; i++) {
				unsigned writePos = wmoFile.tellg();
				wmoFile.read(reinterpret_cast<char *>(&model), sizeof(SMODoodad));
				for(short j = 0; j < 4; j++)
					model.color[j] = color[j];
				wmoFile.seekp(writePos);
				wmoFile.write(reinterpret_cast<const char *>(&model), sizeof(SMODoodad));

				wmoFile.seekg(writePos + sizeof(SMODoodad));
			}
		}
		else if(!strcmp(headerName, "TLOM")) {
			SMOLight light;
			cout << "\tWMO light chunk!\n";
			cout << "\t Found " << header.nLights << " lights.\n";
			wmoFile.seekg(4, ios::cur);
			for(short i = 0; i < header.nLights; i++) {
				unsigned writePos = wmoFile.tellg();
				wmoFile.read(reinterpret_cast<char *>(&light), sizeof(SMOLight));
				for(short j = 0; j < 4; j++)
					light.color[j] = color[j];
				wmoFile.seekp(writePos);
				wmoFile.write(reinterpret_cast<const char *>(&light), sizeof(SMOLight));

				wmoFile.seekg(writePos + sizeof(SMOLight));
			}
		}
		else if(!strcmp(headerName, "VCOM")) {
			unsigned size;
			wmoFile.read(reinterpret_cast<char *>(&size), sizeof(unsigned));

			cout << "\tVertex color chunk!\n";
			cout << "\t Found " << size / 4 << " vertex colors.\n";
			
			for(unsigned i = 0; i < size / 4 ; i++) {
				unsigned writePos = wmoFile.tellg();
				wmoFile.seekp(writePos);

				for(unsigned j = 0; j < 4; j++)
					wmoFile.write(reinterpret_cast<const char *>(&color[j]), sizeof(UINT8));

				wmoFile.seekg(writePos + sizeof(unsigned));
			}
		}


		wmoFile.seekg(pos);
		wmoFile.read(reinterpret_cast<char *>(&offset), sizeof(unsigned));
		wmoFile.seekg(pos + 4 + offset);
	}

	cout << "\n\n";
}