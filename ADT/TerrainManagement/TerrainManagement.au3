; ----------------------------------------------------------------------------
; ----------------------------------------------------------------------------
;
; Author:
; 	Malu05 aka. Mads Hagbart Lund <Batmazbaz@hotmail.com>
;
; Script Function:
; 	Application _ TerrainManagement (public)
;	Version: 0.01
; ----------------------------------------------------------------------------
; ----------------------------------------------------------------------------


;;--------------------------------------------------------------------------------
;;Includes;
;;--------------------------------------------------------------------------------
#Include <NomadMemory.au3>
#Include <GuiListBox.au3>
#include <WindowsConstants.au3>
#Include <Array.au3>
#Include <GuiListView.au3>
#Include <Misc.au3>
#Include <GuiScrollBars.au3>
#include <ScrollBarConstants.au3>
;;--------------------------------------------------------------------------------
;;Variable Definition;
;;--------------------------------------------------------------------------------


$Debug = 0 									;-Set to 1 For more Advanced Indo

;//PlayerBase Structures:
Global $PlayerBaseStatic 	= 0x0127F13C 	;-Static Offset for the Playerbse
Global $PlayerXyz 			= 0x7D0 		;-Offset from Playerbase to the X position

;//ADT Structures:
Global $OffsetADTInfo 		= 0x1286720		;-Static Adress for Info About The Loaded WDT
Global $OffsetAbegin 		= 0x01292A20 	;-Offset to gather ADT info for the SUB Chunks
Global $TerrainGeneral 		= 0x00FED3CC 	;-Staric Offset for the Generel Chunk

;//OBJECT Structures:
Global $OffsetM2Static 		= 0x012CF63C 	;Static Offset Pointing to the first object in the linked list
Global $OffsetWMOStatic 	= 0x012CF614 	;Static Offset Pointing to the first object in the linked list
Global $MouseSelectEnabler 	= 0x012E965C

;//Prefedined Variables Related to ADT Section
dim $ADT[5000][500], $RDHMbase[19999], $closer[19999], $MCNKindex[19999]
dim $ADTdata[5000][500], $finalX[91999], $finalY[19999], $finalZ[19999]
dim $MCNKinden[5000], $MCNKPosX[19999], $MCNKindenOffs[19999], $MCNKPosY[19999], $MCNKPosZ[19999]
dim $begin, $MCINoffs, $MCNKoffs[19999], $Basevaluepointer, $countANA

;//Prefedined Variables Related to OBJECT Section
dim $m2data[89099][9], $dataokM2[89999], $OBJname[89999]
dim $WMOdata[9000][9], $dataokWMO[9000], $_m2uniqueID[89999]
global $M2NameIndex = 0, $M2listitem[89999]
dim $M2count,$WMOcount, $Listammountm2, $Listammountwmo, $transyn = 0
SetPrivilege ("SeDebugPrivilege", 1)
$dll = DllOpen ("user32.dll")

$DllInformation = _memoryopen (WinGetProcess ("World of Warcraft"))
$lvl1Pointer = _MemoryRead ($PlayerBaseStatic, $DllInformation, 'ptr')
$lvl2Pointer = _MemoryRead (($lvl1Pointer + 0x30), $DllInformation, 'ptr')
$PlayerBase = _MemoryRead (($lvl2Pointer + 0x28), $DllInformation, 'ptr')
$X = $PlayerBase + $PlayerXyz
$Y = $PlayerBase + $PlayerXyz + 0x4
$Z = $PlayerBase + $PlayerXyz + 0x8


