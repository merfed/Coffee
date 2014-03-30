VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Begin VB.Form GUI_ADTFile 
   Caption         =   "WoW .adt Map Editor $Ver (by SomeWhiteGuy)"
   ClientHeight    =   7005
   ClientLeft      =   165
   ClientTop       =   855
   ClientWidth     =   14910
   Icon            =   "GUI_ADTFile.frx":0000
   LinkTopic       =   "Form2"
   MaxButton       =   0   'False
   ScaleHeight     =   7005
   ScaleWidth      =   14910
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton Command1 
      Caption         =   "Height-Map Opts"
      Height          =   375
      Left            =   13200
      TabIndex        =   10
      Top             =   1440
      Width           =   1575
   End
   Begin VB.CommandButton btnOtherOpts 
      Caption         =   "Other Opts"
      Height          =   375
      Left            =   13200
      TabIndex        =   8
      Top             =   1920
      Width           =   1575
   End
   Begin VB.CheckBox chkRoundNbrs 
      Caption         =   "Round shown numbers to 2 decimal places"
      Height          =   255
      Left            =   9480
      TabIndex        =   7
      Top             =   6600
      Value           =   1  'Checked
      Width           =   3615
   End
   Begin MSComctlLib.Toolbar Toolbar1 
      Height          =   330
      Left            =   120
      TabIndex        =   4
      Top             =   120
      Width           =   9375
      _ExtentX        =   16536
      _ExtentY        =   582
      ButtonWidth     =   3572
      ButtonHeight    =   582
      Style           =   1
      TextAlignment   =   1
      _Version        =   393216
      BeginProperty Buttons {66833FE8-8583-11D1-B16A-00C0F0283628} 
         NumButtons      =   4
         BeginProperty Button1 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Caption         =   "Doodads (Simple Objs)"
         EndProperty
         BeginProperty Button2 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Caption         =   ".wmo (complex objs)"
         EndProperty
         BeginProperty Button3 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Caption         =   "MCNK chunks"
         EndProperty
         BeginProperty Button4 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Caption         =   "About"
         EndProperty
      EndProperty
   End
   Begin VB.CommandButton BtnPreviewXY 
      Caption         =   "View Map"
      Height          =   375
      Left            =   13200
      TabIndex        =   3
      Top             =   960
      Width           =   1575
   End
   Begin VB.CommandButton BtnSave 
      Caption         =   "Save .adt As.."
      Height          =   375
      Left            =   13200
      TabIndex        =   2
      Top             =   2400
      Width           =   1575
   End
   Begin VB.CommandButton btnEditSel 
      Caption         =   "Edit Selected"
      Height          =   375
      Left            =   13200
      TabIndex        =   1
      Top             =   480
      Width           =   1575
   End
   Begin MSComctlLib.ListView ListView1 
      Height          =   6015
      Left            =   120
      TabIndex        =   0
      Top             =   480
      Width           =   12975
      _ExtentX        =   22886
      _ExtentY        =   10610
      View            =   3
      LabelEdit       =   1
      MultiSelect     =   -1  'True
      LabelWrap       =   -1  'True
      HideSelection   =   -1  'True
      FullRowSelect   =   -1  'True
      _Version        =   393217
      ForeColor       =   -2147483640
      BackColor       =   -2147483643
      BorderStyle     =   1
      Appearance      =   1
      NumItems        =   9
      BeginProperty ColumnHeader(1) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         Text            =   "#"
         Object.Width           =   2540
      EndProperty
      BeginProperty ColumnHeader(2) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   1
         Text            =   "X"
         Object.Width           =   2540
      EndProperty
      BeginProperty ColumnHeader(3) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   2
         Text            =   "Y"
         Object.Width           =   2540
      EndProperty
      BeginProperty ColumnHeader(4) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   3
         Text            =   "Z"
         Object.Width           =   2540
      EndProperty
      BeginProperty ColumnHeader(5) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   4
         Text            =   "A"
         Object.Width           =   2540
      EndProperty
      BeginProperty ColumnHeader(6) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   5
         Text            =   "B"
         Object.Width           =   2540
      EndProperty
      BeginProperty ColumnHeader(7) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   6
         Text            =   "C"
         Object.Width           =   2540
      EndProperty
      BeginProperty ColumnHeader(8) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   7
         Text            =   "Size"
         Object.Width           =   2540
      EndProperty
      BeginProperty ColumnHeader(9) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   8
         Text            =   "Flags"
         Object.Width           =   2540
      EndProperty
   End
   Begin MSComctlLib.ListView ChunkView 
      Height          =   6015
      Left            =   120
      TabIndex        =   9
      Top             =   480
      Visible         =   0   'False
      Width           =   12975
      _ExtentX        =   22886
      _ExtentY        =   10610
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
         Text            =   "X Id"
         Object.Width           =   2540
      EndProperty
      BeginProperty ColumnHeader(3) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   2
         Text            =   "Y Id"
         Object.Width           =   2540
      EndProperty
      BeginProperty ColumnHeader(4) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   3
         Text            =   "Area ID"
         Object.Width           =   2540
      EndProperty
      BeginProperty ColumnHeader(5) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   4
         Text            =   "Area Name"
         Object.Width           =   2540
      EndProperty
   End
   Begin MSComctlLib.ListView WMOListView 
      Height          =   6015
      Left            =   120
      TabIndex        =   5
      Top             =   480
      Width           =   12975
      _ExtentX        =   22886
      _ExtentY        =   10610
      View            =   3
      LabelWrap       =   -1  'True
      HideSelection   =   -1  'True
      FullRowSelect   =   -1  'True
      _Version        =   393217
      ForeColor       =   -2147483640
      BackColor       =   -2147483643
      BorderStyle     =   1
      Appearance      =   1
      NumItems        =   17
      BeginProperty ColumnHeader(1) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         Text            =   "File Name"
         Object.Width           =   2540
      EndProperty
      BeginProperty ColumnHeader(2) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   1
         Text            =   "X"
         Object.Width           =   2540
      EndProperty
      BeginProperty ColumnHeader(3) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   2
         Text            =   "Y"
         Object.Width           =   2540
      EndProperty
      BeginProperty ColumnHeader(4) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   3
         Text            =   "Z"
         Object.Width           =   2540
      EndProperty
      BeginProperty ColumnHeader(5) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   4
         Text            =   "A"
         Object.Width           =   2540
      EndProperty
      BeginProperty ColumnHeader(6) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   5
         Text            =   "B"
         Object.Width           =   2540
      EndProperty
      BeginProperty ColumnHeader(7) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   6
         Text            =   "C"
         Object.Width           =   2540
      EndProperty
      BeginProperty ColumnHeader(8) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   7
         Text            =   "Unknown"
         Object.Width           =   2540
      EndProperty
      BeginProperty ColumnHeader(9) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   8
         Text            =   "Unknown"
         Object.Width           =   2540
      EndProperty
      BeginProperty ColumnHeader(10) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   9
         Text            =   "Unknown"
         Object.Width           =   2540
      EndProperty
      BeginProperty ColumnHeader(11) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   10
         Text            =   "Unknown"
         Object.Width           =   2540
      EndProperty
      BeginProperty ColumnHeader(12) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   11
         Text            =   "Unknown"
         Object.Width           =   2540
      EndProperty
      BeginProperty ColumnHeader(13) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   12
         Text            =   "Unknown"
         Object.Width           =   2540
      EndProperty
      BeginProperty ColumnHeader(14) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   13
         Text            =   "Flags"
         Object.Width           =   2540
      EndProperty
      BeginProperty ColumnHeader(15) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   14
         Text            =   "Doodad Set"
         Object.Width           =   2540
      EndProperty
      BeginProperty ColumnHeader(16) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   15
         Text            =   "Name Set"
         Object.Width           =   2540
      EndProperty
      BeginProperty ColumnHeader(17) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   16
         Text            =   "Special Id"
         Object.Width           =   2540
      EndProperty
   End
   Begin VB.Label lblFullPath 
      Caption         =   "Double click so view location on map, right click for options"
      Height          =   255
      Left            =   120
      TabIndex        =   6
      Top             =   6600
      Width           =   9255
   End
   Begin VB.Menu VataMnu_ 
      Caption         =   "VataMnu_"
      Begin VB.Menu VataMnu_Cpy1ChunkDec 
         Caption         =   "Copy (the outter 9x9) height-map of a single MNCK chunk (as dec values)"
      End
      Begin VB.Menu VataMnu_Cpy1ChunkDecBStr 
         Caption         =   "Copy (the outter 9x9) height-map of a single MNCK chunk (as byte-string values)"
      End
      Begin VB.Menu VataMnu_Cpy1ChunkHex 
         Caption         =   "Copy (the outter 9x9) height-map of a single MNCK chunk (as binary / raw data)"
      End
      Begin VB.Menu VataMnu_Dash 
         Caption         =   "-"
      End
      Begin VB.Menu VataMnu_CpyAllChunksDec 
         Caption         =   "Copy (the outter 9x9) height-map of all MNCK chunks (as dec values)"
      End
      Begin VB.Menu VataMnu_CpyAllChunks 
         Caption         =   "Copy (the outter 9x9) height-map of all MNCK chunks (as byte-string values)"
      End
      Begin VB.Menu VataMnu_CpyAllChunksHex 
         Caption         =   "Copy (the outter 9x9) height-map of all MNCK chunks (as binary / raw data)"
      End
      Begin VB.Menu VataMnu_Dash3 
         Caption         =   "-"
      End
      Begin VB.Menu VataMnu_PasteAllChnks 
         Caption         =   "Paste (outter 9x9) height map of all chunks"
      End
      Begin VB.Menu VataMnu_Dash2 
         Caption         =   "-"
      End
      Begin VB.Menu Mnu_ShiftLand 
         Caption         =   "ATTEMPT to shift land height"
      End
      Begin VB.Menu Mnu_test 
         Caption         =   "ATTEMPT to flatten land"
      End
      Begin VB.Menu Mnu_test2 
         Caption         =   " test dev thing"
      End
   End
   Begin VB.Menu MnuEdit_ 
      Caption         =   "MnuEdit_"
      Begin VB.Menu MnuEdit_Cng 
         Caption         =   "Change X"
         Index           =   0
      End
      Begin VB.Menu MnuEdit_Cng 
         Caption         =   "Change Y"
         Index           =   1
      End
      Begin VB.Menu MnuEdit_Cng 
         Caption         =   "Change Z"
         Index           =   2
      End
      Begin VB.Menu MnuEdit_Cng 
         Caption         =   "XYZ Help"
         Index           =   3
      End
      Begin VB.Menu MnuEdit_Cng 
         Caption         =   "-"
         Index           =   4
      End
      Begin VB.Menu MnuEdit_Cng 
         Caption         =   "Change Rotation A (see help)"
         Index           =   5
      End
      Begin VB.Menu MnuEdit_Cng 
         Caption         =   "Change Rotation B (see help)"
         Index           =   6
      End
      Begin VB.Menu MnuEdit_Cng 
         Caption         =   "Change Rotation C (see help)"
         Index           =   7
      End
      Begin VB.Menu MnuEdit_Cng 
         Caption         =   "ABC Help"
         Index           =   8
      End
      Begin VB.Menu MnuEdit_Dash2 
         Caption         =   "-"
      End
      Begin VB.Menu MnuEdit_CngDoodSize 
         Caption         =   "Change size"
      End
      Begin VB.Menu Mnu_MkDoodDrawnAllChnks 
         Caption         =   "Make doodad drawn in all chunks"
      End
      Begin VB.Menu MnuEdit_Dash 
         Caption         =   "-"
      End
      Begin VB.Menu MnuEdit_Cpy 
         Caption         =   "Copy XYZ to Clipboard"
      End
      Begin VB.Menu MnuEdit_Paste 
         Caption         =   "Pase XYZ into this item"
      End
   End
   Begin VB.Menu MnuEditWMO_ 
      Caption         =   "MnuEditWMO_"
      Begin VB.Menu MnuEditWMO_Chng 
         Caption         =   "Change X"
         Index           =   0
      End
      Begin VB.Menu MnuEditWMO_Chng 
         Caption         =   "Change Y"
         Index           =   1
      End
      Begin VB.Menu MnuEditWMO_Chng 
         Caption         =   "Change Z"
         Index           =   2
      End
      Begin VB.Menu MnuEditWMO_Chng 
         Caption         =   "XYZ Help"
         Index           =   3
      End
      Begin VB.Menu MnuEditWMO_Chng 
         Caption         =   "-"
         Index           =   4
      End
      Begin VB.Menu MnuEditWMO_Chng 
         Caption         =   "Change Rotation A (see help)"
         Index           =   5
      End
      Begin VB.Menu MnuEditWMO_Chng 
         Caption         =   "Change Rotation B (see help)"
         Index           =   6
      End
      Begin VB.Menu MnuEditWMO_Chng 
         Caption         =   "Change Rotation C (see help)"
         Index           =   7
      End
      Begin VB.Menu MnuEditWMO_Chng 
         Caption         =   "ABC Help"
         Index           =   8
      End
      Begin VB.Menu MnuEditWMO_Chng 
         Caption         =   "-"
         Index           =   9
      End
      Begin VB.Menu Mnu_CngWMOUnknown 
         Caption         =   "Change Unknown Flag (X pos2?)"
         Index           =   0
      End
      Begin VB.Menu Mnu_CngWMOUnknown 
         Caption         =   "Change Unknown Flag (Z pos2?)"
         Index           =   1
      End
      Begin VB.Menu Mnu_CngWMOUnknown 
         Caption         =   "Change Unknown Flag (Y pos2?)"
         Index           =   2
      End
      Begin VB.Menu Mnu_CngWMOUnknown 
         Caption         =   "Change Unknown Flag (X pos3?)"
         Index           =   3
      End
      Begin VB.Menu Mnu_CngWMOUnknown 
         Caption         =   "Change Unknown Flag (Z pos3?)"
         Index           =   4
      End
      Begin VB.Menu Mnu_CngWMOUnknown 
         Caption         =   "Change Unknown Flag (Y pos3?)"
         Index           =   5
      End
      Begin VB.Menu Mnu_Dahs7 
         Caption         =   "-"
      End
      Begin VB.Menu MnuEditWMO_CngFileId 
         Caption         =   "Change id in id-to-path"
      End
      Begin VB.Menu Mnu_Dash0 
         Caption         =   "-"
      End
      Begin VB.Menu MnuEditWMO_Cpy 
         Caption         =   "Copy XYZ to clipboard"
      End
      Begin VB.Menu MnuEditWMO_Paste 
         Caption         =   "Paste XYZ into this item"
      End
   End
   Begin VB.Menu MnuOpts_ 
      Caption         =   "MnuOpts_"
      Begin VB.Menu MnuOpts_idpathtable 
         Caption         =   "Edit .WMO id-to-path table"
      End
      Begin VB.Menu MnuOpts_InsrtWMO 
         Caption         =   "Insert new .WMO instance"
      End
      Begin VB.Menu MnuOpts_Dash 
         Caption         =   "-"
      End
      Begin VB.Menu MnuOpts_DecWaterType 
         Caption         =   "Declare water type to use when writing"
      End
      Begin VB.Menu MnuOpts_InjectWater 
         Caption         =   "Set global water level"
      End
      Begin VB.Menu MnuOpts_KillLiquids 
         Caption         =   "Delete all water (ocean/river/magma)"
      End
      Begin VB.Menu MnuOpts_Dash4 
         Caption         =   "-"
      End
      Begin VB.Menu MnuOpts_HolesEverywhere 
         Caption         =   "Places holes in the ground for every chucnk"
      End
      Begin VB.Menu MnuOpts_NoHolesAnywhere 
         Caption         =   "Delete all holes (fill in all holes) in the ground"
      End
      Begin VB.Menu MnuOpts_Dash5 
         Caption         =   "-"
      End
      Begin VB.Menu Mnu_ME_ 
         Caption         =   "Usefull Multi Effects"
         Begin VB.Menu MnuOpts_MassChangeDood 
            Caption         =   "Kill all trees!"
         End
      End
      Begin VB.Menu Mnu_CustDoodCng 
         Caption         =   "Do custom Z change to multiple doodads"
      End
      Begin VB.Menu MnuOpts_Dash3 
         Caption         =   "-"
      End
      Begin VB.Menu MnuOpts_DmmyMPQ 
         Caption         =   "Declare Dummy MPQ Path"
      End
      Begin VB.Menu MnuOpts_ShowDebug 
         Caption         =   "Show debug window"
      End
   End
   Begin VB.Menu MnuMCNK_ 
      Caption         =   "MnuMCNK_"
      Begin VB.Menu MnuMCNK_CHangeSingle 
         Caption         =   "Chnage selected area Id/Title"
      End
      Begin VB.Menu MnuMCNK_CHangeMulti 
         Caption         =   "Replace every area ID that is THIS selected ID  with...."
      End
   End
