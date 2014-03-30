#Include <nomadmemory.au3>
#include <GUIConstants.au3>

SetPrivilege("SeDebugPrivilege", 1)

;Declare Stuff
Const $StaticPlayerZ = 0x00D0E550
Dim $Playerstart = 0x07000BF0
Const $Offset = 0x2000
dim $AtuallyDo = 0
Global $memoryX = 0
Global $memoryY = 0
Global $memoryZ = 0
Global $tString = ""
Global $Handle = 0
;End Declare Stuff

#Region ;Gui Stuff
Opt("GUIOnEventMode", 1)
$MAINFM = GUICreate("In Game ADT/POS Finder By Glitchy", 199, 322, -1, -1, BitOR($WS_MINIMIZEBOX,$WS_SYSMENU,$WS_CAPTION,$WS_POPUP,$WS_POPUPWINDOW,$WS_GROUP,$WS_BORDER,$WS_CLIPSIBLINGS,$DS_MODALFRAME), BitOR($WS_EX_TOPMOST,$WS_EX_WINDOWEDGE))
GUISetOnEvent($GUI_EVENT_CLOSE, "CLOSEClicked")
$Group1 = GUICtrlCreateGroup("Setup", 8, 0, 185, 73)
$FINDBT = GUICtrlCreateButton("Find Offsets", 112, 40, 73, 25)
GUICtrlSetOnEvent($FINDBT, "FindOffsets")
$TITLE = GUICtrlCreateInput("World of Warcraft", 16, 16, 169, 21)
$STATUSLBL = GUICtrlCreateLabel("Status: Not Found", 16, 48, 90, 17)
GUICtrlCreateGroup("", -99, -99, 1, 1)
$Group2 = GUICtrlCreateGroup("ADT Info", 8, 80, 185, 233)
$ADTFile = GUICtrlCreateLabel("ADT Tile:", 16, 104, 150, 17)
$ADTX = GUICtrlCreateLabel("ADT X:", 16, 128, 39, 17)
$ADTY = GUICtrlCreateLabel("ADT Y:", 16, 152, 39, 17)
$ADTZ = GUICtrlCreateLabel("ADT Z:", 16, 176, 39, 17)
$PLAYERX = GUICtrlCreateLabel("Player X:", 16, 208, 46, 17)
$PLAYERY = GUICtrlCreateLabel("Player Y:", 16, 232, 46, 17)
$PLAYERZ = GUICtrlCreateLabel("Player Z:", 16, 256, 46, 17)
$PauseBT = GUICtrlCreateButton("Pause", 112, 280, 75, 25, 0)
GUICtrlSetState(-1, $GUI_DISABLE)
GUICtrlSetOnEvent($PauseBT, "PauseBT")
$InputADTX = GUICtrlCreateInput("0", 64, 128, 121, 21)
$InputADTY = GUICtrlCreateInput("0", 64, 152, 121, 21)
$InputADTZ = GUICtrlCreateInput("0", 64, 176, 121, 21)
$InputPLYX = GUICtrlCreateInput("0", 64, 208, 121, 21)
$InputPLYY = GUICtrlCreateInput("0", 64, 232, 121, 21)
$InputPLYZ = GUICtrlCreateInput("0", 64, 256, 121, 21)
GUICtrlCreateGroup("", -99, -99, 1, 1)
GUISetState(@SW_SHOW)
#EndRegion ;End Gui Stuff

While 1
	if $AtuallyDo = 1 then
		dim $tTile = ""
		dim $tString2
		
		$tString = _MemoryRead($memoryX , $Handle, 'float')
		GUICtrlSetData($InputPLYX,$tString)
		$tString = _MemoryRead($memoryY , $Handle, 'float')
		GUICtrlSetData($InputPLYY,$tString)
		$tString = _MemoryRead($memoryZ , $Handle, 'float')
		GUICtrlSetData($InputPLYZ,$tString)
		
		$tString = _MemoryRead($memoryX , $Handle, 'float')
		$tString = ((32 * 533.3333) - $tString)
		$tString2 = ($tString / 533.3333)
		$tArray = StringSplit($tString2,".")
		$tString2 = $tArray[1]
		$tTile = $tString2
		GUICtrlSetData($InputADTX,$tString)
		
		$tString = _MemoryRead($memoryY , $Handle, 'float')
		$tString = ((32 * 533.3333) - $tString)
		$tString2 = ($tString / 533.3333)
		$tArray = StringSplit($tString2,".")
		$tString2 = $tArray[1]
		$tTile = $tTile & "," & $tString2
		GUICtrlSetData($InputADTY,$tString)
		
		$tString = _MemoryRead($memoryZ , $Handle, 'float')
		GUICtrlSetData($InputADTZ,$tString)
		GUICtrlSetData($ADTFile,"ADT Tile: " & $tTile)
		sleep(1000)
		
	Else
		sleep(1000)
	EndIf
WEnd

Func PauseBT()
	if $AtuallyDo = 1 Then
		$AtuallyDo = 0
		GUICtrlSetData($STATUSLBL,"Status: Paused")
		GUICtrlSetData($PauseBT,"Resume")
	Else
		$AtuallyDo = 1
		GUICtrlSetData($STATUSLBL,"Status: Running")
		GUICtrlSetData($PauseBT,"Pause")
	endif
EndFunc

Func FindOffsets()
	$tString = GUICtrlRead($TITLE)
	$Handle = _memoryopen(WinGetProcess($tString))
	if $Handle = 0 Then
		msgbox(0,"Error","Could Not Find The Process Called ")
		Return
	EndIf
	GUICtrlSetState($TITLE,$GUI_DISABLE)
		
	While _memoryread($Playerstart,$Handle,'dword') <> _memoryread($StaticPlayerZ,$Handle,'dword')
		$Playerstart += $Offset
	Wend
	
	
	
	$memoryZ = "0x" & hex($Playerstart,8)
	msgbox  (0,"",$memoryZ)
	
	$memoryY = StringTrimRight($memoryZ,3) & "BE8"
	$memoryX = StringTrimRight($memoryZ,3) & "BEC"
	GUICtrlSetData($STATUSLBL,"Status: Running")
	GUICtrlSetState($PauseBT,$GUI_ENABLE)		
	$AtuallyDo = 1
	

EndFunc

Func CLOSEClicked()
	Exit
EndFunc