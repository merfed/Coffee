Attribute VB_Name = "MpqStuff"
Option Explicit

Public Declare Function ShellExecute Lib _
    "Shell32.dll" Alias "ShellExecuteA" _
    (ByVal hWnd As Long, _
    ByVal lpOperation As String, _
    ByVal lpFile As String, _
    ByVal lpParameters As String, _
    ByVal lpDirectory As String, _
    ByVal nShowCmd As Long) As Long
Public Declare Sub SHChangeNotify Lib _
    "Shell32.dll" (ByVal wEventId As Long, _
    ByVal uFlags As Integer, _
    ByVal dwItem1 As Any, _
    ByVal dwItem2 As Any)
Public Declare Function SendMessageA Lib _
    "user32.dll" _
    (ByVal hWnd As Long, _
    ByVal Msg As Long, _
    ByVal Wp As Long, _
    Lp As Any) As Long
Declare Function GetLongPathName Lib "Kernel32" Alias "GetLongPathNameA" (ByVal lpszShortPath As String, ByVal lpszLongPath As String, ByVal cchBuffer As Long) As Long
Private Declare Sub CopyMemory Lib "Kernel32.dll" _
    Alias "RtlMoveMemory" ( _
    ByRef Destination As Any, _
    ByRef Source As Any, _
    ByVal Length As Long)

Public CD As OPENFILENAME, PathInput As BROWSEINFO
Public GlobalFileList() As String, FileList() As String, CX As Single, CY As Single, NewFile As Boolean, LocaleID As Long, ListFile As String, AddFolderName As String, ExtractPathNum As Long, CopyPathNum As Long
Public Const AppKey As String = "HKEY_CURRENT_USER\Software\ShadowFlare\WinMPQ\", SharedAppKey As String = "HKEY_LOCAL_MACHINE\Software\ShadowFlare\WinMPQ\"
Public Const MPQ_ERROR_INIT_FAILED As Long = &H85000001 'Unspecified error
Public Const MPQ_ERROR_NO_STAREDIT As Long = &H85000002 'Can't find StarEdit.exe
Public Const MPQ_ERROR_BAD_STAREDIT As Long = &H85000003 'Bad version of StarEdit.exe. Need SC/BW 1.07
Public Const MPQ_ERROR_STAREDIT_RUNNING As Long = &H85000004 'StarEdit.exe is running. Must be closed
Public Const SHCNE_ASSOCCHANGED As Long = &H8000000
Public Const SHCNF_IDLIST  As Long = &H0
Public Const WM_SETREDRAW As Long = &HB
Public Const WM_PAINT  As Long = &HF
Const gintMAX_SIZE% = 255
Function PathInputBox(lpFolderDialog As BROWSEINFO, pCaption As String, StartFolder As String) As String
lpFolderDialog.Title = pCaption
Dim Result As Long
Result = ShowFolder(lpFolderDialog)
If Result = 0 Then Exit Function
PathInputBox = GetPathFromID(Result)
End Function
Function GetLongPath(Path As String) As String
    Dim strBuf As String, StrLength As Long
    strBuf = Space$(gintMAX_SIZE)
    StrLength = GetLongPathName(Path, strBuf, gintMAX_SIZE)
    strBuf = Left(strBuf, StrLength)
    If strBuf <> "" Then
        GetLongPath = strBuf
    Else
        GetLongPath = Path
    End If
End Function
Sub AddAutoFile(Mpq As String, File As String, MpqPath As String)
Dim cType As Integer, bNum As Long, fExt As String
For bNum = 1 To Len(File)
    If InStr(bNum, File, ".") > 0 Then
        bNum = InStr(bNum, File, ".")
    Else
        Exit For
    End If
Next bNum
If bNum > 1 Then
    fExt = Mid(File, bNum - 1)
Else
    fExt = File
End If
If LCase(fExt) = ".bik" Then
    cType = CInt(GetReg(AppKey + "Compression\.bik", "-2"))
ElseIf LCase(fExt) = ".smk" Then
    cType = CInt(GetReg(AppKey + "Compression\.smk", "-2"))
ElseIf LCase(fExt) = ".wav" Then
    cType = CInt(GetReg(AppKey + "Compression\.wav", "0"))