;;--------------------------------------------------------------------------------
;;Sample Hotkey Defenition;
;;--------------------------------------------------------------------------------
HotKeySet ("{Numpad1}", "OffsetTerrainUP")
HotKeySet ("{Numpad2}", "OffsetTerrainDown")
HotKeySet ("{Numpad3}", "SelectM2")
;;--------------------------------------------------------------------------------
;;GUI Creation;
;;--------------------------------------------------------------------------------
$MainGUI = GUICreate ("WoW Terrain Manager 0.2 by Malu05", 900, 800)
GUISetState ()
$GUIList = GUICtrlCreateList ("", 0, 0, 300, 300, bitor ($WS_BORDER, $WS_VSCROLL))
$GUIListSub = GUICtrlCreateList ("", 300, 0, 300, 300, bitor ($WS_BORDER, $WS_VSCROLL))
$GUIListSubM2 = GUICtrlCreateListView("UniqueID   |File Path |X               |Y               |Z               ", 0, 300, 900, 500, bitor ($WS_BORDER, $WS_VSCROLL))
_GUICtrlListView_SetColumnWidth($GUIListSubM2, 1, 480)

;;--------------------------------------------------------------------------------
;;Main App;
;;--------------------------------------------------------------------------------
_report("Welcome to Malu05's Terrain Manager")
TrayTip("Malu05's Terrain Manager","Please Wait While I'm Grappin the Dataz!",5,2)
TerrainManagement()
ObjectManagement()
TrayTip("Malu05's Terrain Manager","Ready to Rock!",5,2)

