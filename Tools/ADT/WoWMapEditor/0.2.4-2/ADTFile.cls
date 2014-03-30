VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "ADTFile"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Public WaterFlag    As Long
Private adtFileLen  As Long

Private adt_REVM As String
Private adt_RDHM As String
Private adt_NICM As String
Private adt_XETM As String
Private adt_XDMM As String
Private adt_DIMM As String
Private adt_OMWM As String
Private adt_DIWM As String
Private adt_FDDM As String
Private adt_FDOM As String

Private adt_KNCM(255, 8) As String      ' // Where h is in adt_KNCM(e, h), you can use MCNKArrOrder to tell what it is

Public Enum MCNKArrOrder
    MCNK_MCNK
    MCNK_TVCM
    MCNK_RNCM
    MCNK_YLCM
    MCNK_FRCM
    MCNK_HSCM
    MCNK_LACM
    MCNK_QLCM
    MCNK_ESCM
End Enum

Const XBase As Integer = 109
Const YBase As Integer = 113
Const ZBase As Integer = 117

Property Get RawMCNK(ByVal id As Integer, ByVal WhichChunk As MCNKArrOrder) As String
    
    RawMCNK = adt_KNCM(id - 1, WhichChunk)
    
End Property

Property Let RawMCNK(ByVal id As Integer, ByVal WhichChunk As MCNKArrOrder, ByVal vNewValue As String)
    
    adt_KNCM(id - 1, WhichChunk) = vNewValue
    
End Property

Property Get AreaIdNumber(ByVal ChunkID As Integer) As Integer
Dim s As String
    
    ChunkID = ChunkID - 1
    
    s = Mid(adt_KNCM(ChunkID, MCNK_MCNK), 53 + 4)
    s = Mid(s, 1, 4)
    AreaIdNumber = String32ToLong(s)
    
End Property

Property Let AreaIdNumber(ByVal ChunkID As Integer, vNewValue As Integer)
Dim s As String
    
    ChunkID = ChunkID - 1
    
    s = Mid(adt_KNCM(ChunkID, MCNK_MCNK), 1, 53 + 4 - 1)
    s = s & LongToString32(vNewValue) & Mid(adt_KNCM(ChunkID, MCNK_MCNK), 53 + 4 + 4)
    adt_KNCM(ChunkID, MCNK_MCNK) = s
    
End Property

Property Get HasWater(ByVal ChunkID As Integer, ByVal SubChunkId As Integer) As Boolean
Dim s As String

    ChunkID = ChunkID - 1

    s = adt_KNCM(ChunkID, MCNK_QLCM)
    
    If Len(s) < 20 Then
        HasWater = False
        Exit Property
    End If
    
    s = Mid(s, 4 + 4 + 4 + ((4 + 4) * 9 * 9) + 1)     ' // HeadLen + WaterHeightInt32 + BaseMagamaLenInt32 + ((UnknownInt32 + HeightInt32) * 9blocksWidth * 9blockHeight)
    s = Mid(s, 1, 8 * 8)
    s = Mid(s, SubChunkId, 1)
    
    If Asc(s) = 4 Then
        HasWater = True
    Else
        HasWater = False
    End If
    
End Property

Sub KllLiquidsForAllChunks(Optional SuppressDoneBox As Boolean = False)
Dim i As Integer, s As String, b As String
    
    For i = 0 To 255
        
        If Len(adt_KNCM(i, MCNK_QLCM)) > 20 Then
        
            s = adt_KNCM(i, MCNK_QLCM)
            s = Mid(s, 1, 660) & String(8 * 8, Chr(15)) & Mid(s, 660 + (8 * 8) + 1)
            adt_KNCM(i, MCNK_QLCM) = s
            
        End If
        
    Next
    
    If SuppressDoneBox Then Exit Sub
    MsgBox "Done", vbInformation
    
End Sub

Sub InjectWaterForAllChunks(ByVal Alt As Single)
Dim i As Integer, s As String
   
    ' Headers
    ' 3  = 0000011 = normal water
    ' 4  = 0000100 = normal water
    ' 5  = 0000101 = normal water
    ' 6  = 0000110 = same as 7?
    ' 7  = 0000111 = sticky water / wall   You will not be abel to enter the water from the side, and if you wall into it from above, you will not be abel to move (you'll get stuck)
    ' 8  = 0001000 = no water.  Nothing.
    ' 9  = 0001001 = water w/ invisible top?
    ' 10 = 0001010 = skitosfrenic stick water.  Like 7.. but not always?
    ' 20 = 0010100 = normal water but it seesm to glitch in a few rare/small places, with (inf?) magma height
    ' 30 = 0011110 = skitosfrenic stick water.  Like 7.. but not always?
    ' 77 = 1001101 = normal water but it seesm to glitch in a few rare/small places, with (inf?) magma height
    ' I think if you move into the "magma" area when you are a ghost it crashes WoW
    
    For i = 0 To 255

        adt_KNCM(i, MCNK_QLCM) = LongToString32(0) & FloatToString32(Alt) & FloatToString32(Alt) & RepeateString(9 * 9, LongToString32(0) & FloatToString32(Alt)) & String(8 * 8, Chr(4)) & LongToString32(0) & RepeateString(3, Chr(255) & Chr(255) & Chr(127) & Chr(127)) & String(4 * 4, Chr(0)) & Chr(255) & Chr(255) & Chr(0) & Chr(0) & Chr(223) & Chr(192) & Chr(1) & Chr(0) & Chr(64) & String(3, Chr(0)) & RepeateString(3, Chr(255) & Chr(255) & Chr(127) & Chr(127)) & String(16, Chr(0)) & Chr(64) & Chr(12) & Chr(69) & Chr(0) & Chr(52) & Chr(1) & String(2 * 3, Chr(0))
        adt_KNCM(i, MCNK_MCNK) = Mid(adt_KNCM(i, MCNK_MCNK), 1, 4) & LongToString32(WaterFlag) & Mid(adt_KNCM(i, MCNK_MCNK), 9)
        
    Next

End Sub

Sub SetWaterLevel(ByVal ChunkID As Integer, ByVal RendString As String, ByVal DefHeight As Single, HeightMap() As String)
Dim HMap As String, X As Integer

    For X = 1 To 9 * 9
        If HeightMap(X) = "" Then HeightMap(X) = DefHeight
        HMap = HMap & LongToString32(0) & FloatToString32(HeightMap(X))
    Next

    RendString = RendString & String(8 * 8, Chr(4))
    RendString = Strings.Left(RendString, 8 * 8)
    
    adt_KNCM(ChunkID - 1, MCNK_QLCM) = LongToString32(0) & FloatToString32(DefHeight) & FloatToString32(DefHeight) & HMap & RendString & LongToString32(0) & RepeateString(3, Chr(255) & Chr(255) & Chr(127) & Chr(127)) & String(4 * 4, Chr(0)) & Chr(255) & Chr(255) & Chr(0) & Chr(0) & Chr(223) & Chr(192) & Chr(1) & Chr(0) & Chr(64) & String(3, Chr(0)) & RepeateString(3, Chr(255) & Chr(255) & Chr(127) & Chr(127)) & String(16, Chr(0)) & Chr(64) & Chr(12) & Chr(69) & Chr(0) & Chr(52) & Chr(1) & String(2 * 3, Chr(0))
    adt_KNCM(ChunkID - 1, MCNK_MCNK) = Mid(adt_KNCM(ChunkID - 1, MCNK_MCNK), 1, 4) & LongToString32(WaterFlag) & Mid(adt_KNCM(ChunkID - 1, MCNK_MCNK), 9)
    
End Sub

Sub SetWaterLevel2(ByVal ChunkID As Integer, ByVal SubId As Integer, ByVal Height As Single)
Dim HMap(9 * 9) As String, s As String

    ChunkID = ChunkID - 1
    
    s = adt_KNCM(ChunkID, MCNK_QLCM)

    If Len(s) < 20 Then
        SetWaterLevel ChunkID + 1, String(SubId - 1, Chr(15)) & Chr(4) & String((8 * 8) - SubId + 1, Chr(15)), Height, HMap
    End If
    
    s = adt_KNCM(ChunkID, MCNK_QLCM)
    
'   Edit height map area
    s = Mid(s, 1, 13 + (8 * (SubId - 1))) & LongToString32(0) & FloatToString32(Height) & Mid(s, 13 + (8 * SubId) + 1)
        
'   Edit render string
    s = Mid(s, 1, 660 + SubId - 1) & Chr(4) & Mid(s, 660 + SubId + 1)
    
    'WriteFile "C:\test.bin", s
    
    'MsgBox HasWater(ChunkId, SubId)
    
    adt_KNCM(ChunkID, MCNK_QLCM) = s
    
End Sub

Sub HeightMap_LandShift(ByVal ChunkID As Integer, ByVal AddVale As Single)
Dim FloatCount As Integer, cPos As Integer, NewValue As Single, X As Integer, s As String
    
    ChunkZ(ChunkID) = ChunkZ(ChunkID) + AddVale
    
    ChunkID = ChunkID - 1
        
    FloatCount = Split(CStr((Len(adt_KNCM(ChunkID, MCNK_TVCM)) - 4) / 4), ".")(0)
    cPos = 1
    For X = 1 To FloatCount
    
        cPos = cPos + 4
    
        s = adt_KNCM(ChunkID, MCNK_TVCM)
    
        NewValue = String32ToFloat(Mid(s, cPos, 4))
        NewValue = NewValue + AddVale
    
        s = Mid(s, 1, cPos - 1) & FloatToString32(NewValue) & Mid(s, cPos + 4)
        adt_KNCM(ChunkID, MCNK_TVCM) = s
    
    Next
    
    's = adt_KNCM(ChunkID, MCNK_RNCM)
    's = Mid(s, 1, 4) & String(Len(s) - 4, Chr(0))
    ''s = Mid(s, 1, 4) & String(145 * 3, Chr(0)) & Mid(s, 4 + (145 * 3) + 1)
    'adt_KNCM(ChunkID, MCNK_RNCM) = s
    
    's = adt_KNCM(ChunkID, MCNK_MCNK)
    's = Mid(s, 1, 67) & String(16, Chr(0)) & Mid(s, 67 + 16 + 1)
    'adt_KNCM(ChunkID, MCNK_MCNK) = s