Else
    cType = CInt(GetReg(AppKey + "Compression\" + fExt, "-1"))
End If
Select Case cType
Case -2
MpqEx.Mpq.AddFile Mpq, File, MpqPath, 0
Case -1
MpqEx.Mpq.AddFile Mpq, File, MpqPath, 1
Case 0, 1, 2
MpqEx.Mpq.AddWavFile Mpq, File, MpqPath, cType
Case Else
MpqEx.Mpq.AddFile Mpq, File, MpqPath, 1
End Select
End Sub
Sub AddScriptOutput(sOutput As String)
SendMessageA ScriptOut.oText.hWnd, WM_SETREDRAW, 0, ByVal 0&
ScriptOut.oText = ScriptOut.oText + sOutput
SendMessageA ScriptOut.oText.hWnd, WM_SETREDRAW, 1, ByVal 0&
ScriptOut.oText.SelStart = Len(ScriptOut.oText)
End Sub
Function GetFileTitle(FileName As String) As String
Dim bNum As Long
If InStr(FileName, "\") > 0 Then
    For bNum = 1 To Len(FileName)
        If InStr(bNum, FileName, "\") > 0 Then
            bNum = InStr(bNum, FileName, "\")
        Else
            Exit For
        End If
    Next bNum
End If
GetFileTitle = Mid(FileName, bNum)
End Function
Function ListFiles(MpqName As String, ByVal FileLists As String) As String
Dim NewFileLists As String, nFileLists() As String, ListName As String, cNum As Long, cNum2 As Long, cNum3 As Long, MpqList1 As String, MpqList2 As String, Path As String, ListLen As Long, OldLists() As String, UseOnlyAutoList As Boolean
If GetReg(AppKey + "AutofindFileLists", 0) = 0 Then
    ListFiles = MpqEx.Mpq.ListFiles(MpqName, FileLists)
Else
    UseOnlyAutoList = GetReg(AppKey + "UseOnlyAutofindLists", 1)
    MpqList2 = GetExtension(MpqName)
    MpqList1 = GetFileTitle(Left(MpqName, Len(MpqName) - Len(MpqList2))) + ".txt"
    MpqList2 = GetFileTitle(MpqName) + ".txt"
    Path = GetLongPath(App.Path)
    If Right(Path, 1) <> "\" Then Path = Path + "\"
    If UseOnlyAutoList Then ListLen = Len(FileLists)
    If FileLists <> "" Then
        FileLists = FileLists + vbCrLf + Path + App.EXEName + ".exe" + vbCrLf + MpqName
    Else
        FileLists = FileLists + vbCrLf + Path + App.EXEName + ".exe" + vbCrLf + MpqName
    End If
    ReDim nFileLists(0) As String
    If UseOnlyAutoList Then ReDim OldLists(0) As String
    For cNum = 1 To Len(FileLists)
        cNum2 = InStr(cNum, FileLists, vbCrLf)
        If cNum2 = 0 Then
            cNum2 = Len(FileLists) + 1
        End If
        ListName = Mid(FileLists, cNum, cNum2 - cNum)
        If UseOnlyAutoList Then
            ReDim Preserve OldLists(UBound(OldLists) + 1) As String
            OldLists(UBound(OldLists)) = GetLongPath(ListName)
        End If
        For cNum3 = 1 To Len(ListName)
            If InStr(cNum3, ListName, "\") Then
                cNum3 = InStr(cNum3, ListName, "\")
                If FileExists(Left(ListName, cNum3) + MpqList1) Then
                    ReDim Preserve nFileLists(UBound(nFileLists) + 1) As String
                    nFileLists(UBound(nFileLists)) = GetLongPath(Left(ListName, cNum3) + MpqList1)
                End If
                If FileExists(Left(ListName, cNum3) + MpqList2) Then
                    ReDim Preserve nFileLists(UBound(nFileLists) + 1) As String
                    nFileLists(UBound(nFileLists)) = GetLongPath(Left(ListName, cNum3) + MpqList2)
                End If
            Else
                Exit For
            End If
        Next cNum3
        If FileExists(ListName) And ListName <> Path + App.EXEName + ".exe" And ListName <> MpqName Then
            ReDim Preserve nFileLists(UBound(nFileLists) + 1) As String
            nFileLists(UBound(nFileLists)) = GetLongPath(ListName)
        End If
        cNum = cNum2 + 1
    Next cNum
    If UseOnlyAutoList Then
        For cNum = 1 To UBound(nFileLists)
            For cNum2 = 1 To UBound(OldLists)
                If LCase(nFileLists(cNum)) <> LCase(OldLists(cNum2)) Then
                    GoTo StartSearch
                End If
            Next cNum2
        Next cNum
        UseOnlyAutoList = False
    End If
StartSearch:
    For cNum = 1 To UBound(nFileLists)
        For cNum2 = 1 To UBound(nFileLists)
            If LCase(nFileLists(cNum)) = LCase(nFileLists(cNum2)) And cNum <> cNum2 Then
                nFileLists(cNum2) = ""
            End If
        Next cNum2
        If UseOnlyAutoList Then
            For cNum2 = 1 To UBound(OldLists)
                If LCase(nFileLists(cNum)) = LCase(OldLists(cNum2)) Then
                    nFileLists(cNum) = ""
                End If
            Next cNum2
        End If
        If nFileLists(cNum) <> "" Then
            NewFileLists = NewFileLists + nFileLists(cNum) + vbCrLf
        End If
    Next cNum
    If Right(NewFileLists, 2) = vbCrLf Then NewFileLists = Left(NewFileLists, Len(NewFileLists) - 2)
    ListFiles = MpqEx.Mpq.ListFiles(MpqName, NewFileLists)
End If
End Function
Function sListFiles(MpqName As String, hMPQ As Long, ByVal FileLists As String) As String
Dim NewFileLists As String, nFileLists() As String, ListName As String, cNum As Long, cNum2 As Long, cNum3 As Long, MpqList1 As String, MpqList2 As String, Path As String, ListLen As Long, OldLists() As String, UseOnlyAutoList As Boolean
If GetReg(AppKey + "AutofindFileLists", 0) = 0 Then
    sListFiles = MpqEx.Mpq.sListFiles(hMPQ, FileLists)
Else
    UseOnlyAutoList = GetReg(AppKey + "UseOnlyAutofindLists", 1)
    MpqList2 = GetExtension(MpqName)
    MpqList1 = GetFileTitle(Left(MpqName, Len(MpqName) - Len(MpqList2))) + ".txt"
    MpqList2 = GetFileTitle(MpqName) + ".txt"
    Path = GetLongPath(App.Path)
    If Right(Path, 1) <> "\" Then Path = Path + "\"
    If UseOnlyAutoList Then ListLen = Len(FileLists)
    If FileLists <> "" Then
        FileLists = FileLists + vbCrLf + Path + App.EXEName + ".exe" + vbCrLf + MpqName
    Else
        FileLists = FileLists + vbCrLf + Path + App.EXEName + ".exe" + vbCrLf + MpqName
    End If
    ReDim nFileLists(0) As String
    If UseOnlyAutoList Then ReDim OldLists(0) As String
    For cNum = 1 To Len(FileLists)
        cNum2 = InStr(cNum, FileLists, vbCrLf)
        If cNum2 = 0 Then
            cNum2 = Len(FileLists) + 1
        End If
        ListName = Mid(FileLists, cNum, cNum2 - cNum)
        If UseOnlyAutoList And cNum < ListLen Then
            ReDim Preserve OldLists(UBound(OldLists) + 1) As String
            OldLists(UBound(OldLists)) = GetLongPath(ListName)
        End If
        For cNum3 = 1 To Len(ListName)
            If InStr(cNum3, ListName, "\") Then
                cNum3 = InStr(cNum3, ListName, "\")
                If FileExists(Left(ListName, cNum3) + MpqList1) Then
                    ReDim Preserve nFileLists(UBound(nFileLists) + 1) As String
                    nFileLists(UBound(nFileLists)) = GetLongPath(Left(ListName, cNum3) + MpqList1)
                End If
                If FileExists(Left(ListName, cNum3) + MpqList2) Then
                    ReDim Preserve nFileLists(UBound(nFileLists) + 1) As String
                    nFileLists(UBound(nFileLists)) = GetLongPath(Left(ListName, cNum3) + MpqList2)
                End If
            Else
                Exit For
            End If
        Next cNum3
        If FileExists(ListName) And ListName <> Path + App.EXEName + ".exe" And ListName <> MpqName Then
            ReDim Preserve nFileLists(UBound(nFileLists) + 1) As String
            nFileLists(UBound(nFileLists)) = GetLongPath(ListName)
        End If
        cNum = cNum2 + 1
    Next cNum
    If UseOnlyAutoList Then
        For cNum = 1 To UBound(nFileLists)
            For cNum2 = 1 To UBound(OldLists)
                If LCase(nFileLists(cNum)) <> LCase(OldLists(cNum2)) Then
                    GoTo StartSearch
                End If
            Next cNum2
        Next cNum
        UseOnlyAutoList = False
    End If
StartSearch:
    For cNum = 1 To UBound(nFileLists)
        For cNum2 = 1 To UBound(nFileLists)
            If LCase(nFileLists(cNum)) = LCase(nFileLists(cNum2)) And cNum <> cNum2 Then
                nFileLists(cNum2) = ""
            End If
        Next cNum2
        If UseOnlyAutoList Then
            For cNum2 = 1 To UBound(OldLists)
                If LCase(nFileLists(cNum)) = LCase(OldLists(cNum2)) Then
                    nFileLists(cNum) = ""
                    Exit For
                End If
            Next cNum2
        End If
        If nFileLists(cNum) <> "" Then
            NewFileLists = NewFileLists + nFileLists(cNum) + vbCrLf
        End If
    Next cNum
    If Right(NewFileLists, 2) = vbCrLf Then NewFileLists = Left(NewFileLists, Len(NewFileLists) - 2)
    sListFiles = MpqEx.Mpq.sListFiles(hMPQ, NewFileLists)
End If
End Function
Sub mAddAutoFile(hMPQ As Long, File As String, MpqPath As String)
Dim cType As Integer, bNum As Long, fExt As String
For bNum = 1 To Len(File)
    If InStr(bNum, File, ".") > 0 Then
        bNum = InStr(bNum, File, ".")
    Else
        Exit For
    End If
Next bNum
If bNum > 1 Then
    fExt = Mid(File, bNum - 1)
Else
    fExt = File
End If
If LCase(fExt) = ".bik" Then
    cType = CInt(GetReg(AppKey + "Compression\.bik", "-2"))
ElseIf LCase(fExt) = ".smk" Then
    cType = CInt(GetReg(AppKey + "Compression\.smk", "-2"))
ElseIf LCase(fExt) = ".wav" Then
    cType = CInt(GetReg(AppKey + "Compression\.wav", "0"))
Else
    cType = CInt(GetReg(AppKey + "Compression\" + fExt, "-1"))
End If
Select Case cType
Case -2
MpqEx.Mpq.mAddFile hMPQ, File, MpqPath, 0
Case -1
MpqEx.Mpq.mAddFile hMPQ, File, MpqPath, 1
Case 0, 1, 2
MpqEx.Mpq.mAddWavFile hMPQ, File, MpqPath, cType
Case Else
MpqEx.Mpq.mAddFile hMPQ, File, MpqPath, 1
End Select
End Sub
Function DirEx(ByVal Path As String, Filter As String, Attributes, Recurse As Boolean) As String
Dim Files() As String, lNum As Long, Folders() As String
If Right(Path, 1) <> "\" And Path <> "" Then Path = Path + "\"
ReDim Files(0) As String
Files(0) = Dir(Path + Filter, Attributes)
If Files(0) <> "" Then
    Do
    ReDim Preserve Files(UBound(Files) + 1) As String
    Files(UBound(Files)) = Dir
    Loop Until Files(UBound(Files)) = ""
    ReDim Preserve Files(UBound(Files) - 1) As String
End If
For lNum = 0 To UBound(Files)
    If Files(lNum) <> "" Then
        If IsDir(Path + Files(lNum)) = False And (Attributes And vbDirectory) <> vbDirectory Then
            DirEx = DirEx + Path + Files(lNum) + vbCrLf
        ElseIf IsDir(Path + Files(lNum)) = True And (Attributes And vbDirectory) Then
            DirEx = DirEx + Path + Files(lNum) + vbCrLf
        End If
    End If
Next lNum
If Recurse = True And (InStr(Filter, "?") > 0 Or InStr(Filter, "*") > 0) Then
    ReDim Folders(0) As String
    Folders(0) = Dir(Path, vbDirectory)
    If Folders(0) = "." Or Folders(0) = ".." Then Folders(0) = Dir
    If Folders(0) = "." Or Folders(0) = ".." Then Folders(0) = Dir
    If Folders(0) <> "" Then
        Do
        ReDim Preserve Folders(UBound(Folders) + 1) As String
        Folders(UBound(Folders)) = Dir
        If Folders(UBound(Folders)) = "." Or Folders(UBound(Folders)) = ".." Then
            ReDim Preserve Folders(UBound(Folders) - 1) As String
        End If
        Loop Until Folders(UBound(Folders)) = ""
        ReDim Preserve Folders(UBound(Folders) - 1) As String
    End If
    For lNum = 0 To UBound(Folders)
        If Folders(lNum) <> "" Then
            If IsDir(Path + Folders(lNum)) Then
                DirEx = DirEx + DirEx(Path + Folders(lNum), Filter, Attributes, Recurse)
            End If
        End If
    Next lNum
End If
End Function
Function GetExtension(FileName As String) As String
Dim bNum As Long
If InStr(FileName, ".") > 0 Then
    For bNum = 1 To Len(FileName)
        If InStr(bNum, FileName, ".") > 0 Then
            bNum = InStr(bNum, FileName, ".")
        Else
            Exit For
        End If
    Next bNum
    GetExtension = Mid(FileName, bNum - 1)
Else
    GetExtension = ""
End If
End Function
Function IsDir(DirPath As String) As Boolean
On Error GoTo IsNotDir
If GetAttr(DirPath) And vbDirectory Then
    IsDir = True
Else
    IsDir = False
End If
Exit Function
IsNotDir:
IsDir = False
End Function
Function FileExists(FileName As String) As Boolean
On Error GoTo NoFile
If (GetAttr(FileName) And vbDirectory) <> vbDirectory Then
    FileExists = True
Else
    FileExists = False
End If
Exit Function
NoFile:
FileExists = False
End Function
Function IsMPQ(MpqFile As String) As Boolean
Dim fNum As Long, Text As String, bNum As Long, MpqHead As Long
If FileExists(MpqFile) = False Then
    IsMPQ = False
    Exit Function
End If
fNum = FreeFile
Open MpqFile For Binary As #fNum
For bNum = 1 To LOF(fNum) Step 2 ^ 20
    Text = String(2 ^ 20 + 32, Chr(0))
    If LOF(fNum) - bNum + 1 >= 2 ^ 20 + 32 Then
        Get #fNum, bNum, Text
    Else
        Text = String(LOF(fNum) - bNum + 1, Chr(0))
        Get #fNum, bNum, Text
    End If
    MpqHead = InStr(Text, "MPQ" + Chr(26))
CheckAgain:
    If MpqHead > 0 Then
        If JBytes(Text, MpqHead + 4, 4) >= 32 And JBytes(Text, MpqHead + 12, 2) = 0 Then
            Exit For
        Else
            MpqHead = InStr(MpqHead + 4, Text, "MPQ" + Chr(26))
            GoTo CheckAgain
        End If
    End If
Next bNum
Close #fNum
IsMPQ = True
If MpqHead = 0 Then IsMPQ = False
End Function
Sub KillEx(ByVal Path As String, Filter As String, Attributes, Recurse As Boolean)
Dim Files() As String, lNum As Long, Folders() As String
If Right(Path, 1) <> "\" And Path <> "" Then Path = Path + "\"
ReDim Files(0) As String
Files(0) = Dir(Path + Filter, Attributes)
If Files(0) <> "" Then
    Do
    ReDim Preserve Files(UBound(Files) + 1) As String
    Files(UBound(Files)) = Dir
    Loop Until Files(UBound(Files)) = ""
    ReDim Preserve Files(UBound(Files) - 1) As String
End If
For lNum = 0 To UBound(Files)
    If Files(lNum) <> "" Then
        If IsDir(Path + Files(lNum)) = False Then
            On Error Resume Next
            Kill Path + Files(lNum)
            On Error GoTo 0
        End If
    End If
Next lNum
If Recurse = True And (InStr(Filter, "?") > 0 Or InStr(Filter, "*") > 0) Then
    ReDim Folders(0) As String
    Folders(0) = Dir(Path, vbDirectory)
    If Folders(0) = "." Or Folders(0) = ".." Then Folders(0) = Dir
    If Folders(0) = "." Or Folders(0) = ".." Then Folders(0) = Dir
    If Folders(0) <> "" Then
        Do
        ReDim Preserve Folders(UBound(Folders) + 1) As String
        Folders(UBound(Folders)) = Dir
        If Folders(UBound(Folders)) = "." Or Folders(UBound(Folders)) = ".." Then
            ReDim Preserve Folders(UBound(Folders) - 1) As String
        End If
        Loop Until Folders(UBound(Folders)) = ""
        ReDim Preserve Folders(UBound(Folders) - 1) As String
    End If
    For lNum = 0 To UBound(Folders)
        If Folders(lNum) <> "" Then
            If IsDir(Path + Folders(lNum)) Then
                KillEx Path + Folders(lNum), Filter, Attributes, Recurse
                On Error Resume Next
                RmDir Path + Folders(lNum)
            End If
            On Error GoTo 0
        End If
    Next lNum
End If
End Sub
Function FullPath(ByVal BasePath As String, File As String) As String
If Right(BasePath, 1) <> "\" Then BasePath = BasePath + "\"
If Mid(File, 2, 1) = ":" Or Left(File, 2) = "\\" Then
    FullPath = File
ElseIf Left(File, 1) = "\" Then
    FullPath = Left(BasePath, 2) + File
Else
    FullPath = BasePath + File
End If
End Function
Function MatchesFilter(FileName As String, ByVal Filters As String) As Boolean
Dim bNum As Long, Filter As String
If InStr(Filters, ";") Then
    If Right(Filters, 1) <> ";" Then Filters = Filters + ";"
    For bNum = 1 To Len(Filters)
        Filter = Mid(Filters, bNum, InStr(bNum, Filters, ";") - bNum)
        If Right(Filter, 3) = "*.*" Then Filter = Left(Filter, Len(Filter) - 2)
        If LCase(FileName) Like LCase(Filter) Then
            MatchesFilter = True
            Exit Function
        End If
        bNum = InStr(bNum, Filters, ";")
    Next bNum
Else
    If Right(Filters, 3) = "*.*" Then Filters = Left(Filters, Len(Filters) - 2)
    If LCase(FileName) Like LCase(Filters) Then MatchesFilter = True
End If
End Function
Function RenameWithFilter(FileName As String, OldFilter As String, NewFilter As String) As String
Dim bNum As Long, Filters() As String, NewFileName As String, bNum2 As Long, bNum3 As Long, bNum4 As Long, bNum5 As Long
If Right(OldFilter, 3) = "*.*" Then OldFilter = Left(OldFilter, Len(OldFilter) - 2)
If Right(NewFilter, 3) = "*.*" Then NewFilter = Left(NewFilter, Len(NewFilter) - 2)
ReDim Filters(0) As String
bNum4 = 1
For bNum = 1 To Len(OldFilter)
    Select Case Mid(OldFilter, bNum, 1)
    Case "*"
        bNum2 = InStr(bNum + 1, OldFilter, "*")
        bNum3 = InStr(bNum + 1, OldFilter, "?")
        If bNum2 = 0 And bNum3 = 0 Then
            bNum2 = Len(OldFilter) + 1
        ElseIf (bNum3 < bNum2 Or bNum2 = 0) And bNum3 > 0 Then
            bNum2 = bNum3
        End If
        bNum5 = InStr(bNum4, FileName, Mid(OldFilter, bNum + 1, bNum2 - bNum - 1), 1)
        If bNum = Len(OldFilter) Then
            bNum5 = Len(FileName) + 1
        End If
        If bNum5 = 0 Then
            RenameWithFilter = FileName
            Exit Function
        End If
        If bNum > 1 Then
            If Mid(OldFilter, bNum - 1, 1) <> "*" And Mid(OldFilter, bNum - 1, 1) <> "?" Then
                ReDim Preserve Filters(UBound(Filters) + 1) As String
            End If
        Else
            ReDim Preserve Filters(UBound(Filters) + 1) As String
        End If
        Filters(UBound(Filters)) = Filters(UBound(Filters)) + Mid(FileName, bNum4, bNum5 - bNum4)
        bNum4 = bNum5
    Case "?"
        bNum2 = bNum + 1
        bNum5 = bNum4 + 1
        If bNum > 1 Then
            If Mid(OldFilter, bNum - 1, 1) <> "*" And Mid(OldFilter, bNum - 1, 1) <> "?" Then
                ReDim Preserve Filters(UBound(Filters) + 1) As String
            End If
        Else
            ReDim Preserve Filters(UBound(Filters) + 1) As String
        End If
        Filters(UBound(Filters)) = Filters(UBound(Filters)) + Mid(FileName, bNum4, 1)
        bNum4 = bNum5
    Case Else
        bNum4 = bNum4 + 1
    End Select
    If bNum4 > Len(FileName) Then
        If (Right(OldFilter, 1) <> "*" Or bNum + 1 < Len(OldFilter)) And bNum < Len(OldFilter) Then
            RenameWithFilter = FileName
            Exit Function
        Else
            Exit For
        End If
    End If
Next bNum
NewFileName = NewFilter
For bNum = 1 To UBound(Filters)
    bNum2 = InStr(bNum, NewFileName, "*")
    bNum3 = InStr(bNum, NewFileName, "?")
    If bNum2 = 0 And bNum3 = 0 Then
        bNum2 = Len(NewFileName) + 1
    ElseIf (bNum3 < bNum2 Or bNum2 = 0) And bNum3 > 0 Then
        bNum2 = bNum3
    End If
    If bNum2 > Len(NewFileName) Then
        RenameWithFilter = NewFileName
        Exit Function
    End If
    bNum4 = 0
    For bNum3 = bNum2 To Len(NewFileName)
        Select Case Mid(NewFileName, bNum3, 1)
        Case "*"
            bNum4 = Len(Filters(bNum))
            bNum3 = bNum3 + 1
            Exit For
        Case "?"
            bNum4 = bNum4 + 1
        Case Else
            Exit For
        End Select
    Next bNum3
    NewFileName = Left(NewFileName, bNum2 - 1) + Left(Filters(bNum), bNum4) + Mid(NewFileName, bNum3)
Next bNum
Do Until InStr(NewFileName, "*") = 0
    NewFileName = Left(NewFileName, InStr(NewFileName, "*") - 1) + Mid(NewFileName, InStr(NewFileName, "*") + 1)
Loop
Do Until InStr(NewFileName, "?") = 0
    NewFileName = Left(NewFileName, InStr(NewFileName, "?") - 1) + Mid(NewFileName, InStr(NewFileName, "?") + 1)
Loop
RenameWithFilter = NewFileName
End Function
Function MpqDir(MpqFile As String, Filters As String)
Dim Files As String, bNum As Long, EndLine As Long, fName As String
Files = ListFiles(MpqFile, ListFile)
bNum = 1
Do Until bNum > Len(Files)
    EndLine = InStr(bNum, Files, vbCrLf)
    If EndLine = 0 Then EndLine = Len(Files) + 1
    fName = Mid(Files, bNum, EndLine - bNum)
    If MatchesFilter(fName, Filters) Then
        bNum = EndLine + 2
    Else
        Files = Left(Files, bNum - 1) + Mid(Files, EndLine + 2)
    End If
Loop
MpqDir = Files
End Function
Sub RunScript(ScriptName As String)
Dim fNum As Long, Script As String, sLine As String, Param() As String, bNum As Long, EndLine As Long, pNum As Long, EndParam As Long, MpqFile As String, OldDefaultMaxFiles As Long, cType As Integer, lNum As Long, OldPath As String, NewPath As String, Rswitch As Boolean, Files As String, fCount As Long, fEndLine As Long, fLine As String, ScriptNewFile As Boolean, CurPath As String, fLine2 As String, fLineTitle As String, hMPQ As Long
If FileExists(ScriptName) = False Then
    ScriptOut.Show
    AddScriptOutput "Could not find script " + ScriptName + vbCrLf
    Exit Sub
End If
fNum = FreeFile
Open ScriptName For Binary As #fNum
Script = String(LOF(fNum), Chr(0))
Get #fNum, 1, Script
Close #fNum
OldPath = CurDir
If InStr(ScriptName, "\") > 0 Then
    For bNum = 1 To Len(ScriptName)
        If InStr(bNum, ScriptName, "\") > 0 Then
            bNum = InStr(bNum, ScriptName, "\")
            NewPath = Left(ScriptName, bNum)
        End If
    Next bNum
    If Mid(NewPath, 2, 1) = ":" Then ChDrive Left(NewPath, 1)
    ChDir NewPath
End If
CurPath = CurDir
If Right(Script, 2) <> vbCrLf Then Script = Script + vbCrLf
ScriptOut.Show
AddScriptOutput "Script: " + ScriptName + vbCrLf + vbCrLf
OldDefaultMaxFiles = MpqEx.Mpq.DefaultMaxFiles
lNum = 1
For bNum = 1 To Len(Script)
    EndLine = InStr(bNum, Script, vbCrLf)
    sLine = Mid(Script, bNum, EndLine - bNum)
    If Right(sLine, 1) <> " " Then sLine = sLine + " "
    If sLine <> "" Then
        AddScriptOutput "Line " + CStr(lNum) + ": "
        ReDim Param(0) As String
        For pNum = 1 To Len(sLine)
            If Mid(sLine, pNum, 1) = Chr(34) Then
                pNum = pNum + 1
                EndParam = InStr(pNum, sLine, Chr(34))
            Else
                EndParam = InStr(pNum, sLine, " ")
            End If
            If EndParam = 0 Then EndParam = Len(sLine) + 1
            If pNum <> EndParam Then
                If Trim(Mid(sLine, pNum, EndParam - pNum)) <> "" Then
                    ReDim Preserve Param(UBound(Param) + 1) As String
                    Param(UBound(Param)) = Trim(Mid(sLine, pNum, EndParam - pNum))
                End If
            End If
            pNum = EndParam
        Next pNum
        If UBound(Param) < 3 Then ReDim Preserve Param(3) As String
        Select Case LCase(Param(1))
        Case "o", "open"
            If Param(2) <> "" Then
                MpqFile = Param(2)
                If Param(3) <> "" And FileExists(MpqFile) = False Then
                    MpqEx.Mpq.DefaultMaxFiles = Param(3)
                End If
                If FileExists(MpqFile) Then
                    AddScriptOutput "Opened " + MpqFile + vbCrLf
                Else
                    AddScriptOutput "Created new " + MpqFile + vbCrLf
                End If
                NewPath = CurPath
            Else
                AddScriptOutput "Required parameter missing" + vbCrLf
            End If
        Case "n", "new"
            If Param(2) <> "" Then
                MpqFile = Param(2)
                If Param(3) <> "" Then
                    MpqEx.Mpq.DefaultMaxFiles = Param(3)
                End If
                ScriptNewFile = True
                AddScriptOutput "Created new " + MpqFile + vbCrLf
                NewPath = CurPath
            Else
                AddScriptOutput "Required parameter missing" + vbCrLf
            End If
        Case "c", "close"
            If MpqFile <> "" Then
                If LCase(CD.FileName) = LCase(FullPath(NewPath, MpqFile)) Then MpqEx.Timer1.Enabled = True
                AddScriptOutput "Closed " + MpqFile + vbCrLf
                MpqFile = ""
            Else
                AddScriptOutput "No archive open" + vbCrLf
            End If
        Case "p", "pause"
            AddScriptOutput "Pause not supported" + vbCrLf
        Case "a", "add"
            If MpqFile <> "" Then
                cType = 0
                Rswitch = False
                fCount = 0
                Files = ""
                fEndLine = 0
                fLine = ""
                For pNum = 3 To UBound(Param)
                    If LCase(Param(pNum)) = "/wav" Then
                        cType = 2
                    ElseIf LCase(Param(pNum)) = "/c" And cType < 2 Then
                        cType = 1
                    ElseIf LCase(Param(pNum)) = "/auto" And cType < 1 Then
                        cType = -1
                    ElseIf LCase(Param(pNum)) = "/r" Then
                        Rswitch = True
                    End If
                Next pNum
                If Left(Param(3), 1) = "/" Or Param(3) = "" Then
                    If InStr(Param(2), "*") <> 0 Or InStr(Param(2), "?") <> 0 Then
                        Param(3) = ""
                    Else
                        Param(3) = Param(2)
                    End If
                End If
                If Left(Param(2), 1) <> "/" And Param(2) <> "" Then
                    If InStr(Param(2), "\") > 0 Then
                        For pNum = 1 To Len(Param(2))
                            If InStr(pNum, Param(2), "\") > 0 Then
                                pNum = InStr(pNum, Param(2), "\")
                                Files = Left(Param(2), pNum)
                            End If
                        Next pNum
                    End If
                    If ScriptNewFile = True Then
                        If FileExists(FullPath(NewPath, MpqFile)) Then Kill FullPath(NewPath, MpqFile)
                        ScriptNewFile = False
                    End If
                    Files = DirEx(Files, Mid(Param(2), Len(Files) + 1), 6, Rswitch)
                    hMPQ = MpqEx.Mpq.mOpenMpq(FullPath(NewPath, MpqFile))
                    If hMPQ = 0 Then
                        AddScriptOutput "Can't create archive " + MpqFile + vbCrLf
                        GoTo CommandError
                    End If
                    For pNum = 1 To Len(Files)
                        fEndLine = InStr(pNum, Files, vbCrLf)
                        fLine = Mid(Files, pNum, fEndLine - pNum)
                        If pNum > 1 Then
                            AddScriptOutput "Line " + CStr(lNum) + ": "
                        End If
                        If cType = 0 Then
                            AddScriptOutput "Adding " + fLine + "..."
                        ElseIf cType = 1 Then
                            AddScriptOutput "Adding compressed " + fLine + "..."
                        ElseIf cType = 2 Then
                            AddScriptOutput "Adding compressed WAV " + fLine + "..."
                        ElseIf cType = -1 Then
                            AddScriptOutput "Adding " + fLine + " (compression auto-select)..."
                        End If
                        If InStr(Param(2), "*") <> 0 Or InStr(Param(2), "?") <> 0 Then
                            If Right(Param(3), 1) <> "\" And Param(3) <> "" Then Param(3) = Param(3) + "\"
                            If cType = 2 Then
                                MpqEx.Mpq.mAddWavFile hMPQ, FullPath(CurPath, fLine), Param(3) + fLine, 0
                            ElseIf cType = -1 Then
                                mAddAutoFile hMPQ, FullPath(CurPath, fLine), Param(3) + fLine
                            ElseIf cType = 1 Then
                                MpqEx.Mpq.mAddFile hMPQ, FullPath(CurPath, fLine), Param(3) + fLine, 1
                            Else
                                MpqEx.Mpq.mAddFile hMPQ, FullPath(CurPath, fLine), Param(3) + fLine, 0
                            End If
                        Else
                            If cType = 2 Then
                                MpqEx.Mpq.mAddWavFile hMPQ, FullPath(CurPath, fLine), Param(3), 0
                            ElseIf cType = -1 Then
                                mAddAutoFile hMPQ, FullPath(CurPath, fLine), Param(3)
                            ElseIf cType = 1 Then
                                MpqEx.Mpq.mAddFile hMPQ, FullPath(CurPath, fLine), Param(3), 1
                            Else
                                MpqEx.Mpq.mAddFile hMPQ, FullPath(CurPath, fLine), Param(3), 0
                            End If
                        End If
                        AddScriptOutput " Done" + vbCrLf
                        SendMessageA ScriptOut.oText.hWnd, WM_PAINT, 0, &O0
                        fCount = fCount + 1
                        pNum = fEndLine + 1
                    Next pNum
                    MpqEx.Mpq.mCloseMpq hMPQ
                    If fCount > 1 Then
                        AddScriptOutput "Line " + CStr(lNum) + ":  " + CStr(fCount) + " files of " + Param(2) + " added" + vbCrLf
                    End If
                Else
                    AddScriptOutput " Required parameter missing" + vbCrLf
                End If
            Else
                AddScriptOutput "No archive open" + vbCrLf
            End If
        Case "e", "extract"
            If MpqFile <> "" Then
                If InStr(Param(2), "*") = 0 And InStr(Param(2), "?") = 0 Then AddScriptOutput "Extracting " + Param(2) + "..."
                cType = 0
                For pNum = 3 To UBound(Param)
                    If LCase(Param(pNum)) = "/fp" Then
                        cType = 1
                        Exit For
                    End If
                Next pNum
                If Left(Param(3), 1) = "/" Then Param(3) = ""
                If Param(3) = "" Then Param(3) = "."
                If Left(Param(2), 1) <> "/" And Param(2) <> "" Then
                    If InStr(Param(2), "*") <> 0 Or InStr(Param(2), "?") <> 0 Then
                        Files = MpqDir(FullPath(NewPath, MpqFile), Param(2))
                        If MpqEx.Mpq.SFileOpenArchive(FullPath(NewPath, MpqFile), 0, 0, hMPQ) = 0 Then
                            AddScriptOutput "Can't open archive " + FullPath(NewPath, MpqFile) + vbCrLf
                            GoTo CommandError
                        End If
                        For pNum = 1 To Len(Files)
                            fEndLine = InStr(pNum, Files, vbCrLf)
                            fLine = Mid(Files, pNum, fEndLine - pNum)
                            If pNum > 1 Then
                                AddScriptOutput "Line " + CStr(lNum) + ": "
                            End If
                            AddScriptOutput "Extracting " + fLine + "..."
                            MpqEx.Mpq.sGetFile hMPQ, fLine, FullPath(CurPath, Param(3)), cType
                            AddScriptOutput " Done" + vbCrLf
                            
                            fCount = fCount + 1
                            pNum = fEndLine + 1
                        Next pNum
                        MpqEx.Mpq.SFileCloseArchive hMPQ
                        If fCount > 1 Then
                            AddScriptOutput "Line " + CStr(lNum) + ":  " + CStr(fCount) + " files of " + Param(2) + " extracted" + vbCrLf
                        End If
                    Else
                        MpqEx.Mpq.GetFile FullPath(NewPath, MpqFile), Param(2), FullPath(CurPath, Param(3)), cType
                        AddScriptOutput " Done" + vbCrLf
                    End If
                Else
                    AddScriptOutput " Required parameter missing" + vbCrLf
                End If
            Else
                AddScriptOutput "No archive open" + vbCrLf
            End If
        Case "r", "ren", "rename"
            If MpqFile <> "" Then
                If InStr(Param(2), "*") = 0 And InStr(Param(2), "?") = 0 Then AddScriptOutput "Renaming " + Param(2) + " => " + Param(3) + "..."
                If Param(2) <> "" And Param(3) <> "" Then
                    If InStr(Param(2), "*") <> 0 Or InStr(Param(2), "?") <> 0 Then
                        If InStr(Param(3), "*") <> 0 Or InStr(Param(3), "?") <> 0 Then
                            Files = MpqDir(FullPath(NewPath, MpqFile), Param(2))
                            For pNum = 1 To Len(Files)
                                fEndLine = InStr(pNum, Files, vbCrLf)
                                fLine = Mid(Files, pNum, fEndLine - pNum)
                                If pNum > 1 Then
                                    AddScriptOutput "Line " + CStr(lNum) + ": "
                                End If
                                fLine2 = RenameWithFilter(fLine, Param(2), Param(3))
                                AddScriptOutput "Renaming " + fLine + " => " + fLine2 + "..."
                                If MpqEx.Mpq.FileExists(FullPath(NewPath, MpqFile), fLine2) Then
                                    MpqEx.Mpq.DelFile FullPath(NewPath, MpqFile), fLine2
                                    MpqEx.Mpq.RenFile FullPath(NewPath, MpqFile), fLine, fLine2
                                Else
                                    MpqEx.Mpq.RenFile FullPath(NewPath, MpqFile), fLine, fLine2
                                End If
                                AddScriptOutput " Done" + vbCrLf
                                fCount = fCount + 1
                                pNum = fEndLine + 1
                            Next pNum
                            If fCount > 1 Then
                                AddScriptOutput "Line " + CStr(lNum) + ":  " + CStr(fCount) + " files of " + Param(2) + " renamed" + vbCrLf
                            End If
                        Else
                        AddScriptOutput "You must use wildcards with new name" + vbCrLf
                        End If
                    Else
                        If MpqEx.Mpq.FileExists(FullPath(NewPath, MpqFile), Param(3)) Then
                            MpqEx.Mpq.DelFile FullPath(NewPath, MpqFile), Param(3)
                            MpqEx.Mpq.RenFile FullPath(NewPath, MpqFile), Param(2), Param(3)
                        Else
                            MpqEx.Mpq.RenFile FullPath(NewPath, MpqFile), Param(2), Param(3)
                        End If
                        AddScriptOutput " Done" + vbCrLf
                    End If
                Else
                    AddScriptOutput " Required parameter missing" + vbCrLf
                End If
            Else
                AddScriptOutput "No archive open" + vbCrLf
            End If
        Case "m", "move"
            If MpqFile <> "" Then
                For pNum = 1 To Len(Param(2))
                    If InStr(bNum, Param(2), "\") Then
                        bNum = InStr(bNum, Param(2), "\")
                    Else
                        Exit For
                    End If
                Next pNum
                fLineTitle = Mid(Param(2), bNum)
                If Right(Param(3), 1) <> "\" And Param(3) <> "" Then Param(3) = Param(3) + "\"
                Param(3) = Param(3) + fLineTitle
                If InStr(Param(2), "*") = 0 And InStr(Param(2), "?") = 0 Then AddScriptOutput "Moving " + Param(2) + " => " + Param(3) + "..."
                If (Left(Param(2), 1) <> "/" And Param(2) <> "") And (Left(Param(3), 1) <> "/") Then
                    If InStr(Param(2), "*") <> 0 Or InStr(Param(2), "?") <> 0 Then
                        Files = MpqDir(FullPath(NewPath, MpqFile), Param(2))
                        For pNum = 1 To Len(Files)
                            fEndLine = InStr(pNum, Files, vbCrLf)
                            fLine = Mid(Files, pNum, fEndLine - pNum)
                            If pNum > 1 Then
                                AddScriptOutput "Line " + CStr(lNum) + ": "
                            End If
                            fLine2 = RenameWithFilter(fLine, Param(2), Param(3))
                            AddScriptOutput "Moving " + fLine + " => " + fLine2 + "..."
                            If MpqEx.Mpq.FileExists(FullPath(NewPath, MpqFile), fLine2) Then
                                MpqEx.Mpq.DelFile FullPath(NewPath, MpqFile), fLine2
                                MpqEx.Mpq.RenFile FullPath(NewPath, MpqFile), fLine, fLine2
                            Else
                                MpqEx.Mpq.RenFile FullPath(NewPath, MpqFile), fLine, fLine2
                            End If
                            AddScriptOutput " Done" + vbCrLf
                            fCount = fCount + 1
                            pNum = fEndLine + 1
                        Next pNum
                        If fCount > 1 Then
                            AddScriptOutput "Line " + CStr(lNum) + ":  " + CStr(fCount) + " files of " + Param(2) + " moved" + vbCrLf
                        End If
                    Else
                        If MpqEx.Mpq.FileExists(FullPath(NewPath, MpqFile), Param(3)) Then
                            MpqEx.Mpq.DelFile FullPath(NewPath, MpqFile), Param(3)
                            MpqEx.Mpq.RenFile FullPath(NewPath, MpqFile), Param(2), Param(3)
                        Else
                            MpqEx.Mpq.RenFile FullPath(NewPath, MpqFile), Param(2), Param(3)
                        End If
                        AddScriptOutput " Done" + vbCrLf
                    End If
                Else
                    AddScriptOutput " Required parameter missing" + vbCrLf
                End If
            Else
                AddScriptOutput "No archive open" + vbCrLf
            End If
        Case "d", "del", "delete"
            If MpqFile <> "" Then
                If InStr(Param(2), "*") = 0 And InStr(Param(2), "?") = 0 Then AddScriptOutput "Deleting " + Param(2) + "..."
                If Left(Param(2), 1) <> "/" And Param(2) <> "" Then
                    If InStr(Param(2), "*") <> 0 Or InStr(Param(2), "?") <> 0 Then
                        Files = MpqDir(FullPath(NewPath, MpqFile), Param(2))
                        For pNum = 1 To Len(Files)
                            fEndLine = InStr(pNum, Files, vbCrLf)
                            fLine = Mid(Files, pNum, fEndLine - pNum)
                            If pNum > 1 Then
                                AddScriptOutput "Line " + CStr(lNum) + ": "
                            End If
                            AddScriptOutput "Deleting " + fLine + "..."
                            MpqEx.Mpq.DelFile FullPath(NewPath, MpqFile), fLine
                            AddScriptOutput " Done" + vbCrLf
                            fCount = fCount + 1
                            pNum = fEndLine + 1
                        Next pNum
                        If fCount > 1 Then
                            AddScriptOutput "Line " + CStr(lNum) + ":  " + CStr(fCount) + " files of " + Param(2) + " deleted" + vbCrLf
                        End If
                    Else
                        MpqEx.Mpq.DelFile FullPath(NewPath, MpqFile), Param(2)
                        AddScriptOutput " Done" + vbCrLf
                    End If
                Else
                    AddScriptOutput " Required parameter missing" + vbCrLf
                End If
            Else
                AddScriptOutput "No archive open" + vbCrLf
            End If
        Case "f", "flush", "compact"
            If MpqFile <> "" Then
                AddScriptOutput "Flushing " + MpqFile + "..."
                MpqEx.Mpq.CompactMpq FullPath(NewPath, MpqFile)
                AddScriptOutput " Done" + vbCrLf
            Else
                AddScriptOutput "No archive open" + vbCrLf
            End If
        Case "l", "list"
            If MpqFile <> "" Then
                If Param(2) <> "" Then
                    AddScriptOutput "Creating list..."
                    If (InStr(Param(2), "*") <> 0 Or InStr(Param(2), "?") <> 0) And Param(3) <> "" Then
                        Files = MpqDir(FullPath(NewPath, MpqFile), Param(2))
                        Param(2) = Param(3)
                    Else
                        Files = ListFiles(FullPath(NewPath, MpqFile), ListFile)
                    End If
                    fNum = FreeFile
                    Open FullPath(CurPath, Param(2)) For Binary As #fNum
                    Put #fNum, 1, Files
                    Close #fNum
                    AddScriptOutput " Done" + vbCrLf
                Else
                    AddScriptOutput " Required parameter missing" + vbCrLf
                End If
            Else
                AddScriptOutput "No archive open" + vbCrLf
            End If
        Case "s", "script"
            AddScriptOutput "Running script " + Param(2) + "..." + vbCrLf + vbCrLf
            If Param(2) <> "" Then
                RunScript FullPath(CurPath, Param(2))
            Else
                AddScriptOutput " Required parameter missing" + vbCrLf
            End If
            AddScriptOutput vbCrLf + "Continuing with previous script..." + vbCrLf
        Case "x", "exit", "quit"
            Unload MpqEx
        Case Else
            If Left(Param(1), 1) <> ";" Then
                If LCase(Param(1)) = "cd" Or LCase(Param(1)) = "chdir" Then
                    On Error Resume Next
                    ChDir Param(2)
                    On Error GoTo 0
                    CurPath = CurDir
                    AddScriptOutput "Current directory is " + CurPath + vbCrLf
                ElseIf Left(LCase(Param(1)), 3) = "cd." Or Left(LCase(Param(1)), 3) = "cd\" Then
                    On Error Resume Next
                    ChDir Mid(Param(1), 3)
                    On Error GoTo 0
                    CurPath = CurDir
                    AddScriptOutput "Current directory is " + CurPath + vbCrLf
                ElseIf Left(LCase(Param(1)), 6) = "chdir." Or Left(LCase(Param(1)), 6) = "chdir\" Then
                    On Error Resume Next
                    ChDir Mid(Param(1), 6)
                    On Error GoTo 0
                    CurPath = CurDir
                    AddScriptOutput "Current directory is " + CurPath + vbCrLf
                ElseIf Mid(Param(1), 2, 1) = ":" And (Len(Param(1)) = 2 Or Right(Param(1), 1) = "\") Then
                    On Error Resume Next
                    ChDrive Left(Param(1), 2)
                    On Error GoTo 0
                    CurPath = CurDir
                    AddScriptOutput "Current directory is " + CurPath + vbCrLf
                Else
                    AddScriptOutput "Running command " + sLine + "..."
                    Shell "command.com /c " + sLine, 1
                    AddScriptOutput " Done" + vbCrLf
                End If
            Else
                AddScriptOutput "Comment  " + sLine + vbCrLf
            End If
        End Select
    End If
CommandError:
    lNum = lNum + 1
    bNum = EndLine + 1
Next bNum
MpqEx.Mpq.DefaultMaxFiles = OldDefaultMaxFiles
If Mid(OldPath, 2, 1) = ":" Then ChDrive Left(OldPath, 1)
ChDir OldPath
End Sub
Function SBytes(Num, Start As Long, Length As Long) As String
Dim buffer() As Byte, NumData As Currency
If Start + Length > 8 Then Length = 8 - Start
On Error Resume Next
NumData = Num / 10000
ReDim buffer(7)
CopyMemory buffer(0), NumData, 8
On Error GoTo 0
SBytes = Mid(StrConv(buffer, vbUnicode), Start + 1, Length)
End Function
Function FindMpqHeader(MpqFile As String) As Long
Dim fNum As Long, Text As String, bNum As Long, MpqHead As Long
If FileExists(MpqFile) = False Then
    FindMpqHeader = -1
    Exit Function
End If
fNum = FreeFile
Open MpqFile For Binary As #fNum
For bNum = 1 To LOF(fNum) Step 2 ^ 20
    Text = String(2 ^ 20 + 32, Chr(0))
    If LOF(fNum) - bNum + 1 >= 2 ^ 20 + 32 Then
        Get #fNum, bNum, Text
    Else
        Text = String(LOF(fNum) - bNum + 1, Chr(0))
        Get #fNum, bNum, Text
    End If
    MpqHead = InStr(Text, "MPQ" + Chr(26))
CheckAgain:
    If MpqHead > 0 Then
        If JBytes(Text, MpqHead + 4, 4) >= 32 And JBytes(Text, MpqHead + 12, 2) = 0 Then
            Exit For
        Else
            MpqHead = InStr(MpqHead + 4, Text, "MPQ" + Chr(26))
            GoTo CheckAgain
        End If
    End If
Next bNum
Close #fNum
FindMpqHeader = bNum + MpqHead - 2
If MpqHead = 0 Then FindMpqHeader = -1
End Function
Function JBytes(Text As String, Start As Long, Length As Long)
Dim buffer() As Byte, NumData As Currency
If Start + Length - 1 > Len(Text) Then Length = Len(Text) - (Start - 1)
On Error Resume Next
ReDim buffer(Length - 1)
buffer = StrConv(Mid(Text, Start, Length), vbFromUnicode)
CopyMemory NumData, buffer(0), Length
On Error GoTo 0
JBytes = NumData * 10000
End Function
Function GetNumMpqFiles(MpqFile As String) As Long
Dim fNum As Long, Text As String, MpqHeader As Long
fNum = FreeFile
Text = String(4, Chr(0))
MpqHeader = FindMpqHeader(MpqFile)
If MpqHeader > -1 Then
    Open MpqFile For Binary As #fNum
    Get #fNum, MpqHeader + 29, Text
    Close #fNum
    GetNumMpqFiles = JBytes(Text, 1, 4)
End If
End Function