;;================================================================================
;;TerrainManagement()
;;--------------------------------------------------------------------------------
;;//This fucntion is used to index all the sub chunks.
;;================================================================================
func TerrainManagement()
	_report("")
	_report("===ADT TERRAIN MANAGEMENT===")
	_report("Initializing ADT load")
	_report(_MemoryRead ($OffsetADTInfo, $DllInformation, 'char[60]'))
	Global $ChrunkCountANA
	Global $bas = 0
	Global $ChrunkCount = 0
	$posX = _MemoryRead ("0x" & hex ($X), $DllInformation, 'float')
	$posy = _MemoryRead ("0x" & hex ($Y), $DllInformation, 'float')
	_report("My Position is: " & floor ($posX) & " . " & floor ($posy))
	sleep (3000)
	;// To Explain the Following, for every 533,3333 coords from the Physical 0,0 WoW Will load a new ADT
	;// However since its in both the X and Y axis the system works like this:
	;
	;// For Every +X New ADT the new offset will be located at 0x100 after the last one
	;// For Every +Y New ADT the new offset will be loacted at 0x4   after the last one
	;
	;// So the first thing we do here is find the Physical 0,0 (32*533,3333-posx and y)
	;// Then to find the ADT's located around US and not just from our position and up we will go down 3 ADT's (+533.3333*3)
	;
	;// Most TBC and WOTLK Areas loads a maximum of 8x8 (64) ADT's at its current stage but this can very likely change with new patches
	;// PRE TBC Zones Only load upto 5x5 (25) ADT's at a time
	;// Note that this changes depending on your View Distance
	$OffsetAfloor = floor ((32 * 533.3333 - floor ($posX + (533.3333 * 4))) / 533.3333) ;Don't Ask!!! ^^
	$OffsetAfloorY = floor ((32 * 533.3333 - floor ($posy + (533.3333 * 4))) / 533.3333)
	$OffsetA = ($OffsetAbegin + (0x100 * $OffsetAfloor)) + (0x4 * $OffsetAfloorY)
	$OffsetB = ($OffsetAbegin + (0x100 * $OffsetAfloor)) + (0x4 * $OffsetAfloorY) + 0x100
	$OffsetC = ($OffsetAbegin + (0x100 * $OffsetAfloor)) + (0x4 * $OffsetAfloorY) + 0x200
	$OffsetD = ($OffsetAbegin + (0x100 * $OffsetAfloor)) + (0x4 * $OffsetAfloorY) + 0x300
	$OffsetE = ($OffsetAbegin + (0x100 * $OffsetAfloor)) + (0x4 * $OffsetAfloorY) + 0x400
	$OffsetF = ($OffsetAbegin + (0x100 * $OffsetAfloor)) + (0x4 * $OffsetAfloorY) + 0x500
	$OffsetG = ($OffsetAbegin + (0x100 * $OffsetAfloor)) + (0x4 * $OffsetAfloorY) + 0x600
	$OffsetH = ($OffsetAbegin + (0x100 * $OffsetAfloor)) + (0x4 * $OffsetAfloorY) + 0x700
	$OffsetI = ($OffsetAbegin + (0x100 * $OffsetAfloor)) + (0x4 * $OffsetAfloorY) + 0x800
	$OffsAPoint = _MemoryRead ("0x" & hex ($OffsetA), $DllInformation, 'int')
	$OffsCPoint = _MemoryRead ("0x" & hex ($OffsetC), $DllInformation, 'int')
	$OffsEPoint = _MemoryRead ("0x" & hex ($OffsetE), $DllInformation, 'int')
	For $n = 1 to 9 Step + 1
		if $n = 1 Then $go = $OffsetA
		if $n = 2 Then $go = $OffsetB
		if $n = 3 Then $go = $OffsetC
		if $n = 4 Then $go = $OffsetD
		if $n = 5 Then $go = $OffsetE
		if $n = 6 Then $go = $OffsetF
		if $n = 7 Then $go = $OffsetG
		if $n = 8 Then $go = $OffsetH
		if $n = 9 Then $go = $OffsetI
		$add = 0x0
		For $i = 1 to 9 Step + 1
			$subOffs = _MemoryRead ("0x" & hex ($go + $add), $DllInformation, 'int')
			$subOffs2 = _MemoryRead ("0x" & hex ($subOffs + 0x238), $DllInformation, 'int')
			if hex ($subOffs2) > "00000000"  Then _report("ADT MCIN Chunk Loaded at: " & hex ($subOffs2),1)
			if _MemoryRead ("0x" & hex ($subOffs2 + 0xC), $DllInformation, 'char[4]') = "RDHM"  Then
				;// Here we are inside the ADT file itself.
				;// Go have a look at www.madx.dk/wowdev/wiki for information about the ADT structure so you can see what possibilities you have get with this.
				$ChrunkCountANA = $ChrunkCountANA + 1
				$ADT[$n][$i] = "0x" & hex ($subOffs2)
				$RDHMbase[$bas] = "0x" & hex ($subOffs2)
				$MCINoffs = hex ((_MemoryRead ("0x" & hex ($subOffs2 + 0x18), $DllInformation, 'int')))
				$MTEXoffs = hex ((_MemoryRead ("0x" & hex ($subOffs2 + 0x1c), $DllInformation, 'int')))
				$MMDXoffs = hex ((_MemoryRead ("0x" & hex ($subOffs2 + 0x20), $DllInformation, 'int')))
				$MMIDoffs = hex ((_MemoryRead ("0x" & hex ($subOffs2 + 0x24), $DllInformation, 'int')))
				$MWMOoffs = hex ((_MemoryRead ("0x" & hex ($subOffs2 + 0x28), $DllInformation, 'int')))
				$MWIDoffs = hex ((_MemoryRead ("0x" & hex ($subOffs2 + 0x2c), $DllInformation, 'int')))
				$MDDFoffs = hex ((_MemoryRead ("0x" & hex ($subOffs2 + 0x30), $DllInformation, 'int')))
				$MODFoffs = hex ((_MemoryRead ("0x" & hex ($subOffs2 + 0x34), $DllInformation, 'int')))
				$MFBOoffs = hex ((_MemoryRead ("0x" & hex ($subOffs2 + 0x38), $DllInformation, 'int')))
				$MH2Ooffs = hex ((_MemoryRead ("0x" & hex ($subOffs2 + 0x3c), $DllInformation, 'int')))
				
				;//MFBO CHUNK Locator - http://www.madx.dk/wowdev/wiki/index.php?title=ADT#MFBO_chunk
				if $MFBOoffs > 0 Then
					_report("Found FLightbox") ;Example of a suboffset
				EndIf
				
				;//MMDX CHUNK Indexing - http://www.madx.dk/wowdev/wiki/index.php?title=ADT#MMDX_chunk
				$M2NameNR = 0
				$MMDXindex = 0
				Do
					$name = _MemoryRead ("0x" & hex (($subOffs2 + 0x1c) + ("0x" & $MMDXoffs) +$MMDXindex), $DllInformation, 'char[200]')
					if $name = "" or StringInStr($name,"DIMM",1) > 0 then ExitLoop
					

					$OBJname[$M2NameNR] = $name
					$MMDXindex = $MMDXindex + StringLen($name)+1
					$M2NameNR = $M2NameNR + 1
				until 1 = 2
				
				
				;//MDDF CHUNK Indexing http://www.madx.dk/wowdev/wiki/index.php?title=ADT#MDDF_chunk
				$MDDFIndex = 0
				$OffsetIndex = "0x" & hex (($subOffs2 + 0x1c) + ("0x" & $MDDFoffs))
				$length = ((($subOffs2 + 0x1c) + ("0x" & $MODFoffs)) - (($subOffs2 + 0x1c) + ("0x" & $MDDFoffs))) / 24
				for $u = 1 to $length
					$_m2ID = _MemoryRead ("0x" & hex ($OffsetIndex), $DllInformation, 'int')
					if StringInStr($_m2ID,"1297040454",1) > 0 then ExitLoop
					$_m2uniqueID[$M2NameIndex] = _MemoryRead ($OffsetIndex + 0x4, $DllInformation, 'int')
					$_m2X = _MemoryRead ($OffsetIndex + 0x8, $DllInformation, 'float')
					$_m2Y = _MemoryRead ($OffsetIndex + 0xc, $DllInformation, 'float')
					$_m2Z = _MemoryRead ($OffsetIndex + 0x10, $DllInformation, 'float')
					_report($_m2uniqueID[$M2NameIndex]& "|" & $OBJname[$_m2ID]& "|" & $_m2X& "|" & $_m2Y& "|" & $_m2Z,2)
					$OffsetIndex = "0x" & hex (($subOffs2 + 0x1c) + ("0x" & $MDDFoffs)+$MDDFIndex)
					$MDDFIndex = $MDDFIndex + 0x24
					$M2NameIndex = $M2NameIndex + 1
					
				Next
				;//MCNK CHUNK Indexing - http://www.madx.dk/wowdev/wiki/index.php?title=ADT#MCNK_chunk
				$MCINintOfs = 0
				For $m = 1 to 256 Step + 1
					$MCNKoffs[$ChrunkCount] = _MemoryRead ("0x" & hex (($subOffs2 + 0x1c) + ("0x" & $MCINoffs) + $MCINintOfs), $DllInformation, 'int')
					$MCNKindenOffs[$m] = "0x" & hex (($subOffs2 + 0x0) + ("0x" & hex ($MCNKoffs[$ChrunkCount])))
					$MCNKinden[$m] = _MemoryRead ($MCNKindenOffs[$m], $DllInformation, 'char[4]')
					$MCNKPosX[$m] = $MCNKindenOffs[$m] + 0x70
					$MCNKPosY[$m] = $MCNKindenOffs[$m] + 0x74
					$MCNKPosZ[$m] = $MCNKindenOffs[$m] + 0x78
					$MCNKindex[$ChrunkCount] = $MCNKindenOffs[$m]
					$finalX[$ChrunkCount] = _MemoryRead ("0x" & hex ($MCNKPosX[$m]), $DllInformation, 'float')
					$finalY[$ChrunkCount] = _MemoryRead ("0x" & hex ($MCNKPosY[$m]), $DllInformation, 'float')
					$finalZ[$ChrunkCount] = _MemoryRead ("0x" & hex ($MCNKPosZ[$m]), $DllInformation, 'float')
					if $Debug = 1 Then _report(floor ($finalX[$ChrunkCount]) & " - " & floor ($finalY[$ChrunkCount]) & " - " & floor ($finalZ[$ChrunkCount]), 1)
					$MCINintOfs = $MCINintOfs + 0x10
					$ChrunkCount = $ChrunkCount + 1
				next
				$bas = $bas + 1
			EndIf
			$add = $add + 0x4
		Next
	Next
	_report("Terrain Details:")
	_report("ADT's Loaded = " & $bas)
	_report("Chrunk's Loaded = " & $ChrunkCount)
	Return
