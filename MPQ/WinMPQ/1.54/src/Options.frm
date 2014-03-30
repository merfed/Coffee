VERSION 4.00
Begin VB.Form Options 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Options"
   ClientHeight    =   4695
   ClientLeft      =   1575
   ClientTop       =   1815
   ClientWidth     =   5415
   Height          =   5100
   Icon            =   "Options.frx":0000
   KeyPreview      =   -1  'True
   Left            =   1515
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   4695
   ScaleWidth      =   5415
   ShowInTaskbar   =   0   'False
   Top             =   1470
   Width           =   5535
   Begin VB.CommandButton Command2 
      Caption         =   "&Cancel"
      Height          =   375
      Left            =   3120
      TabIndex        =   1
      Top             =   4200
      Width           =   1335
   End
   Begin VB.CommandButton Command1 
      Caption         =   "O&k"
      Height          =   375
      Left            =   960
      TabIndex        =   0
      Top             =   4200
      Width           =   1335
   End
   Begin VB.PictureBox TabDisps 
      BorderStyle     =   0  'None
      Height          =   3495
      Index           =   1
      Left            =   240
      ScaleHeight     =   3495
      ScaleWidth      =   4995
      TabIndex        =   3
      Top             =   480
      Width           =   4995
      Begin VB.TextBox Text1 
         Height          =   285
         Left            =   0
         MaxLength       =   6
         TabIndex        =   9
         Text            =   "1024"
         Top             =   600
         Width           =   1215
      End
      Begin VB.TextBox Text2 
         Height          =   285
         Left            =   0
         TabIndex        =   8
         Text            =   "0"
         Top             =   1200
         Width           =   1215
      End
      Begin VB.CheckBox Check2 
         Caption         =   "&Associate WinMPQ with MPQ Archives"
         Height          =   255
         Left            =   0
         TabIndex        =   7
         Top             =   1680
         Value           =   2  'Grayed
         Width           =   3375
      End
      Begin VB.CheckBox Check4 
         Caption         =   "Use &wildcards in filenames for drag and drop"
         Height          =   255
         Left            =   0
         TabIndex        =   6
         Top             =   2400
         Value           =   2  'Grayed
         Width           =   3735
      End
      Begin VB.CheckBox Check5 
         Caption         =   "Automatically update &modified files"
         Height          =   255
         Left            =   0
         TabIndex        =   5
         Top             =   2160
         Value           =   2  'Grayed
         Width           =   3015
      End
      Begin VB.CheckBox Check6 
         Caption         =   "&Load extra file information (disable this for quicker MPQ load times)"
         Height          =   255
         Left            =   0
         TabIndex        =   4
         Top             =   1920
         Value           =   2  'Grayed
         Width           =   4995
      End
      Begin VB.Label Label1 
         AutoSize        =   -1  'True
         Caption         =   "Maximum files for new archives: (this cannot be changed for an existing archive)"
         Height          =   495
         Left            =   0
         TabIndex        =   12
         Top             =   120
         Width           =   4335
         WordWrap        =   -1  'True
      End
      Begin VB.Label Label2 
         AutoSize        =   -1  'True
         Caption         =   "Locale ID for extracting"
         Height          =   195
         Left            =   0
         TabIndex        =   11
         Top             =   960
         Width           =   1650
      End
      Begin VB.Label Label3 
         Caption         =   $"Options.frx":000C
         Height          =   855
         Left            =   0
         TabIndex        =   10
         Top             =   2640
         Width           =   4935
      End
   End
   Begin VB.PictureBox TabDisps 
      BorderStyle     =   0  'None
      Height          =   3495
      Index           =   2
      Left            =   240
      ScaleHeight     =   3495
      ScaleWidth      =   4935
      TabIndex        =   23
      Top             =   480
      Visible         =   0   'False
      Width           =   4935
      Begin VB.CheckBox Check8 
         Caption         =   "Do not use above lists when one is found by above option"
         Height          =   375
         Left            =   0
         TabIndex        =   49
         Top             =   2880
         Value           =   2  'Grayed
         Width           =   3375
      End
      Begin VB.CheckBox Check7 
         Caption         =   "Use file lists for similarly named archives"
         Height          =   195
         Left            =   0
         TabIndex        =   48
         Top             =   2640
         Width           =   3375
      End
      Begin VB.CommandButton cmdDelList 
         Caption         =   "&Remove"
         Height          =   375
         Left            =   3480
         TabIndex        =   45
         Top             =   1440
         Width           =   1335
      End
      Begin VB.ListBox FileLists 
         Height          =   2205
         Left            =   0
         TabIndex        =   44
         Top             =   360
         Width           =   3375
      End
      Begin VB.CommandButton cmdAddList 
         Caption         =   "&Add List File..."
         Height          =   375
         Left            =   3480
         TabIndex        =   24
         Top             =   840
         Width           =   1335
      End
      Begin VB.Label Label11 
         Caption         =   "Note:  Each file list added will increase the load time for archives."
         Height          =   255
         Left            =   0
         TabIndex        =   47
         Top             =   3240
         Width           =   4815
      End
      Begin VB.Label Label10 
         AutoSize        =   -1  'True
         Caption         =   "File Lists:"
         Height          =   195
         Left            =   0
         TabIndex        =   46
         Top             =   120
         Width           =   645
      End
   End
   Begin VB.PictureBox TabDisps 
      BorderStyle     =   0  'None
      Height          =   3495
      Index           =   5
      Left            =   240
      ScaleHeight     =   3495
      ScaleWidth      =   4935
      TabIndex        =   13
      Top             =   480
      Visible         =   0   'False
      Width           =   4935
      Begin VB.CommandButton Command4 
         Caption         =   "&Reset size/position"
         Height          =   375
         Left            =   360
         TabIndex        =   22
         Top             =   840
         Width           =   1695
      End
      Begin VB.CheckBox Check3 
         Caption         =   "Display &confirmation boxes"
         Height          =   255
         Left            =   0
         TabIndex        =   21
         Top             =   120
         Value           =   2  'Grayed
         Width           =   2415
      End
      Begin VB.CheckBox Check1 
         Caption         =   "&Save last window size and position"
         Height          =   255
         Left            =   0
         TabIndex        =   20
         Top             =   480
         Value           =   2  'Grayed
         Width           =   3015
      End
      Begin VB.Frame Frame1 
         Caption         =   "Startup Path"
         Height          =   1215
         Left            =   0
         TabIndex        =   14
         Top             =   2280
         Width           =   4935
         Begin VB.OptionButton Option1 
            Caption         =   "Last &open folder"
            Height          =   255
            Index           =   0
            Left            =   120
            TabIndex        =   19
            Top             =   240
            Value           =   -1  'True
            Width           =   1575
         End
         Begin VB.OptionButton Option1 
            Caption         =   "A&pplication folder"
            Height          =   255
            Index           =   1
            Left            =   1680
            TabIndex        =   18
            Top             =   240
            Width           =   1695
         End
         Begin VB.OptionButton Option1 
            Caption         =   "&User-defined folder"
            Height          =   255
            Index           =   2
            Left            =   120
            TabIndex        =   17
            Top             =   480
            Width           =   1695
         End
         Begin VB.TextBox Text3 
            Enabled         =   0   'False
            Height          =   285
            Left            =   120
            TabIndex        =   16
            Top             =   840
            Width           =   3615
         End
         Begin VB.CommandButton Command5 
            Caption         =   "&Folder..."
            Enabled         =   0   'False
            Height          =   285
            Left            =   3840
            TabIndex        =   15
            Top             =   840
            Width           =   975
         End
      End
   End
   Begin VB.PictureBox TabDisps 
      BorderStyle     =   0  'None
      Height          =   3495
      Index           =   4
      Left            =   240
      ScaleHeight     =   3495
      ScaleWidth      =   4935
      TabIndex        =   26
      Top             =   480
      Visible         =   0   'False
      Width           =   4935
      Begin VB.ListBox Actions 
         Height          =   1215
         IntegralHeight  =   0   'False
         Left            =   3120
         TabIndex        =   38
         Top             =   2280
         Width           =   1815
      End
      Begin MSComctlLib.ListView FileTypes 
         Height          =   2535
         Left            =   0
         TabIndex        =   43
         Top             =   960
         Width           =   3015
         _ExtentX        =   5318
         _ExtentY        =   4471
         View            =   3
         LabelEdit       =   1
         Sorted          =   -1  'True
         MultiSelect     =   -1  'True
         LabelWrap       =   -1  'True
         HideSelection   =   -1  'True
         _Version        =   393217
         ForeColor       =   -2147483640
         BackColor       =   -2147483643
         BorderStyle     =   1
         Appearance      =   1
         NumItems        =   1
         BeginProperty ColumnHeader(1) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
            Text            =   "Registered file types:"
            Object.Width           =   2540
         EndProperty
      End
      Begin VB.Label Label9 
         AutoSize        =   -1  'True
         Caption         =   "File extensions:"
         Height          =   195
         Left            =   3120
         TabIndex        =   42
         Top             =   960
         Width           =   1080
      End
      Begin VB.Label Label7 
         AutoSize        =   -1  'True
         Caption         =   "Default action:"
         Height          =   195
         Left            =   3120
         TabIndex        =   40
         Top             =   2040
         Width           =   1035
      End
      Begin VB.Label Label8 
         Height          =   855
         Left            =   3120
         TabIndex        =   41
         Top             =   1200
         Width           =   1755
      End
      Begin VB.Label Label6 
         AutoSize        =   -1  'True
         Caption         =   $"Options.frx":00F6
         Height          =   855
         Left            =   0
         TabIndex        =   39
         Top             =   120
         Width           =   4935
         WordWrap        =   -1  'True
      End
   End
   Begin VB.PictureBox TabDisps 
      BorderStyle     =   0  'None
      Height          =   3495
      Index           =   3
      Left            =   240
      ScaleHeight     =   3495
      ScaleWidth      =   4935
      TabIndex        =   25
      Top             =   480
      Visible         =   0   'False
      Width           =   4935
      Begin VB.ListBox List1 
         Height          =   1815
         ItemData        =   "Options.frx":01CE
         Left            =   0
         List            =   "Options.frx":01D0
         Sorted          =   -1  'True
         TabIndex        =   35
         Top             =   720
         Width           =   1575
      End
      Begin VB.TextBox Text4 
         Height          =   285
         Left            =   0
         TabIndex        =   34
         Top             =   360
         Width           =   855
      End
      Begin VB.CommandButton cmdAdd 
         Caption         =   "&Add"
         Height          =   285
         Left            =   960
         TabIndex        =   33
         Top             =   360
         Width           =   615
      End
      Begin VB.CommandButton Command6 
         Caption         =   "&Remove"
         Height          =   255
         Left            =   0
         TabIndex        =   32
         Top             =   2640
         Width           =   1095
      End
      Begin VB.ComboBox Combo1 
         Enabled         =   0   'False
         Height          =   315
         ItemData        =   "Options.frx":01D2
         Left            =   1800
         List            =   "Options.frx":01DF
         Style           =   2  'Dropdown List
         TabIndex        =   31
         Top             =   720
         Width           =   2535
      End
      Begin VB.Frame Frame2 
         Caption         =   "Audio Compression"
         Height          =   1335
         Left            =   1800
         TabIndex        =   27
         Top             =   1200
         Visible         =   0   'False
         Width           =   2535
         Begin VB.OptionButton AudioC 
            Caption         =   "Medium"
            Height          =   255
            Index           =   0
            Left            =   120
            TabIndex        =   30
            Top             =   600
            Value           =   -1  'True
            Width           =   2175
         End
         Begin VB.OptionButton AudioC 
            Caption         =   "Highest (Least space)"
            Height          =   255
            Index           =   1
            Left            =   120
            TabIndex        =   29
            Top             =   960
            Width           =   2175
         End
         Begin VB.OptionButton AudioC 
            Caption         =   "Lowest (Best quality)"
            Height          =   255
            Index           =   2
            Left            =   120
            TabIndex        =   28
            Top             =   240
            Width           =   2175
         End
      End
      Begin VB.Label Label5 
         Caption         =   "Compression type"
         Height          =   255
         Left            =   1800
         TabIndex        =   37
         Top             =   480
         Width           =   1935
      End
      Begin VB.Label Label4 
         Caption         =   "File Extension"
         Height          =   255
         Left            =   0
         TabIndex        =   36
         Top             =   120
         Width           =   1215
      End
   End
   Begin MSComctlLib.TabStrip Tabs 
      Height          =   3975
      Left            =   120
      TabIndex        =   2
      Top             =   120
      Width           =   5175
      _ExtentX        =   9128
      _ExtentY        =   7011
      HotTracking     =   -1  'True
      _Version        =   393216
      BeginProperty Tabs {1EFB6598-857C-11D1-B16A-00C0F0283628} 
         NumTabs         =   5
         BeginProperty Tab1 {1EFB659A-857C-11D1-B16A-00C0F0283628} 
            Caption         =   "General"
            ImageVarType    =   2
         EndProperty
         BeginProperty Tab2 {1EFB659A-857C-11D1-B16A-00C0F0283628} 
            Caption         =   "File Lists"
            ImageVarType    =   2
         EndProperty
         BeginProperty Tab3 {1EFB659A-857C-11D1-B16A-00C0F0283628} 
            Caption         =   "Compression Auto-Selection"
            ImageVarType    =   2
         EndProperty
         BeginProperty Tab4 {1EFB659A-857C-11D1-B16A-00C0F0283628} 
            Caption         =   "File Associations"
            ImageVarType    =   2
         EndProperty
         BeginProperty Tab5 {1EFB659A-857C-11D1-B16A-00C0F0283628} 
            Caption         =   "Other"
            ImageVarType    =   2
         EndProperty
      EndProperty
   End
