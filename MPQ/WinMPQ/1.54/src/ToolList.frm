VERSION 4.00
Begin VB.Form ToolList 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Add/Remove Tools"
   ClientHeight    =   2775
   ClientLeft      =   1575
   ClientTop       =   1815
   ClientWidth     =   4065
   Height          =   3180
   Icon            =   "ToolList.frx":0000
   Left            =   1515
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   2775
   ScaleWidth      =   4065
   ShowInTaskbar   =   0   'False
   Top             =   1470
   Width           =   4185
   Begin VB.CommandButton Command2 
      Cancel          =   -1  'True
      Caption         =   "&Cancel"
      Height          =   375
      Left            =   1920
      TabIndex        =   3
      Top             =   2400
      Width           =   1455
   End
   Begin VB.CommandButton cmdOK 
      Caption         =   "O&K"
      Default         =   -1  'True
      Height          =   375
      Left            =   240
      TabIndex        =   2
      Top             =   2400
      Width           =   1455
   End
   Begin VB.ListBox tList 
      Height          =   2370
      IntegralHeight  =   0   'False
      Left            =   0
      TabIndex        =   1
      Top             =   0
      Width           =   3600
   End
   Begin MSComctlLib.Toolbar Toolbar1 
      Height          =   450
      Left            =   3600
      TabIndex        =   0
      Top             =   0
      Width           =   495
      _ExtentX        =   873
      _ExtentY        =   794
      ButtonWidth     =   820
      ButtonHeight    =   794
      AllowCustomize  =   0   'False
      Style           =   1
      ImageList       =   "ImageList1"
      DisabledImageList=   "ImageList2"
      HotImageList    =   "ImageList3"
      _Version        =   393216
      BeginProperty Buttons {66833FE8-8583-11D1-B16A-00C0F0283628} 
         NumButtons      =   6
         BeginProperty Button1 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "Add"
            ToolTipText     =   "Add a program to the list."
            ImageIndex      =   1
         EndProperty
         BeginProperty Button2 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Enabled         =   0   'False
            Key             =   "Remove"
            ToolTipText     =   "Remove selected program from the list."
            ImageIndex      =   2
         EndProperty
         BeginProperty Button3 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Enabled         =   0   'False
            Key             =   "Edit"
            ToolTipText     =   "Edit properties for selected program."
            ImageIndex      =   3
         EndProperty
         BeginProperty Button4 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Style           =   3
         EndProperty
         BeginProperty Button5 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Enabled         =   0   'False
            Key             =   "Up"
            ToolTipText     =   "Move selected program up in the list."
            ImageIndex      =   4
         EndProperty
         BeginProperty Button6 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Enabled         =   0   'False
            Key             =   "Down"
            ToolTipText     =   "Move selected program down in the list."
            ImageIndex      =   5
         EndProperty
      EndProperty
   End
   Begin MSComctlLib.ImageList ImageList3 
      Left            =   3360
      Top             =   2400
      _ExtentX        =   1005
      _ExtentY        =   1005
      BackColor       =   -2147483643
      ImageWidth      =   24
      ImageHeight     =   24
      MaskColor       =   12632256
      _Version        =   393216
      BeginProperty Images {2C247F25-8591-11D1-B16A-00C0F0283628} 
         NumListImages   =   5
         BeginProperty ListImage1 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "ToolList.frx":000C
            Key             =   ""
         EndProperty
         BeginProperty ListImage2 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "ToolList.frx":00D8
            Key             =   ""
         EndProperty
         BeginProperty ListImage3 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "ToolList.frx":0188
            Key             =   ""
         EndProperty
         BeginProperty ListImage4 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "ToolList.frx":0238
            Key             =   ""
         EndProperty
         BeginProperty ListImage5 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "ToolList.frx":0310
            Key             =   ""
         EndProperty
      EndProperty
   End
   Begin MSComctlLib.ImageList ImageList2 
      Left            =   1560
      Top             =   2400
      _ExtentX        =   1005
      _ExtentY        =   1005
      BackColor       =   -2147483643
      ImageWidth      =   24
      ImageHeight     =   24
      MaskColor       =   12632256
      _Version        =   393216
      BeginProperty Images {2C247F25-8591-11D1-B16A-00C0F0283628} 
         NumListImages   =   5
         BeginProperty ListImage1 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "ToolList.frx":03E8
            Key             =   ""
         EndProperty
         BeginProperty ListImage2 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "ToolList.frx":04C4
            Key             =   ""
         EndProperty
         BeginProperty ListImage3 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "ToolList.frx":0574
            Key             =   ""
         EndProperty
         BeginProperty ListImage4 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "ToolList.frx":0654
            Key             =   ""
         EndProperty
         BeginProperty ListImage5 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "ToolList.frx":0734
            Key             =   ""
         EndProperty
      EndProperty
   End
   Begin MSComctlLib.ImageList ImageList1 
      Left            =   0
      Top             =   2280
      _ExtentX        =   1005
      _ExtentY        =   1005
      BackColor       =   -2147483643
      ImageWidth      =   24
      ImageHeight     =   24
      MaskColor       =   12632256
      _Version        =   393216
      BeginProperty Images {2C247F25-8591-11D1-B16A-00C0F0283628} 
         NumListImages   =   5
         BeginProperty ListImage1 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "ToolList.frx":0810
            Key             =   ""
         EndProperty
         BeginProperty ListImage2 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "ToolList.frx":08EC
            Key             =   ""
         EndProperty
         BeginProperty ListImage3 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "ToolList.frx":09A8
            Key             =   ""
         EndProperty
         BeginProperty ListImage4 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "ToolList.frx":0A6C
            Key             =   ""
         EndProperty
         BeginProperty ListImage5 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "ToolList.frx":0B4C
            Key             =   ""
         EndProperty
      EndProperty
   End
