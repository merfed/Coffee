#ifndef __MASH
#define __MASH

#include <string>

#include <wx/progdlg.h>
#include <wx/string.h>
#include "list.h"
#include "zoneGroup.h"

int mash(list<ZoneGroup> zoneGroupList, std::string outputName, bool, bool, wxProgressDialog*, int zOffset=0, int wdtXOffset=0, int wdtYOffset=0, int wdtZOffset=0);

#endif