End
Attribute VB_Name = "Options"
Attribute VB_Creatable = False
Attribute VB_Exposed = False
Option Explicit

Dim OldFileName As String, NewListFile As String
Dim NewExtNames() As String, NewExtComp() As Integer
Dim ActID() As String

Private Sub Check8_Click()
If Check8.Value = 1 Then Check8.Value = 2
End Sub
Private Sub cmdAdd_Click()
Dim eNum As Integer
If Text4 <> "" Then
    If Left(Text4, 1) <> "." Then Text4 = "." + Text4
    For eNum = 1 To UBound(NewExtNames)
        If Text4 = NewExtNames(eNum) Then Exit Sub
    Next eNum
    List1.AddItem Text4
    ReDim Preserve NewExtNames(UBound(NewExtNames) + 1) As String
    NewExtNames(UBound(NewExtNames)) = Text4
    ReDim Preserve NewExtComp(UBound(NewExtComp) + 1) As Integer
    NewExtComp(UBound(NewExtComp)) = -1
    Text4 = ""
End If
End Sub
Private Sub cmdAddList_Click()
Dim lNum As Long
CD.Flags = &H1000 Or &H4 Or &H2
CD.Filter = "Text Files (*.txt)|*.txt|All Files (*.*)|*.*"
If ShowOpen(CD) = False Then GoTo Cancel
FileLists.AddItem CD.FileName
If FileLists.ListCount > 0 Then
    NewListFile = FileLists.List(0)
