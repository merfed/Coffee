Attribute VB_Name = "FileDialog"
Option Explicit

Public NullPtr As String

Type OPENFILENAME
    lStructSize As Long
    hwndOwner As Long
    hInstance As Long
    Filter As String
    CustomFilter As String
    nMaxCustFilter As Long
    FilterIndex As Long
    FileName As String
    MaxFileSize As Long
    FileTitle As String
    MaxFileTitleSize As Long
    InitDir As String
    DialogTitle As String
    Flags As Long
    nFileOffset As Integer
    nFileExtension As Integer
    DefaultExt As String
    lCustData As Long
    lpfnHook As Long
    lpTemplateName As String
End Type

Type BROWSEINFO
    hwndOwner As Long
    pidlRoot As Long
    DisplayName As String
    Title As String
    Flags As Long
    lpfn As Long
    lParam As Long
    iImage As Long
End Type

Declare Function CommDlgExtendedError Lib "Comdlg32.dll" () As Long
Declare Function GetOpenFileName Lib "Comdlg32.dll" _
    Alias "GetOpenFileNameA" (lpofn As OPENFILENAME) As Boolean
Declare Function GetSaveFileName Lib "Comdlg32.dll" _
    Alias "GetSaveFileNameA" (lpofn As OPENFILENAME) As Boolean
Declare Function SHBrowseForFolder Lib "Shell32.dll" _
    (lpbi As BROWSEINFO) As Long
Declare Function SHGetPathFromIDList Lib "Shell32.dll" ( _
    pidl As Long, _
    ByRef pszPath As Byte) As Boolean

Public Const OFN_READONLY             As Long = &H1
Public Const OFN_OVERWRITEPROMPT      As Long = &H2
Public Const OFN_HIDEREADONLY         As Long = &H4
Public Const OFN_NOCHANGEDIR          As Long = &H8
Public Const OFN_SHOWHELP             As Long = &H10
Public Const OFN_ENABLEHOOK           As Long = &H20
Public Const OFN_ENABLETEMPLATE       As Long = &H40
Public Const OFN_ENABLETEMPLATEHANDLE As Long = &H80
Public Const OFN_NOVALIDATE           As Long = &H100
Public Const OFN_ALLOWMULTISELECT     As Long = &H200
Public Const OFN_EXTENSIONDIFFERENT   As Long = &H400
Public Const OFN_PATHMUSTEXIST        As Long = &H800
Public Const OFN_FILEMUSTEXIST        As Long = &H1000
Public Const OFN_CREATEPROMPT         As Long = &H2000
Public Const OFN_SHAREAWARE           As Long = &H4000
Public Const OFN_NOREADONLYRETURN     As Long = &H8000
Public Const OFN_NOTESTFILECREATE     As Long = &H10000
Public Const OFN_NONETWORKBUTTON      As Long = &H20000
Public Const OFN_NOLONGNAMES          As Long = &H40000        ' force no long names for 4.x modules
Public Const OFN_EXPLORER             As Long = &H80000        ' new look commdlg
Public Const OFN_NODEREFERENCELINKS   As Long = &H100000
Public Const OFN_LONGNAMES            As Long = &H200000       ' force long names for 3.x modules
Public Const OFN_ENABLEINCLUDENOTIFY  As Long = &H400000       ' send include message to callback
Public Const OFN_ENABLESIZING         As Long = &H800000

Public Const BIF_RETURNONLYFSDIRS   As Long = &H1     ' For finding a folder to start document searching
Public Const BIF_DONTGOBELOWDOMAIN  As Long = &H2     ' For starting the Find Computer
Public Const BIF_STATUSTEXT         As Long = &H4
Public Const BIF_RETURNFSANCESTORS  As Long = &H8
Public Const BIF_EDITBOX            As Long = &H10
Public Const BIF_VALIDATE           As Long = &H20    ' insist on valid result (or CANCEL)

Public Const BIF_BROWSEFORCOMPUTER  As Long = &H1000  ' Browsing for Computers.
Public Const BIF_BROWSEFORPRINTER   As Long = &H2000  ' Browsing for Printers
Public Const BIF_BROWSEINCLUDEFILES As Long = &H4000  ' Browsing for Everything

Function GetPathFromID(ByVal dwID As Long) As String
Dim buffer(1 To 260) As Byte
GetPathFromID = NullPtr
If SHGetPathFromIDList(ByVal dwID, buffer(1)) Then
    GetPathFromID = StrConv(buffer, vbUnicode)
    StripNull GetPathFromID
End If
End Function
Sub ReplaceChar(ByRef TextStr As String, ByVal Char As String, ByVal NewChar As String)
If Len(Char) > 1 Then Char = Left$(Char, 1)
If Len(NewChar) > 1 Then NewChar = Left$(NewChar, 1)
Dim cNum As Long, cNum2 As Long
For cNum = 1 To Len(TextStr)
    cNum2 = InStr(cNum, TextStr, Char)
    If cNum2 Then
        cNum = cNum2
        Mid$(TextStr, cNum, 1) = NewChar
    Else
        Exit Sub
    End If
