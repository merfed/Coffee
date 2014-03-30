#ifndef __ZONE_GROUP
#define __ZONE_GROUP

#include <string>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <wx/progdlg.h>
#include <wx/string.h>

#include "list.h"

    #ifndef __TYPEDEF_UINT32
        #define __TYPEDEF_UINT32
        typedef unsigned long UInt32;
    #endif

struct Point{ //For my purposes, I only need integers
    int x, y;
};

class ZoneGroup{
  private:
    Point upLeft, downRight; //Bounding box for group

    void combine(ZoneGroup& other, int offX=0, int offY=0){ //Combines the two ADT groups.  Should only be called by one of the mash functions
        for(int i=0; i < other.getSize(); i++){
            std::string temp; //Create a string for the temporary storage of itoa
            char cTemp[10];
            append(other[i], other.origList[i]);
            temp  = getAdtBase(getSize()-1) + "_";
            temp += itoa( other.getAdtX(i)+offX, cTemp, 10);
            temp += "_";
            temp += itoa( other.getAdtY(i)+offY, cTemp, 10);
            temp += ".adt";
            adtList[getSize()-1] = temp;
        }
    }

    void calcBounds(){ //Calculate top-left and bottom-right points of bounding rectangle
        for(int i=0; i < adtList.getSize(); i++){
            if(i==0){ //First value, so it must be the highest and lowest
                upLeft.x    = getAdtX(0);
                upLeft.y    = getAdtY(0);
                downRight.x = upLeft.x;
                downRight.y = upLeft.y;
            }
            else{
                if(getAdtX(i) < upLeft.x)    upLeft.x    = getAdtX(i);
                if(getAdtX(i) > downRight.x) downRight.x = getAdtX(i);
                if(getAdtY(i) < upLeft.y)    upLeft.y    = getAdtY(i);
                if(getAdtY(i) > downRight.y) downRight.y = getAdtY(i);
            }
        }
    }

  protected:
    list<std::string> adtList;  //List of ADTs in group
    list<std::string> origList; //List of ADT original names.  Should stay paired with adtList

  public:
    ZoneGroup(){
        upLeft.x    = 0;
        upLeft.y    = 0;
        downRight.x = 0;
        downRight.y = 0;
    }

    ~ZoneGroup(){;}

    bool isTouching(int x1, int y1, int x2, int y2){ //Checks to see if the two points touch each other
        bool touching=false;
        if     (x1+1==x2 && y1  ==y2) touching=true; //Right
        else if(x1  ==x2 && y1+1==y2) touching=true; //Down
        else if(x1-1==x2 && y1  ==y2) touching=true; //Left
        else if(x1  ==x2 && y1-1==y2) touching=true; //Up
        return touching;
    }

    bool isTouching(Point one, Point two){ //Checks to see if the two points touch each other
        int x1, x2, y1, y2;
        x1 = one.x;
        y1 = one.y;
        x2 = two.x;
        y2 = two.y;

        bool touching=false;
        if     (x1+1==x2 && y1  ==y2) touching=true; //Right
        else if(x1  ==x2 && y1+1==y2) touching=true; //Down
        else if(x1-1==x2 && y1  ==y2) touching=true; //Left
        else if(x1  ==x2 && y1-1==y2) touching=true; //Up
        return touching;
    }

