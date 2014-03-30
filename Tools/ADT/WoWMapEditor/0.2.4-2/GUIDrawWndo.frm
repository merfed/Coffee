VERSION 5.00
Begin VB.Form GUIViewTop_ADT 
   Caption         =   "Privew (X,Y) placements"
   ClientHeight    =   7530
   ClientLeft      =   165
   ClientTop       =   855
   ClientWidth     =   11985
   LinkTopic       =   "Form2"
   ScaleHeight     =   7530
   ScaleWidth      =   11985
   StartUpPosition =   3  'Windows Default
   Begin VB.PictureBox CacheWMORender 
      Appearance      =   0  'Flat
      AutoRedraw      =   -1  'True
      BackColor       =   &H80000005&
      BorderStyle     =   0  'None
      ForeColor       =   &H80000008&
      Height          =   495
      Index           =   0
      Left            =   120
      ScaleHeight     =   495
      ScaleWidth      =   495
      TabIndex        =   5
      Top             =   240
      Visible         =   0   'False
      Width           =   495
   End
   Begin VB.Frame FrameDrawWater 
      Height          =   2055
      Left            =   120
      TabIndex        =   6
      Top             =   120
      Visible         =   0   'False
      Width           =   10335
      Begin VB.CommandButton btnDoneDrawingWater 
         Caption         =   "Done"
         Height          =   495
         Left            =   7920
         TabIndex        =   10
         Top             =   1440
         Width           =   2295
      End
      Begin VB.TextBox txtWaterHeight 
         Height          =   285
         Left            =   1320
         TabIndex        =   9
         Text            =   "500"
         Top             =   1560
         Width           =   1815
      End
      Begin VB.TextBox Text1 
         BackColor       =   &H8000000F&
         BorderStyle     =   0  'None
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   1215
         Index           =   2
         Left            =   240
         Locked          =   -1  'True
         MultiLine       =   -1  'True
         TabIndex        =   7
         Text            =   "GUIDrawWndo.frx":0000
         Top             =   240
         Width           =   9855
      End
      Begin VB.Label Label1 
         Caption         =   "Water height:"
         Height          =   255
         Index           =   0
         Left            =   240
         TabIndex        =   8
         Top             =   1560
         Width           =   1095
      End
   End
   Begin VB.Frame Frame1 
      Height          =   2055
      Left            =   120
      TabIndex        =   1
      Top             =   120
      Width           =   10335
      Begin VB.TextBox Text1 
         BackColor       =   &H8000000F&
         Height          =   735
         Index           =   1
         Left            =   6360
         Locked          =   -1  'True
         MultiLine       =   -1  'True
         TabIndex        =   4
         Text            =   "GUIDrawWndo.frx":0193
         Top             =   240
         Width           =   3855
      End
      Begin VB.TextBox txtDisplay 
         BackColor       =   &H8000000F&
         Height          =   855
         Left            =   120
         Locked          =   -1  'True
         MultiLine       =   -1  'True
         TabIndex        =   2
         Top             =   1080
         Width           =   10095
      End
      Begin VB.TextBox Text1 
         BackColor       =   &H8000000F&
         BorderStyle     =   0  'None
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   9.75
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   855
         Index           =   0
         Left            =   120
         Locked          =   -1  'True
         MultiLine       =   -1  'True
         TabIndex        =   3
         Text            =   "GUIDrawWndo.frx":01F7
         Top             =   240
         Width           =   8655
      End
   End
   Begin VB.PictureBox DrawBox 
      Appearance      =   0  'Flat
      AutoRedraw      =   -1  'True
      ForeColor       =   &H80000008&
      Height          =   13080
      Left            =   120
      ScaleHeight     =   13050
      ScaleWidth      =   17370
      TabIndex        =   0
      Top             =   2280
      Width           =   17400
      Begin VB.Timer tmrKillArrow 
         Enabled         =   0   'False
         Interval        =   3000
         Left            =   1080
         Top             =   120
      End
      Begin VB.Image Arrow 
         Height          =   825
         Left            =   120
         Picture         =   "GUIDrawWndo.frx":0274
         Top             =   120
         Visible         =   0   'False
         Width           =   750
      End
   End
   Begin VB.Menu Mnu_ 
      Caption         =   "Mnu_"
      Begin VB.Menu Mnu_Targ_ 
         Caption         =   "Target"
         Begin VB.Menu Mnu_Targ_Edit 
            Caption         =   "Edit Target Settings"
         End
         Begin VB.Menu Mnu_CopyXYZ 
            Caption         =   "Copy XYZ to clipboard"
         End
         Begin VB.Menu Mnu_Targ_Mv 
            Caption         =   "Move to where I click next"
         End
      End
      Begin VB.Menu Mnu_Dash1 
         Caption         =   "-"
      End
      Begin VB.Menu Mnu_DrawWater 
         Caption         =   "Draw water on map"
      End
      Begin VB.Menu Mnu_DrawWaterDone 
         Caption         =   "Done drawing water"
      End
      Begin VB.Menu Mnu_WaterSetLvl 
         Caption         =   "Set water levelfor this chunk"
         Visible         =   0   'False
      End
      Begin VB.Menu Mnu_Dash2 
         Caption         =   "-"
      End
      Begin VB.Menu MnuSelectChunk 
         Caption         =   "Select Chunk"
      End
      Begin VB.Menu MnudESelectChunk 
         Caption         =   "Deselect Chunk"
      End
      Begin VB.Menu MnuSelAct_ 
         Caption         =   "Selection Action"
         Begin VB.Menu MnuSelAct_RampWHigh 
            Caption         =   "Ramp - West High"
         End
         Begin VB.Menu MnuSelAct_RampEHigh 
            Caption         =   "Ramp - East High"
         End
      End
      Begin VB.Menu Mnu_Dash 
         Caption         =   "-"
      End
      Begin VB.Menu Mnu_GetFloorId 
         Caption         =   "Kill flooring"
      End
      Begin VB.Menu Mnu_RezFloor 
         Caption         =   "UnKill Flooring"
      End
   End
