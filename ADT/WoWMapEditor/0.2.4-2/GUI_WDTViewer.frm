VERSION 5.00
Begin VB.Form GUI_WDTFile 
   Caption         =   "WDT Viewer"
   ClientHeight    =   7050
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   8595
   LinkTopic       =   "Form1"
   ScaleHeight     =   7050
   ScaleWidth      =   8595
   StartUpPosition =   3  'Windows Default
   Begin VB.Frame Frame1 
      Height          =   2175
      Left            =   6000
      TabIndex        =   1
      Top             =   360
      Width           =   2295
      Begin VB.CommandButton btnSave 
         Caption         =   "Save Changes"
         Height          =   495
         Left            =   120
         TabIndex        =   4
         Top             =   1440
         Width           =   2055
      End
      Begin VB.CheckBox chDel 
         Caption         =   "Delete tiles where I click"
         Height          =   255
         Left            =   120
         TabIndex        =   3
         Top             =   720
         Width           =   2055
      End
      Begin VB.CheckBox chAdd 
         Caption         =   "Add tiles where I click"
         Height          =   255
         Left            =   120
         TabIndex        =   2
         Top             =   360
         Width           =   2055
      End
   End
   Begin VB.PictureBox DrawBox 
      AutoRedraw      =   -1  'True
      Height          =   6615
      Left            =   120
      MouseIcon       =   "GUI_WDTViewer.frx":0000
      MousePointer    =   99  'Custom
      ScaleHeight     =   6555
      ScaleWidth      =   5715
      TabIndex        =   0
      Top             =   360
      Width           =   5775
   End
   Begin VB.Label lblMouse 
      Height          =   255
      Left            =   120
      TabIndex        =   5
      Top             =   120
      Width           =   5775
   End
End
Attribute VB_Name = "GUI_WDTFile"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Public TargFile     As String
Public TargFileMain As String
Public TargFilePreMain As String

Const BlockDrawWidth    As Integer = 100
Const BlockDrawHeight   As Integer = 100

Sub SaveWDT(ByVal FilePath As String)
    
    WriteFile FilePath, TargFilePreMain & TargFileMain
    
End Sub

Private Sub btnSave_Click()
Dim s As String
    
    s = InputBox("To what path would you like to save the new file to?", , TargFile)
    If s = "" Then Exit Sub
    SaveWDT s
    
    MsgBox "Saved." & vbCrLf & "Please note that you must supply the .adt files that you created in order for WoW to not crash." & vbCrLf & "You also much corect the X,Y locations of each chunk within the ADT file to the target location for anything to show up.... Just copying one adt-file to another that normally dosnt exist will NOT work (unless you corected the X,Y locations)", vbInformation
    
End Sub

Private Sub chAdd_Click()
    If chAdd.Value = Checked Then chDel.Value = vbUnchecked
End Sub

Private Sub chDel_Click()
    If chDel.Value = Checked Then chAdd.Value = vbUnchecked
End Sub

Private Sub DrawBox_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
Dim qX As Integer, qY As Integer
    
    DrawBox.Cls
    DrawBox.DrawWidth = 2
    
    For qX = 0 To 63
        If qX * BlockDrawWidth > X Then Exit For
    Next
    
    For qY = 0 To 63
        If qY * BlockDrawWidth > Y Then Exit For
    Next
    
    If chAdd.Value = vbChecked Then SetHaveTile qX, qY, True
    If chDel.Value = vbChecked Then SetHaveTile qX, qY, False
    
    DrawWorld
    
End Sub

Private Sub DrawBox_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
Dim qX As Integer, qY As Integer
    
    If Button = vbLeftButton Then DrawBox_MouseDown Button, Shift, X, Y
    
    DrawBox.Cls
    DrawBox.DrawWidth = 2
    
    For qX = 0 To 63
        If qX * BlockDrawWidth > X Then Exit For
    Next
    
    For qY = 0 To 63
        If qY * BlockDrawWidth > Y Then Exit For
    Next
    
    DrawBox.Line (BlockDrawWidth * qX, BlockDrawHeight * qY)-(BlockDrawWidth * (qX + 1), BlockDrawHeight * qY), vbWhite
    DrawBox.Line (BlockDrawWidth * qX, BlockDrawHeight * qY)-(BlockDrawWidth * qX, BlockDrawHeight * (qY + 1)), vbWhite
    DrawBox.Line (BlockDrawWidth * (qX + 1), BlockDrawHeight * (qY + 1))-(BlockDrawWidth * (qX + 1), BlockDrawHeight * qY), vbWhite
    DrawBox.Line (BlockDrawWidth * (qX + 1), BlockDrawHeight * (qY + 1))-(BlockDrawWidth * qX, BlockDrawHeight * (qY + 1)), vbWhite
    
    lblMouse.Caption = "Mouse tile target = " & qX & ", " & qY
    
