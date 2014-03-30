VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Begin VB.Form GUIStart 
   Caption         =   "Welcome"
   ClientHeight    =   8205
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   7590
   LinkTopic       =   "Form2"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   8205
   ScaleWidth      =   7590
   StartUpPosition =   2  'CenterScreen
   Begin VB.CommandButton btnMulti 
      Caption         =   "Show Wizard"
      Height          =   375
      Left            =   480
      TabIndex        =   9
      Top             =   7560
      Width           =   2775
   End
   Begin VB.ListBox lstAreas 
      Height          =   2010
      ItemData        =   "GUIStart.frx":0000
      Left            =   480
      List            =   "GUIStart.frx":0B77
      TabIndex        =   7
      Top             =   4320
      Width           =   6855
   End
   Begin VB.CommandButton vtnShowAssocADTs 
      Caption         =   "Show Associated Maps"
      Height          =   375
      Left            =   480
      TabIndex        =   6
      Top             =   3840
      Width           =   2775
   End
   Begin MSComDlg.CommonDialog CommonDialog1 
      Left            =   6960
      Top             =   120
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.CommandButton btnLoadPrvMap 
      Caption         =   "Open Selected"
      Height          =   375
      Left            =   480
      TabIndex        =   3
      Top             =   1680
      Width           =   2775
   End
   Begin VB.ListBox List1 
      Height          =   1035
      Left            =   480
      TabIndex        =   2
      Top             =   2160
      Width           =   6855
   End
   Begin VB.CommandButton btnOpenNew 
      Caption         =   "Open a new .adt/.wmo/.wdt file"
      Height          =   375
      Left            =   480
      TabIndex        =   1
      Top             =   600
      Width           =   2775
   End
   Begin VB.Label Label3 
      Caption         =   "Edit multiple maps or instances at once"
      Height          =   255
      Index           =   2
      Left            =   120
      TabIndex        =   10
      Top             =   7200
      Width           =   3135
   End
   Begin VB.Label Label3 
      Caption         =   $"GUIStart.frx":4B36
      Height          =   495
      Index           =   1
      Left            =   480
      TabIndex        =   8
      Top             =   6480
      Width           =   6855
   End
   Begin VB.Label Label3 
      Caption         =   "Find map files (ADT) by region name?"
      Height          =   255
      Index           =   0
      Left            =   120
      TabIndex        =   5
      Top             =   3480
      Width           =   2895
   End
   Begin VB.Label Label2 
      Caption         =   "Open a previously editted  file?"
      Height          =   255
      Left            =   120
      TabIndex        =   4
      Top             =   1320
      Width           =   2895
   End
   Begin VB.Label Label1 
      Caption         =   "Open new?"
      Height          =   255
      Left            =   120
      TabIndex        =   0
      Top             =   240
      Width           =   975
   End
End
Attribute VB_Name = "GUIStart"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Public TargFile As String

Private Sub btnLoadPrvMap_Click()
    
    If List1.Text = "" Then
        MsgBox "Select something first n00b", , "Duh"
        Exit Sub
    End If
    
    TargFile = List1.Text

    PreformOpen

End Sub

Private Sub btnMultiMaps_Click()
    
    Load GUIMultiMaps
    GUIMultiMaps.Show
    Unload Me
    
End Sub

Private Sub btnMulti_Click()
    
    Load GUIMultiMaps
    GUIMultiMaps.Show
    
    Unload Me
    
End Sub

Private Sub btnOpenNew_Click()
    
    CommonDialog1.FileName = "*.adt;*.wdt;*.wmo"
    CommonDialog1.Filter = "*.adt;*.wdt;*.wmo"
    CommonDialog1.FilterIndex = 0
    CommonDialog1.Flags = cdlOFNFileMustExist
    CommonDialog1.ShowOpen
    
    TargFile = CommonDialog1.FileName
    
    If TargFile = "" Then Exit Sub
    If InStr(1, TargFile, "*") <> 0 Then Exit Sub
    If CommonDialog1.CancelError = True Then Exit Sub
    
    List1.AddItem TargFile, 0
    SavePrevMaps
    TargFile = CommonDialog1.FileName
    
    PreformOpen
    
End Sub

Sub PreformOpen()


    If InStr(1, TargFile, ".adt", vbTextCompare) <> 0 Then
        
        Me.Hide
        Load GUI_ADTFile
        
        '//MsgBox "Skipping security check"
        '//GoTo SkipBlah
        
        If PrepOpen = False Then
            End
        Else
            
