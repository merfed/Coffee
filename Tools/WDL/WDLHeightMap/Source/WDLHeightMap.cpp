#include <iostream>
using std::cout;
using std::ios;

#include <fstream>
using std::ifstream;

#include <windows.h>

// TODO: Get a portable JPEG library, not this shitty .NET one
//		  that requires .NET 3.0

// must assembly include:
//	System.dll
//	WindowsBase.dll
//	PresentationCore.dll
//	PresentationFramework.dll

using namespace System::IO;
using namespace System::Windows::Media;
using namespace System::Windows::Media::Imaging;

// Offset to header in WDL
#define OFFSET			0x14

// JPEG Colors (256 Color Palette)
#define DEEPOCEAN		0x2
#define SHALLOWOCEAN	0xF
#define SEALEVEL		0x1F
#define GROUND			0x39
#define HILLS			0x8D
#define MOUNTAINS		0xB7
#define HIGH			0xFF

// Console Colors
enum consoleColors {BLACK, DARKBLUE, DARKGREEN, DARKCYAN, DARKRED, DARKMAGENTA, DARKYELLOW, DEFAULT, GREY, BLUE, GREEN, CYAN, RED, MAGENTA, YELLOW, WHITE};

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
ifstream wdlFile;
char *newName;

unsigned int offsets[64 * 64];
signed short MARE[17 * 17];
signed short innerMARE[16 * 16];

/*
*   http://www.wowdev.org/wiki/index.php/WDL                                                      *
*                                                                                                 *
*   Outer and inner work like this (x = outer, y = inner)                                         *
*                                                                                                 *
*   x     x     x     x     x     x     x     x     x     x     x     x     x     x     x     x   *
*      y     y     y     y     y     y     y     y     y     y     y     y     y     y     y      *
*   x     x     x     x     x     x     x     x     x     x     x     x     x     x     x     x   *
*      y     y     y     y     y     y     y     y     y     y     y     y     y     y     y      *
*                                                                                                 *
*   The pixels in-between must be interpolated somehow (nearest neighboor maybe?)                 *
*                                                                                                 *
*/

inline void determineLowResHeight(const double &value) {
	// ground
	if(value > 750)
		SetConsoleTextAttribute(console, WHITE);
	else if(value > 200)
		SetConsoleTextAttribute(console, DEFAULT);
	else if(value > 100)
		SetConsoleTextAttribute(console, GREY);
	else if(value > -100)
		SetConsoleTextAttribute(console, GREEN);
	// level
	else if(value == 0)
		SetConsoleTextAttribute(console, GREY);
	// water
	else if(value < -400)
		SetConsoleTextAttribute(console, DARKBLUE);
	else if(value < 0)
		SetConsoleTextAttribute(console, BLUE);

	cout << (char)(219);
}

inline void determineHighResHeight(const signed short &value, const int &indexX, const int &indexY, const array<System::Byte> ^pixels, const int &xOffset) {
	// ground
	if(value > 1000)
		pixels->SetValue(System::Byte(HIGH), (indexX + indexY*xOffset));
	else if(value > 300)
		pixels->SetValue(System::Byte(MOUNTAINS), (indexX + indexY*xOffset));
	else if(value > 200)
		pixels->SetValue(System::Byte(HILLS), (indexX + indexY*xOffset));
	else if(value > 0)
		pixels->SetValue(System::Byte(GROUND), (indexX + indexY*xOffset));
	// level
	else if(value == 0)
		pixels->SetValue(System::Byte(SEALEVEL), (indexX + indexY*xOffset));
	// water
	else if(value < -250)
		pixels->SetValue(System::Byte(DEEPOCEAN), (indexX + indexY*xOffset));
	else if(value < 0)
		pixels->SetValue(System::Byte(SHALLOWOCEAN), (indexX + indexY*xOffset));
}
inline void determineHighResHeightExact(const signed short &value, const int &indexX, const int &indexY, const array<System::Byte> ^pixels, const int &xOffset) {
	// ground
	if(value > 1500)
		pixels->SetValue(System::Byte(HIGH), (indexX + indexY*xOffset));
	else if(value < -512)
		pixels->SetValue(System::Byte(0x00), (indexX + indexY*xOffset));

	else {
		// offset by 512 so there will be no negative values
		int color = value + 512;

		// wait? Algebra came in handy for once!
		// get it on a scale of (0x00, 0xFF)
		color = (color * 253) / (1500 + 512);

		pixels->SetValue(System::Byte(color), (indexX + indexY*xOffset));
	}
}

