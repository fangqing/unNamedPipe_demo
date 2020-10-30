#include "stdafx.h"
#include <windows.h> 
#include <tchar.h>
#include <stdio.h> 

#define BUFSIZE 1024 
		
HANDLE hChildR, hChildW;
HANDLE hHostR, hHostW;


BOOL CreateChildProcess(VOID);
VOID echoLoop(VOID);
VOID ErrorExit(LPSTR);

int _tmain(int argc, TCHAR *argv[])
{
	SECURITY_ATTRIBUTES saAttr;
	BOOL fSuccess;

	// Set the bInheritHandle flag so pipe handles are inherited. 

	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;


	if (!CreatePipe(&hChildR, &hHostW, &saAttr, 0))
		ErrorExit("pipe creation failed 1\n");


	SetHandleInformation(hChildR, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);


	if (!CreatePipe(&hHostR, &hChildW, &saAttr, 0))
		ErrorExit("pipe creation failed 2\n");


	SetHandleInformation(hChildW, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);


	fSuccess = CreateChildProcess();
	if (!fSuccess)
		ErrorExit("Create process failed with");


	echoLoop();

	return 0;
}

BOOL CreateChildProcess()
{
	TCHAR szCmdline[] = TEXT("unamePipeChild.exe");
	PROCESS_INFORMATION piProcInfo;
	STARTUPINFO siStartInfo;
	BOOL bFuncRetn = FALSE;

	// Set up members of the PROCESS_INFORMATION structure. 

	ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

	// Set up members of the STARTUPINFO structure. 

	ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
	siStartInfo.cb = sizeof(STARTUPINFO);
	siStartInfo.hStdError = hChildW;
	siStartInfo.hStdOutput = hChildW;
	siStartInfo.hStdInput = hChildR;
	siStartInfo.dwFlags |= STARTF_USESTDHANDLES;
	siStartInfo.wShowWindow = 

	// Create the child process. 

	bFuncRetn = CreateProcess(NULL,
		szCmdline,     // command line 
		NULL,          // process security attributes 
		NULL,          // primary thread security attributes 
		TRUE,          // handles are inherited 
		0,             // creation flags 
		NULL,          // use parent's environment 
		NULL,          // use parent's current directory 
		&siStartInfo,  // STARTUPINFO pointer 
		&piProcInfo);  // receives PROCESS_INFORMATION 

	if (bFuncRetn == 0)
		ErrorExit("CreateProcess failed\n");
	else
	{
		CloseHandle(piProcInfo.hProcess);
		CloseHandle(piProcInfo.hThread);
		return bFuncRetn;
	}

	return FALSE;
}

VOID echoLoop(VOID)
{
	DWORD dwRead, dwWritten;
	TCHAR chBuf[BUFSIZE] = { 0 };
	TCHAR szInput[BUFSIZE] = { 0 };

	for (;;)
	{
		memset(szInput, 0, BUFSIZ*sizeof(TCHAR));
		printf("input any character and press return:\n");
		_fgetts(szInput, BUFSIZE-1, stdin);
		if (!WriteFile(hHostW, szInput, lstrlen(szInput)*sizeof(TCHAR),&dwWritten, NULL))
			break;

		memset(chBuf, 0, BUFSIZ*sizeof(TCHAR));
		if (!ReadFile(hHostR, chBuf, (BUFSIZE-1)*sizeof(TCHAR), &dwRead, NULL) ||
			dwRead == 0) 
			break;
		fputws(chBuf, stdout);
	}
}


VOID ErrorExit(LPSTR lpszMessage)
{
	fprintf(stderr, "%s\n", lpszMessage);
	ExitProcess(0);
}