EndFunc   ;==>TerrainManagement


;;================================================================================
;;ObjectManagement()
;;================================================================================
func ObjectManagement()
_report("")	
_report("===OBJECT MANAGEMENT===")	
	;---------------------
	;M2 INDEX
	;---------------------
	$m2LISTCOUNT = 1;OFFS:OFFS:EMPT:OFFS:OFFS
	$ExtchoM2pre = "0x" & hex(_MemoryRead($OffsetM2Static, $DllInformation, 'int')) ;Find Base Pointer
	;Stack Entry
	$M2count = -1
	$searchAdressM2 = $ExtchoM2pre
	$ExtchoM2 = _MemoryRead("0x" & hex($searchAdressM2), $DllInformation, 'int')
	;M2 Gathere
	$i = -1
	while $ExtchoM2 > 0
				$i = $i + 1
				$KnownXm2 = ($ExtchoM2pre + 0x110)
				$m2data[$i][1] = _MemoryRead($KnownXm2, $DllInformation, 'float')
				$m2data[$i][2] = _MemoryRead($KnownXm2 + 0x4, $DllInformation, 'float')
				$m2data[$i][3] = _MemoryRead($KnownXm2 + 0x8, $DllInformation, 'float')
				$m2data[$i][5] = $ExtchoM2pre + 0xc
				$m2data[$i][7] = $KnownXm2
				$m2data[$i][8] = $i
				$searchAdressM2 = $searchAdressM2 + 0xAC
				$m2LISTCOUNT = $m2LISTCOUNT + 1
				$ExtchoM2pre = _MemoryRead("0x" & hex($searchAdressM2), $DllInformation, 'int')
				$ExtchoM2 = _MemoryRead("0x" & hex($ExtchoM2pre), $DllInformation, 'int')
				$searchAdressM2 = "0x" & hex($ExtchoM2pre)
				$M2count = $M2count + 1
	WEnd
	$ExtchoM2 = _MemoryRead("0x" & hex($searchAdressM2), $DllInformation, 'int')
	_report("M2 Files Loaded :" & $M2count)	
	
	;---------------------
	;WMO INDEX
	;---------------------
	$WMOLISTCOUNT = 1;OFFS:OFFS:EMPT:OFFS:OFFS
	$ExtchoWMOpre = 0x & _MemoryRead($OffsetWMOStatic, $DllInformation, 'int')
	$WMOcount = -1
	$searchAdressWMO = $ExtchoWMOpre
	$ExtchoWMO = _MemoryRead("0x" & hex($searchAdressWMO), $DllInformation, 'int')
	;WMO Gathere
	$i = -1
	while $ExtchoWMO > 0
				$i = $i + 1
				$KnownXWMO = ($ExtchoWMOpre + 0xA0)
				$WMOdata[$i][1] = _MemoryRead($KnownXWMO, $DllInformation, 'float')
				$WMOdata[$i][2] = _MemoryRead($KnownXWMO + 0x4, $DllInformation, 'float')
				$WMOdata[$i][3] = _MemoryRead($KnownXWMO + 0x8, $DllInformation, 'float')
				$WMOdata[$i][5] = $ExtchoWMOpre + 0xc
				$WMOdata[$i][7] = $KnownXWMO
				$WMOdata[$i][8] = $i
				$searchAdressWMO = $searchAdressWMO + 0x34
				$WMOLISTCOUNT = $WMOLISTCOUNT + 1
				$ExtchoWMOpre = _MemoryRead("0x" & hex($searchAdressWMO), $DllInformation, 'int')
				$ExtchoWMO = _MemoryRead("0x" & hex($ExtchoWMOpre), $DllInformation, 'int')
				$searchAdressWMO = "0x" & hex($ExtchoWMOpre)
				$WMOcount = $WMOcount + 1
	WEnd
	$ExtchoWMO = _MemoryRead("0x" & hex($searchAdressWMO), $DllInformation, 'int')
	_report("WMO Files Loaded :" & $WMOcount)
	Return