SkipBlah:
    
            'Clipboard.Clear
            'Clipboard.SetText "'//" & TargFile & vbCrLf & _
            '                  "if Instr(1, GUI_ADTFile.ADT.getadt_RDHM,        """ & Mid(GUI_ADTFile.ADT.getadt_RDHM, 22, 73) & """) <> 0 then" & vbCrLf & _
            '                  "if Instr(1, GUI_ADTFile.ADT.getadt_KNCM(1, 1),  """ & Strings.Left(GUI_ADTFile.ADT.getadt_KNCM(1, 1), 70) & """) = 1 then" & vbCrLf & _
            '                  "if instr(1, GUI_ADTFile.ADT.getadt_KNCM(1, 2),  """ & Strings.Left(GUI_ADTFile.ADT.getadt_KNCM(1, 2), 70) & """) = 1 then" & vbCrLf & _
            '                  "if instr(1, GUI_ADTFile.ADT.getadt_KNCM(1, 3),  """ & Strings.Left(GUI_ADTFile.ADT.getadt_KNCM(1, 3), 70) & """) = 1 then" & vbCrLf & _
            '                  "if instr(1, GUI_ADTFile.ADT.getadt_KNCM(1, 4),  """ & Strings.Left(GUI_ADTFile.ADT.getadt_KNCM(1, 4), 70) & """) = 1 then" & vbCrLf & _
            '                  "if instr(1, GUI_ADTFile.ADT.getadt_KNCM(3, 1),  """ & Strings.Left(GUI_ADTFile.ADT.getadt_KNCM(3, 1), 70) & """) = 1 then" & vbCrLf & _
            '                  "if instr(1, GUI_ADTFile.ADT.getadt_KNCM(3, 2),  """ & Strings.Left(GUI_ADTFile.ADT.getadt_KNCM(3, 2), 70) & """) = 1 then" & vbCrLf & _
            '                  "if instr(1, GUI_ADTFile.ADT.getadt_KNCM(3, 3),  """ & Strings.Left(GUI_ADTFile.ADT.getadt_KNCM(3, 3), 70) & """) = 1 then" & vbCrLf & _
            '                  "if instr(1, GUI_ADTFile.ADT.getadt_KNCM(7, 2),  """ & Strings.Left(GUI_ADTFile.ADT.getadt_KNCM(7, 2), 70) & """) = 1 then" & vbCrLf & _
            '                  "    callAcDen" & vbCrLf & _
            '                  "    end" & vbCrLf & _
            '                  "end if" & vbCrLf & _
            '                  "end if" & vbCrLf & _
            '                  "end if" & vbCrLf & _
            '                  "end if" & vbCrLf & _
            '                  "end if" & vbCrLf & _
            '                  "end if" & vbCrLf & _
            '                  "end if" & vbCrLf & _
            '                  "end if" & vbCrLf & _
            '                  "end if" & vbCrLf & vbCrLf
            'End
            
            GUI_ADTFile.Show
            Unload Me
        
        End If
        
    ElseIf InStr(1, TargFile, ".wdt", vbTextCompare) <> 0 Then
        
        Load GUI_WDTFile
        GUI_WDTFile.LoadFile TargFile
        GUI_WDTFile.Show
        
        Unload Me
        
    ElseIf InStr(1, TargFile, ".wmo", vbTextCompare) <> 0 Then
        
        ' // Is it a root file or a group file?
        
        If InStr(1, ReadFile(TargFile), "TMOM", vbTextCompare) <> 0 Then
            
            Load GUI_WMORoot
            GUI_WMORoot.Show
            Unload Me
            
        ElseIf InStr(1, ReadFile(TargFile), "IVOM", vbTextCompare) <> 0 Then
        
            Load GUIWMODraw
            Set GUIWMODraw.WMO = New WMOGrpFile
            GUIWMODraw.WMO.OpenWMO TargFile
            GUIWMODraw.Show
            Unload Me
            
        Else
        
            MsgBox "Error: This .wmo file seems to be currupted."
        
        End If
    
    ElseIf InStr(1, TargFile, ".MPQ", vbTextCompare) <> 0 Then
        
        MsgBox "Dude, you need to use MyWoWStudio or WinMPQ to work with MPQ archives." & vbCrLf & "This program only messes with .ADT and .WMO files within the .MPQs", vbInformation
        
    Else
    
        MsgBox "Error: Unknown file extension.  This program can only work with .adt and .wmo files", vbExclamation
        
    End If

End Sub

Function PrepOpen() As Boolean
Dim s As String, i As Integer

PrepOpen = True

'For i = 1 To 255
'    If InStr(1, s, GUI_ADTFile.ADT.AreaIdNumber(i) & vbCrLf) = 0 Then
'        s = s & GUI_ADTFile.ADT.AreaIdNumber(i) & vbCrLf
'    End If
'Next
'Clipboard.Clear
'Clipboard.SetText s

For i = 1 To 255
    If GUI_ADTFile.adt.AreaIdNumber(i) = 3320 Then PrepOpen = False: callAcDen: End:  Unload GUI_ADTFile
    If GUI_ADTFile.adt.AreaIdNumber(i) = 3321 Then PrepOpen = False: callAcDen: End:  Unload GUI_ADTFile
    If GUI_ADTFile.adt.AreaIdNumber(i) = 3358 Then PrepOpen = False: callAcDen: End:  Unload GUI_ADTFile
    If GUI_ADTFile.adt.AreaIdNumber(i) = 3417 Then PrepOpen = False: callAcDen: End:  Unload GUI_ADTFile
    If GUI_ADTFile.adt.AreaIdNumber(i) = 3277 Then PrepOpen = False: callAcDen: End:  Unload GUI_ADTFile
    If GUI_ADTFile.adt.AreaIdNumber(i) = 3421 Then PrepOpen = False: callAcDen: End:  Unload GUI_ADTFile
    If GUI_ADTFile.adt.AreaIdNumber(i) = 3422 Then PrepOpen = False: callAcDen: End:  Unload GUI_ADTFile
    If GUI_ADTFile.adt.AreaIdNumber(i) = 3423 Then PrepOpen = False: callAcDen: End:  Unload GUI_ADTFile
    If GUI_ADTFile.adt.AreaIdNumber(i) = 3424 Then PrepOpen = False: callAcDen: End:  Unload GUI_ADTFile
    If GUI_ADTFile.adt.AreaIdNumber(i) = 3820 Then PrepOpen = False: callAcDen: End:  Unload GUI_ADTFile
    If GUI_ADTFile.adt.AreaIdNumber(i) = 3869 Then PrepOpen = False: callAcDen: End:  Unload GUI_ADTFile
    If GUI_ADTFile.adt.AreaIdNumber(i) = 3872 Then PrepOpen = False: callAcDen: End:  Unload GUI_ADTFile
    If GUI_ADTFile.adt.AreaIdNumber(i) = 3871 Then PrepOpen = False: callAcDen: End:  Unload GUI_ADTFile
    If GUI_ADTFile.adt.AreaIdNumber(i) = 3870 Then PrepOpen = False: callAcDen: End:  Unload GUI_ADTFile
Next

Exit Function

'//C:\a\good\Maps\Maps\PVPZone04\PVPZone04_28_29.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-129-16-0-0-137-16-0-0-145-16-0-0-153-16-0-0-161-16-0-0-169") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-0-81-97-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "16-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-35-47-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
    callAcDen
    End
    PrepOpen = False: Unload GUI_ADTFile