    /*bool bruteMash(ZoneGroup& other) //Called by mash if it needs help.
    {
        int offsetX=0, offsetY=0;
        int startX=0, startY=0; //The X and Y start positions.
        bool repeated=false; //Becomes true when the outer for loop "loops" around to the beginning
        bool found=false;

        startX = getAdtX(0);
        startY = getAdtY(0);

        //We want to create dynamic arrays of this to preserve CPU cycles.  getAdtX and getAdtY are CPU-intensive
        int **groupOne;
        int **groupTwo;
        int tempX=64, tempY=64; //Added to group2/other to create a temporary location.  Needed to prevent overlap

        groupOne = new int*[getSize()];
        groupTwo = new int*[other.getSize()];
        for(int i=0; i < getSize(); i++)
            groupOne[i] = new int[2];
        for(int i=0; i < other.getSize(); i++)
            groupTwo[i] = new int[2];

        for(int i=0; i < getSize(); i++){
            groupOne[i][0] = getAdtX(i);
            groupOne[i][1] = getAdtY(i);
        }
        for(int i=0; i < other.getSize(); i++){
            groupTwo[i][0] = other.getAdtX(i)+tempX;
            groupTwo[i][1] = other.getAdtY(i)+tempY;
        }

        int minX, minY, maxX, maxY; //Used in determining boundaries of movement
        int curX=200, curY=200, tot=200*200+200*200; //Used in determining closest point.  Initialized with impossible values
        int matrix[64][64]; //Used in populating the possibleMoves list.
        list<Point> possibleMoves; //A list that contains all surrounding points of the current zoneGroup

        minX = (tempX+other.getX()) * -1; //Number of spaces to the left that other can be moved
        minY = (tempY+other.getY()) * -1; //Number of spaces up that other can be moved
        maxX = 0 - (other.getX()+other.getW());
        maxY = 0 - (other.getY()+other.getH());
        //Assign matrix to all 0's
        for(int x=0; x < 64; x++)
            for(int y=0; y < 64; y++)
                matrix[x][y] = 0;

        //Look through list, and assign matrix to 1 for all existing ADT's
        for(int i=0; i < getSize(); i++)
            matrix[ groupOne[i][0] ][ groupOne[i][1] ] = 1;

        //For all adjacent points next to a 1 in the matrix, if the value of that point is 0, assign it to 2
        for(int i=0; i < getSize(); i++){ //We only need to check the points in the groupOne list
            if(!matrix[groupOne[i][0]+1][groupOne[i][1]]) matrix[groupOne[i][0]+1][groupOne[i][1]] = 2;
            if(!matrix[groupOne[i][0]-1][groupOne[i][1]]) matrix[groupOne[i][0]-1][groupOne[i][1]] = 2;
            if(!matrix[groupOne[i][0]][groupOne[i][1]+1]) matrix[groupOne[i][0]][groupOne[i][1]+1] = 2;
            if(!matrix[groupOne[i][0]][groupOne[i][1]-1]) matrix[groupOne[i][0]][groupOne[i][1]-1] = 2;
        }

        //Populate possibleMoves with a list of points containing adjacent movements
        for(int x=0; x < 64; x++)
            for(int y=0; y < 64; y++)
                if( matrix[x][y] == 2){
                    Point a={x, y};
                    possibleMoves.pushFront(a);
                }

        //For every ADT in other, check for the closest possibleMove if within movement range
        for(int i=0; i < other.getSize(); i++)
            for(int j=0; j < possibleMoves.getSize(); j++){
                std::cout << "i=" << i << ", j=" << j << std::endl;
                int distX = possibleMoves[j].x - groupTwo[i][0];
                int distY = possibleMoves[j].y - groupTwo[i][1];
                if( distX >= minX && distY >= minY && distX < maxX && distY < maxY){ //Bounds checking
                    int distance = distX*distX + distY*distY; //Distance (squared) between the two points
                    if(distance < tot || curX == 200 || curY == 200){
                        bool noOverlap=true; //Becomes false if overlap is found
                        for(int k=0; k < getSize() && noOverlap; k++) //Check all possible collisions
                            for(int l=0; l < other.getSize() && noOverlap; l++)
                                if( groupOne[k][0] == groupTwo[l][0]+distX &&
                                    groupOne[k][1] == groupTwo[l][1]+distY)
                                    noOverlap=false;
                        if(noOverlap){
                            tot  = distance;
                            curX = distX;
                            curY = distY;
                        }
                    }
                }
            }

        //If a possible mash was found, then mash away!
        if(curX != 200 && curY != 200){
            offsetX = curX+tempX; //We want to remove the tempX.  Otherwise, coordinates will be off
            offsetY = curY+tempY; //We want to remove the tempY.  Otherwise, coordinates will be off
            combine(other, offsetX, offsetY);
            return true;
        }
        return found;
    }*/

    bool mash(ZoneGroup& other) //Returns true if mash succeeded
    {
        if(getSize() > 0 && other.getSize() > 0)
            combine(other);
        return true;
    }

    int getAdtX(int n){ //Find X value in the file name at index n
        if(n < getSize() && n >= 0){
            std::string temp = adtList[n];

            int underscoreCounter=0;
            if(temp.find_last_of(".")) //Throw away extensions
                temp = temp.substr(0,temp.find_last_of("."));
            while(temp.find_last_of("1234567890_") == strlen(temp.c_str())-1 && underscoreCounter < 1) //Throw away Y coordinate
            {
                if(temp[ strlen(temp.c_str())-1 ] == '_') underscoreCounter++;
                temp = temp.replace( strlen(temp.c_str())-1, 1, ""); //Pop the number or underscore
            }
            temp = temp.substr(temp.find_last_of("_")+1); //Chop off ADT name before X value
            return atoi(temp.c_str());
        }
        else return 0;
    }

    int getAdtY(int n){ //Find Y value in the file name at index n
        if(n < getSize() && n >= 0){
            std::string temp = adtList[n];

            if(temp.find_last_of(".")) //Throw away extensions
                temp = temp.substr(0,temp.find_last_of("."));
            temp = temp.substr(temp.find_last_of("_")+1); //Chop off ADT name before Y value
            return atoi(temp.c_str());
        }
        else return 0;
    }

