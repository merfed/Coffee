VERSION 5.00
Begin VB.Form AltShift 
   Caption         =   "WoW Location Reader"
   ClientHeight    =   1785
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   4395
   Icon            =   "LocationReader.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   1785
   ScaleWidth      =   4395
   StartUpPosition =   3  'Windows Default
   Begin VB.Frame Frame1 
      Caption         =   "Current char location"
      Height          =   1215
      Left            =   240
      TabIndex        =   0
      Top             =   240
      Width           =   3855
      Begin VB.Timer Timer1 
         Enabled         =   0   'False
         Interval        =   500
         Left            =   3240
         Top             =   720
      End
      Begin VB.TextBox txtCharLoc 
         BackColor       =   &H8000000F&
         BorderStyle     =   0  'None
         Height          =   735
         Left            =   240
         Locked          =   -1  'True
         MultiLine       =   -1  'True
         TabIndex        =   1
         Top             =   240
         Width           =   3375
      End
   End
End
Attribute VB_Name = "AltShift"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Declare Function FindWindow Lib "user32" Alias "FindWindowA" (ByVal lpClassName As String, ByVal lpWindowName As String) As Long
Private Declare Function GetWindowThreadProcessId Lib "user32" (ByVal hwnd As Long, lpdwProcessId As Long) As Long
Private Declare Function OpenProcess Lib "kernel32" (ByVal dwDesiredAccess As Long, ByVal bInheritHandle As Long, ByVal dwProcessId As Long) As Long
Private Declare Function WriteProcessMemory Lib "kernel32" (ByVal hProcess As Long, ByVal lpBaseAddress As Any, ByVal lpBuffer As Any, ByVal nSize As Long, lpNumberOfBytesWritten As Long) As Long
Private Declare Function ReadProcessMemory Lib "kernel32" (ByVal hProcess As Long, ByVal lpBaseAddress As Any, ByVal lpBuffer As Any, ByVal nSize As Long, lpNumberOfBytesWritten As Long) As Long
Private Declare Function CloseHandle Lib "kernel32" (ByVal hObject As Long) As Long
Private Declare Function SetSecurityInfo Lib "advapi32.dll" (ByVal Handle As Long, ByVal SE_OBJECT_TYPE As Long, ByVal SECURITY_INFORMATION As Long, psidOwner As Long, psidGroup As Long, pDACL As Long, pSacl As Long) As Long
Private Declare Function GetSecurityInfo Lib "advapi32.dll" (ByVal Handle As Long, ByVal SE_OBJECT_TYPE As Long, ByVal SECURITY_INFORMATION As Long, psidOwner As Long, psidGroup As Long, pDACL As Long, pSacl As Long, PSECURITY_DESCRIPTOR As Long) As Long
Private Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (pDst As Any, pSrc As Any, ByVal ByteLen As Long)


Private Enum SE_OBJECT_TYPE
    SE_UNKNOWN_OBJECT_TYPE = 0
    SE_FILE_OBJECT
    SE_SERVICE
    SE_PRINTER
    SE_REGISTRY_KEY
    SE_LMSHARE
    SE_KERNEL_OBJECT
    SE_WINDOW_OBJECT
    SE_DS_OBJECT
    SE_DS_OBJECT_ALL
    SE_PROVIDER_DEFINED_OBJECT
    SE_WMIGUID_OBJECT
    SE_REGISTRY_WOW64_32
End Enum

Private Enum SECURITY_INFORMATION
    OWNER_SECURITY_INFORMATION = 1
    GROUP_SECURITY_INFORMATION = 2
    DACL_SECURITY_INFORMATION = 4
    SACL_SECURITY_INFORMATION = 8
    PROTECTED_SACL_SECURITY_INFORMATION = 16
    PROTECTED_DACL_SECURITY_INFORMATION = 32
    UNPROTECTED_SACL_SECURITY_INFORMATION = 64
    UNPROTECTED_DACL_SECURITY_INFORMATION = 128
End Enum

Private Const PROCESS_ALL_ACCESS As Long = &H1F0FFF
Private Const WRITE_DAC As Long = &H40000