End If
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\a\good\Maps\Maps\PVPZone04\PVPZone04_28_28.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-129-16-0-0-137-16-0-0-145-16-0-0-153-16-0-0-161-16-0-0-169") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
    callAcDen
    End
    PrepOpen = False: Unload GUI_ADTFile
End If
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\a\good\Maps\Maps\PVPZone04\PVPZone04_29_29.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-94-17-0-0-132-35-0-0-176-36-0-0-239-37-0-0-11-38-0-0-143-1") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "16-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-35-47-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "16-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-35-47-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-39-227-116-30-192-105-247-183-103-222-189-101-239-176-96-18-") = 1 Then
    callAcDen
    End
End If
    PrepOpen = False: Unload GUI_ADTFile
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\a\good\Maps\Maps\PVPZone04\PVPZone04_29_30.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-94-17-0-0-240-37-0-0-64-39-0-0-144-39-0-0-156-39-0-0-224-1") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-126-0-0-126-0-22") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "16-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-35-47-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-240-80-184-192-184-128-115-193-60-82-210-193-178-191-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-41-170-83-56-158-56-66-156-41-50-151-49-30-169-87-71-4-104-1") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "32-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-35-47-0-0-1-0-0-0-0-1-0-0-0-0-0-0-38-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-75-169-52-29-143-49-57-160-58-108-212-49-100-186-32-98-191-4") = 1 Then
    callAcDen
    End
    PrepOpen = False: Unload GUI_ADTFile
End If
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\a\good\Maps\Maps\PVPZone04\PVPZone04_29_31.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-236-16-0-0-171-17-0-0-191-17-0-0-199-17-0-0-207-17-0-0-103") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "16-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-35-47-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "16-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-35-47-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-255-250-126-251-235-125-247-228-123-245-228-123-243-230-123-") = 1 Then
    callAcDen
    End
    PrepOpen = False: Unload GUI_ADTFile
End If
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\a\good\Maps\Maps\PVPZone04\PVPZone04_30_28.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-236-16-0-0-244-16-0-0-252-16-0-0-4-17-0-0-12-17-0-0-20-17-") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
    callAcDen
    End
    PrepOpen = False: Unload GUI_ADTFile
End If
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\a\good\Maps\Maps\PVPZone04\PVPZone04_30_29.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-94-17-0-0-75-32-0-0-67-33-0-0-211-33-0-0-227-33-0-0-175-11") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "16-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-35-47-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-0-0-0-0-228-30-108-65-64-56-109-65-84-35-111-65-120-2") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-48-28-113-85-31-88-72-57-87-63-4-109-49-6-116-51-252-116-20-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "48-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-35-47-0-0-1-0-0-0-0-1-0-0-0-0-0-0-38-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-178-11-99-184-214-95-192-170-67-219-167-81-249-225-122-0-19-") = 1 Then
    callAcDen
    End
    PrepOpen = False: Unload GUI_ADTFile
End If
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\a\good\Maps\Maps\PVPZone04\PVPZone04_30_30.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-147-17-0-0-164-49-0-0-176-51-0-0-162-53-0-0-198-53-0-0-106") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-192-35-30-191-160-158-149-191-160-188-225-191-96-229-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-18-237-124-16-239-124-16-239-124-19-235-123-23-251-124-36-5-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "48-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-35-47-0-0-2-0-0-0-0-1-0-0-0-0-0-0-46-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "64-0-0-0-15-0-0-0-16-0-0-0-25-0-0-0-27-0-0-0-28-0-0-0-29-0-0-0-30-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-160-81-139-192-128-85-154-192-240-5-155-192-240-5-155") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-34-148-57-27-204-112-4-250-126-0-0-126-0-0-126-0-0-126-0-0-1") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "64-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-35-47-0-0-1-0-0-0-0-1-0-0-0-0-0-0-38-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-26-22-122-28-1-123-28-251-123-28-246-123-36-191-102-26-162-8") = 1 Then
    callAcDen
    End
    PrepOpen = False: Unload GUI_ADTFile
End If
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\a\good\Maps\Maps\PVPZone04\PVPZone04_30_31.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-35-17-0-0-165-17-0-0-181-17-0-0-244-17-0-0-0-18-0-0-80-18-") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-192-66-91-64-112-187-223-64-16-177-39-65-48-187-92-65") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-61-65-89-61-71-85-58-72-86-54-72-89-49-68-94-45-60-102-38-49") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "16-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-35-47-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-192-24-93-64-160-137-135-64-0-44-99-64-0-169-220-191-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-55-75-85-54-47-104-53-2-115-46-191-98-58-182-84-77-192-76-75") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "32-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-35-47-0-0-1-0-0-0-0-1-0-0-0-0-0-0-38-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-77-91-42-80-63-74-88-241-89-83-185-63-82-170-43-88-172-34-11") = 1 Then
    callAcDen
    End
    PrepOpen = False: Unload GUI_ADTFile
End If
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\a\good\Maps\Maps\PVPZone04\PVPZone04_31_29.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-94-17-0-0-102-17-0-0-110-17-0-0-118-17-0-0-126-17-0-0-134-") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-1-58-112-252-3-126-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "32-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-35-47-0-0-3-0-0-0-0-1-0-0-0-0-0-0-37-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "16-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-35-47-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
    callAcDen
    End
    PrepOpen = False: Unload GUI_ADTFile
End If
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\a\good\Maps\Maps\PVPZone04\PVPZone04_31_30.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-35-17-0-0-68-18-0-0-92-18-0-0-100-18-0-0-108-18-0-0-184-19") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-0-147-199-190-64-192-135-191-192-177-233-191-64-57-32") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-29-250-123-24-240-123-16-235-124-7-235-124-0-240-125-5-231-1") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "32-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-35-47-0-0-3-0-0-0-0-1-0-0-0-0-0-0-37-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "16-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-35-47-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
    callAcDen
    End
    PrepOpen = False: Unload GUI_ADTFile
