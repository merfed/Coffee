Attribute VB_Name = "GlobalMod"
Option Explicit

Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (pDst As Any, pSrc As Any, ByVal ByteLen As Long)

Global DummyMPQPath As String
Global DBugWndoShown As Boolean
Global AreaIdTable() As String      ' // Only meant for loading the .csv file and for reading.

Function Bin2Hex(ByVal BinData As String) As String
Dim x As Integer, sOut As String, H As String

    For x = 1 To Len(BinData)
        H = Hex(Asc(Mid(BinData, x, 1)))
        H = Right("0" & Hex(Asc(Mid(BinData, x, 1))), 2)
        sOut = sOut & H
    Next
    
    Bin2Hex = sOut

End Function

Function LastSplit(ByVal InString As String, ByVal Delim As String) As String
Dim a() As String
    
    a = Split(InString, Delim)
    LastSplit = a(UBound(a))
    
End Function

Function FloatParse(BinData As String) As Double
Dim Chars(3) As Integer
Dim sBin As String
Dim s As Integer, E As Integer, F As Long, V As Double


    'Chars(0) = Asc(Mid(BinData, 1, 1))
    'Chars(1) = Asc(Mid(BinData, 2, 1))
    'Chars(2) = Asc(Mid(BinData, 3, 1))
    'Chars(3) = Asc(Mid(BinData, 4, 1))
    
    MsgBox Bin2Dec("101")
    
    sBin = "01000000110100000000000000000000"
    
    'sBin = sBin & Dec2Bin(Char(0))
    'sBin = sBin & Dec2Bin(Char(1))
    'sBin = sBin & Dec2Bin(Char(2))
    'sBin = sBin & Dec2Bin(Char(3))

    If Mid(sBin, 1, 1) = "0" Then
        s = 1
    Else
        s = -1
    End If
    
    E = Bin2Dec(Mid(sBin, 2, 8))
    F = Bin2Dec(Mid(sBin, 10))
    MsgBox Mid(sBin, 10)
    
    V = s * 2 * (E - 127) * CDbl("1." & F)
    
    MsgBox V

End Function

Function LookupAreaName(AreaId As Integer) As String
Dim s As String, a() As String
On Error GoTo ErrHandeler
    
    If UBound(AreaIdTable) >= AreaId Then
    If AreaIdTable(AreaId) <> "" Then
        
        LookupAreaName = AreaIdTable(AreaId)
        Exit Function
        
    End If
    End If
    
    s = ReadFile("AreaTable.csv")
    
    If s = "" Then GoTo ErrHandeler
    
    s = Replace(s, vbCrLf, vbCrLf & "START")
    a = Split(s, vbCrLf)
    a = Filter(a, "START" & AreaId & ",")
    a = Split(a(0), ",")
    LookupAreaName = a(11)
    
    If UBound(AreaIdTable) < AreaId Then ReDim Preserve AreaIdTable(AreaId)
    AreaIdTable(AreaId) = a(11)
        
    Exit Function
    
ErrHandeler:
MsgBox "Error reading the AreaTable.csv file!", vbCritical

End Function

Sub LoadDummyMPQPah()
Dim s As String, a() As String, i As Integer, Loaded As String

    s = ReadFile("WoWMapEditor.ini")
    s = Replace(s, "", "")
    a = Split(s, vbCrLf)
    
    For i = 0 To UBound(a)
            
        If InStr(1, a(i), "DummyMPQPath: ", vbTextCompare) <> 0 Then
            DummyMPQPath = a(i)
            DummyMPQPath = Trim(Mid(DummyMPQPath, Len("DummyMPQPath: ")))
            Exit Sub
        End If
            
    Next
    
End Sub

Sub DebugPrint(DBugString As String)
On Error GoTo ClearLog

    If DBugWndoShown Then
        GUIDebug.Text1 = Right(GUIDebug.Text1 & DBugString & vbCrLf, 30000)
        GUIDebug.Text1.SelStart = Len(GUIDebug.Text1)
    End If
    
    Debug.Print DBugString
    
