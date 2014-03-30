VERSION 5.00
Begin VB.Form GUIBitmapAlign 
   Caption         =   "Form2"
   ClientHeight    =   3090
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   4680
   LinkTopic       =   "Form2"
   ScaleHeight     =   3090
   ScaleWidth      =   4680
   StartUpPosition =   3  'Windows Default
End
Attribute VB_Name = "GUIBitmapAlign"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Dim ClickedX As Single, ClickedY As Single

Private Sub Form_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    ClickedX = X
    ClickedY = Y
    Me.Hide
End Sub
