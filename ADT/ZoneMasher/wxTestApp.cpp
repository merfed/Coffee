/***************************************************************
 * Name:      wxTestApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Ryan Cornwall ()
 * Created:   2010-07-15
 * Copyright: Ryan Cornwall ()
 * License:
 **************************************************************/

#include "wx_pch.h"
#include "wxTestApp.h"

//(*AppHeaders
#include "wxTestMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(wxTestApp);

bool wxTestApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	wxTestFrame* Frame = new wxTestFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
