VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "WMOGrpFile"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Public WMOTargFile As String

Private WMOHeadder As String
Private YPOM As String
Private IVOM As String
Private TVOM As String
Private RNOM As String
Private VTOM As String
Private ABOM As String
Private RLOM As String
Private RDOM As String
Private NBOM As String
Private RBOM As String
Private VCOM As String
Public QILM As String



Sub SaveWMO(FilePath As String)
Dim Data As String
    
    Data = WMOHeadder
    If YPOM <> "" Then Data = Data & "YPOM" & YPOM
    If IVOM <> "" Then Data = Data & "IVOM" & IVOM
    If TVOM <> "" Then Data = Data & "TVOM" & TVOM
    If RNOM <> "" Then Data = Data & "RNOM" & RNOM
    If VTOM <> "" Then Data = Data & "VTOM" & VTOM
    If ABOM <> "" Then Data = Data & "ABOM" & ABOM
    If RLOM <> "" Then Data = Data & "RLOM" & RLOM
    If RDOM <> "" Then Data = Data & "RDOM" & RDOM
    If NBOM <> "" Then Data = Data & "NBOM" & NBOM
    If RBOM <> "" Then Data = Data & "RBOM" & RBOM
    If VCOM <> "" Then Data = Data & "VCOM" & VCOM
    If QILM <> "" Then Data = Data & "QILM" & QILM
    
    WriteFile FilePath, Data
    
End Sub

Sub OpenWMO(FilePath As String)
Dim s As String, OldS As String, h As String, harr() As String, a() As String, X As Integer

    WMOHeadder = ""
    YPOM = ""
    IVOM = ""
    TVOM = ""
    RNOM = ""
    VTOM = ""
    ABOM = ""
    RLOM = ""
    RDOM = ""
    NBOM = ""
    RBOM = ""
    VCOM = ""
    QILM = ""

    s = ReadFile(FilePath)
    
    OldS = s
    
    If InStr(1, s, "YPOM") <> 0 Then h = h & "YPOM "
    s = Replace(s, "YPOM", "_DELIM_")
    
    If InStr(1, s, "IVOM") <> 0 Then h = h & "IVOM "
    s = Replace(s, "IVOM", "_DELIM_")
    
    If InStr(1, s, "TVOM") <> 0 Then h = h & "TVOM "
    s = Replace(s, "TVOM", "_DELIM_")
    
    If InStr(1, s, "RNOM") <> 0 Then h = h & "RNOM "
    s = Replace(s, "RNOM", "_DELIM_")
    
    If InStr(1, s, "VTOM") <> 0 Then h = h & "VTOM "
    s = Replace(s, "VTOM", "_DELIM_")
    
    If InStr(1, s, "ABOM") <> 0 Then h = h & "ABOM "
    s = Replace(s, "ABOM", "_DELIM_")
    
    If InStr(1, s, "RLOM") <> 0 Then h = h & "RLOM "
    s = Replace(s, "RLOM", "_DELIM_")
    
    If InStr(1, s, "RDOM") <> 0 Then h = h & "RDOM "
    s = Replace(s, "RDOM", "_DELIM_")
    
    If InStr(1, s, "NBOM") <> 0 Then h = h & "NBOM "
    s = Replace(s, "NBOM", "_DELIM_")
    
    If InStr(1, s, "RBOM") <> 0 Then h = h & "RBOM "
    s = Replace(s, "RBOM", "_DELIM_")
    
    If InStr(1, s, "VCOM") <> 0 Then h = h & "VCOM "
    s = Replace(s, "VCOM", "_DELIM_")
    
    If InStr(1, s, "QILM") <> 0 Then h = h & "QILM "
    s = Replace(s, "QILM", "_DELIM_")
    
    harr = Split("Head " & Trim(h), " ")
    a = Split(s, "_DELIM_")
    
    For X = 0 To UBound(a)
    
        Select Case harr(X)
        Case "Head":        WMOHeadder = a(X)
        Case "YPOM":        YPOM = a(X)
        Case "IVOM":        IVOM = a(X)
        Case "TVOM":        TVOM = a(X)
        Case "RNOM":        RNOM = a(X)
        Case "VTOM":        VTOM = a(X)
        Case "ABOM":        ABOM = a(X)
        Case "RLOM":        RLOM = a(X)
        Case "RDOM":        RDOM = a(X)
        Case "NBOM":        NBOM = a(X)
        Case "RBOM":        RBOM = a(X)
        Case "VCOM":        VCOM = a(X)
        Case "QILM":        QILM = a(X)
        Case Else
            MsgBox "Error: Uncaught headder in WMO file."
            Err.Raise 1
        End Select
       
    Next
    
    
