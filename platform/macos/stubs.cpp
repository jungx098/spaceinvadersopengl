/* C++ link stubs for the OLE picture types. Win32 and FMOD symbols live in stubs.c. */
#include "olectl.h"

unsigned long IStream::Release() { return 0; }
unsigned long IPicture::Release() { return 0; }
long IPicture::get_Width(long* width) { if (width) *width = 0; return 0; }
long IPicture::get_Height(long* height) { if (height) *height = 0; return 0; }
long IPicture::Render(HDC, long, long, long, long, long, long, long, long, const void*) { return 0; }

HRESULT CreateStreamOnHGlobal(HGLOBAL, BOOL, IStream** stream)
{
	if (stream)
		*stream = 0;
	return 1;
}

HRESULT OleLoadPicture(IStream*, LONG, BOOL, const GUID&, void** object)
{
	if (object)
		*object = 0;
	return 1;
}

HRESULT OleLoadPicturePath(const OLECHAR*, void*, DWORD, unsigned long, const GUID&, void** object)
{
	if (object)
		*object = 0;
	return 1;
}
