/* Real bodies for the Win32 calls the game makes during startup. */
#include "windows.h"

#include <errno.h>
#include <time.h>
#include <pthread.h>
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

float FSOUND_GetVersion(void)
{
	return 3.75f;
}

char* _itoa(int value, char* str, int radix)
{
	(void)radix;
	if (!str)
		return 0;
	sprintf(str, "%d", value);
	return str;
}

char* _ltoa(long value, char* str, int radix)
{
	(void)radix;
	if (!str)
		return 0;
	sprintf(str, "%ld", value);
	return str;
}

char* _strdup(const char* text)
{
	size_t n;
	char* copy;
	if (!text)
		return 0;
	n = strlen(text);
	copy = (char*)malloc(n + 1);
	if (!copy)
		return 0;
	memcpy(copy, text, n + 1);
	return copy;
}

char* _strlwr(char* text)
{
	char* cursor;
	if (!text)
		return 0;
	for (cursor = text; *cursor; cursor++)
	{
		if (*cursor >= 'A' && *cursor <= 'Z')
			*cursor = (char)(*cursor - 'A' + 'a');
	}
	return text;
}

char* _strupr(char* text)
{
	char* cursor;
	if (!text)
		return 0;
	for (cursor = text; *cursor; cursor++)
	{
		if (*cursor >= 'a' && *cursor <= 'z')
			*cursor = (char)(*cursor - 'a' + 'A');
	}
	return text;
}

int _stricmp(const char* left, const char* right)
{
	if (!left || !right)
		return left ? 1 : (right ? -1 : 0);
	return strcasecmp(left, right);
}

static void normalize_path(char* path)
{
	char* cursor;
	if (!path)
		return;
	for (cursor = path; *cursor; cursor++)
	{
		if (*cursor == '\\')
			*cursor = '/';
	}
}

#define MAC_FILE_MAGIC 0x4d414304u

typedef struct MacFile {
	unsigned magic;
	FILE* file;
	FILE* alias;
} MacFile;

HANDLE CreateFile(LPCSTR name, DWORD access, DWORD share, void* security, DWORD disposition, DWORD flags, HANDLE template_file)
{
	MacFile* handle;
	char path[MAX_PATH * 2];
	(void)access;
	(void)share;
	(void)security;
	(void)disposition;
	(void)flags;
	(void)template_file;
	if (!name)
		return INVALID_HANDLE_VALUE;
	snprintf(path, sizeof path, "%s", name);
	normalize_path(path);
	handle = (MacFile*)calloc(1, sizeof(MacFile));
	if (!handle)
		return INVALID_HANDLE_VALUE;
	handle->magic = MAC_FILE_MAGIC;
	handle->file = fopen(path, "wb+");
	if (!handle->file)
	{
		free(handle);
		return INVALID_HANDLE_VALUE;
	}
	/* The game later fopen()s the same string, and backslash is not a
	   directory separator here. Keep a second file at that exact name. */
	if (strcmp(name, path) != 0)
		handle->alias = fopen(name, "wb+");
	return (HANDLE)handle;
}

BOOL WriteFile(HANDLE file, LPCVOID buffer, DWORD bytes, DWORD* written, void* overlapped)
{
	MacFile* handle = (MacFile*)file;
	size_t count;
	(void)overlapped;
	if (written)
		*written = 0;
	if (!handle || handle->magic != MAC_FILE_MAGIC || !handle->file || !buffer)
		return FALSE;
	count = fwrite(buffer, 1, bytes, handle->file);
	if (handle->alias)
		fwrite(buffer, 1, bytes, handle->alias);
	if (written)
		*written = (DWORD)count;
	return count == bytes;
}

BOOL CloseHandle(HANDLE object)
{
	MacFile* handle = (MacFile*)object;
	if (!handle || handle->magic != MAC_FILE_MAGIC)
		return TRUE;
	if (handle->file)
		fclose(handle->file);
	if (handle->alias)
		fclose(handle->alias);
	free(handle);
	return TRUE;
}

BOOL DeleteFile(LPCSTR name)
{
	char path[MAX_PATH * 2];
	if (!name)
		return FALSE;
	snprintf(path, sizeof path, "%s", name);
	normalize_path(path);
	if (strcmp(name, path) != 0)
		unlink(name);
	return unlink(path) == 0;
}