void lowResHeightMap() {
	// position at MAOF chunk
	wdlFile.seekg(OFFSET);

	// read all offsets from the file into offsets array
	for(short i = 0; i < 64 * 64; i++)
		wdlFile.read(reinterpret_cast<char *>(&offsets[i]), sizeof(unsigned int));

	double average = 0;
	for(short i = 1; i <= 64 * 64; i++) {
		// if heightmap for tile exists
		if(offsets[i - 1] != 0) {
			wdlFile.seekg(offsets[i - 1] + 8);

			// read outer heights
			for(short j = 0; j < 17 * 17; j++)
				wdlFile.read(reinterpret_cast<char *>(&MARE[j]), sizeof(signed short));

			// get average of all the MARE values
			average = 0;
			for(short j = 0; j < 17 * 17; j++)
				average += MARE[j];
			average /= (17 * 17);

			determineLowResHeight(average);
		}
		else
			cout << ' ';
		if(i % 64 == 0 && i != 0)
			cout << '\n';
	}

	SetConsoleTextAttribute(console, DEFAULT);
}

void highResHeightMap(bool inner, bool gray) {
	const int width = (inner ? (17 + 17) * 64 : 17 * 64);
	int height = width;
	array<System::Byte> ^pixels = gcnew array<System::Byte>(height * width);

	// position at MAOF chunk
	wdlFile.seekg(OFFSET);

	// read all offsets from the file into offsets
	for(short i = 0; i < 64 * 64; i++)
		wdlFile.read(reinterpret_cast<char *>(&offsets[i]), sizeof(unsigned int));

	short percent;
	for(short x = 0; x < 64; x++) {
		for(short y = 0; y < 64; y++) {
			// if heightmap for tile exists
			if(offsets[x + y*64] != 0) {
				wdlFile.seekg(offsets[x + y*64] + 8);

				// read outer heights
				for(short j = 0; j < 17 * 17; j++)
					wdlFile.read(reinterpret_cast<char *>(&MARE[j]), sizeof(signed short));

				// read inner heights
				if(inner) {
					for(short j = 0; j < 16 * 16; j++)
						wdlFile.read(reinterpret_cast<char *>(&innerMARE[j]), sizeof(signed short));
				}

				// populate JPEG
				for(short x2 = 0; x2 < 17; x2++) {
					for(short y2 = 0; y2 < 17; y2++) {
						if(inner) {
							if(!gray)
								determineHighResHeight(MARE[x2 + y2*17], ((x2 + x*17)*2), ((y2 + y*17)*2), pixels, width);
							else
								determineHighResHeightExact(MARE[x2 + y2*17], ((x2 + x*17)*2), ((y2 + y*17)*2), pixels, width);
						}
						else {
							if(!gray)
								determineHighResHeight(MARE[x2 + y2*17], (x2 + x*17), (y2 + y*17), pixels, width);
							else
								determineHighResHeightExact(MARE[x2 + y2*17], (x2 + x*17), (y2 + y*17), pixels, width);
						}
					}
				}
				// inner values (VERY SHITTY ATM)
				if(inner) {
					for(short x2 = 0; x2 < 16; x2++) {
						for(short y2 = 0; y2 < 16; y2++) {
							if(!gray)
								determineHighResHeight(innerMARE[x2 + y2*16], ((x2 + x*17)*2)+1, ((y2 + y*17)*2) + 1, pixels, width);
							else
								determineHighResHeightExact(innerMARE[x2 + y2*16], ((x2 + x*17)*2)+1, ((y2 + y*17)*2) + 1, pixels, width);
						}
					}
				}
			}

			percent = ((x * y) * 100) / (63 * 63);
			// TODO: Find out why it goes to zero besides
			//       the obvious reason that x == 0 at the
			//       first iteration of every new line
			if(percent != 0)
				cout << '\r' << percent << "%";
		}
	}
	// save image
	BitmapPalette^ palette = (gray ? BitmapPalettes::Gray256 : BitmapPalettes::Halftone256);

	BitmapSource^ image = BitmapSource::Create(width, height, 96, 96, (gray ? PixelFormats::Gray8 : PixelFormats::Indexed8), palette, pixels, width);

	System::String ^name;
	// get number of chars in filepath
	short counter = 0;
	while(newName[counter] != '\0')
		counter++;
	// copy filepath byte by byte over to the String^
	for(short i = 0; i < counter; i++)
		name += (wchar_t)(newName[i]);
	name += ".jpg";
	System::IO::FileStream^ outFile = gcnew System::IO::FileStream(name, FileMode::Create);

	JpegBitmapEncoder^ encoder = gcnew JpegBitmapEncoder();
	encoder->QualityLevel = 100;
	encoder->Frames->Add(BitmapFrame::Create(image));
	encoder->Save(outFile);
}