End Sub

Property Get HeightMap_Inner8x8(ByVal ChunkID As Integer, ByVal X As Integer, ByVal Y As Integer) As Single
Dim s As String, FloatNbr As Integer, s1 As Long, s2 As Long, Sngl As Double, sString As String

    FloatNbr = (Y - 1) * 16
    FloatNbr = FloatNbr + X + 9
    
    s = adt_KNCM(ChunkID - 1, MCNK_TVCM)
    s = Mid(s, 5)
    
    Sngl = (String32ToFloat(Mid(s, (FloatNbr * 4) + 1, 4)))
    s1 = Sngl * 100
    
    sString = Me.ChunkY(ChunkID)
    Sngl = Mid(sString, 1, 1) & Replace(Replace(Mid(sString, 2), "E", ""), "-", "")
    s2 = Sngl * 100
    
    s1 = s1 - s2
    
    Sngl = s1 / 100
    
    HeightMap_Inner8x8 = Sngl
    
End Property

Property Let HeightMap_Inner8x8(ByVal ChunkID As Integer, ByVal X As Integer, ByVal Y As Integer, ByVal vNewValue As Single)
Dim s As String, FloatNbr As Integer

    FloatNbr = (Y - 1) * 16
    FloatNbr = FloatNbr + X + 9

    s = adt_KNCM(ChunkID - 1, MCNK_TVCM)
    s = Mid(s, 1, 4 + (FloatNbr * 4)) & FloatToString32(vNewValue) & Mid(s, 4 + (FloatNbr * 4) + 5)
    adt_KNCM(ChunkID - 1, MCNK_TVCM) = s
    
End Property

Property Get HeightMap_Inner8x8bin(ByVal ChunkID As Integer, ByVal X As Integer, ByVal Y As Integer) As String
Dim s As String, FloatNbr As Integer, s1 As Long, s2 As Long, Sngl As Double, sString As String

    FloatNbr = (Y - 1) * 16
    FloatNbr = FloatNbr + X + 9
    
    s = adt_KNCM(ChunkID - 1, MCNK_TVCM)
    s = Mid(s, 5)
    
    's1 = Sngl * 100
    
    'sString = Me.ChunkY(ChunkId)
    'Sngl = Mid(sString, 1, 1) & Replace(Replace(Mid(sString, 2), "E", ""), "-", "")
    's2 = Sngl * 100
    
    's1 = s1 - s2
    
    'Sngl = s1 / 100
    
    HeightMap_Inner8x8bin = Mid(s, (FloatNbr * 4) + 1, 4)
    
End Property

Property Let HeightMap_Inner8x8bin(ByVal ChunkID As Integer, ByVal X As Integer, ByVal Y As Integer, ByVal vNewValue As String)
Dim s As String, FloatNbr As Integer

    FloatNbr = (Y - 1) * 16
    FloatNbr = FloatNbr + X + 9

    s = adt_KNCM(ChunkID - 1, MCNK_TVCM)
    s = Mid(s, 1, 4 + (FloatNbr * 4)) & vNewValue & Mid(s, 4 + (FloatNbr * 4) + 5)
    adt_KNCM(ChunkID - 1, MCNK_TVCM) = s
    
End Property

Property Get HeightMap_Outter9x9(ByVal ChunkID As Integer, ByVal X As Integer, ByVal Y As Integer) As Single
Dim s As String, FloatNbr As Integer, s1 As Single, s2 As Single

    FloatNbr = (Y - 1) * 16
    FloatNbr = FloatNbr + X
    
    s = adt_KNCM(ChunkID - 1, MCNK_TVCM)
    s = Mid(s, 5)
    
    s1 = String32ToFloat(Mid(s, (FloatNbr * 4) + 1, 4))
    's2 = Me.ChunkY(ChunkId)
    
    's1 = AddSingles(s1, s2)
    
    HeightMap_Outter9x9 = s1
    
End Property

Property Let HeightMap_Outter9x9(ByVal ChunkID As Integer, ByVal X As Integer, ByVal Y As Integer, ByVal vNewValue As Single)
Dim s As String, FloatNbr As Integer

    FloatNbr = (Y - 1) * 16
    FloatNbr = FloatNbr + X

    s = adt_KNCM(ChunkID - 1, MCNK_TVCM)
    s = Mid(s, 1, 4 + (FloatNbr * 4)) & FloatToString32(vNewValue) & Mid(s, 4 + (FloatNbr * 4) + 5)
    adt_KNCM(ChunkID - 1, MCNK_TVCM) = s
    
End Property

Property Get HeightMap_Outter9x9bin(ByVal ChunkID As Integer, ByVal X As Integer, ByVal Y As Integer) As String
Dim s As String, FloatNbr As Integer, s1 As Single, s2 As Single

    FloatNbr = (Y - 1) * 16
    FloatNbr = FloatNbr + X
    
    s = adt_KNCM(ChunkID - 1, MCNK_TVCM)
    s = Mid(s, 5)
    
    HeightMap_Outter9x9bin = Mid(s, (FloatNbr * 4) + 1, 4)
    
End Property

Property Let HeightMap_Outter9x9bin(ByVal ChunkID As Integer, ByVal X As Integer, ByVal Y As Integer, ByVal vNewValue As String)
Dim s As String, FloatNbr As Integer

    FloatNbr = (Y - 1) * 16
    FloatNbr = FloatNbr + X

    s = adt_KNCM(ChunkID - 1, MCNK_TVCM)
    s = Mid(s, 1, 4 + (FloatNbr * 4)) & vNewValue & Mid(s, 4 + (FloatNbr * 4) + 5)
    adt_KNCM(ChunkID - 1, MCNK_TVCM) = s
    
End Property

Property Let ChunkX(ByVal id As Integer, vNewValuse As Single)
Dim s As String

    s = adt_KNCM(id - 1, MCNK_MCNK)
    s = Mid(s, 1, XBase - 1) & FloatToString32(vNewValuse) & Mid(s, XBase + 4)
    adt_KNCM(id - 1, MCNK_MCNK) = s
      
End Property

Property Get ChunkX(ByVal id As Integer) As Single
Dim s As String
    
    s = adt_KNCM(id - 1, MCNK_MCNK)
    s = Mid(s, XBase, 4)
    ChunkX = String32ToFloat(s)
    
End Property

Property Let ChunkXbin(ByVal id As Integer, vNewValuse As String)
Dim s As String

    s = adt_KNCM(id - 1, MCNK_MCNK)
    s = Mid(s, 1, XBase - 1) & vNewValuse & Mid(s, XBase + 4)
    Clipboard.Clear
    Clipboard.SetText Replace(s, Chr(0), "")
    adt_KNCM(id - 1, MCNK_MCNK) = s
      
End Property

Property Get ChunkXbin(ByVal id As Integer) As String
Dim s As String
    
    s = adt_KNCM(id - 1, MCNK_MCNK)
    s = Mid(s, XBase)
    ChunkXbin = Mid(s, 1, 4)
    
End Property

Property Get ChunkYbin(ByVal id As Integer) As String
Dim s As String
    
    s = adt_KNCM(id - 1, MCNK_MCNK)
    s = Mid(s, YBase, 4)
    
    ChunkYbin = s
    
End Property

Property Let ChunkYbin(ByVal id As Integer, ByVal vNewValue As String)
Dim s As String
    
    s = adt_KNCM(id - 1, MCNK_MCNK)
    s = Mid(s, 1, YBase - 1) & vNewValue & Mid(s, YBase + 4)
    Clipboard.Clear
    Clipboard.SetText Replace(s, Chr(0), "")
    adt_KNCM(id - 1, MCNK_MCNK) = s
    
End Property

Property Get ChunkY(ByVal id As Integer) As Single
Dim s As String
    
    s = adt_KNCM(id - 1, MCNK_MCNK)
    s = Mid(s, YBase, 4)
    
    ChunkY = String32ToFloat(s)
    
End Property

Property Let ChunkY(ByVal id As Integer, ByVal vNewValue As Single)
Dim s As String
    
    s = adt_KNCM(id - 1, MCNK_MCNK)
    s = Mid(s, 1, YBase - 1) & FloatToString32(vNewValue) & Mid(s, YBase + 4)
    adt_KNCM(id - 1, MCNK_MCNK) = s
    
End Property

Property Let ChunkZ(ByVal id As Integer, vNewValuse As Single)
Dim s As String

    s = adt_KNCM(id - 1, MCNK_MCNK)
    s = Mid(s, 1, ZBase - 1) & FloatToString32(vNewValuse) & Mid(s, ZBase + 4)
    adt_KNCM(id - 1, MCNK_MCNK) = s
      
End Property

Property Get ChunkZ(ByVal id As Integer) As Single
Dim s As String
    
    s = adt_KNCM(id - 1, MCNK_MCNK)
    s = Mid(s, ZBase, 4)
    ChunkZ = String32ToFloat(s)
    
