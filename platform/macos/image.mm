/* Decode the game's BMP, PNG, JPG, and GIF files with ImageIO. */
#include "macos_port.h"
#include "glpng.h"

#import <CoreGraphics/CoreGraphics.h>
#import <ImageIO/ImageIO.h>

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C" int macos_decode_memory(const void* data, unsigned size, int components, int bottom_up,
	unsigned char** out, int* width, int* height)
{
	if (out)
		*out = 0;
	if (!data || size == 0 || (components != 3 && components != 4))
		return 0;

	CFDataRef cf = CFDataCreate(kCFAllocatorDefault, (const UInt8*)data, size);
	if (!cf)
		return 0;
	CGImageSourceRef source = CGImageSourceCreateWithData(cf, NULL);
	CFRelease(cf);
	if (!source)
		return 0;
	CGImageRef image = CGImageSourceCreateImageAtIndex(source, 0, NULL);
	CFRelease(source);
	if (!image)
		return 0;

	int w = (int)CGImageGetWidth(image);
	int h = (int)CGImageGetHeight(image);
	if (w <= 0 || h <= 0)
	{
		CGImageRelease(image);
		return 0;
	}

	unsigned char* rgba = (unsigned char*)calloc((size_t)w * (size_t)h, 4);
	if (!rgba)
	{
		CGImageRelease(image);
		return 0;
	}

	CGColorSpaceRef space = CGColorSpaceCreateDeviceRGB();
	CGContextRef ctx = CGBitmapContextCreate(rgba, w, h, 8, w * 4, space,
		kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
	CGColorSpaceRelease(space);
	if (!ctx)
	{
		free(rgba);
		CGImageRelease(image);
		return 0;
	}
	CGContextDrawImage(ctx, CGRectMake(0, 0, w, h), image);
	CGContextRelease(ctx);
	CGImageRelease(image);

	/* The bitmap context is bottom-up. Flip when the caller wants top-down. */
	if (!bottom_up)
	{
		int stride = w * 4;
		unsigned char* row = (unsigned char*)malloc(stride);
		for (int y = 0; y < h / 2; y++)
		{
			unsigned char* a = rgba + y * stride;
			unsigned char* b = rgba + (h - 1 - y) * stride;
			memcpy(row, a, stride);
			memcpy(a, b, stride);
			memcpy(b, row, stride);
		}
		free(row);
	}

	unsigned char* pixels = (unsigned char*)malloc((size_t)w * (size_t)h * components);
	if (!pixels)
	{
		free(rgba);
		return 0;
	}
	for (int i = 0; i < w * h; i++)
	{
		unsigned char r = rgba[i * 4 + 0];
		unsigned char g = rgba[i * 4 + 1];
		unsigned char b = rgba[i * 4 + 2];
		unsigned char a = rgba[i * 4 + 3];
		if (components == 3)
		{
			pixels[i * 3 + 0] = b;
			pixels[i * 3 + 1] = g;
			pixels[i * 3 + 2] = r;
		}
		else
		{
			pixels[i * 4 + 0] = b;
			pixels[i * 4 + 1] = g;
			pixels[i * 4 + 2] = r;
			pixels[i * 4 + 3] = a;
		}
	}
	free(rgba);
	*out = pixels;
	if (width)
		*width = w;
	if (height)
		*height = h;
	return 1;
}

static unsigned upload_rgba(unsigned char* rgba, int w, int h, int wrapst, int minfilter, int magfilter)
{
	GLuint id = 0;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	if (wrapst)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapst);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapst);
	}
	if (minfilter)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minfilter);
	if (magfilter)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magfilter);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba);
	return id;
}

extern "C" unsigned int pngBind(const char* filename, int mipmap, int trans, pngInfo* info, int wrapst, int minfilter, int magfilter)
{
	(void)mipmap;
	(void)trans;
	(void)info;
	FILE* file = fopen(filename, "rb");
	if (!file)
		return 0;
	unsigned int id = pngBindF(file, mipmap, trans, info, wrapst, minfilter, magfilter);
	fclose(file);
	return id;
}

extern "C" unsigned int pngBindF(FILE* file, int mipmap, int trans, pngInfo* info, int wrapst, int minfilter, int magfilter)
{
	(void)mipmap;
	(void)trans;
	(void)info;
	if (!file)
		return 0;
	if (fseek(file, 0, SEEK_END) != 0)
		return 0;
	long length = ftell(file);
	if (length <= 0)
		return 0;
	rewind(file);
	unsigned char* filebytes = (unsigned char*)malloc((size_t)length);
	if (!filebytes)
		return 0;
	if (fread(filebytes, 1, (size_t)length, file) != (size_t)length)
	{
		free(filebytes);
		return 0;
	}

	unsigned char* pixels = 0;
	int w = 0;
	int h = 0;
	/* pngBind consumers sample the buffer as RGBA. Keep the context's bottom-up order. */
	int ok = macos_decode_memory(filebytes, (unsigned)length, 4, 1, &pixels, &w, &h);
	free(filebytes);
	if (!ok)
		return 0;

	/* macos_decode_memory writes BGRA. png textures in this game use the alpha byte as-is,
	   and the existing PNG path does not swap channels, so convert BGRA back to RGBA. */
	for (int i = 0; i < w * h; i++)
	{
		unsigned char b = pixels[i * 4 + 0];
		pixels[i * 4 + 0] = pixels[i * 4 + 2];
		pixels[i * 4 + 2] = b;
	}
	unsigned id = upload_rgba(pixels, w, h, wrapst, minfilter, magfilter);
	free(pixels);
	return id;
}
