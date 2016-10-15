// si_Basecode.h - space invaders opengl

#ifndef _BASECODE_H
#define _BASECODE_H

#include <Windows.h>

//////////////////////////////////////////////////////////////////////////////////////
//
// BASECODE => Application, fenêtre, etc
//
//

typedef struct {									// Structure For Keyboard Stuff
	BOOL keyDown [256];								// Holds TRUE / FALSE For Each Key
	BOOL wivik [256];								// Wivik virtual keyboard
	BOOL no_wivik [256];							// Wivik virtual keyboard
} Keys;												// Keys

typedef struct {									// Contains Information Vital To Applications
	HINSTANCE		hInstance;						// Application Instance
	const char*		className;						// Application ClassName
} Application;										// Application

typedef struct {									// Window Creation Info
	Application*		application;				// Application Structure
	char*				title;						// Window Title
	int					width;						// Width
	int					height;						// Height
	int					bitsPerPixel;				// Bits Per Pixel
	BOOL				isFullScreen;				// FullScreen?
	BOOL				quickFullScreen;			// quickFullScreen?
} GL_WindowInit;									// GL_WindowInit

typedef struct {									// Contains Information Vital To A Window
	Keys*				keys;						// Key Structure
	HWND				hWnd;						// Window Handle
	HDC					hDC;						// Device Context
	HGLRC				hRC;						// Rendering Context
	GL_WindowInit		init;						// Window Init
	BOOL				isVisible;					// Window Visible?
} GL_Window;										// GL_Window

//////////////////////////////////////////////////////////////////////////////////////
//
// BASECODE => Fonctions
//
//

// FONCTIONS PRINCIPALES
BOOL Initialize (GL_Window* window, Keys* keys);	// Performs All Your Initialization
void Deinitialize (void);							// Performs All Your DeInitialization
void Update (DWORD milliseconds, GL_Window* window);// Perform Motion Updates
void Draw (GL_Window* window);						// Perform All Your Scene Drawing
void GameLogic (GL_Window* window);					// Perform Game Logic
void TerminateApplication (GL_Window* window);		// Terminate The Application
void ToggleFullscreen (GL_Window* window);			// Toggle Fullscreen / Windowed Mode

// SCENE ROTATION
extern bool moving; 
extern double startx, starty;						// Rotation à l'aide de la souris
extern double angle_y;
extern double angle_x;		                        // The Current Position Of The Mouse

// CHARGEMENT
void loading (void *dummy);							// CHARGEMENT DES ELEMENTS
void loading_display1 (void);						// CHARGEMENT => FADING IN
void loading_display2 (void);						// CHARGEMENT => LOG SYSTEM
void loading_display3 (void);						// CHARGEMENT => FADING OUT

// FENETRAGE
void ReshapeGL (int width, int height);
BOOL ChangeScreenResolution(int width, int height, int bitsPerPixel);
BOOL DestroyWindowGL(GL_Window* window);
void ReturnSystemMetrics(int *Width, int *Height, int *BitsPerPixel);
void Fullscreen(void);
void DialogTransparency(void *dummy);

#endif												// _BASECODE_H
