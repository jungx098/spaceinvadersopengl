// si_Textures.cpp - space invaders opengl

#include "si_Main.h"
#include "si_Textures.h"
#include "glpng.h"	// Header File For Loading PNG Images (must be link with glpng.lib)

//////////////////////////////////////////////////////////////////////////////////////
//
// TEXTURES => JPG, GIF ou BMP (à partir d'une ressource ou d'un fichier)
//
//
BOOL LoadTexture(char* file, USHORT resource_id, int num_texture)
{

	//
	// IMAGE GIF OU JPG
	//
	if (FindResource(NULL, MAKEINTRESOURCE(resource_id), "JPG") != NULL
		|| FindResource(NULL, MAKEINTRESOURCE(resource_id), "GIF") != NULL)
		LoadTextureJPG_GIF(file, resource_id, num_texture);

	//
	// IMAGE BMP
	//
	else if (FindResource(NULL, MAKEINTRESOURCE(resource_id), RT_BITMAP) != NULL)
		LoadTextureBMP(file, resource_id, num_texture);

	//
	// IMAGE TGA
	//
	else if (FindResource(NULL, MAKEINTRESOURCE(resource_id), "TGA") != NULL)
		LoadTextureTGA(file, resource_id, num_texture);

	//
	// IMAGE PNG
	//
	else if (FindResource(NULL, MAKEINTRESOURCE(resource_id), "PNG") != NULL)
		LoadTexturePNG(file, resource_id, num_texture);

	else return FALSE;

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////////////////
//
// LOAD TEXTURE (JPG, GIF) Attention, la couleur rose (255,0,255) est réservée pour le
//                         transparent (l'alpha est mis à 0)
//
BOOL LoadTextureJPG_GIF(char* file, USHORT resource_id, int num_texture)
{

	HDC			hdcTemp;												// The DC To Hold Our Bitmap
	HBITMAP		hbmpTemp;												// Holds The Bitmap Temporarily
	IPicture	*pPicture;												// IPicture Interface
	OLECHAR		wszPath[MAX_PATH+1];									// Full Path To Picture (WCHAR)
	char		szPath[MAX_PATH+1];										// Full Path To Picture
	long		lWidth;													// Width In Logical Units
	long		lHeight;												// Height In Logical Units
	long		lWidthPixels;											// Width In Pixels
	long		lHeightPixels;											// Height In Pixels
	GLint		glMaxTexDim ;											// Holds Maximum Texture Size
	HRESULT		hr;

	 // from disk
	if (file != NULL)
	{
		if (strstr(file, "http://"))										// If PathName Contains http:// Then...
			strcpy(szPath, file);											// Append The PathName To szPath
		else																// Otherwise... We Are Loading From A File
		{
			GetCurrentDirectory(MAX_PATH, szPath);							// Get Our Working Directory
			strcat(szPath, "\\");											// Append "\" After The Working Directory
			strcat(szPath, file);											// Append The PathName
		}

		MultiByteToWideChar(CP_ACP, 0, szPath, -1, wszPath, MAX_PATH);		// Convert From ASCII To Unicode
		hr = OleLoadPicturePath(wszPath, 0, 0, 0, IID_IPicture, (void**)&pPicture);
	}	

	// from resource
	else
	{
		HRSRC hRes = FindResource(NULL, MAKEINTRESOURCE(resource_id), "JPG");
		if (hRes == NULL)
			hRes = FindResource(NULL, MAKEINTRESOURCE(resource_id), "GIF");

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

		if (CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) == S_OK)
		{
			hr = OleLoadPicture(pStream, 0, 0, IID_IPicture, (void**)&pPicture);
			pStream->Release();
		}	
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

	// Convert from bgr to rgb format and add an alpha value of 255 (opaque)
	// Also set the pink color transparent (see below)
	for(long i = 0; i < lWidthPixels * lHeightPixels; i++)				// Loop Through All Of The Pixels
	{
		BYTE* pPixel	= (BYTE*)(&pBits[i]);							// Grab The Current Pixel
		BYTE  temp		= pPixel[0];									// Store 1st Color In Temp Variable (Blue)
		pPixel[0]		= pPixel[2];									// Move Red Value To Correct Position (1st)
		pPixel[2]		= temp;											// Move Temp Value To Correct Blue Position (3rd)
		pPixel[3]		= 255;											// Set The Alpha Value To 255

		// Set pink pixel (RGB = 255,0,255) an alpha value of 0 (transparent)
		// so be careful to not use this color otherwise delete the lines below !
		if (pPixel[0] == 255)											// si le pixel est rose (255,0,255)...
			if (pPixel[1] == 0)
				if (pPixel[2] == 255)
				{
					pPixel[0] = 0;
					pPixel[1] = 0;
					pPixel[2] = 0;
					pPixel[3] = 0;										// ...on met l'apha à zéro
				}
	}

	glGenTextures(1, &texture[num_texture]);							// Create The Texture

	// Typical Texture Generation Using Data From The Bitmap
	glBindTexture(GL_TEXTURE_2D, texture[num_texture]);					// Bind To The Texture ID
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S ,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T ,GL_CLAMP);

	// customization du filtre pour certaines textures du jeu
	if (!game_option_mipmap || num_texture == 35 || num_texture == 36)	// pour l'equalizer (35 et 36), pas de mipmap => image brute
	{
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR );// Bilinear Filtering
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	// Bilinear filtering
		glTexImage2D(GL_TEXTURE_2D, 0, 4, lWidthPixels, lHeightPixels, 0, GL_RGBA, GL_UNSIGNED_BYTE, pBits);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR );// Trilinear filtering (évite le scintillement des petites textures)
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	// Bilinear filtering
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, lWidthPixels, lHeightPixels, GL_RGBA, GL_UNSIGNED_BYTE, pBits);
	}

	DeleteObject(hbmpTemp);												// Delete The Object
	DeleteDC(hdcTemp);													// Delete The Device Context

	pPicture->Release();												// Decrements IPicture Reference Count

	return TRUE;														// Return True (All Good)
}


