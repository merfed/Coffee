#include "blpparse.h"
#include "mpq_libmpq.h"
#include <cstdio>
#include <sys/stat.h>

unsigned char *gdata;
int gwidth;
int gheight;

size_t get_filesize(const char *FileName)
{
	struct stat file;
	if(!stat(FileName,&file)) {
		return file.st_size;
	}
	return 0;
}

MPQFile::MPQFile(const char* filename):
	eof(false),
	buffer(0),
	pointer(0),
	size(0)
{
	FILE *f = fopen(filename,"rb");
	if (!f) {
		printf("File not found\n");
		exit(1);
	}
	size = get_filesize(filename);
	buffer = new char[size];
	fread(buffer, size, 1, f);
}

MPQFile::~MPQFile()
{
	close();
}

size_t MPQFile::read(void* dest, size_t bytes)
{
	if (eof) return 0;

	size_t rpos = pointer + bytes;
	if (rpos > size) {
		bytes = size - pointer;
		eof = true;
	}

	memcpy(dest, &(buffer[pointer]), bytes);

	pointer = rpos;

	return bytes;
}

bool MPQFile::isEof()
{
    return eof;
}

void MPQFile::seek(int offset)
{
	pointer = offset;
	eof = (pointer >= size);
}

void MPQFile::seekRelative(int offset)
{
	pointer += offset;
	eof = (pointer >= size);
}

void MPQFile::close()
{
	if (buffer) delete[] buffer;
	buffer = 0;
	eof = true;
}

size_t MPQFile::getSize()
{
	return size;
}

size_t MPQFile::getPos()
{
	return pointer;
}

char* MPQFile::getBuffer()
{
	return buffer;
}

char* MPQFile::getPointer()
{
	return buffer + pointer;
}


bool LoadBLP(const char *filename) {
	int offsets[16],sizes[16],w,h;
	int format;

	char attr[4];

	MPQFile f(filename);
	if (f.isEof()) {
		return false;
	}

	f.seek(8);
	f.read(attr,4);
	f.read(&w,4);
	f.read(&h,4);
	f.read(offsets,4*16);
	f.read(sizes,4*16);

	gwidth = w;
	gheight = h;
	gdata = new unsigned char[w*h*4];

	bool hasmipmaps = attr[4]>0;
	int mipmax = 1;// hasmipmaps ? 16 : 1;

	if (attr[0] == 2) {
		// compressed

		format = DDS_DXT1;
		int blocksize = 8;

		// guesswork here :(
		if (attr[1]==8) {
			format = DDS_DXT3;
			blocksize = 16;
		} else {
			if (!attr[3]) format = DDS_DXT1;
		}

		unsigned char *buf = new unsigned char[sizes[0]];

		// do every mipmap level
		for (int i=0; i<mipmax; i++) {
			if (w==0) w = 1;
			if (h==0) h = 1;
			if (offsets[i] && sizes[i]) {
				f.seek(offsets[i]);
				f.read(buf,sizes[i]);

				int size = ((w+3)/4) * ((h+3)/4) * blocksize;

				DecodeDDSFormat(format, buf, w, h, gdata);

			} else break;
			w >>= 1;
			h >>= 1;
		}

		delete[] buf;
	}
	else if (attr[0]==1) {
		// uncompressed
		unsigned int pal[256];
		f.read(pal,1024);

		unsigned char *buf = new unsigned char[sizes[0]];
		unsigned int *buf2 = new unsigned int[w*h];
		unsigned int *p;
		unsigned char *c, *a;

		int alphabits = attr[1];
		bool hasalpha = alphabits!=0;

		for (int i=0; i<mipmax; i++) {
			if (w==0) w = 1;
			if (h==0) h = 1;
			if (offsets[i] && sizes[i]) {
				f.seek(offsets[i]);
				f.read(buf,sizes[i]);

				int cnt = 0;

				p = buf2;
				c = buf;
				a = buf + w*h;
				for (int y=0; y<h; y++) {
					for (int x=0; x<w; x++) {
						unsigned int k = pal[*c++];

						//k = ((k&0x00FF0000)>>16) | ((k&0x0000FF00)) | ((k& 0x000000FF)<<16);

						int alpha;
						if (hasalpha) {
							if (alphabits == 8) {
								alpha = (*a++);
							} else if (alphabits == 1) {
								//alpha = (*a & (128 >> cnt++)) ? 0xff : 0;
								alpha = (*a & (1 << cnt++)) ? 0xff : 0;
								if (cnt == 8) {
									cnt = 0;
									a++;
								}
							}
						} else alpha = 0xff;

						k |= alpha << 24;
						*p++ = k;
					}
				}

			} else break;
			w >>= 1;
			h >>= 1;
		}

		gdata = (unsigned char*)buf2;

		//delete[] buf2;
		delete[] buf;
	}

	f.close();

}

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;

#pragma pack(push,1)
struct TGAHeader {
	uint16 z1;
	uint8 b1;
	uint8 z2;
	uint32 z3;
	uint32 z4;
	uint16 width;
	uint16 height;
	uint8 bpp1;
	uint8 bpp2;
};
#pragma pack(pop)

void SaveTGA(const char *filename)
{
	TGAHeader bh;
	bh.z1 = 0;
	bh.z2 = 0;
	bh.z3 = 0;
	bh.z4 = 0;
	bh.b1 = 2;
	bh.width = gwidth;
	bh.height = gheight;
	bh.bpp1 = 32;
	bh.bpp2 = 0;

	FILE *f = fopen(filename,"wb");
	fwrite(reinterpret_cast<const char*>(&bh),sizeof(TGAHeader),1,f);
	//fwrite(gdata,gwidth*gheight,4,f);
	for (int j=gheight-1; j>=0; j--) {
		fwrite(gdata + j*gwidth*4, gwidth, 4, f);
	}
	fclose(f);
}

int main(int argc, char **argv)
{
	if (argc!=2) return 1;

	char fn[256];
	strncpy(fn,argv[1],256);

	size_t lp = strlen(fn)-3;
	fn[lp++]='t';
	fn[lp++]='g';
	fn[lp++]='a';

	printf("%s\n",fn);

	if (LoadBLP(argv[1])) {
		SaveTGA(fn);
	}

	printf("done\n");

	return 0;
}