Else
    NewListFile = ""
End If
For lNum = 1 To FileLists.ListCount - 1
    NewListFile = NewListFile + vbCrLf + FileLists.List(lNum)
Next lNum
Cancel:
End Sub
Private Sub cmdDelList_Click()
Dim lNum As Long
If FileLists.ListIndex > -1 Then
    FileLists.RemoveItem FileLists.ListIndex
    If FileLists.ListCount > 0 Then
        NewListFile = FileLists.List(0)
    Else
        NewListFile = ""
    End If
    For lNum = 1 To FileLists.ListCount - 1
        NewListFile = NewListFile + vbCrLf + FileLists.List(lNum)
    Next lNum
End If
End Sub
Private Sub Combo1_Click()
Dim eNum As Integer
For eNum = 1 To UBound(NewExtNames)
    If List1.List(List1.ListIndex) = NewExtNames(eNum) Then Exit For
Next eNum
If UBound(NewExtNames) = 0 Then eNum = 0
If Combo1.ListIndex = 2 Then
    Frame2.Visible = True
    NewExtComp(eNum) = Combo1.ListIndex - 2
Else
    Frame2.Visible = False
    NewExtComp(eNum) = Combo1.ListIndex - 2
End If
End Sub
Private Sub AudioC_Click(Index As Integer)
Dim eNum As Integer
For eNum = 1 To UBound(NewExtNames)
    If List1.List(List1.ListIndex) = NewExtNames(eNum) Then Exit For
