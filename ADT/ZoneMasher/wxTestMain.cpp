/***************************************************************
 * Name:      wxTestMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Ryan Cornwall ()
 * Created:   2010-07-15
 * Copyright: Ryan Cornwall ()
 * License:
 **************************************************************/

#include "wx_pch.h"
#include "wxTestMain.h"
#include <wx/msgdlg.h>

//(*InternalHeaders(wxTestFrame)
#include <wx/artprov.h>
#include <wx/bitmap.h>
#include <wx/icon.h>
#include <wx/intl.h>
#include <wx/image.h>
#include <wx/string.h>
//*)

#include <windows.h> //Used in creating directory
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string>
#include <sstream>

#include "list.h" //Makes my life so much easier
#include "mash.h"

//#pragma optimize("gsy", on)


//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(wxTestFrame)
const long wxTestFrame::ID_PANEL2 = wxNewId();
const long wxTestFrame::ID_CHECKBOX1 = wxNewId();
const long wxTestFrame::ID_CHECKBOX2 = wxNewId();
const long wxTestFrame::ID_STATICTEXT9 = wxNewId();
const long wxTestFrame::ID_SPINCTRL1 = wxNewId();
const long wxTestFrame::ID_STATICTEXT10 = wxNewId();
const long wxTestFrame::ID_SPINCTRL2 = wxNewId();
const long wxTestFrame::ID_STATICTEXT11 = wxNewId();
const long wxTestFrame::ID_SPINCTRL3 = wxNewId();
const long wxTestFrame::ID_STATICTEXT12 = wxNewId();
const long wxTestFrame::ID_SPINCTRL4 = wxNewId();
const long wxTestFrame::ID_TEXTCTRL1 = wxNewId();
const long wxTestFrame::ID_BUTTON1 = wxNewId();
const long wxTestFrame::ID_STATICTEXT1 = wxNewId();
const long wxTestFrame::ID_STATICTEXT2 = wxNewId();
const long wxTestFrame::ID_STATICTEXT3 = wxNewId();
const long wxTestFrame::ID_STATICTEXT4 = wxNewId();
const long wxTestFrame::ID_STATICTEXT5 = wxNewId();
const long wxTestFrame::ID_STATICTEXT6 = wxNewId();
const long wxTestFrame::ID_STATICTEXT7 = wxNewId();
const long wxTestFrame::ID_STATICTEXT8 = wxNewId();
const long wxTestFrame::ID_PANEL1 = wxNewId();
const long wxTestFrame::ID_MENUITEM1 = wxNewId();
const long wxTestFrame::ID_MENUITEM2 = wxNewId();
const long wxTestFrame::idMenuQuit = wxNewId();
const long wxTestFrame::idMenuAbout = wxNewId();
const long wxTestFrame::ID_STATUSBAR1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(wxTestFrame,wxFrame)
    //(*EventTable(wxTestFrame)
    //*)
END_EVENT_TABLE()

