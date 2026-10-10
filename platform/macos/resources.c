/* Disk-backed stand-in for the Windows resource loader. */
#include "windows.h"
#include "macos_port.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

typedef struct Asset {
	unsigned id;
	const char* kind;
	const char* path;
	unsigned char* data;
	unsigned size;
	int loaded;
} Asset;

static Asset g_assets[] = {
	{103, "ICON", "images/icon_app.ico"}, /* IDI_ICON_APP */
	{240, "TEXT", "Readme.txt"}, /* IDR_README_TXT */
	{306, "ICON", "images/icon_music.ico"}, /* IDI_ICON_MUSIC */
	{319, "TEXT", "Readme_es.txt"}, /* IDR_README_ES_TXT */
	{320, "TEXT", "Readme_fr.txt"}, /* IDR_README_FR_TXT */
	{107, "BITMAP", "images/game_startup.bmp"}, /* IDB_STARTUP */
	{112, "BITMAP", "images/barre.bmp"}, /* IDB_10 */
	{113, "BITMAP", "images/chiffre0.bmp"}, /* IDB_14 */
	{114, "BITMAP", "images/chiffre1.bmp"}, /* IDB_15 */
	{115, "BITMAP", "images/chiffre2.bmp"}, /* IDB_16 */
	{116, "BITMAP", "images/chiffre3.bmp"}, /* IDB_17 */
	{117, "BITMAP", "images/chiffre4.bmp"}, /* IDB_18 */
	{118, "BITMAP", "images/chiffre5.bmp"}, /* IDB_19 */
	{119, "BITMAP", "images/chiffre6.bmp"}, /* IDB_20 */
	{120, "BITMAP", "images/chiffre7.bmp"}, /* IDB_21 */
	{121, "BITMAP", "images/chiffre8.bmp"}, /* IDB_22 */
	{122, "BITMAP", "images/chiffre9.bmp"}, /* IDB_23 */
	{125, "BITMAP", "images/fond_skybox2.bmp"}, /* IDB_05 */
	{127, "BITMAP", "images/hiscore.bmp"}, /* IDB_25 */
	{128, "BITMAP", "images/ship3_b.bmp"}, /* IDB_13 */
	{130, "BITMAP", "images/tir.bmp"}, /* IDB_08 */
	{131, "BITMAP", "images/tir_ship.bmp"}, /* IDB_06 */
	{133, "BITMAP", "images/ship1.bmp"}, /* IDB_01 */
	{134, "BITMAP", "images/ship1_b.bmp"}, /* IDB_11 */
	{135, "BITMAP", "images/ship2.bmp"}, /* IDB_02 */
	{136, "BITMAP", "images/ship2_b.bmp"}, /* IDB_12 */
	{137, "BITMAP", "images/ship3.bmp"}, /* IDB_03 */
	{138, "BITMAP", "images/pixel_vert.bmp"}, /* IDB_27 */
	{139, "BITMAP", "images/score.bmp"}, /* IDB_24 */
	{140, "BITMAP", "images/lives.bmp"}, /* IDB_26 */
	{169, "BITMAP", "images/game_loading.bmp"}, /* IDB_LOADING */
	{171, "BITMAP", "images/particle.bmp"}, /* IDB_PARTICLE */
	{199, "BITMAP", "images/barre3d.bmp"}, /* IDB_39 */
	{194, "BITMAP", "images/pixel_vert3d.bmp"}, /* IDB_30 */
	{235, "BITMAP", "images/game_startup_internet.bmp"}, /* IDB_STARTUP_INTERNET */
	{242, "BITMAP", "images/game_startup_internet_over.bmp"}, /* IDB_STARTUP_INTERNET_OVER */
	{246, "BITMAP", "images/game_about_button_mute.bmp"}, /* IDB_BUTTON_MUTE */
	{247, "BITMAP", "images/game_about_button_play.bmp"}, /* IDB_BUTTON_PLAY */
	{141, "WAVE", "sounds/shoot.mp3"}, /* IDR_SOUND_TIR */
	{152, "WAVE", "sounds/explosion.mp3"}, /* IDR_SOUND_EXPLOSION */
	{153, "WAVE", "sounds/applause.mp3"}, /* IDR_SOUND_APPLAUSE */
	{154, "WAVE", "sounds/descent.mp3"}, /* IDR_SOUND_DESCENTE */
	{198, "WAVE", "sounds/shoot_3d.mp3"}, /* IDR_SOUND_TIR3D */
	{218, "WAVE", "sounds/shield_charge.mp3"}, /* IDR_SOUND_SHIELD_CHARGE */
	{244, "WAVE", "sounds/1_music_jt_strng.xm"}, /* IDR_SOUND_XM_ABOUT */
	{156, "WAVE", "sounds/2_music_1mmsan.oxm"}, /* IDR_SOUND_XM */
	{287, "WAVE", "sounds/bonus.mp3"}, /* IDR_SOUND_BONUS */
	{289, "WAVE", "sounds/key.mp3"}, /* IDR_SOUND_KEY */
	{307, "WAVE", "sounds/cross.mp3"}, /* IDR_SOUND_CROSS */
	{160, "BINARY", "images/font.tga"}, /* IDR_BINARY_FONT_TGA */
	{204, "BINARY", "models/player.3ds"}, /* IDR_BINARY_PLAYER_3DS */
	{205, "BINARY", "models/ship.3ds"}, /* IDR_BINARY_SHIP_3DS */
	{206, "BINARY", "models/sphere_ogive.3ds"}, /* IDR_BINARY_SPHERE_OGIVE_3DS */
	{210, "BINARY", "models/supership.3ds"}, /* IDR_BINARY_SUPERSHIP_3DS */
	{282, "BINARY", "models/fighter_laser.3ds"}, /* IDR_BINARY_FIGHTER_LASER_3DS */
	{187, "JPG", "images/fond_planete.jpg"}, /* IDJ_28 */
	{195, "JPG", "images/fond_earth_moon.jpg"}, /* IDJ_37 */
	{196, "JPG", "images/fond_galaxy1.jpg"}, /* IDJ_38 */
	{202, "JPG", "models/S02_5121.jpg"}, /* IDJ_S02_5121 */
	{203, "JPG", "models/sphere_o.jpg"}, /* IDJ_SPHERE_O */
	{209, "JPG", "models/F02_512.jpg"}, /* IDJ_F02_512 */
	{216, "JPG", "images/powerup.jpg"}, /* IDJ_42 */
	{220, "JPG", "images/particule4.jpg"}, /* IDJ_44 */
	{278, "JPG", "images/cadre_centre.jpg"}, /* IDJ_CADRE_CENTRE */
	{279, "JPG", "images/cadre_centre_long.jpg"}, /* IDJ_CADRE_CENTRE_LONG */
	{280, "JPG", "images/fond_skybox.jpg"}, /* IDJ_09 */
	{281, "JPG", "images/particle2.jpg"}, /* IDJ_41 */
	{283, "JPG", "images/tir_3d_base.jpg"}, /* IDJ_31 */
	{284, "JPG", "images/tir_3d_base_dark_backgrd.jpg"}, /* IDJ_32 */
	{285, "JPG", "images/tir_3d_base_double.jpg"}, /* IDJ_45 */
	{318, "JPG", "images/sphere.jpg"}, /* IDJ_53 */
	{123, "GIF", "images/fond_espace.gif"}, /* IDG_07 */
	{217, "GIF", "images/particle3.gif"}, /* IDG_43 */
	{271, "GIF", "images/equalizer_barre.gif"}, /* IDG_36 */
	{286, "GIF", "images/text_weapon.gif"}, /* IDG_46 */
	{288, "GIF", "images/text_bonus.gif"}, /* IDG_47 */
	{317, "GIF", "images/particle3_3d.gif"}, /* IDG_52 */
	{225, "TGA", "images/player.tga"}, /* IDT_00 */
	{232, "TGA", "images/super_ship.tga"}, /* IDT_04 */
	{233, "TGA", "images/player3d.tga"}, /* IDT_40 */
	{312, "PNG", "images/cadre_energy_contour.png"}, /* IDP_48 */
	{313, "PNG", "images/cadre_energy.png"}, /* IDP_49 */
	{314, "PNG", "images/equalizer.png"}, /* IDP_35 */
	{315, "PNG", "images/bonus1.png"}, /* IDP_50 */
	{316, "PNG", "images/particle3.png"}, /* IDP_51 */
};