EndFunc   ;==>ObjectManagement


;;================================================================================
;;Offsterrain
;;--------------------------------------------------------------------------------
;; Here we have the sample code, this is not required.
;;================================================================================
func OffsetTerrainUP()
	OffsetTerrain(1.2)
EndFunc   ;==>OffsetTerrainUP
func OffsetTerrainDown()
	OffsetTerrain(-1.2)
EndFunc   ;==>OffsetTerrainDown
Func OffsetTerrain($_input)
	
	;//First of all we want to figure what ADT and Sub Chrunk we want to affect by the Terrain Mover since we don't want to offset ALL terrain loaded
	$posX = _MemoryRead ("0x" & hex ($X), $DllInformation, 'float')
	$posy = _MemoryRead ("0x" & hex ($Y), $DllInformation, 'float')
	if $ChrunkCount > 10 Then
		redim $closer[$ChrunkCount + 1]
		for $i = 0 to $ChrunkCount step + 1
			if $finalX[$i] > $posX Then
				if $finalY[$i] > $posy Then
					$closer[$i] = Sqrt (($finalX[$i] - $posX) ^ 2 + ($finalY[$i] - $posy) ^ 2)
				Else
					$closer[$i] = 9999999999999
				EndIf
			Else
				$closer[$i] = 99999999999
			EndIf
		Next

		$minadress = _ArrayMinindex ($closer, 1)
		$baseChrunk = "0x" & hex ($MCNKindex[$minadress] + 0x90)
		;// Now we have found the baseChrunk we are standing at
		;// We then edit the 145 SUBchrunks with Its current offset +/- the $_input
		for $i = 0 to 145 step + 1
			$currentValue = _MemoryRead ("0x" & hex ($baseChrunk), $DllInformation, 'float')
			_MemoryWrite ("0x" & hex ($baseChrunk), $DllInformation, $currentValue + $_input, 'float')
			$baseChrunk = $baseChrunk + 0x4
		next
	Else
		TerrainManagement()
	EndIf
