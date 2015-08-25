#ifndef WDT_H_
#define WDT_H_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "Chunk.h"

class Wdt
{
	public:

        Wdt();
        Wdt(const std::string & wdtFileName);
        Wdt(const std::string & name
        , const Chunk & cMver
        , const Chunk & cMphd
        , const Chunk & cMain
        , const Chunk & cMwmo
        , const Chunk & cModf
        );

        std::vector<Chunk> getFullWDT();

        // MPHD
            int getFlags();
            void setFlags(int flags);

        // MAIN
            int getMAINentry(const int X, const int Y);
            void setMAINentry(const int X, const int Y, const char flag);

        // MWMO/MODF
            std::string getWMOpath();
            std::vector<int> getWMOIDinfos();
            std::vector<float> getWMOpos();
            std::vector<float> getWMOori();
            std::vector<float> getWMOext();
            std::vector<short> getWMOaddInfos();

            static Chunk buildMWMO(std::string n);
            static Chunk buildMODF(int id, int uid, float posX, float posY, float posZ, float oriA, float oriB, float oriC,
                                   float upe1, float upe2, float upe3, float loe1, float loe2, float loe3,
                                   short flags, short dds, short ns);

        void toFile();
        friend std::ostream & operator<<(std::ostream & os, const Wdt & wdt);

	private:

        std::string wdtName;
        Chunk mver;
        Chunk mphd;
        Chunk main;
        Chunk mwmo;
        Chunk modf;
};

#endif
