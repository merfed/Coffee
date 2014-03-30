VERSION 5.00
Begin VB.Form GUIWMODraw 
   BackColor       =   &H00FFFFFF&
   Caption         =   ".WMO Simple Reder - TOP VIEW (looking down on object)"
   ClientHeight    =   11055
   ClientLeft      =   165
   ClientTop       =   855
   ClientWidth     =   14280
   LinkTopic       =   "Form2"
   ScaleHeight     =   11055
   ScaleWidth      =   14280
   StartUpPosition =   3  'Windows Default
   Begin VB.Frame Frame1 
      Caption         =   "Controles"
      Height          =   2295
      Left            =   3000
      TabIndex        =   1
      Top             =   7560
      Width           =   8655
      Begin VB.ListBox List2 
         Height          =   1230
         Left            =   7320
         TabIndex        =   16
         Top             =   480
         Width           =   975
      End
      Begin VB.CommandButton btnHideSide 
         Caption         =   "<<   Hide"
         Height          =   375
         Left            =   6240
         TabIndex        =   15
         Top             =   1800
         Width           =   2055
      End
      Begin VB.ListBox List1 
         Height          =   1230
         Left            =   6240
         TabIndex        =   14
         Top             =   480
         Width           =   975
      End
      Begin VB.CommandButton btnRotRight 
         Caption         =   "D"
         Height          =   375
         Left            =   1560
         TabIndex        =   13
         Top             =   840
         Width           =   615
      End
      Begin VB.CommandButton btnShowMnu 
         Caption         =   "Options"
         Height          =   495
         Left            =   4680
         TabIndex        =   12
         Top             =   240
         Width           =   1455
      End
      Begin VB.CheckBox chkDrawTri 
         Caption         =   "Draw triangles"
         Height          =   195
         Left            =   120
         TabIndex        =   11
         Top             =   1680
         Width           =   3735
      End
      Begin VB.CheckBox chkKeepInView 
         Caption         =   "Keep object in view (auto-shift to top-left)"
         Height          =   195
         Left            =   120
         TabIndex        =   10
         Top             =   1440
         Width           =   3735
      End
      Begin VB.TextBox Text1 
         Height          =   375
         Left            =   120
         TabIndex        =   9
         Top             =   2280
         Width           =   975
      End
      Begin VB.CommandButton BtnShiftDn 
         Caption         =   "Dn"
         Height          =   375
         Left            =   3120
         TabIndex        =   8
         Top             =   840
         Width           =   615
      End
      Begin VB.CommandButton BtnShiftUp 
         Caption         =   "^"
         Height          =   375
         Left            =   3120
         TabIndex        =   7
         Top             =   360
         Width           =   615
      End
      Begin VB.CommandButton BtnShiftR 
         Caption         =   ">"
         Height          =   375
         Left            =   3840
         TabIndex        =   6
         Top             =   840
         Width           =   615
      End
      Begin VB.CommandButton BtnShiftL 
         Caption         =   "<"
         Height          =   375
         Left            =   2400
         TabIndex        =   5
         Top             =   840
         Width           =   615
      End
      Begin VB.CommandButton Command2 
         Caption         =   "W"
         Height          =   375
         Left            =   840
         TabIndex        =   4
         Top             =   360
         Width           =   615
      End
      Begin VB.CommandButton btnRotD 
         Caption         =   "S"
         Height          =   375
         Left            =   840
         TabIndex        =   3
         Top             =   840
         Width           =   615
      End
      Begin VB.CommandButton btnRotL 
         Caption         =   "A"
         Height          =   375
         Left            =   120
         TabIndex        =   2
         Top             =   840
         Width           =   615
      End
      Begin VB.Label Label2 
         Caption         =   "Plane types"
         Height          =   255
         Left            =   7320
         TabIndex        =   18
         Top             =   240
         Width           =   1095
      End
      Begin VB.Label Label1 
         Caption         =   "Flags"
         Height          =   255
         Left            =   6240
         TabIndex        =   17
         Top             =   240
         Width           =   855
      End
   End
   Begin VB.PictureBox DrawBox 
      AutoRedraw      =   -1  'True
      BorderStyle     =   0  'None
      Height          =   19575
      Left            =   0
      ScaleHeight     =   19575
      ScaleWidth      =   27975
      TabIndex        =   0
      Top             =   0
      Width           =   27975
   End
   Begin VB.Menu Mnu_ 
      Caption         =   "Mnu_"
      Begin VB.Menu Mnu_HideSel 
         Caption         =   "Hide Selected Points (only for selection purposes)"
      End
      Begin VB.Menu Mnu_UnHideAll 
         Caption         =   "Un-Hide All"
      End
      Begin VB.Menu Mnu_SelectAll 
         Caption         =   "Select All"
      End
      Begin VB.Menu Mnu_Dash0 
         Caption         =   "-"
      End
      Begin VB.Menu Mnu_RendAll 
         Caption         =   "Render all"
         Checked         =   -1  'True
      End
      Begin VB.Menu Mnu_RendSelFlagVals 
         Caption         =   "Render select flag values only"
      End
      Begin VB.Menu Mnu_Dash 
         Caption         =   "-"
      End
      Begin VB.Menu Mnu_ViewFlagList 
         Caption         =   "View list of flag and plane values being used in this .wmo"
      End
      Begin VB.Menu Mnu_SetFlags 
         Caption         =   "Set flags...."
         Begin VB.Menu Mnu_SFlagSomeVert 
            Caption         =   "....for any triangle connected to a ANY vertex that is..."
            Begin VB.Menu Mnu_FlagSetForSel 
               Caption         =   "....selected"
            End
            Begin VB.Menu Mnu_FlagSetForVis 
               Caption         =   "....visible"
            End
         End
         Begin VB.Menu Mnu_SFlagAllVert 
            Caption         =   "....for any triangle that has ALL (3) verticies..."
            Begin VB.Menu Mnu_FlagSetForAllSel 
               Caption         =   "....selected"
            End
            Begin VB.Menu Mnu_FlagSetForAllVis 
               Caption         =   "....visible"
            End
         End
         Begin VB.Menu Mnu_RepFlagValue 
            Caption         =   "For ALL triangles, replace a flag-value of # with #"
         End
      End
      Begin VB.Menu Mnu_Plane 
         Caption         =   "Set plane types..."
         Begin VB.Menu Mnu_PlaneSelectForAllSel 
            Caption         =   "Set all planes to #"
         End
         Begin VB.Menu Mnu_PlaneRep 
            Caption         =   "Replace all plane values of # with #"
         End
         Begin VB.Menu Mnu_PlaneVanish 
            Caption         =   "Set all face counts to 0 (makes everything vanish)"
         End
      End
      Begin VB.Menu mnuVectors_ 
         Caption         =   "Vectors"
         Begin VB.Menu mnuVectors_flatten 
            Caption         =   "Flatten (set all Z valus to same)"
         End
      End
      Begin VB.Menu Mnu_Dash2 
         Caption         =   "-"
      End
      Begin VB.Menu Mnu_ShiftX 
         Caption         =   "For all selected vertices, shift X"
      End
      Begin VB.Menu Mnu_ShiftY 
         Caption         =   "For all selected vertices, shift Y"
      End
      Begin VB.Menu Mnu_ShiftZ 
         Caption         =   "For all selected vertices, shift Z"
      End
      Begin VB.Menu Mnu_Dash3 
         Caption         =   "-"
      End
      Begin VB.Menu Mnu_Save 
         Caption         =   "Save As..."
      End
      Begin VB.Menu Mnu_test 
         Caption         =   "test (dev thing users shouldnt see this)"
      End
   End
