#include <iostream>
using std::ostream;
using std::cout;
using std::cin;
using std::ios;
using std::showbase;
using std::hex;
using std::noshowbase;
using std::dec;

#include <iomanip>
using std::setw;

#include <fstream>
using std::fstream;

struct AnimBlock {
	short			interp;
	short			globalID;
	unsigned int	nInterp;
	unsigned int	ofsInterp;
	unsigned int	nTimestamp;
	unsigned int	ofsTimestamp;
	unsigned int	nValues;
	unsigned int	ofsValues;
};

struct Particle {
	int				ID;
	int				flags;
	float			position[3];
	short			boneID;
	short			textureID;
	unsigned int	nInts;
	unsigned int	ofsInts;
	unsigned int	nInts2;
	unsigned int	ofsInts2;
	short			blendingMode;
	short			emitterType;
	short			particleType;
	short			texRotation;
	short			texRows;
	short			texColumns;
	AnimBlock		parameters[10];
	//float			unk;
	float			midPoint;
	unsigned int	colors[3];
	float			sizes[3];
	short			tiles[10];
	float			unk2[3];
	float			scaling[3];
	float			slowDown;
	float			rotation;
	float			unk3[10];
	float			unk4[6];
	AnimBlock		unk5;
};

ostream &operator<<(ostream &out, Particle &left) {
	out << "\nParticle ID: " << left.ID
		<< "\nFlags: " << showbase << hex << left.flags << noshowbase << dec
		<< "\nPosition: "
		<< "\n      X: " << left.position[0]
		<< "\n      Y: " << left.position[1]
		<< "\n      Z: " << left.position[2]
		<< "\nBone ID: " << left.boneID
		<< "\nTexture ID: " << left.textureID
		<< "\nBlending Mode: " << left.blendingMode
		<< "\nEmitter Type: " << left.emitterType
		<< "\nParticle Type: " << left.particleType
		<< "\nTexture Rotation: " << left.texRotation
		<< "\nMidpoint: " << left.midPoint
		<< "\nColors (ARGB): "
		<< showbase << hex
		<< "\n  Start: " << left.colors[0]
		<< "\n    Mid: " << left.colors[1]
		<< "\n    End: " << left.colors[2]
		<< noshowbase << dec
		<< "\nSizes: "
		<< "\n  Start: " << left.sizes[0]
		<< "\n    Mid: " << left.sizes[1]
		<< "\n    End: " << left.sizes[2]
		<< "\nSlowdown: " << left.slowDown
		<< "\nRotation: " << left.rotation
		<< '\n';

	return out;
}
void printMenu() {
	cout << "\nWhat value do you want to change?"
			"\n   1. Flags"
			"\n   2. XPosition"
			"\n   3. YPosition"
			"\n   4. ZPosition"
			"\n   5. BoneID"
			"\n   6. TextureID"
			"\n   7. Blending Mode"
			"\n   8. Emitter Type"
			"\n   9. Particle Type"
			"\n  10. Texture Rotation"
			"\n  11. Midpoint"
			"\n  12. Start Color (ARGB Hex)"
			"\n  13. Middle Color (ARGB Hex)"
			"\n  14. End Color (ARGB Hex)"
			"\n  15. Start Size"
			"\n  16. Middle Size"
			"\n  17. End Size"
			"\n  18. Slowdown"
			"\n  19. Rotation"
			"\n  20. Emission Speed"
			"\n  21. Speed Variation"
			"\n  22. Spread"
			"\n  23. Gravity"
			"\n  24. Lifespan"
			"\n  25. Emission Rate"
			"\n  26. Emission Area Length"
			"\n  27. Emission Area Width"
			"\n  28. Strong Gravity"
			"\n"
			"\n  29. Save and go to next particle"
			"\n  30. Save and Exit"
			"\n\n? ";
}
void printParameters();
void changeData(short);
void writeData(short);

unsigned int nParticles;
unsigned int ofsParticles;
Particle part;
float parameters[10];

fstream m2File;