Next eNum
If UBound(NewExtNames) = 0 Then eNum = 0
NewExtComp(eNum) = Index
End Sub
Private Sub Check1_Click()
If Check1.Value = 1 Then Check1.Value = 2
End Sub

Private Sub Check2_Click()
If Check2.Value = 1 Then Check2.Value = 2
End Sub

Private Sub Check3_Click()
If Check3.Value = 1 Then Check3.Value = 2
End Sub

Private Sub Check4_Click()
If Check4.Value = 1 Then Check4.Value = 2
End Sub

Private Sub Check5_Click()
If Check5.Value = 1 Then Check5.Value = 2
End Sub

Private Sub Check6_Click()
If Check6.Value = 1 Then Check6.Value = 2
End Sub
Private Sub Command1_Click()
Dim Path As String, BatKey As String
Dim eNum As Integer, ExtList As String
Dim dItem As String, ndItem As String, aNum As Long
Path = App.Path
If Right(Path, 1) <> "\" Then Path = Path + "\"
Text1_LostFocus
Text2_LostFocus
MpqEx.Mpq.DefaultMaxFiles = Text1
LocaleID = Text2
MpqEx.Mpq.SetLocale (LocaleID)
NewKey AppKey
SetReg AppKey + "DefaultMaxFiles", Text1, REG_DWORD
SetReg AppKey + "LocaleID", Text2, REG_DWORD
If Check1.Value > 0 Then
    SetReg AppKey + "SaveWindowStatus", 1, REG_DWORD
Else
    SetReg AppKey + "SaveWindowStatus", 0, REG_DWORD
End If
If Check3.Value > 0 Then
    SetReg AppKey + "ShowConfirmation", 1, REG_DWORD
Else
    SetReg AppKey + "ShowConfirmation", 0, REG_DWORD
End If
If Check4.Value > 0 Then
    SetReg AppKey + "UseDragDropWildcards", 1, REG_DWORD
Else
    SetReg AppKey + "UseDragDropWildcards", 0, REG_DWORD
