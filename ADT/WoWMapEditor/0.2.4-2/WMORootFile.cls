VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "WMORootFile"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Dim REVM_Headder As String
Dim MOHD As String
Dim MOTX As String
Dim MOMT As String
Dim MOGN As String
Dim MOGI As String
Dim MOSB As String
Dim MOPV As String
Dim MOPT As String
Dim MOPR As String
Dim MOVV As String
Dim MOVB As String
Dim MOLT As String
Dim MODS As String
Dim MODN As String
Dim MODD As String
Dim MFOG As String
Dim MCVP As String


Public Property Get Materials_Path(ByVal id As Long) As String
Dim myXTOM As String, a() As String

    myXTOM = Mid(MOTX, 5)
    
    Do While InStr(1, myXTOM, Chr(0) & Chr(0)) <> 0
        myXTOM = Replace(myXTOM, Chr(0) & Chr(0), Chr(0))
    Loop
    
    a = Split(myXTOM, Chr(0))
    
    If id > UBound(a) Then
        Materials_Path = "<<ERROR>>"
    Else
        Materials_Path = a(id)
    End If

End Property

Public Property Get Materials_Count() As Long
Dim s As String

    s = Mid(MOHD, 5, 4)
    Materials_Count = String32ToLong(s)
    
End Property

Public Property Get Materials_Flag(ByVal id As Integer) As Long
Dim s As String

    id = 64 * (id - 1)
    id = id + 5
    s = Mid(MOMT, id, 4)
    Materials_Flag = String32ToLong(s)
    
End Property

Public Property Get Materials_BlendMode(ByVal id As Integer) As Long
Dim s As String

    id = 64 * (id - 1)
    id = id + 5 + 4
    s = Mid(MOMT, id, 4)
    Materials_BlendMode = String32ToLong(s)
    
End Property

Public Property Get DoodadCount() As Integer
    
    DoodadCount = (Len(MODD) - 4) / 40
    
End Property

Public Property Get DoodadPath(ByVal id As Integer) As String
Dim MyMODN As String, MyMODD As String, Offset As String, intOffset As Long, s As String, Nbr As Long

    MyMODN = Mid(MODN, 5)
    MyMODD = Mid(MODD, 5)
    
    Nbr = id - 1
    Nbr = Nbr * 40
    Nbr = Nbr + 1
    Offset = Mid(MyMODD, Nbr, 4)
    intOffset = String32ToLong(Offset)
    
    s = Mid(MyMODN, intOffset + 1)
    s = Strings.Left(s, InStr(2, s, Chr(0)))
    s = Replace(s, Chr(0), "")
    DoodadPath = s
    
End Property

Public Property Get DoodadX(ByVal Ndx As Integer) As Double
Dim StartPos As Double, s As String, i As Double, MyMODD  As String
    
    MyMODD = Mid(MODD, 5)
        
    StartPos = Ndx - 1
    StartPos = StartPos * 40
    StartPos = StartPos + 5
    
    s = Mid(MyMODD, StartPos, 4)
    DoodadX = String32ToFloat(s)

End Property

Public Property Get DoodadZ(ByVal Ndx As Integer) As Double
Dim StartPos As Double, s As String, i As Double, MyMODD  As String
    
    MyMODD = Mid(MODD, 5)
        
    StartPos = Ndx - 1
    StartPos = StartPos * 40
    StartPos = StartPos + 9
    
    s = Mid(MyMODD, StartPos, 4)
    DoodadZ = String32ToFloat(s)

End Property

Public Property Let DoodadZ(ByVal Ndx As Integer, vNewVal As Double)
Dim StartPos As Double, s As String, i As Double, MyMODD  As String
    
    StartPos = Ndx - 1
    StartPos = StartPos * 40
    StartPos = StartPos + 9 + 4
    
    MODD = Mid(MODD, 1, StartPos - 1) & FloatToString32(vNewVal) & Mid(MODD, StartPos + 4)

End Property

Public Property Get DoodadY(ByVal Ndx As Integer) As Double
Dim StartPos As Double, s As String, i As Double, MyMODD  As String
    
    MyMODD = Mid(MODD, 5)
        
    StartPos = Ndx - 1
    StartPos = StartPos * 40
    StartPos = StartPos + 13
    
    s = Mid(MyMODD, StartPos, 4)
    DoodadY = -(String32ToFloat(s))

End Property