int main(int argc, char **argv) {
	if(argc == 1) {
		cout << "<ParticleModder.exe> <M2 File>\n";
		return 1;
	}

	if(strstr(argv[1], ".m2")) {
		m2File.open(argv[1], ios::in | ios::out | ios::binary);

		m2File.seekg(0x13C);

		m2File.read(reinterpret_cast<char *>(&nParticles), sizeof(unsigned int));
		if(nParticles == 0) {
			cout << "Model has no particle effects!\n";
			return 1;
		}
		m2File.read(reinterpret_cast<char *>(&ofsParticles), sizeof(unsigned int));

		m2File.seekg(ofsParticles);
		m2File.read(reinterpret_cast<char *>(&part), sizeof(Particle));

		for(short i = 0; i < 10; i++) {
			m2File.seekg(part.parameters[i].ofsValues);
			m2File.read(reinterpret_cast<char *>(&parameters[i]), sizeof(float));
		}

		for(short i = 0; i < nParticles; i++) {
			m2File.seekg(ofsParticles + (i * 0x1F8));
			m2File.read(reinterpret_cast<char *>(&part), sizeof(Particle));

			for(short i = 0; i < 10; i++) {
				m2File.seekg(part.parameters[i].ofsValues);
				m2File.read(reinterpret_cast<char *>(&parameters[i]), sizeof(float));
			}

			short choice = -1;
			while(choice != 29) {
				cout << "Number of particles: " << nParticles
					<<"\nOffset to particles: " << showbase << hex << ofsParticles << noshowbase << dec <<'\n';
				cout << part;
				printParameters();
				do {
					printMenu();
					cin >> choice;
				} while(choice < 0 || choice > 30);

				if(choice == 30) {
					writeData(i);
					cout << "*** M2 file particle data saved!\n";
					return 0;
				}

				else changeData(choice);

				system("cls");
			}

			writeData(i);
			cout << "*** M2 file particle data saved!\n\n";
		}
	}
	else cout << "File is not a .m2!\n";
}

void printParameters() {
	cout << "\n\nParameters:\n"
		<<"\nEmission Speed: " << parameters[0]
		<<"\nSpeed Variation (0 to 1): " << parameters[1]
		<<"\nSpread (0 to pi): " << parameters[2]
		<<"\nUnknown (0 to 2*pi): " << parameters[3]
		<<"\nGravity: " << parameters[4]
		<<"\nLifespan: " << parameters[5]
		<<"\nEmission Rate: " << parameters[6]
		<<"\nEmission area length: " << parameters[7]
		<<"\nEmission area width: " << parameters[8]
		<<"\nStrong Gravity: " << parameters[9]
		<<'\n';
}

void changeData(short choice) {
	cout << "\nOld Value: ";
	switch(choice) {
		case 1:
			cout << showbase << hex <<part.flags << noshowbase << hex;
			break;
		case 2:
		case 3:
		case 4:
			cout << part.position[choice - 2];
			break;
		case 5:
			cout << part.boneID;
			break;
		case 6:
			cout << part.textureID;
			break;
		case 7:
			cout << part.blendingMode;
			break;
		case 8:
			cout << part.emitterType;
			break;
		case 9:
			cout << part.particleType;
			break;
		case 10:
			cout << part.texRotation;
			break;
		case 11:
			cout << part.midPoint;
			break;
		case 12:
		case 13:
		case 14:
			cout << showbase << hex << part.colors[choice - 12] << noshowbase << dec;
			break;
		case 15:
		case 16:
		case 17:
			cout << part.sizes[choice - 15];
			break;
		case 18:
			cout << part.slowDown;
			break;
		case 19:
			cout << part.rotation;
			break;
		case 20:
		case 21:
		case 22:
			cout << parameters[choice - 20];
			break;
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:
		case 28:
			cout << parameters[choice - 19];
			break;
	}
	cout << "\nEnter new value: ";
		switch(choice) {
		case 1:
			cin >> part.flags;
			break;
		case 2:
		case 3:
		case 4:
			cin >> part.position[choice - 2];
			break;
		case 5:
			cin >> part.boneID;
			break;
		case 6:
			cin >> part.textureID;
			break;
		case 7:
			cin >> part.blendingMode;
			break;
		case 8:
			cin >> part.emitterType;
			break;
		case 9:
			cin >> part.particleType;
			break;
		case 10:
			cin >> part.texRotation;
			break;
		case 11:
			cin >> part.midPoint;
			break;
		case 12:
		case 13:
		case 14:
			cin >> hex >> part.colors[choice - 12] >> dec;
			break;
		case 15:
		case 16:
		case 17:
			cin >> part.sizes[choice - 15];
			break;
		case 18:
			cin >> part.slowDown;
			break;
		case 19:
			cin >> part.rotation;
			break;
		case 20:
		case 21:
		case 22:
			cin >> parameters[choice - 20];
			break;
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:
		case 28:
			cin >> parameters[choice - 19];
			break;
	}
}

void writeData(short number) {
	m2File.seekp(ofsParticles + (number * 0x1F8));
	m2File.write(reinterpret_cast<const char *>(&part), sizeof(Particle));

	for(int i = 0; i < 10; i++) {
		m2File.seekp(part.parameters[i].ofsValues);
		m2File.write(reinterpret_cast<const char *>(&parameters[i]), sizeof(float));
	}
}