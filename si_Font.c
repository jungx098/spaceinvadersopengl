/*  glTexFont.c

    glTexFont Library R6

    Copyright (c) 1999 Nate Miller
    
    Notice: Usage of any code in this file is subject to the rules
    described in the LICENSE.TXT file included in this directory.
    Reading, compiling, or otherwise using this code constitutes
    automatic acceptance of the rules in said text file.

    File        -- glTexFont.c
    Date        -- 5/30/99
    Author      -- Nate 'm|d' Miller
    Contact     -- vandals1@home.com
    Web         -- http://members.home.com/vandals1
*/
//#define FONT_LIB_DEBUG /* un-comment for debug regions */
#include "si_Font.h"
//#include "glTexFont.h"
//#include "glTexFontTga.h"
//#include "glTexFontDefs.h"
//#include "glTexFontInclude.h"

/* just a global for our default colors */
float white[] = {1.0, 1.0, 1.0, 1.0};
float gray[] = {0.5, 0.5, 0.5, 1.0};

/* the font structure we will use in the library, all values are defaults */
texFont_t font = 
{
    {1.0, 1.0, 1.0, 1.0}, /* white text */
    {0.5, 0.5, 0.5, 1.0}, /* gray gradient */
    {0.5, 0.5, 0.5, 1.0}, /* gray shadow */
    12,                   /* size 12 font */
    0,                    /* no shadow */
    0,                    /* no gradient */
    0,                    /* italic amount */
    0,                    /* no bold */
    0,0,0,0,0,            /* text region stuff */
    0.0,0.0,              /* texture inc stuff */
    FONT_BLOCK_ROW,       /* characters per row */
    FONT_BLOCK_COL,       /* characters por col */
    0,                    /* texture id */
};

/*
    tPoints contains our map for the font image.  After fontMakeMap this array
    will contain all the information to get the proper key from the font map.
*/
vec2_t tPoints[257]; 