Next cNum
End Sub
Sub StripNull(ByRef TextStr As String)
Dim cNum As Long, cNum2 As Long
For cNum = 1 To Len(TextStr)
    cNum2 = InStr(cNum, TextStr, Chr$(0))
    If cNum2 Then
        cNum = cNum2
        cNum2 = InStr(cNum + 1, TextStr, Chr$(0))
        If cNum + 1 = cNum2 Or cNum2 = 0 Then
            TextStr = Left(TextStr, cNum - 1)
            Exit Sub
        End If
    Else
        Exit Sub
    End If
Next cNum
End Sub
Sub InitFileDialog(ByRef lpFileDialog As OPENFILENAME)
lpFileDialog.lStructSize = Len(lpFileDialog)
lpFileDialog.hwndOwner = 0
lpFileDialog.hInstance = 0
lpFileDialog.Filter = NullPtr
lpFileDialog.CustomFilter = NullPtr
lpFileDialog.nMaxCustFilter = 0
lpFileDialog.FilterIndex = 0
lpFileDialog.FileName = NullPtr
lpFileDialog.MaxFileSize = 260
lpFileDialog.FileTitle = NullPtr
lpFileDialog.MaxFileTitleSize = 260
lpFileDialog.InitDir = NullPtr
lpFileDialog.DialogTitle = NullPtr
lpFileDialog.Flags = 0
lpFileDialog.nFileOffset = 0
lpFileDialog.nFileExtension = 0
lpFileDialog.DefaultExt = NullPtr
lpFileDialog.lCustData = 0
lpFileDialog.lpfnHook = 0
lpFileDialog.lpTemplateName = NullPtr
End Sub
Sub InitFolderDialog(ByRef lpFolderDialog As BROWSEINFO)
lpFolderDialog.hwndOwner = 0
lpFolderDialog.pidlRoot = 0
lpFolderDialog.DisplayName = NullPtr
lpFolderDialog.Title = NullPtr
lpFolderDialog.Flags = 0
lpFolderDialog.lpfn = 0
lpFolderDialog.lParam = 0
lpFolderDialog.iImage = 0
End Sub
Function ShowOpen(ByRef lpFileDialog As OPENFILENAME) As Boolean
lpFileDialog.lStructSize = Len(lpFileDialog)
ReplaceChar lpFileDialog.Filter, "|", Chr$(0)
lpFileDialog.Filter = lpFileDialog.Filter + Chr$(0)
If Len(lpFileDialog.FileName) <= lpFileDialog.MaxFileSize Then _
    lpFileDialog.FileName = lpFileDialog.FileName + String$(lpFileDialog.MaxFileSize - Len(lpFileDialog.FileName), Chr$(0))
If Len(lpFileDialog.FileTitle) <= lpFileDialog.MaxFileTitleSize Then _
    lpFileDialog.FileTitle = lpFileDialog.FileTitle + String$(lpFileDialog.MaxFileTitleSize - Len(lpFileDialog.FileTitle), Chr$(0))
ShowOpen = GetOpenFileName(lpFileDialog)
lpFileDialog.Filter = Left$(lpFileDialog.Filter, Len(lpFileDialog.Filter) - 1)
ReplaceChar lpFileDialog.Filter, Chr$(0), "|"
StripNull lpFileDialog.FileName
StripNull lpFileDialog.FileTitle
End Function
Function ShowSave(ByRef lpFileDialog As OPENFILENAME) As Boolean
lpFileDialog.lStructSize = Len(lpFileDialog)
ReplaceChar lpFileDialog.Filter, "|", Chr$(0)
lpFileDialog.Filter = lpFileDialog.Filter + Chr$(0)
If Len(lpFileDialog.FileName) <= lpFileDialog.MaxFileSize Then _
    lpFileDialog.FileName = lpFileDialog.FileName + String$(lpFileDialog.MaxFileSize - Len(lpFileDialog.FileName), Chr$(0))
If Len(lpFileDialog.FileTitle) <= lpFileDialog.MaxFileTitleSize Then _
    lpFileDialog.FileTitle = lpFileDialog.FileTitle + String$(lpFileDialog.MaxFileTitleSize - Len(lpFileDialog.FileTitle), Chr$(0))
ShowSave = GetSaveFileName(lpFileDialog)
lpFileDialog.Filter = Left$(lpFileDialog.Filter, Len(lpFileDialog.Filter) - 1)
ReplaceChar lpFileDialog.Filter, Chr$(0), "|"
StripNull lpFileDialog.FileName
StripNull lpFileDialog.FileTitle
End Function
Function ShowFolder(ByRef lpFolderDialog As BROWSEINFO) As Long
If Len(lpFolderDialog.DisplayName) <= 260 Then _
    lpFolderDialog.DisplayName = lpFolderDialog.DisplayName + String$(260 - Len(lpFolderDialog.DisplayName), Chr$(0))
ShowFolder = SHBrowseForFolder(lpFolderDialog)
StripNull lpFolderDialog.DisplayName
End Function