' // Error checking
    
    If (Len(YPOM) / 2) <> Round(Len(YPOM) / 2) Then
        MsgBox "Error: The length in the YPOM (MOPY) string is odd (not even).  This can't be right based on the two-byte per triangle protocall", vbExclamation
        Err.Raise 1
    End If
    
    If (Len(RDOM) / 2) <> Round(Len(RDOM) / 2) Then
        MsgBox "Error: The length in the RDOM (MODR) string is odd (not even).  This can't be right based on the two-byte (16-bit integer) id for each doodad", vbExclamation
        Err.Raise 1
    End If
    
    WMOTargFile = FilePath
    
End Sub

Public Property Get BSPNodeCount() As Long
Dim s As String, i As Integer

    s = NBOM
    s = Mid(s, 1, 4)
    
    i = String32ToLong(s)
    i = i / 16
    
    BSPNodeCount = i

End Property

Public Property Get BSPNode_FirstFace_MOBR(ByVal BSPIndex As Integer) As Long
Dim s As String, i As Integer

    s = NBOM
    s = Mid(s, ((BSPIndex - 1) * 16) + 5 + 5 + 1 + 2)
    
    i = String32ToLong(Mid(s, 1, 2))
    
    BSPNode_FirstFace_MOBR = i

End Property

Public Property Get BSPNode_FirstFace_TriID(ByVal BSPIndex As Integer) As Long
Dim IndexMOBR As Integer
    
    IndexMOBR = BSPNode_FirstFace_MOBR(BSPIndex)
    BSPNode_FirstFace_TriID = RBOM_IndexArray(IndexMOBR)

End Property

Public Property Get BSPNode_NumbFace(ByVal BSPIndex As Integer) As Long
Dim s As String, i As Integer

    s = NBOM
    s = Mid(s, ((BSPIndex - 1) * 16) + 5 + 5 + 1)
    
    i = String32ToLong(Mid(s, 1, 2))
    
    BSPNode_NumbFace = i

End Property

Public Property Let BSPNode_NumbFace(ByVal BSPIndex As Integer, vNewValue As Long)
Dim s As String, i As Integer

    BSPIndex = BSPIndex - 1

    'MsgBox Len(NBOM)
    'MsgBox BSPNode_NumbFace(BSPIndex + 1)

    s = NBOM
    s = Mid(s, 1, (BSPIndex * 16) + 5 + 5 + 1 - 1) & Strings.Left(LongToString32(vNewValue), 2) & Mid(s, (BSPIndex * 16) + 5 + 5 + 1 - 1 + 3)
    NBOM = s
    
    'MsgBox Len(NBOM)
    'MsgBox BSPNode_NumbFace(BSPIndex + 1)
    
End Property

Public Property Get BSPNode_PlaneType(ByVal BSPIndex As Integer) As Long
Dim s As String, i As Integer

    s = NBOM
    s = Mid(s, ((BSPIndex - 1) * 16) + 5)
    
    i = String32ToLong(Mid(s, 1, 2))
    
    BSPNode_PlaneType = i

End Property

