#include <cassert>
#include <cstdio>
#include <string>
#include <fstream>
using namespace std;

const int MAXFIELDS = 256;
char DELIMITER = ',';

struct header {
	char id[4];
	size_t nRecords, nFields, recordSize, textSize;
};

union field {
	float fval;
	int ival;
};

enum types {
	T_INT,
	T_FLOAT,
	T_STR
};

int gInt[MAXFIELDS], gFloat[MAXFIELDS], gStr[MAXFIELDS];

void analyzeRow(header &h, FILE *f, char *text)
{
	field x;

	for (int i=0; i<h.nFields; i++) {
		fread(&x, 4, 1, f);

		if ((x.fval<-0.1f && x.fval > -10000.0f) || (x.fval>0.1f && x.fval < 10000.0f)) {
            gFloat[i]++;
		} else {
			if ((x.ival>10) && (x.ival<h.textSize) && (text[x.ival-1]==0)) {
				gStr[i]+=5;
			} else {
				gInt[i]++;
			}
		}

	}

}

int types[MAXFIELDS];

ofstream out;

void printTypes(header &h)
{
	for (int i=0; i<h.nFields; i++) {
		if (i!=0) out << DELIMITER;
		string s;
		switch (types[i]) {
			case T_INT: s = "INT"; break;
			case T_FLOAT: s = "FLOAT"; break;
			case T_STR: s = "STRING"; break;
		}
		out << s;
	}
	out << endl;
}

void printRow(header &h, FILE *f, char *text)
{
	field x[MAXFIELDS];
	fread(&x, h.nFields, 4, f);

	for (int i=0; i<h.nFields; i++) {
		if (i!=0) out << DELIMITER;
		string s;
		switch (types[i]) {
			case T_INT:
				out << x[i].ival;
				break;
			case T_FLOAT:
				out << x[i].fval;
				break;
			case T_STR:
				s = string(text + x[i].ival);
				bool delim = false;
				for (int k=0; k<s.length(); k++) {
                    if (s[k]==DELIMITER) {
						delim = true;
						break;
					}
				}
				if (delim) out << "\"";
                out << s;
				if (delim) out << "\"";
				break;
		}
	}
	out << endl;
}

void convert(const char *fname)
{
	string fn = fname;
	int l = fn.length();
	if (l<5) return;
	fn[l-4] = '.';
	fn[l-3] = 'c';
	fn[l-2] = 's';
	fn[l-1] = 'v';

	FILE *f = fopen(fname, "r");
	header h;
	fread(&h, 5, 4, f);

	assert(h.nFields <= MAXFIELDS);

	char *text;
	text = new char[h.textSize];

	fseek(f, h.nRecords * h.recordSize, SEEK_CUR);
	fread(text, h.textSize, 1, f);
	fseek(f, 0x14, SEEK_SET);

	// analyze first 300 or so rows
	int nr = 300;
	if (nr > h.nRecords) nr = h.nRecords;

	for (int i=0; i<MAXFIELDS; i++) gInt[i] = gFloat[i] = gStr[i] = 0;

	for (int i=0; i<nr; i++) analyzeRow(h, f, text);

	int val[MAXFIELDS];
	for (int i=0; i<MAXFIELDS; i++) {
		types[i] = T_INT;
		val[i] = gInt[i];

		if (gFloat[i] > val[i]) {
			types[i] = T_FLOAT;
			val[i] = gFloat[i];
		}

		if (gStr[i] > val[i]) {
			types[i] = T_STR;
		}
	}

	// output
	fseek(f, 0x14, SEEK_SET);

	out.open(fn.c_str(), ios::out|ios::trunc);
	printTypes(h);

	for (int i=0; i<h.nRecords; i++) {
		printRow(h, f, text);
	}

    out.close();

	delete[] text;
}

int main(int argc, char *argv[])
{
	for (int i=1; i<argc; i++) {
		if (!strcmp(argv[i],"-d") && i<argc-2) {
			DELIMITER = argv[i+1][0];
		}
		convert(argv[i]);
	}
    return 0;
}