End Property

Property Let ChunkZbin(ByVal id As Integer, vNewValuse As String)
Dim s As String

    s = adt_KNCM(id - 1, MCNK_MCNK)
    s = Mid(s, 1, ZBase - 1) & vNewValuse & Mid(s, ZBase + 4)
    Clipboard.Clear
    Clipboard.SetText Replace(s, Chr(0), "")
    adt_KNCM(id - 1, MCNK_MCNK) = s
      
End Property

Property Get ChunkZbin(ByVal id As Integer) As String
Dim s As String
    
    s = adt_KNCM(id - 1, MCNK_MCNK)
    s = Mid(s, ZBase)
    ChunkZbin = Mid(s, 1, 4)
    
End Property


Function getadt_KNCM(id As Integer, id2 As Integer) As String
Dim s As String, i As Integer
    
    For i = 1 To Len(adt_KNCM(id, id2))
        s = s & Asc(Mid(adt_KNCM(id, id2), i, 1)) & "-"
    Next
    
    getadt_KNCM = s
    
End Function

Function getadt_RDHM() As String
Dim s As String, i As Integer
    
    For i = 1 To Len(adt_RDHM)
        s = s & Asc(Mid(adt_RDHM, i, 1)) & "-"
    Next
    
    getadt_RDHM = s
    
End Function

Public Property Get Ground_Holes(ByVal Ndx As Integer) As String
Dim myKNCM As String

    If Ndx = 0 Then
        DebugPrint "WARNING: Input to Ground_Holes was 0"
        Ground_Holes = "Err" & Chr(0)
        Exit Property
    End If
    
    Ground_Holes = Mid(adt_KNCM(Ndx - 1, MCNK_MCNK), 64, 4)
    
End Property

Public Property Let Ground_Holes(ByVal Ndx As Integer, ByVal vNewValue As String)

     adt_KNCM(Ndx - 1, MCNK_MCNK) = Mid(adt_KNCM(Ndx - 1, MCNK_MCNK), 1, 63) & vNewValue & Mid(adt_KNCM(Ndx - 1, MCNK_MCNK), 68)
    
End Property

Public Property Get Doodad_Count() As Long
    
    Doodad_Count = Split(CStr((Len(adt_FDDM) / 36)) & ".", ".")(0)

End Property

Public Property Get Doodad_DoodadPath(ByVal Ndx As Integer) As String
Dim StartPos As Double, s As String, i As Double, myFDDM As String
On Error GoTo ErrExit:
        
    myFDDM = Mid(adt_FDDM, 5)
    
    StartPos = Ndx - 1
    StartPos = StartPos * 36
    StartPos = StartPos + 1
    
    s = Mid(myFDDM, StartPos, 4)

    i = String32ToLong(s, True)
    If i > DoodadPaths_Count Then
        Doodad_DoodadPath = "<<BAD REFERENCE>> Ref=" & i
    Else
        Doodad_DoodadPath = DoodadPaths_Item(i)
    End If

Exit Function
ErrExit:
Doodad_DoodadPath = "<<BAD REFERENCE>>"

End Property

Public Property Get Doodad_DoodadSpecialID(ByVal Ndx As Integer) As Long
Dim StartPos As Double, s As String, i As Double, myFDDM As String
        
    myFDDM = Mid(adt_FDDM, 5)
    
    StartPos = Ndx - 1
    StartPos = StartPos * 36
    StartPos = StartPos + 1
    StartPos = StartPos + 4
    
    s = Mid(myFDDM, StartPos, 4)

    Doodad_DoodadSpecialID = String32ToLong(s)

End Property

Public Property Let Doodad_DoodadX(ByVal Ndx As Integer, ByVal vNewValue As Double)
Dim StartPos As Double, s As String, i As Double, myFDDM As String
Dim b4 As String, later As String

    b4 = adt_FDDM
    
    myFDDM = Mid(adt_FDDM, 5)       ' // I dunno y I have to do this, but for some reason, this kills the bug
        
    StartPos = 1 + ((Ndx - 1) * 36) + 7
    s = Mid(myFDDM, StartPos, 4)
    myFDDM = Mid(myFDDM, 1, StartPos) & FloatToString32(vNewValue) & Mid(myFDDM, StartPos + 5)  '
    
    adt_FDDM = Mid(adt_FDDM, 1, 4) & myFDDM
    
    later = adt_FDDM
    
End Property

Public Property Get Doodad_DoodadX(ByVal Ndx As Integer) As Double
Dim StartPos As Double, s As String, i As Double, myFDDM As String
    
    myFDDM = Mid(adt_FDDM, 5)       ' // I dunno y I have to do this, but for some reason, this kills the bug
        
    StartPos = Ndx - 1
    StartPos = StartPos * 36
    StartPos = StartPos + 1 + 8
    
    s = Mid(myFDDM, StartPos, 4)
    Doodad_DoodadX = String32ToFloat(s)

End Property

Public Property Let Doodad_DoodadY(ByVal Ndx As Integer, ByVal vNewValue As Double)
Dim StartPos As Double, s As String, i As Double, myFDDM As String
Dim b4 As String, later As String

    b4 = adt_FDDM
    
    myFDDM = Mid(adt_FDDM, 5)       ' // I dunno y I have to do this, but for some reason, this kills the bug
        
    StartPos = 1 + ((Ndx - 1) * 36) + 15
    s = Mid(myFDDM, StartPos, 4)
    myFDDM = Mid(myFDDM, 1, StartPos) & FloatToString32(vNewValue) & Mid(myFDDM, StartPos + 5)  '
    
    adt_FDDM = Mid(adt_FDDM, 1, 4) & myFDDM
    
    later = adt_FDDM
    
End Property

Public Property Get Doodad_DoodadY(ByVal Ndx As Integer) As Double
Dim StartPos As Double, s As String, i As Double, myFDDM As String

    myFDDM = Mid(adt_FDDM, 5)       ' // I dunno y I have to do this, but for some reason, this kills the bug
        
    StartPos = Ndx - 1
    StartPos = StartPos * 36
    StartPos = StartPos + 1 + 16
    
    s = Mid(myFDDM, StartPos, 4)
    Doodad_DoodadY = String32ToFloat(s)


End Property

Public Property Let Doodad_DoodadZ(ByVal Ndx As Integer, ByVal vNewValue As Double)
Dim StartPos As Double, s As String, i As Double, myFDDM As String
Dim b4 As String, later As String

    b4 = adt_FDDM
    
    myFDDM = Mid(adt_FDDM, 5)       ' // I dunno y I have to do this, but for some reason, this kills the bug
    
    StartPos = Ndx - 1
    StartPos = StartPos * 36
    StartPos = 1 + StartPos + 11
    s = Mid(myFDDM, StartPos, 4)
    myFDDM = Mid(myFDDM, 1, StartPos) & FloatToString32(vNewValue) & Mid(myFDDM, StartPos + 5)  '
    
    adt_FDDM = Mid(adt_FDDM, 1, 4) & myFDDM
    
    later = adt_FDDM
    
End Property

Public Property Get Doodad_DoodadZ(ByVal Ndx As Integer) As Double
Dim StartPos As Double, s As String, i As Double, myFDDM As String
'On Error GoTo ErrExit

    myFDDM = Mid(adt_FDDM, 5)       ' // I dunno y I have to do this, but for some reason, this kills the bug

    StartPos = Ndx - 1
    StartPos = StartPos * 36
    StartPos = StartPos + 1 + 12
    
    s = Mid(myFDDM, StartPos, 4)
    Doodad_DoodadZ = String32ToFloat(s)

Exit Property
ErrExit:
    Doodad_DoodadZ = -123456789

End Property

Public Property Let Doodad_DoodadRotA(ByVal Ndx As Integer, ByVal vNewValue As Double)
Dim StartPos As Double, s As String, i As Double, myFDDM As String
Dim b4 As String, later As String

    b4 = adt_FDDM
    
    myFDDM = Mid(adt_FDDM, 5)       ' // I dunno y I have to do this, but for some reason, this kills the bug
        
    StartPos = 1 + ((Ndx - 1) * 36) + 19
    s = Mid(myFDDM, StartPos, 4)
    myFDDM = Mid(myFDDM, 1, StartPos) & FloatToString32(vNewValue) & Mid(myFDDM, StartPos + 5)   '
    
    adt_FDDM = Mid(adt_FDDM, 1, 4) & myFDDM
    
    later = adt_FDDM
    
End Property

Public Property Get Doodad_DoodadRotA(ByVal Ndx As Integer) As Double
Dim StartPos As Double, s As String, i As Double, myFDDM As String
    
    myFDDM = Mid(adt_FDDM, 5)       ' // I dunno y I have to do this, but for some reason, this kills the bug

    StartPos = Ndx - 1
    StartPos = StartPos * 36
    StartPos = StartPos + 1 + 20
    
    s = Mid(myFDDM, StartPos, 4)
    Doodad_DoodadRotA = String32ToFloat(s)

End Property

Public Property Let Doodad_DoodadRotB(ByVal Ndx As Integer, ByVal vNewValue As Double)
Dim StartPos As Double, s As String, i As Double, myFDDM As String
Dim b4 As String, later As String

    b4 = adt_FDDM
    
    myFDDM = Mid(adt_FDDM, 5)       ' // I dunno y I have to do this, but for some reason, this kills the bug
        
    StartPos = 1 + ((Ndx - 1) * 36) + 23
    s = Mid(myFDDM, StartPos, 4)
    myFDDM = Mid(myFDDM, 1, StartPos) & FloatToString32(vNewValue) & Mid(myFDDM, StartPos + 5)  '
    
    adt_FDDM = Mid(adt_FDDM, 1, 4) & myFDDM
    
    later = adt_FDDM
    
