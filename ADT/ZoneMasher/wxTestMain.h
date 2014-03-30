/***************************************************************
 * Name:      wxTestMain.h
 * Purpose:   Defines Application Frame
 * Author:    Ryan Cornwall ()
 * Created:   2010-07-15
 * Copyright: Ryan Cornwall ()
 * License:
 **************************************************************/

#ifndef WXTESTMAIN_H
#define WXTESTMAIN_H

//(*Headers(wxTestFrame)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/menu.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/spinctrl.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/statusbr.h>
//*)
#include <wx/progdlg.h>
#include "gridPanel.h"
#include "list.h"
#include "zoneGroup.h"

class wxTestFrame: public wxFrame
{
    public:

        wxTestFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~wxTestFrame();

    private:

        //(*Handlers(wxTestFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnClose(wxCloseEvent& event);
        void OnOpenFile(wxCommandEvent& event);
        void OnOpenFolder(wxCommandEvent& event);
        void OnPanel2LeftDClick(wxMouseEvent& event);
        void OnPanel2LeftDown(wxMouseEvent& event);
        void OnPanel2MouseMove(wxMouseEvent& event);
        void OnPanel1MouseMove(wxMouseEvent& event);
        void OnPanel2LeftUp(wxMouseEvent& event);
        void OnButton1Click(wxCommandEvent& event);
        //*)

        //(*Identifiers(wxTestFrame)
        static const long ID_PANEL2;
        static const long ID_CHECKBOX1;
        static const long ID_CHECKBOX2;
        static const long ID_STATICTEXT9;
        static const long ID_SPINCTRL1;
        static const long ID_STATICTEXT10;
        static const long ID_SPINCTRL2;
        static const long ID_STATICTEXT11;
        static const long ID_SPINCTRL3;
        static const long ID_STATICTEXT12;
        static const long ID_SPINCTRL4;
        static const long ID_TEXTCTRL1;
        static const long ID_BUTTON1;
        static const long ID_STATICTEXT1;
        static const long ID_STATICTEXT2;
        static const long ID_STATICTEXT3;
        static const long ID_STATICTEXT4;
        static const long ID_STATICTEXT5;
        static const long ID_STATICTEXT6;
        static const long ID_STATICTEXT7;
        static const long ID_STATICTEXT8;
        static const long ID_PANEL1;
        static const long ID_MENUITEM1;
        static const long ID_MENUITEM2;
        static const long idMenuQuit;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        //*)

        int numberOfAdtGroups;
        int numberOfAdts;

        list<ZoneGroup> zoneGroupList;

        //(*Declarations(wxTestFrame)
        wxStaticText* StaticText10;
        wxStaticText* StaticText9;
        wxSpinCtrl* SpinCtrl4;
        wxSpinCtrl* SpinCtrl1;
        GridPanel* Panel2;
        wxStaticText* StaticText2;
        wxButton* Button1;
        wxStaticText* StaticText6;
        wxStaticText* StaticText8;
        wxStaticText* StaticText11;
        wxCheckBox* CheckBox2;
        wxPanel* Panel1;
        wxStaticText* StaticText1;
        wxStaticText* StaticText3;
        wxCheckBox* CheckBox1;
        wxSpinCtrl* SpinCtrl3;
        wxStaticText* StaticText5;
        wxStaticText* StaticText7;
        wxStatusBar* StatusBar1;
        wxSpinCtrl* SpinCtrl2;
        wxTextCtrl* TextCtrl1;
        wxStaticText* StaticText12;
        wxStaticText* StaticText4;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // WXTESTMAIN_H
