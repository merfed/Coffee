#ifndef __CREATE_WDT
#define __CREATE_WDT
#include <fstream>
#include <iostream>
#include <string>
#include <wx/progdlg.h>
#include <wx/string.h>

#include "zoneGroup.h"

/**************************************************************
*  createWDT creates a WDT file based on the fstream passed to*
*  it. The function compares the file name with known file    *
*  names to determine flags.  If the name isn't found, it uses*
*  a default.                                                 *
**************************************************************/

    #ifndef __TYPEDEF_UINT32
        #define __TYPEDEF_UINT32
        typedef unsigned long UInt32;
    #endif

void createWDT(ZoneGroup&, wxProgressDialog*);

#endif