Public Property Let BSPNode_PlaneType(ByVal BSPIndex As Integer, vNewValue As Long)
Dim s As String, i As Integer

    BSPIndex = BSPIndex - 1

    'MsgBox Len(NBOM)
    'MsgBox BSPNode_PlaneType(BSPIndex + 1)

    s = NBOM
    s = Mid(s, 1, (BSPIndex * 16) + 5 - 1) & Strings.Left(LongToString32(vNewValue), 2) & Mid(s, (BSPIndex * 16) + 5 - 1 + 3)
    NBOM = s
    
    'MsgBox Len(NBOM)
    'MsgBox BSPNode_PlaneType(BSPIndex + 1)

End Property

Public Property Get WMOHeadder_GrpName() As Long
Dim i As Integer, s As String

    i = InStr(1, WMOHeadder, "PGOM") + 4 + 4
    s = Mid(WMOHeadder, i, 4)
    WMOHeadder_GrpName = String32ToLong(StrReverse(s))
    
    MsgBox "BAD DATA.  This method does not work"
    
End Property

Public Property Get RDOM_DoodadID(ByVal ArrayNbr As Long) As String

    RDOM_DoodadID = Mid(RDOM, ArrayNbr * 2, 2)
    
End Property

Public Property Let RDOM_DoodadID(ByVal ArrayNbr As Long, ByVal vNewValue As String)
Dim OldLen As Long, NewLen As Long

    ArrayNbr = (ArrayNbr * 2) - 1
    
    OldLen = Len(RDOM)
    RDOM = Mid(RDOM, 1, ArrayNbr - 1) & vNewValue & Mid(RDOM, ArrayNbr + 2)
    NewLen = Len(RDOM)
    
    If OldLen <> NewLen Then
        MsgBox "Error", vbCritical
        Err.Raise 1
    End If
    
End Property

Public Property Get RDOM_DoodadCount() As Integer

    RDOM_DoodadCount = Len(RDOM) / 2
    
End Property

Public Property Get TriangleInfo_Flags(ByVal id As Long) As Long
Dim myYPOM As String

    myYPOM = Mid(YPOM, 5)
    id = (id * 2) - 1
    TriangleInfo_Flags = Asc(Mid(myYPOM, id, 1))
    
End Property

Public Property Let TriangleInfo_Flags(ByVal id As Long, ByVal vNewValue As Long)
Dim OldLen As Long, NewLen As Long

    id = (id * 2) - 1 + 4       ' +4 for the header
    
    OldLen = Len(YPOM)
    YPOM = Mid(YPOM, 1, id - 1) & Chr(vNewValue) & Mid(YPOM, id + 1)
    NewLen = Len(YPOM)
    
    If OldLen <> NewLen Then
        MsgBox "Error", vbCritical
        Err.Raise 1
    End If
    
End Property

Public Property Get TriangleInfo_MaterialID(ByVal id As Long) As Long
Dim OldLen As Long, NewLen As Long
Dim myYPOM As String

    myYPOM = Mid(YPOM, 5)
    id = (id * 2)
    TriangleInfo_MaterialID = CLng(Asc(Mid(myYPOM, id, 1)))
    
End Property

Public Property Let TriangleInfo_MaterialID(ByVal id As Long, ByVal vNewValue As Long)
Dim OldLen As Long, NewLen As Long

    id = (id * 2)
    
    OldLen = Len(YPOM)
    YPOM = Mid(YPOM, 1, id - 1) & Chr(vNewValue) & Mid(YPOM, id + 1)
    NewLen = Len(YPOM)
    
    If OldLen <> NewLen Then
        MsgBox "Error", vbCritical
        Err.Raise 1
    End If
    
End Property

Property Get TriangleInfo_Count() As Integer
    TriangleInfo_Count = (Len(YPOM) - 4) / 2
End Property

Public Property Get Normals_X(ByVal id As Long) As Single
Dim myTVOM As String, s As String
    
    myTVOM = Mid(RNOM, 5)
    id = (id - 1) * 12
    id = id + 1
    
    s = Mid(myTVOM, id, 4)
    Normals_X = String32ToFloat(s)
    
End Property

Public Property Get Normals_Z(ByVal id As Long) As Single
Dim myTVOM As String, s As String
    
    myTVOM = Mid(RNOM, 5)
    id = (id - 1) * 12
    id = id + 1 + 4
    
    s = Mid(myTVOM, id, 4)
    Normals_Z = String32ToFloat(s)
    