Sub SaveWMORootFile(FilePath As String)
Dim s As String

    s = s & "REVM" & REVM_Headder
    s = s & "MOHD" & MOHD
    s = s & "MOTX" & MOTX
    s = s & "MOMT" & MOMT
    s = s & "MOGN" & MOGN
    s = s & "MOGI" & MOGI
    s = s & "MOSB" & MOSB
    s = s & "MOPV" & MOPV
    s = s & "MOPT" & MOPT
    s = s & "MOPR" & MOPR
    s = s & "MOVV" & MOVV
    s = s & "MOVB" & MOVB
    s = s & "MOLT" & MOLT
    s = s & "MODS" & MODS
    s = s & "MODN" & MODN
    s = s & "MODD" & MODD
    s = s & "MFOG" & MFOG
    s = s & "MCVP" & MCVP
    
    WriteFile FilePath, s
    
End Sub

Sub OpendWMORootFile(FilePath As String)
Dim s As String, a() As String, i As Integer

    s = ReadFile(FilePath)

    s = Replace(s, StrReverse("MOHD"), "_DELIM_MOHD")
    s = Replace(s, StrReverse("MOTX"), "_DELIM_MOTX")
    s = Replace(s, StrReverse("MOMT"), "_DELIM_MOMT")
    s = Replace(s, StrReverse("MOGN"), "_DELIM_MOGN")
    s = Replace(s, StrReverse("MOGI"), "_DELIM_MOGI")
    s = Replace(s, StrReverse("MOSB"), "_DELIM_MOSB")
    s = Replace(s, StrReverse("MOPV"), "_DELIM_MOPV")
    s = Replace(s, StrReverse("MOPT"), "_DELIM_MOPT")
    s = Replace(s, StrReverse("MOPR"), "_DELIM_MOPR")
    s = Replace(s, StrReverse("MOVV"), "_DELIM_MOVV")
    s = Replace(s, StrReverse("MOVB"), "_DELIM_MOVB")
    s = Replace(s, StrReverse("MOLT"), "_DELIM_MOLT")
    s = Replace(s, StrReverse("MODS"), "_DELIM_MODS")
    s = Replace(s, StrReverse("MODN"), "_DELIM_MODN")
    s = Replace(s, StrReverse("MODD"), "_DELIM_MODD")
    s = Replace(s, StrReverse("MFOG"), "_DELIM_MFOG")
    s = Replace(s, StrReverse("MCVP"), "_DELIM_MCVP")
        
    a = Split(s, "_DELIM_")


    For i = 0 To UBound(a)
        
        Select Case UCase(Mid(a(i), 1, 4))
        Case "REVM":        REVM_Headder = Mid(a(i), 5)
        Case "MOHD":        MOHD = Mid(a(i), 5)
        Case "MOTX":        MOTX = Mid(a(i), 5)
        Case "MOMT":        MOMT = Mid(a(i), 5)
        Case "MOGN":        MOGN = Mid(a(i), 5)
        Case "MOGI":        MOGI = Mid(a(i), 5)
        Case "MOSB":        MOSB = Mid(a(i), 5)
        Case "MOPV":        MOPV = Mid(a(i), 5)
        Case "MOPT":        MOPT = Mid(a(i), 5)
        Case "MOPR":        MOPR = Mid(a(i), 5)
        Case "MOVV":        MOVV = Mid(a(i), 5)
        Case "MOVB":        MOVB = Mid(a(i), 5)
        Case "MOLT":        MOLT = Mid(a(i), 5)
        Case "MODS":        MODS = Mid(a(i), 5)
        Case "MODN":        MODN = Mid(a(i), 5)
        Case "MODD":        MODD = Mid(a(i), 5)
        Case "MFOG":        MFOG = Mid(a(i), 5)
        Case "MCVP":        MCVP = Mid(a(i), 5)
        Case Else:          MsgBox "Error reading file, unknown chunk header: " & Mid(a(i), 1, 4)
        End Select
        
    Next
    
End Sub

Public Property Get GroupName() As String

    's = ReadFile("C:\Documents and Settings\Name\Desktop\Dummy MPQ\Ironforge\ironforge.wmo")
    'b = InStr(1, s, "NGOM")
    'c = InStr(1, s, "IGOM")
    's = Mid(s, b, c - b)
    'a = Split(s, Chr(0))
    'MsgBox UBound(a)
    'End
    
End Property
