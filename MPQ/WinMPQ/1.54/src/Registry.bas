Attribute VB_Name = "RegistryFunctions"
Option Explicit

Private Const HKEY_CLASSES_ROOT = &H80000000
Private Const HKEY_CURRENT_USER = &H80000001
Private Const HKEY_LOCAL_MACHINE = &H80000002
Private Const HKEY_USERS = &H80000003
Private Const HKEY_PERFORMANCE_DATA = &H80000004
Private Const HKEY_CURRENT_CONFIG = &H80000005
Private Const HKEY_DYN_DATA = &H80000006
Private Const STANDARD_RIGHTS_ALL = &H1F0000
Private Const KEY_QUERY_VALUE = &H1
Private Const KEY_SET_VALUE = &H2
Private Const KEY_CREATE_SUB_KEY = &H4
Private Const KEY_ENUMERATE_SUB_KEYS = &H8
Private Const KEY_NOTIFY = &H10
Private Const SYNCHRONIZE = &H100000
Private Const KEY_CREATE_LINK = &H20
Private Const KEY_ALL_ACCESS = ((STANDARD_RIGHTS_ALL Or _
    KEY_QUERY_VALUE Or KEY_SET_VALUE Or KEY_CREATE_SUB_KEY _
    Or KEY_ENUMERATE_SUB_KEYS Or KEY_NOTIFY Or _
    KEY_CREATE_LINK) And (Not SYNCHRONIZE))
Private Const REG_OPTION_NON_VOLATILE = 0
Global Const REG_NONE = 0
Global Const REG_SZ = 1
Global Const REG_EXPAND_SZ = 2
Global Const REG_BINARY = 3
Global Const REG_DWORD = 4
Global Const REG_DWORD_LITTLE_ENDIAN = 4 ' Same as REG_DWORD
Global Const REG_DWORD_BIG_ENDIAN = 5
Global Const REG_LINK = 6
Global Const REG_MULTI_SZ = 7
Global Const REG_RESOURCE_LIST = 8
Global Const REG_FULL_RESOURCE_DESCRIPTOR = 9
Global Const REG_RESOURCE_REQUIREMENTS_LIST = 10

Private Const REG_CREATED_NEW_KEY = &H1
Private Const REG_OPENED_EXISTING_KEY = &H2

Private Declare Function RegOpenKeyEx Lib "advapi32.dll" Alias "RegOpenKeyExA" _
    (ByVal hKey As Long, ByVal lpSubKey As String, ByVal ulOptions As Long, _
    ByVal samDesired As Long, phkResult As Long) As Long
Private Declare Function RegCloseKey Lib "advapi32.dll" _
    (ByVal hKey As Long) As Long
Private Declare Function RegEnumValue Lib "advapi32.dll" _
    Alias "RegEnumValueA" (ByVal hKey As Long, ByVal dwIndex _
    As Long, lpValueName As String, lpcbValueName As Long, ByVal _
    lpReserved As Long, lpType As Long, lpData As Any, lpcbData As _
    Long) As Long
Private Declare Function RegEnumKeyEx Lib "advapi32.dll" _
    Alias "RegEnumKeyExA" (ByVal hKey As Long, ByVal dwIndex _
    As Long, lpName As String, lpcbName As Long, ByVal _
    lpReserved As Long, lpClass As String, lpcbClass As _
    Long, lpftLastWriteTime As Any) As Long
Private Declare Function RegQueryValueEx Lib "advapi32.dll" _
    Alias "RegQueryValueExA" (ByVal hKey As Long, ByVal lpValueName _
    As String, ByVal lpReserved As Long, lpType As Long, lpData As _
    Any, lpcbData As Long) As Long
Private Declare Function RegCreateKeyEx Lib "advapi32.dll" _
    Alias "RegCreateKeyExA" (ByVal hKey As Long, ByVal lpSubKey _
    As String, ByVal Reserved As Long, ByVal lpClass As String, _
    ByVal dwOptions As Long, ByVal samDesired As Long, _
    lpSecurityAttributes As Any, phkResult _
    As Long, lpdwDisposition As Long) As Long
Private Declare Function RegSetValueEx Lib "advapi32.dll" _
    Alias "RegSetValueExA" (ByVal hKey As Long, ByVal lpValueName _
    As String, ByVal Reserved As Long, ByVal dwType As Long, _
    lpData As Any, ByVal cbData As Long) As Long
Private Declare Function RegDeleteValue Lib "advapi32.dll" _
    Alias "RegDeleteValueA" (ByVal hKey As Long, ByVal _
    lpValueName As String) As Long