//////////////////////////////////////////////////////////////////////////////////////
//
// LOAD TEXTURE (BMP) Attention, les dimensions doivent être des puissances de 2.
//
//
BOOL LoadTextureBMP(char* file, USHORT resource_id, int num_texture)
{
	HBITMAP	hBMP;												// Handle Of The Bitmap
	BITMAP	BMP;												// Bitmap Structure

	hBMP=(HBITMAP)LoadImage(GetModuleHandle(NULL),MAKEINTRESOURCE(resource_id), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

	if (hBMP)												// Does The Bitmap Exist?
	{														// If So...
		GetObject(hBMP,sizeof(BMP), &BMP);					// Get The Object

		glPixelStorei(GL_UNPACK_ALIGNMENT,4);				// Pixel Storage Mode (Word Alignment / 4 Bytes)
		glBindTexture(GL_TEXTURE_2D, texture[num_texture]);	// Bind Our Texture

		// customization pour le jeu -> texture avec mipmap
		if (
			(num_texture >= 0 && num_texture <= 5) || (num_texture >= 9 && num_texture <= 30) || (num_texture >= 37 && num_texture <= 100)
			&& game_option_mipmap
			)
		{
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);	// Trilinear filtering
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);					// Bilinear filtering
		}
		
		// customization pour le jeu -> texture sans mipmap
		else
		{
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);					// Bilinear filtering
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);					// Bilinear filtering
		}

		// customization pour le jeu -> clamp
		if ((num_texture >= 31 && num_texture <= 34))
		{
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S ,GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T ,GL_CLAMP);
		}

		// créé la texture
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, BMP.bmWidth, BMP.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);

		DeleteObject(hBMP);									// Delete The Bitmap Object
		return TRUE;
	}
	else return FALSE;
}