End Property

Public Property Get Doodad_DoodadRotB(ByVal Ndx As Integer) As Double
Dim StartPos As Double, s As String, i As Double, myFDDM As String
    
    myFDDM = Mid(adt_FDDM, 5)       ' // I dunno y I have to do this, but for some reason, this kills the bug
        
    StartPos = Ndx - 1
    StartPos = StartPos * 36
    StartPos = StartPos + 1 + 24
    
    s = Mid(myFDDM, StartPos, 4)
    Doodad_DoodadRotB = String32ToFloat(s)
    
End Property

Public Property Let Doodad_DoodadRotC(ByVal Ndx As Integer, ByVal vNewValue As Double)
Dim StartPos As Double, s As String, i As Double, myFDDM As String
Dim b4 As String, later As String

    b4 = adt_FDDM
    
    myFDDM = Mid(adt_FDDM, 5)       ' // I dunno y I have to do this, but for some reason, this kills the bug
        
    StartPos = 1 + ((Ndx - 1) * 36) + 27
    s = Mid(myFDDM, StartPos, 4)
    myFDDM = Mid(myFDDM, 1, StartPos) & FloatToString32(vNewValue) & Mid(myFDDM, StartPos + 5)  '
    
    adt_FDDM = Mid(adt_FDDM, 1, 4) & myFDDM
    
    later = adt_FDDM
    
End Property

Public Property Get Doodad_DoodadRotC(ByVal Ndx As Integer) As Double
Dim StartPos As Double, s As String, i As Double, myFDDM As String
    
    myFDDM = Mid(adt_FDDM, 5)       ' // I dunno y I have to do this, but for some reason, this kills the bug
        
    StartPos = Ndx - 1
    StartPos = StartPos * 36
    StartPos = StartPos + 1 + 28
    
    s = Mid(myFDDM, StartPos, 4)
    Doodad_DoodadRotC = String32ToFloat(s)
    
End Property

Public Property Get Doodad_DoodadFlags(ByVal Ndx As Integer) As Long
Dim StartPos As Double, s As String, i As Double, myFDDM As String
    
    myFDDM = Mid(adt_FDDM, 5)

    StartPos = Ndx - 1
    StartPos = StartPos * 36
    StartPos = StartPos + 1 + 30
    
    s = Mid(myFDDM, StartPos, 2)
    Doodad_DoodadFlags = String32ToLong(Chr(0) & Chr(0) & s)
    
End Property

Public Property Get Doodad_Size(ByVal Ndx As Integer) As Long
Dim StartPos As Double, s As String, i As Double, myFDDM As String
    
    myFDDM = Mid(adt_FDDM, 5)

    StartPos = Ndx - 1
    StartPos = StartPos * 36
    StartPos = StartPos + 1 + 32
    
    s = Mid(myFDDM, StartPos, 2)
    Doodad_Size = String32ToLong(s & Chr(0) & Chr(0))

End Property

Public Property Let Doodad_Size(ByVal Ndx As Integer, ByVal vNewValue As Long)
Dim StartPos As Double, s As String, i As Double, myFDDM As String
    
    s = LongToString32(vNewValue)
    s = Left(s, 2)
    
    myFDDM = Mid(adt_FDDM, 5)

    StartPos = Ndx - 1
    StartPos = StartPos * 36
    StartPos = StartPos + 1 + 32
    
    adt_FDDM = Mid(adt_FDDM, 1, 4) & Mid(myFDDM, 1, StartPos - 1) & s & Mid(myFDDM, StartPos + 2)
    
End Property

Public Property Get DoodadPaths_Item(ByVal Ndx As Integer) As String
Dim s As String, a() As String
    
    s = Mid(adt_XDMM, InStr(1, adt_XDMM, "world", vbTextCompare))
    a = Split(s, Chr(0))
    DoodadPaths_Item = a(Ndx)
    
End Property

Public Property Get DoodadPaths_Count() As Integer
Dim s As String, a() As String
    
    s = Mid(adt_XDMM, InStr(1, adt_XDMM, "world", vbTextCompare))
    a = Split(s, Chr(0))
    DoodadPaths_Count = UBound(a)
    
End Property

Public Sub MakeDoodadDrawnInChunk(ByVal ChunkID As Integer, ByVal DoodadID As Long)
Dim s As String

    ChunkID = ChunkID - 1
    
    s = adt_KNCM(ChunkID, MCNK_FRCM)
    s = Mid(s, 1, 4) & LongToString32(DoodadID) & Mid(s, 9)
    adt_KNCM(ChunkID, MCNK_FRCM) = s
    
    
    's = adt_KNCM(ChunkID, MCNK_FRCM)
    '
    's = Mid(s, 5)                                               ' trim headder (chunk length)
    's = s & LongToString32(DoodadID)                            ' appaend doodad ID
    's = s & LongToString32(Doodad_DoodadSpecialID(DoodadID))    ' appaend doodad ID
    's = LongToString32(Len(s)) & s                              ' recreate header
    '
    'adt_KNCM(ChunkID, MCNK_FRCM) = s
    
End Sub

Public Property Get WMOPaths_Array() As String()
Dim s As String, a() As String
    
    s = Mid(adt_OMWM, 5)
    s = Replace(s, Chr(0), "")
    s = Replace(s, ".wmo", ".wmo" & vbCrLf)
    a = Split(s, vbCrLf)
    
    If a(UBound(a)) = "" Then ReDim Preserve a(UBound(a) - 1)
    
    WMOPaths_Array = a
    
End Property

Function WMOPaths_Add(ByVal NewPath As String) As Integer
Dim s As String, a() As String, id As Integer

    ' // Adds a path to the id-to-path table for .WMOs   Returns the id
    
    s = Mid(adt_OMWM, 5)
    s = Replace(s, Chr(0), "")
    s = Replace(s, ".wmo", ".wmo" & vbCrLf)
    a = Split(s, vbCrLf)
    If a(UBound(a)) = "" Then ReDim Preserve a(UBound(a) - 1)
    id = UBound(a) + 1
    ReDim Preserve a(id)
    a(id) = NewPath & Chr(0)
    
    adt_OMWM = Join(a, Chr(0))
    adt_OMWM = LongToString32(Len(adt_OMWM)) & adt_OMWM
    
    WMOPaths_Add = id

End Function

Public Property Get WMOPaths_Item(ByVal id As Integer) As String
Dim s As String, a() As String
    
    s = Mid(adt_OMWM, 5)
    s = Replace(s, Chr(0), "")
    s = Replace(s, ".wmo", ".wmo" & vbCrLf)
    a = Split(s, vbCrLf)
    
    If id > UBound(a) Then
        WMOPaths_Item = "<<BAD REFERENCE>> Ref=" & id
    Else
        WMOPaths_Item = a(id)
    End If
    
End Property

Public Property Let WMOPaths_Item(ByVal id As Integer, ByVal vNewValue As String)
Dim s As String, a() As String
    
    s = Mid(adt_OMWM, 5)
    s = Replace(s, Chr(0), "")
    s = Replace(s, ".wmo", ".wmo" & vbCrLf)
    a = Split(s, vbCrLf)
    
    a(id) = vNewValue
    
    s = Join(a, Chr(0))
    
    adt_OMWM = Mid(adt_OMWM, 1, 4) & s
    
End Property

Public Property Get WMO_WMOPath(ByVal Ndx As Long) As String
Dim StartPos As Double, s As String, i As Long, myFDOM As String
On Error GoTo ErrExit

    myFDOM = Mid(adt_FDOM, 5)       ' // I dunno y I have to do this, but for some reason, this kills the bug
        
    StartPos = Ndx - 1
    StartPos = StartPos * 64
    StartPos = StartPos + 1 + 0
    
    s = Mid(myFDOM, StartPos, 4)
    Err.Clear
    i = String32ToLong(s)
    WMO_WMOPath = WMOPaths_Item(i)
    
    Exit Property
    
ErrExit:
    Err.Clear
    WMO_WMOPath = "<<BAD REFERENCE>> Ref=<<BEYOND VB-LONG>>"
    
End Property

Public Property Let WMO_WMOPathId(ByVal Ndx As Long, ByVal vNewValue As Integer)
Dim StartPos As Double, s As String, i As Double, myFDOM As String
Dim b4 As String, later As String

    myFDOM = adt_FDOM
        
    StartPos = Ndx - 1
    StartPos = StartPos * 64
    StartPos = StartPos + 4
    
    myFDOM = Mid(myFDOM, 1, StartPos) & LongToString32(vNewValue) & Mid(myFDOM, StartPos + 5)  '
    
    adt_FDOM = myFDOM
    
End Property

Public Property Get WMO_SpecialId(ByVal Ndx As Long) As Long
Dim StartPos As Double, s As String, i As Double, myFDOM As String

    myFDOM = Mid(adt_FDOM, 5)       ' // I dunno y I have to do this, but for some reason, this kills the bug
        
    StartPos = Ndx - 1
    StartPos = StartPos * 60
    StartPos = StartPos + 1 + 4
    
    s = Mid(myFDOM, StartPos, 4)
    WMO_SpecialId = String32ToFloat(s)
    
End Property