int main(int argc, char **argv) {
	if(argc == 1) {
		cout << "<WDL Heightmap Viewer> <WDL File>\n";
		cout << "\nPress any key to quit...";
		_getch();
		return 1;
	}

	// -l is for a low resolution heightmap printed IN the console
	if(strstr(argv[1], "-l")) {
		if(strstr(argv[2], ".wdl")) {
			// open file and print low res heightmap
			wdlFile.open(argv[2], ios::in | ios::binary);
			lowResHeightMap();

			cout << "Press any key to quit...";
			_getch();
		}
		else
			cout << "Not a WDL file!\n";
	}
	// -i is for a BETA printing of the inner heights along with the outer
	else if(strstr(argv[1], "-i")) {
		if(strstr(argv[2], ".wdl")) {
			// open file and print high res heightmap with BETA inner values
			wdlFile.open(argv[2], ios::in | ios::binary);

			char *index = strstr(argv[2], ".wdl");
			index[0] = 'H';
			index[1] = 'i';
			index[2] = 'g';
			index[3] = 'h';

			newName = argv[2];

			highResHeightMap(true, false);
		}
		else
			cout << "Not a WDL file!\n";
	}
	// -gi is for printing a true heightmap (grayscale) with BETA inner values
	else if(strstr(argv[1], "-gi")) {
		if(strstr(argv[2], ".wdl")) {
			wdlFile.open(argv[2], ios::in | ios::binary);

			char *index = strstr(argv[2], ".wdl");
			index[0] = 'G';
			index[1] = 'r';
			index[2] = 'H';
			index[3] = 'i';

			newName = argv[2];

			highResHeightMap(true, true);
		}
	}
	// -g is for printing a true heightmap (grayscale)
	else if(strstr(argv[1], "-g")) {
		if(strstr(argv[2], ".wdl")) {
			wdlFile.open(argv[2], ios::in | ios::binary);

			char *index = strstr(argv[2], ".wdl");
			index[0] = 'G';
			index[1] = 'r';
			index[2] = 'a';
			index[3] = 'y';

			newName = argv[2];

			highResHeightMap(false, true);
		}
	}
	// else just print out a color heightmap
	else if(strstr(argv[1], ".wdl")) {
		// open file and print high res heightmap
		wdlFile.open(argv[1], ios::in | ios::binary);

		char *index = strstr(argv[1], ".wdl");
		index[0] = '\0';

		newName = argv[1];

		highResHeightMap(false, false);
	}
	else
		cout << "Invalid file or arguments!\n";
}