End
Attribute VB_Name = "GUIViewTop_ADT"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Public adt                  As ADTFile
Public ParentGUI            As Form


Dim MouseDwnX As Double
Dim MouseDwnY As Double

' // The following mapped nodes will contains X,Y values in percent as drawn on the DrawBox
Dim MappedNodes_X()          As Double
Dim MappedNodes_Y()         As Double
Dim MappedNodes_DoodadId()  As Long
Dim MappedNodes_WMOID()     As Long

Dim CurrentNodeSel_WMOid    As Long
Dim CurrentNodeSel_DoodId   As Long

Dim IsMoving As Boolean

Dim GlobQ_MaxX              As Double
Dim GlobQ_MaxY              As Double
Dim GlobQ_MinX              As Double
Dim GlobQ_MinY              As Double

Dim MsLstX
Dim MsLstY

Dim HasRenderedWMO          As Boolean
Dim RenderedWMOs()          As Boolean

Dim DrawingWater            As Boolean
Dim SetWater()              As Boolean

Dim ChunkSelect(16, 16)     As Boolean

Sub PointArowToWMO(WMOid As Integer)
Dim i As Integer

    For i = 0 To UBound(MappedNodes_WMOID)
        If MappedNodes_WMOID(i) = WMOid Then Exit For
    Next
    
    Arrow.Left = MappedNodes_X(i) - (Arrow.Width / 2)
    Arrow.Top = MappedNodes_Y(i) - Arrow.Height
    Arrow.Visible = True
    tmrKillArrow.Enabled = True
    
End Sub

Sub PointArowToDoodad(Did As Integer)
Dim i As Integer

    For i = 0 To UBound(MappedNodes_DoodadId)
        If MappedNodes_DoodadId(i) = Did Then Exit For
    Next
    
    Arrow.Left = MappedNodes_X(i) - (Arrow.Width / 2)
    Arrow.Top = MappedNodes_Y(i) - Arrow.Height
    Arrow.Visible = True
    tmrKillArrow.Enabled = True
    
End Sub

Sub ReDrimMappedNodesArrays(NewUBound As Long)
    ReDim Preserve MappedNodes_X(NewUBound)
    ReDim Preserve MappedNodes_Y(NewUBound)
    ReDim Preserve MappedNodes_DoodadId(NewUBound)
    ReDim Preserve MappedNodes_WMOID(NewUBound)
End Sub

Private Sub Zoom(GoIn As Boolean)
        
    If GoIn = True Then
        
        DrawBox.Left = DrawBox.Left - 2000
        DrawBox.Top = DrawBox.Top - 2000
        
        DrawBox.Width = DrawBox.Width + 4000
        DrawBox.Height = DrawBox.Height + 4000
        
        Draw
    
    Else
        
        DrawBox.Left = DrawBox.Left + 2000
        DrawBox.Top = DrawBox.Top + 2000
        
        DrawBox.Width = DrawBox.Width - 4000
        DrawBox.Height = DrawBox.Height - 4000
        
        Draw
        
    End If
    
End Sub

Private Sub btnDoneDrawingWater_Click()
    
    Me.Hide
    
    Load GUIShowMsg
    GUIShowMsg.Show
    
    FrameDrawWater.Visible = False
    Frame1.Visible = True
    btnDoneDrawingWater.Default = False
    
    Mnu_DrawWater.Visible = True
    Mnu_DrawWaterDone.Visible = False
    
    DrawingWater = False
    
    Draw
    
    Me.Show
    
    Unload GUIShowMsg
    
End Sub

Private Sub DrawBox_DblClick()
    
    Zoom True
    
End Sub

Private Sub DrawBox_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
        
    ReDim SetWater(16 * 167, 8 * 8)
        
    MouseDwnX = X
    MouseDwnY = Y
    
End Sub

