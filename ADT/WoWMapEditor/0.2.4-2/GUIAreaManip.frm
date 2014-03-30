VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Begin VB.Form GUIAreaManip 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Select area Id/Title"
   ClientHeight    =   4365
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   8535
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   4365
   ScaleWidth      =   8535
   StartUpPosition =   3  'Windows Default
   Begin VB.TextBox txtSearch 
      Height          =   285
      Left            =   1200
      TabIndex        =   4
      Top             =   3840
      Width           =   3375
   End
   Begin VB.CommandButton Command2 
      Caption         =   "Ok"
      Height          =   375
      Left            =   7200
      TabIndex        =   2
      Top             =   3240
      Width           =   1215
   End
   Begin VB.CommandButton btnAddArea 
      Caption         =   "Add"
      Height          =   375
      Left            =   7200
      TabIndex        =   1
      Top             =   360
      Width           =   1215
   End
   Begin MSComctlLib.ListView AreaView 
      Height          =   3255
      Left            =   120
      TabIndex        =   0
      Top             =   360
      Width           =   6975
      _ExtentX        =   12303
      _ExtentY        =   5741
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
         Text            =   "Id"
         Object.Width           =   2540
      EndProperty
      BeginProperty ColumnHeader(2) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   1
         Text            =   "Name"
         Object.Width           =   5009
      EndProperty
   End
   Begin VB.Label Label2 
      Caption         =   "Filter / Search"
      Height          =   255
      Left            =   120
      TabIndex        =   5
      Top             =   3840
      Width           =   1695
   End
   Begin VB.Label Label1 
      Caption         =   "Which area do you want it to be?"
      Height          =   255
      Left            =   120
      TabIndex        =   3
      Top             =   120
      Width           =   2895
   End
End
Attribute VB_Name = "GUIAreaManip"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Dim a() As String ' will hold the AreaTable.csv file in memory, array split by each line

Private Sub btnAddArea_Click()
    MsgBox "todo" & vbCrLf & "Blah, im lazy ok", vbExclamation, "todo"
End Sub

Private Sub Command2_Click()
    Me.Hide
End Sub

Private Sub Form_Load()
Dim s As String
    
    s = ReadFile("AreaTable.csv")
    If s = "" Then MsgBox "Error reading AreaTable.csv", vbCritical
    a = Split(s, vbCrLf)
    
    FilterView ""
    
End Sub

Sub FilterView(txt As String)
Dim x As Integer, b() As String
    
    With AreaView.ListItems
    
        .Clear
    
        For x = 0 To UBound(a)
            
            b = Split(a(x), ",")
            
            If UBound(b) >= 11 Then
                If InStr(1, b(11), txt, vbTextCompare) <> 0 Or b(11) = "" Then
                    .Add .Count + 1, , Right("000" & b(0), 4)
                    .Item(.Count).SubItems(1) = b(11)
                End If
            End If
            
            DoEvents
            
        Next
        
    End With
    
    AreaView.SortKey = 0
    AreaView.Sorted = True
    
    
End Sub

Private Sub txtSearch_Change()
    
    FilterView txtSearch.Text
    
End Sub
