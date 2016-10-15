//////////////////////////////////////////////////////////////////////
//
// OpenGL Texture Class
// by: Matthew Fairfax
//
// GLTexture.cpp: implementation of the GLTexture class.
// This class loads a texture file and prepares it
// to be used in OpenGL. It can open a bitmap or a
// targa file. The min filter is set to mipmap b/c
// they look better and the performance cost on
// modern video cards in negligible. I leave all of
// the texture management to the application. I have
// included the ability to load the texture from a
// Visual Studio resource. The bitmap's id must be
// be surrounded by quotation marks (i.e. "Texture.bmp").
// The targa files must be in a resource type of "TGA"
// (including the quotes). The targa's id must be
// surrounded by quotation marks (i.e. "Texture.tga").
//
// Usage:
// GLTexture tex;
// GLTexture tex1;
// GLTexture tex3;
//
// tex.Load("texture.bmp"); // Loads a bitmap
// tex.Use();				// Binds the bitmap for use
// 
// tex1.LoadFromResource("texture.tga"); // Loads a targa
// tex1.Use();				 // Binds the targa for use
//
// // You can also build a texture with a single color and use it
// tex3.BuildColorTexture(255, 0, 0);	// Builds a solid red texture
// tex3.Use();				 // Binds the targa for use
//
//////////////////////////////////////////////////////////////////////

#include "si_3DS_Texture.h"
#include "si_main.h"					// <CUSTOM> For Error Handling : ERR()
#include <math.h>						// Header File For sin(), cos(), _hypot()
#include <olectl.h>						// Header File For The OLE Controls Library

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <shlwapi.h>					// For PathRemoveExtension() etc., à virer avec la lib + tard

extern bool game_option_mipmap;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLTexture::GLTexture()
{

}

GLTexture::~GLTexture()
{

}

void GLTexture::Load(char *name)
{
	// make the texture name all lower case
	texturename = _strlwr(_strdup(name));

	// strip "'s
	if (strstr(texturename, "\""))
		texturename = strtok(texturename, "\"");

   char drive[_MAX_DRIVE];
   char dir[_MAX_DIR];
   char fname[_MAX_FNAME];
   char ext[_MAX_EXT];

   _splitpath( texturename, drive, dir, fname, ext );

	// check the file extension to see what type of texture
	if(strcmp(ext, ".jpg") == NULL)
	{
		texturename = _strupr(fname);
		
		char resource_name[256]="";
		strcpy(resource_name, "IDJ_");
		strcat(resource_name, texturename);
		strcat(resource_name, "\0");

		LoadJPGResource(resource_name);
	}

//  uncomment if you are using bmp textures
/*	else if(strstr(texturename, ".bmp"))	// à modifier par ext
	{
		PathRemoveExtension(texturename);		// needs shlwapi.h and linked with shlwapi.lib
		PathAddExtension(texturename,".bmp");	// idem

		LoadBMP(texturename);
	}
*/	
	else if(strstr(texturename, ".tga"))	// à modifier par ext
		LoadTGA(texturename);
}

void GLTexture::LoadFromResource(char *name)
{
	// make the texture name all lower case
	texturename = _strlwr(_strdup(name));

	// check the file extension to see what type of texture
	if(strstr(texturename, ".bmp"))
		LoadBMPResource(name);
	if(strstr(texturename, ".tga"))	
		LoadTGAResource(name);
}

void GLTexture::Use()
{
	glBindTexture(GL_TEXTURE_2D, texture[0]);				// Bind the texture as the current one
}