End Property

Public Property Get Normals_Y(ByVal id As Long) As Single
Dim myTVOM As String, s As String
    
    myTVOM = Mid(RNOM, 5)
    id = (id - 1) * 12
    id = id + 1 + 4 + 4
    
    s = Mid(myTVOM, id, 4)
    Normals_Y = -String32ToFloat(s)
    
End Property

Public Property Get Vertices_X(ByVal id As Long) As Single
Dim myTVOM As String, s As String
    
    myTVOM = Mid(TVOM, 5)
    id = (id - 1) * 12
    id = id + 1
    
    s = Mid(myTVOM, id, 4)
    Vertices_X = String32ToFloat(s)
    
End Property

Public Property Let Vertices_X(ByVal id As Long, ByVal vNewValue As Single)
Dim myTVOM As String, s As String
    
    id = (id - 1) * 12
    id = id + 5
    
    
    TVOM = Mid(TVOM, 1, id - 1) & FloatToString32(vNewValue) & Mid(TVOM, id + 4)
        
End Property

Public Property Get Vertices_Y(ByVal id As Long) As Single
Dim myTVOM As String, s As String
    
    myTVOM = Mid(TVOM, 5)
    id = (id - 1) * 12
    id = id + 1 + 4
    
    s = Mid(myTVOM, id, 4)
    Vertices_Y = String32ToFloat(s)
    
End Property

Public Property Let Vertices_Y(ByVal id As Long, ByVal vNewValue As Single)
Dim myTVOM As String, s As String
    
    id = (id - 1) * 12
    id = id + 5 + 4
            
    TVOM = Mid(TVOM, 1, id - 1) & FloatToString32(vNewValue) & Mid(TVOM, id + 4)
        
End Property

Public Property Get Vertices_Z(ByVal id As Long) As Single
Dim myTVOM As String, s As String
    
    myTVOM = Mid(TVOM, 5)
    id = (id - 1) * 12
    id = id + 1 + 4 + 4
    
    s = Mid(myTVOM, id, 4)
    Vertices_Z = String32ToFloat(s)
    Vertices_Z = -Vertices_Z
    
End Property

Public Property Let Vertices_Z(ByVal id As Long, ByVal vNewValue As Single)
Dim myTVOM As String, s As String
    
    vNewValue = -vNewValue
    
    id = (id - 1) * 12
    id = id + 5 + 4 + 4
            
    TVOM = Mid(TVOM, 1, id - 1) & FloatToString32(vNewValue) & Mid(TVOM, id + 4)
        
End Property

Public Property Get Vertices_Count() As Long
Dim myTVOM As String, i As Long
    
    myTVOM = Mid(TVOM, 5)
    i = Len(myTVOM) / 12
    Vertices_Count = i
    
End Property

Public Property Get IVOM_1(ByVal id As Integer) As Long
Dim myIVOM As String, s As String
    
    myIVOM = Mid(IVOM, 5)
    id = (id - 1) * 2
    id = id + 1
    
    s = Mid(myIVOM, id, 2)
    IVOM_1 = String32ToLong(s) + 1
    
End Property

Public Property Get RBOM_IndexArray(ByVal id As Long) As Integer
Dim i As Long, s As String

    s = Mid(RBOM, 5)
    s = Mid(s, (id * 2) + 1, 2)
    
    i = String32ToLong(s & Chr(0) & Chr(0))
    
    RBOM_IndexArray = i
    
End Property

Public Property Get RBOM_IndexCount() As Integer
Dim s As String

    s = Mid(RBOM, 5)
    
    RBOM_IndexCount = Len(s) / 2
    
End Property

Public Property Let IVOM_1(ByVal id As Integer, ByVal vNewValue As Long)
Dim StartPos As Double, i As Double, myFDOM As String
Dim b4 As String, later As String
    
    StartPos = id - 1
    StartPos = StartPos * 12
    StartPos = StartPos + 4
    IVOM = Mid(IVOM, 1, StartPos) & LongToString32(vNewValue) & Mid(IVOM, StartPos + 5)
    
    
