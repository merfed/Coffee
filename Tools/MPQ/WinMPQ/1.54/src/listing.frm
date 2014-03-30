VERSION 4.00
Begin VB.Form MpqEx 
   Caption         =   "WinMPQ"
   ClientHeight    =   3510
   ClientLeft      =   1245
   ClientTop       =   1785
   ClientWidth     =   6690
   Height          =   4200
   Icon            =   "listing.frx":0000
   Left            =   1185
   LinkTopic       =   "Form1"
   ScaleHeight     =   3510
   ScaleWidth      =   6690
   Top             =   1155
   Width           =   6810
   Begin VB.Timer Timer1 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   6120
      Top             =   2160
   End
   Begin VB.TextBox txtCommand 
      BackColor       =   &H8000000F&
      Height          =   285
      Left            =   1440
      TabIndex        =   1
      Top             =   2880
      Width           =   4695
   End
   Begin VB.CommandButton cmdGo 
      Caption         =   "Go"
      Height          =   285
      Left            =   6120
      TabIndex        =   2
      Top             =   2880
      Width           =   495
   End
   Begin VB.ComboBox mFilter 
      Height          =   315
      ItemData        =   "listing.frx":27A2
      Left            =   5220
      List            =   "listing.frx":27A9
      Sorted          =   -1  'True
      TabIndex        =   3
      Text            =   "*"
      Top             =   30
      Width           =   675
   End
   Begin MSComctlLib.Toolbar Toolbar 
      Align           =   1  'Align Top
      Height          =   345
      Left            =   0
      TabIndex        =   5
      Top             =   0
      Width           =   6690
      _ExtentX        =   11800
      _ExtentY        =   609
      ButtonWidth     =   1535
      ButtonHeight    =   556
      Wrappable       =   0   'False
      Appearance      =   1
      Style           =   1
      ImageList       =   "ImageList1"
      _Version        =   393216
      BeginProperty Buttons {66833FE8-8583-11D1-B16A-00C0F0283628} 
         NumButtons      =   8
         BeginProperty Button1 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Caption         =   "New"
            Key             =   "New"
            Description     =   "Create a new archive"
            ToolTipText     =   "Create a new archive"
            ImageIndex      =   1
         EndProperty
         BeginProperty Button2 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Caption         =   "Open"
            Key             =   "Open"
            Description     =   "Open an existing archive"
            ToolTipText     =   "Open an existing archive"
         EndProperty
         BeginProperty Button3 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Enabled         =   0   'False
            Caption         =   "Add"
            Key             =   "Add"
            Description     =   "Add files to the archive"
            ToolTipText     =   "Add files to the archive"
         EndProperty
         BeginProperty Button4 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Enabled         =   0   'False
            Caption         =   "Add Folder"
            Key             =   "Add Folder"
            Description     =   "Add files from a folder and its subfolders"
            ToolTipText     =   "Add files from a folder and its subfolders"
         EndProperty
         BeginProperty Button5 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Enabled         =   0   'False
            Caption         =   "Extract"
            Key             =   "Extract"
            Description     =   "Extract files from the archive"
            ToolTipText     =   "Extract files from the archive"
         EndProperty
         BeginProperty Button6 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Enabled         =   0   'False
            Caption         =   "Compact"
            Key             =   "Compact"
            Description     =   "Clear deleted files from the archive"
            ToolTipText     =   "Clear deleted files from the archive"
         EndProperty
         BeginProperty Button7 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Enabled         =   0   'False
            Key             =   "filterspace"
            Style           =   4
            Object.Width           =   675
         EndProperty
         BeginProperty Button8 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Enabled         =   0   'False
            Caption         =   "List"
            Key             =   "List"
         EndProperty
      EndProperty
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      Caption         =   " MPQ2k &Command  "
      Height          =   195
      Left            =   0
      TabIndex        =   6
      Top             =   2880
      Width           =   1425
   End
   Begin MSComctlLib.ImageList ImageList1 
      Left            =   6120
      Top             =   1560
      _ExtentX        =   1005
      _ExtentY        =   1005
      BackColor       =   -2147483643
      ImageWidth      =   1
      ImageHeight     =   1
      MaskColor       =   12632256
      _Version        =   393216
      BeginProperty Images {2C247F25-8591-11D1-B16A-00C0F0283628} 
         NumListImages   =   1
         BeginProperty ListImage1 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "listing.frx":27B0
            Key             =   ""
         EndProperty
      EndProperty
   End
   Begin MSComctlLib.StatusBar StatBar 
      Align           =   2  'Align Bottom
      Height          =   300
      Left            =   0
      TabIndex        =   4
      Top             =   3210
      Width           =   6690
      _ExtentX        =   11800
      _ExtentY        =   529
      _Version        =   393216
      BeginProperty Panels {8E3867A5-8586-11D1-B16A-00C0F0283628} 
         NumPanels       =   2
         BeginProperty Panel1 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
            AutoSize        =   1
            Object.Width           =   5664
            MinWidth        =   2
            Key             =   "FileInfo"
         EndProperty
         BeginProperty Panel2 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
            AutoSize        =   1
            Object.Width           =   5664
            MinWidth        =   2
            Key             =   "MpqInfo"
         EndProperty
      EndProperty
   End
   Begin MSComctlLib.ListView List 
      Height          =   2295
      Left            =   0
      TabIndex        =   0
      Top             =   360
      Width           =   6015
      _ExtentX        =   10610
      _ExtentY        =   4048
      View            =   3
      Arrange         =   2
      Sorted          =   -1  'True
      MultiSelect     =   -1  'True
      LabelWrap       =   -1  'True
      HideSelection   =   -1  'True
      OLEDragMode     =   1
      OLEDropMode     =   1
      AllowReorder    =   -1  'True
      _Version        =   393217
      ForeColor       =   -2147483640
      BackColor       =   -2147483643
      BorderStyle     =   1
      Appearance      =   1
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      OLEDragMode     =   1
      OLEDropMode     =   1
      NumItems        =   5
      BeginProperty ColumnHeader(1) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         Key             =   "N"
         Text            =   "Name"
         Object.Width           =   5080
      EndProperty
      BeginProperty ColumnHeader(2) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         Alignment       =   1
         SubItemIndex    =   1
         Key             =   "S"
         Text            =   "Size"
         Object.Width           =   1905
      EndProperty
      BeginProperty ColumnHeader(3) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         Alignment       =   1
         SubItemIndex    =   2
         Key             =   "R"
         Text            =   "Ratio"
         Object.Width           =   1129
      EndProperty
      BeginProperty ColumnHeader(4) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         Alignment       =   1
         SubItemIndex    =   3
         Key             =   "PK"
         Text            =   "Packed"
         Object.Width           =   1905
      EndProperty
      BeginProperty ColumnHeader(5) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   4
         Key             =   "A"
         Text            =   "Attributes"
         Object.Width           =   1129
      EndProperty
   End
   Begin MPQCONTROLLib.MpqControl Mpq 
      Left            =   6120
      Top             =   600
      _Version        =   65542
      _ExtentX        =   873
      _ExtentY        =   873
      _StockProps     =   0
      TitleHidden     =   -1  'True
   End
   Begin VB.Menu mnuFile 
      Caption         =   "&File"
      Begin VB.Menu mnuFNew 
         Caption         =   "&New..."
         Shortcut        =   ^N
      End
      Begin VB.Menu mnuFOpen 
         Caption         =   "&Open..."
         Shortcut        =   ^O
      End
      Begin VB.Menu mnuFReopen 
         Caption         =   "&Reopen Mpq"
         Shortcut        =   {F5}
      End
      Begin VB.Menu mnuFScript 
         Caption         =   "Run Mo'PaQ 2000 &Script..."
         Shortcut        =   ^S
      End
      Begin VB.Menu mnuFSep 
         Caption         =   "-"
      End
      Begin VB.Menu mnuFExit 
         Caption         =   "E&xit"
      End
      Begin VB.Menu mnuFRecent 
         Caption         =   "-"
         Index           =   0
         Visible         =   0   'False
      End
   End
   Begin VB.Menu mnuMpq 
      Caption         =   "&Mpq"
      Enabled         =   0   'False
      Begin VB.Menu mnuMAdd 
         Caption         =   "&Add..."
         Shortcut        =   ^A
      End
      Begin VB.Menu mnuMAddFolder 
         Caption         =   "Add &Folder..."
         Shortcut        =   ^F
      End
      Begin VB.Menu mnuMCompression 
         Caption         =   "&Compression"
         Begin VB.Menu mnuMCAuto 
            Caption         =   "Auto-Select"
            Checked         =   -1  'True
            Shortcut        =   {F4}
         End
         Begin VB.Menu mnuMCSep 
            Caption         =   "-"
         End
         Begin VB.Menu mnuMCNone 
            Caption         =   "&None"
            Shortcut        =   {F2}
         End
         Begin VB.Menu mnuMCStandard 
            Caption         =   "&Standard"
            Shortcut        =   {F3}
         End
         Begin VB.Menu mnuMCAudio 
            Caption         =   "&Audio"
            Begin VB.Menu mnuMCALowest 
               Caption         =   "&Lowest (Best quality)"
               Shortcut        =   {F6}
            End
            Begin VB.Menu mnuMCAMedium 
               Caption         =   "&Medium"
               Shortcut        =   {F7}
            End
            Begin VB.Menu mnuMCAHighest 
               Caption         =   "&Highest (Least space)"
               Shortcut        =   {F8}
            End
         End
      End
      Begin VB.Menu mnuMExtract 
         Caption         =   "&Extract"
         Shortcut        =   ^E
      End
      Begin VB.Menu mnuMDelete 
         Caption         =   "&Delete    Del or"
         Shortcut        =   ^D
      End
      Begin VB.Menu mnuMRename 
         Caption         =   "Rena&me"
         Shortcut        =   ^R
      End
      Begin VB.Menu mnuMCompact 
         Caption         =   "Com&pact"
         Shortcut        =   ^P
      End
      Begin VB.Menu mnuMSaveList 
         Caption         =   "Save File &List..."
         Shortcut        =   ^L
      End
   End
   Begin VB.Menu mnuTools 
      Caption         =   "&Tools"
      Begin VB.Menu mnuTItem 
         Caption         =   "(Empty)"
         Enabled         =   0   'False
         Index           =   0
      End
      Begin VB.Menu mnuTSep 
         Caption         =   "-"
      End
      Begin VB.Menu mnuTAdd 
         Caption         =   "&Add/Remove..."
      End
   End
   Begin VB.Menu mnuOptions 
      Caption         =   "&Options..."
   End
   Begin VB.Menu mnuHelp 
      Caption         =   "&Help"
      Begin VB.Menu mnuHReadme 
         Caption         =   "View &Readme..."
         Shortcut        =   {F1}
      End
      Begin VB.Menu mnuHSep 
         Caption         =   "-"
      End
      Begin VB.Menu mnuHAbout 
         Caption         =   "&About..."
      End
   End
   Begin VB.Menu mnuPopup 
      Caption         =   "Popup Menu"
      Visible         =   0   'False
      Begin VB.Menu mnuPItem 
         Caption         =   "&Open"
         Index           =   0
      End
      Begin VB.Menu mnuPSep 
         Caption         =   "-"
      End
      Begin VB.Menu mnuPExtract 
         Caption         =   "&Extract"
      End
      Begin VB.Menu mnuPDelete 
         Caption         =   "&Delete"
      End
      Begin VB.Menu mnuPRename 
         Caption         =   "Rena&me"
      End
   End
End
Attribute VB_Name = "MpqEx"
Attribute VB_Creatable = False
Attribute VB_Exposed = False
Option Explicit

Dim txtCommandHasFocus As Boolean
Dim OpenFiles() As String, OpenFileDates() As Date, MpqDate As Date
Sub AddRecentFile(rFileName As String)
Dim bNum As Long, fNum As Long
NewKey AppKey + "Recent\"
For bNum = 1 To 8
    If LCase(GetReg(AppKey + "Recent\File" + CStr(bNum))) = LCase(rFileName) Then
        For fNum = bNum To 7
            If Not IsEmpty(GetReg(AppKey + "Recent\File" + CStr(fNum + 1))) Then
                SetReg AppKey + "Recent\File" + CStr(fNum), GetReg(AppKey + "Recent\File" + CStr(fNum + 1))
            Else
                Exit For
            End If
        Next fNum
        SetReg AppKey + "Recent\File" + CStr(fNum), rFileName
        Exit For
    End If
Next bNum
If fNum = 0 Then
    For bNum = 1 To 8
        If IsEmpty(GetReg(AppKey + "Recent\File" + CStr(bNum))) Then
            SetReg AppKey + "Recent\File" + CStr(bNum), rFileName
            Exit For
        ElseIf bNum = 8 Then
            For fNum = 1 To 7
                SetReg AppKey + "Recent\File" + CStr(fNum), GetReg(AppKey + "Recent\File" + CStr(fNum + 1))
            Next fNum
            SetReg AppKey + "Recent\File" + CStr(bNum), rFileName
        End If
    Next bNum
End If
BuildRecentFileList
End Sub
Sub BuildPopup(FileName As String, Shift As Integer)
Dim aNum As Long, aItem As String, aName As String, bNum As Long, PItem As Menu, dItem As String
mnuPopup.Tag = 0
For Each PItem In mnuPItem
    If PItem.Index <> 0 Then Unload PItem
Next PItem
If InStr(FileName, ".") = 0 Then
    GoSub AddUnknown