End
Attribute VB_Name = "GUIWMODraw"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Public WMO As WMOGrpFile

Public MultZoom     As Double

Dim WorldVertX()    As Double
Dim WorldVertY()    As Double
Dim WorldVertZ()    As Double
Dim WorldVertHide() As Boolean

Dim WorldLoaded     As Boolean

Public LeftEdge     As Double
Public TopEdge      As Double
Public RightEdge    As Double
Public BottomEdge   As Double

Public OffsetDrawX  As Long
Public OffsetDrawY  As Long

Dim MouseDownX      As Integer
Dim MouseDownY      As Integer
Dim MouseUpX        As Integer
Dim MouseUpY        As Integer

Dim RenderAll       As Boolean  ' Determins if we are going to plot all points, or just the ones associated with triangles that have their flags set to the same number as the selected one in the list


Function GetAsImage(ByVal PointPixleRatio As Double, ByVal a As Double, ByVal b As Double, ByVal C As Double) As IPictureDisp
    
    MultZoom = 28
    
    Draw
    
    RotA a
    RotB b
    RotC C
    
    CenterImageByOffsets
    Draw
    
    DrawBox.Width = RightEdge - LeftEdge
    DrawBox.Height = BottomEdge - TopEdge
    
    DrawBox.Picture = DrawBox.Image
    Set GetAsImage = DrawBox.Image
    
End Function

Sub LoadWorld()
Dim i As Long
    
    ReDim WorldVertX(WMO.Vertices_Count)
    ReDim WorldVertY(WMO.Vertices_Count)
    ReDim WorldVertZ(WMO.Vertices_Count)
    ReDim WorldVertHide(WMO.Vertices_Count)
    
    For i = 1 To WMO.Vertices_Count
        
        WorldVertX(i) = WMO.Vertices_X(i)
        WorldVertY(i) = WMO.Vertices_Y(i)
        WorldVertZ(i) = WMO.Vertices_Z(i)
        
    Next
    
    CenterImageByOffsets
    
End Sub

Sub CenterImageByOffsets()
Dim i As Long

    LeftEdge = 70000
    TopEdge = 70000
    RightEdge = -70000
    BottomEdge = -70000
    
    For i = 1 To WMO.Vertices_Count
        If WorldVertX(i) * MultZoom < LeftEdge Then LeftEdge = WorldVertX(i) * MultZoom
        If WorldVertY(i) * MultZoom < TopEdge Then TopEdge = WorldVertY(i) * MultZoom
        If WorldVertX(i) * MultZoom > RightEdge Then RightEdge = WorldVertX(i) * MultZoom
        If WorldVertY(i) * MultZoom > BottomEdge Then BottomEdge = WorldVertY(i) * MultZoom
    Next
    
    OffsetDrawX = LeftEdge
    OffsetDrawY = TopEdge
    
End Sub

Sub HidePointsThatDontHaveSelFlag()
Dim i As Integer

    For i = 1 To WMO.Vertices_Count
        WorldVertHide(i) = True
    Next
    
    For i = 1 To WMO.TriangleInfo_Count
        
        If CStr(WMO.TriangleInfo_Flags(i)) = List1.Text Then
            
            WorldVertHide(WMO.IVOM_1(i)) = False
            WorldVertHide(WMO.IVOM_2(i)) = False
            WorldVertHide(WMO.IVOM_3(i)) = False
            
        End If
        
    Next
    
End Sub

