# Model Edit Fix Documentation

## Offsets
    0xC2A037 - UNKNOWN
    0xCE4CFC  2.0.8
    0xCE6CA4  2.0.12
    if(dwSz == 7095808)  dwOffset = 0xC4ADFF;  // 2.1 (and build 6729)
    if(dwSz == 7096320)  dwOffset = 0xC4AE3F;  // 2.1.1
    if(dwSz == 7105024)  dwOffset = 0xC4CEEF;  // 2.1.2
    if(dwSz == 7107584)  dwOffset = 0xC4DF97;  // 2.1.3
    if(dwSz == 7841280)  dwOffset = 0xCC0A44;  // 2.2.0
    if(dwSz == 7844352)  dwOffset = 0x622647;  // 2.2.2
    if(dwSz == 7837184)  dwOffset = 0x621CD7;  // 2.2.3

## Explanation
The model edit fix works by disabling the signature check done on MPQ files, this allows a user to substitute their own MPQs in place of Blizzard's original ones without the game rejecting the file.

There are two pieces of code used to verify that the MPQ data is consistent. (These can be found simply by running a string search in IDA for the relevant information, in this case, errors about the signature file.)

**Subroutine 1:**

      .text:00402060 ; ¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦ S U B R O U T I N E ¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦

      .text:00402060

      .text:00402060 ; Attributes: bp-based frame

      .text:00402060

      .text:00402060 sub_402060      proc near               ; CODE XREF: sub_404880+52Cp

      .text:00402060

      .text:00402060 var_8C          = dword ptr -8Ch

      .text:00402060 var_7C          = dword ptr -7Ch

      .text:00402060 var_78          = dword ptr -78h

      .text:00402060 var_74          = dword ptr -74h

      .text:00402060 var_70          = dword ptr -70h

      .text:00402060 var_6C          = dword ptr -6Ch

      .text:00402060 var_68          = dword ptr -68h

      .text:00402060 var_4           = dword ptr -4

      .text:00402060 arg_0           = dword ptr  8

      .text:00402060

      .text:00402060                 push    ebp

      .text:00402061                 mov     ebp, esp

      .text:00402063                 sub     esp, 8Ch

      .text:00402069                 mov     eax, dword_8AE4E8

      .text:0040206E                 xor     eax, ebp

      .text:00402070                 mov     [ebp+var_4], eax

      .text:00402073                 call    sub_621170

      .text:00402078                 push    0               ; int

      .text:0040207A                 push    0               ; size_t

      .text:0040207C                 lea     eax, [ebp+var_70]

      .text:0040207F                 push    eax             ; int

      .text:00402080                 lea     ecx, [ebp+var_6C]

      .text:00402083                 push    ecx             ; int

      .text:00402084                 push    offset aSignaturefile ; "signaturefile"

      .text:00402089                 call    sub_61F8A0

      .text:0040208E                 test    eax, eax

      .text:00402090                 jnz     short loc_4020AB

      .text:00402092                 push    1               ; uExitCode

      .text:00402094                 push    eax             ; int

      .text:00402095                 push    offset aSignaturefile ; "signaturefile"

      .text:0040209A                 push    934h            ; int

      .text:0040209F                 push    offset a_Client_cpp ; ".\\Client.cpp"

      .text:004020A4                 push    2               ; int

      .text:004020A6                 call    sub_625DD0

      .text:004020AB

      .text:004020AB loc_4020AB:                             ; CODE XREF: sub_402060+30j

      .text:004020AB                 mov     edx, [ebp+var_70]

      .text:004020AE                 mov     eax, [ebp+var_6C]

      .text:004020B1                 push    4

      .text:004020B3                 push    offset unk_8338E8

      .text:004020B8                 push    100h

      .text:004020BD                 push    offset unk_8AE008

      .text:004020C2                 push    edx

      .text:004020C3                 push    eax

      .text:004020C4                 call    sub_626D20

      .text:004020C9                 add     esp, 18h

      .text:004020CC                 test    eax, eax

      .text:004020CE                 jnz     short loc_40210B

      .text:004020D0                 push    offset aSignaturefile ; "signaturefile"

      .text:004020D5                 push    offset aCouldNotAuthen ; "Could not authenticate the %s file."

      .text:004020DA                 lea     ecx, [ebp+var_68]

      .text:004020DD                 push    64h

      .text:004020DF                 push    ecx

      .text:004020E0                 call    sub_61B710

      .text:004020E5                 add     esp, 10h

      .text:004020E8                 push    0

      .text:004020EA                 call    sub_625B00

      .text:004020EF                 push    1               ; uExitCode

      .text:004020F1                 push    0               ; int

      .text:004020F3                 lea     edx, [ebp+var_68]

      .text:004020F6                 push    edx             ; int

      .text:004020F7                 push    946h            ; int

      .text:004020FC                 push    offset a_Client_cpp ; ".\\Client.cpp"

      .text:00402101                 push    4DCh            ; int

      .text:00402106                 call    sub_625DD0

      .text:0040210B

      .text:0040210B loc_40210B:                             ; CODE XREF: sub_402060+6Ej

      .text:0040210B                 mov     eax, [ebp+var_6C]

      .text:0040210E                 push    ebx

      .text:0040210F                 push    esi

      .text:00402110                 push    edi

      .text:00402111                 push    offset asc_83D040 ; "\r\n"

      .text:00402116                 push    eax

      .text:00402117                 mov     edi, eax

      .text:00402119                 call    sub_61BAB0

      .text:0040211E                 add     esp, 8

      .text:00402121                 mov     esi, eax

      .text:00402123                 push    edi

      .text:00402124                 mov     byte ptr [esi], 0

      .text:00402127                 call    sub_61B770

      .text:0040212C                 add     esi, 2

      .text:0040212F                 push    offset asc_83D040 ; "\r\n"

      .text:00402134                 push    esi

      .text:00402135                 mov     ebx, eax

      .text:00402137                 call    sub_61BAB0

      .text:0040213C                 mov     edi, eax

      .text:0040213E                 push    esi             ; char *

      .text:0040213F                 mov     byte ptr [edi], 0

      .text:00402142                 call    j__atol

      .text:00402147                 add     esp, 0Ch

      .text:0040214A                 cmp     eax, 1

      .text:0040214D                 jz      short loc_402169

      .text:0040214F                 push    1               ; uExitCode

      .text:00402151                 push    0               ; int

      .text:00402153                 push    0               ; int

      .text:00402155                 push    966h            ; int

      .text:0040215A                 push    offset a_Client_cpp ; ".\\Client.cpp"

      .text:0040215F                 push    85100083h       ; int

      .text:00402164                 call    sub_625DD0

      .text:00402169

      .text:00402169 loc_402169:                             ; CODE XREF: sub_402060+EDj

      .text:00402169                 add     edi, 2

      .text:0040216C                 push    offset asc_83D040 ; "\r\n"

      .text:00402171                 push    edi

      .text:00402172                 call    sub_61BAB0

      .text:00402177                 add     esp, 8

      .text:0040217A                 push    7               ; size_t

      .text:0040217C                 push    offset aRelease ; "RELEASE"

      .text:00402181                 mov     esi, eax

      .text:00402183                 push    edi             ; int

      .text:00402184                 mov     byte ptr [esi], 0

      .text:00402187                 call    sub_61B4C0

      .text:0040218C                 test    eax, eax

      .text:0040218E                 jz      short loc_4021AA

      .text:00402190                 push    1               ; uExitCode

      .text:00402192                 push    0               ; int

      .text:00402194                 push    0               ; int

      .text:00402196                 push    983h            ; int

      .text:0040219B                 push    offset a_Client_cpp ; ".\\Client.cpp"

      .text:004021A0                 push    85100083h       ; int

      .text:004021A5                 call    sub_625DD0

      .text:004021AA

      .text:004021AA loc_4021AA:                             ; CODE XREF: sub_402060+12Ej

      .text:004021AA                 add     esi, 2

      .text:004021AD                 cmp     ebx, 2

      .text:004021B0                 jnz     loc_40226D

      .text:004021B6                 call    sub_401DD0

      .text:004021BB                 mov     edx, [ebp+var_70]

      .text:004021BE                 cmp     al, 1

      .text:004021C0                 sbb     eax, eax

      .text:004021C2                 add     eax, 1

      .text:004021C5                 mov     [ebp+var_74], eax

      .text:004021C8                 mov     eax, [ebp+arg_0]

      .text:004021CB                 mov     ecx, [eax]

      .text:004021CD                 mov     eax, [ebp+var_6C]

      .text:004021D0                 add     eax, edx

      .text:004021D2                 cmp     esi, eax

      .text:004021D4                 mov     [ebp+var_78], ecx

      .text:004021D7                 jnb     loc_402287

      .text:004021DD                 lea     ecx, [ecx+0]

      .text:004021E0

      .text:004021E0 loc_4021E0:                             ; CODE XREF: sub_402060+205j

      .text:004021E0                 push    4               ; size_t

      .text:004021E2                 push    offset aNgis    ; "NGIS"

      .text:004021E7                 push    esi             ; int

      .text:004021E8                 call    sub_61B4C0

      .text:004021ED                 test    eax, eax

      .text:004021EF                 jz      loc_402287

      .text:004021F5                 lea     edi, [esi+7]

      .text:004021F8                 lea     eax, [edi+21h]

      .text:004021FB                 mov     byte ptr [esi+4], 0

      .text:004021FF                 push    offset asc_83D040 ; "\r\n"

      .text:00402204                 mov     byte ptr [esi+6], 0

      .text:00402208                 push    eax

      .text:00402209                 mov     byte ptr [edi+20h], 0

      .text:0040220D                 mov     [ebp+var_7C], eax

      .text:00402210                 call    sub_61BAB0

      .text:00402215                 mov     ebx, eax

      .text:00402217                 mov     byte ptr [ebx], 0

      .text:0040221A                 mov     eax, [esi]

      .text:0040221C                 add     esp, 8

      .text:0040221F                 cmp     eax, 65736162h
      .text:00402224                 jz      short loc_40222B
      .text:00402226                 cmp     eax, [ebp+var_78]
      .text:00402229                 jnz     short loc_402258
      .text:0040222B
      .text:0040222B loc_40222B:                             ; CODE XREF: sub_402060+1C4j
      .text:0040222B                 mov     al, [esi+5]
      .text:0040222E                 cmp     al, 63h
      .text:00402230                 jz      short loc_40223C
      .text:00402232                 cmp     al, 65h
      .text:00402234                 jnz     short loc_402258
      .text:00402236                 cmp     [ebp+var_74], 0
      .text:0040223A                 jz      short loc_402258
      .text:0040223C
      .text:0040223C loc_40223C:                             ; CODE XREF: sub_402060+1D0j
      .text:0040223C                 push    edi
      .text:0040223D                 lea     ecx, [ebp+var_8C]
      .text:00402243                 call    sub_624FF0
      .text:00402248                 mov     edx, [ebp+var_7C]
      .text:0040224B                 lea     ecx, [ebp+var_8C]
      .text:00402251                 push    ecx
      .text:00402252                 push    edx
      .text:00402253                 call    sub_6223A0
      .text:00402258
      .text:00402258 loc_402258:                             ; CODE XREF: sub_402060+1C9j
      .text:00402258                                         ; sub_402060+1D4j ...
      .text:00402258                 mov     eax, [ebp+var_70]
      .text:0040225B                 mov     ecx, [ebp+var_6C]
      .text:0040225E                 lea     esi, [ebx+2]
      .text:00402261                 add     ecx, eax
      .text:00402263                 cmp     esi, ecx
      .text:00402265                 jb      loc_4021E0
      .text:0040226B                 jmp     short loc_402287
      .text:0040226D ; ---------------------------------------------------------------------------
      .text:0040226D
      .text:0040226D loc_40226D:                             ; CODE XREF: sub_402060+150j
      .text:0040226D                 push    1               ; uExitCode
      .text:0040226F                 push    0               ; int
      .text:00402271                 push    0               ; int
      .text:00402273                 push    9BAh            ; int
      .text:00402278                 push    offset a_Client_cpp ; ".\\Client.cpp"
      .text:0040227D                 push    85100083h       ; int
      .text:00402282                 call    sub_625DD0
      .text:00402287
      .text:00402287 loc_402287:                             ; CODE XREF: sub_402060+177j
      .text:00402287                                         ; sub_402060+18Fj ...
      .text:00402287                 mov     edx, [ebp+var_6C]
      .text:0040228A                 push    edx
      .text:0040228B                 call    sub_61CD60
      .text:00402290                 mov     ecx, [ebp+var_4]
      .text:00402293                 pop     edi
      .text:00402294                 pop     esi
      .text:00402295                 xor     ecx, ebp
      .text:00402297                 pop     ebx
      .text:00402298                 call    sub_4096EA
      .text:0040229D                 mov     esp, ebp
      .text:0040229F                 pop     ebp
      .text:004022A0                 retn
      .text:004022A0 sub_402060      endp
      .text:004022A0
      .text:004022A0 ; ---------------------------------------------------------------------------

