#ifndef CRB_SH_UTIL_H
#define CRB_SH_UTIL_H

extern bool Skipped,MadeDir;

extern bool RenameError;
extern bool RenameFunc(char * Name,char *ToName);

extern bool CopyError;
extern bool CopyFuncInit(void);
extern void CopyFuncFree(void);
extern bool ShowCopyMkDirError(char *Name);
extern bool ShowCopyError(char *Name,char *ToName);
extern bool CopyFuncGen(char * FmName,char *ToName,bool FmIsDir);
extern bool CopyFunc(char * Name,char *ToName);

extern bool DeleteError;
extern bool DeleteFunc(char * Name,bool IsDir);

extern bool HaveOverwriteAnswer,StoredOverwriteAnswer,OverwriteOnlyNew;
extern bool AskOverwrite(char * Name);
extern bool AskOverwrite2(char* Target,char*Source);

extern bool CheckAndChangeAccess(char * Name);

extern bool AskContinue(void);

// stuff for ts/tr : 
extern void ShowFoundMatch(char *BufPtr,char *BufBase,
						bool ShowLineNum,
						bool UltraQuiet, //= false,
						bool noPrefix,
						bool vcFormat,
						char * FileName); //= false)

#endif /* module */

