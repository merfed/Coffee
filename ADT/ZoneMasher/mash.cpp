/*****************************************************************************************************
*  Author: Written by Ryan "Riukuzaki" Cornwall.  Contains parts of code from Cryect's Tools.        *
*  Title: Riu's Zone Masher.  "Smash ADT's together willy-nilly to create new zones!"                *
*                                                                                                    *
*  Features:                                                                                         *
*      Allows for easy renaming of ADT files                                                         *
*      Accepts any number of ADT files                                                               *
*      Attempts to fit all ADTs into a single map                                                    *
*      Automatically adjusts zones to fit new coordinates.                                           *
*      Automatically creates a WDT file that works with the new ADTs.                                *
*****************************************************************************************************/

#include <cstdio>
#include <string>
#include <sstream>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <windows.h> //Used in creating directory
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <wx/progdlg.h>
#include <wx/string.h>

#include "list.h" //Makes my life so much easier
#include "zoneGroup.h" //Custom class that contains information about a group of ADTs
#include "offsetFix.h" //Used in fixing offsets after movement
#include "createWDT.h" //Used in creating a WDT file for the new map
//#include "createWDL.h" //NOT YET IMPLEMENTED

//#pragma optimize("gsy", on)

int mash(list<ZoneGroup> zoneGroupList, std::string outputName, bool offsetFixEnabled, bool wdtCreateEnabled, wxProgressDialog* dialog, int zOffset, int wdtXOffset, int wdtYOffset, int wdtZOffset)
{
    dialog->Update(1, _T("Combining ADT groups"));
    for(int i=1; i < zoneGroupList.getSize(); i++) //Mash stuff!
        zoneGroupList[0].mash(zoneGroupList[i]);

    //Write the ADT files to the specified directory
    dialog->Update(10, _T("Writing ADT files to the specified directory"));
    WCHAR tempName[1000];
    for(int i=0; i < outputName.length(); i++) tempName[i] = outputName[i];
    tempName[outputName.length()] = '\0';
    CreateDirectory(tempName, NULL); //Create directory if it doesn't exist for storing the files
    zoneGroupList[0].write(outputName + "\\" + outputName, dialog); //Do NOT include in a for loop.  All files are in group one by now

    // Once files are written, we will correct offsets that might have appeared
    // This part of the code was modeled from Cryect's OffsetFix
    if(offsetFixEnabled){
        dialog->Update(50, _T("Correcting offsets"));
        for(int i=0; i < zoneGroupList[0].getSize(); i++) //Fix each file
        {
            std::fstream zoneFile;
            dialog->Update(50+int(i*100.0/zoneGroupList[0].getSize()*0.3), _("Correcting offsets for: ")+wxString(zoneGroupList[0][i].c_str(), wxConvUTF8));
            zoneFile.open(zoneGroupList[0][i].c_str(), std::ios::in | std::ios::out | std::ios::binary);
            //int offsetX=0, offsetY=0; //Get the offsets
            OffsetFixData offData;
            offData.offset.x    = zoneGroupList[0].getAdtX(i);
            offData.offset.y    = zoneGroupList[0].getAdtY(i);
            offData.offset.zOff = 1.0 * zOffset;
            offData.offset.wdtXOff = 1.0 * wdtXOffset;
            offData.offset.wdtYOff = 1.0 * wdtYOffset;
            offData.offset.wdtZOff = 1.0 * wdtZOffset;

            //Fix the offsets
            findMCNKs      (zoneFile, offData);
            findMDDFandMODF(zoneFile, offData);
            fixMCNKs       (zoneFile, offData);
            fixDoodads     (zoneFile, offData);
            fixWMOs        (zoneFile, offData);

            //createWDL      (zoneFile); //NOT YET IMPLEMENTED
            zoneFile.close();
        }
    }
    //Create a WDT file
    /* This part of the code was checked with Cryect's CreateWDT.  I made several improvements compared to the original. */
    if(wdtCreateEnabled){
        dialog->Update(82, _("Creating WDT file."));
        createWDT(zoneGroupList[0], dialog);
    }
    dialog->Update(100, _("Finished!"));
    return 0;
}