**Subroutine 2:**
      .text:00621CC0 ; ¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦ S U B R O U T I N E ¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦

      .text:00621CC0

      .text:00621CC0

      .text:00621CC0 sub_621CC0      proc near               ; CODE XREF: sub_623680+277p

      .text:00621CC0                                         ; sub_623B40+30Ep ...

      .text:00621CC0                 lea     eax, [edi+150h]

      .text:00621CC6                 push    eax

      .text:00621CC7                 lea     ecx, [edi+1FCh]

      .text:00621CCD                 call    sub_624FA0

      .text:00621CD2                 test    al, al

      .text:00621CD4                 jz      short loc_621CF3

      .text:00621CD6                 push    0

      .text:00621CD8                 call    sub_625B00

      .text:00621CDD                 push    1               ; uExitCode

      .text:00621CDF                 push    0               ; int

      .text:00621CE1                 push    0               ; int

      .text:00621CE3                 push    0FFFFFFFCh      ; int

      .text:00621CE5                 lea     ecx, [edi+24h]

      .text:00621CE8                 push    ecx             ; int

      .text:00621CE9                 push    85100083h       ; int

      .text:00621CEE                 call    sub_625DD0

      .text:00621CF3

      .text:00621CF3 loc_621CF3:                             ; CODE XREF: sub_621CC0+14j

      .text:00621CF3                 cmp     dword_CBF61C, 2

      .text:00621CFA                 jz      short loc_621D42

      .text:00621CFC                 mov     eax, [edi+130h]

      .text:00621D02                 push    esi

      .text:00621D03                 push    eax

      .text:00621D04                 call    sub_621330

      .text:00621D09                 mov     esi, eax

      .text:00621D0B                 add     esp, 4

      .text:00621D0E                 test    esi, esi

      .text:00621D10                 jz      short loc_621D1C

      .text:00621D12                 lea     edx, [esi+8]

      .text:00621D15                 push    edx             ; lpCriticalSection

      .text:00621D16                 call    ds:EnterCriticalSection

      .text:00621D1C

      .text:00621D1C loc_621D1C:                             ; CODE XREF: sub_621CC0+50j

      .text:00621D1C                 mov     ecx, [edi+160h]

      .text:00621D22                 add     ecx, 1Ch

      .text:00621D25                 call    sub_625020

      .text:00621D2A                 test    esi, esi

      .text:00621D2C                 jz      short loc_621D38

      .text:00621D2E                 lea     eax, [esi+8]

      .text:00621D31                 push    eax             ; lpCriticalSection

      .text:00621D32                 call    ds:LeaveCriticalSection

      .text:00621D38

      .text:00621D38 loc_621D38:                             ; CODE XREF: sub_621CC0+6Cj

      .text:00621D38                 push    esi

      .text:00621D39                 call    sub_621450

      .text:00621D3E                 add     esp, 4

      .text:00621D41                 pop     esi

      .text:00621D42

      .text:00621D42 loc_621D42:                             ; CODE XREF: sub_621CC0+3Aj

      .text:00621D42                 mov     dword ptr [edi+1A0h], 4

      .text:00621D4C                 retn

      .text:00621D4C sub_621CC0      endp

      .text:00621D4C

      .text:00621D4C ; ---------------------------------------------------------------------------

