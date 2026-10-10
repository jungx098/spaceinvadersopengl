/* Real bodies for the Win32 calls the game makes during startup. */
#include "windows.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static DWORD copy_path(LPSTR dest, DWORD cap, const char* src)
{
	size_t n;
	if (!dest || cap == 0)
		return 0;
	n = strlen(src);
	if (n >= cap)
		n = cap - 1;
	memcpy(dest, src, n);
	dest[n] = 0;
	return (DWORD)n;
}

DWORD GetTempPath(DWORD nBufferLength, LPSTR lpBuffer)
{
	const char* tmp = getenv("TMPDIR");
	char path[MAX_PATH];
	if (!tmp || !tmp[0])
		tmp = "/tmp";
	snprintf(path, sizeof path, "%s/", tmp);
	return copy_path(lpBuffer, nBufferLength, path);
}

UINT GetSystemDirectory(LPSTR lpBuffer, UINT uSize)
{
	return (UINT)copy_path(lpBuffer, uSize, "/tmp");
}

DWORD GetCurrentDirectory(DWORD nBufferLength, LPSTR lpBuffer)
{
	if (!lpBuffer || nBufferLength == 0)
		return 0;
	if (!getcwd(lpBuffer, nBufferLength))
	{
		lpBuffer[0] = 0;
		return 0;
	}
	return (DWORD)strlen(lpBuffer);
}

BOOL CreateDirectory(LPCSTR lpPathName, void* unused)
{
	(void)unused;
	if (!lpPathName)
		return FALSE;
	if (mkdir(lpPathName, 0755) == 0 || errno == EEXIST)
		return TRUE;
	return FALSE;
}

float FSOUND_GetVersion(void)
{
	return 3.75f;
}
