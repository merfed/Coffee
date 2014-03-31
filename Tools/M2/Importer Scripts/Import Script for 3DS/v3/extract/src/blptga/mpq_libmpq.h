#ifndef MPQ_H
#define MPQ_H

#include <string>
#include <set>


class MPQFile
{
	//MPQHANDLE handle;
	bool eof;
	char *buffer;
	size_t pointer,size;

	// disable copying
	MPQFile(const MPQFile &f) {}
	void operator=(const MPQFile &f) {}

public:
	MPQFile(const char* filename);	// filenames are not case sensitive
	~MPQFile();
	size_t read(void* dest, size_t bytes);
	size_t getSize();
	size_t getPos();
	char* getBuffer();
	char* getPointer();
	bool isEof();
	void seek(int offset);
	void seekRelative(int offset);
	void close();

};

inline void flipcc(char *fcc)
{
	char t;
	t=fcc[0];
	fcc[0]=fcc[3];
	fcc[3]=t;
	t=fcc[1];
	fcc[1]=fcc[2];
	fcc[2]=t;
}

#endif

