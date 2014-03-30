#ifndef M2IMPORT_H
#define M2IMPORT_H

#include "DataExchange.h"

#include <maya/MPxFileTranslator.h>

#include <fstream>

// The plugin class that interfaces with Maya
class Plugin : public MPxFileTranslator
{
public:
	Plugin();
	~Plugin();

	static void* creator();

	MStatus reader(const MFileObject& file, const MString& optionsString, FileAccessMode mode);
	MStatus writer(const MFileObject& file, const MString& optionsString, FileAccessMode mode);

	bool haveReadMethod() const;
	bool haveWriteMethod() const;

	MString defaultExtension() const;

	MFileKind identifyFile(const MFileObject& filename, const char* buffer, short size) const;

	void parseOptions(const MString& options);

private:
	void importAll();

	// The actual import routines (defined in import.cpp)
	void readName(const M2Header& header);
	void readBones(const M2Header& header);
	void readVertices(const M2Header& header);
	void readViews(const M2Header& header);
	void sendDataToMaya(const DEMesh& mesh, const M2View* view);

	char* m_file;

	bool m_verbose;
	bool m_importBones;
};

#endif