//////////////////////////////////////////////////////////////////////////////////////
//
// LOAD TEXTURE (TGA)
//
//
BOOL LoadTextureTGA(char* file, USHORT resource_id, int num_texture)
{
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
	HRSRC hrsrc = FindResource(0, MAKEINTRESOURCE(resource_id), "TGA");

	// If you can't find it then return
	if (hrsrc == NULL)
		return FALSE;

	// Load the targa
	HGLOBAL resource = LoadResource(0, hrsrc);

	// If you can't load it then return
	if (resource == NULL)
		return FALSE;

	// Load it into the buffer
	void *buffer = LockResource(resource);

	// Cast it into the targa struct
	TGAstruct *top = (TGAstruct*)buffer;

	// Make sure it checks out against our comparison header
	if (memcmp(TGAheader,top,sizeof(TGAheader)) != 0)
		return FALSE;

	// Determine the TGA width and height (highbyte*256+lowbyte)
	int width  = top->header[1] * 256 + top->header[0];
	int height = top->header[3] * 256 + top->header[2];

	// Check to make sure the targa is valid and is 24 bit or 32 bit
	if (width   <=0 ||										// Is the width less than or equal to zero
		height  <=0 ||										// Is the height less than or equal to zero
		(top->header[4] != 24 && top->header[4] != 32))		// Is it 24 or 32 bit?
	{
		// If anything didn't check out then close the file and return
		return FALSE;
	}

	bpp				= top->header[4];						// Grab the bits per pixel
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

	// Set the type if 24 bits (8x3), in place of 32 bits (8x3 + alpha)
	if (bpp == 24)
		type = GL_RGB;

	// Generate the OpenGL texture id
	glGenTextures(1, &texture[num_texture]);

	// Bind this texture to its id
	glBindTexture(GL_TEXTURE_2D, texture[num_texture]);

	// Use mipmapping filter
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S ,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T ,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);	// Trilinear filtering
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);					// Bilinear filtering

	// Generate the mipmaps
	gluBuild2DMipmaps(GL_TEXTURE_2D, type, width, height, type, GL_UNSIGNED_BYTE, imageData);

	// Cleanup
	free(imageData);
	//	free(buffer);
	//	free(top);

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////////////////
//
// LOAD TEXTURE (TGA) FROM FILE
//
//
/*
bool LoadTextureTGA_fromFile(char *filename, USHORT resource_id, int num_texture)
{    
	texture[i] = &texture[0]
	GLubyte		TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};	// Uncompressed TGA Header
	GLubyte		TGAcompare[12];								// Used To Compare TGA Header
	GLubyte		header[6];									// First 6 Useful Bytes From The Header
	GLuint		bytesPerPixel;								// Holds Number Of Bytes Per Pixel Used In The TGA File
	GLuint		imageSize;									// Used To Store The Image Size When Setting Aside Ram
	GLuint		temp;										// Temporary Variable
	GLuint		type=GL_RGBA;								// Set The Default GL Mode To RBGA (32 BPP)

	FILE *file = fopen(filename, "rb");						// Open The TGA File

	if(	file==NULL ||										// Does File Even Exist?
		fread(TGAcompare,1,sizeof(TGAcompare),file)!=sizeof(TGAcompare) ||	// Are There 12 Bytes To Read?
		memcmp(TGAheader,TGAcompare,sizeof(TGAheader))!=0				||	// Does The Header Match What We Want?
		fread(header,1,sizeof(header),file)!=sizeof(header))				// If So Read Next 6 Header Bytes
	{
		if (file == NULL)									// Did The File Even Exist? *Added Jim Strong*
			return false;									// Return False
		else
		{
			fclose(file);									// If Anything Failed, Close The File
			return false;									// Return False
		}
	}

	texture->width  = header[1] * 256 + header[0];			// Determine The TGA Width	(highbyte*256+lowbyte)
	texture->height = header[3] * 256 + header[2];			// Determine The TGA Height	(highbyte*256+lowbyte)
    
 	if(	texture->width	<=0	||								// Is The Width Less Than Or Equal To Zero
		texture->height	<=0	||								// Is The Height Less Than Or Equal To Zero
		(header[4]!=24 && header[4]!=32))					// Is The TGA 24 or 32 Bit?
	{
		fclose(file);										// If Anything Failed, Close The File
		return false;										// Return False
	}

	texture->bpp	= header[4];							// Grab The TGA's Bits Per Pixel (24 or 32)
	bytesPerPixel	= texture->bpp/8;						// Divide By 8 To Get The Bytes Per Pixel
	imageSize		= texture->width*texture->height*bytesPerPixel;	// Calculate The Memory Required For The TGA Data

	texture->imageData=(GLubyte *)malloc(imageSize);		// Reserve Memory To Hold The TGA Data

	if(	texture->imageData==NULL ||							// Does The Storage Memory Exist?
		fread(texture->imageData, 1, imageSize, file)!=imageSize)	// Does The Image Size Match The Memory Reserved?
	{
		if(texture->imageData!=NULL)						// Was Image Data Loaded
			free(texture->imageData);						// If So, Release The Image Data

		fclose(file);										// Close The File
		return false;										// Return False
	}

	for(GLuint i=0; i<int(imageSize); i+=bytesPerPixel)		// Loop Through The Image Data
	{														// Swaps The 1st And 3rd Bytes ('R'ed and 'B'lue)
		temp=texture->imageData[i];							// Temporarily Store The Value At Image Data 'i'
		texture->imageData[i] = texture->imageData[i + 2];	// Set The 1st Byte To The Value Of The 3rd Byte
		texture->imageData[i + 2] = temp;					// Set The 3rd Byte To The Value In 'temp' (1st Byte Value)
	}

	fclose (file);											// Close The File

	// Build A Texture From The Data
	glGenTextures(1, &texture[0].texID);					// Generate OpenGL texture IDs

	glBindTexture(GL_TEXTURE_2D, texture[0].texID);			// Bind Our Texture
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtered
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// Linear Filtered
	
	if (texture[0].bpp==24)									// Was The TGA 24 Bits
	{
		type=GL_RGB;										// If So Set The 'type' To GL_RGB
	}

	glTexImage2D(GL_TEXTURE_2D, 0, type, texture[0].width, texture[0].height, 0, type, GL_UNSIGNED_BYTE, texture[0].imageData);

	return true;											// Texture Building Went Ok, Return True
}

*/


