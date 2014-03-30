#include "CM2.h"
#include "SimpleIni.h"

int main(int ArgumentCount, char* Arguments[])
{
	char iniFileName[1024];
	strcpy(iniFileName, Arguments[0]);
	char* lastSlash = strrchr(iniFileName, '\\');
	if (NULL == lastSlash)
		lastSlash = strrchr(iniFileName, '/');
	if (NULL != lastSlash)
		lastSlash[1] = 0;
	strcat(iniFileName, "M2Analyze.ini");

	CSimpleIniA Ini;
	if ( Ini.LoadFile( iniFileName ) != SI_OK )
	{
		return 0;
	}

	const char* InputM2 = Ini.GetValue( "", "InputM2" );

	M2Lib::CM2 M2;
	M2.Load( const_cast<Char8*>(InputM2) );

	M2.PrintInfo();

	void* Vertices = M2.Elements[M2Lib::CM2::EElement_Vertices].Data;
	void* Bones = M2.Elements[M2Lib::CM2::EElement_Bones].Data;

	for ( UInt32 i = 0; i != M2.Header.nViews; i++ )
	{
		if ( M2.Skins[i] )
		{
			M2.Skins[i]->PrintInfo( Vertices, Bones );
		}
	}

	return 0;
}