static int is_int_resource(uintptr_t value)
{
	return value < 0x10000;
}

static int kind_matches(const char* requested, const char* kind)
{
	uintptr_t as_int = (uintptr_t)requested;
	if (is_int_resource(as_int))
		return as_int == 2 && strcmp(kind, "BITMAP") == 0;
	if (strcmp(requested, "JPG") == 0 || strcmp(requested, "JPEG") == 0)
		return strcmp(kind, "JPG") == 0;
	return strcmp(requested, kind) == 0;
}

static const char* resolve_path(const char* relative)
{
	static char buf[1024];
	if (!relative)
		return "";
	if (access(relative, R_OK) == 0)
		return relative;
	snprintf(buf, sizeof buf, "../%s", relative);
	if (access(buf, R_OK) == 0)
		return buf;
	return relative;
}

static Asset* find_asset(unsigned id, const char* kind)
{
	size_t count = sizeof g_assets / sizeof g_assets[0];
	size_t i;
	for (i = 0; i < count; i++)
	{
		if (g_assets[i].id == id && kind_matches(kind, g_assets[i].kind))
			return &g_assets[i];
	}
	return 0;
}

static int load_asset(Asset* asset)
{
	FILE* file;
	long length;
	const char* path;
	if (!asset)
		return 0;
	if (asset->loaded)
		return asset->data != 0;
	asset->loaded = 1;
	path = resolve_path(asset->path);
	file = fopen(path, "rb");
	if (!file)
		return 0;
	if (fseek(file, 0, SEEK_END) != 0)
	{
		fclose(file);
		return 0;
	}
	length = ftell(file);
	if (length < 0)
	{
		fclose(file);
		return 0;
	}
	rewind(file);
	asset->data = (unsigned char*)malloc((size_t)length + 1);
	if (!asset->data)
	{
		fclose(file);
		return 0;
	}
	if (length > 0 && fread(asset->data, 1, (size_t)length, file) != (size_t)length)
	{
		free(asset->data);
		asset->data = 0;
		fclose(file);
		return 0;
	}
	asset->data[length] = 0;
	asset->size = (unsigned)length;
	fclose(file);
	return 1;
}

