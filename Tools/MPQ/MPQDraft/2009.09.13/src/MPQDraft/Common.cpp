/*
	The contents of this file are subject to the Common Development and Distribution License Version 1.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.sun.com/cddl/cddl.html.

	Software distributed under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for the specific language governing rights and limitations under the License.

	The Initial Developer of the Original Code is Justin Olbrantz. The Original Code Copyright (C) 2008 Justin Olbrantz. All Rights Reserved.
*/

#include <shlwapi.h>
#include <assert.h>

#include "Common.h"

BOOL LocateGame(LPCSTR lpszGameKey, LPCSTR lpszGameValue)
{
	assert(lpszGameKey);
	assert(lpszGameValue);

	BOOL bRetVal = FALSE;
	HKEY hAppRegKey;
	DWORD dwValueType, dwValueSize = MAX_PATH + 1;
	char szRegPath[MAX_PATH + 1];

	// The exact registry key/value we need to find to locate the game may be in either HKEY_LOCAL_MACHINE or HKEY_CURRENT_USER, depending on game and version. So we need to check both.
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpszGameKey, 0, KEY_READ, &hAppRegKey) == ERROR_SUCCESS
		|| RegOpenKeyEx(HKEY_CURRENT_USER, lpszGameKey, 0, KEY_READ, &hAppRegKey) == ERROR_SUCCESS)
	{
		if (RegQueryValueEx(hAppRegKey, lpszGameValue, NULL, &dwValueType, (LPBYTE)&szRegPath, &dwValueSize) == ERROR_SUCCESS)
			bRetVal = TRUE;

		RegCloseKey(hAppRegKey);
	}

	return bRetVal;
}

BOOL LocateComponent(LPCSTR lpszGameKey, LPCSTR lpszGameValue, LPCSTR lpszFileName, LPSTR lpszFilePath)
{
	assert(lpszGameKey);
	assert(lpszGameValue);
	assert(lpszFileName);

	BOOL bRetVal = FALSE;
	HKEY hAppRegKey;
	DWORD dwValueType, dwValueSize = MAX_PATH + 1;
	char szRegPath[MAX_PATH + 1];

	// This works almost identically to LocateGame, but this time we get the actual path of the game component
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpszGameKey, 0, KEY_READ, &hAppRegKey) == ERROR_SUCCESS
		|| RegOpenKeyEx(HKEY_CURRENT_USER, lpszGameKey, 0, KEY_READ, &hAppRegKey) == ERROR_SUCCESS)
	{
		if (RegQueryValueEx(hAppRegKey, lpszGameValue, NULL, &dwValueType, (LPBYTE)&szRegPath, &dwValueSize) == ERROR_SUCCESS)
		{
			// Found the registry key. Append the file name to the directory.
			PathAddBackslash(szRegPath);
			strcat(szRegPath, lpszFileName);

			if (PathFileExists(szRegPath) && !PathIsDirectory(szRegPath))
				bRetVal = TRUE;
		}

		RegCloseKey(hAppRegKey);
	}

	if (lpszFilePath && bRetVal)
		strcpy(lpszFilePath, szRegPath);

	return bRetVal;
}