Sub Draw()
Dim i As Long, X As Double, Y As Double, ChosenColor As Long, DoRend As Boolean

    DrawBox.Cls
    
    If WorldLoaded = False Then
        LoadWorld
        WorldLoaded = True
    End If
    
    If RenderAll = False Then HidePointsThatDontHaveSelFlag
    
' // Draw points of vertices
    For i = 1 To WMO.Vertices_Count
        
        If WorldVertHide(i) = False Then
        
            X = WorldVertX(i)
            Y = WorldVertY(i)
                    
            X = X * MultZoom
            Y = Y * MultZoom
            
            X = X - OffsetDrawX
            Y = Y - OffsetDrawY
            
            ChosenColor = vbBlack
            
            If MouseDownX > 0 Then
            If X > MouseDownX Then
            If MouseUpX > X Then
            If Y > MouseDownY Then
            If MouseUpY > Y Then
            
                ChosenColor = vbRed
                
            End If
            End If
            End If
            End If
            End If
            
            DrawBox.DrawWidth = 3
            DrawBox.Line (X, Y)-(X + 1, Y + 1), ChosenColor
        
        End If
        
    Next
    
    DoEvents
    
    If chkDrawTri.Value = Unchecked Then Exit Sub
    
    Dim Point1X As Double, Point1Y As Double
    Dim Point2X As Double, Point2Y As Double
    Dim Point3X As Double, Point3Y As Double
    
' // Draw triangles
    For i = 1 To WMO.TriangleInfo_Count
        
        If WorldVertHide(WMO.IVOM_1(i)) = False Then
        If WorldVertHide(WMO.IVOM_2(i)) = False Then
        If WorldVertHide(WMO.IVOM_3(i)) = False Then
        
            Point1X = WorldVertX(WMO.IVOM_1(i))
            Point1Y = WorldVertY(WMO.IVOM_1(i))
            Point2X = WorldVertX(WMO.IVOM_2(i))
            Point2Y = WorldVertY(WMO.IVOM_2(i))
            Point3X = WorldVertX(WMO.IVOM_3(i))
            Point3Y = WorldVertY(WMO.IVOM_3(i))
            
            Point1X = Point1X * MultZoom
            Point1Y = Point1Y * MultZoom
            Point2X = Point2X * MultZoom
            Point2Y = Point2Y * MultZoom
            Point3X = Point3X * MultZoom
            Point3Y = Point3Y * MultZoom
            
            Point1X = Point1X - OffsetDrawX
            Point1Y = Point1Y - OffsetDrawY
            Point2X = Point2X - OffsetDrawX
            Point2Y = Point2Y - OffsetDrawY
            Point3X = Point3X - OffsetDrawX
            Point3Y = Point3Y - OffsetDrawY
    
            DrawBox.DrawWidth = 1
            DrawBox.Line (Point1X, Point1Y)-(Point2X, Point2Y)
            DrawBox.Line (Point2X, Point2Y)-(Point3X, Point3Y)
            DrawBox.Line (Point3X, Point3Y)-(Point1X, Point1Y)
        
        End If
        End If
        End If
        
    Next
    
End Sub

Private Sub btnHideSide_Click()
    Frame1.Width = 5895
End Sub

Private Sub btnRotD_Click()
    RotateYZ 0.1
    If chkKeepInView.Value = vbChecked Then CenterImageByOffsets
    Draw
    Text1.SetFocus
End Sub

Private Sub btnRotL_Click()
    RotateXZ -0.1
    If chkKeepInView.Value = vbChecked Then CenterImageByOffsets
    Draw
    Text1.SetFocus
End Sub

Private Sub btnRotRight_Click()
    RotateXZ 0.1
    If chkKeepInView.Value = vbChecked Then CenterImageByOffsets
    Draw
    Text1.SetFocus
End Sub

Private Sub BtnShiftDn_Click()
    OffsetDrawY = OffsetDrawY - 100
    Draw
    Text1.SetFocus
End Sub

Private Sub BtnShiftL_Click()
    OffsetDrawX = OffsetDrawX + 100
    Draw
    Text1.SetFocus
End Sub


Private Sub BtnShiftR_Click()
    OffsetDrawX = OffsetDrawX - 100
    Draw
    Text1.SetFocus
End Sub


Private Sub BtnShiftUp_Click()
    OffsetDrawY = OffsetDrawY + 100
    Draw
    Text1.SetFocus
End Sub

Private Sub btnShowMnu_Click()
    PopupMenu Mnu_
End Sub

Private Sub chkDrawTri_Click()
    Text1.SetFocus
End Sub

Private Sub chkKeepInView_Click()
    Text1.SetFocus
End Sub

Private Sub Command2_Click()
    RotateYZ -0.1
    If chkKeepInView.Value = vbChecked Then CenterImageByOffsets
    Draw
    Text1.SetFocus
End Sub

Sub RotA(ByVal Degrees As Single)
    RotateYZ (Degrees / 90) * (1.5 + 0.1)
End Sub

Sub RotB(ByVal Degrees As Single)
    RotateXY (Degrees / 90) * 1.5
End Sub

Sub RotC(ByVal Degrees As Single)
    RotateXZ (Degrees / 90) * 1.5
End Sub

Private Sub DrawBox_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    
    MouseUpX = 0
    MouseUpY = 0
    
    MouseDownX = X
    MouseDownY = Y
    
    DrawBox.Picture = DrawBox.Image
    
End Sub

