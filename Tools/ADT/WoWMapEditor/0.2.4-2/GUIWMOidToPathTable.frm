VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomctl.ocx"
Begin VB.Form GUIWMOidToPathTable 
   Caption         =   ".WMO ID-to-Path table"
   ClientHeight    =   3930
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   8295
   LinkTopic       =   "Form2"
   ScaleHeight     =   3930
   ScaleWidth      =   8295
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton btnAddNew 
      Caption         =   "Add"
      Height          =   375
      Left            =   7080
      TabIndex        =   4
      Top             =   840
      Width           =   1095
   End
   Begin VB.CommandButton btnChange 
      Caption         =   "Change"
      Height          =   375
      Left            =   7080
      TabIndex        =   3
      Top             =   1320
      Width           =   1095
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Close"
      Height          =   375
      Left            =   7080
      TabIndex        =   2
      Top             =   3360
      Width           =   1095
   End
   Begin MSComctlLib.ListView ListView1 
      Height          =   2895
      Left            =   240
      TabIndex        =   1
      Top             =   840
      Width           =   6735
      _ExtentX        =   11880
      _ExtentY        =   5106
      View            =   3
      LabelWrap       =   -1  'True
      HideSelection   =   -1  'True
      FullRowSelect   =   -1  'True
      _Version        =   393217
      ForeColor       =   -2147483640
      BackColor       =   -2147483643
      BorderStyle     =   1
      Appearance      =   1
      NumItems        =   2
      BeginProperty ColumnHeader(1) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         Object.Width           =   1305
      EndProperty
      BeginProperty ColumnHeader(2) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   1
         Object.Width           =   7126
      EndProperty
   End
   Begin VB.Label Label1 
      Caption         =   $"GUIWMOidToPathTable.frx":0000
      Height          =   615
      Left            =   240
      TabIndex        =   0
      Top             =   120
      Width           =   7935
   End
End
Attribute VB_Name = "GUIWMOidToPathTable"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Public ADT As New ADTFile

Private Sub btnAddNew_Click()
Dim s As String
    s = InputBox("Input a full path to your target.wmo within a .MPQ file in the WoW Data directory", "Input path")
    If s = "" Then Exit Sub
    ADT.WMOPaths_Add s
    LoadList
End Sub

Private Sub btnChange_Click()
Dim s As String, CurrVal As String

    If ListView1.SelectedItem Is Nothing Then
        MsgBox "Select something first", , "Duh"
        Exit Sub
    End If
    
    CurrVal = ListView1.SelectedItem.SubItems(1)
    s = InputBox("What path would you like to set this id to?" & vbCrLf & "Please set this to a path to a .wmo file within a .MPQ file in the WoW data directory", , CurrVal)
    If s = "" Then Exit Sub
    
    ListView1.SelectedItem.SubItems(1) = s
    
    ADT.WMOPaths_Item(ListView1.SelectedItem.Text) = s
    
End Sub

Private Sub Command1_Click()
    Unload Me
End Sub

Sub LoadList()
Dim i As Integer, a() As String

    a = ADT.WMOPaths_Array
    
    ListView1.ListItems.Clear
    
    For i = 0 To UBound(a)
        
        With ListView1.ListItems
            .Add .Count + 1, , i
            .Item(.Count).SubItems(1) = a(i)
        End With
        
    Next
    
End Sub

