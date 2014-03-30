VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Begin VB.Form GUIMultiMaps 
   Caption         =   "Multi Map Wizard - by SomeWhiteGuy"
   ClientHeight    =   8100
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   10590
   LinkTopic       =   "Form1"
   ScaleHeight     =   8100
   ScaleWidth      =   10590
   StartUpPosition =   3  'Windows Default
   Begin VB.Frame MultiWMOEdit 
      BorderStyle     =   0  'None
      Height          =   7095
      Left            =   0
      TabIndex        =   10
      Top             =   0
      Visible         =   0   'False
      Width           =   6255
      Begin VB.Timer Timer1 
         Interval        =   100
         Left            =   5400
         Top             =   3960
      End
      Begin VB.FileListBox Doc1 
         Height          =   1650
         Left            =   3120
         Pattern         =   "*.WMO"
         TabIndex        =   18
         Top             =   2880
         Width           =   2895
      End
      Begin VB.CommandButton Command2 
         Caption         =   "Cancle"
         Height          =   375
         Left            =   4320
         TabIndex        =   17
         Top             =   6600
         Width           =   1695
      End
      Begin VB.CommandButton btnMakeWMOChanges 
         Caption         =   "Make changes"
         Height          =   615
         Left            =   4320
         TabIndex        =   16
         Top             =   5880
         Width           =   1695
      End
      Begin MSComctlLib.ListView ListView2 
         Height          =   1815
         Left            =   120
         TabIndex        =   13
         Top             =   5160
         Width           =   4095
         _ExtentX        =   7223
         _ExtentY        =   3201
         View            =   3
         LabelWrap       =   -1  'True
         HideSelection   =   -1  'True
         HideColumnHeaders=   -1  'True
         Checkboxes      =   -1  'True
         FullRowSelect   =   -1  'True
         _Version        =   393217
         ForeColor       =   -2147483640
         BackColor       =   -2147483643
         BorderStyle     =   1
         Appearance      =   1
         NumItems        =   1
         BeginProperty ColumnHeader(1) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
            Object.Width           =   9393
         EndProperty
      End
      Begin VB.TextBox Text1 
         BackColor       =   &H8000000F&
         BorderStyle     =   0  'None
         Height          =   2295
         Left            =   120
         MultiLine       =   -1  'True
         TabIndex        =   12
         Text            =   "GUIMultiMaps.frx":0000
         Top             =   120
         Width           =   5895
      End
      Begin VB.DirListBox Dir1 
         Height          =   1665
         Left            =   120
         TabIndex        =   11
         Top             =   2880
         Width           =   2895
      End
      Begin VB.Label Label5 
         Caption         =   "These WMOs wil be effected"
         Height          =   255
         Index           =   1
         Left            =   3120
         TabIndex        =   19
         Top             =   2640
         Width           =   2775
      End
      Begin VB.Label Label5 
         Caption         =   "Make the following changes to WMOs"
         Height          =   255
         Index           =   0
         Left            =   120
         TabIndex        =   15
         Top             =   4920
         Width           =   2775
      End
      Begin VB.Label Label4 
         Caption         =   "Target directory with WMO files:"
         Height          =   255
         Left            =   120
         TabIndex        =   14
         Top             =   2640
         Width           =   2775
      End
   End
   Begin VB.Frame Frame1 
      BorderStyle     =   0  'None
      Height          =   3615
      Left            =   0
      TabIndex        =   20
      Top             =   0
      Width           =   5055
      Begin VB.CommandButton btnWMOs 
         Caption         =   "Instance maps and objects (WMOs)"
         Height          =   615
         Left            =   840
         TabIndex        =   23
         Top             =   2280
         Width           =   3255
      End
      Begin VB.CommandButton btnADTs 
         Caption         =   "Multiple outside-maps (terrain ADTs)"
         Height          =   615
         Left            =   840
         TabIndex        =   22
         Top             =   1080
         Width           =   3255
      End
      Begin VB.Label Label7 
         Alignment       =   2  'Center
         Caption         =   "What do you want to edit?"
         Height          =   255
         Left            =   0
         TabIndex        =   21
         Top             =   240
         Width           =   4935
      End
   End
   Begin VB.Frame MultiADTEdit 
      BorderStyle     =   0  'None
      Height          =   5415
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Visible         =   0   'False
      Width           =   6255
      Begin VB.ListBox lstAreas 
         Height          =   1230
         ItemData        =   "GUIMultiMaps.frx":024B
         Left            =   120
         List            =   "GUIMultiMaps.frx":0252
         TabIndex        =   4
         Top             =   2280
         Width           =   5895
      End
      Begin VB.CommandButton btnOk 
         Caption         =   "Ok"
         Height          =   375
         Left            =   4440
         TabIndex        =   3
         Top             =   4800
         Width           =   1575
      End
      Begin VB.TextBox txtParam 
         Height          =   285
         Left            =   1200
         TabIndex        =   2
         Text            =   "800"
         Top             =   3720
         Visible         =   0   'False
         Width           =   1695
      End
      Begin VB.TextBox txtTargPath 
         Height          =   285
         Left            =   1200
         TabIndex        =   1
         Top             =   4080
         Width           =   3735
      End
      Begin MSComctlLib.ListView ListView1 
         Height          =   1215
         Left            =   120
         TabIndex        =   5
         Top             =   360
         Width           =   5895
         _ExtentX        =   10398
         _ExtentY        =   2143
         View            =   3
         LabelWrap       =   -1  'True
         HideSelection   =   -1  'True
         HideColumnHeaders=   -1  'True
         Checkboxes      =   -1  'True
         FullRowSelect   =   -1  'True
         _Version        =   393217
         ForeColor       =   -2147483640
         BackColor       =   -2147483643
         BorderStyle     =   1
         Appearance      =   1
         NumItems        =   1
         BeginProperty ColumnHeader(1) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
            Object.Width           =   9701
         EndProperty
      End
      Begin VB.Label Label1 
         Caption         =   "What do you want to do?"
         Height          =   255
         Left            =   120
         TabIndex        =   9
         Top             =   120
         Width           =   4815
      End
      Begin VB.Label Label2 
         Caption         =   "What maps would you like to target?"
         Height          =   255
         Left            =   120
         TabIndex        =   8
         Top             =   2040
         Width           =   2655
      End
      Begin VB.Label Label3 
         Caption         =   "Water Alt:"
         Height          =   255
         Index           =   0
         Left            =   120
         TabIndex        =   7
         Top             =   3720
         Visible         =   0   'False
         Width           =   975
      End
      Begin VB.Label Label3 
         Caption         =   "Output Path:"
         Height          =   255
         Index           =   1
         Left            =   120
         TabIndex        =   6
         Top             =   4080
         Width           =   975
      End
   End
