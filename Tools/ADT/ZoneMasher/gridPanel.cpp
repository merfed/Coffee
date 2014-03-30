#include "gridPanel.h"

GridPanel::GridPanel(wxWindow *parent,
            wxWindowID winid,
            const wxPoint& pos,
            const wxSize& size,
            long style,
            const wxString& name)
        : wxPanel(parent, winid, pos,
                  size, style, name){
    for(int i=0; i < gridX; i++)
        for(int j=0; j < gridY; j++){
            matrix[i][j]=0;
        }
    isDragging=false;
    clearGrid();
    Connect(wxEVT_PAINT, wxPaintEventHandler(GridPanel::onPaint));
}

void GridPanel::onPaint(wxPaintEvent& event){
    wxPaintDC dc(this);

    wxSize size = GetClientSize();
    int gridTop = size.GetHeight() - gridY * getCellY();

    for(int i=0; i < gridX; ++i)
        for(int j=0; j < gridY; ++j)
            drawCell(dc, 0+i*getCellX(), gridTop + j*getCellY(), matrix[i][j]);
}

void GridPanel::refresh(){
    wxPaintDC dc(this);

    wxSize size = GetClientSize();
    int gridTop = size.GetHeight() - gridY * getCellY();

    for(int i=0; i < gridX; ++i)
        for(int j=0; j < gridY; ++j)
            drawCell(dc, 0+i*getCellX(), gridTop + j*getCellY(), matrix[i][j]);
    wxWindow::Refresh();
}

void GridPanel::clearGrid(){;}

void GridPanel::drawCell(wxPaintDC& dc, int x, int y, int color){
    //Colors are as follows: 0=Empty, 1=Single ADT, 2=Multiple ADT (sharing spot). This should be an enum.
    if(color > 1)      color = color%4+2;
    else if(color < 0) color = 0;
    static wxColour colors[] = { wxColour(255,255,255), wxColour(228, 11, 37), wxColour( 29,161,228),
                                 wxColour( 20,178, 20), wxColour( 40,172,178), wxColour( 71, 84,213)};
    static wxColour light[]  = { wxColour(255,255,255), wxColour(228, 40, 63), wxColour( 29,181,205),
                                 wxColour( 27,237, 27), wxColour( 48,206,213), wxColour( 83,128,248)};
    static wxColour dark[]   = { wxColour(255,255,255), wxColour(162,  2, 25), wxColour( 29,116,228),
                                 wxColour( 27,118, 27), wxColour( 27,118,122), wxColour( 57, 67,171)};
    if(color){
        wxPen pen(light[color]);
        pen.SetCap(wxCAP_PROJECTING);
        dc.SetPen(pen);

        dc.DrawLine(x, y+getCellY()-1, x+getCellX()-1, y+getCellY()-1);
        dc.DrawLine(x+getCellX()-1, y+getCellY()-1, x+getCellX()-1, y+1);

        wxPen darkpen(dark[color]);
        darkpen.SetCap(wxCAP_PROJECTING);
        dc.SetPen(darkpen);

        dc.DrawLine(x+1,y+getCellY()-1,x+getCellX()-1,y+getCellY()-1);
        dc.DrawLine(x+getCellX()-1,y+getCellY()-1,x+getCellX()-1,y+1);

        dc.SetPen(*wxTRANSPARENT_PEN);
        dc.SetBrush(wxBrush(colors[color]));
        dc.DrawRectangle(x+1, y+1, getCellX()-2, getCellY()-2);
    }
    else{
        dc.SetPen(*wxTRANSPARENT_PEN);
        dc.SetBrush(wxBrush(colors[color]));
        dc.DrawRectangle(x, y+1, getCellX()-2, getCellY()-2);
    }
}