unsigned macos_asset_bytes(unsigned id, const char* kind)
{
	Asset* asset = find_asset(id, kind);
	if (!asset || !load_asset(asset))
		return 0;
	return asset->size;
}

HRSRC FindResource(HMODULE module, LPCSTR name, LPCSTR type)
{
	uintptr_t id;
	Asset* asset;
	(void)module;
	id = (uintptr_t)name;
	if (!is_int_resource(id))
		return 0;
	asset = find_asset((unsigned)id, type);
	if (!asset || !load_asset(asset))
		return 0;
	return (HRSRC)asset;
}

DWORD SizeofResource(HMODULE module, HRSRC resource)
{
	Asset* asset = (Asset*)resource;
	(void)module;
	if (!asset)
		return 0;
	return asset->size;
}

HGLOBAL LoadResource(HMODULE module, HRSRC resource)
{
	(void)module;
	return (HGLOBAL)resource;
}

LPVOID LockResource(HGLOBAL resource)
{
	Asset* asset = (Asset*)resource;
	if (!asset)
		return 0;
	return asset->data;
}

BOOL UnlockResource(HGLOBAL resource)
{
	(void)resource;
	return TRUE;
}

BOOL FreeResource(HGLOBAL resource)
{
	(void)resource;
	return TRUE;
}

#define MAC_BITMAP_MAGIC 0x4d414301u
#define MAC_DC_MAGIC 0x4d414302u
#define MAC_BLOCK_MAGIC 0x4d414303u

typedef struct MacBitmap {
	unsigned magic;
	int width;
	int height;
	int bpp;
	int stride;
	unsigned char* bits;
} MacBitmap;

typedef struct MacDC {
	unsigned magic;
	MacBitmap* selected;
} MacDC;

typedef struct MacBlock {
	unsigned magic;
	unsigned size;
	unsigned char bytes[1];
} MacBlock;

HGLOBAL GlobalAlloc(UINT flags, DWORD bytes)
{
	MacBlock* block;
	(void)flags;
	block = (MacBlock*)calloc(1, sizeof(MacBlock) + bytes);
	if (!block)
		return 0;
	block->magic = MAC_BLOCK_MAGIC;
	block->size = bytes;
	return (HGLOBAL)block;
}

LPVOID GlobalLock(HGLOBAL memory)
{
	MacBlock* block = (MacBlock*)memory;
	if (!block || block->magic != MAC_BLOCK_MAGIC)
		return 0;
	return block->bytes;
}

void* macos_global_lock(void* block, unsigned* size)
{
	MacBlock* memory = (MacBlock*)block;
	if (!memory || memory->magic != MAC_BLOCK_MAGIC)
		return 0;
	if (size)
		*size = memory->size;
	return memory->bytes;
}

BOOL GlobalUnlock(HGLOBAL memory)
{
	(void)memory;
	return TRUE;
}

static MacBitmap* bitmap_from(void* handle)
{
	MacBitmap* bitmap = (MacBitmap*)handle;
	if (!bitmap || bitmap->magic != MAC_BITMAP_MAGIC)
		return 0;
	return bitmap;
}