Private Sub DrawBox_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
    
    If MouseDownX <> 0 And MouseDownY <> 0 And MouseUpX = 0 And MouseUpY = 0 Then
        
        DrawBox.Cls
        DrawBox.Line (MouseDownX, MouseDownY)-(MouseDownX, Y), vbBlue
        DrawBox.Line (MouseDownX, MouseDownY)-(X, MouseDownY), vbBlue
        DrawBox.Line (X, Y)-(X, MouseDownY), vbBlue
        DrawBox.Line (X, Y)-(MouseDownX, Y), vbBlue
    
    End If
    
End Sub

Private Sub DrawBox_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
Dim blah As Integer

    DrawBox.Cls
    
    MouseUpX = X
    MouseUpY = Y
    
    If MouseDownX > MouseUpX Then
        blah = MouseDownX
        MouseDownX = MouseUpX
        MouseUpX = blah
    End If
    
    If MouseDownY > MouseUpY Then
        blah = MouseDownY
        MouseDownY = MouseUpY
        MouseUpY = blah
    End If
    
    DrawBox.Cls
    DrawBox.Picture = Nothing
    Draw
    
    Text1.SetFocus
    
End Sub

Private Sub Form_Load()

    MultZoom = 100
    Mnu_.Visible = False
    WorldLoaded = False
    btnHideSide_Click
    Me.Show
    Text1.SetFocus
    RenderAll = True
    
End Sub

Public Sub RotateXY(Radi As Double)
Dim Tmp1 As Double
Dim Tmp2 As Double
Dim i As Long, J As Long, K As Long

    For i = 1 To UBound(WorldVertX)
    
        Tmp1 = WorldVertX(i)
        Tmp2 = WorldVertY(i)
        WorldVertX(i) = Tmp1 * Cos(Radi) - Tmp2 * Sin(Radi)
        WorldVertY(i) = Tmp1 * Sin(Radi) + Tmp2 * Cos(Radi)
        
    Next
    
End Sub

Public Sub RotateXZ(Radi As Double)
Dim Tmp1 As Double
Dim Tmp2 As Double
Dim i As Long, J As Long, K As Long

    For i = 1 To UBound(WorldVertX)
    
        Tmp1 = WorldVertX(i)
        Tmp2 = WorldVertZ(i)
        WorldVertX(i) = Tmp1 * Cos(Radi) - Tmp2 * Sin(Radi)
        WorldVertZ(i) = Tmp1 * Sin(Radi) + Tmp2 * Cos(Radi)

    Next
    
End Sub

Public Sub RotateYZ(Radi As Double)
Dim Tmp1 As Double
Dim Tmp2 As Double
Dim i As Long, J As Long, K As Long

    For i = 1 To UBound(WorldVertX)
    
        Tmp1 = WorldVertY(i)
        Tmp2 = WorldVertZ(i)
        WorldVertY(i) = Tmp1 * Cos(Radi) - Tmp2 * Sin(Radi)
        WorldVertZ(i) = Tmp1 * Sin(Radi) + Tmp2 * Cos(Radi)
        
    Next

End Sub

Private Sub List1_Click()
    
    If RenderAll = False Then Draw
    
End Sub

Private Sub Mnu__Click()
    
    If RenderAll Then
        Mnu_RendAll.Checked = True
        Mnu_RendSelFlagVals.Checked = False
    Else
        Mnu_RendAll.Checked = False
        Mnu_RendSelFlagVals.Checked = True
    End If
    
End Sub

Private Sub Mnu_FlagSetForAllSel_Click()
Dim i As Integer, FlagToSet As Long, ChangeCount As Integer
Dim X1 As Integer, Y1 As Integer
Dim X2 As Integer, Y2 As Integer
Dim X3 As Integer, Y3 As Integer
Dim OkToEdit As Integer

    If MsgBox("WARNING: What you are about todo is PERMINANT and cannot be undone without restoring from a backup file!" & vbCrLf & "Are you sure about this?", vbYesNo Or vbQuestion) = vbNo Then Exit Sub
    
    FlagToSet = InputBox("What number (decimal integer) would you like to set?", , 100)
    
    For i = 1 To WMO.TriangleInfo_Count
        
        X1 = WorldVertX(WMO.IVOM_1(i))
        Y1 = WorldVertY(WMO.IVOM_1(i))
        X2 = WorldVertX(WMO.IVOM_2(i))
        Y2 = WorldVertY(WMO.IVOM_2(i))
        X3 = WorldVertX(WMO.IVOM_3(i))
        Y3 = WorldVertY(WMO.IVOM_3(i))
                 
        X1 = X1 * MultZoom
        Y1 = Y1 * MultZoom
        X2 = X2 * MultZoom
        Y2 = Y2 * MultZoom
        X3 = X3 * MultZoom
        Y3 = Y3 * MultZoom
        
        X1 = X1 - OffsetDrawX
        Y1 = Y1 - OffsetDrawY
        X2 = X2 - OffsetDrawX
        Y2 = Y2 - OffsetDrawY
        X3 = X3 - OffsetDrawX
        Y3 = Y3 - OffsetDrawY
        
        OkToEdit = False
        
        If WorldVertHide(WMO.IVOM_1(i)) = False Then
        
            If MouseDownX > 0 And X1 > MouseDownX And MouseUpX > X1 Then
            If MouseDownY > 0 And Y1 > MouseDownY And MouseUpY > Y1 Then
            
            
                If WorldVertHide(WMO.IVOM_2(i)) = False Then
                
                    If MouseDownX > 0 And X2 > MouseDownX And MouseUpX > X2 Then
                    If MouseDownY > 0 And Y2 > MouseDownY And MouseUpY > Y2 Then
                    
                            If WorldVertHide(WMO.IVOM_3(i)) = False Then
                                If MouseDownX > 0 And X3 > MouseDownX And MouseUpX > X3 Then
                                If MouseDownY > 0 And Y3 > MouseDownY And MouseUpY > Y3 Then
                                
                                        WMO.TriangleInfo_Flags(i) = FlagToSet
                                        ChangeCount = ChangeCount + 1
                                
                                End If
                                End If
                            End If
                    
                    End If
                    End If
                    
                End If
                
            
            End If
            End If
            
        End If
        
    Next
    
    MsgBox ChangeCount & " triangles were effected by this operation", vbInformation
