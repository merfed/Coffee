VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   5340
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   9315
   LinkTopic       =   "Form1"
   ScaleHeight     =   5340
   ScaleWidth      =   9315
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton Command2 
      Caption         =   "Command2"
      Height          =   1215
      Left            =   4680
      TabIndex        =   2
      Top             =   2760
      Width           =   3255
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Command1"
      Height          =   615
      Left            =   3600
      TabIndex        =   1
      Top             =   1080
      Width           =   1815
   End
   Begin VB.FileListBox Doc1 
      Height          =   2625
      Left            =   1200
      TabIndex        =   0
      Top             =   1080
      Width           =   2295
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Command1_Click()
Dim X As Integer, Y As Integer
Dim WMO As New WMOGrpFile
Dim Flags As String

    For X = 0 To Doc1.ListCount
        
        If InStr(1, Doc1.List(X), "Stormwind_") <> 0 And InStr(1, Doc1.List(X), ".wmo") <> 0 And Doc1.List(X) <> "Stormwind.wmo" Then
        
            WMO.OpenWMO Doc1.Path & "\" & Doc1.List(X)
            
            'For y = 1 To WMO.RDOM_DoodadCount
            '    'If InStr(1, Flags, wmo.RDOM_DoodadID(y) & vbCrLf) = 0 Then Flags = Flags & wmo.RDOM_DoodadID(y) & vbCrLf
            '    WMO.RDOM_DoodadID(y) = "½"
            'Next
            '
            'WMO.SaveWMO Doc1.Path & "\" & Doc1.List(x)
                        
                        
                        
                        
                        
                        
            DoEvents
            
            Me.Caption = Round((X / Doc1.ListCount) * 100)
            
        End If
    Next
    
    Debug.Print Flags
    
End Sub

Private Sub Command2_Click()
Dim X As Integer

    For X = 0 To Doc1.ListCount
        
        If InStr(1, Doc1.List(X), "Stormwind_007.wmo") = 0 And InStr(1, Doc1.List(X), "Stormwind_") <> 0 Then
        
            Kill Doc1.Path & "\" & Doc1.List(X)
            FileCopy Doc1.Path & "\Stormwind_007.wmo", Doc1.Path & "\" & Doc1.List(X)
            
        End If
    Next

End Sub

Private Sub Form_Load()
Dim s As String, b As Long, c As Long
Dim a() As String


    Dim atd As New ADTFile
    atd.OpenADT "C:\Documents and Settings\Name\Desktop\PVPZone03_29_29.adt"
    MsgBox atd.DoodadPaths_Count
    End
    '
    'For x = 1 To atd.WMO_Count
    '
    '    atd.WMO_PosString(x) = (String(12, Chr(1)))
    '
    'Next
    '
    'atd.SaveADT "C:\Documents and Settings\Name\Desktop\Azeroth_31_49.adt"
    
    End

    's = ReadFile("C:\Documents and Settings\Name\Desktop\Dummy MPQ\Ironforge\ironforge.wmo")
    'b = InStr(1, s, "NGOM")
    'c = InStr(1, s, "IGOM")
    's = Mid(s, b, c - b)
    'a = Split(s, Chr(0))
    'MsgBox UBound(a)
    'End
    
    'Clipboard.Clear
    'Clipboard.SetText Dec2Bin(String32ToLong(Chr(108) & Chr(59) & Chr(1) & Chr(0)))
    'End
    
    'Dim w As New WMOGrpFile
    'w.OpenWMO "C:\Documents and Settings\Name\Desktop\Dummy MPQ\Ironforge\ironforge_090.wmo"
    'MsgBox w.WMOHeadder_GrpName
    'End
    
'Dim x As Integer, y As Integer
'Dim WMO As New WMOGrpFile
'Dim Flags As String
'Dim Counter(255) As Integer
'Dim ThisOne As Integer
'Dim arr() As String
'
'    WMO.OpenWMO "C:\Documents and Settings\Name\Desktop\Dummy MPQ\Ironforge\ironforge_090.wmo"
'
'    For y = 1 To WMO.YPOM_TriangleCount
'
'        ThisOne = WMO.YPOM_TriangleInfo_MaterialID(y)
'
'        If InStr(1, Flags & vbCrLf, ThisOne & vbCrLf) = 0 Then
'            Flags = Flags & ThisOne & vbCrLf
'        End If
'
'        Counter(ThisOne) = Counter(ThisOne) + 1
'
'    Next
'
'    'WMO.SaveWMO Doc1.Path & "\" & Doc1.List(x)
'
'    Flags = Replace(Flags, vbCrLf, " ")
'    Flags = Trim(Flags)
'    arr = Split(Flags, " ")
'    For y = 0 To UBound(arr)
'        Debug.Print arr(y) & " - " & Counter(arr(y))
'    Next
'
'    End
    
    
End Sub