Public Property Get WMO_X(ByVal Ndx As Long) As Single
Dim StartPos As Double, s As String, i As Double, myFDOM As String

    myFDOM = Mid(adt_FDOM, 5)       ' // I dunno y I have to do this, but for some reason, this kills the bug
        
    StartPos = Ndx - 1
    StartPos = StartPos * 64
    StartPos = StartPos + 1 + 8
    
    s = Mid(myFDOM, StartPos, 4)
    WMO_X = String32ToFloat(s)
    
End Property

Public Property Let WMO_X(ByVal Ndx As Long, ByVal vNewValue As Single)
Dim StartPos As Double, s As String, i As Double, myFDOM As String
Dim b4 As String, later As String

    myFDOM = Mid(adt_FDOM, 5)       ' // I dunno y I have to do this, but for some reason, this kills the bug
        
    StartPos = 1 + ((Ndx - 1) * 64) + 7
    s = Mid(myFDOM, StartPos, 4)
    myFDOM = Mid(myFDOM, 1, StartPos) & FloatToString32(vNewValue) & Mid(myFDOM, StartPos + 5)  '
    
    adt_FDOM = Mid(adt_FDOM, 1, 4) & myFDOM
    
End Property

Public Property Get WMO_Y(ByVal Ndx As Long) As Single
Dim StartPos As Double, s As String, i As Double, myFDOM As String

    myFDOM = Mid(adt_FDOM, 5)       ' // I dunno y I have to do this, but for some reason, this kills the bug
        
    StartPos = Ndx - 1
    StartPos = StartPos * 64
    StartPos = StartPos + 1 + 16
    
    s = Mid(myFDOM, StartPos, 4)
    WMO_Y = String32ToFloat(s)
    
End Property

Public Property Let WMO_Y(ByVal Ndx As Long, ByVal vNewValue As Single)
Dim StartPos As Double, s As String, i As Double, myFDOM As String
Dim b4 As String, later As String

    myFDOM = Mid(adt_FDOM, 5)       ' // I dunno y I have to do this, but for some reason, this kills the bug
        
    StartPos = 1 + ((Ndx - 1) * 64) + 15
    s = Mid(myFDOM, StartPos, 4)
    myFDOM = Mid(myFDOM, 1, StartPos) & FloatToString32(vNewValue) & Mid(myFDOM, StartPos + 5)  '
    
    adt_FDOM = Mid(adt_FDOM, 1, 4) & myFDOM
    
End Property

Public Property Get WMO_Z(ByVal Ndx As Long) As Single
Dim StartPos As Double, s As String, i As Double, myFDOM As String

    myFDOM = Mid(adt_FDOM, 5)       ' // I dunno y I have to do this, but for some reason, this kills the bug
        
    StartPos = Ndx - 1
    StartPos = StartPos * 64
    StartPos = StartPos + 1 + 12
    
    s = Mid(myFDOM, StartPos, 4)
    WMO_Z = String32ToFloat(s)
    
End Property

Public Property Let WMO_Z(ByVal Ndx As Long, ByVal vNewValue As Single)
Dim StartPos As Double, s As String, i As Double, myFDOM As String
Dim b4 As String, later As String

    myFDOM = Mid(adt_FDOM, 5)       ' // I dunno y I have to do this, but for some reason, this kills the bug
        
    StartPos = 1 + ((Ndx - 1) * 64) + 11
    s = Mid(myFDOM, StartPos, 4)
    myFDOM = Mid(myFDOM, 1, StartPos) & FloatToString32(vNewValue) & Mid(myFDOM, StartPos + 5)  '
    
    adt_FDOM = Mid(adt_FDOM, 1, 4) & myFDOM
    
End Property

Public Property Get WMO_RotA(ByVal Ndx As Long) As Single
Dim StartPos As Double, s As String, i As Double, myFDOM As String

    myFDOM = Mid(adt_FDOM, 5)       ' // I dunno y I have to do this, but for some reason, this kills the bug
        
    StartPos = Ndx - 1
    StartPos = StartPos * 64
    StartPos = StartPos + 1 + 20
    
    s = Mid(myFDOM, StartPos, 4)
    WMO_RotA = String32ToFloat(s)
    
End Property

Public Property Let WMO_RotA(ByVal Ndx As Long, ByVal vNewValue As Single)
Dim StartPos As Double, s As String, i As Double, myFDOM As String
Dim b4 As String, later As String

    myFDOM = Mid(adt_FDOM, 5)       ' // I dunno y I have to do this, but for some reason, this kills the bug
        
    StartPos = 1 + ((Ndx - 1) * 64) + 19
    s = Mid(myFDOM, StartPos, 4)
    myFDOM = Mid(myFDOM, 1, StartPos) & FloatToString32(vNewValue) & Mid(myFDOM, StartPos + 5)  '
    
    adt_FDOM = Mid(adt_FDOM, 1, 4) & myFDOM
    
End Property

Public Property Get WMO_RotB(ByVal Ndx As Long) As Single
Dim StartPos As Double, s As String, i As Double, myFDOM As String

    myFDOM = Mid(adt_FDOM, 5)       ' // I dunno y I have to do this, but for some reason, this kills the bug
        
    StartPos = Ndx - 1
    StartPos = StartPos * 64
    StartPos = StartPos + 1 + 24
    
    s = Mid(myFDOM, StartPos, 4)
    WMO_RotB = String32ToFloat(s)
    
End Property

Public Property Let WMO_RotB(ByVal Ndx As Long, ByVal vNewValue As Single)
Dim StartPos As Double, s As String, i As Double, myFDOM As String
Dim b4 As String, later As String

    myFDOM = Mid(adt_FDOM, 5)       ' // I dunno y I have to do this, but for some reason, this kills the bug
        
    StartPos = 1 + ((Ndx - 1) * 64) + 23
    s = Mid(myFDOM, StartPos, 4)
    myFDOM = Mid(myFDOM, 1, StartPos) & FloatToString32(vNewValue) & Mid(myFDOM, StartPos + 5)  '
    
    adt_FDOM = Mid(adt_FDOM, 1, 4) & myFDOM
    
End Property

Public Property Get WMO_RotC(ByVal Ndx As Long) As Single
Dim StartPos As Double, s As String, i As Double, myFDOM As String

    myFDOM = Mid(adt_FDOM, 5)       ' // I dunno y I have to do this, but for some reason, this kills the bug
        
    StartPos = Ndx - 1
    StartPos = StartPos * 64
    StartPos = StartPos + 1 + 28
    
    s = Mid(myFDOM, StartPos, 4)
    WMO_RotC = String32ToFloat(s)
    
End Property

Public Property Let WMO_RotC(ByVal Ndx As Long, ByVal vNewValue As Single)
Dim StartPos As Double, s As String, i As Double, myFDOM As String
Dim b4 As String, later As String

    myFDOM = Mid(adt_FDOM, 5)       ' // I dunno y I have to do this, but for some reason, this kills the bug
        
    StartPos = 1 + ((Ndx - 1) * 64) + 27
    s = Mid(myFDOM, StartPos, 4)
    myFDOM = Mid(myFDOM, 1, StartPos) & FloatToString32(vNewValue) & Mid(myFDOM, StartPos + 5)  '
    
    adt_FDOM = Mid(adt_FDOM, 1, 4) & myFDOM
    
End Property

Public Property Get WMO_Unknown1(ByVal Ndx As Long) As Single
Dim StartPos As Double, s As String, i As Double, myFDOM As String

    myFDOM = Mid(adt_FDOM, 5)       ' // I dunno y I have to do this, but for some reason, this kills the bug
        
    StartPos = Ndx - 1
    StartPos = StartPos * 64
    StartPos = StartPos + 1 + 32
    
    s = Mid(myFDOM, StartPos, 4)
    WMO_Unknown1 = String32ToFloat(s)
    
End Property

Public Property Let WMO_Unknown1(ByVal Ndx As Long, ByVal vNewValue As Single)
Dim StartPos As Double, s As String, i As Double, myFDOM As String
Dim b4 As String, later As String

    myFDOM = Mid(adt_FDOM, 5)       ' // I dunno y I have to do this, but for some reason, this kills the bug
        
    StartPos = 1 + ((Ndx - 1) * 64) + 31
    s = Mid(myFDOM, StartPos, 4)
    myFDOM = Mid(myFDOM, 1, StartPos) & FloatToString32(vNewValue) & Mid(myFDOM, StartPos + 5)  '
    
    adt_FDOM = Mid(adt_FDOM, 1, 4) & myFDOM
    
End Property

Public Property Get WMO_Unknown2(ByVal Ndx As Long) As Single
Dim StartPos As Double, s As String, i As Double, myFDOM As String

    myFDOM = Mid(adt_FDOM, 5)       ' // I dunno y I have to do this, but for some reason, this kills the bug
        
    StartPos = Ndx - 1
    StartPos = StartPos * 64
    StartPos = StartPos + 1 + 36
    
    s = Mid(myFDOM, StartPos, 4)
    WMO_Unknown2 = String32ToFloat(s)
    
End Property

Public Property Let WMO_Unknown2(ByVal Ndx As Long, ByVal vNewValue As Single)
Dim StartPos As Double, s As String, i As Double, myFDOM As String
Dim b4 As String, later As String

    myFDOM = Mid(adt_FDOM, 5)       ' // I dunno y I have to do this, but for some reason, this kills the bug
        
    StartPos = 1 + ((Ndx - 1) * 64) + 35
    s = Mid(myFDOM, StartPos, 4)
    myFDOM = Mid(myFDOM, 1, StartPos) & FloatToString32(vNewValue) & Mid(myFDOM, StartPos + 5)  '
    
    adt_FDOM = Mid(adt_FDOM, 1, 4) & myFDOM
    
End Property