End
Attribute VB_Name = "GUI_ADTFile"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Public TargFile As String
Public adt As New ADTFile

Dim WarnedAboutMap      As Boolean
Dim SavedSinceChanges   As Boolean

Private Sub btnEditSel_Click()
    If ListView1.Visible = True Then PopupMenu MnuEdit_
    If WMOListView.Visible = True Then PopupMenu MnuEditWMO_
    
End Sub

Private Sub btnOtherOpts_Click()
    PopupMenu MnuOpts_
End Sub

Private Sub BtnPreviewXY_Click()
        
    Load GUIViewTop_ADT
    Set GUIViewTop_ADT.ParentGUI = Me
    Set GUIViewTop_ADT.adt = Me.adt
    GUIViewTop_ADT.Draw
    GUIViewTop_ADT.Show
    GUIViewTop_ADT.WindowState = vbMaximized
    
    If WarnedAboutMap = False Then
        MsgBox "NOTE: Please remember that you can click and drag the map around to get to areas off the GUI", vbInformation
        WarnedAboutMap = True
    End If
    
    Me.Hide
    
End Sub

Private Sub btnSave_Click()
Dim s As String

    s = InputBox("Where should the file be saved to?", "Input Path", TargFile)
    If s = "" Then Exit Sub
    
    Load GUIShowMsg
    GUIShowMsg.Show
    GUIShowMsg.Refresh
    DoEvents
    
    adt.SaveADT s
    
    Unload GUIShowMsg
    
    SavedSinceChanges = True
    
