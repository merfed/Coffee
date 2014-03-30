VERSION 5.00
Begin VB.Form test 
   Caption         =   "Form1"
   ClientHeight    =   7260
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   11070
   LinkTopic       =   "Form1"
   ScaleHeight     =   7260
   ScaleWidth      =   11070
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton Command1 
      Caption         =   "Command1"
      Height          =   1455
      Left            =   5760
      TabIndex        =   1
      Top             =   2520
      Width           =   2895
   End
   Begin VB.FileListBox Doc1 
      Height          =   2625
      Left            =   720
      TabIndex        =   0
      Top             =   2040
      Width           =   4095
   End
End
Attribute VB_Name = "test"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub Command1_Click()
    
    Dim w As WMOGrpFile
    
    Doc1.Path = "C:\Documents and Settings\Name\Desktop\Wail\KL_Wailing\World\wmo\Dungeon\KL_Wailing\"
    Doc1.Refresh
    
    Me.Show
    
    
    Dim i As Integer
    
    For i = 1 To Doc1.ListCount

        Set w = New WMOGrpFile
        
        w.OpenWMO Replace(Doc1.Path & "\" & Doc1.List(i), "\\", "\")
        
        'If w.QILM <> "" Then
        '    MsgBox Doc1.List(i)
        '    DoEvents
        'End If
        
        'w.QILM = ReadFile("c:\water.txt")
        'Kill Replace(Doc1.Path & "\" & Doc1.List(i), "\\", "\")
        
        w.FillWMOWithWater
        w.SaveWMO Replace(Doc1.Path & "\" & Doc1.List(i), "\\", "\")
        
        DebugPrint "Edited file: " & Replace(Doc1.Path & "\" & Doc1.List(i), "\\", "\")
        
        DoEvents
        
    Next
    
End Sub

Private Sub Form_Load()
Dim i As Integer, Lowest As Integer


    'Dim w As New WMOGrpFile
    
    'w.OpenWMO "C:\Documents and Settings\Name\Desktop\SW\World\wmo\Azeroth\Buildings\Stormwind\Stormwind_096.wmo"
    
    'MsgBox w.WaterLocX
    'MsgBox w.WaterLocY
    
    
    'w.FillWMOWithWater
    'w.SaveWMO "C:\Documents and Settings\Name\Desktop\SW\World\wmo\Azeroth\Buildings\Stormwind\Stormwind_096(100).wmo"
    
    
End Sub