static MacBitmap* make_bitmap(int width, int height, int bpp, unsigned char* bits)
{
	MacBitmap* bitmap = (MacBitmap*)calloc(1, sizeof(MacBitmap));
	if (!bitmap)
	{
		free(bits);
		return 0;
	}
	bitmap->magic = MAC_BITMAP_MAGIC;
	bitmap->width = width;
	bitmap->height = height;
	bitmap->bpp = bpp;
	bitmap->stride = width * (bpp / 8);
	bitmap->bits = bits;
	return bitmap;
}

HANDLE LoadImage(HINSTANCE instance, LPCSTR name, UINT type, int cx, int cy, UINT load)
{
	Asset* asset;
	unsigned char* pixels = 0;
	int width = 0;
	int height = 0;
	(void)instance;
	(void)cx;
	(void)cy;
	(void)load;
	if (type != IMAGE_BITMAP)
		return 0;
	asset = find_asset((unsigned)(uintptr_t)name, (const char*)(uintptr_t)2);
	if (!asset || !load_asset(asset))
		return 0;
	if (!macos_decode_memory(asset->data, asset->size, 3, 1, &pixels, &width, &height))
		return 0;
	return (HANDLE)make_bitmap(width, height, 24, pixels);
}

int GetObject(HANDLE object, int bytes, LPVOID out)
{
	MacBitmap* bitmap = bitmap_from(object);
	BITMAP* info;
	(void)bytes;
	if (!bitmap || !out)
		return 0;
	info = (BITMAP*)out;
	memset(info, 0, sizeof *info);
	info->bmWidth = bitmap->width;
	info->bmHeight = bitmap->height;
	info->bmWidthBytes = bitmap->stride;
	info->bmPlanes = 1;
	info->bmBitsPixel = (WORD)bitmap->bpp;
	info->bmBits = bitmap->bits;
	return (int)sizeof(BITMAP);
}

HDC CreateCompatibleDC(HDC hdc)
{
	MacDC* dc;
	(void)hdc;
	dc = (MacDC*)calloc(1, sizeof(MacDC));
	if (!dc)
		return 0;
	dc->magic = MAC_DC_MAGIC;
	return (HDC)dc;
}

HBITMAP CreateDIBSection(HDC hdc, const BITMAPINFO* info, UINT usage, void** bits, HANDLE section, DWORD offset)
{
	int width;
	int height;
	int bpp;
	unsigned char* pixels;
	MacBitmap* bitmap;
	(void)hdc;
	(void)usage;
	(void)section;
	(void)offset;
	if (!info)
		return 0;
	width = info->bmiHeader.biWidth;
	height = info->bmiHeader.biHeight;
	if (height < 0)
		height = -height;
	if (width <= 0 || height <= 0 || width > 4096 || height > 4096)
		return 0;
	bpp = info->bmiHeader.biBitCount ? info->bmiHeader.biBitCount : 32;
	pixels = (unsigned char*)calloc((size_t)width * (size_t)height, bpp / 8);
	bitmap = make_bitmap(width, height, bpp, pixels);
	if (bits)
		*bits = bitmap ? bitmap->bits : 0;
	return (HBITMAP)bitmap;
}

HGDIOBJ SelectObject(HDC hdc, HGDIOBJ object)
{
	MacDC* dc = (MacDC*)hdc;
	MacBitmap* bitmap = bitmap_from(object);
	if (!dc || dc->magic != MAC_DC_MAGIC || !bitmap)
		return 0;
	dc->selected = bitmap;
	return object;
}

BOOL DeleteDC(HDC hdc)
{
	MacDC* dc = (MacDC*)hdc;
	if (!dc || dc->magic != MAC_DC_MAGIC)
		return FALSE;
	free(dc);
	return TRUE;
}

BOOL DeleteObject(HGDIOBJ object)
{
	MacBitmap* bitmap = bitmap_from(object);
	if (!bitmap)
		return TRUE;
	free(bitmap->bits);
	free(bitmap);
	return TRUE;
}

void* macos_dc_bits(void* hdc, int* width, int* height)
{
	MacDC* dc = (MacDC*)hdc;
	if (!dc || dc->magic != MAC_DC_MAGIC || !dc->selected)
		return 0;
	if (width)
		*width = dc->selected->width;
	if (height)
		*height = dc->selected->height;
	return dc->selected->bits;
}

int GetDeviceCaps(HDC hdc, int index)
{
	(void)hdc;
	if (index == LOGPIXELSX || index == LOGPIXELSY)
		return 96;
	if (index == HORZRES)
		return 1440;
	if (index == VERTRES)
		return 900;
	if (index == PLANES || index == BITSPIXEL)
		return 1;
	return 0;
}

int MulDiv(int number, int numerator, int denominator)
{
	if (denominator == 0)
		return 0;
	return (int)(((long long)number * numerator) / denominator);
}
