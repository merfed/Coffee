
/*********
 * 
 * todo: 
 *	-nF	: explicit RenameByPat spec.	Thus, you could do:
 *
 *					spawnm -p:%1 -n:*.java copy $ \class\*.class
 *				and it would copy name.java to \class\name.class
 *				with %1 == name.java
 *
 * RenameByPat instead of $
 * 
*********
 * 
 * on a related note: -glob option should make Relative file names
 *	 (not absolute) using fileutil/MakeRelative(cwd,path)
 * this minimizes the huge-commandline problem
 * 
 ***********/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <crblib/inc.h>
#include <crblib/fileutil.h>
#include <crblib/strutil.h>

#include <crblib/walker.h>
#include <crblib/matchpat.h>

#include "spawnmutil.h"

void ParseCommandStr(char *commandWork,char *Name,char *Path);
bool spawnmCommand(char *command);
bool fileFunc(struct WalkInfo * WI);

bool doQuiet = false;
#define spawnmputs(str)		if ( doQuiet ) ; else errputs(str)
#define spawnmprintf	if ( doQuiet ) ; else printf

/* defaults */
pattern Pattern = NULL;
char commandStr[1024];
char * startPath = NULL,*globWork = NULL; // == NULL for don't use me
bool firstRun = true;

bool fileFunc(struct WalkInfo * WI)
{

	if ( MatchPatternNoCase(WI->Name,Pattern) )
	{
		if ( globWork && ! firstRun )
		{
			strcat(globWork," ");
			if ( startPath ) strcat(globWork,WI->FullName);
			else			 strcat(globWork,WI->Name);
		}
		else
		{
			char commandWork[1024];

			ParseCommandStr(commandWork,WI->Name,WI->Path);

			firstRun = false;

			if ( globWork )
			{
				strcpy(globWork,commandWork);
				return true;
			}
			else
			{
				return spawnmCommand(commandWork);
			}
		}
	}

return true;
}

void ParseCommandStr(char *commandWork,char *Name,char *Path)
{
char *commandWorkPtr=commandWork,*commandStrPtr=commandStr;

	firstRun = false;

	while(*commandStrPtr)
	{
		if ( *commandStrPtr == '@' && commandStrPtr[1] == 'f' )
		{
			strcpy(commandWorkPtr,Name);
			while(*commandWorkPtr) commandWorkPtr++;
			commandStrPtr += 2;
		}
		else if ( *commandStrPtr == '@' && commandStrPtr[1] == 'p' )
		{
			strcpy(commandWorkPtr,Path);
			while(*commandWorkPtr) commandWorkPtr++;
			commandStrPtr += 2;
		}
		else if ( *commandStrPtr == ' ' && commandStrPtr[1] == '@' && commandStrPtr[2] == '+' )
		{
			commandStrPtr++;
		}
		else if ( commandStrPtr[0] == '@' && commandStrPtr[1] == '+' )
		{
			commandStrPtr+=2;
			while (*commandStrPtr == ' ') commandStrPtr++;
		}
		else if ( *commandStrPtr == '@' && commandStrPtr[1] == 'q' )
		{
			*commandWorkPtr++ = '"';
			commandStrPtr += 2;
		}
		else if ( *commandStrPtr == '@' && commandStrPtr[1] == '@' )
		{
			*commandWorkPtr++ = '@';
			commandStrPtr += 2;
		}
		else if ( *commandStrPtr == '@'  )
		{
			puts("illegal escape!");
			commandStrPtr ++;
		}
		else
		{
			*commandWorkPtr++ = *commandStrPtr++;
		}
	}
	*commandWorkPtr = 0;
}

bool doEchoFlag = false;
bool ignoreAllErrors = false;

