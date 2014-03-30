#include "stdafx.h"

// Code is based on Skywing's code, with slight tweaks. The license of this file is not precisely known at the moment.

#define ENCRYPT_START (NULL)
#define ENCRYPT_END (NULL)

typedef unsigned char STACKBYTES, *LPSTACKBYTES;
typedef unsigned char CODEBYTES, *LPCODEBYTES;

char szExceptionInformation[16 << 10];

void __cdecl DumphexToFile(void *Param, const char *pszFormat, ...)
{
	ENCRYPT_START;

	va_list vlArgs;
	char szTempBuffer[255], szBuffer[255], *p;
	int BufPos;
	DWORD dwBytesWritten;

	va_start(vlArgs, pszFormat);
	wvsprintf(szTempBuffer, pszFormat, vlArgs);
	va_end(vlArgs);

	for(p = szTempBuffer, BufPos = 0; *p; p++) {
		if(*p == '\n')
			szBuffer[BufPos++] = '\r';

		szBuffer[BufPos++] = *p;
	}

	szBuffer[BufPos] = '\0';

	WriteFile((HANDLE)Param, szBuffer, strlen(szBuffer), &dwBytesWritten, 0);

	ENCRYPT_END;
}

#pragma warning(disable:4035) // no return value
LPVOID __declspec(naked) __stdcall GetCallerAddress(void)
{
	__asm {
		pop eax
		jmp eax
	}
}
#pragma warning(default:4035) // no return value

int __cdecl wsprintfcat(LPSTR lpszBuffer, LPCSTR lpszFormat, ...)
{
	va_list vlArgs;
	int iResult;

	va_start(vlArgs, lpszFormat);
	iResult = wvsprintf(lpszBuffer + strlen(lpszBuffer), lpszFormat, vlArgs);
	va_end(vlArgs);

	return iResult;
}

HANDLE CreateReportLogFile(void)
{
	ENCRYPT_START;

	char szLogFileName[MAX_PATH];
	HANDLE hReportLog;

	/*GetModuleFileName(0, szLogFileName, sizeof(szLogFileName));

	LPSTR lpszPeriod = strrchr(szLogFileName, '.');

	if(lpszPeriod++)
		strcpy(lpszPeriod, "ERR");*/

	hReportLog = CreateFile("MPQDraft Fatal Exception Log.txt", GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, 0);

	SetFilePointer(hReportLog, 0, 0, FILE_END);

	ENCRYPT_END;

	return hReportLog;
}

bool GetLogicalAddress(LPVOID lpAddress, LPSTR lpszFileName, DWORD dwBufferSize,
					   LPDWORD lpdwSection, LPDWORD lpdwOffset)
{
	MEMORY_BASIC_INFORMATION mbi;
	PIMAGE_DOS_HEADER pDosHdr;
	PIMAGE_NT_HEADERS pNtHdr;
	PIMAGE_SECTION_HEADER pSection;
	DWORD dwRVA, dwSectionStart, dwSectionEnd, dwIdx;

	if(!lpszFileName || !dwBufferSize || !lpdwSection || !lpdwOffset)
		return false;

	if(!VirtualQuery(lpAddress, &mbi, sizeof(mbi)))
		return false;

	if(!GetModuleFileName((HMODULE)mbi.AllocationBase, lpszFileName, dwBufferSize))
		return false;

	pDosHdr = (PIMAGE_DOS_HEADER)mbi.AllocationBase;
	pNtHdr = (PIMAGE_NT_HEADERS)((DWORD)mbi.AllocationBase + pDosHdr->e_lfanew);
	pSection = IMAGE_FIRST_SECTION(pNtHdr);
	dwRVA = (DWORD)lpAddress - (DWORD)mbi.AllocationBase;

	for(dwIdx = 0; dwIdx < pNtHdr->FileHeader.NumberOfSections; dwIdx++, pSection++) {
		dwSectionStart = pSection->VirtualAddress;
		dwSectionEnd = dwSectionStart + max(pSection->SizeOfRawData, pSection->Misc.VirtualSize);

		if(dwRVA >= dwSectionStart && dwRVA <= dwSectionEnd) {
			*lpdwSection = dwIdx+1;
			*lpdwOffset = dwRVA - dwSectionStart;
			
			return true;
		}
	}

	return false;
}