An important thing we can note from these two subroutines is that they are both calling these two relevant subroutines.

**Subroutine 1:**


      .text:00625DD0 ; ¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦ S U B R O U T I N E ¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦

      .text:00625DD0

      .text:00625DD0 ; Attributes: bp-based frame

      .text:00625DD0

      .text:00625DD0 ; int __stdcall sub_625DD0(int,int,int,int,int,UINT uExitCode)

      .text:00625DD0 sub_625DD0      proc near               ; CODE XREF: sub_402060+46p

      .text:00625DD0                                         ; sub_402060+A6p ...

      .text:00625DD0

      .text:00625DD0 SystemTime      = _SYSTEMTIME ptr -0D74h

      .text:00625DD0 var_D64         = dword ptr -0D64h

      .text:00625DD0 ExitCode        = dword ptr -0D60h

      .text:00625DD0 var_D5C         = dword ptr -0D5Ch

      .text:00625DD0 var_D58         = dword ptr -0D58h

      .text:00625DD0 var_D54         = dword ptr -0D54h

      .text:00625DD0 var_D50         = dword ptr -0D50h

      .text:00625DD0 FindFileData    = _WIN32_FIND_DATAA ptr -0D4Ch

      .text:00625DD0 var_C0C         = dword ptr -0C0Ch

      .text:00625DD0 Text            = byte ptr -0B0Ch

      .text:00625DD0 FileName        = byte ptr -30Ch

      .text:00625DD0 Caption         = byte ptr -208h

      .text:00625DD0 var_104         = dword ptr -104h

      .text:00625DD0 var_4           = dword ptr -4

      .text:00625DD0 arg_0           = dword ptr  8

      .text:00625DD0 arg_4           = dword ptr  0Ch

      .text:00625DD0 arg_8           = dword ptr  10h

      .text:00625DD0 arg_C           = dword ptr  14h

      .text:00625DD0 arg_10          = dword ptr  18h

      .text:00625DD0 uExitCode       = dword ptr  1Ch

      .text:00625DD0

      .text:00625DD0                 push    ebp

      .text:00625DD1                 mov     ebp, esp

      .text:00625DD3                 sub     esp, 0D74h

      .text:00625DD9                 mov     eax, dword_8AE4E8

      .text:00625DDE                 xor     eax, ebp

      .text:00625DE0                 mov     [ebp+var_4], eax

      .text:00625DE3                 cmp     dword_CBF70C, 0

      .text:00625DEA                 mov     eax, [ebp+arg_C]

      .text:00625DED                 push    edi

      .text:00625DEE                 mov     edi, [ebp+arg_4]

      .text:00625DF1                 mov     [ebp+var_D58], eax

      .text:00625DF7                 jz      short loc_625E0C

      .text:00625DF9

      .text:00625DF9 loc_625DF9:                             ; CODE XREF: sub_625DD0+84j

      .text:00625DF9                 xor     eax, eax

      .text:00625DFB                 pop     edi

      .text:00625DFC                 mov     ecx, [ebp+var_4]

      .text:00625DFF                 xor     ecx, ebp

      .text:00625E01                 call    sub_4096EA

      .text:00625E06                 mov     esp, ebp

      .text:00625E08                 pop     ebp

      .text:00625E09                 retn    18h

      .text:00625E0C ; ---------------------------------------------------------------------------

      .text:00625E0C

      .text:00625E0C loc_625E0C:                             ; CODE XREF: sub_625DD0+27j

      .text:00625E0C                 cmp     dword_CBF9F4, 0

      .text:00625E13                 jz      short loc_625E4D

      .text:00625E15                 cmp     edi, dword_CBF9AC

      .text:00625E1B                 jnz     short loc_625E3E

      .text:00625E1D                 mov     ecx, [ebp+arg_8]

      .text:00625E20                 cmp     ecx, dword_CBF9A8

      .text:00625E26                 jnz     short loc_625E3E

      .text:00625E28                 mov     eax, 1

      .text:00625E2D                 pop     edi

      .text:00625E2E                 mov     ecx, [ebp+var_4]

      .text:00625E31                 xor     ecx, ebp

      .text:00625E33                 call    sub_4096EA

      .text:00625E38                 mov     esp, ebp

      .text:00625E3A                 pop     ebp

      .text:00625E3B                 retn    18h

      .text:00625E3E ; ---------------------------------------------------------------------------

      .text:00625E3E

      .text:00625E3E loc_625E3E:                             ; CODE XREF: sub_625DD0+4Bj

      .text:00625E3E                                         ; sub_625DD0+56j

      .text:00625E3E                 mov     edx, [ebp+arg_8]

      .text:00625E41                 mov     dword_CBF9AC, edi

      .text:00625E47                 mov     dword_CBF9A8, edx

      .text:00625E4D

      .text:00625E4D loc_625E4D:                             ; CODE XREF: sub_625DD0+43j

      .text:00625E4D                 cmp     dword_CBF6DC, 0

      .text:00625E54                 jnz     short loc_625DF9

      .text:00625E56                 push    104h            ; nSize

      .text:00625E5B                 lea     eax, [ebp+FileName]

      .text:00625E61                 push    eax             ; lpFilename

      .text:00625E62                 push    0               ; hModule

      .text:00625E64                 mov     dword_CBF6DC, 1

      .text:00625E6E                 mov     [ebp+FileName], 0

      .text:00625E75                 mov     [ebp+Caption], 0

      .text:00625E7C                 call    ds:GetModuleFileNameA

      .text:00625E82                 push    140h            ; size_t

      .text:00625E87                 lea     ecx, [ebp+FindFileData]

      .text:00625E8D                 push    0               ; int

      .text:00625E8F                 push    ecx             ; void *

      .text:00625E90                 call    _memset

      .text:00625E95                 add     esp, 0Ch

      .text:00625E98                 lea     edx, [ebp+FindFileData]

      .text:00625E9E                 push    edx             ; lpFindFileData

      .text:00625E9F                 lea     eax, [ebp+FileName]

      .text:00625EA5                 push    eax             ; lpFileName

      .text:00625EA6                 call    ds:FindFirstFileA

      .text:00625EAC                 test    eax, eax

      .text:00625EAE                 jz      short loc_625EB7

      .text:00625EB0                 push    eax             ; hFindFile

      .text:00625EB1                 call    ds:FindClose

      .text:00625EB7

      .text:00625EB7 loc_625EB7:                             ; CODE XREF: sub_625DD0+DEj

      .text:00625EB7                 push    104h

      .text:00625EBC                 lea     ecx, [ebp+FindFileData.cFileName]

      .text:00625EC2                 push    ecx

      .text:00625EC3                 lea     edx, [ebp+Caption]

      .text:00625EC9                 push    edx

      .text:00625ECA                 call    sub_61B540

      .text:00625ECF                 lea     eax, [ebp+Caption]

      .text:00625ED5                 push    2Eh

      .text:00625ED7                 push    eax

      .text:00625ED8                 call    sub_61B480

      .text:00625EDD                 add     esp, 8

      .text:00625EE0                 test    eax, eax

      .text:00625EE2                 jz      short loc_625EF8

      .text:00625EE4                 lea     ecx, [ebp+Caption]

      .text:00625EEA                 push    2Eh

      .text:00625EEC                 push    ecx

      .text:00625EED                 call    sub_61B480

      .text:00625EF2                 add     esp, 8

      .text:00625EF5                 mov     byte ptr [eax], 0

      .text:00625EF8

      .text:00625EF8 loc_625EF8:                             ; CODE XREF: sub_625DD0+112j

      .text:00625EF8                 push    esi

      .text:00625EF9                 mov     esi, [ebp+arg_0]

      .text:00625EFC                 push    100h            ; nSize

      .text:00625F01                 lea     edx, [ebp+var_104]

      .text:00625F07                 push    edx             ; int

      .text:00625F08                 push    esi             ; int

      .text:00625F09                 mov     byte ptr [ebp+var_104], 0

      .text:00625F10                 call    sub_625BC0

      .text:00625F15                 cmp     byte ptr [ebp+var_104], 0

      .text:00625F1C                 jnz     short loc_625F47

      .text:00625F1E                 mov     ecx, esi

      .text:00625F20                 call    sub_6255D0

      .text:00625F25                 push    eax

      .text:00625F26                 movzx   eax, si

      .text:00625F29                 push    esi

      .text:00625F2A                 push    eax

      .text:00625F2B                 xor     esi, esi

      .text:00625F2D                 call    sub_625030

      .text:00625F32                 push    eax

      .text:00625F33                 lea     ecx, [ebp+var_104]

      .text:00625F39                 push    100h

      .text:00625F3E                 push    ecx

      .text:00625F3F                 call    sub_61B710

      .text:00625F44                 add     esp, 18h

      .text:00625F47

      .text:00625F47 loc_625F47:                             ; CODE XREF: sub_625DD0+14Cj

      .text:00625F47                 test    edi, edi

      .text:00625F49                 push    ebx

      .text:00625F4A                 mov     [ebp+var_D64], edi

      .text:00625F50                 jz      short loc_625F86

      .text:00625F52                 cmp     byte ptr [edi], 0

      .text:00625F55                 jz      short loc_625F86

      .text:00625F57                 mov     eax, [ebp+arg_8]

      .text:00625F5A                 cmp     eax, 0FFFFFFFEh

      .text:00625F5D                 jz      short loc_625F64

      .text:00625F5F                 cmp     eax, 0FFFFFFFDh

      .text:00625F62                 jnz     short loc_625F86

      .text:00625F64

      .text:00625F64 loc_625F64:                             ; CODE XREF: sub_625DD0+18Dj

      .text:00625F64                 push    edi

      .text:00625F65                 mov     ebx, 100h

      .text:00625F6A                 lea     esi, [ebp+var_C0C]

      .text:00625F70                 call    sub_625B20

      .text:00625F75                 add     esp, 4

      .text:00625F78                 test    eax, eax

      .text:00625F7A                 jz      short loc_625F86

      .text:00625F7C                 mov     edx, esi

      .text:00625F7E                 mov     [ebp+var_D64], edx

      .text:00625F84                 mov     edi, edx

      .text:00625F86

      .text:00625F86 loc_625F86:                             ; CODE XREF: sub_625DD0+180j

      .text:00625F86                                         ; sub_625DD0+185j ...

      .text:00625F86                 test    edi, edi

      .text:00625F88                 lea     esi, [ebp+Text]

      .text:00625F8E                 jz      short loc_625FA5

      .text:00625F90                 push    800h

      .text:00625F95                 push    edi

      .text:00625F96                 mov     eax, esi

      .text:00625F98                 push    eax

      .text:00625F99                 call    sub_61B540

      .text:00625F9E                 lea     esi, [ebp+eax+Text]

      .text:00625FA5

      .text:00625FA5 loc_625FA5:                             ; CODE XREF: sub_625DD0+1BEj

      .text:00625FA5                 mov     eax, [ebp+arg_8]

      .text:00625FA8                 test    eax, eax

      .text:00625FAA                 jle     short loc_625FCC

      .text:00625FAC                 push    eax

      .text:00625FAD                 lea     ecx, [ebp+FileName]

      .text:00625FB3                 push    offset aU_0     ; "(%u)"

      .text:00625FB8                 sub     ecx, esi

      .text:00625FBA                 push    ecx

      .text:00625FBB                 push    esi

      .text:00625FBC                 call    sub_61B710

      .text:00625FC1                 add     esp, 10h

      .text:00625FC4                 push    esi

      .text:00625FC5                 call    sub_61B5C0

      .text:00625FCA                 add     esi, eax

      .text:00625FCC

      .text:00625FCC loc_625FCC:                             ; CODE XREF: sub_625DD0+1DAj

      .text:00625FCC                 movzx   edx, word ptr [ebp+arg_0]

      .text:00625FD0                 push    edx

      .text:00625FD1                 lea     eax, [ebp+FileName]

      .text:00625FD7                 push    offset aErrorU  ; " : error %u: "

      .text:00625FDC                 sub     eax, esi

      .text:00625FDE                 push    eax

      .text:00625FDF                 push    esi

      .text:00625FE0                 call    sub_61B710

      .text:00625FE5                 add     esp, 10h

      .text:00625FE8                 push    esi

      .text:00625FE9                 call    sub_61B5C0

      .text:00625FEE                 add     esi, eax

      .text:00625FF0                 lea     ecx, [ebp+FileName]

      .text:00625FF6                 sub     ecx, esi

      .text:00625FF8                 push    ecx

      .text:00625FF9                 lea     edx, [ebp+var_104]

      .text:00625FFF                 push    edx

      .text:00626000                 push    esi

      .text:00626001                 call    sub_61B540

      .text:00626006                 lea     eax, [ebp+Text]

      .text:0062600C                 push    eax             ; char

      .text:0062600D                 push    offset aS_0     ; "%s\n"

      .text:00626012                 call    sub_62E610

      .text:00626017                 lea     ecx, [ebp+var_104]

      .text:0062601D                 add     esp, 8

      .text:00626020                 push    ecx

      .text:00626021                 mov     esi, 1

      .text:00626026                 call    sub_625030

      .text:0062602B                 push    eax

      .text:0062602C                 lea     edx, [ebp+Text]

      .text:00626032                 push    800h

      .text:00626037                 push    edx

      .text:00626038                 call    sub_61B710

      .text:0062603D                 add     esp, 10h

      .text:00626040                 lea     eax, [ebp+Text]

      .text:00626046                 push    eax

      .text:00626047                 call    sub_61B5C0

      .text:0062604C                 lea     ecx, [ebp+FileName]

      .text:00626052                 push    ecx

      .text:00626053                 mov     esi, 2

      .text:00626058                 lea     ebx, [ebp+eax+Text]

      .text:0062605F                 call    sub_625030

      .text:00626064

      .text:00626064 loc_626064:                             ; DATA XREF: .rdata:00890378o

      .text:00626064                 push    eax

      .text:00626065                 lea     edx, [ebp+FileName]

      .text:0062606B                 sub     edx, ebx

      .text:0062606D                 push    edx

      .text:0062606E                 push    ebx

      .text:0062606F                 call    sub_61B710

      .text:00626074                 add     esp, 10h

      .text:00626077                 push    ebx

      .text:00626078                 call    sub_61B5C0

      .text:0062607D                 add     ebx, eax

      .text:0062607F                 test    edi, edi

      .text:00626081                 jz      loc_62615A

      .text:00626087                 cmp     byte ptr [edi], 0

      .text:0062608A                 jz      loc_62615A

      .text:00626090                 mov     eax, [ebp+arg_8]

      .text:00626093                 add     eax, 5          ; switch 5 cases

      .text:00626096                 cmp     eax, 4

      .text:00626099                 ja      loc_626130      ; default

      .text:0062609F                 jmp     ds:off_626550[eax*4] ; switch jump

      .text:006260A6

      .text:006260A6 loc_6260A6:                             ; DATA XREF: .text:off_626550o

      .text:006260A6                 mov     esi, 4          ; case -0x1

      .text:006260AB

      .text:006260AB loc_6260AB:                             ; CODE XREF: sub_625DD0+33Ej

      .text:006260AB                 push    edi

      .text:006260AC                 call    sub_625030

      .text:006260B1                 push    eax

      .text:006260B2                 lea     eax, [ebp+FileName]

      .text:006260B8                 sub     eax, ebx

      .text:006260BA                 push    eax

      .text:006260BB                 push    ebx

      .text:006260BC                 call    sub_61B710

      .text:006260C1                 add     esp, 10h

      .text:006260C4                 jmp     loc_626152

      .text:006260C9 ; ---------------------------------------------------------------------------

      .text:006260C9

      .text:006260C9 loc_6260C9:                             ; CODE XREF: sub_625DD0+2CFj

      .text:006260C9                                         ; DATA XREF: .text:off_626550o

      .text:006260C9                 push    edi             ; case -0x2

      .text:006260CA                 mov     esi, 5

      .text:006260CF                 call    sub_625030

      .text:006260D4                 push    eax

      .text:006260D5                 lea     ecx, [ebp+FileName]

      .text:006260DB                 sub     ecx, ebx

      .text:006260DD                 push    ecx

      .text:006260DE                 push    ebx

      .text:006260DF                 call    sub_61B710

      .text:006260E4                 add     esp, 10h

      .text:006260E7                 jmp     short loc_626152

      .text:006260E9 ; ---------------------------------------------------------------------------

      .text:006260E9

      .text:006260E9 loc_6260E9:                             ; CODE XREF: sub_625DD0+2CFj

      .text:006260E9                                         ; DATA XREF: .text:off_626550o

      .text:006260E9                 push    edi             ; case -0x3

      .text:006260EA                 mov     esi, 6

      .text:006260EF                 call    sub_625030

      .text:006260F4                 push    eax

      .text:006260F5                 lea     edx, [ebp+FileName]

      .text:006260FB                 sub     edx, ebx

      .text:006260FD                 push    edx

      .text:006260FE                 push    ebx

      .text:006260FF                 call    sub_61B710

      .text:00626104                 add     esp, 10h

      .text:00626107                 jmp     short loc_626152

      .text:00626109 ; ---------------------------------------------------------------------------

      .text:00626109

      .text:00626109 loc_626109:                             ; CODE XREF: sub_625DD0+2CFj

      .text:00626109                                         ; DATA XREF: .text:off_626550o

      .text:00626109                 mov     esi, 0Bh        ; case -0x4

      .text:0062610E                 jmp     short loc_6260AB

      .text:00626110 ; ---------------------------------------------------------------------------

      .text:00626110

      .text:00626110 loc_626110:                             ; CODE XREF: sub_625DD0+2CFj

      .text:00626110                                         ; DATA XREF: .text:off_626550o

      .text:00626110                 push    edi             ; case -0x5

      .text:00626111                 mov     esi, 0Eh

      .text:00626116                 call    sub_625030

      .text:0062611B                 push    eax

      .text:0062611C                 lea     ecx, [ebp+FileName]

      .text:00626122                 sub     ecx, ebx

      .text:00626124                 push    ecx

      .text:00626125                 push    ebx

      .text:00626126                 call    sub_61B710

      .text:0062612B                 add     esp, 10h

      .text:0062612E                 jmp     short loc_626152

      .text:00626130 ; ---------------------------------------------------------------------------

      .text:00626130

      .text:00626130 loc_626130:                             ; CODE XREF: sub_625DD0+2C9j

      .text:00626130                 mov     edx, [ebp+arg_8] ; default

      .text:00626133                 push    edx

      .text:00626134                 push    edi

      .text:00626135                 mov     esi, 3

      .text:0062613A                 call    sub_625030

      .text:0062613F                 push    eax

      .text:00626140                 lea     eax, [ebp+FileName]

      .text:00626146                 sub     eax, ebx

      .text:00626148                 push    eax

      .text:00626149                 push    ebx

      .text:0062614A                 call    sub_61B710

      .text:0062614F                 add     esp, 14h

      .text:00626152

      .text:00626152 loc_626152:                             ; CODE XREF: sub_625DD0+2F4j

      .text:00626152                                         ; sub_625DD0+317j ...

      .text:00626152                 push    ebx

      .text:00626153                 call    sub_61B5C0

      .text:00626158                 add     ebx, eax

      .text:0062615A

      .text:0062615A loc_62615A:                             ; CODE XREF: sub_625DD0+2B1j

      .text:0062615A                                         ; sub_625DD0+2BAj

      .text:0062615A                 cmp     [ebp+arg_0], 85100000h

      .text:00626161                 mov     eax, [ebp+var_D58]

      .text:00626167                 jnz     short loc_626189

      .text:00626169                 test    eax, eax

      .text:0062616B                 jnz     short loc_626172

      .text:0062616D                 mov     eax, offset byte_834463

      .text:00626172

      .text:00626172 loc_626172:                             ; CODE XREF: sub_625DD0+39Bj

      .text:00626172                 push    eax

      .text:00626173                 mov     esi, 7

      .text:00626178                 call    sub_625030

      .text:0062617D                 lea     ecx, [ebp+FileName]

      .text:00626183                 push    eax

      .text:00626184                 sub     ecx, ebx

      .text:00626186                 push    ecx

      .text:00626187                 jmp     short loc_6261A7

      .text:00626189 ; ---------------------------------------------------------------------------

      .text:00626189

      .text:00626189 loc_626189:                             ; CODE XREF: sub_625DD0+397j

      .text:00626189                 test    eax, eax

      .text:0062618B                 jnz     short loc_626192

      .text:0062618D                 mov     eax, offset byte_834463

      .text:00626192

      .text:00626192 loc_626192:                             ; CODE XREF: sub_625DD0+3BBj

      .text:00626192                 push    eax

      .text:00626193                 mov     esi, 8

      .text:00626198                 call    sub_625030

      .text:0062619D                 lea     edx, [ebp+FileName]

      .text:006261A3                 push    eax

      .text:006261A4                 sub     edx, ebx

      .text:006261A6                 push    edx

      .text:006261A7

      .text:006261A7 loc_6261A7:                             ; CODE XREF: sub_625DD0+3B7j

      .text:006261A7                 push    ebx

      .text:006261A8                 call    sub_61B710

      .text:006261AD                 add     esp, 10h

      .text:006261B0                 push    ebx

      .text:006261B1                 call    sub_61B5C0

      .text:006261B6                 add     ebx, eax

      .text:006261B8                 cmp     dword_CBF9E0, 0

      .text:006261BF                 jnz     loc_6262D1

      .text:006261C5                 call    sub_632850

      .text:006261CA                 push    offset stru_CBF6C4 ; lpCriticalSection

      .text:006261CF                 call    sub_625140

      .text:006261D4                 mov     esi, dword_CBF6C0

      .text:006261DA                 add     esp, 4

      .text:006261DD                 mov     eax, offset stru_CBF6C4

      .text:006261E2                 mov     [ebp+var_D50], esi

      .text:006261E8                 mov     dword_CBF6C0, 0

      .text:006261F2                 call    sub_625190

      .text:006261F7                 test    esi, esi

      .text:006261F9                 jz      short loc_626200

      .text:006261FB                 mov     esi, [esi+4]

      .text:006261FE                 jmp     short loc_626202

      .text:00626200 ; ---------------------------------------------------------------------------

      .text:00626200

      .text:00626200 loc_626200:                             ; CODE XREF: sub_625DD0+429j

      .text:00626200                 xor     esi, esi

      .text:00626202

      .text:00626202 loc_626202:                             ; CODE XREF: sub_625DD0+42Ej

      .text:00626202                 lea     eax, [ebp+SystemTime]

      .text:00626208                 push    eax             ; lpSystemTime

      .text:00626209                 call    ds:GetLocalTime

      .text:0062620F                 test    esi, esi

      .text:00626211                 mov     [ebp+var_D54], offset aCrash ; "Crash"

      .text:0062621B                 jnz     short loc_626227

      .text:0062621D                 mov     [ebp+var_D54], offset aError_1 ; "Error"

      .text:00626227

      .text:00626227 loc_626227:                             ; CODE XREF: sub_625DD0+44Bj

      .text:00626227                 lea     ecx, [ebp+SystemTime]

      .text:0062622D                 push    ecx

      .text:0062622E                 mov     ecx, [ebp+var_D54]

      .text:00626234                 lea     edx, [ebp+Text]

      .text:0062623A                 call    sub_6254B0

      .text:0062623F                 mov     edi, eax

      .text:00626241                 add     esp, 4

      .text:00626244                 cmp     edi, 0FFFFFFFFh

      .text:00626247                 jz      short loc_626265

      .text:00626249                 mov     ecx, esi

      .text:0062624B                 neg     ecx

      .text:0062624D                 sbb     ecx, ecx

      .text:0062624F                 and     ecx, 0FFFFFFFEh

      .text:00626252                 add     ecx, 2

      .text:00626255                 mov     eax, esi

      .text:00626257                 mov     edx, edi

      .text:00626259                 call    sub_6255A0

      .text:0062625E                 mov     eax, edi

      .text:00626260                 call    sub_6255C0

      .text:00626265

      .text:00626265 loc_626265:                             ; CODE XREF: sub_625DD0+477j

      .text:00626265                 call    sub_632CA0

      .text:0062626A                 test    eax, eax

      .text:0062626C                 jz      short loc_6262CC

      .text:0062626E                 cmp     dword_CBF89C, 0

      .text:00626275                 jnz     short loc_6262CC

      .text:00626277                 mov     ecx, [ebp+var_D54]

      .text:0062627D                 push    104h

      .text:00626282                 push    offset off_86FB2C

      .text:00626287                 lea     edi, [ebp+SystemTime]

      .text:0062628D                 lea     edx, [ebp+FindFileData.cFileName+10h]

      .text:00626293                 call    sub_6253A0

      .text:00626298                 mov     edi, eax

      .text:0062629A                 add     esp, 8

      .text:0062629D                 cmp     edi, 0FFFFFFFFh

      .text:006262A0                 jz      short loc_6262CC

      .text:006262A2                 mov     eax, [ebp+var_D50]

      .text:006262A8                 lea     edx, [ebp+var_D5C]

      .text:006262AE                 push    edx

      .text:006262AF                 push    1

      .text:006262B1                 push    eax

      .text:006262B2                 push    edi

      .text:006262B3                 mov     [ebp+var_D5C], offset unk_CBF718

      .text:006262BD                 call    sub_632C00

      .text:006262C2                 add     esp, 10h

      .text:006262C5                 mov     eax, edi

      .text:006262C7                 call    sub_6255C0

      .text:006262CC

      .text:006262CC loc_6262CC:                             ; CODE XREF: sub_625DD0+49Cj

      .text:006262CC                                         ; sub_625DD0+4A5j ...

      .text:006262CC                 call    sub_632860

      .text:006262D1

      .text:006262D1 loc_6262D1:                             ; CODE XREF: sub_625DD0+3EFj

      .text:006262D1                 cmp     dword_CBF9F4, 0

      .text:006262D8                 jz      short loc_6262FC

      .text:006262DA                 pop     ebx

      .text:006262DB                 pop     esi

      .text:006262DC                 mov     dword_CBF6DC, 0

      .text:006262E6                 mov     eax, 1

      .text:006262EB                 pop     edi

      .text:006262EC                 mov     ecx, [ebp+var_4]

      .text:006262EF                 xor     ecx, ebp

      .text:006262F1                 call    sub_4096EA

      .text:006262F6                 mov     esp, ebp

      .text:006262F8                 pop     ebp

      .text:006262F9                 retn    18h

      .text:006262FC ; ---------------------------------------------------------------------------

      .text:006262FC

      .text:006262FC loc_6262FC:                             ; CODE XREF: sub_625DD0+508j

      .text:006262FC                 call    sub_6251D0

      .text:00626301                 test    eax, eax

      .text:00626303                 jz      short loc_626383

      .text:00626305                 cmp     [ebp+arg_10], 0

      .text:00626309                 jz      short loc_626349

      .text:0062630B                 lea     ecx, [ebp+FileName]

      .text:00626311                 sub     ecx, ebx

      .text:00626313                 push    ecx

      .text:00626314                 mov     esi, 0Dh

      .text:00626319                 call    sub_625030

      .text:0062631E                 push    eax

      .text:0062631F                 push    ebx

      .text:00626320                 call    sub_61B540

      .text:00626325                 lea     ebx, [esi+23h]

      .text:00626328                 mov     esi, 103h

      .text:0062632D                 lea     edi, [ebx-29h]

      .text:00626330                 mov     [ebp+var_D50], 2

      .text:0062633A                 mov     [ebp+var_D54], 6

      .text:00626344                 jmp     loc_6263F6

      .text:00626349 ; ---------------------------------------------------------------------------

      .text:00626349

      .text:00626349 loc_626349:                             ; CODE XREF: sub_625DD0+539j

      .text:00626349                 lea     edx, [ebp+FileName]

      .text:0062634F                 sub     edx, ebx

      .text:00626351                 push    edx

      .text:00626352                 mov     esi, 0Ch

      .text:00626357                 call    sub_625030

      .text:0062635C                 push    eax

      .text:0062635D                 push    ebx

      .text:0062635E                 call    sub_61B540

      .text:00626363                 mov     esi, 104h

      .text:00626368                 mov     edi, 7

      .text:0062636D                 mov     [ebp+var_D50], 0

      .text:00626377                 mov     [ebp+var_D54], 6

      .text:00626381                 jmp     short loc_6263F1

      .text:00626383 ; ---------------------------------------------------------------------------

      .text:00626383

      .text:00626383 loc_626383:                             ; CODE XREF: sub_625DD0+533j

      .text:00626383                 cmp     [ebp+arg_10], 0

      .text:00626387                 jz      short loc_6263C4

      .text:00626389                 lea     eax, [ebp+FileName]

      .text:0062638F                 sub     eax, ebx

      .text:00626391                 push    eax

      .text:00626392                 mov     esi, 0Ah

      .text:00626397                 call    sub_625030

      .text:0062639C                 push    eax

      .text:0062639D                 push    ebx

      .text:0062639E                 call    sub_61B540

      .text:006263A3                 lea     ebx, [esi+26h]

      .text:006263A6                 mov     esi, 101h

      .text:006263AB                 lea     edi, [ebx-2Eh]

      .text:006263AE                 mov     [ebp+var_D50], 1

      .text:006263B8                 mov     [ebp+var_D54], 0

      .text:006263C2                 jmp     short loc_6263F6

      .text:006263C4 ; ---------------------------------------------------------------------------

      .text:006263C4

      .text:006263C4 loc_6263C4:                             ; CODE XREF: sub_625DD0+5B7j

      .text:006263C4                 lea     ecx, [ebp+FileName]

      .text:006263CA                 sub     ecx, ebx

      .text:006263CC                 push    ecx

      .text:006263CD                 mov     esi, 9

      .text:006263D2                 call    sub_625030

      .text:006263D7                 push    eax

      .text:006263D8                 push    ebx

      .text:006263D9                 call    sub_61B540

      .text:006263DE                 xor     esi, esi

      .text:006263E0                 xor     eax, eax

      .text:006263E2                 lea     edi, [esi+1]

      .text:006263E5                 mov     [ebp+var_D50], eax

      .text:006263EB                 mov     [ebp+var_D54], eax

      .text:006263F1

      .text:006263F1 loc_6263F1:                             ; CODE XREF: sub_625DD0+5B1j

      .text:006263F1                 mov     ebx, 10h

      .text:006263F6

      .text:006263F6 loc_6263F6:                             ; CODE XREF: sub_625DD0+574j

      .text:006263F6                                         ; sub_625DD0+5F2j

      .text:006263F6                 push    offset stru_CBF6E0 ; lpCriticalSection

      .text:006263FB                 call    sub_625140

      .text:00626400                 or      esi, ebx

      .text:00626402                 or      esi, 52000h

      .text:00626408                 add     esp, 4

      .text:0062640B                 cmp     [ebp+var_D50], 0

      .text:00626412                 mov     [ebp+ExitCode], esi

      .text:00626418                 jz      short loc_626428

      .text:0062641A                 mov     edx, [ebp+var_D50]

      .text:00626420                 mov     [ebp+var_D5C], edx

      .text:00626426                 jmp     short loc_62642E

      .text:00626428 ; ---------------------------------------------------------------------------

      .text:00626428

      .text:00626428 loc_626428:                             ; CODE XREF: sub_625DD0+648j

      .text:00626428                 mov     [ebp+var_D5C], edi

      .text:0062642E

      .text:0062642E loc_62642E:                             ; CODE XREF: sub_625DD0+656j

      .text:0062642E                 mov     ebx, dword_B42E20

      .text:00626434                 test    bl, 1

      .text:00626437                 jnz     short loc_62643D

      .text:00626439                 test    ebx, ebx

      .text:0062643B                 jnz     short loc_62643F

      .text:0062643D

      .text:0062643D loc_62643D:                             ; CODE XREF: sub_625DD0+667j

      .text:0062643D                 xor     ebx, ebx

      .text:0062643F

      .text:0062643F loc_62643F:                             ; CODE XREF: sub_625DD0+66Bj

      .text:0062643F                 mov     edi, [ebp+var_D64]

      .text:00626445                 mov     esi, [ebp+arg_0]

      .text:00626448

      .text:00626448 loc_626448:                             ; CODE XREF: sub_625DD0+6AAj

      .text:00626448                 test    bl, 1

      .text:0062644B                 mov     eax, [ebp+var_D58]

      .text:00626451                 jnz     short loc_62647C

      .text:00626453                 test    ebx, ebx

      .text:00626455                 jz      short loc_62647C

      .text:00626457                 test    eax, eax

      .text:00626459                 jnz     short loc_626460

      .text:0062645B                 mov     eax, offset byte_834463

      .text:00626460

      .text:00626460 loc_626460:                             ; CODE XREF: sub_625DD0+689j

      .text:00626460                 mov     edx, [ebx+8]

      .text:00626463                 push    eax

      .text:00626464                 mov     eax, [ebp+arg_8]

      .text:00626467                 push    eax

      .text:00626468                 push    edi

      .text:00626469                 lea     ecx, [ebp+var_104]

      .text:0062646F                 push    ecx

      .text:00626470                 push    esi

      .text:00626471                 call    edx

      .text:00626473                 test    eax, eax

      .text:00626475                 jz      short loc_6264C1

      .text:00626477                 mov     ebx, [ebx+4]

      .text:0062647A                 jmp     short loc_626448

      .text:0062647C ; ---------------------------------------------------------------------------

      .text:0062647C

      .text:0062647C loc_62647C:                             ; CODE XREF: sub_625DD0+681j

      .text:0062647C                                         ; sub_625DD0+685j

      .text:0062647C                 call    sub_632CD0

      .text:00626481                 mov     edi, eax

      .text:00626483                 xor     ebx, ebx

      .text:00626485                 cmp     edi, ebx

      .text:00626487                 jz      short loc_6264A1

      .text:00626489                 push    edi             ; hWnd

      .text:0062648A                 call    ds:IsWindow

      .text:00626490                 test    eax, eax

      .text:00626492                 jz      short loc_62649F

      .text:00626494                 push    edi             ; hWnd

      .text:00626495                 call    ds:IsWindowVisible

      .text:0062649B                 test    eax, eax

      .text:0062649D                 jnz     short loc_6264A1

      .text:0062649F

      .text:0062649F loc_62649F:                             ; CODE XREF: sub_625DD0+6C2j

      .text:0062649F                 xor     edi, edi

      .text:006264A1

      .text:006264A1 loc_6264A1:                             ; CODE XREF: sub_625DD0+6B7j

      .text:006264A1                                         ; sub_625DD0+6CDj

      .text:006264A1                 mov     eax, [ebp+ExitCode]

      .text:006264A7                 push    eax             ; uType

      .text:006264A8                 lea     ecx, [ebp+Caption]

      .text:006264AE                 push    ecx             ; lpCaption

      .text:006264AF                 lea     edx, [ebp+Text]

      .text:006264B5                 push    edx             ; lpText

      .text:006264B6                 push    edi             ; hWnd

      .text:006264B7                 call    ds:MessageBoxA

      .text:006264BD                 mov     esi, eax

      .text:006264BF                 jmp     short loc_6264C9

      .text:006264C1 ; ---------------------------------------------------------------------------

      .text:006264C1

      .text:006264C1 loc_6264C1:                             ; CODE XREF: sub_625DD0+6A5j

      .text:006264C1                 mov     esi, [ebp+var_D5C]

      .text:006264C7                 xor     ebx, ebx

      .text:006264C9

      .text:006264C9 loc_6264C9:                             ; CODE XREF: sub_625DD0+6EFj

      .text:006264C9                 mov     eax, offset stru_CBF6E0

      .text:006264CE                 call    sub_625190

      .text:006264D3                 cmp     esi, [ebp+var_D54]

      .text:006264D9                 jnz     short loc_6264E7

      .text:006264DB                 call    sub_6250E0

      .text:006264E0

      .text:006264E0 loc_6264E0:                             ; CODE XREF: sub_625DD0+71Dj

      .text:006264E0                 mov     eax, 1

      .text:006264E5                 jmp     short loc_626535

      .text:006264E7 ; ---------------------------------------------------------------------------

      .text:006264E7

      .text:006264E7 loc_6264E7:                             ; CODE XREF: sub_625DD0+709j

      .text:006264E7                 cmp     esi, [ebp+var_D50]

      .text:006264ED                 jz      short loc_6264E0

      .text:006264EF                 push    1

      .text:006264F1                 call    sub_625B00

      .text:006264F6                 call    ds:GetVersion   ; Get current version number of Windows

      .text:006264F6                                         ; and information about the operating system platform

      .text:006264FC                 test    eax, eax

      .text:006264FE                 mov     edi, ds:GetCurrentProcess

      .text:00626504                 jns     short loc_626526

      .text:00626506                 lea     eax, [ebp+ExitCode]

      .text:0062650C                 push    eax             ; lpExitCode

      .text:0062650D                 call    edi ; GetCurrentProcess

      .text:0062650F                 push    eax             ; hProcess

      .text:00626510                 call    ds:GetExitCodeProcess

      .text:00626516                 test    eax, eax

      .text:00626518                 jz      short loc_626526

      .text:0062651A                 cmp     [ebp+ExitCode], 103h

      .text:00626524                 jnz     short loc_626533

      .text:00626526

      .text:00626526 loc_626526:                             ; CODE XREF: sub_625DD0+734j

      .text:00626526                                         ; sub_625DD0+748j

      .text:00626526                 mov     ecx, [ebp+uExitCode]

      .text:00626529                 push    ecx             ; uExitCode

      .text:0062652A                 call    edi ; GetCurrentProcess

      .text:0062652C                 push    eax             ; hProcess

      .text:0062652D                 call    ds:TerminateProcess

      .text:00626533

      .text:00626533 loc_626533:                             ; CODE XREF: sub_625DD0+754j

      .text:00626533                 xor     eax, eax

      .text:00626535

      .text:00626535 loc_626535:                             ; CODE XREF: sub_625DD0+715j

      .text:00626535                 mov     ecx, [ebp+var_4]

      .text:00626538                 mov     dword_CBF6DC, ebx

      .text:0062653E                 pop     ebx

      .text:0062653F                 pop     esi

      .text:00626540                 xor     ecx, ebp

      .text:00626542                 pop     edi

      .text:00626543

      .text:00626543 loc_626543:                             ; DATA XREF: .rdata:off_836AA4o

      .text:00626543                                         ; .data:00B43418o

      .text:00626543                 call    sub_4096EA

      .text:00626548                 mov     esp, ebp

      .text:0062654A                 pop     ebp

      .text:0062654B                 retn    18h

      .text:0062654B sub_625DD0      endp

      .text:0062654B

      .text:0062654B ; ---------------------------------------------------------------------------

