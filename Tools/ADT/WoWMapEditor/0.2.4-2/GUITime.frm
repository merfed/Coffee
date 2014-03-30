VERSION 5.00
Begin VB.Form GUITime 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Checking time, please wait..."
   ClientHeight    =   780
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   5445
   ControlBox      =   0   'False
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   780
   ScaleWidth      =   5445
   StartUpPosition =   2  'CenterScreen
   Begin VB.TextBox txtTimeText 
      Alignment       =   2  'Center
      BackColor       =   &H8000000F&
      BorderStyle     =   0  'None
      Height          =   495
      Left            =   120
      MultiLine       =   -1  'True
      TabIndex        =   0
      Text            =   "GUITime.frx":0000
      Top             =   120
      Width           =   5175
   End
End
Attribute VB_Name = "GUITime"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