End If
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\a\good\Maps\Maps\PVPZone04\PVPZone04_31_31.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-237-16-0-0-245-16-0-0-253-16-0-0-5-17-0-0-13-17-0-0-21-17-") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-0-55-203-64-80-170-45-65-48-173-24-65-128-223-192-64-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-0-113-56-254-101-76-44-38-112-67-202-92-71-186-78-92-214-76-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "32-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-35-47-0-0-1-0-0-0-0-1-0-0-0-0-0-0-38-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "16-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-35-47-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
    callAcDen
    End
    PrepOpen = False: Unload GUI_ADTFile
End If
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\a\good\Maps\Maps\PVPZone03\PVPZone03_27_27.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-80-16-0-0-88-16-0-0-96-16-0-0-104-16-0-0-112-16-0-0-120-16") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
    callAcDen
    End
    PrepOpen = False: Unload GUI_ADTFile
End If
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\a\good\Maps\Maps\PVPZone03\PVPZone03_28_30.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-11-17-0-0-254-19-0-0-54-20-0-0-154-20-0-0-170-20-0-0-90-24") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-2-1-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-0-81-227-62-64-123-37-63-160-62-55-63-112-82-221-63-2") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-100-12-76-101-5-75-101-2-76-99-9-77-97-28-76-90-51-72-86-67-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "16-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-82-46-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-73-70-76-84-50-80-79-50-85-66-34-102-50-8-116-38-242-120-33-") = 1 Then
    callAcDen
    End
    PrepOpen = False: Unload GUI_ADTFile
End If
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\a\good\Maps\Maps\PVPZone03\PVPZone03_29_28.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-225-16-0-0-67-18-0-0-95-18-0-0-103-18-0-0-111-18-0-0-71-21") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-0-0-96-184-0-0-168-184-0-0-168-184-0-0-168-184-0-0-16") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
    callAcDen
    End
    PrepOpen = False: Unload GUI_ADTFile
End If
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\a\good\Maps\Maps\PVPZone03\PVPZone03_29_29.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-230-17-0-0-117-58-0-0-241-60-0-0-139-62-0-0-175-62-0-0-119") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-128-58-9-192-0-37-163-191-0-235-4-192-0-165-141-192-6") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-203-181-87-195-10-110-193-1-110-205-216-108-219-198-106-228-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "64-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-82-46-0-0-1-0-0-0-0-1-0-0-0-0-0-0-83-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "44-0-0-0-17-0-0-0-18-0-0-0-19-0-0-0-20-0-0-0-21-0-0-0-23-0-0-0-24-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-192-84-188-64-64-37-252-64-224-200-9-65-96-98-78-65-1") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-247-116-50-231-79-95-240-39-119-1-53-115-22-103-70-26-109-59") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "64-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-82-46-0-0-1-0-0-0-0-1-0-0-0-0-0-0-83-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-48-174-83-33-158-72-4-163-86-228-203-111-203-249-115-190-22-") = 1 Then
    callAcDen
    End
    PrepOpen = False: Unload GUI_ADTFile
End If
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\a\good\Maps\Maps\PVPZone03\PVPZone03_29_30.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-82-17-0-0-199-39-0-0-63-41-0-0-211-41-0-0-231-41-0-0-111-8") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-0-168-182-63-0-93-253-63-128-146-24-64-128-28-47-64-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-5-35-121-21-30-121-33-10-122-40-12-119-48-14-116-60-28-108-6") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "64-0-0-0-1-0-0-0-0-0-0-0-0-0-0-0-86-46-0-0-4-0-0-0-0-1-0-0-0-0-0-0-197") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "40-0-0-0-10-0-0-0-11-0-0-0-12-0-0-0-13-0-0-0-14-0-0-0-15-0-0-0-23-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-0-160-185-61-0-224-16-189-0-64-243-189-0-248-91-190-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-9-3-126-6-0-126-5-253-126-9-254-126-13-254-126-12-2-126-7-6-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "64-0-0-0-1-0-0-0-0-0-0-0-0-0-0-0-86-46-0-0-4-0-0-0-0-1-0-0-0-0-0-0-197") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-68-166-57-44-207-108-16-238-124-19-245-124-21-252-125-25-1-1") = 1 Then
    callAcDen
    End
    PrepOpen = False: Unload GUI_ADTFile
End If
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\a\good\Maps\Maps\PVPZone03\PVPZone03_30_28.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-113-16-0-0-190-16-0-0-202-16-0-0-210-16-0-0-218-16-0-0-114") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
    callAcDen
    End
    PrepOpen = False: Unload GUI_ADTFile
End If
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\a\good\Maps\Maps\PVPZone03\PVPZone03_30_29.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-2-17-0-0-79-17-0-0-91-17-0-0-99-17-0-0-107-17-0-0-75-18-0-") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-196-196-12-194-43-135-144-194-94-99-209-194-162-188-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-121-219-4-118-212-5-116-207-6-117-209-7-117-210-9-117-209-12") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "16-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-82-46-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "16-0-0-0-0-0-0-0-1-0-0-0-2-0-0-0-5-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
    callAcDen
    End
    PrepOpen = False: Unload GUI_ADTFile
End If
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\a\good\Maps\Maps\PVPZone03\PVPZone03_30_30.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-11-17-0-0-19-17-0-0-27-17-0-0-35-17-0-0-43-17-0-0-51-17-0-") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-128-77-70-64-64-233-236-64-224-225-64-65-240-182-137-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-149-31-59-156-52-57-164-64-58-172-72-60-182-81-62-193-90-62-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "32-0-0-0-2-0-0-0-0-0-0-0-0-0-0-0-90-46-0-0-3-0-0-0-0-1-0-0-0-0-0-0-87-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-64-128-45-193-16-157-134-193-240-10-165-193-192-130-1") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-237-137-37-224-148-57-211-168-79-212-175-86-219-168-82-221-1") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "16-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-82-46-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
    callAcDen
    End
    PrepOpen = False: Unload GUI_ADTFile