**Subroutine 2:**
	  .text:00625B00 ; ¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦ S U B R O U T I N E ¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦

      .text:00625B00

      .text:00625B00 ; Attributes: bp-based frame

      .text:00625B00

      .text:00625B00 sub_625B00      proc near               ; CODE XREF: sub_402060+8Ap

      .text:00625B00                                         ; sub_621CC0+18p ...

      .text:00625B00

      .text:00625B00 arg_0           = dword ptr  8

      .text:00625B00

      .text:00625B00                 push    ebp

      .text:00625B01                 mov     ebp, esp

      .text:00625B03                 mov     eax, [ebp+arg_0]

      .text:00625B06                 mov     dword_CBF70C, eax

      .text:00625B0B                 pop     ebp

      .text:00625B0C                 retn    4

      .text:00625B0C sub_625B00      endp

      .text:00625B0C

      .text:00625B0C ; ---------------------------------------------------------------------------

Prior to patch v2.2 the second subroutine did not exist, only the first one. We can (hopefully) see from the first subroutine that it is used to compare a certain variable to 0, then act based on that.

    .text:00625DE3                 cmp     dword_CBF70C, 0

Before the 'nerf' we could simply set that variable to whatever we wanted other than 0, and the signature check would be disabled. Since patch v2.2 though another subroutine has been added to WoW.exe (the second one). We can see from the code that it takes a value off the stack and sets the aforementioned variable to the value it popped off the stack. When we trace back to where it was called from (in the original two subroutines), we can see that 0 is pushed onto the stack before the subroutine is called, this means that the variable is being reset to 0 every time that this subroutine is called.