Else
    For bNum = 1 To Len(FileName)
        If InStr(bNum, FileName, ".") > 0 Then
            bNum = InStr(bNum, FileName, ".")
        Else
            Exit For
        End If
    Next bNum
    aName = Mid(FileName, bNum - 1)
    aName = GetReg("HKEY_CLASSES_ROOT\" + aName + "\")
    If aName = "" Then
        GoSub AddUnknown
        Exit Sub
    End If
    dItem = GetReg("HKEY_CLASSES_ROOT\" + aName + "\shell\", "open")
    dItem = GetReg(SharedAppKey + "FileDefaultActions\" + aName, dItem)
    If dItem <> "" And Not IsEmpty(GetReg("HKEY_CLASSES_ROOT\" + aName + "\shell\" + dItem + "\command\")) Then
        If LCase(dItem) = "openas" And IsEmpty(GetReg("HKEY_CLASSES_ROOT\" + aName + "\shell\" + dItem + "\")) Then
            mnuPItem(0).Caption = "Op&en with..."
        Else
            mnuPItem(0).Caption = GetReg("HKEY_CLASSES_ROOT\" + aName + "\shell\" + dItem + "\", "&" + UCase(Left(dItem, 1)) + Mid(dItem, 2))
        End If
        mnuPItem(0).Tag = dItem
        mnuPopup.Tag = 1
        aNum = 0
        bNum = 1
    Else
        aItem = EnumKey("HKEY_CLASSES_ROOT\" + aName + "\shell\", 0)
        If aItem = "" Then
            GoSub AddUnknown
            Exit Sub
        End If
        If Not IsEmpty(GetReg("HKEY_CLASSES_ROOT\" + aName + "\shell\" + aItem + "\command\")) Then
            If LCase(aItem) = "openas" And IsEmpty(GetReg("HKEY_CLASSES_ROOT\" + aName + "\shell\" + aItem + "\")) Then
                mnuPItem(0).Caption = "Op&en with..."
            Else
                mnuPItem(0).Caption = GetReg("HKEY_CLASSES_ROOT\" + aName + "\shell\" + aItem + "\", "&" + UCase(Left(aItem, 1)) + Mid(aItem, 2))
            End If
            mnuPItem(0).Tag = aItem
            mnuPopup.Tag = 1
            aNum = 1
            bNum = 1
        Else
            aNum = 1
            bNum = 0
        End If
    End If
    Do
        aItem = EnumKey("HKEY_CLASSES_ROOT\" + aName + "\shell\", aNum)
        If aItem <> "" Then
            If LCase(aItem) <> LCase(dItem) And Not IsEmpty(GetReg("HKEY_CLASSES_ROOT\" + aName + "\shell\" + aItem + "\command\")) Then
                On Error Resume Next
                Load mnuPItem(bNum)
                On Error GoTo 0
                If LCase(aItem) = "openas" And IsEmpty(GetReg("HKEY_CLASSES_ROOT\" + aName + "\shell\" + aItem + "\")) Then
                    mnuPItem(bNum).Caption = "Op&en with..."
                Else
                    mnuPItem(bNum).Caption = GetReg("HKEY_CLASSES_ROOT\" + aName + "\shell\" + aItem + "\", "&" + UCase(Left(aItem, 1)) + Mid(aItem, 2))
                End If
                mnuPItem(bNum).Tag = aItem
                mnuPopup.Tag = mnuPopup.Tag + 1
                bNum = bNum + 1
            End If
            aNum = aNum + 1
        End If
    Loop Until aItem = ""
    If Shift And vbShiftMask Then GoSub AddUnknown
End If
Exit Sub
AddUnknown:
    aNum = 0
    bNum = mnuPopup.Tag
    dItem = ""
    If bNum = 0 Then
        dItem = GetReg("HKEY_CLASSES_ROOT\Unknown\shell\", "open")
        dItem = GetReg(SharedAppKey + "FileDefaultActions\Unknown", dItem)
        If dItem <> "" And Not IsEmpty(GetReg("HKEY_CLASSES_ROOT\Unknown\shell\" + dItem + "\command\")) Then
            If LCase(dItem) = "openas" And IsEmpty(GetReg("HKEY_CLASSES_ROOT\Unknown\shell\" + dItem + "\")) Then
                mnuPItem(bNum).Caption = "Op&en with..."
            Else
                mnuPItem(bNum).Caption = GetReg("HKEY_CLASSES_ROOT\Unknown\shell\" + dItem + "\", "&" + UCase(Left(dItem, 1)) + Mid(dItem, 2))
            End If
            mnuPItem(bNum).Tag = dItem
            bNum = bNum + 1
        End If
    End If
    Do
        aItem = EnumKey("HKEY_CLASSES_ROOT\Unknown\shell\", aNum)
        If aItem <> "" Then
            If LCase(aItem) <> LCase(dItem) And Not IsEmpty(GetReg("HKEY_CLASSES_ROOT\Unknown\shell\" + aItem + "\command\")) Then
                On Error Resume Next
                Load mnuPItem(bNum)
                On Error GoTo 0
                If LCase(aItem) = "openas" And IsEmpty(GetReg("HKEY_CLASSES_ROOT\Unknown\shell\" + aItem + "\")) Then
                    mnuPItem(bNum).Caption = "Op&en with..."
                Else
                    mnuPItem(bNum).Caption = GetReg("HKEY_CLASSES_ROOT\Unknown\shell\" + aItem + "\", "&" + UCase(Left(aItem, 1)) + Mid(aItem, 2))
                End If
                mnuPItem(bNum).Tag = aItem
                bNum = bNum + 1
            End If
            aNum = aNum + 1
        End If
    Loop Until aItem = ""
Return
End Sub
Sub DelRecentFile(rFileName As String)
Dim bNum As Long, fNum As Long
For bNum = 1 To 8
    If LCase(GetReg(AppKey + "Recent\File" + CStr(bNum))) = LCase(rFileName) Then
        For fNum = bNum To 7
            SetReg AppKey + "Recent\File" + CStr(fNum), GetReg(AppKey + "Recent\File" + CStr(fNum + 1))
        Next fNum
        DelReg AppKey + "Recent\File" + CStr(8)
        Exit For
    End If
Next bNum
BuildRecentFileList
End Sub
Sub AddToListing(AddedFile As String)
Dim lIndex As Long, L1 As String, L2 As String, L3 As String, L4 As String, L5 As String, fSize As Long, cSize As Long, fFlags As Long
If Mpq.FileExists(CD.FileName, AddedFile) Then
    L1 = AddedFile
    fSize = Mpq.FileSize(CD.FileName, AddedFile)
    cSize = Mpq.GetFileInfo(CD.FileName, AddedFile, 6)
    If fSize / 1024 > 0 And fSize / 1024 < 1 Then
        L2 = "<1KB"
    ElseIf fSize = 0 Then
        L2 = "0KB"
    Else
        L2 = CStr(Int(fSize / 1024)) + "KB"
    End If
    If cSize / 1024 > 0 And cSize / 1024 < 1 Then
        L4 = "<1KB"
    ElseIf cSize = 0 Then
        L4 = "0KB"
    Else
        L4 = CStr(Int(cSize / 1024)) + "KB"
    End If
    If fSize <> 0 Then
        L3 = CStr(Int((1 - cSize / fSize) * 100)) + "%"
    Else
        L3 = "0%"
    End If
    fFlags = Mpq.GetFileInfo(CD.FileName, AddedFile, 7)
    If (fFlags And &H200) Or (fFlags And &H100) Then L5 = "C" Else L5 = "-"
    If fFlags And &H10000 Then L5 = L5 + "E" Else L5 = L5 + "-"
    If fFlags And &H20000 Then L5 = L5 + "X" Else L5 = L5 + "-"
    On Error Resume Next
    lIndex = List.ListItems.Add(, L1, L1).Index
    On Error GoTo 0
    If lIndex = 0 Then
        lIndex = List.ListItems.Item(L1).Index
        List.ListItems.Item(L1).ListSubItems.Clear
    End If
    List.ListItems.Item(lIndex).Tag = L1
    List.ListItems.Item(lIndex).ListSubItems.Add(, , L2).Tag = fSize
    If fSize <> 0 Then
        List.ListItems.Item(lIndex).ListSubItems.Add(, , L3).Tag = Int((1 - cSize / fSize) * 100)
    Else
        List.ListItems.Item(lIndex).ListSubItems.Add(, , L3).Tag = 0
    End If
    List.ListItems.Item(lIndex).ListSubItems.Add(, , L4).Tag = cSize
    List.ListItems.Item(lIndex).ListSubItems.Add(, , L5).Tag = L5
End If
End Sub
Sub MpqAddToListing(hMPQ As Long, AddedFile As String)
Dim lIndex As Long, L1 As String, L2 As String, L3 As String, L4 As String, L5 As String, fSize As Long, cSize As Long, fFlags As Long, hFile As Long
If Mpq.SFileOpenFileEx(hMPQ, AddedFile, 0, hFile) Then
    L1 = AddedFile
    fSize = Mpq.SFileGetFileSize(hFile, 0)
    cSize = Mpq.SFileGetFileInfo(hFile, 6)
    If fSize / 1024 > 0 And fSize / 1024 < 1 Then
        L2 = "<1KB"
    ElseIf fSize = 0 Then
        L2 = "0KB"
    Else
        L2 = CStr(Int(fSize / 1024)) + "KB"
    End If
    If cSize / 1024 > 0 And cSize / 1024 < 1 Then
        L4 = "<1KB"
    ElseIf cSize = 0 Then
        L4 = "0KB"
    Else
        L4 = CStr(Int(cSize / 1024)) + "KB"
    End If
    If fSize <> 0 Then
        L3 = CStr(Int((1 - cSize / fSize) * 100)) + "%"
    Else
        L3 = "0%"
    End If
    fFlags = Mpq.SFileGetFileInfo(hFile, 7)
    If (fFlags And &H200) Or (fFlags And &H100) Then L5 = "C" Else L5 = "-"
    If fFlags And &H10000 Then L5 = L5 + "E" Else L5 = L5 + "-"
    If fFlags And &H20000 Then L5 = L5 + "X" Else L5 = L5 + "-"
    On Error Resume Next
    lIndex = List.ListItems.Add(, L1, L1).Index
    On Error GoTo 0
    If lIndex = 0 Then
        lIndex = List.ListItems.Item(L1).Index
        List.ListItems.Item(L1).ListSubItems.Clear
    End If
    List.ListItems.Item(lIndex).Tag = L1
    List.ListItems.Item(lIndex).ListSubItems.Add(, , L2).Tag = fSize
    If fSize <> 0 Then
        List.ListItems.Item(lIndex).ListSubItems.Add(, , L3).Tag = Int((1 - cSize / fSize) * 100)
    Else
        List.ListItems.Item(lIndex).ListSubItems.Add(, , L3).Tag = 0
    End If
    List.ListItems.Item(lIndex).ListSubItems.Add(, , L4).Tag = cSize
    List.ListItems.Item(lIndex).ListSubItems.Add(, , L5).Tag = L5
    Mpq.SFileCloseFile hFile
End If
End Sub
Sub RemoveFromListing(RemovedFile As String)
Dim FileCount As Long
On Error GoTo FileRemoved
Do
List.ListItems.Remove RemovedFile
FileCount = FileCount + 1
Loop
FileRemoved:
If FileCount = 0 Then
    For FileCount = 1 To List.ListItems.Count
        If LCase(RemovedFile) = LCase(List.ListItems.Item(FileCount).Key) Then
            List.ListItems.Remove FileCount
            Exit Sub
        End If
    Next FileCount
End If
End Sub
Sub RenameInListing(OldName As String, NewName As String)
Dim lIndex As Long
If LCase(OldName) <> LCase(NewName) Then RemoveFromListing NewName
On Error GoTo RenameError
lIndex = List.ListItems.Item(OldName).Index
List.ListItems.Item(lIndex).Text = NewName
List.ListItems.Item(lIndex).Tag = NewName
On Error Resume Next
List.ListItems.Item(lIndex).Key = NewName
On Error GoTo 0
Exit Sub
RenameError:
For lIndex = 1 To List.ListItems.Count
    If LCase(OldName) = LCase(List.ListItems.Item(lIndex).Key) Then
        List.ListItems.Item(lIndex).Text = NewName
        List.ListItems.Item(lIndex).Tag = NewName
        On Error Resume Next
        List.ListItems.Item(lIndex).Key = NewName
        On Error GoTo 0
        Exit Sub
    End If
Next lIndex
End Sub
Sub ExecuteFile(FileName As String, Index As Integer)
Dim Param As String, bNum As Long, bNum2 As Long, EnvName As String
If Index < mnuPopup.Tag Then
    ShellExecute hWnd, mnuPItem(Index).Tag, FileName, vbNullString, vbNullString, 1
Else
    Param = GetReg("HKEY_CLASSES_ROOT\Unknown\shell\" + mnuPItem(Index).Tag + "\command\")
    Do
        If InStr(Param, "%1") = 0 Then
            Param = Param + " " + FileName
        Else
            bNum = InStr(Param, "%1")
            Param = Left(Param, bNum - 1) + FileName + Mid(Param, bNum + 2)
        End If
    Loop While InStr(Param, "%1")
    bNum = 1
    Do While bNum <= Len(Param)
        If InStr(bNum, Param, "%") Then
            bNum = InStr(bNum, Param, "%")
            If InStr(bNum + 1, Param, "%") Then
                bNum2 = InStr(bNum + 1, Param, "%")
                EnvName = Mid(Param, bNum + 1, bNum2 - bNum - 1)
                If Environ(EnvName) <> "" Then
                    Param = Left(Param, bNum - 1) + Environ(EnvName) + Mid(Param, bNum2 + 1)
                End If
            End If
        End If
        bNum = bNum + 1
    Loop
    On Error GoTo NoProgram
    Shell Param, 1
    On Error GoTo 0
End If
Exit Sub
NoProgram:
If Err.Number = 53 Then MsgBox "No program is assigned for this action.", , "WinMPQ"
End Sub
Sub RunMpq2kCommand(CmdLine As String)
Dim sLine As String, pNum As Long, Param() As String, EndParam As Long, CurPath As String, cType As Integer, Rswitch As Boolean, fCount As Long, Files As String, fEndLine As Long, fLine As String, bNum As Long, OldFileName As String, fNum As Long, cNum As Long, FileFilter As String, TItem As Menu, fLine2 As String, fLineTitle As String, hMPQ As Long, FileShortNames() As String
CurPath = CurDir
If Right(CurPath, 1) <> "\" Then CurPath = CurPath + "\"
sLine = CmdLine
If Right(sLine, 1) <> " " Then sLine = sLine + " "
If sLine <> "" Then
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
    Case "?", "h", "help"
        mnuHReadme_Click
    Case "o", "open"
        OldFileName = CD.FileName
        If Param(2) <> "" Then
            CD.FileName = FullPath(CurPath, Param(2))
        End If
        If Param(3) <> "" And FileExists(CD.FileName) = False And CD.FileName <> "" Then
            Mpq.DefaultMaxFiles = Param(3)
        End If
        If FileExists(CD.FileName) Then
            OpenMpq
            If CD.FileName = "" Then
                CD.FileName = OldFileName
                StatBar.SimpleText = "The file does not contain an MPQ archive."
            Else
                StatBar.SimpleText = "Opened " + CD.FileName
                AddRecentFile CD.FileName
            End If
        ElseIf FileExists(CD.FileName) = False And CD.FileName <> "" Then
            ReDim FileList(0) As String
            List.ListItems.Clear
            ShowSelected
            ShowTotal
            NewFile = True
            ReDim OpenFiles(0) As String, OpenFileDates(0) As Date
            mnuMpq.Enabled = True
            For Each TItem In mnuTItem
                TItem.Enabled = True
            Next TItem
            Toolbar.Buttons.Item("Add").Enabled = True
            Toolbar.Buttons.Item("Add Folder").Enabled = True
            Toolbar.Buttons.Item("Extract").Enabled = True
            Toolbar.Buttons.Item("Compact").Enabled = True
            Toolbar.Buttons.Item("List").Enabled = True
            If InStr(CD.FileName, "\") > 0 Then
                For bNum = 1 To Len(CD.FileName)
                    If InStr(bNum, CD.FileName, "\") > 0 Then
                        bNum = InStr(bNum, CD.FileName, "\")
                    Else
                        Exit For
                    End If
                Next bNum
            End If
            Caption = "WinMPQ - " + Mid(CD.FileName, bNum)
            StatBar.SimpleText = "Created new " + CD.FileName
            AddRecentFile CD.FileName
        ElseIf CD.FileName = "" Then
            StatBar.SimpleText = "Required parameter missing"
        End If
    Case "n", "new"
        If Param(2) <> "" Then
            CD.FileName = FullPath(CurPath, Param(2))
            If Param(3) <> "" Then
                Mpq.DefaultMaxFiles = Param(3)
            End If
            If CD.FileName <> "" Then
                ReDim FileList(0) As String
                List.ListItems.Clear
                ShowSelected
                ShowTotal
                NewFile = True
                ReDim OpenFiles(0) As String, OpenFileDates(0) As Date
                mnuMpq.Enabled = True
                For Each TItem In mnuTItem
                    TItem.Enabled = True
                Next TItem
                Toolbar.Buttons.Item("Add").Enabled = True
                Toolbar.Buttons.Item("Add Folder").Enabled = True
                Toolbar.Buttons.Item("Extract").Enabled = True
                Toolbar.Buttons.Item("Compact").Enabled = True
                Toolbar.Buttons.Item("List").Enabled = True
                If InStr(CD.FileName, "\") > 0 Then
                    For bNum = 1 To Len(CD.FileName)
                        If InStr(bNum, CD.FileName, "\") > 0 Then
                            bNum = InStr(bNum, CD.FileName, "\")
                        Else
                            Exit For
                        End If
                    Next bNum
                End If
                Caption = "WinMPQ - " + Mid(CD.FileName, bNum)
                StatBar.SimpleText = "Created new " + CD.FileName
                AddRecentFile CD.FileName
            End If
        Else
            StatBar.SimpleText = "Required parameter missing"
        End If
    Case "c", "close"
        StatBar.SimpleText = "Close is for scripts only"
    Case "p", "pause"
        StatBar.SimpleText = "Pause not supported"
    Case "a", "add"
        If CD.FileName <> "" Then
            ReDim FileShortNames(0) As String
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
                MousePointer = 11
                If NewFile = True Then
                    If FileExists(CD.FileName) Then Kill CD.FileName
                    NewFile = False
                End If
                Files = DirEx(Files, Mid(Param(2), Len(Files) + 1), 6, Rswitch)
                List.Sorted = False
                FileFilter = mFilter
                hMPQ = Mpq.mOpenMpq(CD.FileName)
                If hMPQ = 0 Then
                    StatBar.SimpleText = "Can't create archive " + CD.FileName
                    Exit Sub
                End If
                For pNum = 1 To Len(Files)
                    fEndLine = InStr(pNum, Files, vbCrLf)
                    fLine = Mid(Files, pNum, fEndLine - pNum)
                    If cType = 0 Then
                        StatBar.SimpleText = "Adding " + fLine + "..."
                    ElseIf cType = 1 Then
                        StatBar.SimpleText = "Adding compressed " + fLine + "..."
                    ElseIf cType = 2 Then
                        StatBar.SimpleText = "Adding compressed WAV " + fLine + "..."
                    ElseIf cType = -1 Then
                        StatBar.SimpleText = "Adding " + fLine + " (compression auto-select)..."
                    End If
                    If InStr(Param(2), "*") <> 0 Or InStr(Param(2), "?") <> 0 Then
                        If Right(Param(3), 1) <> "\" And Param(3) <> "" Then Param(3) = Param(3) + "\"
                        If cType = 2 Then
                            Mpq.mAddWavFile hMPQ, FullPath(CurPath, fLine), Param(3) + fLine, 0
                        ElseIf cType = -1 Then
                            mAddAutoFile hMPQ, FullPath(CurPath, fLine), Param(3) + fLine
                        ElseIf cType = 1 Then
                            Mpq.mAddFile hMPQ, FullPath(CurPath, fLine), Param(3) + fLine, 1
                        Else
                            Mpq.mAddFile hMPQ, FullPath(CurPath, fLine), Param(3) + fLine, 0
                        End If
                        If FileExists(CD.FileName) Then MpqDate = FileDateTime(CD.FileName)
                        mFilter.AddItem "*" + GetExtension(Param(3) + fLine)
                        For cNum = 1 To mFilter.ListCount - 1
                            If LCase(mFilter.List(cNum)) = LCase(mFilter.List(cNum - 1)) Then
                                mFilter.RemoveItem cNum
                                Exit For
                            End If
                        Next cNum
                        If MatchesFilter(Param(3) + fLine, FileFilter) Then
                            ReDim Preserve FileShortNames(UBound(FileShortNames) + 1) As String
                            FileShortNames(UBound(FileShortNames)) = Param(3) + fLine
                        End If
                    Else
                        If cType = 2 Then
                            Mpq.mAddWavFile hMPQ, FullPath(CurPath, fLine), Param(3), 0
                        ElseIf cType = -1 Then
                            mAddAutoFile hMPQ, FullPath(CurPath, fLine), Param(3)
                        ElseIf cType = 1 Then
                            Mpq.mAddFile hMPQ, FullPath(CurPath, fLine), Param(3), 1
                        Else
                            Mpq.mAddFile hMPQ, FullPath(CurPath, fLine), Param(3), 0
                        End If
                        If FileExists(CD.FileName) Then MpqDate = FileDateTime(CD.FileName)
                        mFilter.AddItem "*" + GetExtension(Param(3))
                        For cNum = 1 To mFilter.ListCount - 1
                            If LCase(mFilter.List(cNum)) = LCase(mFilter.List(cNum - 1)) Then
                                mFilter.RemoveItem cNum
                                Exit For
                            End If
                        Next cNum
                        If MatchesFilter(Param(3), FileFilter) Then
                            ReDim Preserve FileShortNames(UBound(FileShortNames) + 1) As String
                            FileShortNames(UBound(FileShortNames)) = Param(3)
                        End If
                    End If
                    StatBar.SimpleText = StatBar.SimpleText + " Done"
                    fCount = fCount + 1
                    pNum = fEndLine + 1
                Next pNum
                Mpq.mCloseMpq hMPQ
                If FileExists(CD.FileName) Then MpqDate = FileDateTime(CD.FileName)
                If UBound(FileShortNames) > 1 Then
                    If Mpq.SFileOpenArchive(CD.FileName, 0, 0, hMPQ) Then
                        StatBar.SimpleText = "Adding files to listing... 0% complete"
                        For pNum = 1 To UBound(FileShortNames)
                            If MatchesFilter(FileShortNames(pNum), FileFilter) Then
                                MpqAddToListing hMPQ, FileShortNames(pNum)
                            End If
                            On Error Resume Next
                            StatBar.SimpleText = "Adding files to listing... " + CStr(Int((pNum / UBound(FileShortNames)) * 100)) + "% complete"
                            On Error GoTo 0
                        Next pNum
                        Mpq.SFileCloseArchive hMPQ
                    End If
                ElseIf UBound(FileShortNames) = 1 Then
                    AddToListing FileShortNames(1)
                End If
                MousePointer = 0
                If MatchesFilter("(listfile)", FileFilter) Then
                    AddToListing "(listfile)"
                End If
                mFilter = FileFilter
                List.Sorted = True
                RemoveDuplicates
                ShowTotal
                If fCount > 1 Then
                    StatBar.SimpleText = CStr(fCount) + " files of " + Param(2) + " added"
                End If
            Else
                StatBar.SimpleText = "Required parameter missing"
            End If
        Else
            StatBar.SimpleText = "No archive open"
        End If
    Case "e", "extract"
        If CD.FileName <> "" Then
            If InStr(Param(2), "*") = 0 And InStr(Param(2), "?") = 0 Then StatBar.SimpleText = "Extracting " + Param(2) + "..."
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
                MousePointer = 11
                If InStr(Param(2), "*") <> 0 Or InStr(Param(2), "?") <> 0 Then
                    Files = MpqDir(CD.FileName, Param(2))
                    If Mpq.SFileOpenArchive(CD.FileName, 0, 0, hMPQ) = 0 Then
                        StatBar.SimpleText = "Can't open archive " + CD.FileName
                        Exit Sub
                    End If
                    For pNum = 1 To Len(Files)
                        fEndLine = InStr(pNum, Files, vbCrLf)
                        fLine = Mid(Files, pNum, fEndLine - pNum)
                        StatBar.SimpleText = "Extracting " + fLine + "..."
                        Mpq.sGetFile hMPQ, fLine, FullPath(CurPath, Param(3)), cType
                        StatBar.SimpleText = StatBar.SimpleText + " Done"
                        fCount = fCount + 1
                        pNum = fEndLine + 1
                    Next pNum
                    Mpq.SFileCloseArchive hMPQ
                    If fCount > 1 Then
                        StatBar.SimpleText = CStr(fCount) + " files of " + Param(2) + " extracted"
                    End If
                Else
                    Mpq.GetFile CD.FileName, Param(2), FullPath(CurPath, Param(3)), cType
                    StatBar.SimpleText = StatBar.SimpleText + " Done"
                End If
                MousePointer = 0
            Else
                StatBar.SimpleText = "Required parameter missing"
            End If
        Else
            StatBar.SimpleText = "No archive open"
        End If
    Case "r", "ren", "rename"
        If CD.FileName <> "" Then
            If InStr(Param(2), "*") = 0 And InStr(Param(2), "?") = 0 Then StatBar.SimpleText = "Renaming " + Param(2) + " => " + Param(3) + "..."
            If Param(2) <> "" And Param(3) <> "" Then
                If InStr(Param(2), "*") <> 0 Or InStr(Param(2), "?") <> 0 Then
                    If InStr(Param(3), "*") <> 0 Or InStr(Param(3), "?") <> 0 Then
                        Files = MpqDir(CD.FileName, Param(2))
                        For pNum = 1 To Len(Files)
                            fEndLine = InStr(pNum, Files, vbCrLf)
                            fLine = Mid(Files, pNum, fEndLine - pNum)
                            fLine2 = RenameWithFilter(fLine, Param(2), Param(3))
                            StatBar.SimpleText = "Renaming " + fLine + " => " + fLine2 + "..."
                            If Mpq.FileExists(CD.FileName, fLine2) Then
                                Mpq.DelFile CD.FileName, fLine2
                                Mpq.RenFile CD.FileName, fLine, fLine2
                            Else
                                Mpq.RenFile CD.FileName, fLine, fLine2
                            End If
                            If FileExists(CD.FileName) Then MpqDate = FileDateTime(CD.FileName)
                            RenameInListing fLine, fLine2
                            StatBar.SimpleText = StatBar.SimpleText + " Done"
                            fCount = fCount + 1
                            pNum = fEndLine + 1
                        Next pNum
                        If fCount > 1 Then
                            StatBar.SimpleText = CStr(fCount) + " files of " + Param(2) + " renamed"
                        End If
                    Else
                    StatBar.SimpleText = "You must use wildcards with new name"
                    End If
                Else
                    If Mpq.FileExists(CD.FileName, Param(3)) Then
                        Mpq.DelFile CD.FileName, Param(3)
                        Mpq.RenFile CD.FileName, Param(2), Param(3)
                    Else
                        Mpq.RenFile CD.FileName, Param(2), Param(3)
                    End If
                    If FileExists(CD.FileName) Then MpqDate = FileDateTime(CD.FileName)
                    RenameInListing Param(2), Param(3)
                    StatBar.SimpleText = StatBar.SimpleText + " Done"
                End If
            Else
                StatBar.SimpleText = "Required parameter missing"
            End If
        Else
            StatBar.SimpleText = "No archive open"
        End If
    Case "m", "move"
        If CD.FileName <> "" Then
            For pNum = 1 To Len(Param(2))
                If InStr(pNum, Param(2), "\") Then
                    pNum = InStr(pNum, Param(2), "\")
                Else
                    Exit For
                End If
            Next pNum
            fLineTitle = Mid(Param(2), pNum)
            If Right(Param(3), 1) <> "\" And Param(3) <> "" Then Param(3) = Param(3) + "\"
            Param(3) = Param(3) + fLineTitle
            If InStr(Param(2), "*") = 0 And InStr(Param(2), "?") = 0 Then StatBar.SimpleText = "Moving " + Param(2) + " => " + Param(3) + "..."
            If (Left(Param(2), 1) <> "/" And Param(2) <> "") And (Left(Param(3), 1) <> "/") Then
                If InStr(Param(2), "*") <> 0 Or InStr(Param(2), "?") <> 0 Then
                    Files = MpqDir(CD.FileName, Param(2))
                    For pNum = 1 To Len(Files)
                        fEndLine = InStr(pNum, Files, vbCrLf)
                        fLine = Mid(Files, pNum, fEndLine - pNum)
                        fLine2 = RenameWithFilter(fLine, Param(2), Param(3))
                        StatBar.SimpleText = "Moving " + fLine + " => " + fLine2 + "..."
                        If Mpq.FileExists(CD.FileName, fLine2) Then
                            Mpq.DelFile CD.FileName, fLine2
                            Mpq.RenFile CD.FileName, fLine, fLine2
                        Else
                            Mpq.RenFile CD.FileName, fLine, fLine2
                        End If
                        If FileExists(CD.FileName) Then MpqDate = FileDateTime(CD.FileName)
                        RenameInListing fLine, fLine2
                        StatBar.SimpleText = StatBar.SimpleText + " Done"
                        fCount = fCount + 1
                        pNum = fEndLine + 1
                    Next pNum
                    If fCount > 1 Then
                        StatBar.SimpleText = CStr(fCount) + " files of " + Param(2) + " moved"
                    End If
                Else
                    If Mpq.FileExists(CD.FileName, Param(3)) Then
                        Mpq.DelFile CD.FileName, Param(3)
                        Mpq.RenFile CD.FileName, Param(2), Param(3)
                    Else
                        Mpq.RenFile CD.FileName, Param(2), Param(3)
                    End If
                    If FileExists(CD.FileName) Then MpqDate = FileDateTime(CD.FileName)
                    RenameInListing Param(2), Param(3)
                    StatBar.SimpleText = StatBar.SimpleText + " Done"
                End If
            Else
                StatBar.SimpleText = "Required parameter missing"
            End If
        Else
            StatBar.SimpleText = "No archive open"
        End If
    Case "d", "del", "delete"
        If CD.FileName <> "" Then
            If InStr(Param(2), "*") = 0 And InStr(Param(2), "?") = 0 Then StatBar.SimpleText = "Deleting " + Param(2) + "..."
            If Left(Param(2), 1) <> "/" And Param(2) <> "" Then
                If InStr(Param(2), "*") <> 0 Or InStr(Param(2), "?") <> 0 Then
                    Files = MpqDir(CD.FileName, Param(2))
                    For pNum = 1 To Len(Files)
                        fEndLine = InStr(pNum, Files, vbCrLf)
                        fLine = Mid(Files, pNum, fEndLine - pNum)
                        StatBar.SimpleText = "Deleting " + fLine + "..."
                        Mpq.DelFile CD.FileName, fLine
                        If FileExists(CD.FileName) Then MpqDate = FileDateTime(CD.FileName)
                        RemoveFromListing fLine
                        StatBar.SimpleText = StatBar.SimpleText + " Done"
                        fCount = fCount + 1
                        pNum = fEndLine + 1
                    Next pNum
                    If fCount > 1 Then
                        StatBar.SimpleText = CStr(fCount) + " files of " + Param(2) + " deleted"
                    End If
                Else
                    Mpq.DelFile CD.FileName, Param(2)
                    If FileExists(CD.FileName) Then MpqDate = FileDateTime(CD.FileName)
                    RemoveFromListing Param(2)
                    StatBar.SimpleText = StatBar.SimpleText + " Done"
                End If
            Else
                StatBar.SimpleText = "Required parameter missing"
            End If
        Else
            StatBar.SimpleText = "No archive open"
        End If
    Case "f", "flush", "compact"
        If CD.FileName <> "" Then
            MousePointer = 11
            StatBar.SimpleText = "Flushing " + CD.FileName + "..."
            Mpq.CompactMpq CD.FileName
            If FileExists(CD.FileName) Then MpqDate = FileDateTime(CD.FileName)
            StatBar.SimpleText = StatBar.SimpleText + " Done"
            MousePointer = 0
            OpenMpq
        Else
            StatBar.SimpleText = "No archive open"
        End If
    Case "l", "list"
        If CD.FileName <> "" Then
            If Param(2) <> "" Then
                StatBar.SimpleText = "Creating list..."
                MousePointer = 11
                If (InStr(Param(2), "*") <> 0 Or InStr(Param(2), "?") <> 0) And Param(3) <> "" Then
                    Files = MpqDir(CD.FileName, Param(2))
                    Param(2) = Param(3)
                Else
                    Files = ListFiles(CD.FileName, ListFile)
                End If
                fNum = FreeFile
                Open FullPath(CurPath, Param(2)) For Binary As #fNum
                Put #fNum, 1, Files
                Close #fNum
                StatBar.SimpleText = StatBar.SimpleText + " Done"
                MousePointer = 0
            Else
                StatBar.SimpleText = "Required parameter missing"
            End If
        Else
            StatBar.SimpleText = "No archive open"
        End If
    Case "s", "script"
        StatBar.SimpleText = "Running script " + Param(2) + "..."
        If Param(2) <> "" Then
            MousePointer = 11
            RunScript FullPath(CurPath, Param(2))
            MousePointer = 0
            StatBar.SimpleText = StatBar.SimpleText + " Done"
        Else
            StatBar.SimpleText = "Required parameter missing"
        End If
    Case "x", "exit", "quit"
        Unload Me
    Case Else
        If Left(Param(1), 1) <> ";" Then
            If LCase(Param(1)) = "cd" Or LCase(Param(1)) = "chdir" Then
                On Error Resume Next
                ChDir Param(2)
                On Error GoTo 0
                txtCommand_GotFocus
            ElseIf Left(LCase(Param(1)), 3) = "cd." Or Left(LCase(Param(1)), 3) = "cd\" Then
                On Error Resume Next
                ChDir Mid(Param(1), 3)
                On Error GoTo 0
                txtCommand_GotFocus
            ElseIf Left(LCase(Param(1)), 6) = "chdir." Or Left(LCase(Param(1)), 6) = "chdir\" Then
                On Error Resume Next
                ChDir Mid(Param(1), 6)
                On Error GoTo 0
                txtCommand_GotFocus
            ElseIf Mid(Param(1), 2, 1) = ":" And (Len(Param(1)) = 2 Or Right(Param(1), 1) = "\") Then
                On Error Resume Next
                ChDrive Left(Param(1), 2)
                On Error GoTo 0
                txtCommand_GotFocus
            Else
                Shell "command.com /k " + sLine, 1
            End If
        End If
    End Select
End If
End Sub
Sub BuildRecentFileList()
Dim rNum As Long, rNum2 As Long, RecentFile As String, FirstSep As Long, LastSep As Long, RItem As Menu
For Each RItem In mnuFRecent
    If RItem.Index <> 0 Then Unload RItem
Next RItem
rNum2 = 1
For rNum = 8 To 1 Step -1
    RecentFile = GetReg(AppKey + "Recent\File" + CStr(rNum))
    If FileExists(RecentFile) Then
        mnuFRecent(0).Visible = True
        On Error Resume Next
        Load mnuFRecent(rNum2)
        On Error GoTo 0
        mnuFRecent(rNum2).Tag = RecentFile
        If TextWidth(RecentFile) > TextWidth("________________________________") Then
            FirstSep = InStr(RecentFile, "\")
            If FirstSep > 0 Then
                For LastSep = FirstSep + 1 To Len(RecentFile)
                    If InStr(LastSep, RecentFile, "\") > 0 Then
                        LastSep = InStr(LastSep, RecentFile, "\")
                    Else
                        Exit For
                    End If
                Next LastSep
                RecentFile = Left(RecentFile, FirstSep) + "..." + Mid(RecentFile, LastSep - 1)
            End If
        End If
        mnuFRecent(rNum2).Caption = "&" + CStr(rNum2) + " " + RecentFile
        rNum2 = rNum2 + 1
    End If
    If rNum2 > 4 Then Exit For
Next rNum
End Sub
Sub BuildToolsList()
Dim tNum As Long, ToolName As String, ToolCommand, TItem As Menu
For Each TItem In mnuTItem
    If TItem.Index <> 0 Then Unload TItem
Next TItem
mnuTItem(0).Caption = "(Empty)"
mnuTItem(0).Tag = ""
Do
    ToolName = GetReg(AppKey + "Tools\Name" + CStr(tNum))
    ToolCommand = GetReg(AppKey + "Tools\Command" + CStr(tNum))
    If ToolName = "" Then ToolName = ToolCommand
    If ToolName <> "" Then
        On Error Resume Next
        Load mnuTItem(tNum)
        On Error GoTo 0
        mnuTItem(tNum).Tag = ToolCommand
        If InStr(ToolName, "&") = 0 And tNum < 9 Then
            mnuTItem(tNum).Caption = "&" + CStr(tNum + 1) + " " + ToolName
        ElseIf InStr(ToolName, "&") = 0 And tNum = 9 Then
            mnuTItem(tNum).Caption = "&0 " + ToolName
        Else
            mnuTItem(tNum).Caption = ToolName
        End If
    End If
    tNum = tNum + 1
Loop Until ToolName = ""
End Sub
Sub OpenMpq()
Dim Path, FileCont As String, bNum As Long, FileLine As String, nFiles As Long, LoadExtraInfo As Integer, MpqFileName As String, FileFilter As String, TItem As Menu, hMPQ As Long, hFile As Long
On Error Resume Next
If FileExists(CD.FileName) And FileLen(CD.FileName) = 0 Then
    ReDim FileList(0) As String
    List.ListItems.Clear
    ShowSelected
    ShowTotal
    NewFile = True
    On Error GoTo 0
    GoTo FileOpened
End If
On Error GoTo 0
If IsMPQ(CD.FileName) = False Then
    CD.FileName = ""
    MsgBox "This file does not contain an MPQ archive.", vbExclamation, "WinMPQ"
    Exit Sub
End If
If Mpq.SFileOpenArchive(CD.FileName, 0, 0, hMPQ) = 0 Then
    CD.FileName = ""
    MsgBox "The MPQ archive could not be opened.", vbExclamation, "WinMPQ"
    Exit Sub
End If
StatBar.Style = 1
StatBar.SimpleText = "Loading list..."
MousePointer = 11
Path = App.Path: If Right(Path, 1) <> "\" Then Path = Path + "\"
ReDim FileList(0) As String
#If InternalListing Then
FileList(0) = "(listfile)"
If Mpq.FileExists(CD.FileName, "(listfile)") Then
    FileCont = Mpq.GetFileEx(CD.FileName, "(listfile)", 0, -1)
#Else
    FileCont = sListFiles(CD.FileName, hMPQ, ListFile)
#End If
    For bNum = 1 To Len(FileCont)
        If InStr(bNum, FileCont, vbCrLf) > 0 Then
            ReDim Preserve FileList(UBound(FileList) + 1) As String
            FileList(UBound(FileList)) = Mid(FileCont, bNum, InStr(bNum, FileCont, vbCrLf) - bNum)
            bNum = InStr(bNum, FileCont, vbCrLf) + 1
        Else
            ReDim Preserve FileList(UBound(FileList) + 1) As String
            FileList(UBound(FileList)) = Mid(FileCont, bNum)
            Exit For
        End If
    Next bNum
#If InternalListing Then
End If
nFiles = UBound(FileList)
ReDim Preserve FileList(UBound(FileList) + UBound(GlobalFileList)) As String
For bNum = nFiles + 1 To UBound(FileList)
    FileList(bNum) = GlobalFileList(bNum - nFiles)
Next bNum
#End If
Dim fNum As Long, lIndex As Long, L1 As String, L2 As String, L3 As String, L4 As String, L5 As String, fSize As Long, cSize As Long, fFlags As Long
SendMessageA List.hWnd, WM_SETREDRAW, 0, ByVal 0&
List.ListItems.Clear
List.Sorted = False
LoadExtraInfo = GetReg(AppKey + "LoadExtraInfo", 1)
FileFilter = mFilter
StatBar.SimpleText = "Building list... 0% complete"
For fNum = 1 To UBound(FileList)
#If InternalListing Then
    If Mpq.FileExists(CD.FileName, FileList(fNum)) Then
#End If
    MpqFileName = FileList(fNum)
    mFilter.AddItem "*" + GetExtension(MpqFileName)
    For bNum = 1 To mFilter.ListCount - 1
        If LCase(mFilter.List(bNum)) = LCase(mFilter.List(bNum - 1)) Then
            mFilter.RemoveItem bNum
            Exit For
        End If
    Next bNum
    If MatchesFilter(MpqFileName, FileFilter) Then
        L1 = FileList(fNum)
        If LoadExtraInfo > 0 And FileList(fNum) <> "" Then
            If Mpq.SFileOpenFileEx(hMPQ, FileList(fNum), 0, hFile) <> 0 Then
                fSize = Mpq.SFileGetFileSize(hFile, 0)
                cSize = Mpq.SFileGetFileInfo(hFile, 6)
                If fSize / 1024 > 0 And fSize / 1024 < 1 Then
                    L2 = "<1KB"
                ElseIf fSize = 0 Then
                    L2 = "0KB"
                Else
                    L2 = CStr(Int(fSize / 1024)) + "KB"
                End If
                If cSize / 1024 > 0 And cSize / 1024 < 1 Then
                    L4 = "<1KB"
                ElseIf cSize = 0 Then
                    L4 = "0KB"
                Else
                    L4 = CStr(Int(cSize / 1024)) + "KB"
                End If
                If fSize <> 0 Then
                    L3 = CStr(Int((1 - cSize / fSize) * 100)) + "%"
                Else
                    L3 = "0%"
                End If
                fFlags = Mpq.SFileGetFileInfo(hFile, 7)
                If (fFlags And &H200) Or (fFlags And &H100) Then L5 = "C" Else L5 = "-"
                If fFlags And &H10000 Then L5 = L5 + "E" Else L5 = L5 + "-"
                If fFlags And &H20000 Then L5 = L5 + "X" Else L5 = L5 + "-"
                Mpq.SFileCloseFile hFile
            End If
        End If
        lIndex = 0
        On Error Resume Next
        lIndex = List.ListItems.Add(, L1, L1).Index
        On Error GoTo 0
        If lIndex = 0 Then
            lIndex = List.ListItems.Item(L1).Index
            List.ListItems.Item(L1).ListSubItems.Clear
        End If
        List.ListItems.Item(lIndex).Tag = L1
        List.ListItems.Item(lIndex).ListSubItems.Add(, , L2).Tag = fSize
        If LoadExtraInfo > 0 Then
            If fSize <> 0 Then
                List.ListItems.Item(lIndex).ListSubItems.Add(, , L3).Tag = Int((1 - cSize / fSize) * 100)
            Else
                List.ListItems.Item(lIndex).ListSubItems.Add(, , L3).Tag = 0
            End If
            List.ListItems.Item(lIndex).ListSubItems.Add(, , L4).Tag = cSize
            List.ListItems.Item(lIndex).ListSubItems.Add(, , L5).Tag = L5
        End If
    End If
#If InternalListing Then
    End If
#End If
    On Error Resume Next
    StatBar.SimpleText = "Building list... " + CStr(Int((fNum / UBound(FileList)) * 100)) + "% complete"
    On Error GoTo 0
Next fNum
Mpq.SFileCloseArchive hMPQ
List.Sorted = True
#If InternalListing Then
RemoveDuplicates
#End If
On Error Resume Next
List.SelectedItem.Selected = False
On Error GoTo 0
SendMessageA List.hWnd, WM_SETREDRAW, 1, ByVal 0&
ShowSelected
ShowTotal
NewFile = False
mFilter = FileFilter
FileOpened:
ReDim OpenFiles(0) As String, OpenFileDates(0) As Date
If FileExists(CD.FileName) Then MpqDate = FileDateTime(CD.FileName)
mnuMpq.Enabled = True
For Each TItem In mnuTItem
    TItem.Enabled = True
Next TItem
Toolbar.Buttons.Item("Add").Enabled = True
Toolbar.Buttons.Item("Add Folder").Enabled = True
Toolbar.Buttons.Item("Extract").Enabled = True
Toolbar.Buttons.Item("Compact").Enabled = True
Toolbar.Buttons.Item("List").Enabled = True
StatBar.Style = 0
StatBar.SimpleText = ""
If InStr(CD.FileName, "\") > 0 Then
    For bNum = 1 To Len(CD.FileName)
        If InStr(bNum, CD.FileName, "\") > 0 Then
            bNum = InStr(bNum, CD.FileName, "\")
        Else
            Exit For
        End If
    Next bNum
End If
Caption = "WinMPQ - " + Mid(CD.FileName, bNum)
AddRecentFile CD.FileName
MousePointer = 0
End Sub
Sub RemoveDuplicates()
Dim fNum As Long
fNum = 1
Do While fNum <= List.ListItems.Count - 1
    If LCase(List.ListItems.Item(fNum).Tag) = LCase(List.ListItems.Item(fNum + 1).Tag) Then
        List.ListItems.Remove (fNum)
        fNum = fNum - 1
    End If
    fNum = fNum + 1
Loop
End Sub
Sub ShowSelected()
Dim fNum As Long, nSelect As Long, sSize As Long, fSize As Long, L2 As String
On Error GoTo NotSelected
List.SelectedItem.Tag = List.SelectedItem.Tag
On Error GoTo 0
For fNum = 1 To List.ListItems.Count
    If List.ListItems.Item(fNum).Selected Then
        nSelect = nSelect + 1
        If List.ListItems.Item(fNum).ListSubItems(1).Text <> "" Then
            sSize = sSize + List.ListItems.Item(fNum).ListSubItems(1).Tag
        Else
            fSize = Mpq.FileSize(CD.FileName, List.ListItems.Item(fNum).Tag)
            If fSize / 1024 > 0 And fSize / 1024 < 1 Then
                L2 = "<1KB"
            ElseIf fSize = 0 Then
                L2 = "0KB"
            Else
                L2 = CStr(Int(fSize / 1024)) + "KB"
            End If
            List.ListItems.Item(fNum).ListSubItems(1).Text = L2
            List.ListItems.Item(fNum).ListSubItems(1).Tag = fSize
            sSize = sSize + List.ListItems.Item(fNum).ListSubItems(1).Tag
        End If
    End If
Next fNum
If sSize / 1024 > 0 And sSize / 1024 < 1 Then
    StatBar.Panels.Item(1).Text = "Selected " + CStr(nSelect) + " files, <1KB"
ElseIf sSize = 0 Then
    StatBar.Panels.Item(1).Text = "Selected " + CStr(nSelect) + " files, 0KB"
Else
    StatBar.Panels.Item(1).Text = "Selected " + CStr(nSelect) + " files, " + CStr(Int(sSize / 1024)) + "KB"
End If
Exit Sub
NotSelected:
StatBar.Panels.Item(1).Text = "Selected 0 files, 0KB"
End Sub
Sub ShowTotal()
Dim fNum As Long, nFiles As Long, tSize As Long
For fNum = 1 To List.ListItems.Count
    nFiles = nFiles + 1
    If List.ListItems.Item(fNum).ListSubItems(1).Text <> "" Then
        tSize = tSize + List.ListItems.Item(fNum).ListSubItems(1).Tag
    End If
Next fNum
If tSize / 1024 > 0 And tSize / 1024 < 1 Then
    StatBar.Panels.Item(2).Text = "Total " + CStr(nFiles) + "/" + CStr(GetNumMpqFiles(CD.FileName)) + " files, <1KB"
Else
    StatBar.Panels.Item(2).Text = "Total " + CStr(nFiles) + "/" + CStr(GetNumMpqFiles(CD.FileName)) + " files, " + CStr(Int(tSize / 1024)) + "KB"
End If
End Sub
Private Sub cmdGo_Click()
StatBar.Style = 1
RunMpq2kCommand txtCommand
txtCommand = ""
If StatBar.SimpleText = "" Then txtCommand_GotFocus
End Sub
Private Sub Form_Load()
Dim FileName As String, bNum As Long, CurPath As String, CurPath2 As String, CurPathType As Integer, sLine As String, Param() As String, pNum As Long, EndParam As Long, ParamCutout As String, OldStartPath As String, NewStartPath As String, ErrorText As String
FixIcon hWnd, 1
InitFileDialog CD
CD.hwndOwner = hWnd
CD.DefaultExt = "mpq"
CD.MaxFileSize = 5120
InitFolderDialog PathInput
PathInput.hwndOwner = hWnd
PathInput.Flags = BIF_RETURNONLYFSDIRS
ReDim OpenFiles(0) As String, OpenFileDates(0) As Date
Dim Path
Path = App.Path: If Right(Path, 1) <> "\" Then Path = Path + "\"
ChDir App.Path
If Mpq.MpqInitialize = False Then
    ErrorText = "Mpq Control did not initialize properly!" + vbCrLf + "Reason:  "
    Select Case Mpq.LastError
    Case MPQ_ERROR_NO_STAREDIT
        ErrorText = ErrorText + "Can't find StarEdit.exe"
    Case MPQ_ERROR_BAD_STAREDIT
        ErrorText = ErrorText + "Wrong version of StarEdit.exe.  Need SC/BW 1.07"
    Case MPQ_ERROR_STAREDIT_RUNNING
        ErrorText = ErrorText + "StarEdit.exe is running.  It must be closed before running this"
    Case Else
        ErrorText = ErrorText + "Unknown"
    End Select
    MsgBox ErrorText
    End
End If
ExtractPathNum = -1
CopyPathNum = -1
OldStartPath = CurDir
CurPath = GetReg(AppKey + "StartupPath", CurDir)
CurPathType = GetReg(AppKey + "StartupPathType", 0)
If CurPathType < 0 Then CurPathType = 0
If CurPathType > 2 Then CurPathType = 2
If CurPathType = 1 Then
    CurPath = App.Path
End If
CurPath2 = CurPath
If Right(CurPath2, 1) <> "\" Then CurPath2 = CurPath2 + "\"
If IsDir(CurPath2) Then
    If Mid(CurPath, 2, 1) = ":" Then ChDrive Left(CurPath, 1)
    ChDir CurPath
End If
NewStartPath = CurDir
On Error Resume Next
Height = GetReg(AppKey + "Status\WindowHeight", Height)
Left = GetReg(AppKey + "Status\WindowLeft", Left)
Top = GetReg(AppKey + "Status\WindowTop", Top)
Width = GetReg(AppKey + "Status\WindowWidth", Width)
If GetReg(AppKey + "Status\WindowState", WindowState) = 2 Then WindowState = 2 Else WindowState = 0
ListFile = GetReg(AppKey + "ListFile", Path + "mpq_data.txt")
Mpq.DefaultMaxFiles = GetReg(AppKey + "DefaultMaxFiles", 1024)
LocaleID = GetReg(AppKey + "LocaleID", 0)
BuildRecentFileList
BuildToolsList
On Error GoTo 0
Mpq.SetLocale LocaleID
ReDim GlobalFileList(0) As String
#If InternalListing Then
If FileExists(ListFile) Then
    Open ListFile For Input As #1
    Do While Not EOF(1)
        ReDim Preserve GlobalFileList(UBound(GlobalFileList) + 1) As String
        Line Input #1, GlobalFileList(UBound(GlobalFileList))
    Loop
    Close #1
End If
#End If
FileName = Trim(Command)
If Left(FileName, 1) = Chr(34) Then FileName = Mid(FileName, 2)
If Right(FileName, 1) = Chr(34) Then FileName = Left(FileName, Len(FileName) - 1)
FileName = Trim(FileName)
If FileExists(FileName) Then
    CD.FileName = FileName
    Show
    OpenMpq
    Exit Sub
End If
ReDim FileList(0) As String
If Right(CurPath, 1) <> "\" Then CurPath = CurPath + "\"
sLine = Command
If Right(sLine, 1) <> " " Then sLine = sLine + " "
If sLine <> "" Then
    ReDim Param(0) As String
    For pNum = 1 To Len(sLine)
        If Mid(sLine, pNum, 1) = Chr(34) Then
            pNum = pNum + 1
            EndParam = InStr(pNum, sLine, Chr(34))
            If UBound(Param) = 1 Then ParamCutout = Mid(sLine, pNum - 1, (EndParam + 1) - (pNum - 1))
        Else
            EndParam = InStr(pNum, sLine, " ")
            If UBound(Param) = 1 Then ParamCutout = Mid(sLine, pNum, EndParam - pNum)
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
    Case "o", "open", "n", "new"
        Show
        If Mid(OldStartPath, 2, 1) = ":" Then ChDrive Left(OldStartPath, 1)
        ChDir OldStartPath
        RunMpq2kCommand sLine
    Case "a", "add", "e", "extract", "r", "ren", "rename", "m", "move", "d", "del", "delete", "f", "flush", "compact", "l", "list"
        If Mid(OldStartPath, 2, 1) = ":" Then ChDrive Left(OldStartPath, 1)
        ChDir OldStartPath
        CD.FileName = FullPath(CurDir, Param(2))
        sLine = Left(sLine, InStr(sLine, ParamCutout) - 1) + Mid(sLine, InStr(sLine, ParamCutout) + Len(ParamCutout))
        RunMpq2kCommand sLine
        If Mid(NewStartPath, 2, 1) = ":" Then ChDrive Left(NewStartPath, 1)
        ChDir NewStartPath
        Unload Me
    Case "s", "script"
        Show
        If Mid(OldStartPath, 2, 1) = ":" Then ChDrive Left(OldStartPath, 1)
        ChDir OldStartPath
        RunMpq2kCommand sLine
        If Mid(NewStartPath, 2, 1) = ":" Then ChDrive Left(NewStartPath, 1)
        ChDir NewStartPath
    End Select
End If
End Sub


Private Sub Form_Resize()
On Error Resume Next
If WindowState <> 1 Then
    List.Top = Toolbar.Height
    List.Width = ScaleWidth
    List.Height = ScaleHeight - List.Top - StatBar.Height - txtCommand.Height
    Label1.Top = List.Top + List.Height + (txtCommand.Height - Label1.Height) / 2
    txtCommand.Top = List.Top + List.Height
    txtCommand.Left = Label1.Width
    txtCommand.Width = ScaleWidth - cmdGo.Width - Label1.Width
    cmdGo.Top = txtCommand.Top
    cmdGo.Left = txtCommand.Left + txtCommand.Width
    mFilter.Width = ScaleWidth - mFilter.Left - Toolbar.Buttons.Item("List").Width
    Toolbar.Buttons.Item("filterspace").Width = mFilter.Width
End If
End Sub
Private Sub Form_Unload(Cancel As Integer)
Dim Path As String
Path = App.Path
If Right(Path, 1) <> "\" Then Path = Path + "\"
On Error Resume Next
If ExtractPathNum > -1 Then
    KillEx Path + "Temp_extract\" + CStr(ExtractPathNum) + "\", "*", 6, True
    RmDir Path + "Temp_extract\" + CStr(ExtractPathNum) + "\"
End If
If DirEx(Path + "Temp_extract\", "*", 6, True) = "" Or App.PrevInstance = False Then
    KillEx Path + "Temp_extract\", "*", 6, True
    RmDir Path + "Temp_extract\"
End If
If CopyPathNum > -1 Then
    KillEx Path + "Temp_copy\" + CStr(CopyPathNum) + "\", "*", 6, True
    RmDir Path + "Temp_copy\" + CStr(CopyPathNum) + "\"
End If
If DirEx(Path + "Temp_copy\", "*", 6, True) = "" Or App.PrevInstance = False Then
    KillEx Path + "Temp_copy\", "*", 6, True
    RmDir Path + "Temp_copy\"
End If
If GetReg(AppKey + "SaveWindowStatus", 1) > 0 Then
    NewKey AppKey
    NewKey AppKey + "Status\"
    If WindowState = 1 Then WindowState = 0
    SetReg AppKey + "Status\WindowState", WindowState, REG_DWORD
    WindowState = 0
    SetReg AppKey + "Status\WindowHeight", Height, REG_DWORD
    SetReg AppKey + "Status\WindowLeft", Left, REG_DWORD
    SetReg AppKey + "Status\WindowTop", Top, REG_DWORD
    SetReg AppKey + "Status\WindowWidth", Width, REG_DWORD
End If
If GetReg(AppKey + "StartupPathType", 0) <= 0 Then
    SetReg AppKey + "StartupPath", CurDir
End If
End
End Sub
Private Sub Label1_Click()
txtCommand.SetFocus
End Sub
Private Sub List_AfterLabelEdit(Cancel As Integer, NewString As String)
Dim Result As Long
If List.SelectedItem.Text <> NewString Then
    If GetReg(AppKey + "ShowConfirmation", 1) = 0 Then
        Result = vbYes
    Else
        Result = MsgBox("Rename file?", vbYesNo Or vbQuestion Or vbDefaultButton2, "WinMPQ")
    End If
    If Result = vbYes Then
        List.SelectedItem.Tag = NewString
        If Mpq.FileExists(CD.FileName, NewString) Then
            Mpq.DelFile CD.FileName, NewString
            Mpq.RenFile CD.FileName, List.SelectedItem.Text, NewString
            RemoveDuplicates
        Else
            Mpq.RenFile CD.FileName, List.SelectedItem.Text, NewString
        End If
        On Error Resume Next
        List.SelectedItem.Key = NewString
        On Error GoTo 0
        If FileExists(CD.FileName) Then MpqDate = FileDateTime(CD.FileName)
    Else
        Cancel = True
    End If
End If
ShowSelected
End Sub
Private Sub List_Click()
On Error GoTo NotSelected
List.SelectedItem.Tag = List.SelectedItem.Tag
On Error GoTo NotClick
List.HitTest(CX, CY).Tag = List.HitTest(CX, CY).Tag
On Error GoTo 0
ShowSelected
Exit Sub
NotClick:
List.SelectedItem.Selected = False
NotSelected:
ShowSelected
End Sub
Private Sub List_ColumnClick(ByVal ColumnHeader As ColumnHeader)
If List.SortKey = ColumnHeader.Index - 1 Then
    If List.SortOrder = 0 Then
        List.SortOrder = 1
    Else
        List.SortOrder = 0
    End If
Else
    List.SortOrder = 0
    List.SortKey = ColumnHeader.Index - 1
End If
End Sub
Private Sub List_DblClick()
Dim fNum As Long, Path As String, fName As String, fName2 As String, bNum As Long, AlreadyInList As Boolean, hMPQ As Long
On Error GoTo NotSelected
List.SelectedItem.Tag = List.SelectedItem.Tag
On Error GoTo NotClick
List.HitTest(CX, CY).Tag = List.HitTest(CX, CY).Tag
On Error GoTo 0
Path = App.Path
If Right(Path, 1) <> "\" Then Path = Path + "\"
Path = Path + "Temp_extract\"
If ExtractPathNum = -1 Then
    fNum = 0
    Do
    If DirEx(Path + CStr(fNum), "*", 6, True) = "" Then Exit Do
    fNum = fNum + 1
    Loop
    ExtractPathNum = fNum
End If
Path = Path + CStr(ExtractPathNum) + "\"
If Mpq.SFileOpenArchive(CD.FileName, 0, 0, hMPQ) = 0 Then Exit Sub
For fNum = 1 To List.ListItems.Count
    If List.ListItems.Item(fNum).Selected Then
        StatBar.Style = 1
        StatBar.SimpleText = "Extracting " + List.ListItems.Item(fNum).Tag + "..."
        MousePointer = 11
        Mpq.sGetFile hMPQ, List.ListItems.Item(fNum).Tag, Path, True
        If GetReg(AppKey + "CheckModDateTime", 1) > 0 Then
            For bNum = 1 To UBound(OpenFiles)
                If LCase(OpenFiles(bNum)) = LCase(List.ListItems.Item(fNum).Tag) Then
                    AlreadyInList = True
                    If FileExists(FullPath(Path, List.ListItems.Item(fNum).Tag)) Then OpenFileDates(bNum) = FileDateTime(FullPath(Path, List.ListItems.Item(fNum).Tag))
                    Exit For
                End If
            Next bNum
            If AlreadyInList = False Then
                ReDim Preserve OpenFiles(UBound(OpenFiles) + 1) As String, OpenFileDates(UBound(OpenFileDates) + 1) As Date
                OpenFiles(UBound(OpenFiles)) = List.ListItems.Item(fNum).Tag
                If FileExists(FullPath(Path, List.ListItems.Item(fNum).Tag)) Then OpenFileDates(UBound(OpenFileDates)) = FileDateTime(FullPath(Path, List.ListItems.Item(fNum).Tag))
            End If
        End If
        StatBar.Style = 1
        StatBar.SimpleText = "Opening " + List.ListItems.Item(fNum).Tag + "..."
        fName = List.ListItems.Item(fNum).Tag
        BuildPopup Path + fName, 0
        ExecuteFile Path + fName, 0
        If GetReg(AppKey + "CheckModDateTime", 1) > 0 Then Timer1.Enabled = True
    End If
Next fNum
Mpq.SFileCloseArchive hMPQ
StatBar.Style = 0
StatBar.SimpleText = ""
MousePointer = 0
Exit Sub
NotClick:
List.SelectedItem.Selected = False
NotSelected:
End Sub
Private Sub List_KeyPress(KeyAscii As Integer)
If KeyAscii = 13 Then List_DblClick
End Sub
Private Sub List_KeyUp(KeyCode As Integer, Shift As Integer)
If KeyCode = vbKeyDelete Then
    mnuMDelete_Click
ElseIf KeyCode = 93 Or ((Shift And vbShiftMask) And KeyCode = vbKeyF10) Then
    On Error GoTo NotSelected
    List.SelectedItem.Tag = List.SelectedItem.Tag
    On Error GoTo 0
    If List.SelectedItem.Selected = True Then
        BuildPopup List.SelectedItem.Tag, Shift
        PopupMenu mnuPopup, vbPopupMenuRightButton, List.Left + List.SelectedItem.Left + 12 * Screen.TwipsPerPixelX, List.Top + List.SelectedItem.Top + 16 * Screen.TwipsPerPixelY, mnuPItem(0)
    End If
End If
NotSelected:
End Sub
Private Sub List_MouseUp(Button As Integer, Shift As Integer, x As Single, y As Single)
CX = x
CY = y
If Button And vbRightButton Then
    On Error GoTo NotSelected
    List.SelectedItem.Tag = List.SelectedItem.Tag
    On Error GoTo NotClick
    List.HitTest(CX, CY).Tag = List.HitTest(CX, CY).Tag
    On Error GoTo 0
    BuildPopup List.SelectedItem.Tag, Shift
    PopupMenu mnuPopup, vbPopupMenuRightButton, , , mnuPItem(0)
End If
NotClick:
NotSelected:
End Sub
Private Sub List_OLECompleteDrag(Effect As Long)
List.Tag = ""
End Sub
Private Sub List_OLEDragDrop(Data As MSComctlLib.DataObject, Effect As Long, Button As Integer, Shift As Integer, x As Single, y As Single)
Dim Files() As String, ShortFiles() As String, bNum As Long, fNum As Long, Path As String, FolderFiles As String, Path2 As String, cNum As Long, FileFilter As String
Dim lIndex As Long, L1 As String, L2 As String, L3 As String, L4 As String, L5 As String, fSize As Long, cSize As Long, fFlags As Long, hMPQ As Long
If Data.GetFormat(ccCFFiles) <> True Then Exit Sub
For fNum = 1 To Data.Files.Count
    Path = Data.Files.Item(fNum)
    If Right(Path, 1) <> "\" Then Path = Path + "\"
    If IsDir(Path) Then
        Path = Path + "*"
        Data.Files.Remove fNum
        Data.Files.Add Path, fNum
    End If
Next fNum
Path = Data.Files.Item(1)
For bNum = 1 To Len(Path)
    If InStr(bNum, Path, "\") > 0 Then
        For fNum = 1 To Data.Files.Count
            If Left(Data.Files.Item(fNum), InStr(bNum, Path, "\")) <> Left(Path, InStr(bNum, Path, "\")) Then GoTo PathFound
        Next fNum
        bNum = InStr(bNum, Path, "\")
    Else
        Exit For
    End If
Next bNum
PathFound:
Path = Left(Path, bNum - 1)
ReDim Files(0) As String
Files(0) = Path
If Right(Path, 1) <> "\" Then Path = Path + "\"
ReDim Preserve Files(Data.Files.Count) As String
For bNum = 1 To Data.Files.Count
    Files(bNum) = Mid(Data.Files.Item(bNum), 1 + Len(Path))
    For fNum = 1 To Len(Files(bNum))
        If InStr(fNum, Files(bNum), "\") > 0 Then
            fNum = InStr(fNum, Files(bNum), "\")
        Else
            Exit For
        End If
    Next fNum
    FolderFiles = FolderFiles + DirEx(Path + Left(Files(bNum), fNum - 1), Mid(Files(bNum), fNum), 6, True)
Next bNum
If FolderFiles = "" Then Exit Sub
ReDim Preserve Files(0) As String
For bNum = 1 To Len(FolderFiles)
    ReDim Preserve Files(UBound(Files) + 1) As String
    If InStr(bNum, FolderFiles, vbCrLf) > 0 Then
        Files(UBound(Files)) = Mid(FolderFiles, bNum + Len(Path), InStr(bNum, FolderFiles, vbCrLf) - bNum - Len(Path))
        bNum = InStr(bNum, FolderFiles, vbCrLf) + 1
    Else
        Files(UBound(Files)) = Mid(FolderFiles, bNum + Len(Path))
        Exit For
    End If
Next bNum
FoldName.Show 1
If UBound(Files) > 1 Then
    ReDim ShortFiles(UBound(Files)) As String
    For bNum = 0 To UBound(Files)
        ShortFiles(bNum) = AddFolderName + Files(bNum)
    Next bNum
    If Right(Files(0), 1) <> "\" Then Files(0) = Files(0) + "\"
    For bNum = 1 To UBound(Files)
        Files(bNum) = FullPath(Files(0), Files(bNum))
    Next bNum
Else
    For bNum = 1 To Len(Files(1))
        If InStr(bNum, Files(1), "\") > 0 Then
            bNum = InStr(bNum, Files(1), "\")
        Else
            Exit For
        End If
    Next bNum
    ReDim ShortFiles(UBound(Files)) As String
    ShortFiles(1) = AddFolderName + Mid(Files(1), bNum)
    Files(1) = FullPath(Files(0), Files(1))
End If
If NewFile = True Then
    If FileExists(CD.FileName) Then Kill CD.FileName
    NewFile = False
End If
List.Sorted = False
FileFilter = mFilter
hMPQ = Mpq.mOpenMpq(CD.FileName)
If hMPQ = 0 Then
    StatBar.SimpleText = "Can't create archive " + CD.FileName
    Exit Sub
End If
For bNum = 1 To UBound(Files)
    StatBar.Style = 1
    StatBar.SimpleText = "Adding " + Files(bNum) + "..."
    MousePointer = 11
    If mnuMCNone.Checked Then
        Mpq.mAddFile hMPQ, Files(bNum), ShortFiles(bNum), 0
    ElseIf mnuMCStandard.Checked Then
        Mpq.mAddFile hMPQ, Files(bNum), ShortFiles(bNum), 1
    ElseIf mnuMCAMedium.Checked Then
        Mpq.mAddWavFile hMPQ, Files(bNum), ShortFiles(bNum), 0
    ElseIf mnuMCAHighest.Checked Then
        Mpq.mAddWavFile hMPQ, Files(bNum), ShortFiles(bNum), 1
    ElseIf mnuMCALowest.Checked Then
        Mpq.mAddWavFile hMPQ, Files(bNum), ShortFiles(bNum), 2
    ElseIf mnuMCAuto.Checked Then
        mAddAutoFile hMPQ, Files(bNum), ShortFiles(bNum)
    End If
    If FileExists(CD.FileName) Then MpqDate = FileDateTime(CD.FileName)
    mFilter.AddItem "*" + GetExtension(ShortFiles(bNum))
    For cNum = 1 To mFilter.ListCount - 1
        If LCase(mFilter.List(cNum)) = LCase(mFilter.List(cNum - 1)) Then
            mFilter.RemoveItem cNum
            Exit For
        End If
    Next cNum
Next bNum
Mpq.mCloseMpq hMPQ
If FileExists(CD.FileName) Then MpqDate = FileDateTime(CD.FileName)
If Mpq.SFileOpenArchive(CD.FileName, 0, 0, hMPQ) Then
    StatBar.SimpleText = "Adding files to listing... 0% complete"
    For bNum = 1 To UBound(Files)
        If MatchesFilter(ShortFiles(bNum), FileFilter) Then
            MpqAddToListing hMPQ, ShortFiles(bNum)
        End If
        On Error Resume Next
        StatBar.SimpleText = "Adding files to listing... " + CStr(Int((bNum / UBound(Files)) * 100)) + "% complete"
        On Error GoTo 0
    Next bNum
    Mpq.SFileCloseArchive hMPQ
End If
StatBar.Style = 0
StatBar.SimpleText = ""
MousePointer = 0
If MatchesFilter("(listfile)", FileFilter) Then
    AddToListing "(listfile)"
End If
mFilter = FileFilter
List.Sorted = True
RemoveDuplicates
ShowTotal
Cancel:
End Sub
Private Sub List_OLEDragOver(Data As MSComctlLib.DataObject, Effect As Long, Button As Integer, Shift As Integer, x As Single, y As Single, State As Integer)
If CD.FileName = "" Or Data.GetFormat(ccCFFiles) <> True Or List.Tag = "WinMPQ" Then
    Effect = ccOLEDropEffectNone
Else
    Effect = ccOLEDropEffectCopy
End If
End Sub
Private Sub List_OLESetData(Data As MSComctlLib.DataObject, DataFormat As Integer)
Dim fNum As Long, Path As String, fCount As Long, FirstFile As String, hMPQ As Long
Path = App.Path
If Right(Path, 1) <> "\" Then Path = Path + "\"
Path = Path + "Temp_copy\"
If CopyPathNum = -1 Then
    fNum = 0
    Do
    If DirEx(Path + CStr(fNum), "*", 6, True) = "" Then Exit Do
    fNum = fNum + 1
    Loop
    CopyPathNum = fNum
End If
Path = Path + CStr(CopyPathNum) + "\"
KillEx Path, "*", 6, True
fCount = 0
If Mpq.SFileOpenArchive(CD.FileName, 0, 0, hMPQ) = 0 Then Exit Sub
For fNum = 1 To List.ListItems.Count
    If List.ListItems.Item(fNum).Selected Then
        StatBar.Style = 1
        StatBar.SimpleText = "Extracting " + List.ListItems.Item(fNum).Tag + "..."
        MousePointer = 11
        Mpq.sGetFile hMPQ, List.ListItems.Item(fNum).Tag, Path, True
        If GetReg(AppKey + "UseDragDropWildcards", 1) = 0 Then
            Data.Files.Add Path + List.ListItems.Item(fNum).Tag
        End If
        fCount = fCount + 1
        If fCount = 1 Then FirstFile = Path + List.ListItems.Item(fNum).Tag
    End If
Next fNum
Mpq.SFileCloseArchive hMPQ
StatBar.Style = 0
StatBar.SimpleText = ""
MousePointer = 0
If GetReg(AppKey + "UseDragDropWildcards", 1) <> 0 And fCount > 1 Then
    Data.Files.Add Path + "*"
ElseIf fCount = 1 Then
    Data.Files.Add FirstFile
End If
End Sub
Private Sub List_OLEStartDrag(Data As MSComctlLib.DataObject, AllowedEffects As Long)
Data.SetData , ccCFFiles
AllowedEffects = ccOLEDropEffectCopy
List.Tag = "WinMPQ"
End Sub
Private Sub mFilter_KeyPress(KeyAscii As Integer)
If KeyAscii = 13 And Toolbar.Buttons.Item("List").Enabled Then
    If NewFile = False Then OpenMpq
End If
End Sub
Private Sub mnuFExit_Click()
Unload Me
End Sub
Private Sub mnuFile_Click()
If FileExists(CD.FileName) Then mnuFReopen.Enabled = True Else mnuFReopen.Enabled = False
End Sub

Private Sub mnuFRecent_Click(Index As Integer)
Dim OldFileName As String
OldFileName = CD.FileName
CD.FileName = mnuFRecent(Index).Tag
If FileExists(CD.FileName) = False Then
    CD.FileName = OldFileName
    MsgBox "The file " + Chr(34) + mnuFRecent(Index).Tag + Chr(34) + " does not exist.", vbExclamation, "WinMPQ"
    DelRecentFile mnuFRecent(Index).Tag
    Exit Sub
End If
OpenMpq
If CD.FileName = "" Then
    CD.FileName = OldFileName
    DelRecentFile mnuFRecent(Index).Tag
End If
End Sub
Private Sub mnuFReopen_Click()
OpenMpq
End Sub

Private Sub mnuFScript_Click()
Dim OldFileName As String, OldPath As String
CD.Flags = &H1000 Or &H4 Or &H2
CD.Filter = "All Files (*.*)|*.*"
OldFileName = CD.FileName
OldPath = CurDir
If ShowOpen(CD) = False Then GoTo Cancel
StatBar.Style = 1
StatBar.SimpleText = "Running script " + CD.FileName + "..."
MousePointer = 11
RunScript CD.FileName
StatBar.Style = 0
StatBar.SimpleText = ""
MousePointer = 0
CD.FileName = OldFileName
Cancel:
If Mid(OldPath, 2, 1) = ":" Then ChDrive Left(OldPath, 1)
ChDir OldPath
End Sub
Private Sub mnuHAbout_Click()
About.Show 1
End Sub
Private Sub mnuHReadme_Click()
Dim Path As String
Path = App.Path
If Right(Path, 1) <> "\" Then Path = Path + "\"
If FileExists(Path + "WinMPQ.rtf") Then
    ShellExecute hWnd, vbNullString, Path + "WinMPQ.rtf", vbNullString, vbNullString, 1
Else
    MsgBox "Could not find WinMPQ.rtf!", vbCritical, "WinMPQ"
End If
End Sub
Private Sub mnuMAdd_Click()
Dim Files() As String, ShortFiles() As String, bNum As Long, OldFileName As String, fNum As Long, cNum As Long, FileFilter As String
Dim lIndex As Long, L1 As String, L2 As String, L3 As String, L4 As String, L5 As String, fSize As Long, cSize As Long, fFlags As Long, hMPQ As Long
CD.Flags = OFN_EXPLORER Or &H1000 Or &H200 Or &H4 Or &H2
CD.Filter = "All Files (*.*)|*.*"
OldFileName = CD.FileName
If ShowOpen(CD) = False Then GoTo Cancel
ReDim Files(0) As String
bNum = 1
If InStr(1, CD.FileName, Chr(0)) > 0 Then
    Files(0) = Mid(CD.FileName, 1, InStr(1, CD.FileName, Chr(0)) - 1)
    bNum = InStr(1, CD.FileName, Chr(0)) + 1
Else
    Files(0) = Mid(CD.FileName, 1)
End If
For bNum = bNum To Len(CD.FileName)
    ReDim Preserve Files(UBound(Files) + 1) As String
    If InStr(bNum, CD.FileName, Chr(0)) > 0 Then
        Files(UBound(Files)) = Mid(CD.FileName, bNum, InStr(bNum, CD.FileName, Chr(0)) - bNum)
        bNum = InStr(bNum, CD.FileName, Chr(0))
    Else
        Files(UBound(Files)) = Mid(CD.FileName, bNum)
        Exit For
    End If
Next bNum
CD.FileName = OldFileName
FoldName.Show 1
If UBound(Files) > 1 Then
    ReDim ShortFiles(UBound(Files)) As String
    For bNum = 0 To UBound(Files)
        ShortFiles(bNum) = AddFolderName + Files(bNum)
    Next bNum
    If Right(Files(0), 1) <> "\" Then Files(0) = Files(0) + "\"
    For bNum = 1 To UBound(Files)
        Files(bNum) = FullPath(Files(0), Files(bNum))
    Next bNum
Else
    For bNum = 1 To Len(Files(1))
        If InStr(bNum, Files(1), "\") > 0 Then
            bNum = InStr(bNum, Files(1), "\")
        Else
            Exit For
        End If
    Next bNum
    ReDim ShortFiles(UBound(Files)) As String
    ShortFiles(1) = AddFolderName + Mid(Files(1), bNum)
    Files(1) = FullPath(Files(0), Files(1))
End If
If NewFile = True Then
    If FileExists(CD.FileName) Then Kill CD.FileName
    NewFile = False
End If
List.Sorted = False
FileFilter = mFilter
hMPQ = Mpq.mOpenMpq(CD.FileName)
If hMPQ = 0 Then
    StatBar.SimpleText = "Can't create archive " + CD.FileName
    Exit Sub
End If
For bNum = 1 To UBound(Files)
    StatBar.Style = 1
    StatBar.SimpleText = "Adding " + Files(bNum) + "..."
    MousePointer = 11
    If mnuMCNone.Checked Then
        Mpq.mAddFile hMPQ, Files(bNum), ShortFiles(bNum), 0
    ElseIf mnuMCStandard.Checked Then
        Mpq.mAddFile hMPQ, Files(bNum), ShortFiles(bNum), 1
    ElseIf mnuMCAMedium.Checked Then
        Mpq.mAddWavFile hMPQ, Files(bNum), ShortFiles(bNum), 0
    ElseIf mnuMCAHighest.Checked Then
        Mpq.mAddWavFile hMPQ, Files(bNum), ShortFiles(bNum), 1
    ElseIf mnuMCALowest.Checked Then
        Mpq.mAddWavFile hMPQ, Files(bNum), ShortFiles(bNum), 2
    ElseIf mnuMCAuto.Checked Then
        mAddAutoFile hMPQ, Files(bNum), ShortFiles(bNum)
    End If
    If FileExists(CD.FileName) Then MpqDate = FileDateTime(CD.FileName)
    mFilter.AddItem "*" + GetExtension(ShortFiles(bNum))
    For cNum = 1 To mFilter.ListCount - 1
        If LCase(mFilter.List(cNum)) = LCase(mFilter.List(cNum - 1)) Then
            mFilter.RemoveItem cNum
            Exit For
        End If
    Next cNum
Next bNum
Mpq.mCloseMpq hMPQ
If FileExists(CD.FileName) Then MpqDate = FileDateTime(CD.FileName)
If Mpq.SFileOpenArchive(CD.FileName, 0, 0, hMPQ) Then
    StatBar.SimpleText = "Adding files to listing... 0% complete"
    For bNum = 1 To UBound(Files)
        If MatchesFilter(ShortFiles(bNum), FileFilter) Then
            MpqAddToListing hMPQ, ShortFiles(bNum)
        End If
        On Error Resume Next
        StatBar.SimpleText = "Adding files to listing... " + CStr(Int((bNum / UBound(Files)) * 100)) + "% complete"
        On Error GoTo 0
    Next bNum
    Mpq.SFileCloseArchive hMPQ
End If
StatBar.Style = 0
StatBar.SimpleText = ""
MousePointer = 0
If MatchesFilter("(listfile)", FileFilter) Then
    AddToListing "(listfile)"
End If
mFilter = FileFilter
List.Sorted = True
RemoveDuplicates
ShowTotal
Cancel:
End Sub
Private Sub mnuMAddFolder_Click()
Dim Files() As String, ShortFiles() As String, bNum As Long, fNum As Long, Path As String, FolderFiles As String, cNum As Long, FileFilter As String, hMPQ As Long
Dim lIndex As Long, L1 As String, L2 As String, L3 As String, L4 As String, L5 As String, fSize As Long, cSize As Long, fFlags As Long
Path = PathInputBox(PathInput, "Folder to add files from...", CurDir)
If Path = "" Then GoTo Cancel
FolderFiles = DirEx(Path, "*", 6, True)
If FolderFiles = "" Then Exit Sub
ReDim Files(0) As String
Files(0) = Path
If Right(Path, 1) <> "\" Then Path = Path + "\"
For bNum = 1 To Len(FolderFiles)
    ReDim Preserve Files(UBound(Files) + 1) As String
    If InStr(bNum, FolderFiles, vbCrLf) > 0 Then
        Files(UBound(Files)) = Mid(FolderFiles, bNum + Len(Path), InStr(bNum, FolderFiles, vbCrLf) - bNum - Len(Path))
        bNum = InStr(bNum, FolderFiles, vbCrLf) + 1
    Else
        Files(UBound(Files)) = Mid(FolderFiles, bNum + Len(Path))
        Exit For
    End If
Next bNum
FoldName.Show 1
If UBound(Files) > 1 Then
    ReDim ShortFiles(UBound(Files)) As String
    For bNum = 0 To UBound(Files)
        ShortFiles(bNum) = AddFolderName + Files(bNum)
    Next bNum
    If Right(Files(0), 1) <> "\" Then Files(0) = Files(0) + "\"
    For bNum = 1 To UBound(Files)
        Files(bNum) = FullPath(Files(0), Files(bNum))
    Next bNum
Else
    For bNum = 1 To Len(Files(1))
        If InStr(bNum, Files(1), "\") > 0 Then
            bNum = InStr(bNum, Files(1), "\")
        Else
            Exit For
        End If
    Next bNum
    ReDim ShortFiles(UBound(Files)) As String
    ShortFiles(1) = AddFolderName + Mid(Files(1), bNum)
    Files(1) = FullPath(Files(0), Files(1))
End If
If NewFile = True Then
    If FileExists(CD.FileName) Then Kill CD.FileName
    NewFile = False
End If
List.Sorted = False
FileFilter = mFilter
hMPQ = Mpq.mOpenMpq(CD.FileName)
If hMPQ = 0 Then
    StatBar.SimpleText = "Can't create archive " + CD.FileName
    Exit Sub
End If
For bNum = 1 To UBound(Files)
    StatBar.Style = 1
    StatBar.SimpleText = "Adding " + Files(bNum) + "..."
    MousePointer = 11
    If mnuMCNone.Checked Then
        Mpq.mAddFile hMPQ, Files(bNum), ShortFiles(bNum), 0
    ElseIf mnuMCStandard.Checked Then
        Mpq.mAddFile hMPQ, Files(bNum), ShortFiles(bNum), 1
    ElseIf mnuMCAMedium.Checked Then
        Mpq.mAddWavFile hMPQ, Files(bNum), ShortFiles(bNum), 0
    ElseIf mnuMCAHighest.Checked Then
        Mpq.mAddWavFile hMPQ, Files(bNum), ShortFiles(bNum), 1
    ElseIf mnuMCALowest.Checked Then
        Mpq.mAddWavFile hMPQ, Files(bNum), ShortFiles(bNum), 2
    ElseIf mnuMCAuto.Checked Then
        mAddAutoFile hMPQ, Files(bNum), ShortFiles(bNum)
    End If
    If FileExists(CD.FileName) Then MpqDate = FileDateTime(CD.FileName)
    mFilter.AddItem "*" + GetExtension(ShortFiles(bNum))
    For cNum = 1 To mFilter.ListCount - 1
        If LCase(mFilter.List(cNum)) = LCase(mFilter.List(cNum - 1)) Then
            mFilter.RemoveItem cNum
            Exit For
        End If
    Next cNum
Next bNum
Mpq.mCloseMpq hMPQ
If FileExists(CD.FileName) Then MpqDate = FileDateTime(CD.FileName)
If Mpq.SFileOpenArchive(CD.FileName, 0, 0, hMPQ) Then
    StatBar.SimpleText = "Adding files to listing... 0% complete"
    For bNum = 1 To UBound(Files)
        If MatchesFilter(ShortFiles(bNum), FileFilter) Then
            MpqAddToListing hMPQ, ShortFiles(bNum)
        End If
        On Error Resume Next
        StatBar.SimpleText = "Adding files to listing... " + CStr(Int((bNum / UBound(Files)) * 100)) + "% complete"
        On Error GoTo 0
    Next bNum
    Mpq.SFileCloseArchive hMPQ
End If
StatBar.Style = 0
StatBar.SimpleText = ""
MousePointer = 0
If MatchesFilter("(listfile)", FileFilter) Then
    AddToListing "(listfile)"
End If
mFilter = FileFilter
List.Sorted = True
RemoveDuplicates
ShowTotal
Cancel:
End Sub
Private Sub mnuMCAHighest_Click()
mnuMCNone.Checked = False
mnuMCStandard.Checked = False
mnuMCALowest.Checked = False
mnuMCAMedium.Checked = False
mnuMCAHighest.Checked = True
mnuMCAuto.Checked = False
End Sub
Private Sub mnuMCALowest_Click()
mnuMCNone.Checked = False
mnuMCStandard.Checked = False
mnuMCALowest.Checked = True
mnuMCAMedium.Checked = False
mnuMCAHighest.Checked = False
mnuMCAuto.Checked = False
End Sub


Private Sub mnuMCAMedium_Click()
mnuMCNone.Checked = False
mnuMCStandard.Checked = False
mnuMCALowest.Checked = False
mnuMCAMedium.Checked = True
mnuMCAHighest.Checked = False
mnuMCAuto.Checked = False
End Sub

Private Sub mnuMCAuto_Click()
mnuMCNone.Checked = False
mnuMCStandard.Checked = False
mnuMCALowest.Checked = False
mnuMCAMedium.Checked = False
mnuMCAHighest.Checked = False
mnuMCAuto.Checked = True
End Sub

Private Sub mnuMCNone_Click()
mnuMCNone.Checked = True
mnuMCStandard.Checked = False
mnuMCALowest.Checked = False
mnuMCAMedium.Checked = False
mnuMCAHighest.Checked = False
mnuMCAuto.Checked = False
End Sub

Private Sub mnuMCompact_Click()
Dim fNum As Long, Result As Long
If GetReg(AppKey + "ShowConfirmation", 1) = 0 Then
    Result = vbYes
Else
    Result = MsgBox("Uncompressed files with an 'X' in the attributes column and are not" + vbCrLf + "listed in (listfile) may be corrupted or deleted by the compacting process." + vbCrLf + "(Note:  These files are fairly rare)  Continue with compact?", vbYesNo Or vbQuestion Or vbDefaultButton2, "WinMPQ")
End If
If Result = vbYes Then
    StatBar.Style = 1
    StatBar.SimpleText = "Compacting " + CD.FileName + "..."
    MousePointer = 11
    Mpq.CompactMpq CD.FileName
    If FileExists(CD.FileName) Then MpqDate = FileDateTime(CD.FileName)
    StatBar.Style = 0
    StatBar.SimpleText = ""
    MousePointer = 0
    OpenMpq
End If
End Sub
Private Sub mnuMCStandard_Click()
mnuMCNone.Checked = False
mnuMCStandard.Checked = True
mnuMCALowest.Checked = False
mnuMCAMedium.Checked = False
mnuMCAHighest.Checked = False
End Sub
Private Sub mnuMDelete_Click()
Dim fNum As Long, Result As Long
On Error GoTo NotSelected
List.SelectedItem.Tag = List.SelectedItem.Tag
On Error GoTo 0
For fNum = 1 To List.ListItems.Count
    If List.ListItems.Item(fNum).Selected Then
        GoTo FileSelected
    End If
Next fNum
GoTo NotSelected
FileSelected:
    If GetReg(AppKey + "ShowConfirmation", 1) = 0 Then
        Result = vbYes
    Else
        Result = MsgBox("Delete file(s)?", vbYesNo Or vbQuestion Or vbDefaultButton2, "WinMPQ")
    End If
    If Result = vbYes Then
        fNum = 1
        Do While fNum <= List.ListItems.Count
            If List.ListItems.Item(fNum).Selected Then
                StatBar.Style = 1
                StatBar.SimpleText = "Deleting " + List.ListItems.Item(fNum).Tag + "..."
                MousePointer = 11
                Mpq.DelFile CD.FileName, List.ListItems.Item(fNum).Tag
                If FileExists(CD.FileName) Then MpqDate = FileDateTime(CD.FileName)
                List.ListItems.Remove (fNum)
                fNum = fNum - 1
            End If
            fNum = fNum + 1
        Loop
    End If
    StatBar.Style = 0
    StatBar.SimpleText = ""
    MousePointer = 0
    ShowSelected
    ShowTotal
Exit Sub
NotSelected:
MsgBox "No files are selected.", , "WinMPQ"
End Sub
Private Sub mnuMExtract_Click()
Dim fNum As Long, Path As String, Result As Long, hMPQ As Long
On Error GoTo NotSelected
List.SelectedItem.Tag = List.SelectedItem.Tag
On Error GoTo 0
For fNum = 1 To List.ListItems.Count
    If List.ListItems.Item(fNum).Selected Then
        GoTo FileSelected
    End If
Next fNum
GoTo NotSelected
FileSelected:
Path = PathInputBox(PathInput, "Extract file(s) to...", CurDir)
If Path = "" Then Exit Sub
If Right(Path, 1) <> "\" Then Path = Path + "\"
If Mpq.SFileOpenArchive(CD.FileName, 0, 0, hMPQ) = 0 Then Exit Sub
For fNum = 1 To List.ListItems.Count
    If List.ListItems.Item(fNum).Selected Then
        StatBar.Style = 1
        StatBar.SimpleText = "Extracting " + List.ListItems.Item(fNum).Tag + "..."
        MousePointer = 11
        Mpq.sGetFile hMPQ, List.ListItems.Item(fNum).Tag, Path, True
    End If
Next fNum
Mpq.SFileCloseArchive hMPQ
StatBar.Style = 0
StatBar.SimpleText = ""
MousePointer = 0
Exit Sub
NotSelected:
If GetReg(AppKey + "ShowConfirmation", 1) = 0 Then
    Result = vbYes
Else
    Result = MsgBox("No files are selected." + vbCrLf + "Extract all listed files?", vbYesNo Or vbQuestion Or vbDefaultButton2, "WinMPQ")
End If
If Result = vbYes Then
    Path = PathInputBox(PathInput, "Extract file(s) to...", CurDir)
    If Path = "" Then Exit Sub
    If Right(Path, 1) <> "\" Then Path = Path + "\"
    If Mpq.SFileOpenArchive(CD.FileName, 0, 0, hMPQ) = 0 Then Exit Sub
    For fNum = 1 To List.ListItems.Count
        StatBar.Style = 1
        StatBar.SimpleText = "Extracting " + List.ListItems.Item(fNum).Tag + "..."
        MousePointer = 11
        Mpq.sGetFile hMPQ, List.ListItems.Item(fNum).Tag, Path, True
    Next fNum
    Mpq.SFileCloseArchive hMPQ
    StatBar.Style = 0
    StatBar.SimpleText = ""
    MousePointer = 0
End If
End Sub
Private Sub mnuFNew_Click()
Dim TItem As Menu
CD.Flags = &H1000 Or &H4 Or &H2
CD.DefaultExt = "mpq"
CD.Filter = "Mpq Archives (*.mpq;*.exe;*.snp;*.scm;*.scx;*.w3m)|*.mpq;*.exe;*.snp;*.scm;*.scx;*.w3m|All Files (*.*)|*.*"
If ShowSave(CD) = False Then GoTo Cancel
ReDim FileList(0) As String
List.ListItems.Clear
ShowSelected
ShowTotal
NewFile = True
ReDim OpenFiles(0) As String, OpenFileDates(0) As Date
mnuMpq.Enabled = True
For Each TItem In mnuTItem
    TItem.Enabled = True
Next TItem
Toolbar.Buttons.Item("Add").Enabled = True
Toolbar.Buttons.Item("Add Folder").Enabled = True
Toolbar.Buttons.Item("Extract").Enabled = True
Toolbar.Buttons.Item("Compact").Enabled = True
Toolbar.Buttons.Item("List").Enabled = True
Caption = "WinMPQ - " + CD.FileTitle
AddRecentFile CD.FileName
Cancel:
End Sub
Private Sub mnuFOpen_Click()
Dim OldFileName As String
CD.Flags = &H1000 Or &H4 Or &H2
CD.Filter = "Mpq Archives (*.mpq;*.exe;*.snp;*.scm;*.scx;*.w3m)|*.mpq;*.exe;*.snp;*.scm;*.scx;*.w3m|All Files (*.*)|*.*"
OldFileName = CD.FileName
If ShowOpen(CD) = False Then GoTo Cancel
OpenMpq
If CD.FileName = "" Then CD.FileName = OldFileName
Cancel:
End Sub
Private Sub mnuMRename_Click()
List.StartLabelEdit
End Sub
Private Sub mnuMSaveList_Click()
Dim fNum As Long, fList As String, OldFileName As String
CD.Flags = &H1000 Or &H4 Or &H2
CD.DefaultExt = "txt"
CD.Filter = "Text File (*.txt)|*.txt|All Files (*.*)|*.*"
OldFileName = CD.FileName
CD.FileName = CD.FileName + ".txt"
If ShowSave(CD) = False Then GoTo Cancel
StatBar.Style = 1
StatBar.SimpleText = "Creating list..."
MousePointer = 11
For fNum = 1 To List.ListItems.Count
    fList = fList + List.ListItems.Item(fNum).Tag + vbCrLf
Next fNum
fNum = FreeFile
Open CD.FileName For Binary As #fNum
Put #fNum, 1, fList
Close #fNum
Cancel:
CD.FileName = OldFileName
StatBar.Style = 0
StatBar.SimpleText = ""
MousePointer = 0
End Sub
Private Sub mnuOptions_Click()
Options.Show 1
End Sub
Private Sub mnuPDelete_Click()
mnuMDelete_Click
End Sub
Private Sub mnuPExtract_Click()
mnuMExtract_Click
End Sub
Private Sub mnuPItem_Click(Index As Integer)
Dim fNum As Long, Path As String, fName As String, fName2 As String, bNum As Long, AlreadyInList As Boolean, hMPQ As Long
Path = App.Path
If Right(Path, 1) <> "\" Then Path = Path + "\"
Path = Path + "Temp_extract\"
If ExtractPathNum = -1 Then
    fNum = 0
    Do
    If DirEx(Path + CStr(fNum), "*", 6, True) = "" Then Exit Do
    fNum = fNum + 1
    Loop
    ExtractPathNum = fNum
End If
Path = Path + CStr(ExtractPathNum) + "\"
If Mpq.SFileOpenArchive(CD.FileName, 0, 0, hMPQ) = 0 Then Exit Sub
For fNum = 1 To List.ListItems.Count
    If List.ListItems.Item(fNum).Selected Then
        StatBar.Style = 1
        StatBar.SimpleText = "Extracting " + List.ListItems.Item(fNum).Tag + "..."
        MousePointer = 11
        Mpq.sGetFile hMPQ, List.ListItems.Item(fNum).Tag, Path, True
        If GetReg(AppKey + "CheckModDateTime", 1) > 0 Then
            For bNum = 1 To UBound(OpenFiles)
                If LCase(OpenFiles(bNum)) = LCase(List.ListItems.Item(fNum).Tag) Then
                    AlreadyInList = True
                    If FileExists(FullPath(Path, List.ListItems.Item(fNum).Tag)) Then OpenFileDates(bNum) = FileDateTime(FullPath(Path, List.ListItems.Item(fNum).Tag))
                    Exit For
                End If
            Next bNum
            If AlreadyInList = False Then
                ReDim Preserve OpenFiles(UBound(OpenFiles) + 1) As String, OpenFileDates(UBound(OpenFileDates) + 1) As Date
                OpenFiles(UBound(OpenFiles)) = List.ListItems.Item(fNum).Tag
                If FileExists(FullPath(Path, List.ListItems.Item(fNum).Tag)) Then OpenFileDates(UBound(OpenFileDates)) = FileDateTime(FullPath(Path, List.ListItems.Item(fNum).Tag))
            End If
        End If
        StatBar.Style = 1
        StatBar.SimpleText = "Opening " + List.ListItems.Item(fNum).Tag + "..."
        fName = List.ListItems.Item(fNum).Tag
        ExecuteFile Path + fName, Index
        If GetReg(AppKey + "CheckModDateTime", 1) > 0 Then Timer1.Enabled = True
    End If
Next fNum
Mpq.SFileCloseArchive hMPQ
StatBar.Style = 0
StatBar.SimpleText = ""
MousePointer = 0
End Sub
Private Sub mnuPRename_Click()
mnuMRename_Click
End Sub

Private Sub mnuTAdd_Click()
ToolList.Show 1
BuildToolsList
End Sub
Private Sub mnuTItem_Click(Index As Integer)
Dim Param As String, bNum As Long, FileName As String, Path As String, fNum As Long, AlreadyInList As Boolean, UseFile As Boolean, NewParam As String, FileNameList As String, hMPQ As Long
Param = mnuTItem(Index).Tag
On Error GoTo NoProgram
If Param = "" Then Err.Raise 53
On Error GoTo 0
Do
    If InStr(1, Param, "%mpq", 1) Then
        bNum = InStr(1, Param, "%mpq", 1)
        Param = Left(Param, bNum - 1) + CD.FileName + Mid(Param, bNum + 4)
    End If
Loop While InStr(1, Param, "%mpq", 1)
NewParam = Param
On Error GoTo NotSelected
List.SelectedItem.Tag = List.SelectedItem.Tag
On Error GoTo 0
If List.SelectedItem.Selected Then FileName = List.SelectedItem.Tag
NotSelected:
If FileName <> "" And (InStr(Param, "%1") Or (InStr(Param, "%1") = 0 And InStr(1, mnuTItem(Index).Tag, "%mpq", 1) = 0)) Then
    Path = App.Path
    If Right(Path, 1) <> "\" Then Path = Path + "\"
    Path = Path + "Temp_extract\"
    If ExtractPathNum = -1 Then
        fNum = 0
        Do
        If DirEx(Path + CStr(fNum), "*", 6, True) = "" Then Exit Do
        fNum = fNum + 1
        Loop
        ExtractPathNum = fNum
    End If
    Path = Path + CStr(ExtractPathNum) + "\"
    If Mpq.SFileOpenArchive(CD.FileName, 0, 0, hMPQ) = 0 Then Exit Sub
    For fNum = 1 To List.ListItems.Count
        If List.ListItems.Item(fNum).Selected Then
            StatBar.Style = 1
            StatBar.SimpleText = "Extracting " + List.ListItems.Item(fNum).Tag + "..."
            MousePointer = 11
            Mpq.sGetFile hMPQ, List.ListItems.Item(fNum).Tag, Path, True
            If GetReg(AppKey + "CheckModDateTime", 1) > 0 Then
                For bNum = 1 To UBound(OpenFiles)
                    If LCase(OpenFiles(bNum)) = LCase(List.ListItems.Item(fNum).Tag) Then
                        AlreadyInList = True
                        If FileExists(FullPath(Path, List.ListItems.Item(fNum).Tag)) Then OpenFileDates(bNum) = FileDateTime(FullPath(Path, List.ListItems.Item(fNum).Tag))
                        Exit For
                    End If
                Next bNum
                If AlreadyInList = False Then
                    ReDim Preserve OpenFiles(UBound(OpenFiles) + 1) As String, OpenFileDates(UBound(OpenFileDates) + 1) As Date
                    OpenFiles(UBound(OpenFiles)) = List.ListItems.Item(fNum).Tag
                    If FileExists(FullPath(Path, List.ListItems.Item(fNum).Tag)) Then OpenFileDates(UBound(OpenFileDates)) = FileDateTime(FullPath(Path, List.ListItems.Item(fNum).Tag))
                End If
            End If
            StatBar.Style = 1
            StatBar.SimpleText = "Opening " + List.ListItems.Item(fNum).Tag + "..."
            FileName = FullPath(Path, List.ListItems.Item(fNum).Tag)
            UseFile = True
            Param = NewParam
            Do
                If InStr(Param, "%1") = 0 And InStr(1, mnuTItem(Index).Tag, "%mpq", 1) = 0 Then
                    If FileName <> "" Then
                        Param = Param + " " + FileName
                    End If
                ElseIf InStr(Param, Chr(34) + "%1" + Chr(34)) Then
                    bNum = InStr(Param, Chr(34) + "%1" + Chr(34))
                    If FileName <> "" Then
                        Param = Left(Param, bNum - 1) + Chr(34) + FileName + Chr(34) + Mid(Param, bNum + 4)
                    Else
                        Param = Left(Param, bNum - 1) + Mid(Param, bNum + 4)
                    End If
                ElseIf InStr(Param, "%1") Then
                    bNum = InStr(Param, "%1")
                    If FileName <> "" Then
                        Param = Left(Param, bNum - 1) + FileName + Mid(Param, bNum + 2)
                    Else
                        Param = Left(Param, bNum - 1) + Mid(Param, bNum + 2)
                    End If
                End If
            Loop While InStr(Param, "%1")
            On Error GoTo NoProgram
            Shell Param, 1
            On Error GoTo 0
            If GetReg(AppKey + "CheckModDateTime", 1) > 0 Then Timer1.Enabled = True
        End If
    Next fNum
    Mpq.SFileCloseArchive hMPQ
ElseIf InStr(1, mnuTItem(Index).Tag, "%mpq", 1) Then
    If FileExists(CD.FileName) Then MpqDate = FileDateTime(CD.FileName)
    On Error GoTo NoProgram
    Shell Param, 1
    On Error GoTo 0
    Timer1.Enabled = True
Else
    MsgBox "No files are selected.", , "WinMPQ"
End If
If FileName <> "" Then
    StatBar.Style = 0
    StatBar.SimpleText = ""
    MousePointer = 0
End If
Exit Sub
NoProgram:
If Err.Number = 53 Then MsgBox "No program is assigned for this action.", , "WinMPQ"
End Sub
Private Sub Timer1_Timer()
Dim fNum As Long, Path As String, Result As Long, bNum As Long
If Screen.ActiveForm.Name <> "MpqEx" Then Exit Sub
Path = App.Path
If Right(Path, 1) <> "\" Then Path = Path + "\"
Path = Path + "Temp_extract\"
Path = Path + CStr(ExtractPathNum) + "\"
For fNum = 1 To UBound(OpenFiles)
    If FileExists(FullPath(Path, OpenFiles(fNum))) Then
        If FileDateTime(FullPath(Path, OpenFiles(fNum))) > OpenFileDates(fNum) Then
            If GetReg(AppKey + "ShowConfirmation", 1) = 0 Then
                Result = vbYes
            Else
            Result = MsgBox("File " + OpenFiles(fNum) + " has been changed since it was extracted." + vbCrLf + vbCrLf + "Update archive with this file?", vbYesNo Or vbInformation, "WinMPQ")
            End If
            OpenFileDates(fNum) = FileDateTime(FullPath(Path, OpenFiles(fNum)))
            If Result = vbYes Then
                List.Sorted = False
                StatBar.Style = 1
                StatBar.SimpleText = "Adding " + OpenFiles(fNum) + "..."
                MousePointer = 11
                If mnuMCNone.Checked Then
                    Mpq.AddFile CD.FileName, FullPath(Path, OpenFiles(fNum)), OpenFiles(fNum), 0
                ElseIf mnuMCStandard.Checked Then
                    Mpq.AddFile CD.FileName, FullPath(Path, OpenFiles(fNum)), OpenFiles(fNum), 1
                ElseIf mnuMCAMedium.Checked Then
                    Mpq.AddWavFile CD.FileName, FullPath(Path, OpenFiles(fNum)), OpenFiles(fNum), 0
                ElseIf mnuMCAHighest.Checked Then
                    Mpq.AddWavFile CD.FileName, FullPath(Path, OpenFiles(fNum)), OpenFiles(fNum), 1
                ElseIf mnuMCALowest.Checked Then
                    Mpq.AddWavFile CD.FileName, FullPath(Path, OpenFiles(fNum)), OpenFiles(fNum), 2
                ElseIf mnuMCAuto.Checked Then
                    AddAutoFile CD.FileName, FullPath(Path, OpenFiles(fNum)), OpenFiles(fNum)
                End If
                If FileExists(CD.FileName) Then MpqDate = FileDateTime(CD.FileName)
                AddToListing OpenFiles(fNum)
                StatBar.Style = 0
                StatBar.SimpleText = ""
                MousePointer = 0
                List.Sorted = True
                RemoveDuplicates
                ShowTotal
            End If
        End If
    Else
        For bNum = fNum To UBound(OpenFiles) - 1
            OpenFiles(bNum) = OpenFiles(bNum + 1)
            OpenFileDates(bNum) = OpenFileDates(bNum + 1)
        Next bNum
        ReDim Preserve OpenFiles(UBound(OpenFiles) - 1) As String, OpenFileDates(UBound(OpenFileDates) - 1) As Date
        fNum = fNum - 1
        If UBound(OpenFiles) = 0 Then Timer1.Enabled = False
    End If
    If fNum >= UBound(OpenFiles) Then Exit For
Next fNum
If FileExists(CD.FileName) Then
    If FileDateTime(CD.FileName) <> MpqDate And NewFile = False Then OpenMpq
Else
    OpenMpq
End If
End Sub
Private Sub Toolbar_ButtonClick(ByVal Button As Button)
Select Case Button.Key
Case "New"
    mnuFNew_Click
Case "Open"
    mnuFOpen_Click
Case "Add"
    mnuMAdd_Click
Case "Add Folder"
    mnuMAddFolder_Click
Case "Extract"
    mnuMExtract_Click
Case "Compact"
    mnuMCompact_Click
Case "List"
    If NewFile = False Then OpenMpq
End Select
End Sub
Private Sub txtCommand_GotFocus()
cmdGo.Default = True
txtCommandHasFocus = True
StatBar.Style = 1
StatBar.SimpleText = "Current directory: " + Chr(34) + CurDir + Chr(34)
End Sub
Private Sub txtCommand_LostFocus()
cmdGo.Default = False
txtCommandHasFocus = False
StatBar.Style = 0
StatBar.SimpleText = ""
End Sub