End Sub

Private Sub Mnu_FlagSetForAllVis_Click()
Dim i As Integer, FlagToSet As Long, ChangeCount As Integer

    If MsgBox("WARNING: What you are about todo is PERMINANT and cannot be undone without restoring from a backup file!" & vbCrLf & "Are you sure about this?", vbYesNo Or vbQuestion) = vbNo Then Exit Sub
    
    FlagToSet = InputBox("What number (decimal integer) would you like to set?", , 100)
    
    For i = 1 To WMO.TriangleInfo_Count
        
        If WorldVertHide(WMO.IVOM_1(i)) = False And WorldVertHide(WMO.IVOM_2(i)) = False And WorldVertHide(WMO.IVOM_3(i)) = False Then
            
            WMO.TriangleInfo_Flags(i) = FlagToSet
            ChangeCount = ChangeCount + 1
        
        End If
        
    Next
    
    MsgBox ChangeCount & " triangles were effected by this operation", vbInformation
    
End Sub

Private Sub Mnu_FlagSetForSel_Click()
Dim i As Integer, FlagToSet As Long, ChangeCount As Integer
Dim X1 As Integer, Y1 As Integer
Dim X2 As Integer, Y2 As Integer
Dim X3 As Integer, Y3 As Integer
Dim OkToEdit As Boolean

    If MsgBox("WARNING: What you are about todo is PERMINANT and cannot be undone without restoring from a backup file!" & vbCrLf & "Are you sure about this?", vbYesNo Or vbQuestion) = vbNo Then Exit Sub
    
    FlagToSet = InputBox("What number (decimal integer) would you like to set?", , 100)
    
    For i = 1 To WMO.TriangleInfo_Count
        
        X1 = WorldVertX(WMO.IVOM_1(i))
        Y1 = WorldVertY(WMO.IVOM_1(i))
        X2 = WorldVertX(WMO.IVOM_2(i))
        Y2 = WorldVertY(WMO.IVOM_2(i))
        X3 = WorldVertX(WMO.IVOM_3(i))
        Y3 = WorldVertY(WMO.IVOM_3(i))
                 
        X1 = X1 * MultZoom
        Y1 = Y1 * MultZoom
        X2 = X2 * MultZoom
        Y2 = Y2 * MultZoom
        X3 = X3 * MultZoom
        Y3 = Y3 * MultZoom
        
        X1 = X1 - OffsetDrawX
        Y1 = Y1 - OffsetDrawY
        X2 = X2 - OffsetDrawX
        Y2 = Y2 - OffsetDrawY
        X3 = X3 - OffsetDrawX
        Y3 = Y3 - OffsetDrawY
        
        OkToEdit = False
        
        If WorldVertHide(WMO.IVOM_1(i)) = False Then
            If MouseDownX > 0 And X1 > MouseDownX And MouseUpX > X1 Then
            If MouseDownY > 0 And Y1 > MouseDownY And MouseUpY > Y1 Then
            
                OkToEdit = True
            
            End If
            End If
        End If
        
        If WorldVertHide(WMO.IVOM_2(i)) = False Then
            If MouseDownX > 0 And X2 > MouseDownX And MouseUpX > X2 Then
            If MouseDownY > 0 And Y2 > MouseDownY And MouseUpY > Y2 Then
            
                OkToEdit = True
            
            End If
            End If
        End If
        
        If WorldVertHide(WMO.IVOM_3(i)) = False Then
            If MouseDownX > 0 And X3 > MouseDownX And MouseUpX > X3 Then
            If MouseDownY > 0 And Y3 > MouseDownY And MouseUpY > Y3 Then
            
                OkToEdit = True
            
            End If
            End If
        End If
        
        
        If OkToEdit Then

            WMO.TriangleInfo_Flags(i) = FlagToSet
            ChangeCount = ChangeCount + 1
        
        End If
        
    Next
    
    MsgBox ChangeCount & " triangles were effected by this operation", vbInformation

End Sub

Private Sub Mnu_FlagSetForVis_Click()
Dim i As Integer, FlagToSet As Long, ChangeCount As Integer

    If MsgBox("WARNING: What you are about todo is PERMINANT and cannot be undone without restoring from a backup file!" & vbCrLf & "Are you sure about this?", vbYesNo Or vbQuestion) = vbNo Then Exit Sub
    
    FlagToSet = InputBox("What number (decimal integer) would you like to set?", , 100)
    
    For i = 1 To WMO.TriangleInfo_Count
        
        If WorldVertHide(WMO.IVOM_1(i)) = False Or WorldVertHide(WMO.IVOM_2(i)) = False Or WorldVertHide(WMO.IVOM_3(i)) = False Then
            
            WMO.TriangleInfo_Flags(i) = FlagToSet
            ChangeCount = ChangeCount + 1
        
        End If
        
    Next
    
    MsgBox ChangeCount & " triangles were effected by this operation", vbInformation
    