wxTestFrame::wxTestFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(wxTestFrame)
    wxStaticBoxSizer* StaticBoxSizer2;
    wxMenuItem* MenuItem2;
    wxFlexGridSizer* FlexGridSizer3;
    wxMenuItem* MenuItem1;
    wxMenuItem* MenuItem4;
    wxFlexGridSizer* FlexGridSizer2;
    wxBoxSizer* BoxSizer2;
    wxMenu* Menu1;
    wxMenuItem* MenuItem3;
    wxStaticBoxSizer* StaticBoxSizer3;
    wxBoxSizer* BoxSizer1;
    wxMenuBar* MenuBar1;
    wxStaticBoxSizer* StaticBoxSizer1;
    wxFlexGridSizer* FlexGridSizer1;
    wxMenu* Menu2;

    Create(parent, wxID_ANY, _("Riu\'s Zone Masher"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
    SetClientSize(wxSize(640,640));
    {
    	wxIcon FrameIcon;
    	FrameIcon.CopyFromBitmap(wxBitmap(wxImage(_T("earth.ico"))));
    	SetIcon(FrameIcon);
    }
    BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
    Panel1 = new wxPanel(this, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    Panel2 = new GridPanel(Panel1, ID_PANEL2, wxDefaultPosition, wxSize(480,480), wxTAB_TRAVERSAL|wxFULL_REPAINT_ON_RESIZE, _T("ID_PANEL2"));
    BoxSizer2->Add(Panel2, 3, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer1 = new wxFlexGridSizer(2, 1, 0, 0);
    StaticBoxSizer1 = new wxStaticBoxSizer(wxHORIZONTAL, Panel1, _("Options"));
    FlexGridSizer3 = new wxFlexGridSizer(0, 2, 0, 0);
    CheckBox1 = new wxCheckBox(Panel1, ID_CHECKBOX1, _("Fix Offsets"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
    CheckBox1->SetValue(true);
    CheckBox1->SetToolTip(_("Fix X and Y offsets of all ADT\'s.  Don\'t disable this unless you know what you\'re doing."));
    CheckBox1->SetHelpText(_("Fixes the X and Y coordinates of ADT\'s.  This is needed when changing the coordinates of ADT\'s.  Without it, map objects will be in the wrong places (typically this results in \"missing\" map objects or \"holes\" in the ADT file).  The map objects aren\'t really missing.  They\'re just going to be really far off of where you expect them."));
    FlexGridSizer3->Add(CheckBox1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    CheckBox2 = new wxCheckBox(Panel1, ID_CHECKBOX2, _("Create WDT"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX2"));
    CheckBox2->SetValue(true);
    CheckBox2->SetToolTip(_("Creates a WDT map file based on the new map name and the new ADT locations."));
    FlexGridSizer3->Add(CheckBox2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText9 = new wxStaticText(Panel1, ID_STATICTEXT9, _("Z-Offset"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT9"));
    StaticText9->SetToolTip(_("This changes the Z-Offset of all ADTs.  Do not change this value unless you know what you\'re doing."));
    StaticText9->SetHelpText(_("This changes the Z-Offset of all ADTs.  Do not change this value unless you know what you\'re doing."));
    FlexGridSizer3->Add(StaticText9, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    SpinCtrl1 = new wxSpinCtrl(Panel1, ID_SPINCTRL1, _T("0"), wxDefaultPosition, wxDefaultSize, 0, -5000, 5000, 0, _T("ID_SPINCTRL1"));
    SpinCtrl1->SetValue(_T("0"));
    SpinCtrl1->SetToolTip(_("This changes the Z-Offset of all ADTs.  Do not change this value unless you know what you\'re doing."));
    SpinCtrl1->SetHelpText(_("This changes the Z-Offset of all ADTs.  Do not change this value unless you know what you\'re doing."));
    FlexGridSizer3->Add(SpinCtrl1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText10 = new wxStaticText(Panel1, ID_STATICTEXT10, _("WMO X-Offset"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT10"));
    StaticText10->SetToolTip(_("This changes the X-Offset of all WMOs (World Map Objects).  Do not change this value unless you know what you\'re doing."));
    StaticText10->SetHelpText(_("This changes the X-Offset of all WMOs (World Map Objects).  Do not change this value unless you know what you\'re doing."));
    FlexGridSizer3->Add(StaticText10, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    SpinCtrl2 = new wxSpinCtrl(Panel1, ID_SPINCTRL2, _T("0"), wxDefaultPosition, wxDefaultSize, 0, -5000, 5000, 0, _T("ID_SPINCTRL2"));
    SpinCtrl2->SetValue(_T("0"));
    FlexGridSizer3->Add(SpinCtrl2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText11 = new wxStaticText(Panel1, ID_STATICTEXT11, _("WMO Y-Offset"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT11"));
    StaticText11->SetToolTip(_("This changes the Y-Offset of all WMOs (World Map Objects).  Do not change this value unless you know what you\'re doing."));
    StaticText11->SetHelpText(_("This changes the Y-Offset of all WMOs (World Map Objects).  Do not change this value unless you know what you\'re doing."));
    FlexGridSizer3->Add(StaticText11, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    SpinCtrl3 = new wxSpinCtrl(Panel1, ID_SPINCTRL3, _T("0"), wxDefaultPosition, wxDefaultSize, 0, -5000, 5000, 0, _T("ID_SPINCTRL3"));
    SpinCtrl3->SetValue(_T("0"));
    FlexGridSizer3->Add(SpinCtrl3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText12 = new wxStaticText(Panel1, ID_STATICTEXT12, _("WMO Z-Offset"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT12"));
    StaticText12->SetToolTip(_("This changes the Z-Offset of all WMOs (World Map Objects).  Do not change this value unless you know what you\'re doing."));
    StaticText12->SetHelpText(_("This changes the Z-Offset of all WMOs (World Map Objects).  Do not change this value unless you know what you\'re doing."));
    FlexGridSizer3->Add(StaticText12, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    SpinCtrl4 = new wxSpinCtrl(Panel1, ID_SPINCTRL4, _T("0"), wxDefaultPosition, wxDefaultSize, 0, -5000, 5000, 0, _T("ID_SPINCTRL4"));
    SpinCtrl4->SetValue(_T("0"));
    FlexGridSizer3->Add(SpinCtrl4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticBoxSizer1->Add(FlexGridSizer3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer1->Add(StaticBoxSizer1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticBoxSizer2 = new wxStaticBoxSizer(wxHORIZONTAL, Panel1, _("New Map Name"));
    TextCtrl1 = new wxTextCtrl(Panel1, ID_TEXTCTRL1, _("Enter New Map Name Here"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    StaticBoxSizer2->Add(TextCtrl1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer1->Add(StaticBoxSizer2, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Button1 = new wxButton(Panel1, ID_BUTTON1, _("Mash!"), wxDefaultPosition, wxSize(186,39), 0, wxDefaultValidator, _T("ID_BUTTON1"));
    FlexGridSizer1->Add(Button1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticBoxSizer3 = new wxStaticBoxSizer(wxHORIZONTAL, Panel1, _("Detailed Map Information"));
    FlexGridSizer2 = new wxFlexGridSizer(0, 2, 0, 0);
    StaticText1 = new wxStaticText(Panel1, ID_STATICTEXT1, _("Current Mouse Position:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    FlexGridSizer2->Add(StaticText1, 2, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
    StaticText2 = new wxStaticText(Panel1, ID_STATICTEXT2, _("(--, --)"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    FlexGridSizer2->Add(StaticText2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText3 = new wxStaticText(Panel1, ID_STATICTEXT3, _("Current ADT Group Selected:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    FlexGridSizer2->Add(StaticText3, 2, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
    StaticText4 = new wxStaticText(Panel1, ID_STATICTEXT4, _("--"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
    FlexGridSizer2->Add(StaticText4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText5 = new wxStaticText(Panel1, ID_STATICTEXT5, _("Number of ADT Groups:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
    FlexGridSizer2->Add(StaticText5, 2, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
    StaticText6 = new wxStaticText(Panel1, ID_STATICTEXT6, _("0"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT6"));
    FlexGridSizer2->Add(StaticText6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText7 = new wxStaticText(Panel1, ID_STATICTEXT7, _("Number of ADTs:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT7"));
    FlexGridSizer2->Add(StaticText7, 2, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
    StaticText8 = new wxStaticText(Panel1, ID_STATICTEXT8, _("0"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT8"));
    FlexGridSizer2->Add(StaticText8, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticBoxSizer3->Add(FlexGridSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer1->Add(StaticBoxSizer3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer2->Add(FlexGridSizer1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Panel1->SetSizer(BoxSizer2);
    BoxSizer2->Fit(Panel1);
    BoxSizer2->SetSizeHints(Panel1);
    BoxSizer1->Add(Panel1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    SetSizer(BoxSizer1);
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    MenuItem3 = new wxMenuItem(Menu1, ID_MENUITEM1, _("Add File..."), _("Add an ADT file to the application"), wxITEM_NORMAL);
    MenuItem3->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_FILE_OPEN")),wxART_OTHER));
    Menu1->Append(MenuItem3);
    MenuItem4 = new wxMenuItem(Menu1, ID_MENUITEM2, _("Add Folder..."), _("Add a folder containing ADT files to the application"), wxITEM_NORMAL);
    MenuItem4->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_FOLDER_OPEN")),wxART_OTHER));
    Menu1->Append(MenuItem4);
    Menu1->AppendSeparator();
    MenuItem1 = new wxMenuItem(Menu1, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    MenuItem1->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_CROSS_MARK")),wxART_OTHER));
    Menu1->Append(MenuItem1);
    MenuBar1->Append(Menu1, _("&File"));
    Menu2 = new wxMenu();
    MenuItem2 = new wxMenuItem(Menu2, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    MenuItem2->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_HELP_FOLDER")),wxART_OTHER));
    Menu2->Append(MenuItem2);
    MenuBar1->Append(Menu2, _("Help"));
    SetMenuBar(MenuBar1);
    StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBar1->SetFieldsCount(1,__wxStatusBarWidths_1);
    StatusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(StatusBar1);
    BoxSizer1->SetSizeHints(this);

    Panel2->Connect(wxEVT_LEFT_DOWN,(wxObjectEventFunction)&wxTestFrame::OnPanel2LeftDown,0,this);
    Panel2->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&wxTestFrame::OnPanel2LeftUp,0,this);
    Panel2->Connect(wxEVT_MOTION,(wxObjectEventFunction)&wxTestFrame::OnPanel2MouseMove,0,this);
    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxTestFrame::OnButton1Click);
    Panel1->Connect(wxEVT_MOTION,(wxObjectEventFunction)&wxTestFrame::OnPanel1MouseMove,0,this);
    Connect(ID_MENUITEM1,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxTestFrame::OnOpenFile);
    Connect(ID_MENUITEM2,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxTestFrame::OnOpenFolder);
    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxTestFrame::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxTestFrame::OnAbout);
    //*)
}

wxTestFrame::~wxTestFrame()
{
    //(*Destroy(wxTestFrame)
    //*)
}

void wxTestFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void wxTestFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = _("Created By: Ryan Cornwall\nVersion: 0.0.4.8\nLast Updated On: 7/23/2010");//wxbuildinfo(long_f);
    wxMessageBox(msg, _("Riu's Zone Masher"));
}

void wxTestFrame::OnClose(wxCloseEvent& event)
{
}

void wxTestFrame::OnOpenFile(wxCommandEvent& event)
{
	wxFileDialog* OpenDialog = new wxFileDialog(
		this, _("Choose a file to open"), wxEmptyString, wxEmptyString,
		_("ADT Files (*.adt)|*.adt"),
		wxFD_OPEN | wxFD_MULTIPLE, wxDefaultPosition);

	// Create an "open file" dialog for ADT files
	if (OpenDialog->ShowModal() == wxID_OK) // if the user clicks "Open" instead of "Cancel"
	{
	    wxProgressDialog* pDialog = new wxProgressDialog( _("Opening Files"), _("Opening:                                                                                       "));
	    //Append the file(s) to its/their own little group
		ZoneGroup newZoneGroup;
		zoneGroupList.pushBack(newZoneGroup);
		wxArrayString paths;
		OpenDialog->GetPaths(paths);
		for(int i=0; i < paths.GetCount(); i++){
		    pDialog->Update(int(100.0*i/paths.GetCount()), _("Opening: ")+paths[i]);
            zoneGroupList[zoneGroupList.getSize()-1].append(std::string(paths[i].mb_str()));
            numberOfAdts++;
		}
		pDialog->Update(100, _("Finished opening files."));
		numberOfAdtGroups++;
        Panel2->updateMatrixOverlay(zoneGroupList[zoneGroupList.getSize()-1], zoneGroupList.getSize()-1);

        //If the current map name is the default, change it to the map name that was just added.
		if(TextCtrl1->GetValue() == _("Enter New Map Name Here")){
		    wxString wxTempStr(zoneGroupList[zoneGroupList.getSize()-1].getOnlyMapName().c_str(), wxConvUTF8);
		    TextCtrl1->SetValue(wxTempStr);
		}

		pDialog->Destroy();
	}

	//Update the gridPanel
    Panel2->setMatrix(zoneGroupList);
    Panel2->refresh();

    //Update Detailed Map Information
    StaticText6->SetLabel(wxString::Format(wxT("%i"),numberOfAdtGroups));
    StaticText8->SetLabel(wxString::Format(wxT("%i"),numberOfAdts));

	// Clean up after ourselves
	OpenDialog->Destroy();
}

void wxTestFrame::OnOpenFolder(wxCommandEvent& event)
{
    wxDirDialog* OpenDialog = new wxDirDialog(
        this,_("Choose a folder/directory to open"));

    if (OpenDialog->ShowModal() == wxID_OK)
    {
        //Load in the folder's ADT files
        ZoneGroup newZoneGroup;
        list<std::string> tempStrList;
        zoneGroupList.pushBack(newZoneGroup);

        std::string directoryFile;
        DIR *directory;
        struct dirent *dirp; //Directory pointer
        struct stat filestat;
        directory = opendir( OpenDialog->GetPath().mb_str()); //Attempts to open directory
        if(directory == NULL){;} //If directory failed to open, do nothing
        else{
            while(dirp = readdir(directory)){
                directoryFile = std::string(OpenDialog->GetPath().mb_str()) + "\\" + dirp->d_name;
                //We want to skip sub-directories and invalid names
                if(directoryFile.find(".adt")==std::string::npos){;} //Do nothing.  Not an ADT
                else if(stat(directoryFile.c_str(), &filestat)){;}
                else if(S_ISDIR( filestat.st_mode)){;}
                else{
                    tempStrList.pushBack(directoryFile);
                    numberOfAdts++;
                }
            }
            if(tempStrList.getSize()){
                numberOfAdtGroups++;
                zoneGroupList[zoneGroupList.getSize()-1].append(tempStrList);
                Panel2->updateMatrixOverlay(zoneGroupList[zoneGroupList.getSize()-1], zoneGroupList.getSize()-1);

                //If the current map name is the default, change it to the map name that was just added.
                if(TextCtrl1->GetValue() == _("Enter New Map Name Here")){
                    wxString wxTempStr(zoneGroupList[zoneGroupList.getSize()-1].getOnlyMapName().c_str(), wxConvUTF8);
                    TextCtrl1->SetValue(wxTempStr);
                }

                Panel2->setMatrix(zoneGroupList);
                Panel2->refresh();
            }
            StaticText6->SetLabel(wxString::Format(wxT("%i"),numberOfAdtGroups));
            StaticText8->SetLabel(wxString::Format(wxT("%i"),numberOfAdts));
        }
        delete directory;
    }

    //We don't want any memory leaks
    OpenDialog->Destroy();
}

void wxTestFrame::OnPanel2LeftDown(wxMouseEvent& event)
{
    long x = event.GetX();
    long y = event.GetY();
    int tempZoneOverlay;

    wxSize size = GetClientSize();
    int gridTop = size.GetHeight() - Panel2->getGridY() * Panel2->getCellY();

    y -= (gridTop-9); //The 9 is a kludge until I figure out why the numbers are off

    x /= Panel2->getCellX();
    y /= Panel2->getCellY();

    //Some bounds checking to prevent "weird" results
    if(x < 0) { x=-1;y=-1; }
    if(y < 0) { x=-1;y=-1; }
    if(x >= Panel2->getGridX()){ x=-1;y=-1; }
    if(y >= Panel2->getGridY()){ x=-1;y=-1; }

    if(x>=0 && y>=0){
        tempZoneOverlay=Panel2->getZoneOverlay(x,y)+1;
        if(tempZoneOverlay){
            StaticText4->SetLabel(wxString::Format(wxT("%i"),tempZoneOverlay));
            if(event.m_leftDown){
                Panel2->setIsDragging(true);
                Panel2->setDragX(x);
                Panel2->setDragY(y);
                Panel2->setOffX(0);
                Panel2->setOffY(0);
            }
        }
        else StaticText4->SetLabel(_("--"));
    }
}

void wxTestFrame::OnPanel2MouseMove(wxMouseEvent& event)
{
    long x = event.GetX();
    long y = event.GetY();

    wxSize size = GetClientSize();
    int gridTop = size.GetHeight() - Panel2->getGridY() * Panel2->getCellY();

    y -= (gridTop-9); //The 9 is a kludge until I figure out why the numbers are off

    x /= Panel2->getCellX();
    y /= Panel2->getCellY();

    //Some bounds checking to prevent "weird" results
    if(x < 0) { x=-1;y=-1; }
    if(y < 0) { x=-1;y=-1; }
    if(x >= Panel2->getGridX()){ x=-1;y=-1; }
    if(y >= Panel2->getGridY()){ x=-1;y=-1; }

    if(x>=0 && y>=0){
        StaticText2->SetLabel(_("(") + wxString::Format(wxT("%i"),x) + _(", ") + wxString::Format(wxT("%i"),y) + _(")"));

        //If user is dragging a group
        if(Panel2->getIsDragging()){
            int oldX = Panel2->getDragX();
            int oldY = Panel2->getDragY();
            int offX = x-oldX;
            int offY = y-oldY;

            if((offX || offY) && Panel2->getZoneOverlay(oldX,oldY) >= 0 &&
                zoneGroupList[Panel2->getZoneOverlay(oldX,oldY)].getX() + offX >= 0 &&
                zoneGroupList[Panel2->getZoneOverlay(oldX,oldY)].getY() + offY >= 0 &&
                zoneGroupList[Panel2->getZoneOverlay(oldX,oldY)].getX() + zoneGroupList[Panel2->getZoneOverlay(oldX,oldY)].getW() + offX < Panel2->getGridX() &&
                zoneGroupList[Panel2->getZoneOverlay(oldX,oldY)].getY() + zoneGroupList[Panel2->getZoneOverlay(oldX,oldY)].getH() + offY < Panel2->getGridY()){
                Panel2->setMatrix(zoneGroupList, Panel2->getZoneOverlay(oldX,oldY), offX, offY);
                Panel2->setOffX(offX);
                Panel2->setOffY(offY);
                Panel2->refresh();
            }
        }
        //else Panel2->setIsDragging(false);
    }
    else StaticText2->SetLabel(_("(--, --)"));
}

void wxTestFrame::OnPanel1MouseMove(wxMouseEvent& event)
{
    //TODO: Fix this stuff!  The borders are way off.
    //This whole part of the code is very "kludgy."
    long x, y;
    x = event.GetX();
    y = event.GetY();

    wxSize size = GetClientSize();
    int gridTop = size.GetHeight() - Panel2->getGridY() * Panel2->getCellY();

    x -= 5; //-5 because of the border
    y -= (gridTop-4); //The 4 is a kludge until I figure out why the numbers are off

    x /= Panel2->getCellX();
    y /= Panel2->getCellY();

    if(x > 63 || y > 63 || x < 0 || y < 0) StaticText2->SetLabel(_("(--, --)"));
}

void wxTestFrame::OnPanel2LeftUp(wxMouseEvent& event)
{
    if(Panel2->getIsDragging()){
        Panel2->setIsDragging(false);
        int zGroupIndex = Panel2->getZoneOverlay(Panel2->getDragX(),Panel2->getDragY());
        for(int i=0; i < zoneGroupList[zGroupIndex].getSize(); i++){
            Panel2->pushZoneOverlay(zGroupIndex, zoneGroupList[zGroupIndex].getAdtX(i)+Panel2->getOffX(), zoneGroupList[zGroupIndex].getAdtY(i)+Panel2->getOffY());
            Panel2->popZoneOverlay(zoneGroupList[zGroupIndex].getAdtX(i), zoneGroupList[zGroupIndex].getAdtY(i));
        }
        zoneGroupList[zGroupIndex].movePos(Panel2->getOffX(), Panel2->getOffY());
        Panel2->setMatrix(zoneGroupList);
        Panel2->setOffX(0);
        Panel2->setOffY(0);
        Panel2->setDragX(-1);
        Panel2->setDragY(-1);
    }
}

void wxTestFrame::OnButton1Click(wxCommandEvent& event)
{
    if(zoneGroupList.getSize() > 0){
        wxProgressDialog* OpenDialog = new wxProgressDialog( _("Mash Progress"), _("Currently mashing the map.  Please be patient."));
        mash(zoneGroupList, std::string(TextCtrl1->GetValue().mb_str()), CheckBox1->IsChecked(), CheckBox2->IsChecked(), OpenDialog, SpinCtrl1->GetValue(), SpinCtrl2->GetValue(), SpinCtrl3->GetValue(), SpinCtrl4->GetValue());
        OpenDialog->Destroy();
        wxString msg = _("The files appear to have been mashed successfully.");
        wxMessageBox(msg, _("Success!"));
    }
    else{
        wxString msg = _("You must include at least one file before mashing.");
        wxMessageBox(msg, _("Failure!"));
    }
}
