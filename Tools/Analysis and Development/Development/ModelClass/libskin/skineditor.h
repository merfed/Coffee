

#include "skin.h"
#include "../libm2/trace.h"
#ifndef SKINEDITOR_H_
#define SKINEDITOR_H_

class SkinEditor{
public:
	SkinEditor(char *m,int FileSize);
	SkinEditor();
	~SkinEditor();
	char *getFile();
	int GetFileSize();


	Index *getIndices();
	int getnIndices();
	void newIndices(Index *ind, int num);
	void newIndices(int num);

	Triangle *getTriangles();
	int getnTriangles();
	void newTriangles(Triangle *tri, int num);

	VProp *getProperties();
	int getnProperties();
	void newProperties(VProp *pro,int num);
	void newProperties(int num);

	Submesh *getSubmeshes();
	int getnSubmeshes();
	void newSubmeshes(Submesh *sub,int num);

	TexUnit *getTexUnits();
	int getnTexUnits();
	void newTexUnits(TexUnit *tex, int num);

private:
	void FillLine();
	void SetStructure();
	char *f;
	int FileSize;
	SkinHeader *header_s;
	Index *index_s;
	Triangle *triangle_s;
	VProp *propertie_s;
	Submesh *submesh_s;
	TexUnit *texunit_s;

};


#endif