int _vsntprintf(char* buffer, size_t count, const char* format, va_list args)
{
	if (!buffer || count == 0 || !format)
		return 0;
	return vsnprintf(buffer, count, format, args);
}

BOOL CreateDirectory(LPCSTR path, void* security)
{
	char normalized[MAX_PATH * 2];
	(void)security;
	if (!path)
		return FALSE;
	snprintf(normalized, sizeof normalized, "%s", path);
	normalize_path(normalized);
	if (mkdir(normalized, 0755) == 0 || errno == EEXIST)
		return TRUE;
	return FALSE;
}

HANDLE FindFirstFile(LPCSTR path, WIN32_FIND_DATA* data)
{
	if (data)
		data->cFileName[0] = 0;
	/* Pretend the bundled fmod.dll is already current so startup does not try to write it. */
	if (path && strstr(path, "fmod.dll"))
		return (HANDLE)(intptr_t)1;
	return INVALID_HANDLE_VALUE;
}

BOOL FindNextFile(HANDLE find, WIN32_FIND_DATA* data)
{
	(void)find;
	if (data)
		data->cFileName[0] = 0;
	return FALSE;
}

BOOL FindClose(HANDLE find)
{
	(void)find;
	return TRUE;
}

DWORD timeGetTime(void)
{
	static int started = 0;
	static struct timespec origin;
	struct timespec now;
	if (!started)
	{
		clock_gettime(CLOCK_MONOTONIC, &origin);
		started = 1;
	}
	clock_gettime(CLOCK_MONOTONIC, &now);
	return (DWORD)((now.tv_sec - origin.tv_sec) * 1000 + (now.tv_nsec - origin.tv_nsec) / 1000000);
}

DWORD GetTickCount(void)
{
	return timeGetTime();
}

BOOL QueryPerformanceFrequency(LARGE_INTEGER* frequency)
{
	if (!frequency)
		return FALSE;
	frequency->QuadPart = 1000000000LL;
	return TRUE;
}

BOOL QueryPerformanceCounter(LARGE_INTEGER* counter)
{
	struct timespec now;
	if (!counter)
		return FALSE;
	clock_gettime(CLOCK_MONOTONIC, &now);
	counter->QuadPart = (long long)now.tv_sec * 1000000000LL + now.tv_nsec;
	return TRUE;
}

void _splitpath(const char* path, char* drive, char* dir, char* fname, char* ext)
{
	const char* slash = 0;
	const char* dot = 0;
	const char* cursor;
	const char* file;
	if (drive)
		drive[0] = 0;
	if (!path)
		path = "";
	for (cursor = path; *cursor; cursor++)
	{
		if (*cursor == '/' || *cursor == '\\')
			slash = cursor;
		else if (*cursor == '.')
			dot = cursor;
	}
	if (dot && slash && dot < slash)
		dot = 0;
	file = slash ? slash + 1 : path;
	if (dir)
	{
		if (slash)
		{
			size_t n = (size_t)(slash - path) + 1;
			if (n > 255)
				n = 255;
			memcpy(dir, path, n);
			dir[n] = 0;
		}
		else
			dir[0] = 0;
	}
	if (fname)
	{
		const char* end = dot ? dot : cursor;
		size_t n = (size_t)(end - file);
		if (n > 255)
			n = 255;
		memcpy(fname, file, n);
		fname[n] = 0;
	}
	if (ext)
	{
		if (dot)
			snprintf(ext, 256, "%s", dot);
		else
			ext[0] = 0;
	}
}

DWORD GetLastError(void)
{
	return ERROR_NO_MORE_FILES;
}

typedef struct ThreadStart {
	void (*fn)(void*);
	void* arg;
} ThreadStart;

static void* thread_main(void* raw)
{
	ThreadStart* start = (ThreadStart*)raw;
	void (*fn)(void*) = start->fn;
	void* arg = start->arg;
	free(start);
	fn(arg);
	return 0;
}

unsigned long _beginthread(void (*fn)(void*), unsigned stack, void* arg)
{
	pthread_t thread;
	ThreadStart* start;
	(void)stack;
	if (!fn)
		return 0;
	start = (ThreadStart*)malloc(sizeof(ThreadStart));
	if (!start)
		return 0;
	start->fn = fn;
	start->arg = arg;
	if (pthread_create(&thread, 0, thread_main, start) != 0)
	{
		free(start);
		return 0;
	}
	pthread_detach(thread);
	return 1;
}