void GLTexture::BuildColorTexture(unsigned char r, unsigned char g, unsigned char b)
{
	unsigned char data[12];	// a 2x2 texture at 24 bits

	// Store the data
	for(int i = 0; i < 12; i += 3)
	{
		data[i] = r;
		data[i+1] = g;
		data[i+2] = b;
	}

	// Generate the OpenGL texture id
	glGenTextures(1, &texture[0]);

	// Bind this texture to its id
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	if (!game_option_mipmap) // <CUSTOM> if you don't use an mipmap option, delete this statement
	{
		// Not use mipmapping filter
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		// Use mipmapping filter
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

		// Generate the texture
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, 2, 2, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
}

BOOL GLTexture::LoadJPGResource(char *name)
{
		HDC			hdcTemp;												// The DC To Hold Our Bitmap
		HBITMAP		hbmpTemp;												// Holds The Bitmap Temporarily
		IPicture	*pPicture;												// IPicture Interface
		long		lWidth;													// Width In Logical Units
		long		lHeight;												// Height In Logical Units
		long		lWidthPixels;											// Width In Pixels
		long		lHeightPixels;											// Height In Pixels
		GLint		glMaxTexDim ;											// Holds Maximum Texture Size


		// from resources <CUSTOM> replace with your resource textures identifiers
		HRSRC	hRes;
		if (strcmp(name,"IDJ_SPHERE_O") == NULL)
			hRes = FindResource(NULL, MAKEINTRESOURCE(IDJ_SPHERE_O), "JPG");
		else if (strcmp(name,"IDJ_S02_5121") == NULL)
			hRes = FindResource(NULL, MAKEINTRESOURCE(IDJ_S02_5121), "JPG");
		else if (strcmp(name,"IDJ_F02_512") == NULL)
			hRes = FindResource(NULL, MAKEINTRESOURCE(IDJ_F02_512), "JPG");
		
		else ERR("A 3DS texture is missing");

		DWORD	dwDataSize	= SizeofResource(NULL,hRes);
		HGLOBAL	hGlob		= LoadResource(NULL,hRes);
		LPVOID	pData		= LockResource(hGlob);

		HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwDataSize);
		void* pData2 = GlobalLock(hGlobal);
		memcpy(pData2, pData, dwDataSize);
		GlobalUnlock(hGlobal);
		UnlockResource(hGlob);
		FreeResource(hGlob);	

		IStream* pStream = NULL;
		pPicture = NULL;
		HRESULT hr;

		if (CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) == S_OK)
		{
	        hr = OleLoadPicture(pStream, 0, 0, IID_IPicture, (void**)&pPicture);
			pStream->Release();
		}	
	
		if(FAILED(hr))														// If Loading Failed
			return FALSE;													// Return False

		hdcTemp = CreateCompatibleDC(GetDC(0));								// Create The Windows Compatible Device Context
		if(!hdcTemp)														// Did Creation Fail?
		{
			pPicture->Release();											// Decrements IPicture Reference Count
			return FALSE;													// Return False (Failure)
		}

		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glMaxTexDim);					// Get Maximum Texture Size Supported
	
		pPicture->get_Width(&lWidth);										// Get IPicture Width (Convert To Pixels)
		lWidthPixels	= MulDiv(lWidth, GetDeviceCaps(hdcTemp, LOGPIXELSX), 2540);
		pPicture->get_Height(&lHeight);										// Get IPicture Height (Convert To Pixels)
		lHeightPixels	= MulDiv(lHeight, GetDeviceCaps(hdcTemp, LOGPIXELSY), 2540);

		// Resize Image To Closest Power Of Two
		if (lWidthPixels <= glMaxTexDim) // Is Image Width Less Than Or Equal To Cards Limit
			lWidthPixels = 1 << (int)floor((log((double)lWidthPixels)/log(2.0f)) + 0.5f); 
		else  // Otherwise  Set Width To "Max Power Of Two" That The Card Can Handle
			lWidthPixels = glMaxTexDim;
	 
		if (lHeightPixels <= glMaxTexDim) // Is Image Height Greater Than Cards Limit
			lHeightPixels = 1 << (int)floor((log((double)lHeightPixels)/log(2.0f)) + 0.5f);
		else  // Otherwise  Set Height To "Max Power Of Two" That The Card Can Handle
			lHeightPixels = glMaxTexDim;
	
		//	Create A Temporary Bitmap
		BITMAPINFO	bi = {0};												// The Type Of Bitmap We Request
		DWORD		*pBits = 0;												// Pointer To The Bitmap Bits

		bi.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);				// Set Structure Size
		bi.bmiHeader.biBitCount		= 32;									// 32 Bit
		bi.bmiHeader.biWidth		= lWidthPixels;							// Power Of Two Width
		bi.bmiHeader.biHeight		= lHeightPixels;						// Make Image Top Up (Positive Y-Axis)
		bi.bmiHeader.biCompression	= BI_RGB;								// RGB Encoding
		bi.bmiHeader.biPlanes		= 1;									// 1 Bitplane

		//	Creating A Bitmap This Way Allows Us To Specify Color Depth And Gives Us Imediate Access To The Bits
		hbmpTemp = CreateDIBSection(hdcTemp, &bi, DIB_RGB_COLORS, (void**)&pBits, 0, 0);
	
		if(!hbmpTemp)														// Did Creation Fail?
		{
			DeleteDC(hdcTemp);												// Delete The Device Context
			pPicture->Release();											// Decrements IPicture Reference Count
			return FALSE;													// Return False (Failure)
		}

		SelectObject(hdcTemp, hbmpTemp);									// Select Handle To Our Temp DC And Our Temp Bitmap Object

		// Render The IPicture On To The Bitmap
		pPicture->Render(hdcTemp, 0, 0, lWidthPixels, lHeightPixels, 0, lHeight, lWidth, -lHeight, 0);

		// Convert From BGR To RGB Format And Add An Alpha Value Of 255
		for(long i = 0; i < lWidthPixels * lHeightPixels; i++)				// Loop Through All Of The Pixels
		{
			BYTE* pPixel	= (BYTE*)(&pBits[i]);							// Grab The Current Pixel
			BYTE  temp		= pPixel[0];									// Store 1st Color In Temp Variable (Blue)
			pPixel[0]		= pPixel[2];									// Move Red Value To Correct Position (1st)
			pPixel[2]		= temp;											// Move Temp Value To Correct Blue Position (3rd)
			pPixel[3]		= 255;											// Set The Alpha Value To 255
		}

		glGenTextures(1, &texture[0]);							// Create The Texture

		// Typical Texture Generation Using Data From The Bitmap
		glBindTexture(GL_TEXTURE_2D, texture[0]);							// Bind To The Texture ID

		if (!game_option_mipmap) // <CUSTOM> if you don't use an mipmap option, delete this statement
		{
			// Not use mipmapping filter
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);		// (Modify This For The Type Of Filtering You Want)
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);     // (Modify This For The Type Of Filtering You Want)
			glTexImage2D(GL_TEXTURE_2D, 0, 3, lWidthPixels, lHeightPixels, 0, GL_RGBA, GL_UNSIGNED_BYTE, pBits);	// (Modify This If You Want Mipmaps)
		}
		else
		{
			// Use mipmapping filter
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			// Generate the mipmaps
			gluBuild2DMipmaps(GL_TEXTURE_2D, 3, lWidthPixels, lHeightPixels, GL_RGBA, GL_UNSIGNED_BYTE, pBits);
		}

		DeleteObject(hbmpTemp);												// Delete The Object
		DeleteDC(hdcTemp);													// Delete The Device Context

		pPicture->Release();												// Decrements IPicture Reference Count

		return TRUE;
}

