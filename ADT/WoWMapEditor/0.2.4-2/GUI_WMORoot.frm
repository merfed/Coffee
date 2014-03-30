VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Begin VB.Form GUI_WMORoot 
   Caption         =   "WoW .wmo Object Editor (by SomeWhiteGuy)"
   ClientHeight    =   4740
   ClientLeft      =   165
   ClientTop       =   855
   ClientWidth     =   10965
   LinkTopic       =   "Form2"
   MaxButton       =   0   'False
   ScaleHeight     =   4740
   ScaleWidth      =   10965
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton btnSave 
      Caption         =   "Save"
      Height          =   375
      Left            =   9480
      TabIndex        =   4
      Top             =   4200
      Width           =   1335
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Other Opts"
      Height          =   375
      Left            =   9480
      TabIndex        =   3
      Top             =   600
      Width           =   1335
   End
   Begin MSComctlLib.ListView MOMTListView 
      Height          =   3975
      Left            =   240
      TabIndex        =   0
      Top             =   600
      Width           =   9135
      _ExtentX        =   16113
      _ExtentY        =   7011
      View            =   3
      LabelWrap       =   -1  'True
      HideSelection   =   -1  'True
      _Version        =   393217
      ForeColor       =   -2147483640
      BackColor       =   -2147483643
      BorderStyle     =   1
      Appearance      =   1
      NumItems        =   4
      BeginProperty ColumnHeader(1) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         Text            =   "id"
         Object.Width           =   671
      EndProperty
      BeginProperty ColumnHeader(2) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   1
         Text            =   "Flags"
         Object.Width           =   952
      EndProperty
      BeginProperty ColumnHeader(3) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   2
         Text            =   "Blend Mode"
         Object.Width           =   776
      EndProperty
      BeginProperty ColumnHeader(4) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   3
         Text            =   ".blp file"
         Object.Width           =   13124
      EndProperty
   End
   Begin MSComctlLib.Toolbar Toolbar1 
      Height          =   330
      Left            =   240
      TabIndex        =   2
      Top             =   240
      Width           =   4815
      _ExtentX        =   8493
      _ExtentY        =   582
      ButtonWidth     =   3572
      ButtonHeight    =   582
      Style           =   1
      TextAlignment   =   1
      _Version        =   393216
      BeginProperty Buttons {66833FE8-8583-11D1-B16A-00C0F0283628} 
         NumButtons      =   2
         BeginProperty Button1 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Caption         =   "Doodads (Simple Objs)"
         EndProperty
         BeginProperty Button2 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Caption         =   "Materials (2D bitmaps)"
         EndProperty
      EndProperty
   End
   Begin MSComctlLib.ListView DoodadListView 
      Height          =   3975
      Left            =   240
      TabIndex        =   1
      Top             =   600
      Width           =   9135
      _ExtentX        =   16113
      _ExtentY        =   7011
      View            =   3
      LabelWrap       =   -1  'True
      HideSelection   =   -1  'True
      FullRowSelect   =   -1  'True
      _Version        =   393217
      ForeColor       =   -2147483640
      BackColor       =   -2147483643
      BorderStyle     =   1
      Appearance      =   1
      NumItems        =   5
      BeginProperty ColumnHeader(1) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         Text            =   "id"
         Object.Width           =   2540
      EndProperty
      BeginProperty ColumnHeader(2) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   1
         Text            =   "File"
         Object.Width           =   2540
      EndProperty
      BeginProperty ColumnHeader(3) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   2
         Text            =   "X"
         Object.Width           =   2540
      EndProperty
      BeginProperty ColumnHeader(4) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   3
         Text            =   "Y"
         Object.Width           =   2540
      EndProperty
      BeginProperty ColumnHeader(5) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   4
         Text            =   "Z"
         Object.Width           =   2540
      EndProperty
   End
   Begin VB.Menu Mnu_ 
      Caption         =   "Mnu"
      Begin VB.Menu Mnu_KillAllDoodads 
         Caption         =   "Kill all doodads"
      End
   End
End
Attribute VB_Name = "GUI_WMORoot"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Public WMORoot As New WMORootFile
Public TargWMOPath As String

Private Sub btnSave_Click()
Dim s As String
    
    s = InputBox("Where would you like to save this file to?", , TargWMOPath)
    If s = "" Then Exit Sub
    
    WMORoot.SaveWMORootFile s
    
    MsgBox "File saved", vbInformation
    
End Sub

Private Sub Command1_Click()
    
    PopupMenu Mnu_
    
End Sub

Private Sub Form_Load()
Dim i As Integer
        
    Mnu_.Visible = False
        
    MsgBox "You have opened a WMO-root file.  The GUI for this is unfinnished." & vbCrLf & "If this isn't what you expected, you probably ment to open a group-file, this will be named the same way the root file is, only it will have _001 or greater in it.", vbInformation
    
    TargWMOPath = GUIStart.TargFile
    WMORoot.OpendWMORootFile TargWMOPath

    For i = 1 To WMORoot.Materials_Count
        
        With MOMTListView.ListItems
        .Add .Count + 1, , i
        .Item(.Count).SubItems(1) = WMORoot.Materials_Flag(i)
        .Item(.Count).SubItems(2) = WMORoot.Materials_BlendMode(i)
        .Item(.Count).SubItems(3) = WMORoot.Materials_Path(i)
        End With
        
    Next
    
    For i = 1 To WMORoot.DoodadCount
        
        With DoodadListView.ListItems
        .Add .Count + 1, , i
        .Item(.Count).SubItems(1) = WMORoot.DoodadPath(i)
        .Item(.Count).SubItems(2) = WMORoot.DoodadX(i)
        .Item(.Count).SubItems(3) = WMORoot.DoodadY(i)
        .Item(.Count).SubItems(4) = WMORoot.DoodadZ(i)
        End With
        
    Next
    
    
End Sub

Private Sub Mnu_KillAllDoodads_Click()
Dim X As Long

    Load GUIShowMsg
    GUIShowMsg.Show
    
    GUIShowMsg.Bar1.Max = WMORoot.DoodadCount
    
    For X = 1 To WMORoot.DoodadCount
        
        WMORoot.DoodadZ(X) = -800
        
        GUIShowMsg.Bar1.Value = X
        GUIShowMsg.Bar1.Refresh
        GUIShowMsg.Refresh
        
    Next
    
    Unload GUIShowMsg

End Sub

Private Sub Toolbar1_ButtonClick(ByVal Button As MSComctlLib.Button)
    
    DoodadListView.Visible = False
    MOMTListView.Visible = False
    
    Select Case Button.Index
    Case 1:     DoodadListView.Visible = True
    Case 2:     MOMTListView.Visible = True
    End Select
    
End Sub
