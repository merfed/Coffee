#include "createWDT.h"
#include <wx/progdlg.h>
#include <wx/string.h>

/**************************************************************
*  All WDT's start with 0x4d564552 (REVM), 0x04, 0x12,        *
*  0x4d504844 (DHPM), and 0x20.  All of those are 1 byte      *
*  in length.  The next byte contains the flags. 0x01 is      *
*  for WMO-based maps, and 0x08 is probably for maps that     *
*  contain phasing.  The other two flags are unknown. After   *
*  the flag byte, there appears to be 7 empty bytes. After    *
*  those bytes, all WDT files seem to contain: 0x4d41494e     *
*  (NIAM), and 0x8000 (weird C symbol).  After these bytes,   *
*  there is a giant array of 64x64 bytes which contains 0x0   *
*  for ADT files that don't exist and 0x1 for ADT's that do   *
*  exist.                                                     *
**************************************************************/

void createWDT(ZoneGroup& zGroup, wxProgressDialog* dialog){
    if(zGroup.getSize()){ //Guard against empty zoneFile
        std::fstream wdtFile;
        wdtFile.open((zGroup.getAdtBase(0) + ".wdt").c_str(), std::fstream::out | std::fstream::binary);
        UInt32 buffer;
        buffer=0x4d564552;
        wdtFile.write((char*)&buffer, 4*1);
        buffer=0x04;
        wdtFile.write((char*)&buffer, 4*1);
        buffer=0x12;
        wdtFile.write((char*)&buffer, 4*1);
        buffer=0x4d504844; //DHPM
        wdtFile.write((char*)&buffer, 4*1);
        buffer=0x20;
        wdtFile.write((char*)&buffer, 4*1);

        /**************************************************************
        *  This next part uses a hard-coded database that compares    *
        *  the name supplied by the user to a list of known maps to   *
        *  "automagically" determine the flag state.  If the name is  *
        *  not found, then a default of 0x0 for the flag will be used.*
        **************************************************************/
        std::string a=zGroup.getOnlyMapName();
        if( a == "test" || a == "Test" || a == "StormwindJail" ||
            a == "StormwindPrison" || a == "Collin" || a == "WailingCaverns" ||
            a == "Monastery" || a == "Blackfathom" || a == "Uldaman" ||
            a == "GnomeragonInstance" || a == "SunkenTemple" || a == "BlackRockSpire" ||
            a == "BlackrockDepths" || a == "OnyxiaLairInstance" || a == "Mauradon" ||
            a == "DeeprunTram" || a == "OrgrimmarInstance" || a == "MoltenCore" ||
            a == "DireMaul" || a == "AlliancePVPBarracks" || a == "HordePVPBarracks" ||
            a == "HellfireMilitary" || a == "HellfireDemon" || a == "HellfireRaid" ||
            a == "CoilfangPumping" || a == "CoilfangMarsh" || a == "CoilfangDraenei" ||
            a == "CoilfangRaid" || a == "TempestKeepRaid" || a == "TempestKeepArcane" ||
            a == "TempestKeepAtrium" || a == "TempestKeepFactor" || a == "AuchindounShadow" ||
            a == "AuchindounDemon" || a == "AuchindounEthereal" || a == "AuchindounDraenei" ||
            a == "GruulsLair" || a == "Nexus70" || a == "Sunwell5Man")

            buffer=0x01;
        else if( a == "CraigTest")
            buffer = 0x04;
        else if( a == "ExteriorTest" || a == "Valgarde70" || a == "UtgardePinnacle" ||
                 a == "Nexus80" || a == "Sunwell5ManFix" || a == "Ulduar70" ||
                 a == "DrakTheronKeep" || a == "Azjol_Uppercity" || a == "Ulduar80" ||
                 a == "UlduarRaid" || a == "GunDrak")
            buffer = 0x06;
        else if( a == "development_nonweighted")
            buffer = 0x08;
        else if( a == "Northrend" || a == "QA_DVD" || a == "NorthrendBG" ||
                 a == "DalaranPrison" || a == "DeathKnightStart" || a == "ChamberOfAspectsBlack" ||
                 a == "NexusRaid" || a == "DalaranArena" || a == "OrgrimmarArena" ||
                 a == "Azjol_LowerCity" || a == "WintergraspRaid")
            buffer = 0x0E;
        else buffer = 0x00;
        wdtFile.write((char*)&buffer, 4*1);
        buffer = 0x00; //Create 7 empty bytes
        for(int i=0; i < 7; i++)
            wdtFile.write((char*)&buffer, 4*1);
        buffer = 0x4d41494e; //NIAM
        wdtFile.write((char*)&buffer, 4*1);
        buffer = 0x8000; //Weird C symbol
        wdtFile.write((char*)&buffer, 4*1);
        for(int y=0; y < 64; y++)
            for(int x=0; x < 64; x++){
                dialog->Update(int(82+18.0*(y*64+x)/(64*64)), _("Creating WDT file."));
                buffer = 0x00;
                for(int i=0; i < zGroup.getSize(); i++)
                    if(x == zGroup.getAdtX(i) && y == zGroup.getAdtY(i))
                        buffer = 0x01;
                wdtFile.write((char*)&buffer, 4*1);
                buffer = 0x00;
                wdtFile.write((char*)&buffer, 4*1);
            }
        buffer = 0x4d574d4f; //OMWM
        wdtFile.write((char*)&buffer, 4*1);
        buffer = 0x00;
        wdtFile.write((char*)&buffer, 4*1);
        wdtFile.close();
    }
}
