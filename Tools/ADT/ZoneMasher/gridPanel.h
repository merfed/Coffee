#ifndef __GRID_PANEL
#define __GRID_PANEL
#include <wx/wx.h>
#include "list.h"
#include "zoneGroup.h"

class GridPanel : public wxPanel{
  private:
    enum{
        gridX = 64,
        gridY = 64
    };
    bool isDragging; //True if user is dragging a ZoneGroup
    int matrix[gridX][gridY]; //Matrix used for locations/overlapping of ADT files.  Needed for proper coloring.
    int offX, offY; //Used for keeping track of the offset of drag events
    int dragX, dragY; //Keeps track of the point that a drag event occurred from
    list<int> matrixOverlay[gridX][gridY]; //Matrix that contains list of ZoneGroup references of ZoneGroups that modified that cell.
    void clearGrid(); //Clears grid of all cell states
    void drawCell(wxPaintDC &dc, int x, int y, int color=0); //Draws grid with all cells

  protected: //This is where events go.  Still needs OnClick, etc.
    void onPaint(wxPaintEvent& event);

  public:
    GridPanel(wxWindow *parent,
            wxWindowID winid = wxID_ANY,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxTAB_TRAVERSAL | wxNO_BORDER,
            const wxString& name = wxPanelNameStr);

    void setMatrix(list<ZoneGroup> zoneGroupList, int modifiedGroup=-1, int offsetX=0, int offsetY=0){
        //Zero-out the matrix.  Prevents strange bugs when calling this function more than once.
        for(int i=0; i < gridX; i++)
            for(int j=0; j < gridY; j++)
                matrix[i][j] = 0;
        for(int i=0; i < zoneGroupList.getSize(); i++)
            for(int j=0; j < zoneGroupList[i].getSize(); j++){
                int x=zoneGroupList[i].getAdtX(j);
                int y=zoneGroupList[i].getAdtY(j);
                if(modifiedGroup == i){
                    x += offsetX;
                    y += offsetY;
                }
                if(matrix[x][y] == 0)
                    matrix[x][y] = i+2;
                else if(matrix[x][y] > 1)
                    matrix[x][y] = 1;
            }
    }

    void updateMatrixOverlay(ZoneGroup zGroup, int zGroupIndex){
        for(int i=0; i < zGroup.getSize(); i++){
            int x=zGroup.getAdtX(i);
            int y=zGroup.getAdtY(i);
            matrixOverlay[x][y].pushFront(zGroupIndex);
        }
    }

    bool getIsDragging(){return isDragging;}
    int getCellX(){return GetClientSize().GetWidth()  / gridX;}
    int getCellY(){return GetClientSize().GetHeight() / gridY;}
    int getGridX(){return gridX;}
    int getGridY(){return gridY;}
    int getOffX() {return offX;}
    int getOffY() {return offY;}
    int getDragX(){return dragX;}
    int getDragY(){return dragY;}

    //Return the ZoneGroup that is on top.  Returns -1 if DNE
    int getZoneOverlay(int x, int y){return matrixOverlay[x][y].getSize() ? matrixOverlay[x][y][0] : -1;}

    void setIsDragging(bool val){isDragging=val;}
    void setOffX(int x){offX=x;}
    void setOffY(int y){offY=y;}
    void setDragX(int x){dragX=x;}
    void setDragY(int y){dragY=y;}

    void popZoneOverlay(int x, int y){if(matrixOverlay[x][y].getSize()) matrixOverlay[x][y].popElement(0);}
    void pushZoneOverlay(int val, int x, int y){matrixOverlay[x][y].pushFront(val);}
    void refresh();
};

#endif