End If
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\a\good\Maps\Maps\PVPZone05\PVPZone05_25_23.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-29-17-0-0-37-17-0-0-45-17-0-0-53-17-0-0-61-17-0-0-69-17-0-") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
    callAcDen
    End
    PrepOpen = False: Unload GUI_ADTFile
End If
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\a\good\Maps\Maps\PVPZone05\PVPZone05_25_24.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-239-16-0-0-247-16-0-0-255-16-0-0-7-17-0-0-15-17-0-0-23-17-") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
    callAcDen
    End
    PrepOpen = False: Unload GUI_ADTFile
End If
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\a\good\Maps\Maps\PVPZone05\PVPZone05_25_25.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-225-16-0-0-233-16-0-0-241-16-0-0-249-16-0-0-1-17-0-0-9-17-") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
    callAcDen
    End
    PrepOpen = False: Unload GUI_ADTFile
End If
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\a\good\Maps\Maps\PVPZone05\PVPZone05_26_24.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-29-17-0-0-37-17-0-0-45-17-0-0-115-17-0-0-127-17-0-0-135-17") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-0-85-128-62-0-72-125-61-0-203-22-190-0-77-170-190-64-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-244-4-126-244-1-126-246-250-126-243-251-126-241-249-125-239-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "32-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-214-101-0-0-1-0-0-0-0-1-0-0-0-0-0-0-2") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-0-43-75-62-64-3-29-63-128-246-151-63-128-95-242-63-96") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-241-2-125-244-9-126-249-14-125-255-20-125-1-19-125-1-6-126-2") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "32-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-214-101-0-0-1-0-0-0-0-1-0-0-0-0-0-0-2") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-177-201-81-186-233-102-200-14-112-218-44-112-231-52-112-246-") = 1 Then
    callAcDen
    End
    PrepOpen = False: Unload GUI_ADTFile
End If
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\a\good\Maps\Maps\PVPZone05\PVPZone05_26_25.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-29-17-0-0-37-17-0-0-45-17-0-0-53-17-0-0-61-17-0-0-69-17-0-") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-69-210-124-62-142-31-188-62-100-92-171-62-217-99-133-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-4-9-126-4-5-126-5-1-126-5-250-126-5-240-125-7-231-124-9-222-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "48-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-214-101-0-0-1-0-0-0-0-1-0-0-0-0-0-0-2") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-240-40-113-190-224-51-122-190-176-209-56-190-252-196-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-15-250-125-11-254-126-15-0-126-34-247-121-42-251-119-46-2-11") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "48-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-214-101-0-0-1-0-0-0-0-1-0-0-0-0-0-0-2") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-3-254-126-2-2-126-0-5-126-18-54-113-38-84-86-21-16-124-10-23") = 1 Then
    callAcDen
    End
    PrepOpen = False: Unload GUI_ADTFile
End If
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\a\good\Maps\Maps\PVPZone05\PVPZone05_27_24.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-29-17-0-0-37-17-0-0-45-17-0-0-53-17-0-0-61-17-0-0-69-17-0-") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-0-148-56-62-0-72-95-62-0-141-58-62-0-80-249-61-0-44-3") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-0-4-126-0-4-126-0-255-126-1-255-126-3-254-126-5-255-126-5-8-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "48-0-0-0-1-0-0-0-0-0-0-0-0-0-0-0-216-101-0-0-0-0-0-0-0-1-0-0-0-0-0-0-2") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-0-97-40-63-224-166-218-63-128-178-246-63-0-143-54-190") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-17-9-125-23-25-122-17-19-124-4-230-124-0-191-108-252-188-107") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "48-0-0-0-1-0-0-0-0-0-0-0-0-0-0-0-216-101-0-0-0-0-0-0-0-1-0-0-0-0-0-0-2") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
    callAcDen
    End
    PrepOpen = False: Unload GUI_ADTFile
End If
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\a\good\Maps\Maps\PVPZone05\PVPZone05_27_25.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-225-16-0-0-233-16-0-0-241-16-0-0-249-16-0-0-1-17-0-0-9-17-") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-128-100-7-63-240-66-155-63-96-43-193-63-96-210-215-63") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-251-13-126-253-16-125-1-14-126-250-4-126-249-12-126-249-9-12") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "48-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-214-101-0-0-2-0-0-0-0-1-0-0-0-0-0-0-2") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-64-130-16-63-224-160-74-64-176-210-249-64-58-220-129-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-0-1-126-238-41-118-209-77-89-202-95-63-237-109-61-240-92-85-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "32-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-214-101-0-0-1-0-0-0-0-1-0-0-0-0-0-0-2") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
    callAcDen
    End
    PrepOpen = False: Unload GUI_ADTFile
End If
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\Documents and Settings\Name\Desktop\Dummy MPQ\World\Maps\Maps\NetherstormBG\NetherstormBG_26_25.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-128-16-0-0-136-16-0-0-144-16-0-0-152-16-0-0-160-16-0-0-168") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "16-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-255-255-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "16-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-255-255-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
    callAcDen
    End
    PrepOpen = False: Unload GUI_ADTFile
End If
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\Documents and Settings\Name\Desktop\Dummy MPQ\World\Maps\Maps\NetherstormBG\NetherstormBG_27_27.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-128-16-0-0-30-17-0-0-46-17-0-0-54-17-0-0-62-17-0-0-142-17-") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "16-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-170-105-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "16-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-170-105-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
    callAcDen
    End
    PrepOpen = False: Unload GUI_ADTFile
End If
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\Documents and Settings\Name\Desktop\Dummy MPQ\World\Maps\Maps\NetherstormBG\NetherstormBG_27_26.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-128-16-0-0-211-16-0-0-223-16-0-0-231-16-0-0-239-16-0-0-27-") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "16-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-170-105-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "16-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-170-105-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
    callAcDen
    End
    PrepOpen = False: Unload GUI_ADTFile