Because of this we can no longer simply change the value of the variable, because it will simply be reset if we try, so we have to try a different approach. The method I devised was to change the value pushed onto the stack so the subroutine overwrote the variable with our own arbitrary number rather than the 0 as it was intended. This means that we change this:

    .text:00621CD6                 push    0

Into this:

    .text:00621CD6                 push    1

This effectively enables model editing again, but with a small hurdle, it does not pass the consistency check. Because we have modified the text segment of WoW.exe rather than the data segment as we previously did pre v2.2, WoW has picked up on how modification and will not let us log in. To get around this we must enable and disable the fix at the appropriate time so as to bypass this annoyance (which becomes an annoyance in itself).

There are a few ways we could theoretically get around this (thanks to Greyman for some great ideas about this):
Method 1: Hook events and enable/disable the patch at the right times. This would require a DLL to be injected into WoW which monitors events to see what WoW is doing so it can act based upon that, a major problem with this is that Warden looks for behavior like this and it would be difficult to code an undetected way, and once released public it would likely be patched for fear of the same method being used for more nefarious purposes such as botting/hacking/etc.

Method 2: Disable the consistency check. This would be very easy to code most likely, but again, the problem would be warden. Warden would most likely be on the lookout for changes to the consistency routine.

Method 3: Change the consistency values. This way would mean we could change the file hashes so it would be a data only mod, that way Warden would most likely not be on the lookout. The problem with this is that the user would have to input the hashes themselves to be written to memory, which would be an even bigger pain than simply enabling/disabling the fix manually.