//////////////////////////////////////////////////////////////////////////////////////
//
// LOAD TEXTURE (PNG)  Ce format supporte la composante alpha : 255 valeurs par couleur
//                     en 24 bits (RGBA) alors que le GIF ne supporte que 2 valeurs
//                     (transparent ou opaque).
//
//                     A utiliser pour l'antialiasing des contours de textures, différents
//                     degrés de transparence dans une même texture, des effets d'ombres...
//
//                     Les dimensions en puissances de 2 évitent le redimensionnement
//                     si vous utilisez la texure pour de la 2D orthogonale (on-screen
//                     display, HUD) Pour ceci mettez votre graphique en bas à gauche de
//                     la texture et ce qui est en trop en transparent.
//
//                     Voir la fonction display_energy() pour un exemple d'affichage.
//
BOOL LoadTexturePNG(char* file, USHORT resource_id, int num_texture)
{
	// from resource
	if (file == NULL && resource_id != NULL)
	{
		// find it
		HRSRC	hRes = FindResource(NULL, MAKEINTRESOURCE(resource_id), "PNG");
		if (hRes == NULL) return FALSE;

		// open it
		DWORD rsize  = SizeofResource(NULL, hRes);
		HGLOBAL hMem = LoadResource(NULL, hRes);
		if (hMem == NULL) return FALSE;
			
		LPVOID lp = LockResource(hMem);
		if (lp == NULL)	return FALSE;

		// create a temporary file
		FILE * pFile;
		pFile = tmpfile();
		if (pFile == NULL) return FALSE;

		// copy the resource in the temporary file
		fwrite(lp, rsize, 1, pFile);
		fseek(pFile, 0, SEEK_SET);

		// load the temporary file as a normal PNG image
		texture[num_texture] = pngBindF(pFile, PNG_NOMIPMAP, PNG_ALPHA, NULL, GL_CLAMP, GL_LINEAR, GL_LINEAR);
		if (texture[num_texture] == 0)
			BOX("erreur loading png");

		fclose (pFile);
	}
	
	// from file
	else if (file != NULL)
	{
		// load it
		texture[num_texture] = pngBind(file, PNG_NOMIPMAP, PNG_ALPHA, NULL, GL_CLAMP, GL_LINEAR, GL_LINEAR);
		if (texture[num_texture] == 0)
			BOX("erreur loading png");
	}

	return TRUE;
}




