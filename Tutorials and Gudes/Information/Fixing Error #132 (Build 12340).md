# Fixing Error #132 (Build 12340)

## Introduction
To find out why your client crashes find the numbers matching your error message below. Error messages are of the form:

    ERROR #132 ...
    Program ...
    Exception ... (TYPE) ... at ...
    
    The instruction at INSTRUCTION referenced memory at DATA.
    The memory could not be ACCESS_TYPE.

The following sections are separated by "TYPE". Each subsection then is named with form "INSTRUCTION ACCESS_TYPE DATA".

If your combination of values does not appear below, you may want to add a reply with those values and preferably the "stack trace" part of the error log.

## ACCESS_VIOLATION

### 0x004057FB read 0x00000004
The server sent you to an invalid map id. The client just finished transferring you there and failed getting the internal name of the map in order to load the ADTs, as the map DBC entry does not exist in your client.

### 0x007A07E4 read 0x******** (random address on heap)
    EAX=1D30EF51  EBX=25B15BA0  ECX=00000006  EDX=00000040  ESI=000001C9
    EDI=25B141C0  EBP=036DFA6C  ESP=036DFA58  EIP=007A07E4  FLG=00010212
    CS =0023  	  DS =002B  	ES =002B  	  SS =002B  	FS =0053  		GS =002B
    
    007A07E4 036DFA6C
    007B56C4 036DFA88
    007B6B59 036DFBBC
    00783357 036DFBD8
    004FAAE4 036DFC98
    004FB036 036DFD24
    00485128 036DFDE4
    00494F67 036DFE00
    0049545B 036DFE1C
    004A8A42 036DFEE8
    00480B79 036DFF18
    0047DC89 036DFF40
    0047F29A 036DFF94
    0047F2E1 036DFFAC
    0040B7D8 036DFFB4
    756CBFF2 036DFFEC 0001:0002BFF2 C:\Windows\syswow64\kernel32.dll
    756CBFAA 036DFFFC 0001:0002BFAA C:\Windows\syswow64\kernel32.dll

### 0x007B31F8 read 0x00000010
This issue is caused by the ADT referencing a detail doodad ID that is not given in the DBC, either by saving incorrectly or not editing the DBC correctly. This can be fixed by adding an entry to the DBC for all IDs or by removing the id from the ADT.

> **Ascathos wrote:**
> 
> This is one of the more hairy topics to approach. However, I have a failsafe way to handle it. Basically, each chunk can contain 4 textures (each on a layer), which each can have a specific applied groundeffect ID. The ID is further defined within the .dbc, however, this is not important. The most important thing is simple; Wow tries to visual a groundeffect for a texture that is not existing. On a chunk with 3 layers, but 4 ground effects noted, it is bound to lead to a "false", leading to a crash.
> 
> A simple fix is easy; simply use the texture painter, apply some texture, set it to some size but the lowest speed and apply it onto the adt, until every chunk has 4 layers. The crashes stop.
> 
> Another possibility is to remove all the groundeffects IDs (MCLY) for every chunk, or even specific chunks, to avoid the problem. Refer to the 010 Editor for that.
> 
> The last one is to completely change the groundeffects according to needs. Either by hex, or getting a tool (post it once you have it.)

### 0x007B4D61 read 0x00000071
    EAX=00000001  EBX=00000000  ECX=00000001  EDX=00000FC0  ESI=1155D030
    EDI=00CE87D0  EBP=0385DA28  ESP=0385D9B4  EIP=007B4D61  FLG=00010202
    CS =0023  	  DS =002B  	ES =002B  	  SS =002B  	FS =0053  		GS =002B
    
    007B4D61 0385DA28
    007B5CF5 0385FA8C
    007B6B4E 0385FBBC
    00783357 0385FBD8
    004FAAE4 0385FC98
    004FB036 0385FD24
    00485128 0385FDE4
    00494F67 0385FE00
    0049545B 0385FE1C
    004A8A42 0385FEE8
    00480B79 0385FF18
    0047DC89 0385FF40
    0047F29A 0385FF94
    0047F2E1 0385FFAC
    0040B7D8 0385FFB4
    764BC002 0385FFEC 0001:0002C002 C:\Windows\syswow64\kernel32.dll
    764BBFBA 0385FFFC 0001:0002BFBA C:\Windows\syswow64\kernel32.dll

### 0x007D74B6 read 0x******** (random address on heap)
Reading MOMT of an WMO went terribly wrong. Either the file's size is smaller than it should be, opening the file failed somehow of the chunks before MOMT (MOTX) indicate a bad size. You may encounter this error with WMOs "ported" from other versions (I.e. MoP). You should never encounter thus with not a single WMO on the ADT. Removing then all or fixing the corrupted one should fix this thus.

It should be possible to reproduce this with an WMO being

    00 00 00 00  00 00 00 00  00 00 00 00  FF FF FF FF 00

only. (I.e. infinite MOTX, thus invalid access on MOMT.)

### 0x008347AA read 0x00000008
Related to MCIN according to Will.

### 0x00836D11 read 0x00000000

### 0x007F69D1 read 0x00000000
Trying to call GtBarberCosts.dbc for an entry that does not exist.

### 0x007C59EA read 0x1F312341

### 0x004E0A9B read 0x0000001C

### 0x776B2A83 read 0x5C523282
Caused by MAX_RACES (charactercreate.lua) not matching the amount of buttons in charactercreate.xml.