void GLTexture::LoadBMP(char *name)
{
/*
	// Create a place to store the texture
	AUX_RGBImageRec *TextureImage[1];

	// Set the pointer to NULL
	memset(TextureImage,0,sizeof(void *)*1);

	// Load the bitmap and assign our pointer to it
	TextureImage[0] = auxDIBImageLoad(name);

	// Just in case we want to use the width and height later
	width = TextureImage[0]->sizeX;
	height = TextureImage[0]->sizeY;

	// Generate the OpenGL texture id
	glGenTextures(1, &texture[0]);

	// Bind this texture to its id
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	// Use mipmapping filter
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	// Generate the mipmaps
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);

	// Cleanup
	if (TextureImage[0])
	{
		if (TextureImage[0]->data)
			free(TextureImage[0]->data);

		free(TextureImage[0]);
	}
	*/
}

void GLTexture::LoadTGA(char *name)
{
	/*
	GLubyte		TGAheader[12]	= {0,0,2,0,0,0,0,0,0,0,0,0};// Uncompressed TGA header
	GLubyte		TGAcompare[12];								// Used to compare TGA header
	GLubyte		header[6];									// First 6 useful bytes of the header
	GLuint		bytesPerPixel;								// Holds the number of bytes per pixel used
	GLuint		imageSize;									// Used to store the image size
	GLuint		temp;										// Temporary variable
	GLuint		type			= GL_RGBA;					// Set the default type to RBGA (32 BPP)
	GLubyte		*imageData;									// Image data (up to 32 Bits)
	GLuint		bpp;										// Image color depth in bits per pixel.

	FILE *file = fopen(name, "rb");							// Open the TGA file

	// Load the file and perform checks
	if(file == NULL ||														// Does file exist?
	   fread(TGAcompare,1,sizeof(TGAcompare),file) != sizeof(TGAcompare) ||	// Are there 12 bytes to read?
	   memcmp(TGAheader,TGAcompare,sizeof(TGAheader)) != 0				 ||	// Is it the right format?
	   fread(header,1,sizeof(header),file) != sizeof(header))				// If so then read the next 6 header bytes
	{
		if (file == NULL)									// If the file didn't exist then return
			return;
		else
		{
			fclose(file);									// If something broke then close the file and return
			return;
		}
	}

	// Determine the TGA width and height (highbyte*256+lowbyte)
	width  = header[1] * 256 + header[0];
	height = header[3] * 256 + header[2];
    
	// Check to make sure the targa is valid and is 24 bit or 32 bit
	if(width	<=0	||										// Is the width less than or equal to zero
	   height	<=0	||										// Is the height less than or equal to zero
	   (header[4] != 24 && header[4] != 32))				// Is it 24 or 32 bit?
	{
		fclose(file);										// If anything didn't check out then close the file and return
		return;
	}

	bpp				= header[4];							// Grab the bits per pixel
	bytesPerPixel	= bpp / 8;								// Divide by 8 to get the bytes per pixel
	imageSize		= width * height * bytesPerPixel;		// Calculate the memory required for the data

	// Allocate the memory for the image data
	imageData		= new GLubyte[imageSize];

	// Make sure the data is allocated write and load it
	if(imageData == NULL ||									// Does the memory storage exist?
	   fread(imageData, 1, imageSize, file) != imageSize)	// Does the image size match the memory reserved?
	{
		if(imageData != NULL)								// Was the image data loaded
			free(imageData);								// If so, then release the image data

		fclose(file);										// Close the file
		return;
	}

	// Loop through the image data and swap the 1st and 3rd bytes (red and blue)
	for(GLuint i = 0; i < int(imageSize); i += bytesPerPixel)
	{
		temp = imageData[i];
		imageData[i] = imageData[i + 2];
		imageData[i + 2] = temp;
	}

	// We are done with the file so close it
	fclose(file);

	// Set the type
	if (bpp == 24)
		type = GL_RGB;
	
	// Generate the OpenGL texture id
	glGenTextures(1, &texture[0]);

	// Bind this texture to its id
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	// Use mipmapping filter
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	// Generate the mipmaps
	gluBuild2DMipmaps(GL_TEXTURE_2D, type, width, height, type, GL_UNSIGNED_BYTE, imageData);

	// Cleanup
	free(imageData);
	*/
}