bool spawnmCommand(char *command)
{
bool ret;

	if ( doEchoFlag )
		spawnmputs(command);

	if ( startPath )
	{
		char CurPath[1024];
		if ( !getcwd(CurPath,1024) ) return 0;
		chdir(startPath);

		ret = sub_system(command);

		chdir(CurPath);
	}
	else
	{
		ret = sub_system(command);
	}

	if ( ret == -1 )
	{
		perror("system failed");
		return(0);
	}
	else if ( ret != 0 && !ignoreAllErrors )
	{
		int c;

		// <> never happens !?
		// we can't get the error returns status of command ?

		spawnmprintf(">> %s << failed!\n",command);
		spawnmprintf("Quit(Y) or keep going(N), or ignore all errors (A) ?"); fflush(stderr);
		c = getc(stdin);
		if ( c != '\n' ) getc(stdin);
		if ( toupper(c) == 'A' ) { ignoreAllErrors = true; return true; }
		if ( toupper(c) == 'N' ) return true;
		return(0);
	}

	return true;
}

int spawnm_main(int argc,char *argv[])
{
	int i;
	bool recurseFlag,doDirsFlag,RunLocalFlag,doGlobFlag;
	char * patArg = NULL;
	bool gotCommand = 0;
	int firstCommandArg = -1;

	doDirsFlag = recurseFlag = RunLocalFlag = doGlobFlag = 0;

	if ( argc < 2 || argv[1][0] == '?' || ( argv[1][0] == '-' && argv[1][1] == '?' ) 
			|| ( argv[1][0] == '-' && tolower(argv[1][1]) == 'h' ) )
	{
		errputs("spawnm (ChukSH) v1.1 by Charles Bloom, copyright (c) 1996-8");
		errputs("USAGE: SpawnM [spawnm flags] <taskname> <taskparam1> [taskparam2...]");
		errputs("SpawnM will launch <taskname> for all files in a directory.");
		errputs("SpawnM flags are:");
		errputs("	-r	recurse into subdirectories from current path");
		errputs("	-d	don't exclude directories");
		errputs("	-e	echo all commands before running them");
		errputs("	-l	run locally (in target) [def: in source]");
		errputs("	-pS explicitly specify pattern spec S ");	
		errputs("	-g	glob : run <taskname> only once, replace wilds (or $) with");
		errputs("			 list of all matches");
		errputs("	-i	ignoreAll errors");
		errputs("	-q	be very quiet (should be first param for true quietude)");
		errputs("");
		errputs("The first pattern-match characters in <params> will be used");
		
	#if 0
		errputs("subsequent pattern-match characters will be properly renamed"); 
	#endif
		errputs("");
		errputs("All @p characters will be replaced by the path");
		errputs("All @f characters will be replaced by the file name");
		errputs("All @+ characters eat the spaces near them");
		errputs(" e.g. spawnm @p@+ ram:*");
		errputs("note: the combination @p@f should be used to get the absolute file spec");
		errputs("");
		errputs("use @q (apostraphes) around <taskparam> to pass quotes to <taskname>");
		errputs("use quotes around redirection and pipes to pass them to <taskname>");
		errputs("");
	#if 0
		errputs(" proper renaming example:");
		errputs("in a dir with files 'ab','ac','bc' ");
		errputs("	spawnm echo > a* *");
		errputs("	 > ab b");
		errputs("	 > ac c");
		errputs("	spawnm echo > *c *b");
		errputs("	 > ac ab");
		errputs("	 > bc bb");
	#endif

		sub_help();

		exit(0);
	}

	for(i=1;i<argc;i++)
	{
		if ( !gotCommand )
		{
			if ( argv[i][0] == '-' )
			{
				char * ArgStr;
				ArgStr = &argv[i][2];
				if ( *ArgStr == ':' || *ArgStr == '=' ) ArgStr++;
		
				switch(toupper(argv[i][1]))
				{
					case 'Q':
						doQuiet = true;
						break;
					case 'D':
						doDirsFlag = true;
						spawnmputs("spawnm got option: Not excluding dirs");
						break;
					case 'E':
						doEchoFlag = true;
						spawnmputs("spawnm got option: Do Echo");
						break;
					case 'I':
						ignoreAllErrors = true;
						spawnmputs("spawnm got option: ignore all errors");
						break;
					case 'G':
						doGlobFlag = true;
						spawnmputs("spawnm got option: Globbing");
						break;
					case 'P': 
						patArg = ArgStr;
						spawnmputs("spawnm got option: explicit pattern");
						break;
					case 'R': 
						recurseFlag = true;
						spawnmputs("spawnm got option: Recursing into directories");
						break;
		
					case 'L': 
						RunLocalFlag = true;
						spawnmputs("spawnm got option: Running Apps Locally (in target)");
						break;

					default:
						spawnmprintf("spawnm : >>> unknown switch: %c , ignored\n",argv[i][1]);
						break;
				}
			}
			else
			{
				strcpy(commandStr,argv[i]);
				gotCommand = true;
			}
		}
		else
		{
			firstCommandArg = i;
			strcat(commandStr," ");
			if ( patArg == NULL && IsWild(argv[i]) )
			{
				patArg = argv[i];
				if ( RunLocalFlag )
					strcat(commandStr,"@f");
				else
					strcat(commandStr,"@p@f");
			}
			else
			{
				strcat(commandStr,argv[i]);
			}
		}
	}

	if ( firstCommandArg == -1 )
	{
		errexit("no args for command!");
	}

	if ( !patArg ) 
	{
		char commandwork[1024];
		char *filename = NULL;

//		system(commandStr);
		for(i=firstCommandArg;i<argc;i++)
		{
			if ( FileExists(argv[i]) )
				filename = argv[i];
		}
		if ( filename == NULL )
		{
			// try to guess something
			filename = argv[firstCommandArg];
		}

		ParseCommandStr(commandwork,
			FilePart(filename),
			PathPart(filename));
		spawnmCommand(commandwork);
	}
	else 
	{
		if ( ! RunLocalFlag )
		{
			if ( (startPath = malloc(1024)) == NULL ) return 10;
			if ( !getcwd(startPath,1024) ) return 10;
		}
		if ( doGlobFlag )
		{
			if ( (globWork = malloc(4096)) == NULL ) return 10;
			*globWork = 0;
		}

		if ( (Pattern = MakePatternNoCase(FilePart(patArg))) == NULL ) 
		{
			spawnmputs("spawnm : MakePat error");
		}
		else 
		{
			if ( ! WalkDir(PathPart(patArg),recurseFlag,doDirsFlag,
					fileFunc,NULL,NULL) ) 
			{
				spawnmputs("spawnm :  >>>>>>WalkDir error!<<<<<<<");
			}
			FreePattern(Pattern);
		}

		if ( globWork )
		{
			spawnmCommand(globWork);
			smartfree(globWork);
		}
		smartfree(startPath);
	}

return 0;
}