End Sub

Private Sub chkRoundNbrs_Click()
        
    ListView1.ListItems.Clear
    WMOListView.ListItems.Clear
    Me.Refresh
    DoEvents
    
    DisplayDataromBuffer_Doodads
    DisplayDataromBuffer_WMOs
    
End Sub

Private Sub ChunkView_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    
    If Button = vbRightButton Then PopupMenu MnuMCNK_
    
End Sub

Function MassDoodadEffect_ZOnly(ByVal Filter As String, ByVal ChangeOpSet As Boolean, ByVal zChange As Single) As Integer
Dim i As Integer, CountChange As Integer

    For i = 1 To adt.Doodad_Count
        
        If InStr(1, adt.Doodad_DoodadPath(i), Filter, vbTextCompare) <> 0 Then
            
            If ChangeOpSet Then
            
                adt.Doodad_DoodadZ(i) = zChange
                
            Else
            
                adt.Doodad_DoodadZ(i) = adt.Doodad_DoodadZ(i) + zChange
                
            End If
            
            DebugPrint "Doodad " & i & " now has a Z value of " & adt.Doodad_DoodadZ(i)
            CountChange = CountChange + 1
            
        End If
        
    Next
    
    MassDoodadEffect_ZOnly = CountChange
    
End Function

Private Sub Command1_Click()
    PopupMenu VataMnu_
End Sub

Private Sub Form_Load()
    
    MnuEdit_.Visible = False
    MnuEditWMO_.Visible = False
    MnuOpts_.Visible = False
    MnuMCNK_.Visible = False
    VataMnu_.Visible = False
    
    WarnedAboutMap = False
    
    Me.Caption = Replace(Me.Caption, "$Ver", App.Major & "." & App.Minor & "." & App.Revision)
    
    TargFile = GUIStart.TargFile
    adt.OpenADT TargFile
    
    DisplayDataromBuffer_Doodads
    DisplayDataromBuffer_WMOs
    
    SavedSinceChanges = True
    
    
    
    Exit Sub
    MsgBox "testing"
    
    Dim X As Integer, Y As Integer, C As Integer

    
    
    For C = 1 To 255 + 1
        
        For X = 1 To 8
        For Y = 1 To 8
    
            adt.HeightMap_Inner8x8(C, X, Y) = adt.HeightMap_Inner8x8(C, X, Y) - 20
    
        Next
        Next
    
        For X = 1 To 9
        For Y = 1 To 9
            
            If X = 9 And Y = 9 Then
                adt.HeightMap_Outter9x9(C, 9, 9) = 0
                DoEvents
            Else
                adt.HeightMap_Outter9x9(C, X, Y) = adt.HeightMap_Outter9x9(C, X, Y) - 20
            End If
            
        Next
        Next
   
    Next
   
    'For C = 1 To 255
    '
    '    For x = 1 To 8
    '    For Y = 1 To 8
    '
    '        ADT.HeightMap_Inner8x8(C, x, Y) = ADT.HeightMap_Inner8x8(C, x, Y) - 20
    '
    '    Next
    '    Next
    '
    'Next
    
    'MsgBox "testing block terr change"
    'CopyHeightMapToClipboard2
    'End
    
End Sub

Sub CopyHeighMap9x9Outter_OneChunk(adt As ADTFile, ByVal CId As Integer, CopyType As Integer)
    
'   // CopyType
'   // 1    Dec values
'   // 2    Byte-String values
'   // 3    Binary

Dim C As Integer, s As String, X As Integer, Y As Integer
    
    For Y = 1 To 9
    
        For X = 1 To 9
            
            Select Case CopyType
            Case 1:     s = s & adt.HeightMap_Outter9x9(CId, X, Y) & vbTab
            Case 2:     s = s & StringToByteString(adt.HeightMap_Outter9x9bin(CId, X, Y)) & vbTab
            Case 3:     s = s & adt.HeightMap_Outter9x9bin(CId, X, Y) & vbTab
            End Select
            
        Next
        
        s = s & vbCrLf
        
    Next
    
    Clipboard.Clear
    Clipboard.SetText s
    
End Sub

Sub CopyHeighMap9x9Outter_AllChunks(adt As ADTFile, CopyType As Integer)

'   // CopyType
'   // 1    Dec values
'   // 2    Byte-String values
'   // 3    Binary

Dim a() As String, s As String
Dim Out(16 * 9, 16 * 9) As String
Dim bOut(16 * 9, 16 * 9) As Boolean
Dim outX As Integer, outY As Integer
Dim C As Integer, CX As Integer, CY As Integer, readX As Integer, readY As Integer
    
    Load GUIShowMsg
    GUIShowMsg.Show
    
    GUIShowMsg.Bar1.Max = 41761
    GUIShowMsg.Bar1.Value = 0
    
    For CX = 1 To 16
    For CY = 1 To 16
        
        C = ((CY - 1) * 16) + CX
        
        For readX = 1 To 9
        For readY = 1 To 9
            
            Select Case CopyType
            Case 1:     Out(readX + ((CX - 1) * 9), readY + ((CY - 1) * 9)) = adt.HeightMap_Outter9x9(C, readX, readY)
            Case 2:     Out(readX + ((CX - 1) * 9), readY + ((CY - 1) * 9)) = StringToByteString(adt.HeightMap_Outter9x9bin(C, readX, readY))
            Case 3:     Out(readX + ((CX - 1) * 9), readY + ((CY - 1) * 9)) = adt.HeightMap_Outter9x9bin(C, readX, readY)
            End Select
                   
            bOut(readX + CX, readY + CY) = True
                   
            GUIShowMsg.Bar1.Value = GUIShowMsg.Bar1.Value + 1
            DoEvents
                   
        Next
        Next
        
    Next
    Next
    
    
    Dim X As Integer, Y As Integer, ThisRow As String
    
    For Y = 1 To (16 * 9)
        
        ThisRow = ""
        
        For X = 1 To 16 * 9
        
            If X <> 1 Then ThisRow = ThisRow & vbTab
            
            ThisRow = ThisRow & Out(X, Y)
            
            GUIShowMsg.Bar1.Value = GUIShowMsg.Bar1.Value + 1
            DoEvents
            
        Next
        
        If Y <> 1 Then s = s & vbCrLf
        
        s = s & ThisRow
    
    Next
    
    Clipboard.Clear
    Clipboard.SetText s
    
    Unload GUIShowMsg
    
