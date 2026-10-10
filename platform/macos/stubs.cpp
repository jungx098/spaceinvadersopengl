/* OLE picture objects. Pixel decoding lives in image.mm. */
#include "olectl.h"
#include "macos_port.h"

#include <cstdlib>

unsigned long IStream::Release()
{
	delete this;
	return 0;
}

unsigned long IPicture::Release()
{
	std::free(bgra);
	delete this;
	return 0;
}

static long himetric(int pixels)
{
	return (long)((pixels * 2540) / 96);
}

long IPicture::get_Width(long* out)
{
	if (out)
		*out = himetric(width);
	return 0;
}

long IPicture::get_Height(long* out)
{
	if (out)
		*out = himetric(height);
	return 0;
}

long IPicture::Render(HDC hdc, long x, long y, long cx, long cy,
	long, long, long, long cySrc, const void*)
{
	(void)x;
	(void)y;
	int dest_w = 0;
	int dest_h = 0;
	unsigned char* dest = (unsigned char*)macos_dc_bits(hdc, &dest_w, &dest_h);
	if (!dest || !bgra || width <= 0 || height <= 0 || cx == 0 || cy == 0)
		return 0;
	if (dest_w <= 0)
		dest_w = (int)cx;
	if (dest_h <= 0)
		dest_h = (int)cy;

	/* The game passes a negative source height, which flips the picture into
	   the bottom-up DIB. OpenGL then treats the first row as the bottom. */
	bool flip = cySrc < 0;
	for (int row = 0; row < dest_h; row++)
	{
		int src_y = (int)((long)row * height / dest_h);
		if (src_y >= height)
			src_y = height - 1;
		if (flip)
			src_y = height - 1 - src_y;
		for (int col = 0; col < dest_w; col++)
		{
			int src_x = (int)((long)col * width / dest_w);
			if (src_x >= width)
				src_x = width - 1;
			const unsigned char* src = bgra + ((src_y * width + src_x) * 4);
			unsigned char* dst = dest + ((row * dest_w + col) * 4);
			dst[0] = src[0];
			dst[1] = src[1];
			dst[2] = src[2];
			dst[3] = src[3];
		}
	}
	return 0;
}

extern "C" const GUID IID_IPicture = {0, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0}};

HRESULT CreateStreamOnHGlobal(HGLOBAL global, BOOL, IStream** stream)
{
	if (!stream)
		return 1;
	IStream* out = new IStream();
	unsigned size = 0;
	out->data = macos_global_lock(global, &size);
	out->size = size;
	*stream = out;
	return 0;
}

static HRESULT load_picture(const void* data, unsigned size, void** object)
{
	if (!object)
		return 1;
	*object = 0;
	unsigned char* pixels = 0;
	int w = 0;
	int h = 0;
	if (!macos_decode_memory(data, size, 4, 1, &pixels, &w, &h) || w <= 0 || h <= 0)
		return 1;
	IPicture* picture = new IPicture();
	picture->width = w;
	picture->height = h;
	picture->bgra = pixels;
	*object = picture;
	return 0;
}

HRESULT OleLoadPicture(IStream* stream, LONG, BOOL, const GUID&, void** object)
{
	if (!stream)
		return 1;
	return load_picture(stream->data, stream->size, object);
}

HRESULT OleLoadPicturePath(const OLECHAR*, void*, DWORD, unsigned long, const GUID&, void** object)
{
	if (object)
		*object = 0;
	return 1;
}