Exit Sub
ClearLog:

    GUIDebug.Text1 = Right(GUIDebug.Text1, 3000)
    On Error Resume Next
    GUIDebug.Text1 = GUIDebug.Text1 & DBugString & vbCrLf
    GUIDebug.Text1.SelStart = Len(GUIDebug.Text1)
    Debug.Print DBugString
    
End Sub

Sub SaveDummyMPQPath(Path As String)
    
    Load GUIStart
    GUIStart.LoadPrevMaps
    DummyMPQPath = Path
    GUIStart.SavePrevMaps
    Unload GUIStart
    
End Sub

Function Exist(DocLocation As String) As Boolean
Dim Doc As String
On Local Error GoTo ErrHandeler
    Doc = Right(DocLocation, Len(DocLocation) - InStrRev(DocLocation, "\"))
    Exist = CBool(UCase(Dir(DocLocation)) = UCase(Doc))
    Exit Function
ErrHandeler:
    Exist = False
    Exit Function
End Function

Sub Main()
Dim s As String
Dim a() As String, i As Integer
    
    ReDim AreaIdTable(0)
    
    Load GUIAbout
    GUIAbout.ShowText_DontCheatNoob
    GUIAbout.Show vbModal
    
    Load GUIStart
    
    If Command <> "" Then
        GUIStart.TargFile = Replace(Command, """", "")
        GUIStart.PreformOpen
    Else
        GUIStart.Show
    End If
    
End Sub

Function String32ToLong(s As String, Optional Flip As Boolean = False) As Double
Dim InptStr As String
Dim C As Long

    s = s & Chr(0) & Chr(0) & Chr(0) & Chr(0)
    s = Strings.Left(s, 4)
    
    InptStr = s
    CopyMemory C, ByVal InptStr, 4
    String32ToLong = C

    Exit Function

End Function

Function LongToString32(ByVal D As Single, Optional ByVal Flip As Boolean = False) As String
Dim b(3) As Byte
Dim C As Long
Dim s As String

    C = D
    CopyMemory b(0), C, 4
    s = Chr(b(0)) & Chr(b(1)) & Chr(b(2)) & Chr(b(3))
    LongToString32 = s
    
End Function

Function Bin2Dec(ByVal Num As String)
Dim n As Integer, a As Integer, x As String
     n = Len(Num) - 1
     a = n
     Do While n > -1
        x = Mid(Num, ((a + 1) - n), 1)
        Bin2Dec = IIf((x = "1"), Bin2Dec + (2 ^ (n)), Bin2Dec)
        n = n - 1
     Loop
End Function

Function FloatToString32(ByVal InSngl As Single) As String
Dim b(3) As Byte
Dim s As Single

    s = InSngl
    CopyMemory b(0), s, 4
    FloatToString32 = Chr(b(0)) & Chr(b(1)) & Chr(b(2)) & Chr(b(3))
    
End Function

Function String32ToFloat(ByVal InBinStr As String) As Single
Dim c1 As Integer, c2 As Integer, c3 As Integer, c4 As Integer
Dim b(3) As Byte
Dim s As String
Dim Sngl As Single

    's = InBinStr & Chr(0) & Chr(0) & Chr(0) & Chr(0)
    's = Strings.Left(s, 4)
    
    s = Mid(InBinStr, 1, 4)
    CopyMemory Sngl, ByVal s, 4
    'CopyMemory b(0), ByVal s, 4
    
    String32ToFloat = StripVBFloatFaggotry(Sngl)
    
End Function

Function StripVBFloatFaggotry(InputBullshit As Single) As Single
    
    ' VB is a bitch when it somes to floats because it inputs in crap like "-e"+ some number or "#Inf" into the number....
    ' Oh... and btw,, when it does that. IT CANT FUCKING ADD!
    ' Yeah, that's right.  Any addition, subtraction, multiplications and so on cant be done to numbers where it has that crap in it
    ' Makes no fucking sence, but neither does half the other Mirocrap products
    
    Dim a As String, Sng1 As Single
    
    ' // Convert to string
    a = InputBullshit
    
    ' // Remove the ending crap that sometimes exists ("-e" + some number) as it gets in the way
    a = Replace(a, "e", "", , , vbTextCompare)
    
    ' // Remove "#Inf"
    a = Replace(a, "#Inf", "", , , vbTextCompare)
    
    ' // Remove "+"
    a = Replace(a, "+", "", , , vbTextCompare)
    
    ' // Remove any minus sign left, but not a negative sign
    a = Mid(a, 1, 1) & Replace(Mid(a, 2), "-", "")
    
    ' // Convert back to single (float)
    Sng1 = a
    
    StripVBFloatFaggotry = Sng1

End Function

Function AddSingles(ByVal Sng1 As Single, ByVal Sng2 As Single) As String
    
    ' // Fucking faggotry fucking retarded Microsoft!!!  The fucking retared idoits cant make a compiler that adds floats coreclty!?!?  What the flying fuck!
    ' // Now I have to jump through hoops just to add two decimal numbers!  WHAT THE FUCK!
    
    Dim a As String, b As String, C As Single, Rslt As Single
    
    ' // Convert to string
    a = Sng1
    b = Sng2
    
    ' // Remove the ending crap that sometimes exists ("-e" + some number) as it gets in the way
    a = Replace(a, "e", "", , , vbTextCompare)
    b = Replace(b, "e", "", , , vbTextCompare)
    
    ' // Remove any minus sign left, but not a negative sign
    a = Mid(a, 1, 1) & Replace(Mid(a, 2), "-", "")
    b = Mid(b, 1, 1) & Replace(Mid(b, 2), "-", "")
    
    ' // Convert back to single (float)
    Sng1 = a
    Sng2 = b
    
    ' // Add
    C = Sng1 - Sng2
    
    AddSingles = C
    
End Function

Private Function Dec2Bin(ByVal x As Long) As String
Dim Y As Long
Dim Num As String

While (x \ 2) > 0
    Y = x \ 2
    If x > 1 Then
    Num = Val(x - (Y * 2)) & Num
    End If
    x = Y
Wend
Num = x & Num
Dec2Bin = Num
End Function

Function ByteStringToString(ByVal ByteString As String) As String
Dim i As Integer, a() As String, s As String

    a = Split(ByteString, "-")

    For i = 0 To UBound(a)
        If a(i) <> "" Then a(i) = Chr(a(i))
    Next
    
    s = Join(a, "")
    ByteStringToString = s
    
End Function

Function StringToByteString(InString As String) As String
Dim i As Long, a() As String, s As String
    
    If InString = "" Then
        Load GUIDebug
        DebugPrint "Error: The sub StringToByteString(str) was called with a blank paramiter"
        Exit Function
    End If
    
    ReDim a(Len(InString) - 1)
    
    For i = 1 To Len(InString)
        a(i - 1) = Asc(Mid(InString, i, 1))
    Next
    
    StringToByteString = Join(a, "-")
    
End Function

Function ReadFile(ByVal FilePath As String) As Byte()
Dim b() As Byte, s As String, x As Double, a() As String

    If InStr(1, FilePath, ":\") = 0 Then
        
        FilePath = Replace(App.Path & "\", "\\", "\") & FilePath
        
    End If

    Open FilePath For Binary Access Read As 1
    ReDim b(LOF(1))
    Get 1, , b()
    Close 1
    
    ReDim a(UBound(b))
    For x = 0 To UBound(b)
        a(x) = Chr(CInt(b(x)))
    Next
        
    s = Join(a, "")
    
    ReadFile = s

End Function

Function RepeateString(NumberOfTimes As Integer, RepString As String) As String
Dim i As Integer, s As String
    
    For i = 1 To NumberOfTimes
        
        s = s & RepString
        
    Next
    
    RepeateString = s
    
End Function

Sub WriteFile(FilePath As String, DataInFile As String)
Dim SFSO
On Error Resume Next
    
    If InStr(1, FilePath, ":\") = 0 Then
        
        FilePath = Replace(App.Path & "\", "\\", "\") & FilePath
        
    End If

    Set SFSO = CreateObject("Scripting.FileSystemObject")
    Set SFSO = SFSO.CreateTextFile(FilePath)
    
    SFSO.Write DataInFile
    SFSO.Close
    
    Set SFSO = Nothing

End Sub

Sub Kill2(FilePath As String)
On Error Resume Next
    Kill FilePath  ' // the point of this is the error handeler above this
End Sub
