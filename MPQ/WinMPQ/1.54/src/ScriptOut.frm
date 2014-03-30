VERSION 4.00
Begin VB.Form ScriptOut 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Script Output"
   ClientHeight    =   4575
   ClientLeft      =   480
   ClientTop       =   1275
   ClientWidth     =   9615
   Height          =   4980
   Icon            =   "ScriptOut.frx":0000
   Left            =   420
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   4575
   ScaleWidth      =   9615
   ShowInTaskbar   =   0   'False
   Top             =   930
   Width           =   9735
   Begin VB.TextBox oText 
      BackColor       =   &H8000000F&
      BorderStyle     =   0  'None
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Terminal"
         Size            =   9
         Charset         =   255
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   4575
      Left            =   0
      Locked          =   -1  'True
      MultiLine       =   -1  'True
      TabIndex        =   0
      Top             =   0
      Width           =   9615
   End
End
Attribute VB_Name = "ScriptOut"
Attribute VB_Creatable = False
Attribute VB_Exposed = False
Option Explicit

Private Sub Form_Load()
Left = (Screen.Width - Width) / 2
Top = (Screen.Height - Height) / 2
End Sub
Private Sub oText_Change()
If Len(oText) > 8192 Then oText = Right(oText, 8192)
End Sub