EndFunc   ;==>OffsetTerrain

;;================================================================================
;;SelectM2
;;================================================================================
func SelectM2()
	HotKeySet ("{Numpad3}")
	if $M2count < 1 Then
		ObjectManagement()
	EndIf
	Dim $radpointX = $MouseSelectEnabler + 0x98
	dim $radpointY = $radpointX + 0x4
	dim $radpointZ = $radpointX + 0x8
	
	_MemoryWrite($MouseSelectEnabler, $DllInformation, 64, 'byte')
	_MemoryWrite($MouseSelectEnabler +0x4, $DllInformation, 170691528, 'int')
	sleep(40)
	$radpointXpos = _MemoryRead($radpointX, $DllInformation, 'float')
	$radpointYpos = _MemoryRead($radpointY, $DllInformation, 'float')
	$radpointZpos = _MemoryRead($radpointZ, $DllInformation, 'float')
	redim $closer[$M2count + 1]
	for $i = 0 to $M2count step +1
		$closer[$i] = Sqrt(($m2data[$i][1] - $radpointXpos) ^ 2 + ($m2data[$i][2] - $radpointYpos) ^ 2)
	Next
	$minadress = _ArrayMinindex($closer, 1)
	$testTOTAL = $m2data[$minadress][7]
	$minadress = _ArrayMinindex($closer, 1)
	$testTOTAL = $m2data[$minadress][7]
	$testTOTAL4 = $testTOTAL - 0x78

	$radpointXpos = _MemoryRead($radpointX, $DllInformation, 'float')
	$radpointYpos = _MemoryRead($radpointY, $DllInformation, 'float')
	$radpointZpos = _MemoryRead($radpointZ, $DllInformation, 'float')


	;ConsoleWrite(" " &hex($testTOTAL) & " " &hex($testTOTAL2) &@CRLF)
	While _IsPressed("63", $dll)
		;ConsoleWrite("M2 Selected "& @CRLF)
		
		;ConsoleWrite("Next Step" & @CRLF)
			Sleep(5)
			$radpointXpos = _MemoryRead($radpointX, $DllInformation, 'float')
			$radpointYpos = _MemoryRead($radpointY, $DllInformation, 'float')
			$radpointZpos = _MemoryRead($radpointZ, $DllInformation, 'float')
			if _MemoryRead("0x" & hex($testTOTAL4), $DllInformation, 'float') < 2.0 Then
				lightup($testTOTAL4)
			EndIf
			;sleep(5)
			redim $closer[$M2count + 1]
			for $i = 0 to $M2count step +1
				$closer[$i] = Sqrt(($m2data[$i][1] - $radpointXpos) ^ 2 + ($m2data[$i][2] - $radpointYpos) ^ 2)
			Next
			$minadressbef = $minadress
			$minadress = _ArrayMinindex($closer, 1)
			if $minadressbef <> $minadress Then
				lightdown($testTOTAL4)
			EndIf	
			$testTOTAL = $m2data[$minadress][7]
			$testTOTAL4 = $testTOTAL - 0x78
			
	WEnd
	lightdown($testTOTAL4)
	$UniqueID = _MemoryRead("0x" & hex($testTOTAL4 + 0x4), $DllInformation, 'int')
	$Index = _ArraySearch($_m2uniqueID,$UniqueID)
	if $Index = -1 then 
		msgbox(0,"Error " & @error,"This Object Have Not Been Indexed: " & $UniqueID)
	Else
		
		_GUICtrlListView_SetSelectedColumn($GUIListSubM2,$Index)
		_GUICtrlListView_SetItemFocused($GUIListSubM2,$Index)
		_GUICtrlListView_EnsureVisible($GUIListSubM2, $Index)
		GUICtrlSetState($GUIListSubM2, $GUI_FOCUS)
		GUICtrlSetColor($M2listitem[$Index], 0xff0000)
	EndIf

	_MemoryWrite($MouseSelectEnabler, $DllInformation, 0, 'int')
	_MemoryWrite($MouseSelectEnabler +0x4, $DllInformation, 0, 'int')
	HotKeySet ("{Numpad3}", "SelectM2")
	Return