End Sub

Private Sub Mnu_HideSel_Click()
Dim i As Long, X As Double, Y As Double

    For i = 1 To WMO.Vertices_Count
        
        X = WorldVertX(i)
        Y = WorldVertY(i)
                
        X = X * MultZoom
        Y = Y * MultZoom
        
        X = X - OffsetDrawX
        Y = Y - OffsetDrawY
        
        
        If MouseDownX > 0 Then
        If X > MouseDownX Then
        If MouseUpX > X Then
        If Y > MouseDownY Then
        If MouseUpY > Y Then
        
            WorldVertHide(i) = True
            
        End If
        End If
        End If
        End If
        End If
                
    Next
    
    Draw
    
End Sub

Private Sub Mnu_PlaneSelectForAllSel_Click_7()
Dim i As Integer, FlagToSet As Long, ChangeCount As Integer
Dim X1 As Integer, Y1 As Integer
Dim X2 As Integer, Y2 As Integer
Dim X3 As Integer, Y3 As Integer
Dim OkToEdit As Boolean

    If MsgBox("WARNING: What you are about todo is PERMINANT and cannot be undone without restoring from a backup file!" & vbCrLf & "Are you sure about this?", vbYesNo Or vbQuestion) = vbNo Then Exit Sub
    
    FlagToSet = InputBox("What number (decimal integer) would you like to set?", , 100)
    
    For i = 1 To WMO.TriangleInfo_Count
        
        X1 = WorldVertX(WMO.IVOM_1(i))
        Y1 = WorldVertY(WMO.IVOM_1(i))
        X2 = WorldVertX(WMO.IVOM_2(i))
        Y2 = WorldVertY(WMO.IVOM_2(i))
        X3 = WorldVertX(WMO.IVOM_3(i))
        Y3 = WorldVertY(WMO.IVOM_3(i))
                 
        X1 = X1 * MultZoom
        Y1 = Y1 * MultZoom
        X2 = X2 * MultZoom
        Y2 = Y2 * MultZoom
        X3 = X3 * MultZoom
        Y3 = Y3 * MultZoom
        
        X1 = X1 - OffsetDrawX
        Y1 = Y1 - OffsetDrawY
        X2 = X2 - OffsetDrawX
        Y2 = Y2 - OffsetDrawY
        X3 = X3 - OffsetDrawX
        Y3 = Y3 - OffsetDrawY
        
        OkToEdit = False
        
        If WorldVertHide(WMO.IVOM_1(i)) = False Then
        
            If MouseDownX > 0 And X1 > MouseDownX And MouseUpX > X1 Then
            If MouseDownY > 0 And Y1 > MouseDownY And MouseUpY > Y1 Then
            
            
                If WorldVertHide(WMO.IVOM_2(i)) = False Then
                
                    If MouseDownX > 0 And X2 > MouseDownX And MouseUpX > X2 Then
                    If MouseDownY > 0 And Y2 > MouseDownY And MouseUpY > Y2 Then
                    
                            If WorldVertHide(WMO.IVOM_3(i)) = False Then
                            
                                If MouseDownX > 0 And X3 > MouseDownX And MouseUpX > X3 Then
                                If MouseDownY > 0 And Y3 > MouseDownY And MouseUpY > Y3 Then
                                
                                        WMO.BSPNode_PlaneType(i) = FlagToSet
                                        ChangeCount = ChangeCount + 1
                                
                                End If
                                End If
                                
                            End If
                    
                    End If
                    End If
                    
                End If
            
            End If
            End If
            
        End If
        
    Next
    
    MsgBox ChangeCount & " triangles were effected by this operation", vbInformation

End Sub

Private Sub Mnu_PlaneRep_Click()
Dim i As Integer, Find As Single, Rep As String, a As Integer, b As Integer
    
    Find = InputBox("To what value would you like to target? (Find to replace)")
    Rep = InputBox("To what value would you like to set replace with (set to)?")
    
    If Find = "" Then Exit Sub
    If Rep = "" Then Exit Sub
    
    If IsNumeric(Find) = False Or IsNumeric(Rep) = False Then
        MsgBox "Error: Invalid input", vbExclamation
        Exit Sub
    End If
    
    a = Find
    b = Rep
    
    For i = 1 To WMO.BSPNodeCount
        
        If WMO.BSPNode_PlaneType(i) = a Then
            
            WMO.BSPNode_PlaneType(i) = b
            
        End If
        
    Next
    
End Sub

Private Sub Mnu_PlaneSelectForAllSel_Click()
Dim i As Integer, r As Integer, s As String
    
    s = InputBox("To what value would you like to set as the new plane type to all planes?")
    
    If s = "" Then Exit Sub
    
    If IsNumeric(s) = False Then
        MsgBox "Error: Invalid input", vbExclamation
        Exit Sub
    End If
    
    r = s
    
    For i = 1 To WMO.BSPNodeCount
        
        WMO.BSPNode_PlaneType(i) = r
        
    Next
    
End Sub

Private Sub Mnu_PlaneVanish_Click()
Dim i As Integer, r As Integer, s As String
    
    For i = 1 To WMO.BSPNodeCount
        
        WMO.BSPNode_NumbFace(i) = 0
        
    Next
    
End Sub

Private Sub Mnu_RendAll_Click()
    RenderAll = True
    Draw
End Sub

Private Sub Mnu_RendSelFlagVals_Click()
    RenderAll = False
    Draw
End Sub