Sub DrawWater()
Dim h As Integer, X As Integer, Y As Integer
Dim id As Integer, SubId As Integer
Dim C As Integer, s As Integer
Dim CX As Integer, CY As Integer
Dim SX As Integer, SY As Integer

    Dim Cnt As Integer
    
    X = 1
    Y = 1
    
    ' // Determin chunk ID
            Do
                Cnt = Cnt + 1
                
                If MsLstX < (((X * 16) / 256) * DrawBox.Width) And MsLstY < (((Y * 16) / 256) * DrawBox.Height) Then
                    Exit Do
                Else
                    X = X + 1
                End If
                
                If X > 16 Then
                    Y = Y + 1
                    X = 1
                
                End If
                
                If Cnt > 255 Then
                    MsgBox "Error"
                    Exit Sub
                End If
                
            Loop
            
            CX = X
            CY = Y
            id = Cnt
            
            
    ' // Determin sub-chunk ID (the 8x8 grid within the target MCNK chunk)
            X = 0
            Y = 0
            Cnt = 0
            Do
                Cnt = Cnt + 1
                
                'MsgBox MsLstX
                'MsgBox (((1 / 16) * DrawBox.Width) * (CX - 1)) + (((1 / 8) * (1 / 16) * DrawBox.Width) * X)
                'MsgBox DrawBox.Width
                
                If MsLstX < (((1 / 16) * DrawBox.Width) * (CX - 1)) + (((1 / 8) * (1 / 16) * DrawBox.Width) * X) And MsLstY < (((1 / 16) * DrawBox.Height) * (CY - 1)) + (((1 / 8) * (1 / 16) * DrawBox.Height) * Y) Then
                    Exit Do
                Else
                    X = X + 1
                End If
                
                If X > 16 Then
                    Y = Y + 1
                    X = 1
                End If
                
                If Cnt > 255 * 8 Then
                    MsgBox "Error"
                    Exit Sub
                End If
                
            Loop
            
            'Cnt = Cnt - ((16 * 8) * (Y - 1))
            'Cnt = Cnt - (16 * (X - 1))
            
            SX = X
            SY = Y
            SubId = Cnt
    
    ' // Draw it on map
        C = ((CY - 1) * 16) + CX
        s = ((SY - 1) * 8) + SX
        
    ' // Set the setting
        adt.SetWaterLevel2 C + 1, s + 1, txtWaterHeight
        
        Dim FromFracX As Single, FromFracY As Single, ToFracX As Single, ToFracY As Single
    
        FromFracX = ((8 * (CX - 1)) + (SX - 1)) / (16 * 8)
        FromFracY = ((8 * (CY - 1)) + (SY - 1)) / (16 * 8)
        ToFracX = ((8 * (CX - 1)) + SX) / (16 * 8)
        ToFracY = ((8 * (CY - 1)) + SY) / (16 * 8)
        
        For X = FromFracX * DrawBox.Width To ToFracX * DrawBox.Width Step Screen.TwipsPerPixelX
        For Y = FromFracY * DrawBox.Height To ToFracY * DrawBox.Height Step Screen.TwipsPerPixelY
            
            DrawBox.PSet (X, Y), RGB(0, 255, 255)
            
        Next
        Next
        
        DrawBox.Picture = DrawBox.Image
    
    
End Sub

Private Sub DrawBox_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
Dim i As Long, dist As Double
Dim ShortestDistLen As Double, ShortestDistId As Long
Dim myX As Long, myY As Long, id As Long
Dim MsPcntX As Double, MsPcntY As Double
    
    MsLstX = X
    MsLstY = Y
    
    If Button = vbRightButton And DrawingWater = True Then
        DrawWater
        Exit Sub
    End If
    
    
    If IsMoving Then
        DrawBox.Cls

        txtDisplay = "Current Pos: {X=" & (GlobQ_MaxX - GlobQ_MinX) * MsPcntX & "; Y=" & (GlobQ_MaxY - GlobQ_MinY) * MsPcntY & "}"
        txtDisplay = txtDisplay & vbCrLf & "Click when you have chosen your location to place this object"
        
        Exit Sub
    End If
        
    If Button <> 0 Then
        DrawBox.Left = DrawBox.Left + ((X) - MouseDwnX)
        DrawBox.Top = DrawBox.Top + ((Y) - MouseDwnY)
    Else
        
        ShortestDistLen = 70000000
            
        For i = 1 To UBound(MappedNodes_X)
        
                dist = Distance(X, Y, MappedNodes_X(i), MappedNodes_Y(i))
                
                If dist < ShortestDistLen Then
                    ShortestDistId = i
                    ShortestDistLen = dist
                End If
        Next
            
        myX = MappedNodes_X(ShortestDistId)
        myY = MappedNodes_Y(ShortestDistId)
        
        DrawBox.Cls
        DrawBox.DrawWidth = 2
        DrawBox.Line (myX, myY)-(X, Y), vbGreen
        DrawBox.DrawWidth = 8
        DrawBox.Line (myX, myY)-(myX + 1, myY + 1), vbYellow
        DrawBox.Refresh
            
        If MappedNodes_DoodadId(ShortestDistId) <> -1 Then
            id = MappedNodes_DoodadId(ShortestDistId) + 1
            txtDisplay = "Doodad Model Path = " & adt.Doodad_DoodadPath(id) & vbCrLf & _
                         "Doodad {X,Y,Z} = " & Round(adt.Doodad_DoodadX(id), 1) & ", " & Round(adt.Doodad_DoodadY(id), 1) & ", " & Round(adt.Doodad_DoodadZ(id), 1)
            CurrentNodeSel_WMOid = -1
            CurrentNodeSel_DoodId = id
    
        Else
            id = MappedNodes_WMOID(ShortestDistId)
            txtDisplay = ".WMO Model Path = " & adt.WMO_WMOPath(id) & vbCrLf & _
                         "WMO {X,Y,Z} = " & Round(adt.WMO_X(id), 1) & ", " & Round(adt.WMO_Y(id), 1) & ", " & Round(adt.WMO_Z(id), 1)
            CurrentNodeSel_WMOid = id
            CurrentNodeSel_DoodId = -1
        End If
            
        If DoesHoverCellHaveHoles <> -1 Then
            If InStr(1, txtDisplay, " has holes") = 0 Then
                txtDisplay = txtDisplay & vbCrLf & "This map-chunk has holes in the ground!  ID is " & DoesHoverCellHaveHoles
            End If
        End If
        
    End If
    