EndFunc   ;==>SelectM2

func lightup($LightVarInput)
	ConsoleWrite(hex($LightVarInput - 0x98) &@CRLF)
		For $i = _MemoryRead("0x" & hex($LightVarInput), $DllInformation, 'float') to 2.5 Step +0.1
			_MemoryWrite("0x" & hex($LightVarInput), $DllInformation, $i, 'float')
			sleep(1)
		Next

	Return
EndFunc   ;==>lightup
func lightdown($LightdwnVarInput)
		For $i = _MemoryRead("0x" & hex($LightdwnVarInput), $DllInformation, 'float') to 0.5 Step -0.1
			_MemoryWrite("0x" & hex($LightdwnVarInput), $DllInformation, $i, 'float')
			sleep(1)
		Next
	Return
EndFunc   ;==>lightdown



;;================================================================================
;;_report()
;;================================================================================
func _report($_report, $sub = 0)
	if $sub = 0 Then
		_GUICtrlListBox_AddString ($GUIList, $_report)
		ConsoleWrite (" --- Report: " & $_report & @crlf)
	Elseif $sub = 1 Then
		_GUICtrlListBox_AddString ($GUIListSub, $_report)
		
	ElseIf $sub = 2 Then
		$M2listitem[$M2NameIndex] = GUICtrlCreateListViewItem($_report,$GUIListSubM2)
		if $Debug = 1 Then ConsoleWrite(" --- Report: " &$_report &@crlf)
	EndIf
EndFunc   ;==>_report
;;================================================================================
;;End Loop
;;================================================================================
while 1
	$msg = GUIGetMsg ()
	Select
		Case $msg = $GUI_EVENT_CLOSE
			Exit
	EndSelect
	sleep (10)
WEnd