void GLTexture::LoadBMPResource(char *name)
{/*
	// Find the bitmap in the bitmap resources
	HRSRC hrsrc = FindResource(0, name, RT_BITMAP);

	// If you can't find it then return
	if (hrsrc==0)
		return;

	// Load the bitmap
	HGLOBAL resource = LoadResource(0, hrsrc);
	
	// If you can't load it then return
	if (resource==0)
		return;

	// Load it into the buffer
	void *buffer = LockResource(resource);

	// Cast it into a bitmap
	BITMAP *bmp = (BITMAP*)buffer;

	// Get the height and width for future use
	width = bmp->bmWidth;
	height = bmp->bmHeight;

	// Reverse the blue colot bit and the red color bit
	unsigned char *ptr = (unsigned char *)buffer+sizeof(BITMAPINFO)+2;
	unsigned char temp;

	for (int i = 0; i < width*height; i++)
	{
		temp = ptr[i*3];
		ptr[i*3] = ptr[i*3+2];
		ptr[i*3+2] = temp;
	}

	// Generate the OpenGL texture id
	glGenTextures(1, &texture[0]);

	// Bind this texture to its id
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	// Use mipmapping filter
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	// Generate the mipmaps
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, (unsigned char *)buffer+sizeof(BITMAPINFO)+2);

	// Cleanup
	free(buffer);
	free(bmp);
	*/
}

