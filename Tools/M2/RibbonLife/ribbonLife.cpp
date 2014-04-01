#include <iostream>
using std::cout;
using std::cin;
using std::ios;

#include <fstream>
using std::fstream;

/*
struct AnimBlock {
	short interp;
	short globalID;
	unsigned int nInterp;
	unsigned int ofsInterp;
	unsigned int nTimestamp;
	unsigned int ofsTimestamp;
	unsigned int nValues;
	unsigned int ofsValues;
};
*/

unsigned int nRibbons;
unsigned int ofsRibbons;
float life1;
float life2;

fstream m2File;

int main(int argc, char **argv) {
	if(argc == 1) {
		cout << "<RibbonLife.exe> <M2 file>\n";
		return 1;
	}
	if(strstr(argv[1], ".m2")) {
		m2File.open(argv[1], ios::in | ios::out | ios::binary);
		if(!m2File) {
			cout << "File could not be opened!\n";
			return 1;
		}

		m2File.seekg(0x134);

		m2File.read(reinterpret_cast<char *>(&nRibbons), sizeof(unsigned int));
		if(nRibbons == 0) {
			cout << "No ribbons exist in the model!\n";
			return 1;
		}
		m2File.read(reinterpret_cast<char *>(&ofsRibbons), sizeof(unsigned int));

		int offset = 0x94;
		for(int i = 0; i < nRibbons; i++, offset += 0xDC) {
			m2File.seekg(ofsRibbons + offset);

			m2File.read(reinterpret_cast<char *>(&life1), sizeof(float));
			m2File.read(reinterpret_cast<char *>(&life2), sizeof(float));

			cout << "Ribbon #" << i + 1 << ":\n"
				<<"\nLife1: " << life1
				<<"\nLife2: " << life2 << '\n';

			float newLife = -1;
			while(newLife < 0 || newLife > 100) {
				cout << "\nEnter a new life (0 to 100): ";
				cin >> newLife;
			}
			life1 = life2 = newLife;
			m2File.seekp(ofsRibbons + offset);

			m2File.write(reinterpret_cast<const char *>(&life1), sizeof(float));
			m2File.write(reinterpret_cast<const char *>(&life2), sizeof(float));

			cout << '\n';
		}

		cout << "\n   *** Lives both written. Saving file...\n\n";
	}
	else {
		cout << "File is not a .m2 file!\n";
		return 1;
	}

	m2File.close();
}