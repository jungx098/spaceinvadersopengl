/* OLE picture stubs so the JPG/GIF loader type-checks. */
#ifndef _OLECTL_H_SHIM_
#define _OLECTL_H_SHIM_
#include "windows.h"

#ifdef __cplusplus
struct IStream {
	unsigned long Release();
};

struct IPicture {
	unsigned long Release();
	long get_Width(long* width);
	long get_Height(long* height);
	long Render(HDC hdc, long x, long y, long cx, long cy,
		long xSrc, long ySrc, long cxSrc, long cySrc, const void* rect);
};

extern "C" const GUID IID_IPicture;

HRESULT CreateStreamOnHGlobal(HGLOBAL global, BOOL delete_on_release, IStream** stream);
HRESULT OleLoadPicture(IStream* stream, LONG size, BOOL runmode, const GUID& iid, void** object);
HRESULT OleLoadPicturePath(const OLECHAR* path, void* caller, DWORD reserved, unsigned long color, const GUID& iid, void** object);
#endif

#endif