void GLTexture::LoadTGAResource(char *name)
{
	/*
	// struct to cast the resource into
	struct TGAstruct {
		GLubyte	TGAcompare[12];								// Used to compare TGA header
		GLubyte	header[6];									// First 6 useful bytes of the header
	};

	GLubyte		TGAheader[12]	= {0,0,2,0,0,0,0,0,0,0,0,0};// Uncompressed TGA header
	GLuint		bytesPerPixel;								// Holds the number of bytes per pixel used
	GLuint		imageSize;									// Used to store the image size
	GLuint		temp;										// Temporary variable
	GLuint		type			= GL_RGBA;					// Set the default type to RBGA (32 BPP)
	GLubyte		*imageData;									// Image data (up to 32 Bits)
	GLuint		bpp;										// Image color depth in bits per pixel.

	// Find the targa in the "TGA" resources
	HRSRC hrsrc = FindResource(0, name, "TGA");

	// If you can't find it then return
	if (hrsrc==0)
		return;

	// Load the targa
	HGLOBAL resource = LoadResource(0, hrsrc);

	// If you can't load it then return
	if (resource==0)
		return;

	// Load it into the buffer
	void *buffer = LockResource(resource);

	// Cast it into the targa struct
	TGAstruct *top = (TGAstruct*)buffer;

	// Make sure it checks out against our comparison header
	if (memcmp(TGAheader,top,sizeof(TGAheader)) != 0)
		return;

	// Determine the TGA width and height (highbyte*256+lowbyte)
	width  = top->header[1] * 256 + top->header[0];
	height = top->header[3] * 256 + top->header[2];
    
	// Check to make sure the targa is valid and is 24 bit or 32 bit
	if(width	<=0	||										// Is the width less than or equal to zero
	   height	<=0	||										// Is the height less than or equal to zero
	   (top->header[4] != 24 && top->header[4] != 32))		// Is it 24 or 32 bit?
	{
		// If anything didn't check out then close the file and return
		return;
	}

	bpp				= top->header[4];							// Grab the bits per pixel
	bytesPerPixel	= bpp / 8;								// Divide by 8 to get the bytes per pixel
	imageSize		= width * height * bytesPerPixel;		// Calculate the memory required for the data

	// Allocate the memory for the image data
	imageData		= new GLubyte[imageSize];

	// Load the data in
	memcpy(imageData, (GLubyte*)buffer+18, imageSize);

	// Loop through the image data and swap the 1st and 3rd bytes (red and blue)
	for(GLuint i = 0; i < int(imageSize); i += bytesPerPixel)
	{
		temp = imageData[i];
		imageData[i] = imageData[i + 2];
		imageData[i + 2] = temp;
	}

	// Set the type
	if (bpp == 24)
		type = GL_RGB;
	
	// Generate the OpenGL texture id
	glGenTextures(1, &texture[0]);

	// Bind this texture to its id
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	// Use mipmapping filter
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	// Generate the mipmaps
	gluBuild2DMipmaps(GL_TEXTURE_2D, type, width, height, type, GL_UNSIGNED_BYTE, imageData);

	// Cleanup
	free(imageData);
	free(buffer);
	free(top);
	*/
}