Public Property Get WMO_Unknown3(ByVal Ndx As Long) As Single
Dim StartPos As Double, s As String, i As Double, myFDOM As String

    myFDOM = Mid(adt_FDOM, 5)       ' // I dunno y I have to do this, but for some reason, this kills the bug
        
    StartPos = Ndx - 1
    StartPos = StartPos * 64
    StartPos = StartPos + 1 + 40
    
    s = Mid(myFDOM, StartPos, 4)
    WMO_Unknown3 = String32ToFloat(s)
    
End Property

Public Property Let WMO_Unknown3(ByVal Ndx As Long, ByVal vNewValue As Single)
Dim StartPos As Double, s As String, i As Double, myFDOM As String
Dim b4 As String, later As String

    myFDOM = Mid(adt_FDOM, 5)       ' // I dunno y I have to do this, but for some reason, this kills the bug
        
    StartPos = 1 + ((Ndx - 1) * 64) + 39
    s = Mid(myFDOM, StartPos, 4)
    myFDOM = Mid(myFDOM, 1, StartPos) & FloatToString32(vNewValue) & Mid(myFDOM, StartPos + 5)  '
    
    adt_FDOM = Mid(adt_FDOM, 1, 4) & myFDOM
    
End Property

Public Property Get WMO_Unknown4(ByVal Ndx As Long) As Single
Dim StartPos As Double, s As String, i As Double, myFDOM As String

    myFDOM = Mid(adt_FDOM, 5)       ' // I dunno y I have to do this, but for some reason, this kills the bug
        
    StartPos = Ndx - 1
    StartPos = StartPos * 64
    StartPos = StartPos + 1 + 44
    
    s = Mid(myFDOM, StartPos, 4)
    WMO_Unknown4 = String32ToFloat(s)
    
End Property

Public Property Let WMO_Unknown4(ByVal Ndx As Long, ByVal vNewValue As Single)
Dim StartPos As Double, s As String, i As Double, myFDOM As String
Dim b4 As String, later As String

    myFDOM = Mid(adt_FDOM, 5)       ' // I dunno y I have to do this, but for some reason, this kills the bug
        
    StartPos = 1 + ((Ndx - 1) * 64) + 43
    s = Mid(myFDOM, StartPos, 4)
    myFDOM = Mid(myFDOM, 1, StartPos) & FloatToString32(vNewValue) & Mid(myFDOM, StartPos + 5)  '
    
    adt_FDOM = Mid(adt_FDOM, 1, 4) & myFDOM
    
End Property

Public Property Get WMO_Unknown5(ByVal Ndx As Long) As Single
Dim StartPos As Double, s As String, i As Double, myFDOM As String

    myFDOM = Mid(adt_FDOM, 5)       ' // I dunno y I have to do this, but for some reason, this kills the bug
        
    StartPos = Ndx - 1
    StartPos = StartPos * 64
    StartPos = StartPos + 1 + 48
    
    s = Mid(myFDOM, StartPos, 4)
    WMO_Unknown5 = String32ToFloat(s)
    
End Property

Public Property Let WMO_Unknown5(ByVal Ndx As Long, ByVal vNewValue As Single)
Dim StartPos As Double, s As String, i As Double, myFDOM As String
Dim b4 As String, later As String

    myFDOM = Mid(adt_FDOM, 5)       ' // I dunno y I have to do this, but for some reason, this kills the bug
        
    StartPos = 1 + ((Ndx - 1) * 64) + 47
    s = Mid(myFDOM, StartPos, 4)
    myFDOM = Mid(myFDOM, 1, StartPos) & FloatToString32(vNewValue) & Mid(myFDOM, StartPos + 5)  '
    
    adt_FDOM = Mid(adt_FDOM, 1, 4) & myFDOM
    
End Property

Public Property Get WMO_Unknown6(ByVal Ndx As Long) As Single
Dim StartPos As Double, s As String, i As Double, myFDOM As String

    myFDOM = Mid(adt_FDOM, 5)       ' // I dunno y I have to do this, but for some reason, this kills the bug
        
    StartPos = Ndx - 1
    StartPos = StartPos * 64
    StartPos = StartPos + 1 + 52
    
    s = Mid(myFDOM, StartPos, 4)
    WMO_Unknown6 = String32ToFloat(s)
    
End Property

Public Property Let WMO_Unknown6(ByVal Ndx As Long, ByVal vNewValue As Single)
Dim StartPos As Double, s As String, i As Double, myFDOM As String
Dim b4 As String, later As String

    myFDOM = Mid(adt_FDOM, 5)       ' // I dunno y I have to do this, but for some reason, this kills the bug
        
    StartPos = 1 + ((Ndx - 1) * 64) + 51
    s = Mid(myFDOM, StartPos, 4)
    myFDOM = Mid(myFDOM, 1, StartPos) & FloatToString32(vNewValue) & Mid(myFDOM, StartPos + 5)  '
    
    adt_FDOM = Mid(adt_FDOM, 1, 4) & myFDOM
    
End Property

Public Property Get WMO_Flags(ByVal Ndx As Long) As Single
Dim StartPos As Double, s As String, i As Double, myFDOM As String

    myFDOM = Mid(adt_FDOM, 5)       ' // I dunno y I have to do this, but for some reason, this kills the bug
        
    StartPos = Ndx - 1
    StartPos = StartPos * 64
    StartPos = StartPos + 1 + 56
    
    s = Mid(myFDOM, StartPos, 4)
    WMO_Flags = String32ToFloat(s)
    
End Property

Public Property Get WMO_DoodadSet(ByVal Ndx As Long) As Single
Dim StartPos As Double, s As String, i As Double, myFDOM As String

    myFDOM = Mid(adt_FDOM, 5)       ' // I dunno y I have to do this, but for some reason, this kills the bug
        
    StartPos = Ndx - 1
    StartPos = StartPos * 64
    StartPos = StartPos + 1 + 60
    
    s = Mid(myFDOM, StartPos, 2)
    WMO_DoodadSet = String32ToFloat(s)
    
End Property

Public Property Get WMO_NameSet(ByVal Ndx As Long) As Single
Dim StartPos As Double, s As String, i As Double, myFDOM As String

    myFDOM = Mid(adt_FDOM, 5)       ' // I dunno y I have to do this, but for some reason, this kills the bug
        
    StartPos = Ndx - 1
    StartPos = StartPos * 62
    StartPos = StartPos + 1 + 62
    
    s = Mid(myFDOM, StartPos, 2)
    WMO_NameSet = String32ToFloat(s)
    
End Property

Public Property Get WMO_Count() As Integer
Dim s As String, a() As String, i As Integer
    
    s = Mid(adt_FDOM, 5)
    i = Len(s) / 64
    WMO_Count = i
    
End Property

Sub WMO_Add(ByVal file_id As Integer, BaseCopyId As Integer, ByVal X As Single, ByVal Y As Single, ByVal Z As Single, ByVal a As Single, ByVal b As Single, ByVal C As Single)
Dim uid As String, i As Long, s As String, pos As String, rot As String
Dim sInject As String
    
    ' a = WMO targ ID
    ' b = Unique ID for instance
    ' c = pos
    ' d = rotation
    ' e = unknown (copy from base)
    ' f = unknwon (copy from base)
    ' g = always 0?
    ' h = doodad set index
    ' i = name set
        sInject = "AAAABBBBCCCCCCCCCCCCDDDDDDDDDDDDEEEEEEEEEEEEFFFFFFFFFFFFGGHHIIII"
    
    ' WMO targ ID
        sInject = Replace(sInject, "AAAA", LongToString32(file_id))
    
    ' Unique ID
        For i = 0 To 10000
            If InStr(1, adt_FDOM, LongToString32(i)) = 0 Then Exit For
        Next
        uid = LongToString32(i)
        sInject = Replace(sInject, "BBBB", uid)
    
    ' Position
        pos = FloatToString32(X)
        pos = pos & FloatToString32(Y)
        pos = pos & FloatToString32(Z)
        sInject = Replace(sInject, "CCCCCCCCCCCC", pos)
    
    ' Rotation
        rot = FloatToString32(a)
        rot = rot & FloatToString32(b)
        rot = rot & FloatToString32(C)
        'sInject = Replace(sInject, "DDDDDDDDDDDD", pos)
        sInject = Replace(sInject, "DDDDDDDDDDDD", Chr(0) & Chr(0) & Chr(0) & Chr(0) & Chr(0) & Chr(0) & Chr(26) & Chr(66) & Chr(0) & Chr(0) & Chr(0) & Chr(0))
        
    ' Unknown flags
        sInject = Replace(sInject, "EEEEEEEEEEEE", pos)
        sInject = Replace(sInject, "FFFFFFFFFFFF", rot)
        
    ' 0 block?
        sInject = Replace(sInject, "GG", Chr(0) & Chr(0))
        
    ' Doodad set index?
        sInject = Replace(sInject, "HH", Chr(0) & Chr(0))
        
    ' Name set?
        sInject = Replace(sInject, "IIII", Chr(0) & Chr(0) & Chr(0) & Chr(0))
        
        
    adt_FDOM = sInject & Mid(adt_FDOM, 5)
    adt_FDOM = LongToString32(Len(adt_FDOM)) & adt_FDOM
        
    's = Mid(adt_FDOM, 5, 5 + 64)
    's = LongToString32(file_id) & uid & Mid(s, 9)
    's = Strings.Left(s & String(32, Chr(0)), 64)
    'adt_FDOM = s & Mid(adt_FDOM, 5)
    'adt_FDOM = LongToString32(Len(adt_FDOM)) & adt_FDOM
    
End Sub