// use like :
//	char * argv[32] = { 0 };
//	int argc = 0;
void parse_argcargv(char * command, int * pArgc, char * argv[], int max_argc)
{
	char * p;
	int argc = 0;

	p = command;
	while(*p)
	{
		char work[1024];
		// fill out work
		char *pwork = work;

		if ( *p == '"' )
		{
			p++;
			while(*p && *p != '"')
			{
				*pwork++ = *p++;
			}
			if ( *p == '"' )
				p++;
			p = skipwhitespace(p);
		}
		else
		{
			while(*p && ! iswhitespace(*p) )
			{
				*pwork++ = *p++;
			}
			p = skipwhitespace(p);
		}
		*pwork = 0;

		if ( strlen(work) > 0 )
		{
			argv[argc] = malloc(strlen(work)+1);
			strcpy(argv[argc],work);
			argc++;
			if ( argc == max_argc )
				break;
		}
	}
	
	*pArgc = argc;
}

void free_argcargv(int argc,char * argv[])
{
	int i;
	for(i=0;i<argc;i++)
	{
		free( argv[i] );
	}
}

int system_argcargv(char * commandStr,char * exeName)
{
	char * argv[32] = { 0 };
	int argc = 0;

	parse_argcargv(commandStr,&argc,argv,32);

	// parse command to argc/argv , respect quotes
	
	if ( stricmp(argv[0],exeName) != 0 )
	{
		fprintf(stderr,"Expected command %s, got %s\n",exeName,argv[0]);
		errexit("command should be expected!");
	}

	sub_system_argcargv(argc,argv);

	free_argcargv(argc,argv);

	return 0;
}