End
Attribute VB_Name = "GUIMultiMaps"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Sub MoveTileXYs(TileToChange As String, SrcReadTile As String, TargADTSavePath As String)
Dim C As Integer
Dim adtTileToChange As New ADTFile, adtSrcReadTile As New ADTFile

    adtSrcReadTile.OpenADT SrcReadTile
    adtTileToChange.OpenADT TileToChange

    For C = 1 To 256
        
        adtTileToChange.ChunkXbin(C) = adtSrcReadTile.ChunkXbin(C)
        adtTileToChange.ChunkYbin(C) = adtSrcReadTile.ChunkYbin(C)
        adtTileToChange.ChunkZbin(C) = adtSrcReadTile.ChunkZbin(C)
        
        
    Next
    
    adtTileToChange.SaveADT TargADTSavePath

End Sub

Sub CopyOverLandShape(TileToChange As String, SrcReadTile As String, TargADTSavePath As String)
Dim C As Integer
Dim adtTileToChange As New ADTFile, adtSrcReadTile As New ADTFile

    adtSrcReadTile.OpenADT SrcReadTile
    adtTileToChange.OpenADT TileToChange

    For C = 1 To 256
        
    '    adtTileToChange.ChunkXbin(C) = adtSrcReadTile.ChunkXbin(C)
    '    'adtTileToChange.ChunkYbin(C) = adtSrcReadTile.ChunkYbin(C)
    '    'adtTileToChange.ChunkZbin(C) = adtSrcReadTile.ChunkZbin(C)
    '
    '    For X = 1 To 8
    '    For Y = 1 To 8
    '
    '        adtTileToChange.HeightMap_Inner8x8bin(C, X, Y) = adtSrcReadTile.HeightMap_Inner8x8bin(C, X, Y)
    '
        'Next
        'Next
    '
        'For X = 1 To 9
        'For Y = 1 To 9
        '
        '    'If X = 9 And Y = 9 Then
        '    '    DoEvents
        '    'Else
        '        adtTileToChange.HeightMap_Outter9x9bin(C, X, Y) = adtSrcReadTile.HeightMap_Outter9x9bin(C, X, Y)
        '    'End If
        '
        'Next
        'Next-
        
        adtTileToChange.RawMCNK(C, MCNK_MCNK) = adtSrcReadTile.RawMCNK(C, MCNK_MCNK)
        adtTileToChange.RawMCNK(C, MCNK_TVCM) = adtSrcReadTile.RawMCNK(C, MCNK_TVCM)
        adtTileToChange.RawMCNK(C, MCNK_RNCM) = adtSrcReadTile.RawMCNK(C, MCNK_RNCM)
        
    Next
    
    adtTileToChange.SaveADT TargADTSavePath