Sub SaveADT(FilePath As String)
Dim X As Integer, s As String
Dim SFSO
Dim Data As String
Dim KNCM As String
Dim off As Long
Dim Mode As Integer
Dim DummyKNCMHead, OffsetLiq As Integer, LiqLen As Integer, OffseSound As Integer
        
        
    ' //  I hate offsets.....  why cant we just live in a world of XML!?    *cry*
        
        
        
    For Mode = 1 To 2
        
        Data = ""
        Data = Data & StrReverse("MCIN") & adt_NICM
        Data = Data & StrReverse("MTEX") & adt_XETM
        Data = Data & StrReverse("MMDX") & adt_XDMM
        Data = Data & StrReverse("MMID") & adt_DIMM
        Data = Data & StrReverse("MWMO") & adt_OMWM
        Data = Data & StrReverse("MWID") & adt_DIWM
        Data = Data & StrReverse("MDDF") & adt_FDDM
        Data = Data & StrReverse("MODF") & adt_FDOM
        
        If Mode = 1 Then
            
            adt_NICM = CompileNICM(Len(Data) + 84)
            
        End If
        
    Next
    
            Kill2 FilePath
            Set SFSO = CreateObject("Scripting.FileSystemObject")
            Set SFSO = SFSO.CreateTextFile(FilePath)
            
            For X = 0 To UBound(adt_KNCM)
            
                
                DummyKNCMHead = ""
                DummyKNCMHead = DummyKNCMHead & "KNCM" & adt_KNCM(X, MCNK_MCNK)
                DummyKNCMHead = DummyKNCMHead & "TVCM" & adt_KNCM(X, MCNK_TVCM)
                DummyKNCMHead = DummyKNCMHead & "RNCM" & adt_KNCM(X, MCNK_RNCM)
                DummyKNCMHead = DummyKNCMHead & "YLCM" & adt_KNCM(X, MCNK_YLCM)
                DummyKNCMHead = DummyKNCMHead & "FRCM" & adt_KNCM(X, MCNK_FRCM)
                DummyKNCMHead = DummyKNCMHead & "HSCM" & adt_KNCM(X, MCNK_HSCM)
                DummyKNCMHead = DummyKNCMHead & "LACM" & adt_KNCM(X, MCNK_LACM)
                DummyKNCMHead = DummyKNCMHead & "QLCM" & adt_KNCM(X, MCNK_QLCM)
                DummyKNCMHead = DummyKNCMHead & "ESCM" & adt_KNCM(X, MCNK_ESCM)
                
                OffsetLiq = InStr(1, DummyKNCMHead, "QLCM", vbTextCompare) - 1
                OffseSound = InStr(1, DummyKNCMHead, "ESCM", vbTextCompare) - 1
                
                LiqLen = Len(adt_KNCM(X, MCNK_QLCM)) + 4
                
                s = adt_KNCM(X, MCNK_MCNK)

                s = Mid(s, 1, (22 * 4) + 4) & LongToString32(OffseSound) & Mid(s, (23 * 4) + 1 + 4)
                DummyKNCMHead = Mid(s, 1, 100) & LongToString32(OffsetLiq) & LongToString32(LiqLen) & Mid(s, 109)
                
                If Len(adt_KNCM(X, MCNK_QLCM)) < 4 Then
                    'adt_KNCM(X, MCNK_QLCM) = chr(0)
                    'Err.Raise 1
                End If
                
                
                SFSO.Write "KNCM" & DummyKNCMHead
                SFSO.Write "TVCM" & adt_KNCM(X, MCNK_TVCM)
                SFSO.Write "RNCM" & adt_KNCM(X, MCNK_RNCM)
                SFSO.Write "YLCM" & adt_KNCM(X, MCNK_YLCM)
                SFSO.Write "FRCM" & adt_KNCM(X, MCNK_FRCM)
                SFSO.Write "HSCM" & adt_KNCM(X, MCNK_HSCM)
                SFSO.Write "LACM" & adt_KNCM(X, MCNK_LACM)
                SFSO.Write "QLCM" & adt_KNCM(X, MCNK_QLCM)
                SFSO.Write "ESCM" & adt_KNCM(X, MCNK_ESCM)
                DoEvents
            Next
            
            SFSO.Close
            Set SFSO = Nothing
            
            
        KNCM = ReadFile(FilePath)
        Kill FilePath
        Data = Data & KNCM
        
        
        adt_RDHM = ""
        adt_RDHM = adt_RDHM & Chr(64) & Chr(0) & Chr(0) & Chr(0)
        adt_RDHM = adt_RDHM & LongToString32(0)
        adt_RDHM = adt_RDHM & LongToString32(83 + InStr(1, Data, "NICM") - 20)
        adt_RDHM = adt_RDHM & LongToString32(83 + InStr(1, Data, "XETM") - 20)
        adt_RDHM = adt_RDHM & LongToString32(83 + InStr(1, Data, "XDMM") - 20)
        adt_RDHM = adt_RDHM & LongToString32(83 + InStr(1, Data, "DIMM") - 20)
        adt_RDHM = adt_RDHM & LongToString32(83 + InStr(1, Data, "OMWM") - 20)
        adt_RDHM = adt_RDHM & LongToString32(83 + InStr(1, Data, "DIWM") - 20)
        adt_RDHM = adt_RDHM & LongToString32(83 + InStr(1, Data, "FDDM") - 20)
        adt_RDHM = adt_RDHM & LongToString32(83 + InStr(1, Data, "FDOM") - 20)
        
        adt_RDHM = StrReverse("MVER") & adt_REVM & "RDHM" & adt_RDHM & String(100, Chr(0))
        adt_RDHM = Strings.Left(adt_RDHM, 84)
        
        WriteFile FilePath, adt_RDHM & Data
    
    
    
    

'    adt_RDHM_len = 16 * 4
'    adt_REVM_len = Len(adt_REVM)
'    adt_XETM_len = Len(adt_XETM)
'    adt_NICM_len = Len(adt_NICM)
'    adt_XDMM_len = Len(adt_XDMM)
'    adt_DIMM_len = Len(adt_DIMM)
'    adt_OMWM_len = Len(adt_OMWM)
'    adt_DIWM_len = Len(adt_DIWM)
'    adt_FDDM_len = Len(adt_FDDM)
'
'' // Recompile adt_RDHM.  All offsts relitive to 20.  This block length = 68.
'    ' //
'        adt_RDHM = ""
'        adt_RDHM = adt_RDHM & LongToString32(64)
'    ' // Always 0?  Dont ask me why
'        adt_RDHM = adt_RDHM & Chr(0) & Chr(0) & Chr(0) & Chr(0)
''    ' //
'        adt_RDHM = adt_RDHM & LongToString32(adt_REVM_len + adt_RDHM_len - 20)
'    ' // offset to MTEX block
'        adt_RDHM = adt_RDHM & LongToString32(adt_REVM_len + adt_NICM_len + 20)
'    ' // offset to XDMM block
'        adt_RDHM = adt_RDHM & LongToString32(adt_REVM_len + adt_XETM_len + adt_NICM_len + 20)
'    ' // offset to DIMM block
'        adt_RDHM = adt_RDHM & LongToString32(adt_REVM_len + adt_XETM_len + adt_NICM_len + adt_XDMM_len + 20)
'    ' // offset to OMWM block
'        adt_RDHM = adt_RDHM & LongToString32(adt_REVM_len + adt_XETM_len + adt_NICM_len + adt_XDMM_len + adt_DIMM_len + 20)
'    ' // offset to DIWM block
'        adt_RDHM = adt_RDHM & LongToString32(adt_REVM_len + adt_XETM_len + adt_NICM_len + adt_XDMM_len + adt_DIMM_len + adt_OMWM_len + 20)
'    ' // offset to FDDM block
'        adt_RDHM = adt_RDHM & LongToString32(adt_REVM_len + adt_XETM_len + adt_NICM_len + adt_XDMM_len + adt_DIMM_len + adt_OMWM_len + adt_DIWM_len + 20)
'    ' // offset to FDDM block
'        adt_RDHM = adt_RDHM & LongToString32(adt_REVM_len + adt_XETM_len + adt_NICM_len + adt_XDMM_len + adt_DIMM_len + adt_OMWM_len + adt_DIWM_len + 20)
'    ' // offset to MODF block
'        adt_RDHM = adt_RDHM & LongToString32(adt_REVM_len + adt_XETM_len + adt_NICM_len + adt_XDMM_len + adt_DIMM_len + adt_OMWM_len + adt_DIWM_len + adt_FDDM_len + 20)
'    ' // Padding with 7 0-value DWORDs
'        adt_RDHM = adt_RDHM & String(4 * 7, Chr(0))
'    ' // Bug killer
'        adt_RDHM = Strings.Left(adt_RDHM, 68)

End Sub


Function CompileNICM(Sect1Len As Long) As String
Dim i As Integer, s As String, LastAbsOffset As Long, ThisAbsOffset As Long
    
    s = LongToString32(Sect1Len) & LongToString32(KNCMLength(0)) & LongToString32(0) & LongToString32(0)
    LastAbsOffset = Sect1Len
    
    For i = 1 To 255     ' all section-2 (KNCM) chunks, but not the 1st
        
        ThisAbsOffset = LastAbsOffset + KNCMLength(i - 1)
        s = s & LongToString32(ThisAbsOffset) & LongToString32(KNCMLength(i)) & LongToString32(0) & LongToString32(0)
        LastAbsOffset = ThisAbsOffset
        
    Next
    
    s = LongToString32(Len(s)) & s
    
    CompileNICM = s