End If
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\Documents and Settings\Name\Desktop\Dummy MPQ\World\Maps\Maps\NetherstormBG\NetherstormBG_27_29.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-128-16-0-0-136-16-0-0-144-16-0-0-152-16-0-0-160-16-0-0-168") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "16-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-170-105-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "16-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-170-105-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
    callAcDen
    End
    PrepOpen = False: Unload GUI_ADTFile
End If
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\Documents and Settings\Name\Desktop\Dummy MPQ\World\Maps\Maps\NetherstormBG\NetherstormBG_28_26.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-34-17-0-0-43-19-0-0-75-19-0-0-83-19-0-0-91-19-0-0-203-20-0") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "16-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-170-105-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "16-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-170-105-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
    callAcDen
    End
    PrepOpen = False: Unload GUI_ADTFile
End If
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\Documents and Settings\Name\Desktop\Dummy MPQ\World\Maps\Maps\NetherstormBG\NetherstormBG_28_27.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-88-17-0-0-197-27-0-0-101-28-0-0-232-28-0-0-248-28-0-0-60-5") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "16-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-170-105-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "4-0-0-0-28-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "16-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-170-105-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-1-126-3-7-126-5-11") = 1 Then
    callAcDen
    End
    PrepOpen = False: Unload GUI_ADTFile
End If
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\Documents and Settings\Name\Desktop\Dummy MPQ\World\Maps\Maps\NetherstormBG\NetherstormBG_28_28.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-88-17-0-0-171-30-0-0-115-31-0-0-175-31-0-0-187-31-0-0-139-") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "16-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-170-105-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "8-0-0-0-1-0-0-0-167-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "16-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-170-105-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-20-125-5-248-126-2-2-126-4-19-125-5-34-121-7-55-113-10-94-83") = 1 Then
    callAcDen
    End
    PrepOpen = False: Unload GUI_ADTFile
End If
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\Documents and Settings\Name\Desktop\Dummy MPQ\World\Maps\Maps\NetherstormBG\NetherstormBG_28_29.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-128-16-0-0-213-17-0-0-237-17-0-0-245-17-0-0-253-17-0-0-217") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "16-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-170-105-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "4-0-0-0-0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "16-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-170-105-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
    callAcDen
    End
    PrepOpen = False: Unload GUI_ADTFile
End If
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\Documents and Settings\Name\Desktop\Dummy MPQ\World\Maps\Maps\NetherstormBG\NetherstormBG_29_26.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-34-17-0-0-118-19-0-0-154-19-0-0-162-19-0-0-170-19-0-0-18-2") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "16-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-170-105-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "16-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-170-105-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
    callAcDen
    End
    PrepOpen = False: Unload GUI_ADTFile
End If
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\Documents and Settings\Name\Desktop\Dummy MPQ\World\Maps\Maps\NetherstormBG\NetherstormBG_29_27.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-88-17-0-0-167-30-0-0-115-31-0-0-130-32-0-0-158-32-0-0-6-90") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-0-224-13-64-0-230-149-64-0-49-234-64-0-36-26-65-128-2") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-77-54-84-79-49-85-76-52-86-71-53-89-70-48-93-70-36-98-74-23-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "48-0-0-0-3-0-0-0-0-0-0-0-0-0-0-0-171-105-0-0-1-0-0-0-0-1-0-0-0-0-0-0-1") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "12-0-0-0-0-0-0-0-47-0-0-0-109-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-128-193-50-65-128-66-147-65-64-121-152-65-128-253-146") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-101-58-50-43-107-51-41-72-95-32-251-122-27-236-122-29-232-12") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "48-0-0-0-3-0-0-0-0-0-0-0-0-0-0-0-171-105-0-0-1-0-0-0-0-1-0-0-0-0-0-0-1") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-96-174-6-97-175-6-97-175-4-89-167-3-85-163-5-60-168-68-9-240") = 1 Then
    callAcDen
    End
    PrepOpen = False: Unload GUI_ADTFile
End If
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\Documents and Settings\Name\Desktop\Dummy MPQ\World\Maps\Maps\NetherstormBG\NetherstormBG_29_28.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-88-17-0-0-95-30-0-0-31-31-0-0-170-31-0-0-186-31-0-0-222-78") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-0-192-155-61-0-96-202-190-0-96-9-191-0-112-61-191-0-1") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-7-33-122-254-251-126-249-246-126-246-255-126-249-215-119-1-1") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "64-0-0-0-3-0-0-0-0-0-0-0-0-0-0-0-171-105-0-0-1-0-0-0-0-1-0-0-0-0-0-0-1") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "64-0-0-0-14-0-0-0-16-0-0-0-18-0-0-0-19-0-0-0-26-0-0-0-27-0-0-0-28-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-64-255-26-194-184-57-138-194-248-104-177-194-56-213-1") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-21-132-12-32-135-14-60-146-18-91-171-23-111-202-27-121-236-3") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "64-0-0-0-3-0-0-0-0-0-0-0-0-0-0-0-171-105-0-0-1-0-0-0-0-1-0-0-0-0-0-0-1") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-126-5-4-126-8-4-126-0-4-126-241-4-125-235-4-126-246-5-125-19") = 1 Then
    callAcDen
    End
    PrepOpen = False: Unload GUI_ADTFile
End If
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\Documents and Settings\Name\Desktop\Dummy MPQ\World\Maps\Maps\NetherstormBG\NetherstormBG_29_29.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-128-16-0-0-121-19-0-0-165-19-0-0-173-19-0-0-181-19-0-0-65-") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-0-127-236-61-128-98-62-62-0-247-70-62-0-24-8-62-0-56-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-219-4-121-218-2-121-217-0-120-217-0-120-216-254-120-214-253-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "16-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-170-105-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "28-0-0-0-0-0-0-0-1-0-0-0-9-0-0-0-14-0-0-0-15-0-0-0-16-0-0-0-20-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-148-38-237-194-91-101-110-195-148-162-163-195-125-173") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-25-132-6-6-130-6-254-130-7-10-131-15-89-169-19-99-190-41-85-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "16-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-170-105-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-43-0-119-43-0-119-43-255-119-43-255-119-44-255-118-44-254-11") = 1 Then
    callAcDen
    End
    PrepOpen = False: Unload GUI_ADTFile
