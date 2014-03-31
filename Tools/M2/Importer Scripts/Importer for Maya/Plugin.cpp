#include "Plugin.h"
#include "ImportException.h"

#include <maya/MGlobal.h>
#include <maya/MFnPlugin.h>

#include <iostream>

using namespace std;

MStatus initializePlugin(MObject obj)
{
	MGlobal::startErrorLogging();

	MFnPlugin plugin(obj, "Esa Nuuros", "1.0", "Any");

	return plugin.registerFileTranslator("M2", "none", Plugin::creator, "m2FileTranslatorOpts", 0, false);
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin(obj);

	MStatus ret = plugin.deregisterFileTranslator("M2");

	MGlobal::stopErrorLogging();

	return ret;
}

Plugin::Plugin():
m_verbose(true),
m_importBones(false)
{
}

Plugin::~Plugin()
{
}

void *Plugin::creator()
{
	return new Plugin();
}

MStatus Plugin::reader(const MFileObject &file, const MString &optionsString, FileAccessMode mode)
{	
	parseOptions(optionsString);

	const char *fname = file.fullName().asChar();

	cout << "Importing .m2 from " << fname << " ..." << endl;
	std::ifstream inputFile(fname, ios::in | ios::binary);
	if(inputFile.fail())
	{
		cerr << "Error: the file " << fname << " could not be opened for reading." << endl;
		return MS::kFailure;
	}

	// Read the entire file into memory
	inputFile.seekg(0, ios::end);

	unsigned int fileSize = inputFile.tellg();
	inputFile.seekg(0, ios::beg);
	m_file = new char [fileSize];
	inputFile.read(m_file, fileSize);

	inputFile.close();

	try
	{
		if( (mode == MPxFileTranslator::kImportAccessMode) ||
			(mode == MPxFileTranslator::kSaveAccessMode))
		{
			importAll();
		}   
	}
	catch(ImportException e)
	{
		cerr << "Exception: " << endl << e.getMessage() << endl;
		return MS::kFailure;
	}

	return MS::kSuccess;
}

MStatus Plugin::writer(const MFileObject &file, const MString &optionsString, FileAccessMode mode)
{
	cerr << "Plugin::writer isn't implemented." << endl;

	return MS::kFailure;
}

bool Plugin::haveReadMethod() const
{
	return true;
}

bool Plugin::haveWriteMethod() const
{
	return false;
}

MString Plugin::defaultExtension() const
{
	return "m2";
}

MPxFileTranslator::MFileKind Plugin::identifyFile(const MFileObject &filename, const char *buffer, short size) const
{
	const char *fname = filename.name().asChar();
	char magic[4];

	std::ifstream inputFile(fname, ios::in | ios::binary);
	if(inputFile.fail()) 
		return kNotMyFileType;

	inputFile.read(magic, 4);
	inputFile.close();

	if(magic[0] == 'M' && magic[1] == 'D' && magic[2] == '2' && magic[3] == '0')
		return kCouldBeMyFileType;
	else 
		return kNotMyFileType;
}

void Plugin::parseOptions(const MString& options)
{
	if(options.length() < 1) return;

	MStringArray optionList;
	options.split(';', optionList);

	for(unsigned int i = 0; i < optionList.length(); i++)
	{
		MStringArray option;
		optionList[i].split('=', option);

		if(option.length() < 1) continue;

		if(option[0]		== "verbose") m_verbose = (option[1] == "1" ? true : false);
		else if(option[0]	== "bones") m_importBones = (option[1] == "1" ? true : false);    	
	}		
}