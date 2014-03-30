VERSION 5.00
Begin VB.Form GUIAbout 
   Caption         =   "WoW Map Edittor - Help"
   ClientHeight    =   3945
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   7245
   ControlBox      =   0   'False
   LinkTopic       =   "Form2"
   ScaleHeight     =   3945
   ScaleWidth      =   7245
   StartUpPosition =   2  'CenterScreen
   Begin VB.CommandButton Command1 
      Caption         =   "Ok"
      Height          =   375
      Left            =   5640
      TabIndex        =   1
      Top             =   3480
      Width           =   1455
   End
   Begin VB.TextBox txtAbout 
      Height          =   3255
      Left            =   120
      Locked          =   -1  'True
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertical
      TabIndex        =   0
      Text            =   "About.frx":0000
      Top             =   120
      Width           =   6975
   End
   Begin VB.TextBox txtLocHelpABC 
      Height          =   3255
      Left            =   120
      Locked          =   -1  'True
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertical
      TabIndex        =   3
      Text            =   "About.frx":00CB
      Top             =   120
      Width           =   6975
   End
   Begin VB.TextBox txtLocHelpXYZ 
      Height          =   3255
      Left            =   120
      Locked          =   -1  'True
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertical
      TabIndex        =   2
      Text            =   "About.frx":02F9
      Top             =   120
      Width           =   6975
   End
   Begin VB.TextBox txtBlankTextBox 
      Height          =   3255
      Left            =   120
      Locked          =   -1  'True
      MultiLine       =   -1  'True
      ScrollBars      =   3  'Both
      TabIndex        =   6
      Top             =   120
      Visible         =   0   'False
      Width           =   6975
   End
   Begin VB.TextBox txtAccessDenied 
      Height          =   3255
      Left            =   120
      Locked          =   -1  'True
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertical
      TabIndex        =   5
      Text            =   "About.frx":0429
      Top             =   120
      Width           =   6975
   End
   Begin VB.TextBox txtNoCheatPlz 
      Height          =   3255
      Left            =   120
      Locked          =   -1  'True
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertical
      TabIndex        =   4
      Text            =   "About.frx":0674
      Top             =   120
      Width           =   6975
   End
End
Attribute VB_Name = "GUIAbout"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Command1_Click()
    Unload Me
End Sub

Private Sub Form_Load()
    
    txtAbout.Text = "WoW Map and .WMO Editor" & vbCrLf & "version: " & App.Major & "." & App.Minor & "." & App.Revision & vbCrLf & vbCrLf & txtAbout.Text
    
End Sub

Sub ShowText_Text()
    HideAll
    txtAbout.Visible = True
End Sub

Sub ShowText_HelpXYZ()
    HideAll
    txtLocHelpXYZ.Visible = True
End Sub

Sub ShowText_HelpABC()
    HideAll
    txtLocHelpABC.Visible = True
End Sub

Sub ShowText_DontCheatNoob()
    HideAll
    txtNoCheatPlz.Visible = True
End Sub

Sub ShowText_AccessDenied()
    HideAll
    txtAccessDenied.Visible = True
End Sub

Sub ShowText_BlankText()
    HideAll
    txtBlankTextBox.Visible = True
End Sub

Sub HideAll()

    txtAbout.Visible = False
    txtLocHelpABC.Visible = False
    txtLocHelpXYZ.Visible = False
    txtNoCheatPlz.Visible = False
    txtAccessDenied.Visible = False
    txtBlankTextBox.Visible = False

End Sub