End If
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\Documents and Settings\Name\Desktop\Dummy MPQ\World\Maps\Maps\NetherstormBG\NetherstormBG_30_27.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-128-16-0-0-204-17-0-0-228-17-0-0-236-17-0-0-244-17-0-0-248") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "16-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-170-105-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "4-0-0-0-0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "16-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-170-105-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
    callAcDen
    End
    PrepOpen = False: Unload GUI_ADTFile
End If
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\Documents and Settings\Name\Desktop\Dummy MPQ\World\Maps\Maps\NetherstormBG\NetherstormBG_30_28.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-128-16-0-0-210-17-0-0-234-17-0-0-242-17-0-0-250-17-0-0-34-") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "16-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-170-105-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "20-0-0-0-0-0-0-0-2-0-0-0-3-0-0-0-6-0-0-0-7-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "16-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-170-105-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
    callAcDen
    End
    PrepOpen = False: Unload GUI_ADTFile
End If
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\Documents and Settings\Name\Desktop\Dummy MPQ\World\Maps\Maps\NetherstormBG\NetherstormBG_30_29.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-128-16-0-0-30-17-0-0-46-17-0-0-54-17-0-0-62-17-0-0-178-17-") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "16-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-170-105-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "12-0-0-0-0-0-0-0-1-0-0-0-2-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "16-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-170-105-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
    callAcDen
    End
    PrepOpen = False: Unload GUI_ADTFile
End If
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\Documents and Settings\Name\Desktop\PatchMPQ\PVPZone03\PVPZone03_28_28.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-182-16-0-0-3-17-0-0-15-17-0-0-23-17-0-0-31-17-0-0-71-18-0-") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-178-0-99-178-0-99-178-0-99-178-0-99-178-0-99-178-0-99-178-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-178-0-99-178-0-99-178-0-99-178-0-99-178-0-99-178-0-99-178-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-178-0-99-178-0-99-178-0-99-178-0-99-178-0-99-178-0-99-178-0-") = 1 Then
    callAcDen
    End
End If
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\Documents and Settings\Name\Desktop\PatchMPQ\PVPZone03\PVPZone03_29_29.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-230-17-0-0-117-58-0-0-241-60-0-0-86-62-0-0-118-62-0-0-62-1") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-128-58-9-192-0-37-163-191-0-235-4-192-0-165-141-192-6") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-203-181-86-195-9-110-193-0-110-206-217-108-219-199-106-229-2") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "64-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-82-46-0-0-1-0-0-0-0-1-0-0-0-0-0-0-83-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "44-0-0-0-17-0-0-0-18-0-0-0-19-0-0-0-20-0-0-0-21-0-0-0-23-0-0-0-24-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-192-84-188-64-64-37-252-64-224-200-9-65-96-98-78-65-1") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-248-115-49-232-79-95-241-39-118-0-53-114-22-102-69-25-109-58") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "64-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-82-46-0-0-1-0-0-0-0-1-0-0-0-0-0-0-83-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-47-175-82-32-159-71-3-163-85-229-203-111-203-250-114-191-22-") = 1 Then
    callAcDen
    End
End If
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\Documents and Settings\Name\Desktop\PatchMPQ\PVPZone03\PVPZone03_29_30.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-82-17-0-0-199-39-0-0-63-41-0-0-163-41-0-0-179-41-0-0-15-84") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-0-168-182-63-0-93-253-63-128-146-24-64-128-28-47-64-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-5-35-121-21-30-121-33-10-122-40-12-119-48-14-116-60-28-108-6") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "64-0-0-0-1-0-0-0-0-0-0-0-0-0-0-0-86-46-0-0-4-0-0-0-0-1-0-0-0-0-0-0-197") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "40-0-0-0-10-0-0-0-11-0-0-0-12-0-0-0-13-0-0-0-14-0-0-0-15-0-0-0-23-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-0-160-185-61-0-224-16-189-0-64-243-189-0-248-91-190-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-9-3-126-6-0-126-5-253-126-9-254-126-13-254-126-12-2-126-7-6-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "64-0-0-0-1-0-0-0-0-0-0-0-0-0-0-0-86-46-0-0-4-0-0-0-0-1-0-0-0-0-0-0-197") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-68-166-57-44-207-108-16-238-124-19-245-124-21-252-125-25-1-1") = 1 Then
    callAcDen
    End
End If
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\Documents and Settings\Name\Desktop\PatchMPQ\PVPZone03\PVPZone03_30_28.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-113-16-0-0-190-16-0-0-202-16-0-0-210-16-0-0-218-16-0-0-114") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-178-0-99-178-0-99-178-0-99-178-0-99-178-0-99-178-0-99-178-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-178-0-99-178-0-99-178-0-99-178-0-99-178-0-99-178-0-99-178-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-178-0-99-178-0-99-178-0-99-178-0-99-178-0-99-178-0-99-178-0-") = 1 Then
    callAcDen
    End
End If
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\Documents and Settings\Name\Desktop\PatchMPQ\PVPZone03\PVPZone03_30_29.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-2-17-0-0-79-17-0-0-91-17-0-0-99-17-0-0-107-17-0-0-75-18-0-") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-196-196-12-194-43-135-144-194-94-99-209-194-162-188-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-121-219-3-117-212-4-115-208-5-116-209-6-116-210-8-116-209-11") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "16-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-82-46-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "16-0-0-0-0-0-0-0-1-0-0-0-2-0-0-0-5-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
    callAcDen
    End