End Sub

Private Sub btnADTs_Click()
    
    Frame1.Visible = False
    MultiADTEdit.Visible = True
    Me.Width = MultiADTEdit.Width
    Me.Height = MultiADTEdit.Height + 480
    
End Sub

Private Sub btnMakeWMOChanges_Click()
Dim X As Integer, T As Integer, TargFile As String
Dim WMO As WMOGrpFile, HitRootFile As Boolean
Dim BarMain As New GUIShowMsg
Dim ZSum As Long

    'ListView2.ListItems.Add , "water", "Put water everywhere (BETA TESTING)"
    'ListView2.ListItems.Add , "killdoods", "Remove all doodads"
    'ListView2.ListItems.Add , "solid", "Set all polygons (walls/floor/etc) as solid"
    'ListView2.ListItems.Add , "notsolid", "Set all polygons (walls/floor/etc) as not-solid (lets you go through them)"
    'ListView2.ListItems.Add , "delpolyface", "Delete polygons faces (BETA TESTING)"
    
    Load GUIDebug
    GUIDebug.Show
    
    
    BarMain.Bar1.Max = Doc1.ListCount - 1 + 1
    BarMain.Show
    BarMain.Top = (BarMain.Top - BarMain.Height) - 240
    
    For X = 0 To Doc1.ListCount - 1
        
        TargFile = Doc1.Path & "\" & Doc1.List(X)
        
        If InStr(1, TargFile, "_") <> 0 Then
            
            With ListView2.ListItems
                
                If .Item("water").Checked = True Or .Item("solid").Checked = True Or .Item("notsolid").Checked = True Or .Item("delpolyface").Checked = True Then
                
                    Set WMO = New WMOGrpFile
                    DebugPrint "Opening WMO-group file: " & TargFile
                    WMO.OpenWMO TargFile
                    
                    If .Item("water").Checked = True Then
                        
                        DebugPrint "Injecting water into WMO...."
                        WMO.FillWMOWithWater
                        
                    End If
                    
                    If .Item("solid").Checked = True Then
                    
                        For T = 1 To WMO.TriangleInfo_Count
                            WMO.TriangleInfo_Flags(T) = 32
                        Next
                        
                    End If
                    
                    If .Item("notsolid").Checked = True Then
                    
                        DebugPrint "Making all polygons non-collidable..."
                        For T = 1 To WMO.TriangleInfo_Count
                            WMO.TriangleInfo_Flags(T) = 100
                        Next
                    
                    End If
                    
                    If .Item("delpolyface").Checked = True Then
                        
                        DebugPrint "Setting all BSP-Node face-counts to 0..."
                        
                        For T = 1 To WMO.BSPNodeCount
                            
                            WMO.BSPNode_NumbFace(T) = 0
                            
                        Next
                        
                    End If
                    
                    
                    If .Item("flat").Checked = True Then
                    
                        If X = 0 Then
                        
                            For i = 1 To WMO.Vertices_Count
                                ZSum = ZSum + WMO.Vertices_Z(i)
                            Next
                        
                            ZSum = ZSum / WMO.Vertices_Count    ' // gets the average
                        
                            For i = 1 To WMO.Vertices_Count
                                WMO.Vertices_Z(i) = ZSum        ' sets the average
                            Next
                            
                        Else
                            
                            For i = 1 To WMO.Vertices_Count
                                WMO.Vertices_Z(i) = ZSum        ' sets the average
                            Next
                        
                        End If
                        
                    End If
                    

                    
                    DebugPrint "Saving WMO-group file @ " & TargFile
                    Kill TargFile
                    WMO.SaveWMO TargFile
                    
                End If
            
            End With
            
        Else
            
            HitRootFile = True
            
            If ListView2.ListItems.Item("killdoods").Checked = True Then
            
                DebugPrint "Opening WMO root file: " & TargFile
                
                Dim WMOR As New WMORootFile
                WMOR.OpendWMORootFile TargFile
                
                Load GUIShowMsg
                GUIShowMsg.Show
                GUIShowMsg.Bar1.Max = WMOR.DoodadCount + 1
                
                For T = 1 To WMOR.DoodadCount
                    DebugPrint "Setting doodad " & T & " (" & LastSplit(WMOR.DoodadPath(T), "\") & ") Z value to -800"
                    WMOR.DoodadZ(T) = -800
                    GUIShowMsg.Bar1.Value = T
                    DoEvents
                Next
                
                Unload GUIShowMsg
                
                DebugPrint "Saving WMO root file @ " & TargFile
                Kill TargFile
                WMOR.SaveWMORootFile TargFile
                
            Else
            
                DebugPrint "Skipping file " & Doc1.List(X) & " because it doens not have _### in it, it is probably a WMO root file, and not a WMO group file."
            
            End If
            
            
        End If
        
        BarMain.Bar1.Value = X
        DoEvents
        
    Next
    
    BarMain.Hide
    
    If ListView2.ListItems.Item("killdoods").Checked = True And HitRootFile = False Then
        MsgBox "FAILED!" & vbCrLf & "The WMO root file was not found in the target directory.  The root file is a .WMO file that does NOT have the _### in the file name, this file is needed when you want to do anything with doodads.", vbExclamation, "Could not make change(s)"
    Else
        MsgBox "Done", vbInformation
    End If
    
    End
    
