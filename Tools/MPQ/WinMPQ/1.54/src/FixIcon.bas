Attribute VB_Name = "FixWindowIcon"
Option Explicit

Private Const WM_SETICON = &H80
Private Const ICON_SMALL = 0
Private Const IMAGE_ICON = 1
Private Const LR_DEFAULTSIZE = &H40

Private Declare Function GetModuleHandle Lib "Kernel32.dll" _
    Alias "GetModuleHandleA" _
    (ByRef lpModuleName As Any) As Long
Private Declare Function LoadImage Lib "User32.dll" _
    Alias "LoadImageA" ( _
    ByVal hinst As Long, _
    ByRef lpszName As Any, _
    ByVal uType As Long, _
    ByVal cxDesired As Long, _
    ByVal cyDesired As Long, _
    ByVal fuLoad As Long) As Long
Private Declare Function SendMessageA Lib _
    "User32.dll" _
    (ByVal hWnd As Long, _
    ByVal Msg As Long, _
    ByVal Wp As Long, _
    Lp As Any) As Long

Sub FixIcon(hWnd As Long, lpszName)
Dim hModule As Long, hIcon As Long, szName As String, nName As Long, Width As Long, Height As Long
hModule = GetModuleHandle(ByVal 0&)
If hModule = 0 Then Exit Sub
Width = Abs(GetReg("HKEY_USERS\.Default\Control Panel\Desktop\WindowMetrics\CaptionWidth", -270)) / Screen.TwipsPerPixelX - 2
Height = Abs(GetReg("HKEY_USERS\.Default\Control Panel\Desktop\WindowMetrics\CaptionHeight", -270)) / Screen.TwipsPerPixelY - 2
If VarType(lpszName) = vbString Then
    szName = lpszName
    hIcon = LoadImage(hModule, szName, IMAGE_ICON, Width, Height, LR_DEFAULTSIZE)
ElseIf VarType(lpszName) = vbByte Or VarType(lpszName) = vbInteger Or VarType(lpszName) = vbLong Then
    nName = lpszName
    hIcon = LoadImage(hModule, ByVal nName, IMAGE_ICON, Width, Height, LR_DEFAULTSIZE)
End If
If hIcon = 0 Then Exit Sub
SendMessageA MpqEx.hWnd, WM_SETICON, ICON_SMALL, ByVal hIcon
End Sub