Well, if there's anything you don't understand feel free to ask. Also, I'm open to suggestions as to how to improve my method. Posted below is the source code of my model edit fix.


      #include <Windows.h>

      #include <cstdio>


      int main(int argc, const char* argv[]) {

              DWORD dwOffset = 0x00621CD7;

              BYTE bEnable = 0x01;

              BYTE bDisable = 0x00;

              bool Enabled = true;


              STARTUPINFO sInfo;

              PROCESS_INFORMATION pInfo;


              ZeroMemory(&sInfo, sizeof(sInfo));

              sInfo.cb = sizeof(sInfo);

              ZeroMemory(&pInfo, sizeof(pInfo));


              if (CreateProcess(NULL, "WoW.exe", NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE, NULL, NULL, &sInfo, &pInfo)) {

                      if (WriteProcessMemory(pInfo.hProcess, (LPVOID)dwOffset, &bEnable, sizeof(bEnable), NULL)){

                              puts("Press END on your keyboard to disable the fix when prompted and HOME to enable it.\r");

                              puts("Once WoW has finished loading fully please press END then log in.\r");

                              puts("Once logged in (before or after entering the actual realm) press HOME to renable the fix and all model edits.\r");

                              puts("Hopfeully this inconvenience is only temporary, a 'better' fix is being worked on right now.\r");

                              while (true) {

                                      if (Enabled) {

                                              while (true) {

                                              if (GetAsyncKeyState(VK_END)&1) {

                                                      WriteProcessMemory(pInfo.hProcess, (LPVOID)dwOffset, &bDisable, sizeof(bDisable), NULL);

                                                      puts("Patch disabled");

                                                      Enabled = false;

                                                      break;

                                              }

                                              Sleep(1);

                                      }

                                      } else {

                                              while (true) {

                                              if (GetAsyncKeyState(VK_HOME)&1) {

                                                      WriteProcessMemory(pInfo.hProcess, (LPVOID)dwOffset, &bEnable, sizeof(bEnable), NULL);

                                                      puts("Patch enabled");

                                                      Enabled = true;

                                                      break;

                                              }

                                              Sleep(1);

                                              }

                                      }

                                      Sleep(1);

                              }

                      }

              }

              CloseHandle(pInfo.hProcess);

              CloseHandle(pInfo.hThread);

      }

