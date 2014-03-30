VERSION 5.00
Begin VB.Form GUIDebug 
   BorderStyle     =   4  'Fixed ToolWindow
   Caption         =   "Debug data"
   ClientHeight    =   2820
   ClientLeft      =   45
   ClientTop       =   315
   ClientWidth     =   7635
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   2820
   ScaleWidth      =   7635
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows Default
   Begin VB.TextBox Text1 
      BeginProperty Font 
         Name            =   "Fixedsys"
         Size            =   9
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   2535
      Left            =   120
      MultiLine       =   -1  'True
      ScrollBars      =   3  'Both
      TabIndex        =   0
      Top             =   120
      Width           =   7335
   End
End
Attribute VB_Name = "GUIDebug"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub Form_Load()

    DBugWndoShown = True
    
    Me.Left = Screen.Width - Me.ScaleWidth - 120
    Me.Top = Screen.Height - Me.ScaleHeight - 800
    
    Me.Show
    
End Sub

Private Sub Form_Terminate()
    DBugWndoShown = False
End Sub

Private Sub Form_Unload(Cancel As Integer)
    DBugWndoShown = False
End Sub
