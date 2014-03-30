#define M2MOD_VERSION "v4.04 for Cata"

#include "CM2.h"
#include "SimpleIni.h"
#include <iostream>
#include <string.h>
#include <cstdio>

extern int g_Verbose;

static const Float32 DegreesToRadians = 0.017453292519943295f;

static const Float32 MaxPositionalTolerance = 0.005f;
static const Float32 MaxAngularTolerance = 90.0f;


int main(int ArgumentCount, char* Arguments[])
{
	M2Lib::CM2 M2;

	char iniFileName[1024];
	strcpy(iniFileName, Arguments[0]);
	char* lastSlash = strrchr(iniFileName, '\\');
	if (NULL == lastSlash)
	{
		lastSlash = strrchr(iniFileName, '/');
	}
	if (NULL != lastSlash)
	{
		lastSlash[1] = 0;
	}
	strcat(iniFileName, "M2Mod.ini");

	const char* szInputM2 = 0;
	const char* szOutputM2I = 0;
	const char* szInputM2I = 0;
	const char* szOutputM2 = 0;
	const char* szSubmeshPositionalTolerance = 0;
	const char* szSubmeshAngularTolerance = 0;
	const char* szBodyPositionalTolerance = 0;
	const char* szBodyAngularTolerance = 0;
	const char* szClothingPositionalTolerance = 0;
	const char* szClothingAngularTolerance = 0;
	const char* szIgnoreMaterials = 0;
	const char* szIgnoreBones = 0;
	const char* szIgnoreAttachments = 0;
	const char* szIgnoreCameras = 0;
	const char* szMirrorCamera = 0;
	const char* szScale = 0;
	const char* szShowVersion = 0;
	g_Verbose = 0;
	Float32 SubmeshPositionalTolerance = 0.0005f;
	Float32 SubmeshAngularTolerance = 60.0f;
	Float32 BodyPositionalTolerance = 0.0005f;
	Float32 BodyAngularTolerance = 90.0f;
	Float32 ClothingPositionalTolerance = 0.0005f;
	Float32 ClothingAngularTolerance = 45.0f;
	bool IgnoreMaterials = false;
	bool IgnoreBones = false;
	bool IgnoreAttachments = false;
	bool IgnoreCameras = false;
	bool MirrorCamera = false;
	Float32 Scale = 1.0f;
	bool ShowVersion = false;

	CSimpleIniA Ini;
	bool Pause = true;
	if ( ArgumentCount <= 1 )
	{
		// take inputs from INI
		//std::cout << "loading INI" << std::endl;
		if ( Ini.LoadFile( iniFileName ) != SI_OK )
		{
			std::cout << "Error: Could not load M2Mod.ini. Make sure M2Mod.ini is in same directory as M2Mod.exe." << std::endl;
			system("pause");
			return 0;
		}
		szInputM2 = Ini.GetValue( "input", "InputM2" );
		szOutputM2I = Ini.GetValue( "input", "OutputM2I" );
		szInputM2I = Ini.GetValue( "input", "InputM2I" );
		szOutputM2 = Ini.GetValue( "input", "OutputM2" );
		szSubmeshPositionalTolerance = Ini.GetValue( "options", "SubmeshPositionalTolerance" );
		szSubmeshAngularTolerance = Ini.GetValue( "options", "SubmeshAngularTolerance" );
		szBodyPositionalTolerance = Ini.GetValue( "options", "BodyPositionalTolerance" );
		szBodyAngularTolerance = Ini.GetValue( "options", "BodyAngularTolerance" );
		szClothingPositionalTolerance = Ini.GetValue( "options", "ClothingPositionalTolerance" );
		szClothingAngularTolerance = Ini.GetValue( "options", "ClothingAngularTolerance" );
		szIgnoreMaterials = Ini.GetValue( "options", "IgnoreMaterials" );
		szIgnoreBones = Ini.GetValue( "options", "IgnoreBones" );
		szIgnoreAttachments = Ini.GetValue( "options", "IgnoreAttachments" );
		szMirrorCamera = Ini.GetValue( "options", "MirrorCamera" );
		szScale = Ini.GetValue( "options", "Scale" );
		szShowVersion = Ini.GetValue( "debug", "ShowVersion" );
		g_Verbose = Ini.GetLongValue( "debug", "Verbose" );
	}
	else
	{
		// take inputs from command line
		//std::cout << "loading CMD" << std::endl;
		Pause = false;
		for ( SInt32 i = 1; i < ArgumentCount; i++ )
		{
			if ( strcmp( Arguments[i], "/InputM2" ) == 0 )
			{
				i++;
				szInputM2 = Arguments[i];
			}
			else if ( strcmp( Arguments[i], "/OutputM2I" ) == 0 )
			{
				i++;
				szOutputM2I = Arguments[i];
			}
			else if ( strcmp( Arguments[i], "/InputM2I" ) == 0 )
			{
				i++;
				szInputM2I = Arguments[i];
			}
			else if ( strcmp( Arguments[i], "/OutputM2" ) == 0 )
			{
				i++;
				szOutputM2 = Arguments[i];
			}
			else if ( strcmp( Arguments[i], "/SubmeshPositionalTolerance" ) == 0 )
			{
				i++;
				szSubmeshPositionalTolerance = Arguments[i];
			}
			else if ( strcmp( Arguments[i], "/SubmeshAngularTolerance" ) == 0 )
			{
				i++;
				szSubmeshAngularTolerance = Arguments[i];
			}
			else if ( strcmp( Arguments[i], "/BodyPositionalTolerance" ) == 0 )
			{
				i++;
				szBodyPositionalTolerance = Arguments[i];
			}
			else if ( strcmp( Arguments[i], "/BodyAngularTolerance" ) == 0 )
			{
				i++;
				szBodyAngularTolerance = Arguments[i];
			}
			else if ( strcmp( Arguments[i], "/ClothingPositionalTolerance" ) == 0 )
			{
				i++;
				szClothingPositionalTolerance = Arguments[i];
			}
			else if ( strcmp( Arguments[i], "/ClothingAngularTolerance" ) == 0 )
			{
				i++;
				szClothingAngularTolerance = Arguments[i];
			}
			else if ( strcmp( Arguments[i], "/IgnoreMaterials" ) == 0 )
			{
				szIgnoreMaterials = "1";
			}
			else if ( strcmp( Arguments[i], "/IgnoreBones" ) == 0 )
			{
				szIgnoreBones = "1";
			}
			else if ( strcmp( Arguments[i], "/IgnoreAttachments" ) == 0 )
			{
				szIgnoreBones = "1";
			}
			else if ( strcmp( Arguments[i], "/IgnoreCameras" ) == 0 )
			{
				szIgnoreBones = "1";
			}
			else if ( strcmp( Arguments[i], "/MirrorCamera" ) == 0 )
			{
				szMirrorCamera = "1";
			}
			else if ( strcmp( Arguments[i], "/Scale" ) == 0 )
			{
				i++;
				szScale = Arguments[i];
			}
			else if ( strcmp( Arguments[i], "/ShowVersion" ) == 0 )
			{
				szShowVersion = "1";
			}
			else if ( strcmp( Arguments[i], "/Verbose" ) == 0 )
			{
				i++;
				g_Verbose = atoi( Arguments[i] );
			}
		}
	}

	if ( szShowVersion )
	{
		if ( szShowVersion[0] == '0' )
		{
			ShowVersion = false;
		}
		else
		{
			ShowVersion = true;
		}
	}
	if ( ShowVersion )
	{
		std::cout << "M2Mod version: " << M2MOD_VERSION << std::endl;
	}

	if ( szIgnoreMaterials )
	{
		if ( szIgnoreMaterials[0] == '0' )
		{
			IgnoreMaterials = false;
		}
		else
		{
			IgnoreMaterials = true;
		}
	}

	if ( szIgnoreBones )
	{
		if ( szIgnoreBones[0] == '0' )
		{
			IgnoreBones = false;
		}
		else
		{
			IgnoreBones = true;
		}
	}

	if ( szIgnoreAttachments )
	{
		if ( szIgnoreAttachments[0] == '0' )
		{
			IgnoreAttachments = false;
		}
		else
		{
			IgnoreAttachments = true;
		}
	}

	if ( szIgnoreCameras )
	{
		if ( szIgnoreCameras[0] == '0' )
		{
			IgnoreCameras = false;
		}
		else
		{
			IgnoreCameras = true;
		}
	}

	if ( szMirrorCamera )
	{
		if ( szMirrorCamera[0] == '0' )
		{
			MirrorCamera = false;
		}
		else
		{
			MirrorCamera = true;
		}
	}

	if ( szSubmeshPositionalTolerance )
	{
		SubmeshPositionalTolerance = (Float32)atof( szSubmeshPositionalTolerance );
	}
	if ( szSubmeshAngularTolerance )
	{
		SubmeshAngularTolerance = (Float32)atof( szSubmeshAngularTolerance );
	}
	if ( szBodyPositionalTolerance )
	{
		BodyPositionalTolerance = (Float32)atof( szBodyPositionalTolerance );
	}
	if ( szBodyAngularTolerance )
	{
		BodyAngularTolerance = (Float32)atof( szBodyAngularTolerance );
	}
	if ( szClothingPositionalTolerance )
	{
		ClothingPositionalTolerance = (Float32)atof( szClothingPositionalTolerance );
	}
	if ( szClothingAngularTolerance )
	{
		ClothingAngularTolerance = (Float32)atof( szClothingAngularTolerance );
	}

	if ( SubmeshPositionalTolerance < 0.0f )
	{
		SubmeshPositionalTolerance = 0.0f;
	}
	else if ( SubmeshPositionalTolerance > MaxPositionalTolerance )
	{
		SubmeshPositionalTolerance = MaxPositionalTolerance;
	}

	if ( SubmeshAngularTolerance < 0.0f )
	{
		SubmeshAngularTolerance = 0.0f;
	}
	else if ( SubmeshAngularTolerance > MaxAngularTolerance )
	{
		SubmeshAngularTolerance = MaxAngularTolerance;
	}

	if ( BodyPositionalTolerance < 0.0f )
	{
		BodyPositionalTolerance = 0.0f;
	}
	else if ( BodyPositionalTolerance > MaxPositionalTolerance )
	{
		BodyPositionalTolerance = MaxPositionalTolerance;
	}

	if ( BodyAngularTolerance < 0.0f )
	{
		BodyAngularTolerance = 0.0f;
	}
	else if ( BodyAngularTolerance > MaxAngularTolerance )
	{
		BodyAngularTolerance = MaxAngularTolerance;
	}

	if ( ClothingPositionalTolerance < 0.0f )
	{
		ClothingPositionalTolerance = 0.0f;
	}
	else if ( ClothingPositionalTolerance > MaxPositionalTolerance )
	{
		ClothingPositionalTolerance = MaxPositionalTolerance;
	}

	if ( ClothingAngularTolerance < 0.0f )
	{
		ClothingAngularTolerance = 0.0f;
	}
	else if ( ClothingAngularTolerance > MaxAngularTolerance )
	{
		ClothingAngularTolerance = MaxAngularTolerance;
	}

	SubmeshAngularTolerance *= DegreesToRadians;
	BodyAngularTolerance *= DegreesToRadians;
	ClothingAngularTolerance *= DegreesToRadians;

	if ( szInputM2 )
	{
		std::cout << "Loading M2:    " << szInputM2 << std::endl;
		if ( M2Lib::EError Error = M2.Load( const_cast<Char8*>(szInputM2) ) )
		{
			std::cout << M2Lib::GetErrorText( Error ) << std::endl;
			system("pause");
			return -1;
		}
	}
	else
	{
		std::cout << "Error: No input M2 specified" << std::endl;
		system("pause");
		return -1;
	}

	if ( szOutputM2I )
	{
		std::cout << "Exporting M2I: " << szOutputM2I << std::endl;
		if ( M2Lib::EError Error = M2.ExportM2Intermediate( const_cast<Char8*>(szOutputM2I) ) )
		{
			std::cout << M2Lib::GetErrorText( Error ) << std::endl;
			system("pause");
			return -1;
		}
	}

	if ( szInputM2I )
	{
		std::cout << "Importing M2I: " << szInputM2I << std::endl;
		if ( M2Lib::EError Error = M2.ImportM2Intermediate( const_cast<Char8*>(szInputM2I), IgnoreMaterials, IgnoreBones, IgnoreAttachments, IgnoreCameras, SubmeshPositionalTolerance, SubmeshAngularTolerance, BodyPositionalTolerance, BodyAngularTolerance, ClothingPositionalTolerance, ClothingAngularTolerance ) )
		{
			std::cout << M2Lib::GetErrorText( Error ) << std::endl;
			system("pause");
			return -1;
		}
	}

	if ( szMirrorCamera )
	{
		M2.MirrorCamera();
	}

	if ( szScale )
	{
		Float32 Scale = (Float32)atof( szScale );
		if ( Scale != 1.0f )
		{
			if ( Scale < 0.8f )
			{
				Scale = 0.8f;
			}
			else if ( Scale > 1.2f )
			{
				Scale = 1.2f;
			}
			M2.Scale( Scale );
		}
	}

	if ( szOutputM2 )
	{
		std::cout << "Saving M2:     " << szOutputM2 << std::endl;
		if ( M2Lib::EError Error = M2.Save( const_cast<Char8*>(szOutputM2) ) )
		{
			std::cout << M2Lib::GetErrorText( Error ) << std::endl;
			system("pause");
			return -1;
		}
	}

	std::cout << "Operations Complete" << std::endl;

	if ( Pause )
	{
		system("pause");
	}

	return 0;
}
