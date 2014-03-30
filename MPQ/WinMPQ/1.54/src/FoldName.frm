VERSION 4.00
Begin VB.Form FoldName 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Folder name..."
   ClientHeight    =   1935
   ClientLeft      =   1995
   ClientTop       =   2430
   ClientWidth     =   4575
   Height          =   2340
   Icon            =   "FoldName.frx":0000
   Left            =   1935
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1935
   ScaleWidth      =   4575
   ShowInTaskbar   =   0   'False
   Top             =   2085
   Width           =   4695
   Begin VB.TextBox Text1 
      Height          =   285
      Left            =   120
      TabIndex        =   1
      Top             =   1080
      Width           =   4335
   End
   Begin VB.CommandButton Command2 
      Cancel          =   -1  'True
      Caption         =   "&Cancel"
      Height          =   375
      Left            =   2640
      TabIndex        =   3
      Top             =   1440
      Width           =   1335
   End
   Begin VB.CommandButton Command1 
      Caption         =   "O&K"
      Default         =   -1  'True
      Height          =   375
      Left            =   600
      TabIndex        =   2
      Top             =   1440
      Width           =   1335
   End
   Begin VB.Label Label2 
      Caption         =   "Example:  If ""title.wav"" is the original filename, and you type ""music\"" the file will become ""music\title.wav"""
      Height          =   495
      Left            =   120
      TabIndex        =   4
      Top             =   600
      Width           =   4335
      WordWrap        =   -1  'True
   End
   Begin VB.Label Label1 
      Caption         =   "The text in the box below will be put at the beginnings of the names of every file you selected."
      Height          =   510
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   4335
      WordWrap        =   -1  'True
   End
End
Attribute VB_Name = "FoldName"
Attribute VB_Creatable = False
Attribute VB_Exposed = False
Option Explicit

Private Sub Command1_Click()
AddFolderName = Text1
Unload Me
End Sub
Private Sub Command2_Click()
AddFolderName = ""
Unload Me
End Sub
Private Sub Form_Load()
Left = MpqEx.Left + 330 * 2
If Left < 0 Then Left = 0
If Left + Width > Screen.Width Then Left = Screen.Width - Width
Top = MpqEx.Top + 315 * 2
If Top < 0 Then Top = 0
If Top + Height > Screen.Height Then Top = Screen.Height - Height
Text1 = AddFolderName
End Sub