## Simple 2.3 Fix
    00640D40  /$ 8D87 50010000  LEA EAX,DWORD PTR DS:[EDI+150]
    00640D46  |. 50             PUSH EAX                                 ; /Arg1
    00640D47  |. 8D8F FC010000  LEA ECX,DWORD PTR DS:[EDI+1FC]           ; |
    00640D4D  |. E8 7E320000    CALL WoW.00643FD0                        ; \Call before error check, returns 1 if ok, 0 if fail.
    00640D52  |. 84C0           TEST AL,AL
    00640D54  |. 74 16          JE SHORT WoW.00640D6C                    ;  Jump over the error if ok, otherwise continue.
    00640D56  |. 6A 01          PUSH 1                                   ; /Arg6 = 00000001
    00640D58  |. 6A 00          PUSH 0                                   ; |Arg5 = 00000000
    00640D5A  |. 6A 00          PUSH 0                                   ; |Arg4 = 00000000
    00640D5C  |. 6A FC          PUSH -4                                  ; |Arg3 = FFFFFFFC
    00640D5E  |. 8D4F 24        LEA ECX,DWORD PTR DS:[EDI+24]            ; |
    00640D61  |. 51             PUSH ECX                                 ; |Arg2
    00640D62  |. 68 83001085    PUSH 85100083                            ; |Arg1 = 85100083
    00640D67  |. E8 04440000    CALL WoW.00645170                        ; \Call the error dialog
    00640D6C  |> 833D D44CD200 >CMP DWORD PTR DS:[D24CD4],2
    00640D73  |. 74 46          JE SHORT WoW.00640DBB
    00640D75  |. 8B87 30010000  MOV EAX,DWORD PTR DS:[EDI+130]
    00640D7B  |. 56             PUSH ESI
    00640D7C  |. 50             PUSH EAX                                 ; /Arg1
    00640D7D  |. E8 2EF6FFFF    CALL WoW.006403B0                        ; \WoW.006403B0



## Credits
Kynox