End If
If Check5.Value > 0 Then
    SetReg AppKey + "CheckModDateTime", 1, REG_DWORD
Else
    SetReg AppKey + "CheckModDateTime", 0, REG_DWORD
    MpqEx.Timer1.Enabled = False
End If
If Check6.Value > 0 Then
    SetReg AppKey + "LoadExtraInfo", 1, REG_DWORD
Else
    SetReg AppKey + "LoadExtraInfo", 0, REG_DWORD
End If
If Check7.Value > 0 Then
    SetReg AppKey + "AutofindFileLists", 1, REG_DWORD
Else
    SetReg AppKey + "AutofindFileLists", 0, REG_DWORD
End If
If Check8.Value > 0 Then
    SetReg AppKey + "UseOnlyAutofindLists", 1, REG_DWORD
Else
    SetReg AppKey + "UseOnlyAutofindLists", 0, REG_DWORD
End If
If Check2.Value > 0 Then
    NewKey "HKEY_CLASSES_ROOT\.mpq\", "Mpq.Archive"
    NewKey "HKEY_CLASSES_ROOT\.mpq\ShellNew\"
    SetReg "HKEY_CLASSES_ROOT\.mpq\ShellNew\NullFile", ""
    NewKey "HKEY_CLASSES_ROOT\Mpq.Archive\", "MPQ Archive"
    NewKey "HKEY_CLASSES_ROOT\Mpq.Archive\DefaultIcon\", Path + App.EXEName + ".exe,1"
    NewKey "HKEY_CLASSES_ROOT\Mpq.Archive\shell\"
    NewKey "HKEY_CLASSES_ROOT\Mpq.Archive\shell\open\"
    NewKey "HKEY_CLASSES_ROOT\Mpq.Archive\shell\open\command\", Chr(34) + Path + App.EXEName + ".exe" + Chr(34) + " " + Chr(34) + "%1" + Chr(34)
    BatKey = "HKEY_CLASSES_ROOT\" + GetReg("HKEY_CLASSES_ROOT\.bat\", "batfile") + "\"
    NewKey "HKEY_CLASSES_ROOT\.mscript\", "Mpq.Script"
    NewKey "HKEY_CLASSES_ROOT\.mbat\", "Mpq.Script"
    NewKey "HKEY_CLASSES_ROOT\.mscript\ShellNew\"
    SetReg "HKEY_CLASSES_ROOT\.mscript\ShellNew\NullFile", ""
    NewKey "HKEY_CLASSES_ROOT\Mpq.Script\", "Mo'PaQ 2000 Script"
    NewKey "HKEY_CLASSES_ROOT\Mpq.Script\DefaultIcon\", GetReg(BatKey + "DefaultIcon\", "C:\WINDOWS\SYSTEM\shell32.dll,-153")
    NewKey "HKEY_CLASSES_ROOT\Mpq.Script\shell\"
    NewKey "HKEY_CLASSES_ROOT\Mpq.Script\shell\open\"
    NewKey "HKEY_CLASSES_ROOT\Mpq.Script\shell\open\command\", GetReg(BatKey + "shell\edit\command\", "C:\WINDOWS\NOTEPAD.EXE %1")
    NewKey "HKEY_CLASSES_ROOT\Mpq.Script\shell\"
    NewKey "HKEY_CLASSES_ROOT\Mpq.Script\shell\run\", "&Run"
    NewKey "HKEY_CLASSES_ROOT\Mpq.Script\shell\run\command\", Chr(34) + Path + App.EXEName + ".exe" + Chr(34) + " script " + Chr(34) + "%1" + Chr(34)
