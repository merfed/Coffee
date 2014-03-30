VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   6345
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   9795
   LinkTopic       =   "Form1"
   ScaleHeight     =   6345
   ScaleWidth      =   9795
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton Command2 
      Caption         =   "Command2"
      Height          =   735
      Left            =   3960
      TabIndex        =   2
      Top             =   4320
      Width           =   1935
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Command1"
      Height          =   1095
      Left            =   5160
      TabIndex        =   1
      Top             =   1920
      Width           =   1695
   End
   Begin VB.FileListBox Doc1 
      Height          =   2235
      Left            =   840
      TabIndex        =   0
      Top             =   840
      Width           =   3855
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub Command1_Click()
Dim i As Integer, o As Integer, WereWMOs As Boolean
Dim Log As String, AreaNames As String, aname As String
Dim ADT As ADTFile
Dim SFSO
    
    If InStr(1, FilePath, ":\") = 0 Then
        
        FilePath = Replace(App.Path & "\", "\\", "\") & FilePath
        
    End If

    Set SFSO = CreateObject("Scripting.FileSystemObject")
    Set SFSO = SFSO.CreateTextFile("C:\MapDB.txt")
    
    For i = 0 To Doc1.ListCount
        
        Set ADT = New ADTFile
        ADT.OpenADT Doc1.Path & "\" & Doc1.List(i)
        
        SFSO.Write vbCrLf & vbCrLf & Doc1.Path & "\" & Doc1.List(i) & vbCrLf
        
        SFSO.Write "Number of doodads in this ADT:" & vbCrLf & vbTab & ADT.Doodad_Count & vbCrLf
        
        SFSO.Write "WMOs instanced in this ADT:" & vbCrLf
        
        WereWMOs = False
        For o = 1 To ADT.WMO_Count
            SFSO.Write vbTab & ADT.WMO_WMOPath(o) & vbCrLf
            WereWMOs = True
            DoEvents
        Next
        
        If WereWMOs = False Then
            SFSO.Write vbTab & "None" & vbCrLf
        End If
        
        SFSO.Write "Area-titles in this ADT:" & vbCrLf
        
        AreaNames = ""
        
        For a = 1 To 255 + 1
            
            aname = LookupAreaName(ADT.AreaIdNumber(a))
            
            If aname = "" Then aname = "Unknown Title"
            
            If InStr(1, AreaNames, vbTab & aname, vbTextCompare) = 0 Then
                
                AreaNames = AreaNames & vbTab & aname & " (AreaID=" & ADT.AreaIdNumber(a) & ")" & vbCrLf
                
            End If
            
            DoEvents
            
        Next
        
        SFSO.Write Replace(AreaNames, vbCrLf & vbCrLf, vbCrLf)
        
        DoEvents
        
    Next
    
    SFSO.Close
    
    Set SFSO = Nothing
    
End Sub

Private Sub Command2_Click()
Dim s As String, i As Long

    s = ReadFile("C:\MapDB.txt")
    i = InStr(1, s, "Echo Isles ", vbTextCompare)
    
    
End Sub

Private Sub Form_Load()
    ReDim AreaIdTable(0)
    Doc1.Path = "D:\World of Warcraft\Data\ex\Expansion01\"
End Sub