End Sub

Private Sub Form_Click()
Dim X As Integer
Dim Y As Integer
Dim b As Boolean

    For X = 0 To 63
    For Y = 0 To 63
        
        If 36 <= X And X <= 38 Then
            
            
            If 32 <= Y And Y <= 34 Then
            
                b = True
            
            Else
                
                b = False
                
            End If
            
            
        Else
            
            b = False
            
        End If
        
        
        SetHaveTile X, Y, b
        
        DoEvents
        
    Next
    Next
    
    DrawWorld
    
    SaveWDT "C:\test.wdt"
    
End Sub

Sub LoadFile(sTargFile As String)
Dim s As String

    TargFile = sTargFile

    s = ReadFile(TargFile)
    TargFilePreMain = Mid(s, 1, InStr(1, s, "NIAM") + 3 + 4)
    s = Mid(s, InStr(1, s, "NIAM") + 4 + 4)
    TargFileMain = s
    
    Me.Show
    DrawWorld
    
    Me.Width = DrawBox.Width + (DrawBox.Left * 2) + Frame1.Width + 240
    Me.Height = DrawBox.ScaleHeight + (DrawBox.Top * 2) + 480
    Frame1.Left = DrawBox.Width + 240
    
    
End Sub

Sub DrawWorld()
Dim X As Integer, Y As Integer
    
    DrawBox.Picture = Nothing
    DrawBox.Cls
    
    DrawBox.DrawWidth = 1
    
    For Y = 0 To 63
    For X = 0 To 63
        
        If DoesHaveTile(X, Y) = True Then
            
            DrawBox.Line (BlockDrawWidth * X, BlockDrawHeight * Y)-(BlockDrawWidth * (X + 1), BlockDrawHeight * Y)
            DrawBox.Line (BlockDrawWidth * X, BlockDrawHeight * Y)-(BlockDrawWidth * X, BlockDrawHeight * (Y + 1))
            DrawBox.Line (BlockDrawWidth * (X + 1), BlockDrawHeight * (Y + 1))-(BlockDrawWidth * (X + 1), BlockDrawHeight * Y)
            DrawBox.Line (BlockDrawWidth * (X + 1), BlockDrawHeight * (Y + 1))-(BlockDrawWidth * X, BlockDrawHeight * (Y + 1))
            
            DrawBox.Line (BlockDrawWidth * (X + 1), BlockDrawHeight * (Y + 1))-(BlockDrawWidth * X, BlockDrawHeight * Y)
            DrawBox.Line (BlockDrawWidth * (X + 1), BlockDrawHeight * Y)-(BlockDrawWidth * X, BlockDrawHeight * (Y + 1))
            
        End If
        
    Next
    Next
    
    DrawBox.Width = 64 * BlockDrawWidth
    DrawBox.Height = 64 * BlockDrawHeight
    
    DrawBox.Picture = DrawBox.Image
    
End Sub

Sub SetHaveTile(ByVal X As Integer, ByVal Y As Integer, DoesHave As Boolean)
Dim id As Integer, Offset As Integer
Dim SetValue As String

'   // Where (1,1) is the top left corner

    id = (Y * 64) + X
    Offset = id
    Offset = (Offset * 8) + 1 + 1
    
    If DoesHave Then
        SetValue = Chr(1)
    Else
        SetValue = Chr(0)
    End If
    
    TargFileMain = " " & TargFileMain
    
    'MsgBox Len(TargFileMain)
    TargFileMain = Mid(TargFileMain, 1, Offset - 1) & SetValue & Mid(TargFileMain, Offset + 1)
    'MsgBox Len(TargFileMain)
    
    TargFileMain = Mid(TargFileMain, 2)
    
    Set DrawBox.Picture = Nothing
    DrawBox.Cls
    
End Sub

Function DoesHaveTile(ByVal X As Integer, ByVal Y As Integer) As Boolean
Dim id      As Integer
Dim Offset  As Long

'   // Where (1,1) is the top left corner

    id = (Y * 64) + X
    Offset = id
    Offset = (Offset * 8) + 1
    
    If Asc(Mid(TargFileMain, Offset, 1)) = 1 Then
        
        DoesHaveTile = True
    
    ElseIf Asc(Mid(TargFileMain, Offset, 1)) = 0 Then
    
        DoesHaveTile = False
        
    Else
    
        Err.Raise 1
    
    End If

End Function