End
Attribute VB_Name = "ToolList"
Attribute VB_Creatable = False
Attribute VB_Exposed = False
Option Explicit

Dim tCommands() As String
Private Sub cmdOK_Click()
Dim tNum As Long
DelKey AppKey + "Tools\"
NewKey AppKey + "Tools\"
For tNum = 0 To tList.ListCount - 1
    SetReg AppKey + "Tools\Name" + CStr(tNum), tList.List(tNum)
    SetReg AppKey + "Tools\Command" + CStr(tNum), tCommands(tNum + 1)
Next tNum
Unload Me
End Sub
Private Sub Command2_Click()
Unload Me
End Sub
Private Sub Form_Load()
Dim tNum As Long, ToolName As String, ToolCommand
Left = MpqEx.Left + 330
If Left < 0 Then Left = 0
If Left + Width > Screen.Width Then Left = Screen.Width - Width
Top = MpqEx.Top + 315
If Top < 0 Then Top = 0
If Top + Height > Screen.Height Then Top = Screen.Height - Height
ReDim tCommands(0) As String
Do
    ToolName = GetReg(AppKey + "Tools\Name" + CStr(tNum))
    ToolCommand = GetReg(AppKey + "Tools\Command" + CStr(tNum))
    If ToolName = "" Then ToolName = ToolCommand
    If ToolName <> "" Then
        ReDim Preserve tCommands(UBound(tCommands) + 1) As String
        tCommands(UBound(tCommands)) = ToolCommand
        tList.AddItem ToolName
    End If
    tNum = tNum + 1
Loop Until ToolName = ""
End Sub
Private Sub tList_Click()
If tList.ListIndex > -1 Then
    Toolbar1.Buttons.Item("Remove").Enabled = True
    Toolbar1.Buttons.Item("Edit").Enabled = True
Else
    Toolbar1.Buttons.Item("Remove").Enabled = False
    Toolbar1.Buttons.Item("Edit").Enabled = False
End If
If tList.ListIndex > 0 Then
    Toolbar1.Buttons.Item("Up").Enabled = True
Else
    Toolbar1.Buttons.Item("Up").Enabled = False
End If
If tList.ListIndex < tList.ListCount - 1 And tList.ListIndex > -1 Then
    Toolbar1.Buttons.Item("Down").Enabled = True
Else
    Toolbar1.Buttons.Item("Down").Enabled = False
End If
End Sub
Private Sub Toolbar1_ButtonClick(ByVal Button As Button)
Dim TempText As String, tNum As Long
Select Case Button.Key
Case "Add"
    TempText = EditTItem.EditItem("Add Command", "", "")
    If TempText <> vbCrLf Then
        tList.AddItem Left(TempText, InStr(TempText, vbCrLf) - 1)
        ReDim Preserve tCommands(UBound(tCommands) + 1) As String
        tCommands(UBound(tCommands)) = Mid(TempText, InStr(TempText, vbCrLf) + 2)
        tList.ListIndex = tList.ListCount - 1
    End If
Case "Remove"
    For tNum = tList.ListIndex + 1 To tList.ListCount - 1
        tCommands(tNum) = tCommands(tNum + 1)
    Next tNum
    ReDim Preserve tCommands(UBound(tCommands) - 1) As String
    tList.RemoveItem tList.ListIndex
    tList_Click
Case "Edit"
    TempText = EditTItem.EditItem("Edit Command", tList.List(tList.ListIndex), tCommands(tList.ListIndex + 1))
    tList.List(tList.ListIndex) = Left(TempText, InStr(TempText, vbCrLf) - 1)
    tCommands(tList.ListIndex + 1) = Mid(TempText, InStr(TempText, vbCrLf) + 2)
Case "Up"
    TempText = tList.List(tList.ListIndex)
    tList.List(tList.ListIndex) = tList.List(tList.ListIndex - 1)
    tList.List(tList.ListIndex - 1) = TempText
    TempText = tCommands(tList.ListIndex + 1)
    tCommands(tList.ListIndex + 1) = tCommands(tList.ListIndex)
    tCommands(tList.ListIndex) = TempText
    tList.ListIndex = tList.ListIndex - 1
Case "Down"
    TempText = tList.List(tList.ListIndex)
    tList.List(tList.ListIndex) = tList.List(tList.ListIndex + 1)
    tList.List(tList.ListIndex + 1) = TempText
    TempText = tCommands(tList.ListIndex + 1)
    tCommands(tList.ListIndex + 1) = tCommands(tList.ListIndex + 2)
    tCommands(tList.ListIndex + 2) = TempText
    tList.ListIndex = tList.ListIndex + 1
End Select
End Sub
