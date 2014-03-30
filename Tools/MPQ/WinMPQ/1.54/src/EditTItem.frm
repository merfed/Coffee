VERSION 4.00
Begin VB.Form EditTItem 
   BorderStyle     =   3  'Fixed Dialog
   ClientHeight    =   2535
   ClientLeft      =   1890
   ClientTop       =   2145
   ClientWidth     =   5535
   Height          =   2940
   Icon            =   "EditTItem.frx":0000
   Left            =   1830
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   2535
   ScaleWidth      =   5535
   ShowInTaskbar   =   0   'False
   Top             =   1800
   Width           =   5655
   Begin VB.CommandButton cmdBrowse 
      Caption         =   "&Browse..."
      Height          =   285
      Left            =   4560
      TabIndex        =   2
      Top             =   1680
      Width           =   855
   End
   Begin VB.CommandButton cmdCancel 
      Cancel          =   -1  'True
      Caption         =   "&Cancel"
      Height          =   375
      Left            =   3120
      TabIndex        =   4
      Top             =   2040
      Width           =   1455
   End
   Begin VB.CommandButton cmdOK 
      Caption         =   "O&K"
      Default         =   -1  'True
      Height          =   375
      Left            =   960
      TabIndex        =   3
      Top             =   2040
      Width           =   1455
   End
   Begin VB.TextBox txtCommand 
      Height          =   285
      Left            =   120
      TabIndex        =   1
      Top             =   1680
      Width           =   4335
   End
   Begin VB.TextBox txtName 
      Height          =   285
      Left            =   120
      TabIndex        =   0
      Top             =   480
      Width           =   5295
   End
   Begin VB.Label Label1 
      Caption         =   "Title:  (Use a && before a letter to make it the key for the menu option.  Use &&&& to display a &&.)"
      Height          =   390
      Left            =   120
      TabIndex        =   5
      Top             =   0
      Width           =   5295
      WordWrap        =   -1  'True
   End
   Begin VB.Label Label2 
      Caption         =   $"EditTItem.frx":000C
      Height          =   870
      Left            =   120
      TabIndex        =   6
      Top             =   840
      Width           =   5295
      WordWrap        =   -1  'True
   End
End
Attribute VB_Name = "EditTItem"
Attribute VB_Creatable = False
Attribute VB_Exposed = False
Option Explicit

Dim ClickedOK As Boolean, Finished As Boolean
Function EditItem(FormCaption As String, cName As String, cCommand As String) As String
Left = ToolList.Left + 330
If Left < 0 Then Left = 0
If Left + Width > Screen.Width Then Left = Screen.Width - Width
Top = ToolList.Top + 315
If Top < 0 Then Top = 0
If Top + Height > Screen.Height Then Top = Screen.Height - Height
Caption = FormCaption
txtName = cName
txtCommand = cCommand
Finished = False
ClickedOK = False
Show 1
If ClickedOK = True Then
    EditItem = txtName + vbCrLf + txtCommand
Else
    EditItem = cName + vbCrLf + cCommand
End If
Finished = True
Unload Me
End Function
Private Sub cmdBrowse_Click()
Dim OldFileName As String, OldPath As String
CD.Flags = &H1000 Or &H4 Or &H2
CD.Filter = "Programs (*.exe;*.com;*.bat)|*.exe;*.com;*.bat|All Files (*.*)|*.*"
OldFileName = CD.FileName
OldPath = CurDir
If Mid(App.Path, 2, 1) = ":" Then
    ChDrive Left(App.Path, 1)
    ChDir Left(App.Path, 2) + "\"
End If
CD.FileName = ""
If ShowOpen(CD) = False Then GoTo Cancel
txtCommand = Chr(34) + CD.FileName + Chr(34)
Cancel:
CD.FileName = OldFileName
If Mid(OldPath, 2, 1) = ":" Then ChDrive Left(OldPath, 1)
ChDir OldPath
End Sub
Private Sub cmdCancel_Click()
Hide
End Sub
Private Sub cmdOK_Click()
ClickedOK = True
Hide
End Sub
Private Sub Form_Unload(Cancel As Integer)
If Finished = False Then
    Cancel = True
    Hide
End If
End Sub