Private Sub Mnu_RepFlagValue_Click()
Dim i As Integer, FlagToFind As Long, FlagToSet As Long, CountChange As Long

    If MsgBox("WARNING: What you are about todo is PERMINANT and cannot be undone without restoring from a backup file!" & vbCrLf & "Are you sure about this?", vbYesNo Or vbQuestion) = vbNo Then Exit Sub
    
    FlagToFind = InputBox("What number (decimal integer) would you like to find (to be replaced)?", , 0)
    FlagToSet = InputBox("And what number (decimal integer) would you like replace with (to set)?", , 100)
    
    For i = 1 To WMO.TriangleInfo_Count
        
        If WMO.TriangleInfo_Flags(i) = FlagToFind Then
            
            WMO.TriangleInfo_Flags(i) = FlagToSet
            CountChange = CountChange + 1
        
        End If
        
    Next
    
    MsgBox CountChange & " triangles were effected by this process.", vbInformation
    
End Sub

Private Sub Mnu_Save_Click()
    
    WMO.SaveWMO InputBox("Where would you like to save this file?", "Input location", WMO.WMOTargFile)
    MsgBox "Save done", vbInformation
    
End Sub

Private Sub Mnu_SelectAll_Click()
Dim i As Integer
    
    MouseDownX = 1
    MouseDownY = 1
    MouseUpX = 10000
    MouseUpY = 10000
    
    Draw
    
End Sub

Private Sub Mnu_ShiftX_Click()
Dim i As Long, X As Double, Y As Double, AddValue As Single, s As String
    
    s = InputBox("What value would you like to add/subtract?" & vbCrLf & "Input + or - and then a number.", , "+0")
    If s = "" Then Exit Sub
    
    
    For i = 1 To WMO.Vertices_Count
        
        X = WorldVertX(i)
        Y = WorldVertY(i)
                
        X = X * MultZoom
        Y = Y * MultZoom
        
        X = X - OffsetDrawX
        Y = Y - OffsetDrawY
        
        
        If WorldVertHide(i) = False Then
        If MouseDownX > 0 Then
        If X > MouseDownX Then
        If MouseUpX > X Then
        If Y > MouseDownY Then
        If MouseUpY > Y Then
            
            If Mid(s, 1, 1) = "*" Then
                AddValue = Mid(s, 2)
                WMO.Vertices_X(i) = WMO.Vertices_X(i) * AddValue
            Else
                AddValue = s
                WMO.Vertices_X(i) = WMO.Vertices_X(i) + AddValue
            End If
            
        End If
        End If
        End If
        End If
        End If
        End If
                
    Next
    
    LoadWorld
    Draw
    
End Sub

Private Sub Mnu_ShiftY_Click()
Dim i As Long, X As Double, Y As Double, AddValue As Single, s As String
    
    s = InputBox("What value would you like to add/subtract?" & vbCrLf & "Input + or - and then a number.", , "+0")
    If s = "" Then Exit Sub
    
    For i = 1 To WMO.Vertices_Count
        
        X = WorldVertX(i)
        Y = WorldVertY(i)
                
        X = X * MultZoom
        Y = Y * MultZoom
        
        X = X - OffsetDrawX
        Y = Y - OffsetDrawY
        
        
        If WorldVertHide(i) = False Then
        If MouseDownX > 0 Then
        If X > MouseDownX Then
        If MouseUpX > X Then
        If Y > MouseDownY Then
        If MouseUpY > Y Then
        
            If Mid(s, 1, 1) = "*" Then
                AddValue = Mid(s, 2)
                WMO.Vertices_Y(i) = WMO.Vertices_Y(i) * AddValue
            Else
                AddValue = s
                WMO.Vertices_Y(i) = WMO.Vertices_Y(i) + AddValue
            End If
            
        End If
        End If
        End If
        End If
        End If
        End If
                
    Next
    
    LoadWorld
    Draw
    
End Sub

Private Sub Mnu_ShiftZ_Click()
Dim i As Long, X As Double, Y As Double, AddValue As Single, s As String
    
    s = InputBox("What value would you like to add/subtract?" & vbCrLf & "Input + or - and then a number.", , "+0")
    If s = "" Then Exit Sub
    AddValue = s
    
    For i = 1 To WMO.Vertices_Count
        
        X = WorldVertX(i)
        Y = WorldVertY(i)
                
        X = X * MultZoom
        Y = Y * MultZoom
        
        X = X - OffsetDrawX
        Y = Y - OffsetDrawY
        
        
        If WorldVertHide(i) = False Then
        If MouseDownX > 0 Then
        If X > MouseDownX Then
        If MouseUpX > X Then
        If Y > MouseDownY Then
        If MouseUpY > Y Then
        
            If Mid(s, 1, 1) = "*" Then
                AddValue = Mid(s, 2)
                WMO.Vertices_Z(i) = WMO.Vertices_Z(i) * AddValue
            Else
                AddValue = s
                WMO.Vertices_Z(i) = WMO.Vertices_Z(i) + (-AddValue)
            End If
            
        End If
        End If
        End If
        End If
        End If
        End If
                
    Next
    
    LoadWorld
    Draw
    
End Sub

Private Sub Mnu_test_Click()
Dim BSP As Integer, BSPMOBR_index As Integer, TriID As Integer
Dim X1 As Integer, Y1 As Integer
Dim X2 As Integer, Y2 As Integer
Dim X3 As Integer, Y3 As Integer
Dim OkToEdit As Boolean, TriEffect As Integer