End Sub

Sub CopyHeighMap9x9Outter2_AllChunks(adt As ADTFile, CopyType As Integer)

'   // CopyType
'   // 1    Dec values
'   // 2    Byte-String values
'   // 3    Binary

Dim a() As String, s As String
Dim Out(16 * 9, 16 * 9) As String
Dim bOut(16 * 9, 16 * 9) As Boolean
Dim outX As Integer, outY As Integer
Dim C As Integer, CX As Integer, CY As Integer, readX As Integer, readY As Integer
Dim myX As Integer, myY As Integer, MyOffset As Single

    Load GUIShowMsg
    GUIShowMsg.Show
    
    GUIShowMsg.Bar1.Max = 41761
    GUIShowMsg.Bar1.Value = 0
    
    For CX = 1 To 16
    For CY = 1 To 16
        
        C = ((CY - 1) * 16) + CX
        
        For readX = 1 To 9
        For readY = 1 To 9
            
            
            myX = readX + ((CX - 1) * 9)
            myY = readY + ((CY - 1) * 9)
            
            If CY > 2 Then MyOffset = CSng(Out(myX, 1 + ((CY - 1) * 9))) - CSng(Out(myX, 1 + ((CY - 2) * 9)))
            
            Out(myX, myY) = adt.HeightMap_Outter9x9(C, readX, readY)
            Out(myX, myY) = Out(myX, myY) + MyOffset
                   
            bOut(readX + CX, readY + CY) = True
                   
            GUIShowMsg.Bar1.Value = GUIShowMsg.Bar1.Value + 1
            DoEvents
                   
        Next
        Next
        
    Next
    Next
    
    
    Dim X As Integer, Y As Integer, ThisRow As String
    
    For Y = 1 To (16 * 9)
        
        ThisRow = ""
        
        For X = 1 To 16 * 9
        
            If X <> 1 Then ThisRow = ThisRow & vbTab
            
            ThisRow = ThisRow & Out(X, Y)
            
            GUIShowMsg.Bar1.Value = GUIShowMsg.Bar1.Value + 1
            DoEvents
            
        Next
        
        If Y <> 1 Then s = s & vbCrLf
        
        s = s & ThisRow
    
    Next
    
    Clipboard.Clear
    Clipboard.SetText s
    
    Unload GUIShowMsg
    
End Sub

Sub CopyHeightMapToClipboard2()
Dim X As Integer, Y As Integer
Dim s As String, C As Integer, subC As Integer, MyOff As Single
Dim ChunkRow As Integer
Dim Report(17, 17) As Single

Const TILESIZE As Single = 533.33333
Const CHUNKSIZE As Single = (TILESIZE) / 16
Const UNITSIZE As Single = CHUNKSIZE / 8
Const ZEROPOINT As Single = 32# * (TILESIZE)

    '5,7

    'For ChunkRow = 0 To 15
        For Y = 1 To 9
            
            If s <> "" Then s = s & vbCrLf
            
            For C = 15 + (ChunkRow * 16) To 16 + (ChunkRow * 16)
            
                If C <> 15 Then s = s & vbTab
                
                'Report(subC, ChunkRow) = ADT.HeightMap_Outter9x9(c, 1, Y)
                
                'MyOff = 0
                'If ChunkRow <> 0 Then
                '    MyOff = Report(subC, ChunkRow - 1) - Report(subC, ChunkRow)
                'End If
                
                s = s & Stripfy_GetRow(C, Y, 0)
                
                subC = subC + 1
                
            Next
            
            subC = 1
        
        Next
    'Next
    
    Clipboard.Clear
    Clipboard.SetText s

End Sub

Function Stripfy_GetRow(ByVal Chunk As Integer, ByVal Y As Integer, ByVal OffsetAdd As Single) As String
Dim X As Integer, s As String, Sngl As Single

    For X = 1 To 9
        
        If s <> "" Then s = s & vbTab
        Sngl = adt.HeightMap_Outter9x9(Chunk, X, Y)
        Sngl = Sngl + OffsetAdd
        s = s & Sngl
        
    Next
    
    Stripfy_GetRow = s
    
End Function

Sub CopyHeightMapToClipboard()
Dim X As Integer, Y As Integer, s As String
Dim GlobalHeightMap(8 * 17, 8 * 17) As Single
Dim ChkX As Integer, ChkY As Integer, ChkId As Integer
Dim rX As Integer, rY As Integer
    
    ChkId = 1
    
    For ChkY = 1 To 16
    For ChkX = 1 To 16
    
        For Y = 1 To 9
        For X = 1 To 9
    
            rX = ((ChkX - 1) * 8) + X
            rY = ((ChkY - 1) * 8) + Y
    
            GlobalHeightMap(rX, rY) = adt.HeightMap_Outter9x9(ChkId, X, Y)
    
        Next
        Next
   
        ChkId = ChkId + 1
    
    Next
    Next
    
    s = ""
    
    For Y = 1 To 128
    
        For X = 1 To 128
            
            If X <> 1 Then s = s & vbTab
            s = s & GlobalHeightMap(X, Y)
            
        Next
    
        s = s & vbCrLf
        
    Next
    
    Clipboard.Clear
    Clipboard.SetText s
    
End Sub