End Sub

Private Sub btnOk_Click()
Dim s As String, a() As String, i As Integer, TargMap As String, b() As String, X As Integer
Dim adt As ADTFile
Dim ThereWasAnError As Boolean
    
    If DummyMPQPath = "" Then
        
        MsgBox "You have not set a Dummy MPQ path (the place where you have extracted all the map files from Common.MPQ for reading).  Please read the SetupAndInstallationGuide.html file that came in the WoWME archive.", vbExclamation
        
        If MsgBox("Would you like to set your Dummy MPQ path now?", vbQuestion) = vbYes Then
            
            DummyMPQPath = InputBox("Input path")
            SaveDummyMPQPath DummyMPQPath
            
        End If
        
    End If
    
    If lstAreas = "" Then
        
        MsgBox "Select an area first N00B!!1", , "Duh"
        Exit Sub
        
    End If

    If txtTargPath = "" Then
        MsgBox "Dude, put in an output path to save the edited map files to...", vbExclamation
        Exit Sub
    End If

    s = ReadFile("MapDB.txt")
    If Trim(s) = "" Then
        
        MsgBox "Error reading MapDB.txt", vbCritical
        Exit Sub
        
    End If
    
    a = Split(s, vbCrLf & vbCrLf)
    
    Load GUIShowMsg
    
    Load GUIDebug
    GUIDebug.Show
    
    For i = 0 To UBound(a)
        
        If InStr(1, a(i), lstAreas.Text) <> 0 Then
            
            TargMap = Split(a(i), vbCrLf)(1)
            TargMap = Replace(DummyMPQPath & "\", "\\", "\") & TargMap
            
            If Exist(TargMap) Then
                
                DebugPrint "Reading source ADT-map file from " & TargMap
                
                Set adt = New ADTFile
                adt.OpenADT TargMap
            
                With ListView1.ListItems
                
                    If .Item("water").Checked = True Then
                            adt.InjectWaterForAllChunks txtParam
                    End If
            
                            
                    If .Item("dood").Checked = True Then
                            For X = 1 To adt.Doodad_Count
                                adt.Doodad_DoodadZ(X) = -800
                            Next
                    End If
                    
                    If .Item("ground").Checked = True Then
                            For X = 1 To 255 + 1
                                adt.Ground_Holes(X) = Chr(0) & Chr(254) & Chr(254) & Chr(0)
                            Next
                    End If
                    
                    If .Item("killwater").Checked = True Then
                            adt.KllLiquidsForAllChunks True
                    End If
                
                End With
                
                DoEvents
                
                b = Split(TargMap, "\")
                adt.SaveADT Replace(txtTargPath.Text & "\", "\\", "\") & b(UBound(b))
                
                DebugPrint "Saved edited ADT to " & Replace(txtTargPath.Text & "\", "\\", "\") & b(UBound(b))
                
                DoEvents
                
            Else
            
                DebugPrint "Error: Map file not found!  Expected to find " & TargMap
                
                ThereWasAnError = True
                
            End If
            
        End If
        
    Next

    If ThereWasAnError Then
        
        MsgBox "There was an error loading one or more of the target .ADT map files.  Please make sure you followed the installation guide right (SetupAndInstallationGuide.html) and declaired the Dummy MPQ path", vbExclamation
    
    Else
    
        MsgBox "Done!" & vbCrLf & "ADT files have been read and saved into your output directory.  You will need to put these files in a MPQ file (i.e. patch-5.MPQ) with the same tree-path as where you got them, and place the MPQ in your WoW Data folder.", vbInformation
        
    End If
    
    End
    
End Sub

Private Sub btnWMOs_Click()

    Frame1.Visible = False
    MultiWMOEdit.Visible = True
    Me.Width = MultiWMOEdit.Width
    Me.Height = MultiWMOEdit.Height + 480
    
End Sub

Private Sub Dir1_Change()
    Doc1.Refresh
End Sub

Private Sub Dir1_Click()
    Doc1.Path = Dir1.Path
    Doc1.Refresh
End Sub

Private Sub Dir1_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    Doc1.Path = Dir1.Path
    Doc1.Refresh
End Sub

Private Sub Form_Load()
Dim i As Integer
    
    Me.Caption = "Multi Map Wizard " & App.Major & "." & App.Minor & "." & App.Revision & " by SomeWhiteGuy"
    
    ListView1.ListItems.Add , "water", "Place water everywhere and set water altitude for multiple maps"
    ListView1.ListItems.Add , "dood", "Kill all doodads on multiple maps"
    ListView1.ListItems.Add , "ground", "Erase land (the ground) on multiple maps"
    ListView1.ListItems.Add , "killwater", "Kill all liquids (water/ocean/magma) on multiple maps"
    
    ListView2.ListItems.Add , "water", "Put water everywhere (BETA TESTING)"
    ListView2.ListItems.Add , "killdoods", "Remove all doodads"
    ListView2.ListItems.Add , "solid", "Set all polygons (walls/floor/etc) as solid"
    ListView2.ListItems.Add , "notsolid", "Set all polygons (walls/floor/etc) as not-solid (lets you go through them)"
    ListView2.ListItems.Add , "delpolyface", "Delete polygons faces (BETA TESTING)"
    ListView2.ListItems.Add , "flat", "Flatten vecters (sets all Z values the same)"
    
    'populate the location-names list by copying the list from the list on GUIStart
    For i = 0 To GUIStart.lstAreas.ListCount
        
        Me.lstAreas.AddItem GUIStart.lstAreas.List(i)
        
    Next
    
    Me.Width = Frame1.Width
    Me.Height = Frame1.Height
        
End Sub

Private Sub ListView1_Click()
    
    txtParam.Visible = False
    Label3(0).Visible = False
    
    If ListView1.ListItems.Item("water").Checked = True Then
        txtParam.Visible = True
        Label3(0).Visible = True
    End If
    
End Sub


Private Sub Timer1_Timer()
    Doc1.Path = Dir1.Path
    Doc1.Refresh
End Sub