End If
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\Documents and Settings\Name\Desktop\PatchMPQ\PVPZone04\PVPZone04_29_28.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-35-17-0-0-43-17-0-0-51-17-0-0-59-17-0-0-67-17-0-0-75-17-0-") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "0-0-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-127-0-0-") = 1 Then
    callAcDen
    End
End If
End If
End If
End If
End If
End If
End If
End If
End If

'//C:\Documents and Settings\Name\Desktop\PatchMPQ\PVPZone04\PVPZone04_30_30.adt
If InStr(1, GUI_ADTFile.adt.getadt_RDHM, "-0-0-72-16-0-0-147-17-0-0-164-49-0-0-176-51-0-0-162-53-0-0-198-53-0-0-142") <> 0 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 1), "68-2-0-0-0-0-0-0-192-35-30-191-160-158-149-191-160-188-225-191-96-229-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 2), "179-1-0-0-18-237-124-16-239-124-16-239-124-19-235-123-23-251-124-36-5-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 3), "48-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-35-47-0-0-2-0-0-0-0-1-0-0-0-0-0-0-46-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(1, 4), "64-0-0-0-15-0-0-0-16-0-0-0-25-0-0-0-27-0-0-0-28-0-0-0-29-0-0-0-30-0-0-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 1), "68-2-0-0-0-0-0-0-160-81-139-192-128-85-154-192-240-5-155-192-240-5-155") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 2), "179-1-0-0-34-148-57-27-204-112-4-250-126-0-0-126-0-0-126-0-0-126-0-0-1") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(3, 3), "64-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-35-47-0-0-1-0-0-0-0-1-0-0-0-0-0-0-38-") = 1 Then
If InStr(1, GUI_ADTFile.adt.getadt_KNCM(7, 2), "179-1-0-0-26-22-122-28-1-123-28-251-123-28-246-123-36-191-102-26-162-8") = 1 Then
    callAcDen
    End
End If
End If
End If
End If
End If
End If
End If
End If
End If

End Function

Sub callAcDen()
On Error Resume Next

    Load GUIAbout
    
    GUIAbout.ShowText_AccessDenied
    GUIAbout.Show
    
    GUIAbout.Hide
    GUIAbout.Show vbModal
    
    Unload GUIAbout
    Unload GUI_ADTFile
    
    End
    
End Sub

Sub SavePrevMaps()
Dim i As Integer, s As String

    s = "Recent Paths:" & vbCrLf

    For i = List1.ListCount - 1 To 0 Step -1
        
        If i <> List1.ListCount - 1 Then s = s & vbCrLf
        s = s & List1.List(i)
    
    Next
    
    If DummyMPQPath <> "" Then
        s = s & vbCrLf & "DummyMPQPath: " & DummyMPQPath & vbCrLf & vbCrLf
    End If
    
    WriteFile "WoWMapEditor.ini", s
    
End Sub

Sub LoadPrevMaps()
Dim s As String, a() As String, i As Integer, Loaded As String, InSection As Boolean

    s = ReadFile("WoWMapEditor.ini")
    s = Replace(s, "", "")
    a = Split(s, vbCrLf)
    
    For i = 0 To UBound(a)
        
        If a(i) = "" Then InSection = False
        
        If Len(a(i)) > 3 Then
            
            If InSection And InStr(1, Loaded, a(i)) = 0 Then
                List1.AddItem a(i), 0
                Loaded = Loaded & vbCrLf & a(i)
            End If
            
            If a(i) = "Recent Paths:" Then InSection = True
            
        End If
    Next
    
    
End Sub

Private Sub Form_Load()
Dim u As String, s As String, a() As String, ia(100) As Integer, CompVar As String
    
    LoadPrevMaps
    LoadDummyMPQPah
    
End Sub

Private Sub List1_DblClick()
    btnLoadPrvMap_Click
End Sub

Private Sub lstAreas_DblClick()
    vtnShowAssocADTs_Click
End Sub

Private Sub vtnShowAssocADTs_Click()
Dim s As String, a() As String, i As Integer

    If lstAreas = "" Then
        
        MsgBox "Select an area first N00B!!1", , "Duh"
        Exit Sub
        
    End If

    s = ReadFile("MapDB.txt")
    If Trim(s) = "" Then
        
        MsgBox "Error reading MapDB.txt", vbCritical
        Exit Sub
        
    End If
    
    a = Split(s, vbCrLf & vbCrLf)
    
    Load GUIAbout
    Load GUIShowMsg
    GUIShowMsg.Show
    
    For i = 0 To UBound(a)
        
        If InStr(1, a(i), lstAreas.Text) <> 0 Then
            
            GUIAbout.txtBlankTextBox.Text = GUIAbout.txtBlankTextBox.Text & vbCrLf & vbCrLf & String(80, "-") & vbCrLf & a(i)
            DoEvents
            
        End If
        
    Next
    
    If GUIAbout.txtBlankTextBox.Text = "" Then
        
        GUIAbout.txtBlankTextBox.Text = "Error: No maps seem to use that Area ID, even though it is apparently mapped in AreaTable.dbc" & vbCrLf & "Note that this search seaches a pre-compiled database which only covers Azeroth (The Eastern Kingdom) and Kailmdor."
    
    Else
        
        GUIAbout.txtBlankTextBox.Text = "The following maps (.adt files) listed can be found in the Common.MPQ, patch.MPQ, and other .MPQ files. " & vbCrLf & "(These .MPQ files can be found in the World of Warcraf /Data/ directory)" & vbCrLf & "You will need to use MyWoWStudio to extract them and open them with this application." & vbCrLf & "After you are done editing them and want to use them, please remember they need to to placed in the same" & vbCrLf & "tree-path in order for them to work." & GUIAbout.txtBlankTextBox.Text
        
    End If
    
    GUIAbout.ShowText_BlankText
    GUIAbout.Width = GUIAbout.Width + 1000
    GUIAbout.txtBlankTextBox.Width = GUIAbout.txtBlankTextBox.Width + 1000
    
    GUIAbout.Show
    Unload GUIShowMsg
    
End Sub