Dim phandle         As Long
Dim myHandle        As Long

Dim BaseZMemAddress As Long
Dim TargZMemAdrTarg As Long



Sub Scann()
Dim b As String, a As String, Loc As Long
Dim DataStringLen As Long, LocZ As String
    
    Loc = BaseZMemAddress
    DataStringLen = 4
    b = "         "
    Call ReadProcessMemory(myHandle, Loc, b, DataStringLen, DataStringLen)


    txtCharLoc.Text = "Scanning, hold on..."
    
    Dim x As Long
    For x = Val("&HB90") To 505000000 Step Val("&H1000")
        
        DataStringLen = 4
        a = "         "
        Call ReadProcessMemory(myHandle, x, a, DataStringLen, DataStringLen)
        
        If a = b Then
            
            TargZMemAdrTarg = x
            
            Exit Sub
            
        End If
        
        DoEvents
        
    Next
    
    MsgBox "Error: Could not find character XYZ access memory adress!", vbExclamation
    
End Sub

Private Sub Form_Load()
        
    MsgBox "WoWLocReader" & vbCrLf & vbCrLf & _
            "So, the purpose of this small program was to read the location of the character in the WoW 3D world, so map editors had a better idea as to where to put/move things.  Say if you want something in a certain spot, you could just move your character there, and read the XYZ location from this...." & vbCrLf & vbCrLf & _
            "....however it seems nothing can be easy *sigh*" & vbCrLf & vbCrLf & _
            "The Z seems to be right, if I was to put water on the map, and use this app to get the Z-value for the water altitude, then the water comes up to the character.  But the X and Y values are nothing like what I expected." & vbCrLf & vbCrLf & _
            "I think I may need to do some math, perhaps the XY values of objects on the WoW ADT files are based in a coordinate system where 0,0 is the center of the world?  And in memory it's not?  I dunno yet. I need to work on it more." & vbCrLf & vbCrLf & _
            "...but I figured I mise well compile this program for people so they can at least use it to find the Z value of things, should they need help with it.", vbInformation
            
    MsgBox "How to use:" & vbCrLf & vbCrLf & "After you have logged into WoW and your character is walking around, run this application.  This version will only work for WoW version 2.1.3", vbInformation
            
    If Not InitProcessCheater(Me.hwnd) Then
        MsgBox "Could not open/find WoW in memory. Is it running?" & vbCrLf & "Program will now terminate", vbCritical
        End
    End If
    
    BaseZMemAddress = Val("&HCD900C")
    
    Timer1.Enabled = True
    
End Sub

Public Function OpenSProcess(WindowName As String, ByVal urWindowHandle, ByVal dwDesiredAccess As Long) As Long
Dim hwnd As Long

Dim ourHwnd As Long
Dim ProcessID As Long
Dim pProcessID As Long
Dim ProcessHandle As Long
Dim ourPhandle As Long
Dim pOldACL As Long
Dim pOldSecDesc As Long
Dim secyinfo As Long

    hwnd = FindWindow(vbNullString, WindowName)
    ourHwnd = urWindowHandle
    
    If hwnd = 0 Then
        OpenSProcess = 0
        Exit Function
    End If
    
    GetWindowThreadProcessId hwnd, ProcessID
    GetWindowThreadProcessId ourHwnd, pProcessID
    
    ProcessHandle = OpenProcess(dwDesiredAccess, False, ProcessID)
    
    If ProcessHandle <> 0 Then
        OpenSProcess = ProcessHandle
        Exit Function
    End If
    
    ourPhandle = OpenProcess(dwDesiredAccess, False, pProcessID)
    
    If GetSecurityInfo(ourPhandle, SE_KERNEL_OBJECT, DACL_SECURITY_INFORMATION, 0, 0, pOldACL, 0, pOldSecDesc) = 0 Then
    
        ProcessHandle = OpenProcess(WRITE_DAC, False, ProcessID)
        
        If ProcessHandle = 0 Then
        
            OpenSProcess = 0
        
        Else
        
            If SetSecurityInfo(ProcessHandle, SE_KERNEL_OBJECT, DACL_SECURITY_INFORMATION Or UNPROTECTED_DACL_SECURITY_INFORMATION, 0, 0, ByVal pOldACL, 0) = 0 Then
                
                CloseHandle ProcessHandle
                ProcessHandle = OpenProcess(dwDesiredAccess, False, ProcessID)
                OpenSProcess = ProcessHandle
            
            Else
                OpenSProcess = 0
            End If
            
        End If
        
    Else
        
        OpenSProcess = 0
        Exit Function
    
    End If
    