End Sub

Function Distance(ByVal X1 As Long, ByVal Y1 As Long, ByVal X2 As Long, ByVal Y2 As Long) As Double

    Distance = Sqr(((X2 - X1) * (X2 - X1)) + ((Y2 - Y1) * (Y2 - Y1)))

End Function

Sub RenderWMOsAndCacheThem()
Dim i As Integer, p As String, s As String, a() As String
Dim WMORenderer As GUIWMODraw, w As WMOGrpFile

    If DummyMPQPath = "" Then DebugPrint "No DummyMPQPath declaired, not rendering .WMOs": Exit Sub
    If HasRenderedWMO = True Then DebugPrint "Not re-rendering .WMOs, they should be cached":  Exit Sub
    
    HasRenderedWMO = True
    DebugPrint "Sub:RenderWMOsAndCacheThem() has been called"
    
    Load GUIShowMsg
    GUIShowMsg.Show
    
    GUIShowMsg.Bar1.Max = adt.WMO_Count
    
    For i = 1 To adt.WMO_Count
        
        GUIShowMsg.Bar1 = i
        GUIShowMsg.Refresh
        
        p = adt.WMO_WMOPath(i)
        p = Replace(DummyMPQPath & "\" & p, "\\", "\")
        
        If Exist(p) Then
            
            p = Replace(p, ".wmo", "_000.wmo")
            DebugPrint "Doing basic/choppy render of " & p
            
            Set w = New WMOGrpFile
            w.OpenWMO p
            
            Set WMORenderer = New GUIWMODraw
            WMORenderer.Hide
            DoEvents
            Set WMORenderer.WMO = w
            
            ReDim Preserve RenderedWMOs(i)
            Load CacheWMORender(i)
            CacheWMORender(i).Picture = WMORenderer.GetAsImage(-1, adt.WMO_RotA(i), adt.WMO_RotB(i), adt.WMO_RotC(i))
            CacheWMORender(i).Width = WMORenderer.DrawBox.Width
            CacheWMORender(i).Height = WMORenderer.DrawBox.Height
            RenderedWMOs(i) = True
            
        Else
        
            DebugPrint "Cout not find .WMO " & p
            
        End If
        
    Next
    
    GUIShowMsg.Bar1.Value = 0
    Unload GUIShowMsg
    
    DebugPrint "Sub:RenderWMOsAndCacheThem() has ended"
    
End Sub

Sub Draw()
Dim i As Long, X As Long, Y As Long
Dim MaxX As Double, MaxY As Double, MinX As Double, MinY As Double
Dim XPercent As Double, YPercent As Double
Dim HideDebugger As Boolean

    MinX = 70000
    MinY = 70000
    
    DrawBox.Cls
    DrawBox.Picture = Nothing
    
    If DBugWndoShown = False Then HideDebugger = True
    Load GUIDebug
    GUIDebug.Show
    
    RenderWMOsAndCacheThem
    
    ReDrimMappedNodesArrays adt.Doodad_Count
    
    Load GUIShowMsg
    GUIShowMsg.Show
    
' // Draw water
    Dim C As Integer, s As Integer
    Dim CX As Integer, CY As Integer
    Dim SX As Integer, SY As Integer
    Dim FromFracX As Single, ToFracX As Single
    Dim FromFracY As Single, ToFracY As Single
    
    GUIShowMsg.Bar1.Max = 16385
    GUIShowMsg.Bar1.Value = 0
    
    For CX = 1 To 16
    For CY = 1 To 16
        
        DebugPrint "Checking for water to draw in MCNK chunk " & CX & "," & CY
        
        For SX = 1 To 8
        For SY = 1 To 8
            
            GUIShowMsg.Bar1.Value = GUIShowMsg.Bar1.Value + 1
            
            C = ((CY - 1) * 16) + CX
            s = ((SY - 1) * 8) + SX
            
            If adt.HasWater(C, s) = True Then
            
                FromFracX = ((8 * (CX - 1)) + (SX - 1)) / (16 * 8)
                FromFracY = ((8 * (CY - 1)) + (SY - 1)) / (16 * 8)
                ToFracX = ((8 * (CX - 1)) + SX) / (16 * 8)
                ToFracY = ((8 * (CY - 1)) + SY) / (16 * 8)
                
                For X = FromFracX * DrawBox.Width To ToFracX * DrawBox.Width Step Screen.TwipsPerPixelX
                For Y = FromFracY * DrawBox.Height To ToFracY * DrawBox.Height Step Screen.TwipsPerPixelY
                    
                    DrawBox.PSet (X, Y), RGB(0, 255, 255)
                    
                Next
                Next
                
                DoEvents
            
            Else
            
                DoEvents
                
            End If
            
        Next
        Next
        
    Next
    Next
    
    GUIShowMsg.Bar1.Value = 0
    
'// Get the smallest and greatest X & Y so we know our scope

            For i = 1 To adt.Doodad_Count
                
                X = adt.Doodad_DoodadX(i)
                Y = adt.Doodad_DoodadY(i)
                
                If X > MaxX Then MaxX = X
                If Y > MaxY Then MaxY = Y
                If 0 < X And X < MinX Then MinX = X
                If 0 < Y And Y < MinY Then MinY = Y
                
            Next
            
            For i = 1 To adt.WMO_Count
                
                X = adt.WMO_X(i)
                Y = adt.WMO_Y(i)
                
                If X > MaxX Then MaxX = X
                If Y > MaxY Then MaxY = Y
                If 0 < X And X < MinX Then MinX = X
                If 0 < Y And Y < MinY Then MinY = Y
                
            Next
            
            GlobQ_MaxX = MaxX
            GlobQ_MaxY = MaxY
            GlobQ_MinX = MinX
            GlobQ_MinY = MinY
        
'// Draw using percents to ensure somewhat-correct scale

        For i = 1 To adt.Doodad_Count
        
            XPercent = (adt.Doodad_DoodadX(i) - MinX) / (MaxX - MinX)
            YPercent = (adt.Doodad_DoodadY(i) - MinY) / (MaxY - MinY)
            
            X = XPercent * DrawBox.Width
            Y = YPercent * DrawBox.Height
            
            DrawBox.DrawWidth = 3
            DrawBox.Line (X, Y)-(X + 1, Y + 1), vbBlack
            
            If X > 0 And Y > 0 Then
                MappedNodes_X(i) = X
                MappedNodes_Y(i) = Y
                MappedNodes_DoodadId(i) = i
                MappedNodes_WMOID(i) = -1
            End If
        
        Next
        
        For i = 1 To adt.WMO_Count
       
            XPercent = 0
            If (MaxX - MinX) Then XPercent = (adt.WMO_X(i) - MinX) / (MaxX - MinX)
            
            YPercent = 0
            If (MaxY - MinY) <> 0 Then YPercent = (adt.WMO_Y(i) - MinY) / (MaxY - MinY)
       
            X = XPercent * DrawBox.Width
            Y = YPercent * DrawBox.Height
       
            ' // If we can, draw rendered version
                If DummyMPQPath <> "" Then
                If UBound(RenderedWMOs) > i Then
                If CacheWMORender.UBound >= i And RenderedWMOs(i) = True Then
                    DrawBox.PaintPicture CacheWMORender(i), X - (CacheWMORender(i).ScaleWidth / 2), Y - (CacheWMORender(i).ScaleHeight / 2)
                End If
                End If
                End If
                
            ' // Mark center location of .WMO
                DrawBox.DrawWidth = 11
                DrawBox.Line (X, Y)-(X + 1, Y + 1), vbBlue
       
            If X > 0 And Y > 0 Then
                MappedNodes_X(i) = X
                MappedNodes_Y(i) = Y
                MappedNodes_DoodadId(i) = -1
                MappedNodes_WMOID(i) = i
            End If
       
        Next


'// Draw .adt chunk borders
    
        DrawBox.DrawWidth = 1
        For i = 1 To 16
            X = ((i * 16) / 256) * DrawBox.Width
            DrawBox.Line (X, 0)-(X, DrawBox.Height), vbRed
            Y = ((i * 16) / 256) * DrawBox.Height
            DrawBox.Line (0, Y)-(DrawBox.Width, Y), vbRed
        Next
        
' // Hilight selected chunks
        Dim K As Integer, R As Integer
        
        For X = 1 To 16
        For Y = 1 To 16
        
             If ChunkSelect(X, Y) = True Then
                
                For K = (1 / 16) * DrawBox.Width * (X - 1) To (1 / 16) * DrawBox.Width * X Step 2 * Screen.TwipsPerPixelX
                For R = (1 / 16) * DrawBox.Height * (Y - 1) To (1 / 16) * DrawBox.Height * Y Step 2 * Screen.TwipsPerPixelY
                    
                    DrawBox.DrawWidth = 2
                    DrawBox.Line (K, R)-(K + 1, R + 1), vbGreen
                    
                Next
                Next
                
             End If
            
        Next
        Next

'// Update/Repaint
        DrawBox.Refresh
        Me.Refresh
        DrawBox.Picture = DrawBox.Image
        DrawBox.Cls
        
        Unload GUIShowMsg
        
        If HideDebugger Then
            Unload GUIDebug
        End If

End Sub

Private Sub DrawBox_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    Dim NewX As Double, NewY As Double
    Dim i As Long, dist As Double
    Dim ShortestDistLen As Double, ShortestDistId As Long
    Dim myX As Long, myY As Long, id As Long
    Dim MsPcntX As Double, MsPcntY As Double
    
    If IsMoving Then
    
        ' // then we are done moving the thing, set the new Qs
        
        DrawBox.Cls
        
        MsPcntX = X / DrawBox.Width
        MsPcntY = Y / DrawBox.Height
        
        NewX = ((GlobQ_MaxX - GlobQ_MinX) * MsPcntX) + GlobQ_MinX
        NewY = ((GlobQ_MaxY - GlobQ_MinY) * MsPcntY) + GlobQ_MinY
        
        txtDisplay = "Current Pos: {X=" & NewX & "; Y=" & NewY & "}"
        txtDisplay = txtDisplay & vbCrLf & "Click when you have chosen your location to place this object"
        
        If CurrentNodeSel_DoodId <> -1 Then
            adt.Doodad_DoodadX(CurrentNodeSel_DoodId) = NewX
            adt.Doodad_DoodadY(CurrentNodeSel_DoodId) = NewY
        Else
            adt.WMO_X(CurrentNodeSel_WMOid) = NewX
            adt.WMO_Y(CurrentNodeSel_WMOid) = NewY
        End If
        
        IsMoving = False
        Draw
        
        Exit Sub
    
    End If
    
    If Button = vbRightButton And DrawingWater = False Then PopupMenu Mnu_
    If Button = vbLeftButton And Shift = 1 Then Zoom False
    
End Sub

Private Sub Form_Load()
    
    ReDim MappedNodes_X(0)
    ReDim MappedNodes_Y(0)
    ReDim MappedNodes_DoodadId(0)
    ReDim MappedNodes_WMOID(0)
    
    DrawBox.Left = 0
    DrawBox.Top = 0
    
    HasRenderedWMO = False
    
    Mnu_.Visible = False
    Mnu_DrawWaterDone.Visible = False
    
End Sub

Private Sub Form_Unload(Cancel As Integer)
    
    If Not ParentGUI Is Nothing Then
        Me.Hide
        
        Load GUIShowMsg
        GUIShowMsg.Show
        GUIShowMsg.Refresh
        DoEvents
        
        ParentGUI.DisplayDataromBuffer_Doodads
        ParentGUI.DisplayDataromBuffer_WMOs
        
        ParentGUI.Show
        Unload GUIShowMsg
        Unload Me
        
    End If
    
End Sub

Private Sub Mnu_CopyXYZ_Click()
    Clipboard.Clear
    
    If CurrentNodeSel_DoodId <> -1 Then
        Clipboard.SetText adt.Doodad_DoodadX(CurrentNodeSel_DoodId) & ", " & adt.Doodad_DoodadY(CurrentNodeSel_DoodId) & ", " & adt.Doodad_DoodadZ(CurrentNodeSel_DoodId)
    Else
        Clipboard.SetText adt.WMO_X(CurrentNodeSel_WMOid) & ", " & adt.WMO_Y(CurrentNodeSel_WMOid) & ", " & adt.Doodad_DoodadZ(CurrentNodeSel_WMOid)
    End If
End Sub

Function DoesHoverCellHaveHoles() As Double
Dim X As Integer, Y As Integer, i As Integer, id As Double
    
    ' // Returns the id of the current cell on true, and -1 on false
    
    Dim Cnt As Integer
    
    X = 1
    Y = 1
    
    Do
        Cnt = Cnt + 1
        
        If MsLstX < (((X * 16) / 256) * DrawBox.Width) And MsLstY < (((Y * 16) / 256) * DrawBox.Height) Then
            Exit Do
        Else
            X = X + 1
        End If
        
        If X > 16 Then
            Y = Y + 1
            X = 1
        
        End If
        
        If Cnt > 255 Then
            MsgBox "Error"
            Exit Function
        End If
        
    Loop
    
    id = Cnt
    
    If adt.Ground_Holes(id) = Chr(0) & Chr(0) & Chr(0) & Chr(0) Then
        DoesHoverCellHaveHoles = -1
    Else
        DoesHoverCellHaveHoles = id
    End If
    
End Function

Private Sub Mnu_DrawWater_Click()
    FrameDrawWater.Visible = True
    Frame1.Visible = False
    ReDim SetWater(16 * 16, 8 * 8)
    DrawingWater = True
    Mnu_DrawWater.Visible = False
    Mnu_DrawWaterDone.Visible = True
    btnDoneDrawingWater.Default = True
End Sub

Private Sub Mnu_DrawWaterDone_Click()
    btnDoneDrawingWater_Click
End Sub

Private Sub Mnu_GetFloorId_Click()
Dim X As Integer, Y As Integer, i As Integer, id As Integer

    If MsgBox("Warning: You are about to put holes in the ground here." & vbCrLf & "Continue?", vbOKCancel Or vbInformation) = vbCancel Then Exit Sub
    Dim Cnt As Integer
    
    X = 1
    Y = 1
    
    Do
        Cnt = Cnt + 1
        
        If MsLstX < (((X * 16) / 256) * DrawBox.Width) And MsLstY < (((Y * 16) / 256) * DrawBox.Height) Then
            Exit Do
        Else
            X = X + 1
        End If
        
        If X > 16 Then
            Y = Y + 1
            X = 1
        
        End If
        
        If Cnt > 255 Then
            MsgBox "Error"
            Exit Sub
        End If
        
    Loop
    
    id = Cnt
    
    adt.Ground_Holes(id) = Chr(0) & Chr(254) & Chr(254) & Chr(0)
    
End Sub

Private Sub Mnu_RezFloor_Click()
Dim X As Integer, Y As Integer, i As Integer, id As Integer

    Dim Cnt As Integer
    
    X = 1
    Y = 1
    
    Do
        Cnt = Cnt + 1
        
        If MsLstX < (((X * 16) / 256) * DrawBox.Width) And MsLstY < (((Y * 16) / 256) * DrawBox.Height) Then
            Exit Do
        Else
            X = X + 1
        End If
        
        If X > 16 Then
            Y = Y + 1
            X = 1
        
        End If
        
        If Cnt > 255 Then
            MsgBox "Error"
            Exit Sub
        End If
        
    Loop
    
    id = Cnt
    
    adt.Ground_Holes(id) = Chr(0) & Chr(0) & Chr(0) & Chr(0)
    
End Sub

Private Sub Mnu_Targ_Edit_Click()
Dim i As Integer

    Me.Hide
    ParentGUI.Show

    If CurrentNodeSel_DoodId <> -1 Then
        
        ParentGUI.Toolbar1_ButtonClick GUI_ADTFile.Toolbar1.Buttons(1)
        
        With ParentGUI.ListView1.ListItems
            For i = 1 To .Count
            
                .Item(i).Selected = False
                
                If .Item(i).Tag = CurrentNodeSel_DoodId Then
                    .Item(i).Selected = True
                End If
                
            Next
        End With
        
        ParentGUI.ListView1.SetFocus
        
    Else
    
        ParentGUI.Toolbar1_ButtonClick GUI_ADTFile.Toolbar1.Buttons(2)
        
        With ParentGUI.WMOListView.ListItems
            For i = 1 To .Count
            
                .Item(i).Selected = False
                
                If .Item(i).Tag = CurrentNodeSel_WMOid Then
                    .Item(i).Selected = True
                End If
                
            Next
        End With
        
        ParentGUI.WMOListView.SetFocus
    
    End If
    
    Unload Me
        
End Sub

Private Sub Mnu_Targ_Mv_Click()
    
    IsMoving = True
    
    
End Sub

Private Sub Mnu_WaterSetLvl_Click()
Dim h As Integer, X As Integer, Y As Integer, id As Integer

    h = InputBox("How high should the water rise for this chunk?", , 500.7)
    Dim Cnt As Integer
    
    X = 1
    Y = 1
    
    Do
        Cnt = Cnt + 1
        
        If MsLstX < (((X * 16) / 256) * DrawBox.Width) And MsLstY < (((Y * 16) / 256) * DrawBox.Height) Then
            Exit Do
        Else
            X = X + 1
        End If
        
        If X > 16 Then
            Y = Y + 1
            X = 1
        
        End If
        
        If Cnt > 255 Then
            MsgBox "Error"
            Exit Sub
        End If
        
    Loop
    
    id = Cnt

End Sub

Private Sub MnudESelectChunk_Click()
Dim X As Integer, Y As Integer, i As Integer, id As Integer
Dim Cnt As Integer
    
    X = 1
    Y = 1
    
    Do
        Cnt = Cnt + 1
        
        If MsLstX < (((X * 16) / 256) * DrawBox.Width) And MsLstY < (((Y * 16) / 256) * DrawBox.Height) Then
            Exit Do
        Else
            X = X + 1
        End If
        
        If X > 16 Then
            Y = Y + 1
            X = 1
        
        End If
        
        If Cnt > 255 Then
            MsgBox "Error"
            Exit Sub
        End If
        
    Loop
    
    ChunkSelect(X, Y) = False
    Draw
    
End Sub

Private Sub MnuSelAct_RampEHigh_Click()
Dim X As Integer, Y As Integer, i As Integer, id As Integer
Dim Cnt As Integer
    
    X = 1
    Y = 1
    
    Do
        Cnt = Cnt + 1
        
        If MsLstX < (((X * 16) / 256) * DrawBox.Width) And MsLstY < (((Y * 16) / 256) * DrawBox.Height) Then
            Exit Do
        Else
            X = X + 1
        End If
        
        If X > 16 Then
            Y = Y + 1
            X = 1
        
        End If
        
        If Cnt > 255 Then
            MsgBox "Error"
            Exit Sub
        End If
        
    Loop
    



Dim s As String, n As Integer
Dim AvgRampSide As Single, AvgBase As Single, PlusToRamp As Single

    s = InputBox("How high would you like to push up the east side of your ramp?")
    If IsNumeric(s) = False Then
        MsgBox "Error, input not numberic", vbExclamation
        Exit Sub
    End If
    n = s
    
    For Y = 1 To 9
        AvgRampSide = AvgRampSide + adt.HeightMap_Outter9x9(Cnt, 9, Y)
    Next
    AvgRampSide = AvgRampSide / 9
    AvgRampSide = AvgRampSide + n
    
    For Y = 1 To 9
        AvgBase = AvgBase + adt.HeightMap_Outter9x9(Cnt, 1, Y)
    Next
    AvgBase = AvgBase / 9
    
    PlusToRamp = AvgRampSide - AvgBase
    
    For X = 1 To 9
    For Y = 1 To 9
        
        adt.HeightMap_Outter9x9(Cnt, X, Y) = AvgBase + (((1 / 9) * PlusToRamp) * X)
        
    Next
    Next
    
    For X = 1 To 8
    For Y = 1 To 8
        
        adt.HeightMap_Outter9x9(Cnt, X, Y) = AvgBase + (((1 / 8) * PlusToRamp) * X)
        
    Next
    Next
    
    MsgBox "Done"
    
End Sub

Private Sub MnuSelAct_RampWHigh_Click()
Dim X As Integer, Y As Integer, i As Integer, id As Integer
Dim Cnt As Integer
    
    X = 1
    Y = 1
    
    Do
        Cnt = Cnt + 1
        
        If MsLstX < (((X * 16) / 256) * DrawBox.Width) And MsLstY < (((Y * 16) / 256) * DrawBox.Height) Then
            Exit Do
        Else
            X = X + 1
        End If
        
        If X > 16 Then
            Y = Y + 1
            X = 1
        
        End If
        
        If Cnt > 255 Then
            MsgBox "Error"
            Exit Sub
        End If
        
    Loop
    



Dim s As String, n As Integer
Dim AvgRampSide As Single, AvgBase As Single, PlusToRamp As Single

    s = InputBox("How high would you like to push up the west side of your ramp?")
    If IsNumeric(s) = False Then
        MsgBox "Error, input not numberic", vbExclamation
        Exit Sub
    End If
    n = s
    
    For Y = 1 To 9
        AvgRampSide = AvgRampSide + adt.HeightMap_Outter9x9(Cnt, 1, Y)
    Next
    AvgRampSide = AvgRampSide / 9
    AvgRampSide = AvgRampSide + n
    
    For Y = 1 To 9
        AvgBase = AvgBase + adt.HeightMap_Outter9x9(Cnt, 9, Y)
    Next
    AvgBase = AvgBase / 9
    
    PlusToRamp = AvgRampSide - AvgBase
    
    For X = 1 To 9
    For Y = 1 To 9
        
        adt.HeightMap_Outter9x9(Cnt, X, Y) = AvgBase + (((1 / 9) * PlusToRamp) * (9 - X))
        
    Next
    Next
    
    For X = 1 To 8
    For Y = 1 To 8
        
        adt.HeightMap_Outter9x9(Cnt, X, Y) = AvgBase + (((1 / 8) * PlusToRamp) * (8 - X))
        
    Next
    Next
    
    MsgBox "Done"
    
End Sub

Private Sub MnuSelectChunk_Click()
Dim X As Integer, Y As Integer, i As Integer, id As Integer
Dim Cnt As Integer
    
    X = 1
    Y = 1
    
    Do
        Cnt = Cnt + 1
        
        If MsLstX < (((X * 16) / 256) * DrawBox.Width) And MsLstY < (((Y * 16) / 256) * DrawBox.Height) Then
            Exit Do
        Else
            X = X + 1
        End If
        
        If X > 16 Then
            Y = Y + 1
            X = 1
        
        End If
        
        If Cnt > 255 Then
            MsgBox "Error"
            Exit Sub
        End If
        
    Loop
    
    ChunkSelect(X, Y) = True
    MsgBox "Chunk " & Cnt & " selected"
    Draw
    
End Sub

Private Sub tmrKillArrow_Timer()
    
    Arrow.Visible = False
    tmrKillArrow.Enabled = False
    
End Sub