    int getX(){ return upLeft.x;}

    int getY(){ return upLeft.y;}

    int getH(){ return downRight.y - upLeft.y;}

    int getW(){ return downRight.x - upLeft.x;}

    std::string getAdtBase(int n){ //Strips coords from ADT at index n and returns the base
        if(n < getSize() && n >= 0){
            std::string temp = adtList[n];

            int underscoreCounter=0;
            if(temp.find_last_of(".")) //Throw away extensions
                temp = temp.substr(0,temp.find_last_of("."));
            while(temp.find_last_of("1234567890_") == strlen(temp.c_str())-1 && underscoreCounter < 2) //Throw away coordinates
            {
                if(temp[ strlen(temp.c_str())-1 ] == '_') underscoreCounter++;
                temp = temp.replace( strlen(temp.c_str())-1, 1, ""); //Pop the number or underscore
            }
            return temp;
        }
        else return "ERROR";
    }

    std::string getAdtFileName(int n){ //Returns name + extension without parent directory
        if(n < getSize() && n >= 0){
            std::string temp = adtList[n];
            if(temp.find_last_of("/\\")) //Throw away parent folder
                temp = temp.substr(temp.find_last_of("/\\")+1);
            return temp;
        }
        return "ERROR"; //Bad index
    }

    std::string getOnlyMapName(){ //Returns name without extension or parent folder
        if(0 < getSize()){
            std::string temp = adtList[0];

            int underscoreCounter=0;
            if(temp.find_last_of("/\\")) //Throw away parent folder
                temp = temp.substr(temp.find_last_of("/\\")+1);
            if(temp.find_last_of(".")) //Throw away extensions
                temp = temp.substr(0,temp.find_last_of("."));
            while(temp.find_last_of("1234567890_") == strlen(temp.c_str())-1 && underscoreCounter < 2) //Throw away coordinates
            {
                if(temp[ strlen(temp.c_str())-1 ] == '_') underscoreCounter++;
                temp = temp.replace( strlen(temp.c_str())-1, 1, ""); //Pop the number or underscore
            }
            return temp;
        }
        else return "ERROR";
    }

    std::string& operator [](int n){ return adtList[n];} //list header takes care of bounds-checking

    void append(std::string str){
        origList.pushBack(str);
        adtList.pushBack(str);
        calcBounds(); //Bounds probably changed.  Be sure to check.
    }

    void append(std::string str, std::string origStr){
        origList.pushBack(origStr);
        adtList.pushBack(str);
        calcBounds(); //Bounds probably changed.  Be sure to check.
    }

    void append(list<std::string> strList){
        for(int i=0; i < strList.getSize(); i++){
            origList.pushBack(strList[i]);
            adtList.pushBack(strList[i]);
        }
        calcBounds();
    }

    void append(list<std::string> strList, list<std::string> origStrList){
        for(int i=0; i < strList.getSize(); i++){
            origList.pushBack(origStrList[i]);
            adtList.pushBack(strList[i]);
        }
        calcBounds();
    }

    int getSize(){ return adtList.getSize();}

    void write(std::string parentFolder, wxProgressDialog* dialog){ //Writes all files from origList to location at adtList
        for(int i=0; i < getSize(); i++){
            char *buffer;
            long size;
            std::ifstream inFile(origList[i].c_str(), std::ifstream::binary);
            std::string temp;
            char cTemp[10]; //Doesn't need to be more than 2 characters, but I'm using 10 for robustness
            temp = parentFolder + "_" ;
            temp += itoa(getAdtX(i), cTemp, 10);
            temp += "_";
            temp += itoa(getAdtY(i), cTemp, 10);
            temp += ".adt";
            adtList[i] = temp; //Update the adtList to the new directory.  Used in fixing offset.
            std::ofstream outFile( adtList[i].c_str(), std::ofstream::binary);
            inFile.seekg(0, std::ifstream::end);
            size = inFile.tellg();
            inFile.seekg(0);
            buffer = new char[size];
            inFile.read(buffer, size);
            outFile.write(buffer, size);
            delete [] buffer;
            inFile.close();
            outFile.close();
            dialog->Update(10+int(100.0*i/getSize()*0.4), _T("Writing ADT files to the specified directory"));
        }
    }

    void movePos(int offsetX, int offsetY){
        for(int i=0; i < getSize(); i++){
            std::string temp; //Create a string for the temporary storage of itoa
            char cTemp[10];
            temp  = getAdtBase(getSize()-1) + "_";
            temp += itoa( getAdtX(i)+offsetX, cTemp, 10);
            temp += "_";
            temp += itoa( getAdtY(i)+offsetY, cTemp, 10);
            temp += ".adt";
            adtList[i] = temp;
        }
        calcBounds();
    }
};

#endif