void AppendStamp(LPSTR lpszBuffer)
{
	ENCRYPT_START;
	SYSTEMTIME st;
	GetLocalTime(&st);

	wsprintfcat(lpszBuffer, "%02u/%02u/%02u %02u:%02u:%02u.%03d",
		st.wMonth, st.wDay, st.wYear,
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
//		tm->tm_mon+1, tm->tm_mday, tm->tm_year+1900,
//		tm->tm_hour, tm->tm_min, tm->tm_sec, tm->tm_s);
	ENCRYPT_END;
}

void DumpBytes(HANDLE hReportLog, LPBYTE lpBytes, DWORD dwMaxPosition)
{
	ENCRYPT_START;
	DWORD dwPosition, dwBytesWritten;
	char szBuffer[16];

	for(dwPosition = 0;
		dwPosition < dwMaxPosition && !IsBadReadPtr(&lpBytes[dwPosition], 16);
		dwPosition += 16) {
		wsprintf(szBuffer, "0x%08x: ", &lpBytes[dwPosition]);
		WriteFile(hReportLog, szBuffer, strlen(szBuffer), &dwBytesWritten, 0);
		//CMemDumpHex((const char *)&lpBytes[dwPosition], 16, -1, DumphexToFile, (void *)hReportLog);
	}
	ENCRYPT_END;
}

void DumpStackBytes(HANDLE hReportLog, LPSTACKBYTES lpStack, DWORD dwMaxStackPosition)
{
	ENCRYPT_START;
	DWORD dwBytesWritten;
	static LPCSTR lpszStackBytesStart = "\r\nStack bytes:\r\n";

	WriteFile(hReportLog, lpszStackBytesStart, strlen(lpszStackBytesStart), &dwBytesWritten, 0);
	DumpBytes(hReportLog, (LPBYTE)lpStack, dwMaxStackPosition);
	ENCRYPT_END;
}

void DumpCodeBytes(HANDLE hReportLog, LPCODEBYTES lpCode, DWORD dwMaxCodePosition)
{
	ENCRYPT_START;
	DWORD dwBytesWritten;
	static LPCSTR lpszCodeBytesStart = "\r\nCode bytes:\r\n";

	WriteFile(hReportLog, lpszCodeBytesStart, strlen(lpszCodeBytesStart), &dwBytesWritten, 0);
	DumpBytes(hReportLog, (LPBYTE)lpCode, dwMaxCodePosition);
	ENCRYPT_END;
}

LPCSTR GetExceptionName(DWORD dwExceptionCode)
{
	switch(dwExceptionCode) {

	case EXCEPTION_ACCESS_VIOLATION:
		return "EXCEPTION_ACCESS_VIOLATION";

	case EXCEPTION_DATATYPE_MISALIGNMENT:
		return "EXCEPTION_DATATYPE_MISALIGNMENT";

	case EXCEPTION_BREAKPOINT:
		return "EXCEPTION_BREAKPOINT";

	case EXCEPTION_SINGLE_STEP:
		return "EXCEPTION_SINGLE_STEP";

	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
		return "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";

	case EXCEPTION_FLT_DENORMAL_OPERAND:
		return "EXCEPTION_FLT_DENORMAL_OPERAND";

	case EXCEPTION_FLT_DIVIDE_BY_ZERO:
		return "EXCEPTION_FLT_DIVIDE_BY_ZERO";

	case EXCEPTION_FLT_INEXACT_RESULT:
		return "EXCEPTION_FLT_INEXACT_RESULT";

	case EXCEPTION_FLT_INVALID_OPERATION:
		return "EXCEPTION_FLT_INVALID_OPERATION";

	case EXCEPTION_FLT_OVERFLOW:
		return "EXCEPTION_FLT_OVERFLOW";

	case EXCEPTION_FLT_STACK_CHECK:
		return "EXCEPTION_FLT_STACK_CHECK";

	case EXCEPTION_FLT_UNDERFLOW:
		return "EXCEPTION_FLT_UNDERFLOW";

	case EXCEPTION_INT_DIVIDE_BY_ZERO:
		return "EXCEPTION_INT_DIVIDE_BY_ZERO";

	case EXCEPTION_INT_OVERFLOW:
		return "EXCEPTION_INT_OVERFLOW";

	case EXCEPTION_PRIV_INSTRUCTION:
		return "EXCEPTION_PRIV_INSTRUCTION";

	case EXCEPTION_IN_PAGE_ERROR:
		return "EXCEPTION_IN_PAGE_ERROR";

	case EXCEPTION_ILLEGAL_INSTRUCTION:
		return "EXCEPTION_ILLEGAL_INSTRUCTION";

	case EXCEPTION_NONCONTINUABLE_EXCEPTION:
		return "EXCEPTION_NONCONTINUABLE_EXCEPTION";

	case EXCEPTION_STACK_OVERFLOW:
		return "EXCEPTION_STACK_OVERFLOW";

	case EXCEPTION_INVALID_DISPOSITION:
		return "EXCEPTION_INVALID_DISPOSITION";

	case EXCEPTION_GUARD_PAGE:
		return "EXCEPTION_GUARD_PAGE";

	case EXCEPTION_INVALID_HANDLE:
		return "EXCEPTION_INVALID_HANDLE";

	default:
		return "<unknown>";
		
	}

	__assume(0);
}

int HandleException(DWORD dwExceptionCode, LPEXCEPTION_POINTERS lpExceptionPointers,
					LPVOID lpCallerAddress, bool bFatal)
{
/*	static critical_section ctExcept;
	autolock lock(ctExcept);*/

	ENCRYPT_START;

	char szModule[MAX_PATH];
	DWORD dwSection, dwOffset, dwContext, dwBytesWritten;
	LPDWORD lpdwFrame, lpdwPrevFrame;
	HANDLE hReportLog;

	GetLogicalAddress(lpExceptionPointers->ExceptionRecord->ExceptionAddress, szModule,
		sizeof(szModule), &dwSection, &dwOffset);

	wsprintf(szExceptionInformation,
		"------------------------------------------------------\r\n\r\nMPQDraft fatal exception"
		" report:\r\n");

	/*wsprintfcat(szExceptionInformation, "Version: %d.%02d\r\nTime: ", version >> 8,
		version & 0xff);*/

	AppendStamp(szExceptionInformation);

	strcat(szExceptionInformation, "\r\n\r\n");

	wsprintfcat(szExceptionInformation,	"Exception code:  %08x %s\r\nFault address:   %08x"
		" %02x:%08x %s\r\n", dwExceptionCode, GetExceptionName(dwExceptionCode),
		lpExceptionPointers->ExceptionRecord->ExceptionAddress, dwSection, dwOffset, szModule);

	GetLogicalAddress(lpCallerAddress, szModule, sizeof(szModule), &dwSection, &dwOffset);

	wsprintfcat(szExceptionInformation, "Handler address: %08x %02x:%08x %s\r\n\r\n",
		lpCallerAddress, dwSection, dwOffset, szModule);

	wsprintfcat(szExceptionInformation,
		"Registers:\r\nEAX:%08x\r\nEBX:%08x\r\nECX:%08x\r\nEDX:%08x\r\nESI:%08x\r\nEDI:%08x\r\n",
		lpExceptionPointers->ContextRecord->Eax, lpExceptionPointers->ContextRecord->Ebx,
		lpExceptionPointers->ContextRecord->Ecx, lpExceptionPointers->ContextRecord->Edx,
		lpExceptionPointers->ContextRecord->Esi, lpExceptionPointers->ContextRecord->Edi);

	wsprintfcat(szExceptionInformation, "CS:EIP:%04x:%08x\r\nSS:ESP:%04x:%08x  EBP:%08x\r\n", 
		lpExceptionPointers->ContextRecord->SegCs, lpExceptionPointers->ContextRecord->Eip,
		lpExceptionPointers->ContextRecord->SegSs, lpExceptionPointers->ContextRecord->Esp,
		lpExceptionPointers->ContextRecord->Ebp);

	wsprintfcat(szExceptionInformation, "DS:%04x ES:%04x FS:%04x GS:%04x\r\nFlags:%08x\r\n",
		lpExceptionPointers->ContextRecord->SegDs, lpExceptionPointers->ContextRecord->SegEs,
		lpExceptionPointers->ContextRecord->SegFs, lpExceptionPointers->ContextRecord->SegGs,
		lpExceptionPointers->ContextRecord->EFlags);

	strcat(szExceptionInformation,
		"\r\nCall stack:\r\nAddress   Frame     Logical addr   Module\r\n");

	dwContext = lpExceptionPointers->ContextRecord->Eip;
	lpdwFrame = (LPDWORD)lpExceptionPointers->ContextRecord->Ebp;

	do { // Stack walk
		GetLogicalAddress((LPVOID)dwContext, szModule, sizeof(szModule), &dwSection, &dwOffset);

		wsprintfcat(szExceptionInformation, "%08x  %08x  %04x:%08x  %s\r\n", dwContext,
			lpdwFrame,
			dwSection, dwOffset, szModule);

		dwContext = lpdwFrame[1];
		lpdwPrevFrame = lpdwFrame;
		lpdwFrame = (LPDWORD)lpdwFrame[0];

		if((DWORD)lpdwFrame & 0x03)
			break;						// Require DWORD alignment!

		if(lpdwFrame <= lpdwPrevFrame)
			break;

		if(IsBadReadPtr(lpdwFrame, sizeof(DWORD)*2))
			break;
	} while(strlen(szExceptionInformation) < sizeof(szExceptionInformation) - (MAX_PATH + 35));

//	StackTrace(lpExceptionPointers->ContextRecord, szExceptionInformation + strlen(szExceptionInformation), 50, 0);

//	strcat(szExceptionInformation, "\r\n\r\n");

	hReportLog = CreateReportLogFile();

	if(hReportLog != INVALID_HANDLE_VALUE) {
		WriteFile(hReportLog, szExceptionInformation, strlen(szExceptionInformation),
			&dwBytesWritten, 0);

		DumpStackBytes(hReportLog, (LPSTACKBYTES)lpExceptionPointers->ContextRecord->Esp, 1024);
		DumpCodeBytes(hReportLog, (LPCODEBYTES)lpExceptionPointers->ContextRecord->Eip, 64);

		WriteFile(hReportLog, "\r\n\r\n\r\n\r\n", 4, &dwBytesWritten, 0);

		CloseHandle(hReportLog);
	}

	if(bFatal == true)
		MessageBox(HWND_DESKTOP, szExceptionInformation,
		"MPQDraft fatal exception report - log saved to disk", MB_ICONERROR);

	ENCRYPT_END;

	return EXCEPTION_EXECUTE_HANDLER;
}

//#include <imagehlp_351.h>
#if 0
//********************************************************************
void StackTrace(CONTEXT *pCtx,char *szBuffer,int MaxFrames,DWORD Flags)
{
  PSTR        pPath;
  CONTEXT     Ctx;
  STACKFRAME  Stk;
  DWORD       symDisplacement,dwInstance;
  HANDLE      hProcess = GetCurrentProcess();
  BYTE    symbolBuffer[ sizeof(IMAGEHLP_SYMBOL) + 512 ];

  // enter madness
  //if (!(Flags & STK_NOLOCK))
//    EnterCriticalSection(&Sec);
  //pTrace->Init();
  //Printf(pTrace,"Stack Trace\n");

  // initialise IMAGEHLP
//  pPath =  GetSearchPath();
  char wnddir[MAX_PATH], Path[MAX_PATH];
  GetWindowsDirectory(wnddir, sizeof(wnddir));
  SymInitialize(hProcess,wnddir,FALSE);
  //LocalFree(pPath);

  // init the argument contexts.
  memset(&Stk,0,sizeof(STACKFRAME));
  memset(&Ctx,0,sizeof(CONTEXT));	
  
  // funny - if I copy the struct wholesale I get crashes...
  Ctx.Esp = pCtx->Esp;
  Ctx.Ebp = pCtx->Ebp;
  Ctx.Eip = pCtx->Eip;

  Stk.AddrStack.Offset = pCtx->Esp;
  Stk.AddrStack.Mode   = AddrModeFlat;
  Stk.AddrFrame.Offset = pCtx->Ebp;
  Stk.AddrFrame.Mode   = AddrModeFlat;
  Stk.AddrPC.Offset    = pCtx->Eip;
  Stk.AddrPC.Mode      = AddrModeFlat;    

  // run the loop. Just that.
  for(int nFrames = 0;
      StackWalk(IMAGE_FILE_MACHINE_I386,hProcess,
		        NULL,&Stk,&Ctx,
   				ReadProcessMemory,SymFunctionTableAccess,
				SymGetModuleBase,NULL) && (nFrames < MaxFrames);nFrames++)
  {
    dwInstance = SymGetModuleBase(hProcess,Stk.AddrPC.Offset);    	
	// invalid address - no module covers it.
    if (!dwInstance)
	  continue;

    // get name of dll. No Module - no valid frame.
    if (!GetModuleFileName((HINSTANCE)dwInstance,Path,sizeof(Path)))
	  continue;	  

	// get the symbol name
    PIMAGEHLP_SYMBOL pSymbol = (PIMAGEHLP_SYMBOL)symbolBuffer;
    pSymbol->MaxNameLength = sizeof(symbolBuffer)-sizeof(PIMAGEHLP_SYMBOL);

    if (SymGetSymFromAddr(GetCurrentProcess(), 
					      Stk.AddrPC.Offset,
					     &symDisplacement,
					      pSymbol))        
//	  Printf(pTrace,"Function: %s\n",pSymbol->szName+1);
wsprintfcat(szBuffer,"Function: %s\r\n", pSymbol->szName+1);
	else 
	  //Printf(pTrace,"Function: <NOSYMBOL>\n");
  	  wsprintfcat(szBuffer, "Function: %08x\r\n", pSymbol->addr);

	// show the header line
	wsprintfcat(szBuffer,"File: %s\nRVA: %8.8X HINSTANCE: %8.8X BP: %8.8X\n",
           Path,Stk.AddrPC.Offset - dwInstance,
		   dwInstance,Stk.AddrFrame.Offset);


	nFrames ++;
	wsprintfcat(szBuffer,"\r\n");
  }
  SymCleanup(GetCurrentProcess());

  // Uff! leave madness
  wsprintf(szBuffer, "\r\n\r\n\r\n");
//  Printf(pTrace,"End Of Stack Trace\n\n\n");
  //pTrace->Close();
  //if (!(Flags & STK_NOLOCK))
//    LeaveCriticalSection(&Sec);
}
#endif