Else
    If GetReg("HKEY_CLASSES_ROOT\.mpq\") = "Mpq.Archive" Then
        DelKey "HKEY_CLASSES_ROOT\.mpq\ShellNew\"
        DelKey "HKEY_CLASSES_ROOT\.mpq\"
        SetReg "HKEY_CLASSES_ROOT\Mpq.Archive\shell\open\command\", "not used"
        DelKey "HKEY_CLASSES_ROOT\.mscript\ShellNew\"
        DelKey "HKEY_CLASSES_ROOT\.mscript\"
        DelKey "HKEY_CLASSES_ROOT\.mbat\"
    End If
End If
SHChangeNotify SHCNE_ASSOCCHANGED, SHCNF_IDLIST, vbNullString, vbNullString
If Option1(0).Value = True Then
    SetReg AppKey + "StartupPathType", 0, REG_DWORD
    Text3 = CurDir
ElseIf Option1(1).Value = True Then
    SetReg AppKey + "StartupPathType", 1, REG_DWORD
    Text3 = App.Path
ElseIf Option1(2).Value = True Then
    SetReg AppKey + "StartupPathType", 2, REG_DWORD
End If
Path = Text3
If Right(Path, 1) <> "\" Then Path = Path + "\"
If IsDir(Path) Then
    SetReg AppKey + "StartupPath", Text3
    ChDir Text3
End If
DelKey AppKey + "Compression\"
NewKey AppKey + "Compression\"
For eNum = 1 To UBound(NewExtNames)
    ExtList = ExtList + NewExtNames(eNum)
    SetReg AppKey + "Compression\" + NewExtNames(eNum), CStr(NewExtComp(eNum))
Next eNum
SetReg AppKey + "Compression\List", ExtList
NewKey SharedAppKey + "FileDefaultActions\"
For aNum = 1 To FileTypes.ListItems.Count
    dItem = GetReg("HKEY_CLASSES_ROOT\" + FileTypes.ListItems.Item(aNum).Key + "\shell\", "open")
    dItem = GetReg(SharedAppKey + "FileDefaultActions\" + FileTypes.ListItems.Item(aNum).Key, dItem)
    ndItem = FileTypes.ListItems.Item(aNum).Tag
    If LCase(dItem) <> LCase(ndItem) And ndItem <> "" Then
        SetReg SharedAppKey + "FileDefaultActions\" + FileTypes.ListItems.Item(aNum).Key, ndItem
    End If
Next aNum
Hide
If LCase(ListFile) <> LCase(NewListFile) Then
    ListFile = NewListFile
    SetReg AppKey + "ListFile", ListFile
    CD.FileName = OldFileName
    If FileExists(OldFileName) Then MpqEx.OpenMpq
End If
Unload Me
End Sub
Private Sub Command2_Click()
Unload Me
End Sub
Private Sub Command4_Click()
DelReg AppKey + "Status\WindowState"
DelReg AppKey + "Status\WindowHeight"
DelReg AppKey + "Status\WindowLeft"
DelReg AppKey + "Status\WindowTop"
DelReg AppKey + "Status\WindowWidth"
Check1.Value = 0
End Sub

Private Sub Command5_Click()
Dim Path As String
Path = PathInputBox(PathInput, "WinMPQ Startup Path", Text3)
If Path <> "" Then Text3 = Path
End Sub

Private Sub Command6_Click()
Dim eNum As Integer
If List1.ListIndex > -1 Then
    For eNum = 1 To UBound(NewExtNames)
        If List1.List(List1.ListIndex) = NewExtNames(eNum) Then Exit For
    Next eNum
    If eNum < UBound(NewExtNames) Then
        For eNum = eNum To UBound(NewExtNames) - 1
            NewExtNames(eNum) = NewExtNames(eNum + 1)
            NewExtComp(eNum) = NewExtComp(eNum + 1)
        Next eNum
    End If
    ReDim Preserve NewExtNames(UBound(NewExtNames) - 1) As String
    ReDim Preserve NewExtComp(UBound(NewExtComp) - 1) As Integer
    On Error Resume Next
    List1.RemoveItem List1.ListIndex
End If
End Sub
Private Sub Form_Load()
Dim Path As String, PathType As Integer, NewFileListNames As String
Dim ExtList As String
Dim aExt As String, aName As String, aNum As Long
Left = MpqEx.Left + 330
If Left < 0 Then Left = 0
If Left + Width > Screen.Width Then Left = Screen.Width - Width
Top = MpqEx.Top + 315
If Top < 0 Then Top = 0
If Top + Height > Screen.Height Then Top = Screen.Height - Height
Path = App.Path
If Right(Path, 1) <> "\" Then Path = Path + "\"
Text1 = MpqEx.Mpq.DefaultMaxFiles
Text2 = LocaleID
OldFileName = CD.FileName
CD.FileName = ""
NewListFile = GetReg(AppKey + "ListFile", Path + "mpq_data.txt")
For aNum = 1 To Len(NewListFile)
    If InStr(aNum, NewListFile, vbCrLf) Then
        aName = Mid(NewListFile, aNum, InStr(aNum, NewListFile, vbCrLf) - aNum)
        If FileExists(aName) Then
            FileLists.AddItem aName
            NewFileListNames = NewFileListNames + aName + vbCrLf
        End If
        aNum = InStr(aNum, NewListFile, vbCrLf) + 1
    Else
        aName = Mid(NewListFile, aNum)
        If FileExists(aName) Then
            FileLists.AddItem aName
            NewFileListNames = NewFileListNames + aName
        End If
        Exit For
    End If
Next aNum
NewListFile = NewFileListNames
If Right(NewListFile, 2) = vbCrLf Then NewListFile = Left(NewListFile, Len(NewListFile) - 2)
If GetReg(AppKey + "SaveWindowStatus", 1) > 0 Then Check1.Value = 1 Else Check1.Value = 0
If GetReg(AppKey + "ShowConfirmation", 1) > 0 Then Check3.Value = 1 Else Check3.Value = 0
If GetReg(AppKey + "UseDragDropWildcards", 1) > 0 Then Check4.Value = 1 Else Check4.Value = 0
If GetReg(AppKey + "CheckModDateTime", 1) > 0 Then Check5.Value = 1 Else Check5.Value = 0
If GetReg(AppKey + "LoadExtraInfo", 1) > 0 Then Check6.Value = 1 Else Check6.Value = 0
If GetReg(AppKey + "AutofindFileLists", 0) > 0 Then Check7.Value = 1 Else Check7.Value = 0
If GetReg(AppKey + "UseOnlyAutofindLists", 1) > 0 Then Check8.Value = 1 Else Check8.Value = 0
If GetReg("HKEY_CLASSES_ROOT\.mpq\", "Mpq.Archive") = "Mpq.Archive" And InStr(1, GetReg("HKEY_CLASSES_ROOT\Mpq.Archive\shell\open\command\", Chr(34) + Path + App.EXEName + ".exe" + Chr(34) + " " + Chr(34) + "%1" + Chr(34)), App.EXEName + ".exe", 1) > 0 Then Check2.Value = 1 Else Check2.Value = 0
Text3 = GetReg(AppKey + "StartupPath", CurDir)
PathType = GetReg(AppKey + "StartupPathType", 0)
If PathType < 0 Then PathType = 0
If PathType > 2 Then PathType = 2
Option1(PathType).Value = True
If PathType = 0 Then
    Text3 = CurDir
ElseIf PathType = 1 Then
    Text3 = App.Path
End If
ReDim NewExtNames(0) As String
ReDim NewExtComp(0) As Integer
Combo1.ListIndex = 1
ExtList = GetReg(AppKey + "Compression\List", ".bik.smk.wav")
If InStr(1, ExtList, ".") > 0 And Len(ExtList) > 1 Then
Do
    ReDim Preserve NewExtNames(UBound(NewExtNames) + 1) As String
    ReDim Preserve NewExtComp(UBound(NewExtComp) + 1) As Integer
    If InStr(2, ExtList, ".") > 0 Then
        NewExtNames(UBound(NewExtNames)) = Left(ExtList, InStr(2, ExtList, ".") - 1)
    Else
        NewExtNames(UBound(NewExtNames)) = ExtList
    End If
    ExtList = Mid(ExtList, Len(NewExtNames(UBound(NewExtNames))) + 1)
    List1.AddItem NewExtNames(UBound(NewExtNames))
    If LCase(NewExtNames(UBound(NewExtNames))) = ".bik" Then
        NewExtComp(UBound(NewExtComp)) = CInt(GetReg(AppKey + "Compression\.bik", "-2"))
    ElseIf LCase(NewExtNames(UBound(NewExtNames))) = ".smk" Then
        NewExtComp(UBound(NewExtComp)) = CInt(GetReg(AppKey + "Compression\.smk", "-2"))
    ElseIf LCase(NewExtNames(UBound(NewExtNames))) = ".wav" Then
        NewExtComp(UBound(NewExtComp)) = CInt(GetReg(AppKey + "Compression\.wav", "0"))
    Else
        NewExtComp(UBound(NewExtComp)) = CInt(GetReg(AppKey + "Compression\" + NewExtNames(UBound(NewExtNames)), "-1"))
    End If
Loop Until ExtList = ""
End If
Do
    aExt = EnumKey("HKEY_CLASSES_ROOT\", aNum)
    If Left(aExt, 1) = "." Then
        aName = GetReg("HKEY_CLASSES_ROOT\" + aExt + "\")
        If aName <> "" Then
            On Error GoTo AlreadyExists
            FileTypes.ListItems.Add(, aName, GetReg("HKEY_CLASSES_ROOT\" + aName + "\", UCase(Mid(aExt, 2)) + " File")).ToolTipText = UCase(aExt)
            On Error GoTo 0
        End If
    ElseIf LCase(aExt) = "unknown" Then
        FileTypes.ListItems.Add(, aExt, GetReg("HKEY_CLASSES_ROOT\" + aExt + "\")).ToolTipText = ""
        If FileTypes.ListItems.Item(aExt).Text = "" Then FileTypes.ListItems.Item(aExt).Text = " Unknown File"
    End If
    aNum = aNum + 1
Loop Until aExt = ""
Exit Sub
AlreadyExists:
    FileTypes.ListItems.Item(aName).ToolTipText = FileTypes.ListItems.Item(aName).ToolTipText + " " + UCase(aExt)
Resume Next
End Sub
Private Sub Form_Resize()
FileTypes.ColumnHeaders.Item(1).Width = FileTypes.Width - 30 * Screen.TwipsPerPixelX
End Sub

Private Sub Form_Unload(Cancel As Integer)
CD.FileName = OldFileName
End Sub

Private Sub List1_Click()
Dim eNum As Integer, OldExtComp As Integer
If List1.ListIndex > -1 Then
    Combo1.Enabled = True
    For eNum = 1 To UBound(NewExtNames)
        If List1.List(List1.ListIndex) = NewExtNames(eNum) Then Exit For
    Next eNum
    Select Case NewExtComp(eNum)
    Case -2
        AudioC(0).Value = True
        Combo1.ListIndex = 0
    Case -1
        AudioC(0).Value = True
        Combo1.ListIndex = 1
    Case 0, 1, 2
        OldExtComp = NewExtComp(eNum)
        Combo1.ListIndex = 2
        AudioC(OldExtComp).Value = True
    Case Else
        AudioC(0).Value = True
        Combo1.ListIndex = 1
    End Select
Else
    Combo1.ListIndex = 1
    Combo1.Enabled = False
End If
End Sub
Private Sub Option1_Click(Index As Integer)
If Index = 2 Then
    Text3.Enabled = True
    Command5.Enabled = True
Else
    Text3.Enabled = False
    Command5.Enabled = False
End If
End Sub

Private Sub Tabs_Click()
Dim TabDisp As PictureBox
For Each TabDisp In TabDisps
    TabDisp.Visible = False
Next TabDisp
TabDisps(Tabs.SelectedItem.Index).Visible = True
End Sub
Private Sub Text1_KeyPress(KeyAscii As Integer)
If (KeyAscii < 48 Or KeyAscii > 57) And KeyAscii <> 8 Then KeyAscii = 0
End Sub
Private Sub Text1_LostFocus()
If Text1 = "" Then Text1 = 0
If Text1 < 16 Then Text1 = 16
If Text1 > 262144 Then Text1 = 262144
End Sub
Private Sub Text2_KeyPress(KeyAscii As Integer)
Dim NewValue As Long
If (KeyAscii < 48 Or KeyAscii > 57) And KeyAscii <> 8 And KeyAscii <> Asc("-") Then KeyAscii = 0
On Error GoTo TooBig
If (KeyAscii >= 48 And KeyAscii <= 57) Or KeyAscii = Asc("-") Then NewValue = CLng(Text2 + Chr(KeyAscii))
On Error GoTo 0
Exit Sub
TooBig:
KeyAscii = 0
End Sub
Private Sub Text2_LostFocus()
If Text2 = "" Then Text2 = 0
End Sub

Private Sub Text4_GotFocus()
cmdAdd.Default = True
End Sub

Private Sub Text4_LostFocus()
Command1.Default = True
End Sub
Private Sub Actions_Click()
On Error GoTo NotSelected
FileTypes.SelectedItem.Tag = FileTypes.SelectedItem.Tag
On Error GoTo 0
If FileTypes.SelectedItem.Selected = True Then
    FileTypes.SelectedItem.Tag = ActID(Actions.ListIndex + 1)
End If
NotSelected:
End Sub
Private Sub FileTypes_ItemClick(ByVal Item As ListItem)
Dim aNum As Long, aItem As String, aName As String, bNum As Long, dItem As String
Label8 = Item.ToolTipText
Actions.Clear
ReDim ActID(0) As String
aName = Item.Key
Do
    aItem = EnumKey("HKEY_CLASSES_ROOT\" + aName + "\shell\", aNum)
    If aItem <> "" Then
        If LCase(aItem) = "openas" And IsEmpty(GetReg("HKEY_CLASSES_ROOT\" + aName + "\shell\" + aItem + "\")) Then
            Actions.AddItem "Open with..."
        Else
            Actions.AddItem GetReg("HKEY_CLASSES_ROOT\" + aName + "\shell\" + aItem + "\", UCase(Left(aItem, 1)) + Mid(aItem, 2))
        End If
        ReDim Preserve ActID(UBound(ActID) + 1) As String
        ActID(UBound(ActID)) = aItem
        aNum = aNum + 1
    End If
Loop Until aItem = ""
If Item.Tag = "" Then
    dItem = GetReg("HKEY_CLASSES_ROOT\" + aName + "\shell\", "open")
    dItem = GetReg(SharedAppKey + "FileDefaultActions\" + aName, dItem)
Else
    dItem = Item.Tag
End If
If Actions.ListCount > 0 Then Actions.ListIndex = 0
For bNum = 0 To Actions.ListCount - 1
    If LCase(ActID(bNum + 1)) = LCase(dItem) Then
        Actions.ListIndex = bNum
    End If
Next bNum
Item.Tag = dItem
End Sub
