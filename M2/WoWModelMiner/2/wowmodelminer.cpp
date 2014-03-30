// WoWModelMiner.cpp : Defines the entry point for the console application.
// Created by: jb55 (jackbox55@gmail.com)


#include "stdafx.h"
#include <vector>
#include "../../../wdbtocsv/wdbtocsv.h"

extern vector<CWDBDef> gDefList;

enum itemclass
{
	Weapon = 2,
	Armor = 4
};
enum itemsubclass
{
	Axe,
	TAxe,
	Bow,
	Gun,
	Mace,
	TMace,
	Polearm,
	Sword,
	TSword,
	Obsolete,
	Staff,
	Exotic,
	TExotic,
	Fist,
	Miscellaneous,
	Dagger,
	Thrown,
	Spear,
	Crossbow,
	Wand,
	FishingPole,
	Jewelry=0,
	Misc=0,
	Cloth,
	Leather,
	Mail,
	Plate,
	Buckler,
	Shield
};

void wtoc(char* Dest, TCHAR* Source, int SourceSize)
{
for(int i = 0; i < SourceSize; ++i)
Dest[i] = (char)Source[i];
}

bool makeCSV(CWDB *pWDB);

int _tmain(int argc, _TCHAR* argv[])
{
	CWDB wdb;
	LoadDefinitions();


	char* file = (char*)malloc(0xff);

	if( !argv[1] )
		file = "itemcache.wdb";
	else
		wtoc(file,argv[1],256);

	if (!wdb.loadWDB(file))
		return 0;

	if(memcmp(wdb.getDef()->getSignature(),"BDIW",4))
		return 0;

	wdb.parseWDB();
	makeCSV(&wdb);
	
	return 0;
}

int getBack(int c, int s)
{
	if( c == Armor ){
		switch( s ){
		case Shield:
			return 28;
		default:
			return 0;
		}
	}

	if( c == Weapon ){
		switch( s ){
			case FishingPole:
			case Polearm:
			case TSword:
			case TMace:
			case TAxe:
				return 26;
			case Dagger:
			case Miscellaneous:
			case Fist:
			case Mace:
			case Axe:
			case Sword:
				return 32;
			case Staff:
				return 30;
			default:
				return 0;
		}
	}
	return 0;

}

bool makeCSV(CWDB* pWDB)
{
	FILE* pFile;
	int c,s;
	char* str;
	fopen_s(&pFile,"items_gen.csv","w");

	if(!pFile)
		return false;

	// write the goods
	for(int i=0;i<(int)pWDB->getRecords()->size();++i)
	{
		c = (int)pWDB->getField(i,3)->uival;
		if( c != Weapon && c != Armor )
			continue;
		s = (int)pWDB->getField(i,4)->uival;
		str = pWDB->getField(i,6)->pchar;
		fprintf(pFile,"%u,%u,%u,%u,%u,%u,%u,%s\n",
			pWDB->getField(i,1)->uival, // ItemID
			pWDB->getField(i,10)->uival,	// DisplayID
			c, // ItemClass
			s, // ItemSubClass
			pWDB->getField(i,15)->uival, // Slot
			(u32)getBack(c,s), // Position on character
			pWDB->getField(i,113)->uival, // Sheath
			pWDB->getField(i,6)->pchar
		); 

		// Time to
	}

	return true;
}