Private Declare Function RegDeleteKey Lib "advapi32.dll" Alias _
    "RegDeleteKeyA" (ByVal hKey As Long, ByVal lpSubKey As String) As Long
Sub ConvertValueName(Path As String, ByRef hKey As Long, ByRef Key As String, ValueName As String)
Dim Data As String, bNum As Long
Data = Mid$(Path, 1, InStr(Path, "\") - 1)
Select Case Data
Case "HKEY_CLASSES_ROOT"
hKey = HKEY_CLASSES_ROOT
Case "HKEY_CURRENT_USER"
hKey = HKEY_CURRENT_USER
Case "HKEY_LOCAL_MACHINE"
hKey = HKEY_LOCAL_MACHINE
Case "HKEY_USERS"
hKey = HKEY_USERS
Case "HKEY_PERFORMANCE_DATA"
hKey = HKEY_PERFORMANCE_DATA
Case "HKEY_CURRENT_CONFIG"
hKey = HKEY_CURRENT_CONFIG
Case "HKEY_DYN_DATA"
hKey = HKEY_DYN_DATA
End Select
bNum = 1
Do Until InStr(bNum, Path, "\") = 0
bNum = InStr(bNum, Path, "\") + 1
Loop
On Error Resume Next
Key = Mid$(Path, Len(Data) + 2, bNum - 2 - (Len(Data) + 1))
ValueName = Mid$(Path, bNum)
On Error GoTo 0
End Sub
Function GetReg(Path As String, Optional Default)
Attribute GetReg.VB_Description = "Reads a value from the registry."
Dim hKey As Long, kHandle As Long, Key As String, ValueName As String, Data As String, vLen As Long, vType As Long, NumData As Long
ConvertValueName Path, hKey, Key, ValueName
If RegOpenKeyEx(hKey, Key, 0&, KEY_QUERY_VALUE, kHandle) = 0 Then
    If RegQueryValueEx(kHandle, ValueName, 0&, vType, ByVal 0&, vLen) = 0 Then
        Data = String$(vLen, Chr$(0))
        If vType = REG_DWORD Or vType = REG_DWORD_BIG_ENDIAN Then
            If RegQueryValueEx(kHandle, ValueName, 0&, 0&, NumData, vLen) = 0 Then
                GetReg = NumData
            End If
        Else
            If RegQueryValueEx(kHandle, ValueName, 0&, 0&, ByVal Data, vLen) = 0 Then
                If vType = REG_SZ Or vType = REG_EXPAND_SZ Or vType = REG_MULTI_SZ Then
                    Data = Left$(Data, vLen - 1)
                    If Data <> "" Then GetReg = Data
                Else
                    GetReg = Data
                End If
            End If
        End If
    End If
    RegCloseKey kHandle
    If Not IsEmpty(GetReg) Then Exit Function
End If
If Not IsError(Default) Then GetReg = Default
End Function
Function GetRegType(Path As String) As Long
Dim hKey As Long, kHandle As Long, Key As String, ValueName As String, Data As String, vLen As Long, vType As Long
ConvertValueName Path, hKey, Key, ValueName
If RegOpenKeyEx(hKey, Key, 0&, KEY_QUERY_VALUE, kHandle) = 0 Then
    If RegQueryValueEx(kHandle, ValueName, 0&, vType, ByVal 0&, ByVal 0&) Then
        GetRegType = vType
    End If
    RegCloseKey kHandle
End If
End Function
Function EnumReg(ByVal Path As String, Index As Long) As String
Dim hKey As Long, kHandle As Long, Key As String, ValueName As String, Data As String, vLen As Long
If Right$(Path, 1) <> "\" Then Path = Path + "\"
ConvertValueName Path, hKey, Key, ValueName
ValueName = ""
If RegOpenKeyEx(hKey, Key, 0&, KEY_QUERY_VALUE, kHandle) = 0 Then
    vLen = 255
    Data = String$(255, Chr$(0))
    If RegEnumValue(kHandle, Index, ByVal Data, vLen, 0&, 0&, ByVal 0&, 0&) = 0 Then
        Data = Left$(Data, vLen)
        If Data = String$(255, Chr$(0)) Then Data = ""
        EnumReg = Data
    End If
    RegCloseKey kHandle
End If
End Function
Function EnumKey(ByVal Path As String, Index As Long) As String
Dim hKey As Long, kHandle As Long, Key As String, ValueName As String, Data As String, vLen As Long
If Right$(Path, 1) <> "\" Then Path = Path + "\"
ConvertValueName Path, hKey, Key, ValueName
ValueName = ""
If RegOpenKeyEx(hKey, Key, 0&, KEY_ENUMERATE_SUB_KEYS, kHandle) = 0 Then
    vLen = 255
    Data = String$(255, Chr$(0))
    If RegEnumKeyEx(kHandle, Index, ByVal Data, vLen, 0&, ByVal 0&, 0&, ByVal 0&) = 0 Then
        Data = Left$(Data, vLen)
        If Data = String$(255, Chr$(0)) Then Data = ""
        EnumKey = Data
    End If
    RegCloseKey kHandle
End If
End Function
Sub MultiStringToArray(MultiString As String, ByRef StrArray() As String)
Dim cNum As Long, cNum2 As Long
ReDim StrArray(0)
For cNum = 1 To Len(MultiString)
    cNum2 = InStr(cNum, MultiString, Chr(0))
    If cNum2 = 0 Then cNum2 = Len(MultiString) + 1
    ReDim Preserve StrArray(UBound(StrArray) + 1)
    StrArray(UBound(StrArray)) = Mid$(MultiString, cNum, cNum2 - cNum)
    cNum = cNum2
Next cNum
End Sub
Sub ArrayToMultiString(StrArray() As String, ByRef MultiString As String)
Dim sNum As Long
MultiString = ""
For sNum = 1 To UBound(StrArray)
    MultiString = MultiString + StrArray(sNum) + Chr$(0)
Next sNum
End Sub
Sub NewKey(ByVal Path As String, Optional Default, Optional vType)
Attribute NewKey.VB_Description = "Creates a new key in the registry."
Dim hKey As Long, kHandle As Long, Key As String, ValueName As String, Temp As Long, Setg As String, NumData As Long
If Right$(Path, 1) <> "\" Then Path = Path + "\"
ConvertValueName Path, hKey, Key, ValueName
ValueName = ""
If RegCreateKeyEx(hKey, Key, 0&, 0&, REG_OPTION_NON_VOLATILE, KEY_CREATE_SUB_KEY Or KEY_SET_VALUE, ByVal 0&, kHandle, Temp) = 0 Then
    If Not IsError(Default) Then
        If IsError(vType) Then vType = REG_SZ
        If vType = REG_DWORD Or vType = REG_DWORD_BIG_ENDIAN Then
            NumData = Default
            RegSetValueEx kHandle, ValueName, 0&, vType, NumData, 4
        Else
            Setg = Default
            If vType = REG_SZ Or vType = REG_EXPAND_SZ Or vType = REG_MULTI_SZ Then _
                Setg = Setg + Chr$(0)
            RegSetValueEx kHandle, ValueName, 0&, vType, ByVal Setg, Len(Setg)
        End If
    End If
    RegCloseKey kHandle
End If
End Sub
Sub SetReg(Path As String, NewValue, Optional vType)
Attribute SetReg.VB_Description = "Writes a value to the registry."
Dim hKey As Long, kHandle As Long, Key As String, ValueName As String, Setg As String, NumData As Long
ConvertValueName Path, hKey, Key, ValueName
If RegOpenKeyEx(hKey, Key, 0&, KEY_SET_VALUE, kHandle) = 0 Then
    If IsError(vType) Then vType = REG_SZ
    If vType = REG_DWORD Or vType = REG_DWORD_BIG_ENDIAN Then
        NumData = NewValue
        RegSetValueEx kHandle, ValueName, 0&, vType, NumData, 4
    Else
        Setg = NewValue
        If vType = REG_SZ Or vType = REG_EXPAND_SZ Or vType = REG_MULTI_SZ Then _
            Setg = Setg + Chr$(0)
        RegSetValueEx kHandle, ValueName, 0&, vType, ByVal Setg, Len(Setg)
    End If
    RegCloseKey kHandle
End If
End Sub
Sub DelReg(Path As String)
Attribute DelReg.VB_Description = "Deletes a value from the registry."
Dim hKey As Long, kHandle As Long, Key As String, ValueName As String
ConvertValueName Path, hKey, Key, ValueName
If RegOpenKeyEx(hKey, Key, 0&, KEY_SET_VALUE, kHandle) = 0 Then
    RegDeleteValue kHandle, ValueName
    RegCloseKey kHandle
End If
End Sub
Sub DelKey(ByVal Path As String)
Attribute DelKey.VB_Description = "Deletes a key from the registry."
Dim hKey As Long, Key As String, Data As String
If Right$(Path, 1) <> "\" Then Path = Path + "\"
ConvertValueName Path, hKey, Key, Data
RegDeleteKey hKey, Key
End Sub
