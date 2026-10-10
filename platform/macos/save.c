/* Hiscore and startup options, stored under Application Support. */
#include "windows.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define SAVE_VALUES 64
#define SAVE_NAME 64
#define SAVE_BYTES 1024

typedef struct SaveValue {
	char name[SAVE_NAME];
	DWORD type;
	unsigned size;
	unsigned char data[SAVE_BYTES];
} SaveValue;

static SaveValue g_values[SAVE_VALUES];
static int g_count = 0;
static int g_ready = 0;
static char g_path[1024];

static void save_path(void)
{
	const char* home = getenv("HOME");
	char dir[1024];
	if (!home || !home[0])
		home = "/tmp";
	snprintf(dir, sizeof dir, "%s/Library/Application Support/SpaceInvaders-OpenGL", home);
	mkdir(dir, 0755);
	snprintf(g_path, sizeof g_path, "%s/save.bin", dir);
}

static void load_file(void)
{
	FILE* file;
	unsigned count = 0;
	unsigned i;
	if (g_ready)
		return;
	g_ready = 1;
	save_path();
	file = fopen(g_path, "rb");
	if (!file)
		return;
	if (fread(&count, sizeof count, 1, file) != 1)
	{
		fclose(file);
		return;
	}
	if (count > SAVE_VALUES)
		count = SAVE_VALUES;
	for (i = 0; i < count; i++)
	{
		unsigned size = 0;
		if (fread(g_values[i].name, SAVE_NAME, 1, file) != 1)
			break;
		if (fread(&g_values[i].type, sizeof g_values[i].type, 1, file) != 1)
			break;
		if (fread(&size, sizeof size, 1, file) != 1)
			break;
		if (size > SAVE_BYTES)
			size = SAVE_BYTES;
		if (size && fread(g_values[i].data, size, 1, file) != 1)
			break;
		g_values[i].size = size;
		g_count++;
	}
	fclose(file);
}

static void write_file(void)
{
	FILE* file;
	unsigned count;
	int i;
	load_file();
	file = fopen(g_path, "wb");
	if (!file)
		return;
	count = (unsigned)g_count;
	fwrite(&count, sizeof count, 1, file);
	for (i = 0; i < g_count; i++)
	{
		fwrite(g_values[i].name, SAVE_NAME, 1, file);
		fwrite(&g_values[i].type, sizeof g_values[i].type, 1, file);
		fwrite(&g_values[i].size, sizeof g_values[i].size, 1, file);
		if (g_values[i].size)
			fwrite(g_values[i].data, g_values[i].size, 1, file);
	}
	fclose(file);
}

static SaveValue* find_value(const char* name, int create)
{
	int i;
	load_file();
	if (!name)
		return 0;
	for (i = 0; i < g_count; i++)
	{
		if (strcmp(g_values[i].name, name) == 0)
			return &g_values[i];
	}
	if (!create || g_count >= SAVE_VALUES)
		return 0;
	memset(&g_values[g_count], 0, sizeof g_values[g_count]);
	snprintf(g_values[g_count].name, SAVE_NAME, "%s", name);
	return &g_values[g_count++];
}

LONG RegOpenKeyEx(HKEY key, LPCSTR subkey, DWORD reserved, DWORD access, HKEY* out)
{
	(void)key;
	(void)subkey;
	(void)reserved;
	(void)access;
	load_file();
	if (out)
		*out = (HKEY)1;
	return ERROR_SUCCESS;
}

LONG RegCreateKeyEx(HKEY key, LPCSTR subkey, DWORD reserved, LPSTR class_name, DWORD options, DWORD access, void* security, HKEY* out, DWORD* disposition)
{
	(void)key;
	(void)subkey;
	(void)reserved;
	(void)class_name;
	(void)options;
	(void)access;
	(void)security;
	load_file();
	if (out)
		*out = (HKEY)1;
	if (disposition)
		*disposition = 1;
	return ERROR_SUCCESS;
}

LONG RegCloseKey(HKEY key)
{
	(void)key;
	write_file();
	return ERROR_SUCCESS;
}

LONG RegQueryValueEx(HKEY key, LPCSTR name, DWORD* reserved, DWORD* type, LPBYTE data, DWORD* size)
{
	SaveValue* value;
	unsigned copy;
	(void)key;
	(void)reserved;
	value = find_value(name, 0);
	if (!value || !size)
		return 1;
	if (type)
		*type = value->type;
	copy = value->size;
	if (copy > *size)
		copy = *size;
	if (data && copy)
		memcpy(data, value->data, copy);
	*size = value->size;
	return ERROR_SUCCESS;
}

LONG RegSetValueEx(HKEY key, LPCSTR name, DWORD reserved, DWORD type, const BYTE* data, DWORD size)
{
	SaveValue* value;
	(void)key;
	(void)reserved;
	if (!data || size > SAVE_BYTES)
		return 1;
	value = find_value(name, 1);
	if (!value)
		return 1;
	value->type = type;
	value->size = size;
	memcpy(value->data, data, size);
	write_file();
	return ERROR_SUCCESS;
}