Sub DisplayDataromBuffer_WMOs()
Dim X As Long, p() As String, Nbr As Double

    DebugPrint "RePainting ListView with WMO location info"

    With WMOListView.ListItems
        
        .Clear
            
        For X = 1 To adt.WMO_Count
            
            p = Split(adt.WMO_WMOPath(X), "\")
            .Add .Count + 1, , p(UBound(p))
            .Item(.Count).Tag = X
            
            Nbr = adt.WMO_X(X)
            If chkRoundNbrs.Value = vbChecked Then Nbr = Round(Nbr, 2)
            .Item(.Count).SubItems(1) = Nbr
            
            Nbr = adt.WMO_Y(X)
            If chkRoundNbrs.Value = vbChecked Then Nbr = Round(Nbr, 2)
            .Item(.Count).SubItems(2) = Nbr
            
            Nbr = adt.WMO_Z(X)
            If chkRoundNbrs.Value = vbChecked Then Nbr = Round(Nbr, 2)
            .Item(.Count).SubItems(3) = Nbr
            
            Nbr = adt.WMO_RotA(X)
            If chkRoundNbrs.Value = vbChecked Then Nbr = Round(Nbr, 2)
            .Item(.Count).SubItems(4) = Nbr
            
            Nbr = adt.WMO_RotB(X)
            If chkRoundNbrs.Value = vbChecked Then Nbr = Round(Nbr, 2)
            .Item(.Count).SubItems(5) = Nbr
            
            Nbr = adt.WMO_RotC(X)
            If chkRoundNbrs.Value = vbChecked Then Nbr = Round(Nbr, 2)
            .Item(.Count).SubItems(6) = Nbr
            
            Nbr = adt.WMO_Unknown1(X)
            If chkRoundNbrs.Value = vbChecked Then Nbr = Round(Nbr, 2)
            .Item(.Count).SubItems(7) = Nbr
            
            Nbr = adt.WMO_Unknown2(X)
            If chkRoundNbrs.Value = vbChecked Then Nbr = Round(Nbr, 2)
            .Item(.Count).SubItems(8) = Nbr
            
            Nbr = adt.WMO_Unknown3(X)
            If chkRoundNbrs.Value = vbChecked Then Nbr = Round(Nbr, 2)
            .Item(.Count).SubItems(9) = Nbr
            
            Nbr = adt.WMO_Unknown4(X)
            If chkRoundNbrs.Value = vbChecked Then Nbr = Round(Nbr, 2)
            .Item(.Count).SubItems(10) = Nbr
            
            Nbr = adt.WMO_Unknown5(X)
            If chkRoundNbrs.Value = vbChecked Then Nbr = Round(Nbr, 2)
            .Item(.Count).SubItems(11) = Nbr
            
            Nbr = adt.WMO_Unknown6(X)
            If chkRoundNbrs.Value = vbChecked Then Nbr = Round(Nbr, 2)
            .Item(.Count).SubItems(12) = Nbr
            
            Nbr = adt.WMO_Flags(X)
            If chkRoundNbrs.Value = vbChecked Then Nbr = Round(Nbr, 2)
            .Item(.Count).SubItems(13) = Nbr
            
            Nbr = adt.WMO_DoodadSet(X)
            If chkRoundNbrs.Value = vbChecked Then Nbr = Round(Nbr, 2)
            .Item(.Count).SubItems(14) = Nbr
            
            Nbr = adt.WMO_NameSet(X)
            If chkRoundNbrs.Value = vbChecked Then Nbr = Round(Nbr, 2)
            .Item(.Count).SubItems(15) = Nbr

            Nbr = adt.WMO_SpecialId(X)
            If chkRoundNbrs.Value = vbChecked Then Nbr = Round(Nbr, 2)
            .Item(.Count).SubItems(15 + 1) = Nbr

        Next
            
    End With
    
End Sub


Sub DisplayDataromBuffer_Doodads()
Dim X As Integer, p() As String, s As String, Nbr As Double
    
    DebugPrint "RePainting ListView with doodad location info"
    
    With ListView1.ListItems
    
        .Clear
        
        For X = 1 To adt.Doodad_Count
            
            p = Split(adt.Doodad_DoodadPath(X), "\")
            
            .Add .Count + 1, , p(UBound(p))
            .Item(.Count).Tag = X
            
            Nbr = adt.Doodad_DoodadX(X)
            If chkRoundNbrs.Value = vbChecked Then Nbr = Round(Nbr, 2)
            .Item(.Count).SubItems(1) = Nbr
            
            Nbr = adt.Doodad_DoodadY(X)
            If chkRoundNbrs.Value = vbChecked Then Nbr = Round(Nbr, 2)
            .Item(.Count).SubItems(2) = Nbr
            
            Nbr = adt.Doodad_DoodadZ(X)
            If chkRoundNbrs.Value = vbChecked Then Nbr = Round(Nbr, 2)
            .Item(.Count).SubItems(3) = Nbr
            
            
            Nbr = adt.Doodad_DoodadRotA(X)
            If chkRoundNbrs.Value = vbChecked Then Nbr = Round(Nbr, 2)
            .Item(.Count).SubItems(4) = Nbr
            
            Nbr = adt.Doodad_DoodadRotB(X)
            If chkRoundNbrs.Value = vbChecked Then Nbr = Round(Nbr, 2)
            .Item(.Count).SubItems(5) = Nbr
            
            Nbr = adt.Doodad_DoodadRotC(X)
            If chkRoundNbrs.Value = vbChecked Then Nbr = Round(Nbr, 2)
            .Item(.Count).SubItems(6) = Nbr
            
            
            Nbr = adt.Doodad_Size(X)
            If chkRoundNbrs.Value = vbChecked Then Nbr = Round(Nbr, 2)
            .Item(.Count).SubItems(7) = Nbr
            
            Nbr = adt.Doodad_DoodadFlags(X)
            If chkRoundNbrs.Value = vbChecked Then Nbr = Round(Nbr, 2)
            .Item(.Count).SubItems(8) = Nbr
            
        Next
        
    End With
    
End Sub

Private Sub Form_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
    lblFullPath.Caption = "Double click so view location on map, right click for options."
End Sub

Private Sub Form_Unload(Cancel As Integer)
    
    If SavedSinceChanges = False Then
        
        If MsgBox("Exit WITHOUT saving changes?", vbYesNo Or vbQuestion) = vbNo Then
            
            Cancel = 1
            
        End If
        
    End If
    
End Sub

Private Sub ListView1_Click()
    lblFullPath.Caption = "ID=" & ListView1.SelectedItem.Tag & " Path=" & adt.Doodad_DoodadPath(ListView1.SelectedItem.Tag)
End Sub

Private Sub ListView1_ColumnClick(ByVal ColumnHeader As MSComctlLib.ColumnHeader)
    
    ListView1.SortKey = ColumnHeader.Index - 1
    ListView1.Sorted = True
    ListView1.Sorted = False
    
End Sub


Private Sub ListView1_DblClick()

    If ListView1.SelectedItem Is Nothing Then Exit Sub
    
    BtnPreviewXY_Click
    GUIViewTop_ADT.PointArowToDoodad ListView1.SelectedItem.Tag
    
End Sub

Private Sub ListView1_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    If Button = vbRightButton Then PopupMenu MnuEdit_
End Sub

Private Sub Mnu_CngWMOUnknown_Click(Index As Integer)
Dim sel As Integer, D As String

    If WMOListView.SelectedItem Is Nothing Then Exit Sub
    sel = WMOListView.SelectedItem.Tag
    
    Select Case Index
    Case 0:     D = adt.WMO_Unknown1(sel)
    Case 1:     D = adt.WMO_Unknown2(sel)
    Case 2:     D = adt.WMO_Unknown3(sel)
    Case 3:     D = adt.WMO_Unknown4(sel)
    Case 4:     D = adt.WMO_Unknown5(sel)
    Case 5:     D = adt.WMO_Unknown6(sel)
    End Select
    
    D = InputBox("Input new value", , D)
    If D = "" Then Exit Sub

    Select Case Index
    Case 0:     adt.WMO_Unknown1(sel) = D
    Case 1:     adt.WMO_Unknown2(sel) = D
    Case 2:     adt.WMO_Unknown3(sel) = D
    Case 3:     adt.WMO_Unknown4(sel) = D
    Case 4:     adt.WMO_Unknown5(sel) = D
    Case 5:     adt.WMO_Unknown6(sel) = D
    End Select
    
End Sub

Private Sub Mnu_CustDoodCng_Click()
Dim Targ As String, NewVal As String
    
    Targ = InputBox("What doodads would you like to search for?" & vbCrLf & "When looping through all the doodads in this map, if the input string is in a given doodad, it will be changed according to the next prompt.", , "tree")
    
    If Targ = "" Then
        If MsgBox("In the input was blank/null, does that mean you want to effect ALL doodads?" & vbCrLf & vbCrLf & "No = Cancle", vbYesNo, vbQuestion) = vbNo Then
            Exit Sub
        End If
    End If
    
    NewVal = InputBox("What would you like to change the Z axis to on each doodad that passes the previous filter?" & vbCrLf & "Each doodad that passes the previous filter will have its Z value set to this number", , -800)
    If NewVal = "" Then
        MsgBox "Input was blank/null, process canceled", vbInformation
    End If
    
    MsgBox MassDoodadEffect_ZOnly(Targ, True, NewVal) & " doodads effected by this process.", vbInformation
    
    SavedSinceChanges = False
    
End Sub

Private Sub Mnu_ShiftLand_Click_dis()
Dim X As Integer, Y As Integer, C As Integer
Dim s As String, n As Single
    
    MsgBox "testing"
    
    s = InputBox("What values would you like to influence land-height-map by?" & vbCrLf & "Putting in a negative number will lower the altitude, a positive will increase it", , -20)
    
    If s = "" Then Exit Sub
    
    If IsNumeric(s) = False Then
        MsgBox "Error: Input was not numeric", vbExclamation
        Exit Sub
    End If
    
    n = CSng(s)
    
    For C = 1 To 255 + 1
        
        For X = 1 To 8
        For Y = 1 To 8
    
            adt.HeightMap_Inner8x8bin(C, X, Y) = adt.HeightMap_Inner8x8bin(2, X, Y)
    
        Next
        Next
    
        For X = 1 To 9
        For Y = 1 To 9
            
            If X = 9 And Y = 9 Then
                'ADT.HeightMap_Outter9x9bin(C, 9, 9) = ADT.HeightMap_Outter9x9bin(2, 9, 9)       '//  Commented out cause it crashes WoW
                DoEvents
            Else
                adt.HeightMap_Outter9x9(C, X, Y) = adt.HeightMap_Outter9x9(2, X, Y)
            End If
            
        Next
        Next
        
        adt.RawMCNK(C, MCNK_RNCM) = adt.RawMCNK(2, MCNK_RNCM)
        
   
    Next
    
    MsgBox "Done", vbInformation
    SavedSinceChanges = False
    
End Sub

Private Sub Mnu_MkDoodDrawnAllChnks_Click()
Dim sel As Long, X As Integer

    sel = ListView1.SelectedItem.Tag
    
    For X = 1 To 256
        adt.MakeDoodadDrawnInChunk X, sel
    Next
    
End Sub

Private Sub Mnu_ShiftLand_Click()
Dim X As Integer, Y As Integer, C As Integer
Dim s As String, n As Single
        
    s = InputBox("What values would you like to influence land-height-map by?" & vbCrLf & "Putting in a negative number will lower the altitude, a positive will increase it", , -20)
    
    If s = "" Then Exit Sub
    
    If IsNumeric(s) = False Then
        MsgBox "Error: Input was not numeric", vbExclamation
        Exit Sub
    End If
    
    n = CSng(s)
    
    
    For C = 1 To 255 + 1
        adt.HeightMap_LandShift C, n
    Next
    
    Dim D As Integer, w As Integer
    GUIDebug.Show
    
    'For D = 1 To adt.Doodad_Count
    '    adt.Doodad_DoodadZ(D) = adt.Doodad_DoodadZ(D) + n
    '    DebugPrint "Shifted altitude of doodad " & D & " by " & n
    'Next
    
    'For w = 1 To adt.WMO_Count
    '    adt.WMO_Z(w) = adt.WMO_Z(w) + n
    '    DebugPrint "Shifted altitude of WMO " & D & " by " & n
    'Next
    
    MsgBox "Done", vbInformation
    GUIDebug.Hide
    SavedSinceChanges = False
    
End Sub

Private Sub Mnu_test_Click()
Dim X As Integer, Y As Integer
Dim C As Integer

    MsgBox "NOTICE: This function isnt really working all that well. It's experimental so dont expect much from it,", vbExclamation

    For C = 1 To 256    '// For all MCNK chunks
        
        adt.ChunkZ(C) = adt.ChunkZ(32)
        
        For X = 1 To 8
        For Y = 1 To 8
    
            adt.HeightMap_Inner8x8(C, X, Y) = 0
    
        Next
        Next
    
        For X = 1 To 9
        For Y = 1 To 9
            
            If X = 9 And Y = 9 Then
                adt.HeightMap_Outter9x9bin(C, 9, 9) = adt.HeightMap_Outter9x9bin(5, 9, 9)
                DoEvents
            Else
                adt.HeightMap_Outter9x9(C, X, Y) = 0
            End If
            
        Next
        Next
        
        adt.ChunkX(C) = 0
        adt.RawMCNK(C, MCNK_RNCM) = adt.RawMCNK(1, MCNK_RNCM)
        
    Next
    
    MsgBox "Done"
    
End Sub

Private Sub Mnu_test2_Click()
    
    '   204 205
    '   220 221
    
    ' AB 29 29
        ' chunk 204
            adt.HeightMap_Outter9x9(204, 9, 9) = adt.HeightMap_Outter9x9(204, 9, 9) + 4
            adt.HeightMap_Inner8x8(204, 8, 8) = adt.HeightMap_Inner8x8(204, 8, 8) + 4
            adt.HeightMap_Outter9x9(204, 8, 8) = adt.HeightMap_Outter9x9(204, 8, 8) + 2
            adt.HeightMap_Inner8x8(204, 7, 7) = adt.HeightMap_Inner8x8(204, 7, 7) + 2
            adt.HeightMap_Inner8x8(204, 6, 7) = adt.HeightMap_Inner8x8(204, 6, 7) + 2
            adt.HeightMap_Inner8x8(204, 7, 6) = adt.HeightMap_Inner8x8(204, 7, 6) + 2
            

    
End Sub

'Private Sub MMap_BitAssoc_Click()
'Dim s As String
'
'    s = InputBox("Where is the bitmap?", "Input path")
'
'    GUIBitmapAlign.Picture = StdFunctions.LoadPicture(s)
'    GUIBitmapAlign.Width = GUIBitmapAlign.Picture.Width
'    GUIBitmapAlign.Height = GUIBitmapAlign.Picture.Height
'
'    MsgBox "Please click on the image the center of where the X,Y Qs in terms of the .wmo"
'
'    GUIBitmapAlign.Show vbModal
'
'    BitmapAssoc = Join(BitmapAssoc, vbCrLf ) &
'
'End Sub

Private Sub MnuEdit_Cng_Click(Index As Integer)
Dim D As Double, InTxt As String, sel As Integer, DefValue As String
    
    If Index = 3 Then
        Load GUIAbout
        GUIAbout.ShowText_HelpXYZ
        GUIAbout.Show
        Exit Sub
    End If
    
    If Index = 8 Then
        Load GUIAbout
        GUIAbout.ShowText_HelpABC
        GUIAbout.Show
        Exit Sub
    End If
    
    If ListView1.SelectedItem Is Nothing Then
        MsgBox "Select something first", , "Duh"
        Exit Sub
    Else
        sel = ListView1.SelectedItem.Tag
    End If
    
    Select Case Index
    Case 0:         DefValue = adt.Doodad_DoodadX(sel)
    Case 1:         DefValue = adt.Doodad_DoodadY(sel)
    Case 2:         DefValue = adt.Doodad_DoodadZ(sel)
    Case Else:      DefValue = ""
    End Select
    
    InTxt = InputBox("What should the new value be?", "Input new value", DefValue)
    
    If IsNumeric(InTxt) And Mid(InTxt, 1, 1) <> "=" Then
        D = InTxt
    Else
    
        InTxt = Mid(InTxt, 2)
        
        If Mid(InTxt, 1, 1) = "+" Then
            D = CDbl(DefValue) + CDbl(Replace(InTxt, "+", ""))
        ElseIf Mid(InTxt, 1, 1) = "-" Then
            D = CDbl(DefValue) - CDbl(Replace(InTxt, "-", ""))
        ElseIf Mid(InTxt, 1, 1) = "*" Then
            D = CDbl(DefValue) * CDbl(Replace(InTxt, "*", ""))
        ElseIf Mid(InTxt, 1, 1) = "/" Then
            D = CDbl(DefValue) / CDbl(Replace(InTxt, "/", ""))
        Else
            MsgBox "Error: Invalid input", vbExclamation
            Exit Sub
        End If
        
    End If
    
    Select Case Index
    
    Case 0:     adt.Doodad_DoodadX(sel) = D
    Case 1:     adt.Doodad_DoodadY(sel) = D
    Case 2:     adt.Doodad_DoodadZ(sel) = D
    
    Case 4:     adt.Doodad_DoodadRotA(sel) = D
    Case 5:     adt.Doodad_DoodadRotB(sel) = D
    Case 6:     adt.Doodad_DoodadRotC(sel) = D
    
    End Select
    
    DisplayDataromBuffer_Doodads
    
End Sub

Private Sub MnuEdit_CngDoodSize_Click()
Dim sel As Long, s As String, NewSize As Long

    sel = ListView1.SelectedItem.Tag
    
    s = InputBox("What would you like to make the new size?")
    If s = "" Then Exit Sub
    If IsNumeric(s) = False Then
        MsgBox "Error, input is not numeric", vbExclamation
        Exit Sub
    End If
    
    NewSize = s
    
    adt.Doodad_Size(sel) = NewSize
    
End Sub

Private Sub MnuEdit_Cpy_Click()
Dim sel As Long

    sel = ListView1.SelectedItem.Tag
    
    Clipboard.Clear
    Clipboard.SetText adt.Doodad_DoodadX(sel) & ", " & adt.Doodad_DoodadY(sel) & ", " & adt.Doodad_DoodadZ(sel)
    
End Sub

Private Sub MnuEdit_Paste_Click()
Dim i As Integer, s As String, a() As Single, X As Single, Y As Single, Z As Single, sel As Long

    s = Clipboard.GetText
    a = Split(s, ", ")
    
    X = a(0)
    Y = a(1)
    Z = a(2)
    
    With ListView1.ListItems
    For i = 1 To .Count
        
        If .Item(i).Selected = True Then
            sel = .Item(i).Tag
            adt.Doodad_DoodadX(sel) = X
            adt.Doodad_DoodadY(sel) = Y
            adt.Doodad_DoodadZ(sel) = Z
        End If
        
    Next
    End With
    
End Sub

Private Sub MnuEditWMO_Chng_Click(Index As Integer)
Dim D As Double, InTxt As String, sel As Integer, DefValue As String
    
    If Index = 3 Then
        Load GUIAbout
        GUIAbout.ShowText_HelpXYZ
        GUIAbout.Show
        Exit Sub
    End If
    
    If Index = 8 Then
        Load GUIAbout
        GUIAbout.ShowText_HelpABC
        GUIAbout.Show
        Exit Sub
    End If
    
    
    If WMOListView.SelectedItem Is Nothing Then
        MsgBox "Select something first", , "Duh"
        Exit Sub
    Else
        sel = WMOListView.SelectedItem.Tag
    End If
    
    Select Case Index
    Case 0:         DefValue = adt.WMO_X(sel)
    Case 1:         DefValue = adt.WMO_Y(sel)
    Case 2:         DefValue = adt.WMO_Z(sel)
    Case Else:      DefValue = ""
    End Select
    
    InTxt = InputBox("What should the new value be?", "Input new value", DefValue)
    
    If IsNumeric(InTxt) And Mid(InTxt, 1, 1) <> "=" Then
        D = InTxt
    Else
    
        InTxt = Mid(InTxt, 2)
        
        If Mid(InTxt, 1, 1) = "+" Then
            D = CDbl(DefValue) + CDbl(Replace(InTxt, "+", ""))
        ElseIf Mid(InTxt, 1, 1) = "-" Then
            D = CDbl(DefValue) - CDbl(Replace(InTxt, "-", ""))
        ElseIf Mid(InTxt, 1, 1) = "*" Then
            D = CDbl(DefValue) * CDbl(Replace(InTxt, "*", ""))
        ElseIf Mid(InTxt, 1, 1) = "/" Then
            D = CDbl(DefValue) / CDbl(Replace(InTxt, "/", ""))
        Else
            MsgBox "Error: Invalid input", vbExclamation
            Exit Sub
        End If
        
    End If
    
    Select Case Index
    Case 0:     adt.WMO_X(sel) = D
    Case 1:     adt.WMO_Y(sel) = D
    Case 2:     adt.WMO_Z(sel) = D
    Case 5:     adt.WMO_RotA(sel) = D
    Case 6:     adt.WMO_RotB(sel) = D
    Case 7:     adt.WMO_RotC(sel) = D
    End Select
    
    DisplayDataromBuffer_WMOs
    
End Sub

Private Sub MnuEditWMO_CngFileId_Click()
Dim sel As Integer, s As String

    Load GUIWMOidToPathTable
    Set GUIWMOidToPathTable.adt = Me.adt
    GUIWMOidToPathTable.LoadList
    GUIWMOidToPathTable.Show

    sel = WMOListView.SelectedItem.Tag
    s = InputBox("Looking at the id-to-path table, which path/id would you like to set this to?" & vbCrLf & "You must input a NUMBER", , 0)
    
    Unload GUIWMOidToPathTable
    
    If s = "" Then Exit Sub
    
    adt.WMO_WMOPathId(sel) = s
    DisplayDataromBuffer_WMOs
    
End Sub

Private Sub MnuEditWMO_Cpy_Click()
Dim sel As Long

    sel = WMOListView.SelectedItem.Tag
    
    Clipboard.Clear
    Clipboard.SetText adt.WMO_X(sel) & ", " & adt.WMO_Y(sel) & ", " & adt.WMO_Z(sel)
    
End Sub

Private Sub MnuEditWMO_Paste_Click()
Dim i As Integer, s As String, a() As String, X As Single, Y As Single, Z As Single, sel As Long

    s = Clipboard.GetText
    a = Split(s, ", ")
    
    X = a(0)
    Y = a(1)
    Z = a(2)
    
    With WMOListView.ListItems
    For i = 1 To .Count
        
        If .Item(i).Selected = True Then
            sel = .Item(i).Tag
            adt.Doodad_DoodadX(sel) = X
            adt.Doodad_DoodadY(sel) = Y
            adt.Doodad_DoodadZ(sel) = Z
        End If
        
    Next
    End With
    
    DisplayDataromBuffer_WMOs
    
End Sub

Private Sub MnuMCNK_CHangeMulti_Click()
Dim NewId As Integer, TargId As Integer, X As Integer
    
    TargId = ChunkView.SelectedItem.SubItems(3)
    
    Load GUIAreaManip
    GUIAreaManip.Show vbModal
    
    NewId = GUIAreaManip.AreaView.SelectedItem.Text
    
    For X = 1 To 256
        If adt.AreaIdNumber(X) = TargId Then
            adt.AreaIdNumber(X) = NewId
        End If
    Next
    
End Sub

Private Sub MnuMCNK_CHangeSingle_Click()
Dim id As Integer
    
    Load GUIAreaManip
    GUIAreaManip.Show vbModal
    id = GUIAreaManip.AreaView.SelectedItem.Text
    adt.AreaIdNumber(ChunkView.SelectedItem.Text) = id
    
End Sub

Private Sub MnuOpts_DecWaterType_Click()
Dim s As String, i As Integer
    
    MsgBox "NOTE: This changes the variable that is written to any MCNK-header that will have water" & vbCrLf & "After you change this flag, any process that tries to write water onto the map will be effected by the change" & vbCrLf & "So if you fuck this up, it may screw up all ettemps to write water onto the map untill you close this application (the variable is reset on load)", vbExclamation
    
    s = InputBox("What MCNK-header-flag (water type) would you like to use while writing water onto the map?" & vbCrLf & vbCrLf & _
    "Here are some reported flags and their effects" & vbCrLf & "(thanks to Cryect & Demonkunga for info):" & vbCrLf & vbCrLf & _
    "00 - No visible Water" & vbCrLf & _
    "01 - Visible Water but no water physics" & vbCrLf & _
    "02 - No Visible Water (Lava Effect on Screen)" & vbCrLf & _
    "03 - No Visible Water" & vbCrLf & _
    "04 - No Visible Water (Water Underneath but needs to have the river flag set to see the water visibly)" & vbCrLf & _
    "05 - No Visible Water or Water Physics" & vbCrLf & _
    "08 - No Visible Water (Water underneath)" & vbCrLf & _
    "09 - No Visible Water or Water Physics" & vbCrLf & _
    "10 - No Visible Water" & vbCrLf & _
    "20 - No Visible Water" & vbCrLf & _
    "40 - No Visible Water" & vbCrLf & _
    "80 - No Visible Water", , adt.WaterFlag)
    
    If s = "" Then Exit Sub
    
    If IsNumeric(s) = False Then
        MsgBox "Error: Invalid input", vbCritical
        Exit Sub
    End If
    
    adt.WaterFlag = i
    
End Sub

Private Sub MnuOpts_DmmyMPQ_Click()
    
    DummyMPQPath = InputBox("Where is the directory path to an exploded .MPQ directory tree?", "Input dir path", DummyMPQPath)
    
    If DummyMPQPath <> "" Then SaveDummyMPQPath DummyMPQPath
    
End Sub

Private Sub MnuOpts_HolesEverywhere_Click()
Dim id As Integer

    For id = 1 To 255 + 1
        
        adt.Ground_Holes(id) = Chr(0) & Chr(254) & Chr(254) & Chr(0)
        
    Next
    
    MsgBox "done", vbInformation
    SavedSinceChanges = False
    
End Sub

Private Sub MnuOpts_idpathtable_Click()
    Load GUIWMOidToPathTable
    Set GUIWMOidToPathTable.adt = Me.adt
    GUIWMOidToPathTable.LoadList
    GUIWMOidToPathTable.Show
    SavedSinceChanges = False
End Sub

Private Sub MnuOpts_InjectWater_Click()
Dim s As String, a As Single

    s = InputBox("What altitude would you like to make the water?", , 500.8)
    If Trim(s) = "" Then Exit Sub
    
    On Error GoTo ErrHandeler
    a = s
    
    adt.InjectWaterForAllChunks a
    MsgBox "Done", vbInformation
    
    SavedSinceChanges = False
    
    Exit Sub
ErrHandeler:
    MsgBox "Error: The inputed number is either not a number, to high of a number, or to low of a number.", vbExclamation
    
End Sub

Private Sub MnuOpts_InsrtWMO_Click()
Dim id As Integer, p() As String, s As String
    
    Me.Hide
    
    Load GUIWMOidToPathTable
    Set GUIWMOidToPathTable.adt = Me.adt
    GUIWMOidToPathTable.LoadList
    GUIWMOidToPathTable.Show
    
    s = InputBox("Looking at the id-to-path table, whick path-id is the .wmo you want to insert?", "Input number")
    If s = "" Then GoTo BringBackGUI
    
    Unload GUIWMOidToPathTable
    
    adt.WMO_Add s, InputBox("base copy id?", , 2), adt.WMO_X(1), adt.WMO_Y(1), adt.WMO_Z(1), 0, 0, 0
    p = Split(adt.WMO_WMOPath(2), "\")
    
    MsgBox "The new.wmo has been created in the same XYZ location as the 1st .wmo in this .adt file (" & p(UBound(p)) & ").  You can now goto the main GUI to edit the XYZ and move it to where you want it.", vbInformation
    
    Toolbar1_ButtonClick Toolbar1.Buttons(2)
    DisplayDataromBuffer_WMOs
    
BringBackGUI:
    Me.Show
    SavedSinceChanges = False

    
End Sub

Private Sub MnuOpts_KillLiquids_Click()
    
    adt.KllLiquidsForAllChunks
    SavedSinceChanges = False
    
End Sub

Private Sub MnuOpts_MassChangeDood_Click()
    
    MassDoodadEffect_ZOnly "tree", True, -800
    MsgBox "Done", vbInformation
    SavedSinceChanges = False
End Sub

Private Sub MnuOpts_NoHolesAnywhere_Click()
Dim id As Integer

    For id = 1 To 255 + 1
        
        adt.Ground_Holes(id) = Chr(0) & Chr(0) & Chr(0) & Chr(0)
        
    Next
    
    MsgBox "done", vbInformation
    SavedSinceChanges = False
    
End Sub

Private Sub MnuOpts_ShowDebug_Click()
    Load GUIDebug
End Sub

Sub DisplayDataFromChuncks()
Dim X As Integer, Y As Integer, id As Integer

    With ChunkView.ListItems
    
        .Clear
    
        For Y = 0 To 15
        For X = 0 To 15
            
            id = (Y * 16) + X + 1
            .Add .Count + 1, , id
            .Item(.Count).SubItems(1) = X
            .Item(.Count).SubItems(2) = Y
            .Item(.Count).SubItems(3) = adt.AreaIdNumber(id)
            .Item(.Count).SubItems(4) = LookupAreaName(adt.AreaIdNumber(id))
            
        Next
        Next
        
    End With
    
End Sub

Sub Toolbar1_ButtonClick(ByVal Button As MSComctlLib.Button)

    ListView1.Visible = False
    WMOListView.Visible = False
    ChunkView.Visible = False

    Select Case Button.Index
    Case 1
                ListView1.Visible = True
                DisplayDataromBuffer_Doodads
                
    Case 2
                WMOListView.Visible = True
                DisplayDataromBuffer_WMOs
                
    Case 3
                DisplayDataFromChuncks
                ChunkView.Visible = True
    Case 4
                Load GUIAbout
                GUIAbout.Show
                GUIAbout.ShowText_Text
    End Select
    
    
    
    
End Sub

Private Sub VataMnu_Cpy1ChunkDec_Click()
Dim s As String, id As Integer

    s = InputBox("MCNK chunks are in matrix where the IDs for each cell are layed out in such a way that the first row is IDs 1-16, the second row is 17-32, and so on.  So ID 1 is the top-left corner. 16, is the top-right (north-west) corner. And 256 is the bottom right corner." & vbCrLf & vbCrLf & "Knowing this, wich chunck would you like to copy?")
    
    If s = "" Then Exit Sub
    
    If IsNumeric(s) = False Then
        MsgBox "Invalid input"
        Exit Sub
    End If
    
    id = s
    
    If id > 256 Then
        MsgBox "Invalid input, the input number must be bewteen 1 and 256."
        Exit Sub
    End If
    
    CopyHeighMap9x9Outter_OneChunk adt, id, 1

End Sub

Private Sub VataMnu_Cpy1ChunkDecBStr_Click()
Dim s As String, id As Integer

    s = InputBox("MCNK chunks are in matrix where the IDs for each cell are layed out in such a way that the first row is IDs 1-16, the second row is 17-32, and so on.  So ID 1 is the top-left corner. 16, is the top-right (north-west) corner. And 256 is the bottom right corner." & vbCrLf & vbCrLf & "Knowing this, wich chunck would you like to copy?")
    
    If s = "" Then Exit Sub
    
    If IsNumeric(s) = False Then
        MsgBox "Invalid input"
        Exit Sub
    End If
    
    id = s
    
    If id > 256 Then
        MsgBox "Invalid input, the input number must be bewteen 1 and 256."
        Exit Sub
    End If
    
    CopyHeighMap9x9Outter_OneChunk adt, id, 2
    
End Sub

Private Sub VataMnu_Cpy1ChunkHex_Click()
Dim s As String, id As Integer

    s = InputBox("MCNK chunks are in matrix where the IDs for each cell are layed out in such a way that the first row is IDs 1-16, the second row is 17-32, and so on.  So ID 1 is the top-left corner. 16, is the top-right (north-west) corner. And 256 is the bottom right corner." & vbCrLf & vbCrLf & "Knowing this, wich chunck would you like to copy?")
    
    If s = "" Then Exit Sub
    
    If IsNumeric(s) = False Then
        MsgBox "Invalid input"
        Exit Sub
    End If
    
    id = s
    
    If id > 256 Then
        MsgBox "Invalid input, the input number must be bewteen 1 and 256."
        Exit Sub
    End If
    
    CopyHeighMap9x9Outter_OneChunk adt, id, 3
    
End Sub

Private Sub VataMnu_CpyAllChunks_Click()
        CopyHeighMap9x9Outter_AllChunks adt, 2
End Sub

Private Sub VataMnu_CpyAllChunksDec_Click()
    CopyHeighMap9x9Outter_AllChunks adt, 1
End Sub

Private Sub VataMnu_CpyAllChunksHex_Click()
        CopyHeighMap9x9Outter_AllChunks adt, 3
End Sub

Private Sub VataMnu_PasteAllChnks_Click()
Dim s() As String, b() As String, sng As Single
Dim ArrIn(16 * 9, 16 * 9) As Single
Dim Y As Integer, X As Integer

    s = Split(Clipboard.GetText, vbCrLf)
    
    If UBound(s) <> 16 * 9 Then
        MsgBox "Error: The imput matrix does not have the expected bounds of 16x16 chunks each with 9x9 matrices (expecting a 144x144 matrix)", vbCritical
        Exit Sub
    End If
    
    For Y = 0 To UBound(s)
        
        b = Split(s(Y), vbTab)
        
        For X = 0 To UBound(b)
            
            If IsNumeric(b) = False Then
                MsgBox "Error: Matrix element " & X & ", " & Y & " is not a number", vbCritical
                Exit Sub
            End If
            
            ArrIn(X, Y) = b(X)
            
        Next
        
        DoEvents
        
    Next
    
    Dim CX As Integer, CY As Integer, C As Integer
    Dim readX As Integer, readY As Integer
    
    For CX = 1 To 16
    For CY = 1 To 16
        
        C = ((CY - 1) * 16) + CX
        
        For readX = 1 To 9
        For readY = 1 To 9
            
            adt.HeightMap_Outter9x9bin(C, readX, readY) = ArrIn(readX + ((CX - 1) * 9), readY + ((CY - 1) * 9))
            
        Next
        Next
        
    Next
    Next
    

End Sub

Private Sub WMOListView_Click()
    lblFullPath.Caption = adt.WMO_WMOPath(WMOListView.SelectedItem.Tag)
End Sub

Private Sub WMOListView_DblClick()
    
    If WMOListView.SelectedItem Is Nothing Then Exit Sub
    
    BtnPreviewXY_Click
    GUIViewTop_ADT.PointArowToWMO WMOListView.SelectedItem.Tag
    
End Sub

Private Sub WMOListView_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    If Button = vbRightButton Then PopupMenu MnuEditWMO_
End Sub