End Function

Function GetCharZ() As Single
Dim b As String, Loc As Long
Dim DataStringLen As Long
    
    Loc = BaseZMemAddress
    DataStringLen = 4
    b = "         "
    Call ReadProcessMemory(myHandle, Loc, b, DataStringLen, DataStringLen)
    
    GetCharZ = String32ToFloat(b)
    
End Function

Function InitProcessCheater(hwnd As Long)

'phandle = OpenProcess(&H1F0FFF, False, pid)
phandle = OpenSProcess("World of Warcraft", hwnd, &H1F0FFF)

If (phandle = 0) Then
    InitProcessCheater = False
    myHandle = 0
Else
    InitProcessCheater = True
    myHandle = phandle
End If

End Function

Function GetCharY() As Single
Dim b As String, Loc As Long
Dim DataStringLen As Long
    
    Loc = BaseZMemAddress - 4
    DataStringLen = 4
    b = "         "
    Call ReadProcessMemory(myHandle, Loc, b, DataStringLen, DataStringLen)
    
    GetCharY = String32ToFloat(b)
    
End Function

Function GetCharX() As Single
Dim b As String, Loc As Long
Dim DataStringLen As Long
    
    Loc = BaseZMemAddress - 8
    DataStringLen = 4
    b = "         "
    Call ReadProcessMemory(myHandle, Loc, b, DataStringLen, DataStringLen)
    
    GetCharX = String32ToFloat(b)
    
End Function

Function String32ToFloat(ByVal InBinStr As String) As Single
Dim c1 As Integer, c2 As Integer, c3 As Integer, c4 As Integer
Dim b(3) As Byte
Dim s As String
Dim Sngl As Single

    's = InBinStr & Chr(0) & Chr(0) & Chr(0) & Chr(0)
    's = Strings.Left(s, 4)
    
    s = InBinStr
    CopyMemory Sngl, ByVal s, 4
    'CopyMemory b(0), ByVal s, 4
    
    String32ToFloat = StripVBFloatFaggotry(Sngl)
    
End Function

Function StripVBFloatFaggotry(InputBullshit As Single) As Single
    
    ' VB is a bitch when it somes to floats because it inputs in crap like "-e"+ some number or "#Inf" into the number....
    ' Oh... and btw,, when it does that. IT CANT FUCKING ADD!
    ' Yeah, that's right.  Any addition, subtraction, multiplications and so on cant be done to numbers where it has that crap in it
    ' Makes no fucking sence, but neither does half the other Mirocrap products
    
    Dim a As String, Sng1 As Single
    
    ' // Convert to string
    a = InputBullshit
    
    ' // Remove the ending crap that sometimes exists ("-e" + some number) as it gets in the way
    a = Replace(a, "e", "", , , vbTextCompare)
    
    ' // Remove "#Inf"
    a = Replace(a, "#Inf", "", , , vbTextCompare)
    
    ' // Remove "+"
    a = Replace(a, "+", "", , , vbTextCompare)
    
    ' // Remove any minus sign left, but not a negative sign
    a = Mid(a, 1, 1) & Replace(Mid(a, 2), "-", "")
    
    ' // Convert back to single (float)
    Sng1 = a
    
    StripVBFloatFaggotry = Sng1

End Function

Private Sub TimerCharLoc_Timer()
    
    txtCharLoc.Text = "X: " & GetCharX & vbCrLf & "Y: " & GetCharY & vbCrLf & "Z: " & GetCharZ
    
End Sub

Private Sub Timer1_Timer()
    txtCharLoc.Text = "X: " & GetCharX & vbCrLf & "Y: " & GetCharY & vbCrLf & "Z: " & GetCharZ
End Sub