End Function

Private Function KNCMLength(id As Integer) As Long
Dim s As String

    s = s & "KNCM" & adt_KNCM(id, MCNK_MCNK)
    s = s & "TVCM" & adt_KNCM(id, MCNK_TVCM)
    s = s & "RNCM" & adt_KNCM(id, MCNK_RNCM)
    s = s & "YLCM" & adt_KNCM(id, MCNK_YLCM)
    s = s & "FRCM" & adt_KNCM(id, MCNK_FRCM)
    s = s & "HSCM" & adt_KNCM(id, MCNK_HSCM)
    s = s & "LACM" & adt_KNCM(id, MCNK_LACM)
    s = s & "QLCM" & adt_KNCM(id, MCNK_QLCM)
    s = s & "ESCM" & adt_KNCM(id, MCNK_ESCM)
    
    KNCMLength = Len(s)

End Function


'Sub OffsetMCIN(ByNbrOfBytes As Long)
'Dim b() As Long
'Dim s As String
'Dim OrigLen As Long
'Dim i As Long
'
'    OrigLen = Len(adt_NICM)
'
'    ReDim b(Len(adt_NICM) + 1)
'    s = Mid(adt_NICM, 5)
'
'    CopyMemory b(0), ByVal s, Len(s)
'
'    For i = 0 To UBound(b) Step 4
'        b(i) = b(i) + ByNbrOfBytes
'    Next
'
'    CopyMemory ByVal s, b(0), Len(s)
'
'    adt_NICM = Mid(adt_NICM, 1, 4) & s
'    adt_NICM = Strings.Left(adt_NICM, OrigLen)
'
'End Sub

Sub OpenADT(FilePath As String)
Dim ADTdata As String
Dim s As String, X As Integer, Y As Integer, a() As String, arr() As String

'//
'//     ADT chunk parser
'//
'//
'//     INPUT:  The file path to the .adt file extracted from the .mpq file
'//
'//     OUTPUT: This method will place data into the public vars adt_*
'//
'//
'//     .adt files are chuk based, each chunk has its own rules
'//     on how long it is, but seeing as each header (in section
'//     one) appears only once, I decided to not care on
'//     pulling a certain # of chars out after a header...
'//     I'll just split it into an aray  using the headders delimiters.
'//     The stardard .adt header order is: REVM, RDHM, NICM,
'//     XETM, XDMM, DIMM, OMWM, DIWM, FDDM, FDOM
'//
'//     Then for section two (where headers appear more than once)
'//     I'll split it  into an array by the KNCM headder, and then
'//     into sub array w/ the other headders.  Headder list is:
'//     KNCM, TVCM, RNCM, YLCM, FRCM, HSCM, LACM, QLCM, ESCM

    Load GUIShowMsg
    GUIShowMsg.Show
    GUIShowMsg.Bar1.Value = 0
    GUIShowMsg.Bar1.Value = 20
    GUIShowMsg.Refresh
    DoEvents

    ADTdata = ReadFile(FilePath)
    adtFileLen = Len(ADTdata)   ' // Save data as the var s will have half cut off
    
    GUIShowMsg.Bar1.Value = 1
    GUIShowMsg.Refresh
    DoEvents

    s = Split(ADTdata, "KNCM")(0)               ' // Split out section 1 fom section 2
    
    If InStr(1, s, "REVM") = 0 Then MsgBox "Error: Could not find header 'REVM' in section 1 of .adt file!"
    If InStr(1, s, "RDHM") = 0 Then MsgBox "Error: Could not find header 'RDHM' in section 1 of .adt file!"
    If InStr(1, s, "NICM") = 0 Then MsgBox "Error: Could not find header 'NICM' in section 1 of .adt file!"
    If InStr(1, s, "XETM") = 0 Then MsgBox "Error: Could not find header 'XETM' in section 1 of .adt file!"
    If InStr(1, s, "XDMM") = 0 Then MsgBox "Error: Could not find header 'XDMM' in section 1 of .adt file!"
    If InStr(1, s, "DIMM") = 0 Then MsgBox "Error: Could not find header 'DIMM' in section 1 of .adt file!"
    If InStr(1, s, "OMWM") = 0 Then MsgBox "Error: Could not find header 'OMWM' in section 1 of .adt file!"
    If InStr(1, s, "DIWM") = 0 Then MsgBox "Error: Could not find header 'DIWM' in section 1 of .adt file!"
    If InStr(1, s, "FDDM") = 0 Then MsgBox "Error: Could not find header 'FDDM' in section 1 of .adt file!"
    If InStr(1, s, "FDOM") = 0 Then MsgBox "Error: Could not find header 'FDOM' in section 1 of .adt file!"
    
    s = Replace(s, "REVM", "_DELIM_")           ' // Replace headers with a constant delimiter
    GUIShowMsg.Bar1.Value = 1
    GUIShowMsg.Refresh
    DoEvents
    s = Replace(s, "RDHM", "_DELIM_")           ' // Replace headers with a constant delimiter
    GUIShowMsg.Bar1.Value = 2
    s = Replace(s, "NICM", "_DELIM_")           ' // Replace headers with a constant delimiter
    GUIShowMsg.Bar1.Value = 3
    GUIShowMsg.Refresh
    DoEvents
    s = Replace(s, "XETM", "_DELIM_")           ' // Replace headers with a constant delimiter
    GUIShowMsg.Bar1.Value = 4
    s = Replace(s, "XDMM", "_DELIM_")           ' // Replace headers with a constant delimiter
    GUIShowMsg.Bar1.Value = 5
    GUIShowMsg.Refresh
    DoEvents
    s = Replace(s, "DIMM", "_DELIM_")           ' // Replace headers with a constant delimiter
    GUIShowMsg.Bar1.Value = 6
    s = Replace(s, "OMWM", "_DELIM_")           ' // Replace headers with a constant delimiter
    GUIShowMsg.Bar1.Value = 7
    GUIShowMsg.Refresh
    DoEvents
    s = Replace(s, "DIWM", "_DELIM_")           ' // Replace headers with a constant delimiter
    GUIShowMsg.Bar1.Value = 8
    s = Replace(s, "FDDM", "_DELIM_")           ' // Replace headers with a constant delimiter
    GUIShowMsg.Bar1.Value = 9
    GUIShowMsg.Refresh
    DoEvents
    s = Replace(s, "FDOM", "_DELIM_")           ' // Replace headers with a constant delimiter
    GUIShowMsg.Bar1.Value = 10
    
    a = Split(s, "_DELIM_")
    
    If UBound(a) <> 10 Then
        MsgBox "Error reading .adt file in section 1", vbCritical
        Err.Raise 1
    End If
    
    adt_REVM = a(1)
    adt_RDHM = a(2)
    adt_NICM = a(3)
    adt_XETM = a(4)
    adt_XDMM = a(5)
    adt_DIMM = a(6)
    adt_OMWM = a(7)
    adt_DIWM = a(8)
    adt_FDDM = a(9)
    adt_FDOM = a(10)
    
    
    ' // Now for section 2 - where header appear more than once.  I'll divide this
    s = Mid(ADTdata, InStr(1, ADTdata, "KNCM") + 4)
    
    s = Replace(s, "TVCM", "_DELMI_TVCM")
    s = Replace(s, "RNCM", "_DELMI_RNCM")
    s = Replace(s, "YLCM", "_DELMI_YLCM")
    s = Replace(s, "FRCM", "_DELMI_FRCM")
    s = Replace(s, "HSCM", "_DELMI_HSCM")
    s = Replace(s, "LACM", "_DELMI_LACM")
    s = Replace(s, "QLCM", "_DELMI_QLCM")
    s = Replace(s, "ESCM", "_DELMI_ESCM")
    
    s = Replace(s, "KNCM", "_2DELIM2_KNCM")
    a = Split(s, "_2DELIM2_")
    a(0) = "KNCM" & a(0)
    
    If UBound(a) <> 255 Then
        MsgBox "Error read .adt file in section 2 - there is not 255 KNCM (MCNK) chunks in this file", vbExclamation
        Err.Raise 1
    End If

    For X = 0 To UBound(a)
        
        arr = Split(a(X), "_DELMI_")
        
        For Y = 0 To UBound(arr)
            Select Case UCase(Mid(arr(Y), 1, 4))
            Case "TVCM":    adt_KNCM(X, MCNK_TVCM) = Mid(arr(Y), 5)
            Case "RNCM":    adt_KNCM(X, MCNK_RNCM) = Mid(arr(Y), 5)
            Case "YLCM":    adt_KNCM(X, MCNK_YLCM) = Mid(arr(Y), 5)
            Case "FRCM":    adt_KNCM(X, MCNK_FRCM) = Mid(arr(Y), 5)
            Case "HSCM":    adt_KNCM(X, MCNK_HSCM) = Mid(arr(Y), 5)
            Case "LACM":    adt_KNCM(X, MCNK_LACM) = Mid(arr(Y), 5)
            Case "QLCM":    adt_KNCM(X, MCNK_QLCM) = Mid(arr(Y), 5)
            Case "ESCM":    adt_KNCM(X, MCNK_ESCM) = Mid(arr(Y), 5)
            Case "KNCM":    adt_KNCM(X, MCNK_MCNK) = Mid(arr(Y), 5)
            Case Else:      Err.Raise 1
            End Select
        Next
        
        GUIShowMsg.Refresh
        DoEvents
        
    Next
    
    GUIShowMsg.Bar1.Value = 20
    Unload GUIShowMsg
    
    
End Sub

Private Sub Class_Initialize()
    WaterFlag = 4
End Sub