Dim IDs As String


    For BSP = 1 To WMO.BSPNodeCount
        
        TriID = WMO.BSPNode_FirstFace_TriID(BSP)
        
        If InStr(1, IDs, "-" & TriID) = 0 Then IDs = IDs & "-" & TriID
        
        'X1 = WorldVertX(WMO.IVOM_1(TriID))
        'Y1 = WorldVertY(WMO.IVOM_1(TriID))
        'X2 = WorldVertX(WMO.IVOM_2(TriID))
        'Y2 = WorldVertY(WMO.IVOM_2(TriID))
        'X3 = WorldVertX(WMO.IVOM_3(TriID))
        'Y3 = WorldVertY(WMO.IVOM_3(TriID))
        '
        'X1 = X1 * MultZoom
        'Y1 = Y1 * MultZoom
        'X2 = X2 * MultZoom
        'Y2 = Y2 * MultZoom
        'X3 = X3 * MultZoom
        'Y3 = Y3 * MultZoom
        '
        'X1 = X1 - OffsetDrawX
        'Y1 = Y1 - OffsetDrawY
        'X2 = X2 - OffsetDrawX
        'Y2 = Y2 - OffsetDrawY
        'X3 = X3 - OffsetDrawX
        'Y3 = Y3 - OffsetDrawY
        
        OkToEdit = True
        
        'If WorldVertHide(WMO.IVOM_1(TriID)) = False Then
        '    If MouseDownX > 0 And X1 > MouseDownX And MouseUpX > X1 Then
        '    If MouseDownY > 0 And Y1 > MouseDownY And MouseUpY > Y1 Then
        '
        '        OkToEdit = True
        '
        '    End If
        '    End If
        'End If
        '
        'If WorldVertHide(WMO.IVOM_2(TriID)) = False Then
        '    If MouseDownX > 0 And X2 > MouseDownX And MouseUpX > X2 Then
        '    If MouseDownY > 0 And Y2 > MouseDownY And MouseUpY > Y2 Then
        '
        '        OkToEdit = True
        '
        '    End If
        '    End If
        'End If
        '
        'If WorldVertHide(WMO.IVOM_3(TriID)) = False Then
        '    If MouseDownX > 0 And X3 > MouseDownX And MouseUpX > X3 Then
        '    If MouseDownY > 0 And Y3 > MouseDownY And MouseUpY > Y3 Then
        '
        '        OkToEdit = True
        '
        '    End If
        '    End If
        'End If
        
        
        If OkToEdit Then
            
            'WMO.BSPNode_PlaneType(BSP) = 0
            WMO.BSPNode_NumbFace(BSP) = 0
            TriEffect = TriEffect + 1
            
        End If

        
    Next
    
    Clipboard.Clear
    Clipboard.SetText Replace(IDs, "-", vbCrLf)
    
    MsgBox "Done. " & TriEffect & " (" & Round((TriEffect / WMO.TriangleInfo_Count) * 100, 0) & "%) triangles were effected by this process.", vbInformation
    
End Sub

Private Sub Mnu_UnHideAll_Click()
Dim i As Long, X As Double, Y As Double

    For i = 1 To WMO.Vertices_Count
        
        WorldVertHide(i) = False
                
    Next
    
    Draw
    
End Sub

Private Sub Mnu_ViewFlagList_Click()
Dim i As Integer, a() As String, s As String

    Frame1.Width = 8535
    
    List1.Clear
    
    For i = 1 To WMO.TriangleInfo_Count
        
        If InStr(1, s, " " & WMO.TriangleInfo_Flags(i) & " ") = 0 Then
            If s <> "" Then s = s & vbCrLf
            s = s & " " & WMO.TriangleInfo_Flags(i) & " "
        End If
        
    Next
    
    s = Replace(s, " ", "")
    a = Split(s, vbCrLf)
    
    For i = 0 To UBound(a)
        List1.AddItem a(i)
    Next
    
    
    
    List2.Clear
    s = ""
    
    For i = 1 To WMO.BSPNodeCount
        
        If InStr(1, s, " " & WMO.BSPNode_PlaneType(i) & " ") = 0 Then
            If s <> "" Then s = s & vbCrLf
            s = s & " " & WMO.BSPNode_PlaneType(i) & " "
        End If
        
    Next
    
    s = Replace(s, " ", "")
    a = Split(s, vbCrLf)
    
    For i = 0 To UBound(a)
        List2.AddItem a(i)
    Next
    
    
    
End Sub

Private Sub mnuVectors_flatten_Click()
Dim i As Integer
Dim ZSum As Long

    For i = 1 To WMO.Vertices_Count
        ZSum = ZSum + WMO.Vertices_Z(i)
    Next
    
    ZSum = ZSum / WMO.Vertices_Count    ' // gets the average
    
    For i = 1 To WMO.Vertices_Count
        WMO.Vertices_Z(i) = ZSum        ' sets the average
    Next
    
    LoadWorld
    Draw
    
End Sub

Private Sub Text1_Change()
    
    Select Case UCase(Strings.Right(Text1.Text, 1))
    Case "A":   btnRotL_Click
    Case "S":   btnRotD_Click
    Case "W":   Command2_Click
    Case "D":   btnRotRight_Click
    End Select
    
    Text1.Text = ""
    
End Sub

Private Sub Text1_KeyDown(KeyCode As Integer, Shift As Integer)
    
    Select Case KeyCode
    Case vbKeyLeft:     BtnShiftL_Click
    Case vbKeyDown:     BtnShiftDn_Click
    Case vbKeyRight:    BtnShiftR_Click
    Case vbKeyUp:       BtnShiftUp_Click
    End Select
    
End Sub