/*
=============
fontLoad

Load the passed image name and use it for the font.  
=============
*/
int fontLoad (char *name)
{
    glGenTextures (1, &font.texId);

	font.blockRow = FONT_BLOCK_ROW;
	font.blockCol = FONT_BLOCK_COL;

	fontMakeMap ();

    return fontLoadTGA (name, font.texId);
}
/*
=============
fontLoadEx

Loads the passed image which has 'col' characters per column and 'row' 
characters per row. 
=============
*/
int fontLoadEx (char *name, int row, int col)
{
    /* make sure we have valid dimensions */
	if (row * col != 256)
		return 0;

    glGenTextures (1, &font.texId);

	font.blockRow = row;
	font.blockCol = col;

	fontMakeMap ();

    return fontLoadTGA (name, font.texId);
}
/*
=============
fontDrawChar

Draws a character that is 'size' pixels in w and h.  
=============
*/
void fontDrawChar (char c, int x, int y, int size, int shadow)
{
    if (!font.gradient && !shadow)
        glColor4fv (font.fgColor);
    else if (!font.gradient && shadow)
        glColor4fv (font.bgColor);
    else if (font.gradient && !shadow)
        glColor4fv (font.gdColor);

    glBegin (GL_QUADS);
		glTexCoord2f (tPoints[(int)c][0], tPoints[(int)c][1]);
		glVertex2i (x, y);

    	glTexCoord2f (tPoints[(int)c][0] + font.tIncX, tPoints[(int)c][1]);
		glVertex2i (x + size, y);

        if (!shadow)
            glColor4fv (font.fgColor);
        else glColor4fv (font.bgColor);

        glTexCoord2f (tPoints[(int)c][0] + font.tIncX, tPoints[(int)c][1] + font.tIncY);
		glVertex2i (x + size + font.italic, y + size);

        glTexCoord2f (tPoints[(int)c][0], tPoints[(int)c][1] + font.tIncY);
		glVertex2i (x + font.italic, y + size);
	glEnd ();
}
/*
=============
fontScissorNormal

Normal scissor region for text rendering.  
=============
*/
void fontScissorNormal (int xpos, int ypos, int tabs, int carrage, int size, int len)
{
    int sy;
    int ex;
    int ey;

    ex = len * size * tabs;

    if (carrage)
	{
		sy = ypos - (size * carrage);
		ey = size * (carrage + 1);
	} else {
		sy = ypos;
		ey = size;
	}
    glScissor (xpos, sy, ex, ey);
}
/*
=============
fontScissorTextRegion

Scissor region used setup for a text region.
=============
*/
void fontScissorTextRegion (void)
{
    glScissor (font.regionX, font.regionY, font.regionW, font.regionH);
}
/*
=============
fontFgColorReset

Resets the font color.
=============
*/
void fontForeColorReset (void)
{
    fontColorCopy (white, font.fgColor);
}
/*
=============
fontBgColorReset

Resets the shadow color.
=============
*/
void fontShadowColorReset (void)
{
    fontColorCopy (gray, font.bgColor);
}
/*
=============
fontGdColorReset

Resets the gradient color.
=============
*/
void fontGradientColorReset (void)
{
    fontColorCopy (gray, font.gdColor);
}
/*
=============
fontReset

Resets the font.  Only resets variables that could possible change.
=============
*/
void fontReset (void)
{
    font.size = 12;
    font.shadow = 0;
    font.region = 0;
    font.gradient = 0;
    font.italic = 0;
    font.bold = 0;
    font.regionX = 0;
    font.regionY = 0;
    font.regionW = 0;
    font.regionH = 0;
    fontForeColorReset ();
    fontShadowColorReset ();
    fontGradientColorReset ();
}
/*
=============
fontRegion

Sets up a font region.  Only good for one fontDrawString. 
=============
*/
void fontRegion (int x, int y, int w, int h)
{
    font.region = 1;
    font.regionX = x;
    font.regionY = y - h;
    font.regionW = w;
    font.regionH = h;
}
/*
=============
fontSize

Sets the font size.
=============
*/
void fontSize (int size)
{
    font.size = size;
}
/*
=============
fontShadow

Draws a shadow if called.
=============
*/
void fontShadow (void)
{
    font.shadow = 1;
}
/*
=============
fontGradient

Draws gradient text if called.
=============
*/
void fontGradient (void)
{
    font.gradient = 1;
}
/*
=============
fontRenderChar

Draws a character to the screen
Bold is just a hack, nothing special
=============
*/
void fontRenderChar (char c, int x, int y, int size)
{
    if (font.shadow)
    {
        if (!font.bold)
	        fontDrawChar (c, x + 1,  y + 1, size, 1);
        else fontDrawChar (c, x + 2,  y + 1, size, 1);
    }
    
    fontDrawChar (c, x, y, size, 0);

    if (font.bold)
        fontDrawChar (c, x + 1, y, size, 0);
}
/*
=============
fontSlashParser

Handles all the fun that comes with a \\, returns amount to advance string.
After this funtion *buffptr ++ will be the next character to draw or parse.
=============
*/
int fontSlashParser (char *buffPtr, int *x, int *y)
{
    int ret = 0;

    *buffPtr ++;

    if (!*buffPtr)
        return ret;

    switch (*buffPtr)
    {
        case 'a':
        case 'c':
            *x -= font.size;
            return fontSetColorFromToken (buffPtr);
        break;
        case 'i':
            *x -= font.size;
            return fontItalicsMode (buffPtr);
        break;
        case 'b':
            *x -= font.size;
            return fontBoldMode (buffPtr);
        break;
        default:
            *buffPtr --;
            fontRenderChar (*buffPtr, *x, *y, font.size);
            return ret;
        break;
    }
}
/*
=============
fontWalkString

Does the actual rendering of our string.  
=============
*/
void fontWalkString (char *buffPtr, int xpos, int ypos, int *vPort)
{
    int size = font.size;
	int x = xpos;
	int y = ypos;
	int carrage = 0;
	int tabs = 0;
    int len = strlen (buffPtr);
    int xMax;

    xMax = vPort[0] + vPort[2];

    carrage = fontGetCharHits (buffPtr, '\n');
	tabs = fontGetCharHits (buffPtr, '\t');

	if (!tabs)
		tabs = 1;
	else tabs *= FONT_TAB_SPACE;

    if (font.region)
    {
        fontScissorTextRegion ();
        x = font.regionX;
        y = (font.regionY + font.regionH) - font.size;
    } else fontScissorNormal (xpos, ypos, tabs, carrage, font.size, len);

#ifdef FONT_LIB_DEBUG
    glClearColor (1,0,1,1);
    glClear (GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
#endif

    /* lets draw! */
	for ( ; *buffPtr; *buffPtr ++, x += size)
	{
		if (x > xMax)
			break;
        if (font.region)
        {
            if (x + size > (font.regionX + font.regionW))
            {
                y -= size;
                x = font.regionX;
            }
            if (y < font.regionY)
                break;
        }
		switch (*buffPtr)
		{
			case '\n':
				y -= size;
                x = xpos - size;
				continue; 
			break;
			case '\t':
				x += (size * FONT_TAB_SPACE);
				continue; 
			break;
            case '\\':
                buffPtr += fontSlashParser (buffPtr, &x, &y);
                if (*buffPtr == '\n' || *buffPtr == '\t')
                {
                    buffPtr -= 1;
                    continue;
                }
            break;
            default :
                fontRenderChar (*buffPtr, x, y, size);
            break;
		}
	}
}
/*
=============
fontDrawString

Renders a string at xpos, ypos.
=============
*/
void fontDrawString (int xpos, int ypos, const char *s, ...)
{
	va_list	msg;
    char buffer[FONT_MAX_LEN] = {'\0'};
    int vPort[4];
	
	va_start (msg, s);
#ifdef _WIN32
	_vsntprintf (buffer, FONT_MAX_LEN - 1, s, msg);	
#else /* linux */
	vsnprintf (buffer, FONT_MAX_LEN - 1, s, msg);	
#endif
	va_end (msg);

    /* get current viewport */
    glGetIntegerv (GL_VIEWPORT, vPort);
    /* setup various opengl things that we need */
	fontSetModes (FONT_GET_MODES);
    
	glMatrixMode (GL_PROJECTION);
	glPushMatrix ();
	glLoadIdentity ();

    glOrtho (0, vPort[2], 0, vPort[3], -1, 1);
	glMatrixMode (GL_MODELVIEW);
	glPushMatrix ();
	glLoadIdentity ();
	glBindTexture (GL_TEXTURE_2D, font.texId);

    /* draw the string */
    fontWalkString (buffer, xpos, ypos, vPort);
	
	glMatrixMode (GL_PROJECTION);
	glPopMatrix ();
	glMatrixMode (GL_MODELVIEW);
	glPopMatrix ();

	fontSetModes (FONT_RESTORE_MODES);
    fontReset ();
}
/*
=============
fontSetColorFromToken

Grabs a color token from a buffer and sets color.  
=============
*/	
int fontSetColorFromToken (char *s)
{
	int clr[4];
	int ret = 1;

	if (*s == 'c')
	{
		s += 1;
		if (sscanf (s, "(%d %d %d)", &clr[0], &clr[1], &clr[2]) != 3)
			return -1;
		fontColor (clr[0] * FONT_ITOF, clr[1] * FONT_ITOF, clr[2] * FONT_ITOF);
	} else if (*s == 'a')
	{
		s += 1;
		if (sscanf (s, "(%d %d %d %d)", &clr[0], &clr[1], &clr[2], &clr[3]) != 4)
			return -1;
		fontColorA (clr[0] * FONT_ITOF, clr[1] * FONT_ITOF, clr[2] * FONT_ITOF, clr[3] * FONT_ITOF);
	}

	while (*s != ')' && ret ++)
		s += 1;

    return ret + 1;
}
/*
=============
fontItalicsMode

Either turns on or off italics.
=============
*/	
int fontItalicsMode (char *s)
{
    s += 1;

    if (*s == '+')
        font.italic = FONT_ITALIC;
    else if (*s == '-')
        font.italic = 0;
    else return -1;

    return 2;
}
/*
=============
fontBoldMode

Either turns on or off bold.
=============
*/	
int fontBoldMode (char *s)
{
    s += 1;

    if (*s == '+')
        font.bold = 1;
    else if (*s == '-')
        font.bold = 0;
    else return -1;

    return 2;
}
/*
=============
fontGetCharHits

Returns number of times a character is found in a buffer.
=============
*/
int fontGetCharHits (char *s, char f)
{
	int hits = 0;

	while (*s)
	{
		if (*s == f)
			hits ++;
		*s ++;
	}

	return hits;
}
/*
=============
fontMakeMap

Makes the font map which allows the correct characters to be drawn.
=============
*/
void fontMakeMap (void)
{
#define VCOPY(d,x,y) {d[0] = x; d[1] = y;}
	int i = 0;
	float x, y;


	font.tIncX = (float)pow (font.blockCol, -1);
	font.tIncY = (float)pow (font.blockRow, -1);

	
	for (y = 1 - font.tIncY; y >= 0; y -= font.tIncY)
		for (x = 0; x <= 1 - font.tIncX; x += font.tIncX, i ++)
			VCOPY (tPoints[i], x, y);
#undef VCOPY
}
/*
=============
fontSetModes

Sets or restores OpenGL modes that we need to use.  Here to prevent the drawing
of text from messing up other stuff.  
=============
*/
void fontSetModes (int state)
{
	static int matrixMode;
	static int polyMode[2];
	static int lightingOn;
	static int blendOn;
	static int depthOn;
	static int textureOn;
	static int scissorOn;
	static int blendSrc;
	static int blendDst;

	/* grab the modes that we might need to change */
	if (state == FONT_GET_MODES)
	{
		glGetIntegerv(GL_POLYGON_MODE, polyMode);

		if (polyMode[0] != GL_FILL)
			glPolygonMode (GL_FRONT, GL_FILL);
		if (polyMode[1] != GL_FILL)
			glPolygonMode (GL_BACK, GL_FILL);
	
		textureOn = glIsEnabled (GL_TEXTURE_2D);
		if (!textureOn)
			glEnable (GL_TEXTURE_2D);

		depthOn = glIsEnabled (GL_DEPTH_TEST);

		if (depthOn)
			glDisable (GL_DEPTH_TEST);

		lightingOn= glIsEnabled (GL_LIGHTING);        
	
		if (lightingOn) 
			glDisable(GL_LIGHTING);

		scissorOn= glIsEnabled (GL_SCISSOR_TEST);        
	
		if (!scissorOn) 
			glEnable (GL_SCISSOR_TEST);

		glGetIntegerv(GL_MATRIX_MODE, &matrixMode); 
	
		/* i don't know if this is correct */
		blendOn= glIsEnabled (GL_BLEND);        
        glGetIntegerv (GL_BLEND_SRC, &blendSrc);
	    glGetIntegerv (GL_BLEND_DST, &blendDst);
		if (!blendOn)
			glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	} else if (state == FONT_RESTORE_MODES)
	{
		/* put everything back where it was before */
		if (polyMode[0] != GL_FILL)
			glPolygonMode (GL_FRONT, polyMode[0]);
		if (polyMode[1] != GL_FILL)
			glPolygonMode (GL_BACK, polyMode[1]);
	
		if (lightingOn)
			glEnable(GL_LIGHTING);
	
		/* i don't know if this is correct */
		if (!blendOn)
        {
			glDisable (GL_BLEND);
            glBlendFunc (blendSrc, blendDst);
        } else glBlendFunc (blendSrc, blendDst);

		if (depthOn)
			glEnable (GL_DEPTH_TEST);

		if (!textureOn)
			glDisable (GL_TEXTURE_2D);
		
		if (!scissorOn) 
			glDisable (GL_SCISSOR_TEST);
	
		glMatrixMode (matrixMode);
	}
}

/*  glTexFontColor.c

    Copyright (c) 1999 Nate Miller
    
    Notice: Usage of any code in this file is subject to the rules
    described in the LICENSE.TXT file included in this directory.
    Reading, compiling, or otherwise using this code constitutes
    automatic acceptance of the rules in said text file.

    File        -- glTexFontColor.c
    Date        -- 6/10/99
    Author      -- Nate 'm|d' Miller
    Contact     -- vandals1@home.com
    Web         -- http://members.home.com/vandals1
*/
//#include "glTexFont.h"

/* color functions must be defined in texFont.h */

extern texFont_t font;

/*
=============
fontColor, fontColorA, fontColorp, fontColorAp
   
Sets the currect color for the text. 
=============
*/
void fontColor (float r, float g, float b)
{
    font.fgColor[0] = r;
    font.fgColor[1] = g;
    font.fgColor[2] = b;
    font.fgColor[3] = 1.0;
}
void fontColorA (float r, float g, float b, float a)
{
    font.fgColor[0] = r;
    font.fgColor[1] = g;
    font.fgColor[2] = b;
    font.fgColor[3] = a;
}
void fontColorp (float *clr)
{
    font.fgColor[0] = clr[0];
    font.fgColor[1] = clr[1];
    font.fgColor[2] = clr[2];
    font.fgColor[3] = 1.0;
}
void fontColorAp (float *clr)
{
    font.fgColor[0] = clr[0];
    font.fgColor[1] = clr[1];
    font.fgColor[2] = clr[2];
    font.fgColor[3] = clr[3];
}
/*
=============
fontShadow, fontShadowA, fontShadowp, fontShadowAp
   
Sets the currect shadow color for the text. 
=============
*/
void fontShadowColor (float r, float g, float b)
{
    font.bgColor[0] = r;
    font.bgColor[1] = g;
    font.bgColor[2] = b;
    font.bgColor[3] = 1.0;
}
void fontShadowColorA (float r, float g, float b, float a)
{
    font.bgColor[0] = r;
    font.bgColor[1] = g;
    font.bgColor[2] = b;
    font.bgColor[3] = a;
}
void fontShadowColorp (float *clr)
{
    font.bgColor[0] = clr[0];
    font.bgColor[1] = clr[1];
    font.bgColor[2] = clr[2];
    font.bgColor[3] = 1.0;
}
void fontShadowColorAp (float *clr)
{
    font.bgColor[0] = clr[0];
    font.bgColor[1] = clr[1];
    font.bgColor[2] = clr[2];
    font.bgColor[3] = clr[3];
}
/*
=============
fontGradientColor, fontGradientColorA, fontGradientColorp, fontGradientColorAp
   
Sets the currect gradient color for the text. 
=============
*/
void fontGradientColor (float r, float g, float b)
{
    font.gdColor[0] = r;
    font.gdColor[1] = g;
    font.gdColor[2] = b;
    font.gdColor[3] = 1.0;
}
void fontGradientColorA (float r, float g, float b, float a)
{
    font.gdColor[0] = r;
    font.gdColor[1] = g;
    font.gdColor[2] = b;
    font.gdColor[3] = a;
}
void fontGradientColorp (float *clr)
{
    font.gdColor[0] = clr[0];
    font.gdColor[1] = clr[1];
    font.gdColor[2] = clr[2];
    font.gdColor[3] = 1.0;
}
void fontGradientColorAp (float *clr)
{
    font.gdColor[0] = clr[0];
    font.gdColor[1] = clr[1];
    font.gdColor[2] = clr[2];
    font.gdColor[3] = clr[3];
}

/*  glTexFontTGA.c

    Copyright (c) 1999 Nate Miller
    
    Notice: Usage of any code in this file is subject to the rules
    described in the LICENSE.TXT file included in this directory.
    Reading, compiling, or otherwise using this code constitutes
    automatic acceptance of the rules in said text file.

    File        -- glTexFontTGA.c
    Date        -- 5/30/99
    Author      -- Nate 'm|d' Miller
    Contact     -- vandals1@home.com
    Web         -- http://members.home.com/vandals1

    Change Log
    **********
    6/11/99 - added support for 8bit images, changed commenting
    5/30/99 - original file
*/
//#include "glTexFont.h"
//#include "glTexFontTGA.h"
//#include "glTexFontInclude.h"

GLenum texFormat;

/*
=============
fontCheckSize

Make sure its a power of 2.
=============
*/
int fontCheckSize (int x)
{
	if (x == 2	 || x == 4 || 
		x == 8	 || x == 16 || 
		x == 32  || x == 64 ||
		x == 128 || x == 256 || x == 512)
		return 1;
	else return 0;
}
/*
=============
fontGetRGBA

Reads in RGBA data for a 32bit image. 
=============
*/
unsigned char *fontGetRGBA (FILE *s, int size)
{
	unsigned char *rgba;
	unsigned char temp;
	int bread;
	int i;

	rgba = malloc (size * 4); 

	if (rgba == NULL)
		return 0;

	bread = fread (rgba, sizeof (unsigned char), size * 4, s); 
	
    /* TGA is stored in BGRA, make it RGBA */
	if (bread != size * 4)
	{
		free (rgba);
		return 0;
	}

	for (i = 0; i < size * 4; i += 4 )
	{
		temp = rgba[i];
		rgba[i] = rgba[i + 2];
		rgba[i + 2] = temp;
	}

	texFormat = GL_RGBA;
	return rgba;
}
/*
=============
fontGetRGB

Reads in RGB data for a 24bit image. 
=============
*/
unsigned char *fontGetRGB (FILE *s, int size)
{
	unsigned char *rgb;
	unsigned char temp;
	int bread;
	int i;

	rgb = malloc (size * 3); 
	
	if (rgb == NULL)
		return 0;

	bread = fread (rgb, sizeof (unsigned char), size * 3, s);

	if (bread != size * 3)
	{
		free (rgb);
		return 0;
	}

    /* TGA is stored in BGR, make it RGB */
	for (i = 0; i < size * 3; i += 3)
	{
		temp = rgb[i];
		rgb[i] = rgb[i + 2];
		rgb[i + 2] = temp;
	}
	
	texFormat = GL_RGB;

	return rgb;
}
/*
=============
fontGetGray

Reads a gray scale image. 
=============
*/
unsigned char *fontGetGray (FILE *s, int size)
{
	unsigned char *grayData;
	int bread;

	grayData = malloc (size);

	if (grayData == NULL)
		return 0;

	bread = fread (grayData, sizeof (unsigned char), size, s);

	if (bread != size)
	{
		free (grayData);
		return 0;
	}

	{
		unsigned char *expanded = malloc (size * 2);
		int i;
		if (expanded == NULL)
		{
			free (grayData);
			return 0;
		}
		for (i = 0; i < size; i++)
		{
			expanded[i * 2] = grayData[i];
			expanded[i * 2 + 1] = grayData[i];
		}
		free (grayData);
		grayData = expanded;
	}

	texFormat = GL_LUMINANCE_ALPHA;

	return grayData;
}
/*
=============
fontGetData

Gets the image data for the specified bit depth.
=============
*/
char *fontGetData (FILE *s, int sz, int iBits)
{
	if (iBits == 32)
		return fontGetRGBA (s, sz);
	else if (iBits == 24)
		return fontGetRGB (s, sz);	
    else if (iBits == 8)
        return fontGetGray (s, sz);
	else return NULL;
}
/*
=============
fontLoadTGA

Loads up a targa file.  Supported types are 8,24 and 32 uncompressed images.  
=============
*/
int fontLoadTGA (char *name, int id)
{
	unsigned char type[4];
	unsigned char info[7];
	unsigned char *imageData = NULL;
	int imageWidth, imageHeight;
	int imageBits, size;
	FILE *s;
	
	if (!(s = fopen (name, "r+bt")))
		return FONT_FILE_NOT_FOUND;

	fread (&type, sizeof (char), 3, s); // read in colormap info and image type, byte 0 ignored
	fseek (s, 12, SEEK_SET);			// seek past the header and useless info
	fread (&info, sizeof (char), 6, s);

	if (type[1] != 0 || (type[2] != 2 && type[2] != 3))
		return FONT_BAD_IMAGE_TYPE;
    
    imageWidth = info[0] + info[1] * 256; 
	imageHeight = info[2] + info[3] * 256;
	imageBits =	info[4]; 

	size = imageWidth * imageHeight; 

	/* make sure dimension is a power of 2 */
	if (!fontCheckSize (imageWidth) || !fontCheckSize (imageHeight))
		return FONT_BAD_DIMENSION;

	/* make sure we are loading a supported type */
	if (imageBits != 32 && imageBits != 24 && imageBits != 8)
		return FONT_BAD_BITS;

	imageData = fontGetData (s, size, imageBits);
	
	fclose (s);

	/* no image data */
	if (imageData == NULL)
		return FONT_BAD_DATA;

	glBindTexture (GL_TEXTURE_2D, id);
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	/* glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); */
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	/* glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); */
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D (GL_TEXTURE_2D, 0, texFormat, imageWidth, imageHeight, 0, texFormat, GL_UNSIGNED_BYTE, imageData);

	/* release data, its been uploaded */
	free (imageData);

	return 1;
}