End Property

Public Property Get IVOM_2(ByVal id As Integer) As Long
Dim myIVOM As String, s As String
    
    myIVOM = Mid(IVOM, 5)
    id = (id - 1) * 2
    id = id + 1 + 2
    
    s = Mid(myIVOM, id, 2)
    IVOM_2 = String32ToLong(s) + 1
    
End Property

Public Property Let IVOM_2(ByVal id As Integer, ByVal vNewValue As Long)
Dim StartPos As Double, i As Double, myFDOM As String
Dim b4 As String, later As String
    
    StartPos = id - 1
    StartPos = StartPos * 12
    StartPos = StartPos + 4
    StartPos = StartPos + 4
    IVOM = Mid(IVOM, 1, StartPos) & LongToString32(vNewValue) & Mid(IVOM, StartPos + 5)
    
End Property

Public Property Get IVOM_3(ByVal id As Integer) As Long
Dim myIVOM As String, s As String
    
    myIVOM = Mid(IVOM, 5)
    id = (id - 1) * 2
    id = id + 1 + 2 + 2
    
    s = Mid(myIVOM, id, 2)
    IVOM_3 = String32ToLong(s) + 1
    
End Property

Public Property Let IVOM_3(ByVal id As Integer, ByVal vNewValue As Long)
Dim StartPos As Double, i As Double, myFDOM As String
Dim b4 As String, later As String
    
    StartPos = id - 1
    StartPos = StartPos * 12
    StartPos = StartPos + 4
    StartPos = StartPos + 4
    StartPos = StartPos + 4

    IVOM = Mid(IVOM, 1, StartPos) & LongToString32(vNewValue) & Mid(IVOM, StartPos + 5)
       
End Property

Public Property Get WaterLocX() As Single
Dim sng As Single, s As String

    s = Mid(QILM, 21)
    s = Mid(s, 1, 4)
    
    sng = String32ToFloat(s)
    WaterLocX = sng
    
End Property

Public Property Let WaterLocX(vNewVal As Single)
Dim sng As Single, s As String

    s = FloatToString32(vNewVal)
    
    QILM = Mid(QILM, 1, 20) & s & Mid(QILM, 25)
    
End Property

Public Property Get WaterLocY() As Single
Dim sng As Single, s As String

    s = Mid(QILM, 25)
    s = Mid(s, 1, 4)
    
    sng = String32ToFloat(s)
    WaterLocY = sng
    
End Property

Public Property Let WaterLocY(vNewVal As Single)
Dim sng As Single, s As String

    s = FloatToString32(vNewVal)
    
    QILM = Mid(QILM, 1, 24) & s & Mid(QILM, 29)
    
End Property

Public Property Get WaterLocZ() As Single
Dim sng As Single, s As String

    s = Mid(QILM, 29)
    s = Mid(s, 1, 4)
    
    sng = String32ToFloat(s)
    WaterLocZ = sng
    
End Property

Public Property Let WaterLocZ(vNewVal As Single)
Dim sng As Single, s As String

    s = FloatToString32(vNewVal)
    
    QILM = Mid(QILM, 1, 28) & s & Mid(QILM, 33)
    
End Property

Sub FillWMOWithWater()
Dim LowX As Single, LowY As Single, X As Integer, Y As Integer
    
    QILM = ReadFile("WaterWMO.dat")
    
    If QILM = "" Then
        MsgBox "Error: Could not read WaterWMO.dat", vbCritical
        Exit Sub
    End If
    
    LowX = 7877
    For X = 1 To Vertices_Count
        If LowX > Vertices_X(X) Then LowX = Vertices_X(X)
        DoEvents
    Next
    
    WaterLocX = LowX
    
    LowY = 7877
    For Y = 1 To Vertices_Count
        If LowY > Vertices_Y(Y) Then LowY = Vertices_Y(Y)
        DoEvents
    Next
    
    WaterLocY = LowY + 30

End Sub
