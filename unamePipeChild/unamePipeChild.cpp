#include "stdafx.h"
#include <windows.h> 
#include <conio.h>
#include <ctype.h>


#define BUFSIZE 1024 

void main(VOID)
{
	TCHAR chReadBuf[BUFSIZE] = { 0 };
	TCHAR chWriteBuf[BUFSIZE] = { 0 };

	DWORD dwRead, dwWritten;
	HANDLE hStdin, hStdout;
	BOOL fSuccess;

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	hStdin = GetStdHandle(STD_INPUT_HANDLE);
	if ((hStdout == INVALID_HANDLE_VALUE) ||
		(hStdin == INVALID_HANDLE_VALUE))
		ExitProcess(1);

	bool bquit = false;
	while (!bquit)
	{
		memset(chReadBuf, 0, BUFSIZ * sizeof(TCHAR));
		fSuccess = ReadFile(hStdin, chReadBuf, (BUFSIZE - 1) * sizeof(TCHAR), &dwRead, NULL);
		if (!fSuccess || dwRead == 0)
			break;

		memset(chWriteBuf, 0, BUFSIZ * sizeof(TCHAR));
		if (_tcsicmp(chReadBuf, _T("quit\n")) == 0)
		{
			bquit = true;
			_stprintf_s(chWriteBuf, BUFSIZE, _T("byte byte\n"));
		}
		else
			_stprintf_s(chWriteBuf, BUFSIZE, _T("[Reply from child] you just input %d character,content is: %s\n"), lstrlen(chReadBuf), chReadBuf);
		fSuccess = WriteFile(hStdout, chWriteBuf, lstrlen(chWriteBuf)*sizeof(TCHAR), &dwWritten, NULL);
		if (!fSuccess)
			break;
	}
	CloseHandle(hStdin);
	CloseHandle(hStdout);
}

