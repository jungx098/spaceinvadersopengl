// si_Basecode.cpp - space invaders opengl

/***********************************************
*    Jeff Molofee's Revised OpenGL Basecode    *
*  Huge Thanks To Maxwell Sayles & Peter Puck  *
*            http://nehe.gamedev.net           *
*                     2001                     *
***********************************************/

// This file handles : - the program entry (WinMain() function)
//                     - the main loop
//                     - the Windows messages
//                     - the dialog boxes (startup, about, streaming URL)

#include <windows.h>													// Header File For The Windows Library
#include <Wininet.h>													// Header File For Internet Library
#include <gl/gl.h>														// Header File For The OpenGL32 Library
#include <gl/glu.h>														// Header File For The OpenGL Utility Library
#include "si_Basecode.h"												// Header File For The NeHeGL Basecode
#include "si_Main.h"													// Header File For Main Global Variables
#include "commctrl.h"													// Header File For Windows XP Common Controls
#include "si_Functions.h"												// Header File For The Game Functions
#include "resource.h"													// Header File For the resources ids

#define WM_TOGGLEFULLSCREEN (WM_USER+1)									// Application Define Message For Toggling Fullscreen

// ALPHA LAYERED WINDOW (WINDOW TRANSPARENCY)
typedef BOOL (WINAPI *SLWA_FUNC)(HWND hwnd,COLORREF crKey,BYTE bAlpha,DWORD dwFlags);
//bool first_pass = true;
//DWORD start, actual;
//BYTE alpha=0;

// DIALOG BOXES HANDLES
HWND			g_hwndDlg;												// Startup DialogBox Handle
HWND			g_hwndDlgAbout;											// About DialogBox Handle

// MISC.
bool			equalizer_moving			= false;					// Equalizer en train d'être déplacé ?
bool			dialogbox_internet_over		= false;					// Bouton Internet (mouse over)
extern bool		game_spectrum;											// Spectrum activated ?
HCURSOR			cur_hand, cur_hand_closed;								// Handles Of Cursors
bool			custom_streaming_dialogbox	= false;					// for playing song when enter 'about' but not 'custom stream'
COLORREF textcolor2;
bool			AntialiasingMode_over		= false;

// TOOLTIPS
HWND g_hwndTT;															// The window which handles tooltips
TOOLINFO ti;															// The toolinfo structure (for tooltips properties)
WNDPROC OrigControlProc, OrigAntiAliasingModeProc;						// To return the unprocessed messages in the procedure of the subclassed controls

// GAME PARAMETERS (STEP, LOADING)
extern int		game_step;												// Etape du jeu (chargement etc.)
extern bool		Fps2_firstpass;											// FPS Méthode n°2
bool			game_loading				= false;					// To make the Loading Thread called only once
bool			game_loading_finished		= true;

// THREADS
unsigned long	h_thread2;												// Handle Of The Loading Thread
bool			my_mod_about_on				= false;					// Music For About-DialogBox
bool			my_mod_about_working		= false;					// Music For About-DialogBox

// PROGRAM
static BOOL		g_isProgramLooping;										// Window Creation Loop, For FullScreen/Windowed Toggle																		// Between Fullscreen / Windowed Mode
static BOOL		g_createFullScreen;										// If true, Then Create Fullscreen
GL_Window		window;												    // Window Structure

// TERMINATE APPLICATION
void TerminateApplication(GL_Window* window)							// Terminate The Application
{
	PostMessage(window->hWnd, WM_QUIT, 0, 0);							// Send A WM_QUIT Message
	g_isProgramLooping = FALSE;											// Stop Looping Of The Program
}

// SWITCH FULLSCREEN / FENETRE
void ToggleFullscreen(GL_Window* window)								// Toggle Fullscreen/Windowed
{
	PostMessage(window->hWnd, WM_TOGGLEFULLSCREEN, 0, 0);				// Send A WM_TOGGLEFULLSCREEN Message
}

// REPLACE CAMERA
void ReshapeGL(int width, int height)									// Reshape The Window When It's Moved Or Resized
{
	extern float ciblex, cibley, zoom_total;
	glViewport(0, 0, (GLsizei)(width), (GLsizei)(height));				// Reset The Current Viewport
	glMatrixMode(GL_PROJECTION);										// Select The Projection Matrix
	glLoadIdentity();													// Reset The Projection Matrix
	gluPerspective(45.0f, (GLfloat)(width)/ (GLfloat)(height),			// Calculate The Aspect Ratio Of The Window
					0.1f, 500.0f);

	pcamera.x = pcamera.y = ciblex = cibley = zoom_total = 0;
	pcamera.z = 20;

	!game_3d ? angle_y = 0 : angle_y = -40;								// si pas game_3d angle_y=0 sinon angle_y=40
	!game_3d ? player[0].y = -6.15 : player[0].y = -7.00;				// si pas game_3d ...
	
	angle_x = 0;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(	pcamera.x, pcamera.y, pcamera.z,						// la caméra
				ciblex, cibley, 0,										// la cible
				0.0, 1.0,  0.0	);										// le haut
}

// CHANGE RESOLUTION
BOOL ChangeScreenResolution(int width, int height, int bitsPerPixel)	// Change The Screen Resolution
{
	DEVMODE dmScreenSettings;											// Device Mode
	ZeroMemory(&dmScreenSettings, sizeof(DEVMODE));						// Make Sure Memory Is Cleared
	dmScreenSettings.dmSize				= sizeof(DEVMODE);				// Size Of The Devmode Structure
	dmScreenSettings.dmPelsWidth		= width;						// Select Screen Width
	dmScreenSettings.dmPelsHeight		= height;						// Select Screen Height
	dmScreenSettings.dmBitsPerPel		= bitsPerPixel;					// Select Bits Per Pixel
	dmScreenSettings.dmFields			= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

	if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		return FALSE;													// Display Change Failed, Return False

	return TRUE;														// Display Change Was Successful, Return True
}

void Fullscreen(void)
{
	// fenêtre
	//DWORD windowStyle = WS_OVERLAPPEDWINDOW;							// Define Our Window Style
	//DWORD windowExtendedStyle = WS_EX_APPWINDOW;						// Define The Window's Extended Style

	// fullscreen
	DWORD windowStyle = WS_POPUP;											// Set The WindowStyle To WS_POPUP (Popup Window)
	DWORD windowExtendedStyle = WS_EX_TOPMOST;							// Set The Extended Window Style To WS_EX_TOPMOST

	// retrieve the resolution
	HWND	hDesktop = GetDesktopWindow();
	HDC	hDc = GetDC(hDesktop);
	int w = GetDeviceCaps(hDc, PLANES) * GetDeviceCaps(hDc, HORZRES);
	int h = GetDeviceCaps(hDc, PLANES) * GetDeviceCaps(hDc, VERTRES);
	int bits = GetDeviceCaps(hDc, PLANES) * GetDeviceCaps(hDc, BITSPIXEL);
	ReleaseDC(hDesktop, hDc);
	
	// change style
	DWORD dwStyle,dwNewStyle;
	
	dwStyle = GetWindowLong(g_window->hWnd, (TRUE ? GWL_EXSTYLE : GWL_STYLE));
	//dwNewStyle = (dwStyle & (~NULL)) | windowExtendedStyle;
	//dwStyle = GetWindowLong(g_window->hWnd, GWL_EXSTYLE);
	dwNewStyle = (dwStyle & (~NULL)) | windowExtendedStyle;
	
	SetWindowLong(g_window->hWnd, (TRUE ? GWL_EXSTYLE : GWL_STYLE), dwNewStyle);
	//SetWindowPos(g_window->hWnd,NULL,0,0,w,h,SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE);
	//SetWindowLong(g_window->hWnd, GWL_EXSTYLE, dwNewStyle);
	SetWindowPos(g_window->hWnd,NULL,0,0,w,h,SWP_NOZORDER);
}

//	RETOURNE LA TAILLE DU BUREAU
void ReturnSystemMetrics(int *Width, int *Height, int *BitsPerPixel)
{
	HWND	hDesktop = GetDesktopWindow();
	HDC	hDc = GetDC(hDesktop);

	*(Width) = GetDeviceCaps(hDc, PLANES) * GetDeviceCaps(hDc, HORZRES);
	*(Height) = GetDeviceCaps(hDc, PLANES) * GetDeviceCaps(hDc, VERTRES);
	*(BitsPerPixel) = GetDeviceCaps(hDc, PLANES) * GetDeviceCaps(hDc, BITSPIXEL);
	ReleaseDC(hDesktop, hDc);

	return;
	// une autre méthode : HEIGHT = GetSystemMetrics(SM_CYSCREEN); WIDTH = GetSystemMetrics(SM_CXSCREEN);
}

//////////////////////////////////////////////////////////////////////////////////////
//
// DANS UN NOUVEAU THREAD - ESSAYE DE SE CONNECTER A INTERNET POUR DECLENCHER LA CONNEXION
//                          INTERNET ET EVENTUELLEMENT LE FIREWALL
//
void InternetConnect(void* dummy)
{
	static bool already_tried = false;
	if (already_tried) return;

	InternetAttemptConnect(0);
	HINTERNET my_internet = InternetOpen("Microsoft Internet Explorer", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
	if (InternetOpenUrl(my_internet, "http://www.google.com", NULL, NULL, NULL, NULL) == NULL)
	{
		if (GetLastError() == ERROR_INTERNET_EXTENDED_ERROR)
		{
			char buf[2000];
			DWORD e, l=sizeof (buf);
			InternetGetLastResponseInfo (&e, buf, &l);
			MessageBox(g_hwndDlg, buf, "Internet Error", MB_APPLMODAL);
		}
		else
			MessageBox(g_hwndDlg, "Info : Internet connection can't be used.", "Warning", MB_APPLMODAL);
	}

	already_tried = true;
	_endthread();
}

//////////////////////////////////////////////////////////////////////////////////////
//
// CREATE WINDOW GL
//
//
BOOL CreateWindowGL(GL_Window* window)									// This Code Creates Our OpenGL Window
{
	DWORD windowStyle = WS_OVERLAPPEDWINDOW;							// Define Our Window Style
	DWORD windowExtendedStyle = WS_EX_APPWINDOW;						// Define The Window's Extended Style

	PIXELFORMATDESCRIPTOR pfd =											// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),									// Size Of This Pixel Format Descriptor
			1,															// Version Number
			PFD_DRAW_TO_WINDOW |										// Format Must Support Window
			PFD_SUPPORT_OPENGL |										// Format Must Support OpenGL
			PFD_DOUBLEBUFFER,											// Must Support Double Buffering
			PFD_TYPE_RGBA,												// Request An RGBA Format
			window->init.bitsPerPixel,									// Select Our Color Depth
			0, 0, 0, 0, 0, 0,											// Color Bits Ignored
			0,															// No Alpha Buffer
			0,															// Shift Bit Ignored
			0,															// No Accumulation Buffer
			0, 0, 0, 0,													// Accumulation Bits Ignored
			16,															// 16Bit Z-Buffer (Depth Buffer)  
			0,															// No Stencil Buffer
			0,															// No Auxiliary Buffer
			PFD_MAIN_PLANE,												// Main Drawing Layer
			0,															// Reserved
			0, 0, 0														// Layer Masks Ignored
	};
	RECT windowRect = {0, 0, window->init.width, window->init.height};	// Define Our Window Coordinates

	GLuint PixelFormat;													// Will Hold The Selected Pixel Format

	// quick fullscreen mode
	if (window->init.quickFullScreen == TRUE) 
	{
		ShowCursor(TRUE);												// Turn Off The Cursor
		windowStyle = WS_POPUP;											// Set The WindowStyle To WS_POPUP (Popup Window)
		windowExtendedStyle |= WS_EX_TOPMOST;							// Set The Extended Window Style To WS_EX_TOPMOST
	}																	// (Top Window Covering Everything Else)

	// fullscreen mode
	else if (window->init.isFullScreen == TRUE)								// Fullscreen Requested, Try Changing Video Modes
	{
		if (ChangeScreenResolution(window->init.width, window->init.height, window->init.bitsPerPixel))
		{
			ShowCursor(TRUE);											// Turn On The Cursor
			windowStyle = WS_POPUP;										// Set The WindowStyle To WS_POPUP (Popup Window)
			windowExtendedStyle |= WS_EX_TOPMOST;						// Set The Extended Window Style To WS_EX_TOPMOST
		}																// (Top Window Covering Everything Else)
		else
		{
			MessageBox(HWND_DESKTOP, "Mode Switch Failed.\nRunning In Windowed Mode.", "Error", MB_OK | MB_ICONEXCLAMATION);
			window->init.isFullScreen = FALSE;							// Set isFullscreen To False (Windowed Mode)
		}
	}

	// window mode
	else																// If Fullscreen Was Not Selected
	{
		AdjustWindowRectEx(&windowRect, windowStyle, 0, windowExtendedStyle);// Adjust Window, Account For Window Borders
	}

	// window position (will be centered for the "window mode")
	int left_pos = 0;
	int top_pos  = 0;

	if (!window->init.isFullScreen && !window->init.quickFullScreen)
	{
		HWND hDesktop = GetDesktopWindow();
		HDC  hDc = GetDC(hDesktop);
		int desktop_width  = GetDeviceCaps(hDc, PLANES) * GetDeviceCaps(hDc, HORZRES);
		int desktop_height = GetDeviceCaps(hDc, PLANES) * GetDeviceCaps(hDc, VERTRES);
		ReleaseDC(hDesktop, hDc);

		if (window->init.width < desktop_width && window->init.height < desktop_height) // on ne centre que si la fenêtre est + petite que le bureau
		{
			left_pos = desktop_width  / 2 - window->init.width  / 2;
			top_pos  = desktop_height / 2 - window->init.height / 2 - 40;
		}
	}

	// Create The Game Window
	window->hWnd = CreateWindowEx(windowExtendedStyle,					// Extended Style
		window->init.application->className,							// Class Name
		window->init.title,												// Window Title
		windowStyle,													// Window Style
		left_pos, top_pos,												// Window X,Y Position
		windowRect.right - windowRect.left,								// Window Width
		windowRect.bottom - windowRect.top,								// Window Height
		HWND_DESKTOP,													// Desktop Is Window's Parent
		0,																// No Menu
		window->init.application->hInstance,							// Pass The Window Instance
		window);

	if (window->hWnd == 0)												// Was Window Creation A Success?
		return FALSE;													// If Not Return False

	// Get a Device Context for this Window
	window->hDC = GetDC(window->hWnd);									// Grab A Device Context
	if (window->hDC == 0)												// Did We Get A Device Context?
	{
		DestroyWindow(window->hWnd);									// Destroy The Window
		window->hWnd = 0;												// Zero The Window Handle
		return FALSE;													// Return False
	}
	
	// Anti-aliasing
	//
	// Our first pass, Multisampling hasn't been created yet, so we create a window normally
	// If it is supported, then we're on our second pass
	// that means we want to use our pixel format for sampling
	// so set PixelFormat to arbMultiSampleformat instead

	if(!arbMultisampleSupported)
	{
		PixelFormat = ChoosePixelFormat (window->hDC, &pfd);				// Find A Compatible Pixel Format
		if (PixelFormat == 0)												// Did We Find A Compatible Format?
		{
			// Failed
			ReleaseDC (window->hWnd, window->hDC);							// Release Our Device Context
			window->hDC = 0;												// Zero The Device Context
			DestroyWindow (window->hWnd);									// Destroy The Window
			window->hWnd = 0;												// Zero The Window Handle
			return FALSE;													// Return False
		}
	}
	else
		PixelFormat = arbMultisampleFormat;

	if (SetPixelFormat (window->hDC, PixelFormat, &pfd) == FALSE)		// Try To Set The Pixel Format
	{
		// Failed
		ReleaseDC (window->hWnd, window->hDC);							// Release Our Device Context
		window->hDC = 0;												// Zero The Device Context
		DestroyWindow (window->hWnd);									// Destroy The Window
		window->hWnd = 0;												// Zero The Window Handle
		return FALSE;													// Return False
	}

	window->hRC = wglCreateContext (window->hDC);						// Try To Get A Rendering Context
	if (window->hRC == 0)												// Did We Get A Rendering Context?
	{
		// Failed
		ReleaseDC (window->hWnd, window->hDC);							// Release Our Device Context
		window->hDC = 0;												// Zero The Device Context
		DestroyWindow (window->hWnd);									// Destroy The Window
		window->hWnd = 0;												// Zero The Window Handle
		return FALSE;													// Return False
	}

	// Make The Rendering Context Our Current Rendering Context
	if (wglMakeCurrent (window->hDC, window->hRC) == FALSE)
	{
		// Failed
		wglDeleteContext (window->hRC);									// Delete The Rendering Context
		window->hRC = 0;												// Zero The Rendering Context
		ReleaseDC (window->hWnd, window->hDC);							// Release Our Device Context
		window->hDC = 0;												// Zero The Device Context
		DestroyWindow (window->hWnd);									// Destroy The Window
		window->hWnd = 0;												// Zero The Window Handle
		return FALSE;													// Return False
	}

	/*
	Now that our window is created, we want to query what samples are available
	we call our InitMultiSample window
	if we return a valid context, we want to destroy our current window
	and create a new one using the multisample interface.
	*/
	if(!arbMultisampleSupported && game_option_anti_aliasing)
	{
		if(InitMultisample(window->init.application->hInstance,window->hWnd,pfd))
		{
			// Anti-aliasing disponible
			DestroyWindowGL (window);		// on détruit la fenêtre actuelle,
			return CreateWindowGL(window);	// et on relance la fonction dans laquelle
											// on se trouve pour recréer une nouvelle
											// fenêtre avec l'interface multisample !
		}
	}

	// For text in loading screen
	glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_FASTEST);

	ShowWindow(window->hWnd, SW_NORMAL);								// Make The Window Visible
	window->isVisible = TRUE;											// Set isVisible To True

	ReshapeGL(window->init.width, window->init.height);					// Reshape Our GL Window

	ZeroMemory(window->keys, sizeof(Keys));								// Clear All Keys

	return TRUE;														// Window Creating Was A Success
	// Initialization Will Be Done In WM_CREATE
}

//////////////////////////////////////////////////////////////////////////////////////
//
// DESTROY WINDOW GL
//
//
BOOL DestroyWindowGL(GL_Window* window)									// Destroy The OpenGL Window & Release Resources
{
	if (window->hWnd != 0)												// Does The Window Have A Handle?
	{	
		if (window->hDC != 0)											// Does The Window Have A Device Context?
		{
			wglMakeCurrent(window->hDC, 0);								// Set The Current Active Rendering Context To Zero
			if (window->hRC != 0)										// Does The Window Have A Rendering Context?
			{
				wglDeleteContext(window->hRC);							// Release The Rendering Context
				window->hRC = 0;										// Zero The Rendering Context
			}
			ReleaseDC(window->hWnd, window->hDC);						// Release The Device Context
			window->hDC = 0;											// Zero The Device Context
		}
		DestroyWindow(window->hWnd);									// Destroy The Window
		window->hWnd = 0;												// Zero The Window Handle
	}

	if (window->init.isFullScreen)										// Is Window In Fullscreen Mode
	{
		ChangeDisplaySettings(NULL, 0);									// Switch Back To Desktop Resolution
		ShowCursor(TRUE);												// Show The Cursor
	}	
	return TRUE;														// Return True
}

//////////////////////////////////////////////////////////////////////////////////////
//
// Process Window Message Callbacks (GAME)
//
//
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Get The Window Context
	GL_Window* window = (GL_Window*)(GetWindowLong(hWnd, GWL_USERDATA));

	switch (uMsg)													// Evaluate Window Message
	{
	case WM_SYSCOMMAND:												// Intercept System Commands
		{
			switch (wParam)											// Check System Calls
			{
				case SC_SCREENSAVE:									// Screensaver Trying To Start?
				case SC_MONITORPOWER:								// Monitor Trying To Enter Powersave?
					return 0;										// Prevent From Happening
				case SC_RESTORE:
					{
						//ShowWindow(window->hWnd, SW_SHOW);
						//BOX("nono");
						//return 0;
					}
			}
			break;													// Exit
		}
		return 0;													// Return

	case WM_CREATE:													// Window Creation
		{
			CREATESTRUCT* creation = (CREATESTRUCT*)(lParam);		// Store Window Structure Pointer
			window = (GL_Window*)(creation->lpCreateParams);
			SetWindowLong(hWnd, GWL_USERDATA, (LONG)(window));
		}
		return 0;													// Return

	case WM_CLOSE:													// Closing The Window
		TerminateApplication(window);								// Terminate The Application
		return 0;													// Return

	case WM_ENTERSIZEMOVE:
		if (game_step == 2)											// pendant le chargement on baisse la priorité
			SetThreadPriority((void*)h_thread2, THREAD_PRIORITY_LOWEST);
		return 0;

	case WM_EXITSIZEMOVE:
		{
			if (game_step == 2)										// pendant le chargement on remet en haute priorité
				SetThreadPriority((void*)h_thread2, THREAD_PRIORITY_NORMAL );
			else
				SetThreadPriority((void*)h_thread2, THREAD_PRIORITY_NORMAL );
			Fps_Reinitialize();										// FPS Méthode n°1 - To Avoid Jump Movement After Resize / Move
			Fps2_firstpass = true;									// FPS Méthode n°2
		}
		return 0;

	case WM_SIZE:													// Size Action Has Taken Place
		switch (wParam)												// Evaluate Size Action
		{
		case SIZE_MINIMIZED:										// Was Window Minimized?
			//BOX("minimized")
			window->isVisible = FALSE;								// Set isVisible To False
			return 0;												// Return

		case SIZE_MAXIMIZED:										// Was Window Maximized?
			window->isVisible = TRUE;								// Set isVisible To True
			ReshapeGL(LOWORD(lParam), HIWORD(lParam));				// Reshape Window - LoWord=Width, HiWord=Height
			window->init.width = LOWORD(lParam);
			window->init.height = HIWORD(lParam);
			Fps_Reinitialize();										// FPS Méthode n°1
			Fps2_firstpass = true;									// FPS Méthode n°2
			return 0;												// Return

		case SIZE_RESTORED:											// Was Window Restored?
			window->isVisible = TRUE;								// Set isVisible To True
			ReshapeGL(LOWORD(lParam), HIWORD(lParam));				// Reshape Window - LoWord=Width, HiWord=Height
			window->init.width = LOWORD(lParam);
			window->init.height = HIWORD(lParam);
			Fps_Reinitialize();										// FPS Méthode n°1
			Fps2_firstpass = true;									// FPS Méthode n°2
			return 0;												// Return
		}
		break;														// Break

	case WM_KEYDOWN:												// Update Keyboard Buffers For Keys Pressed
		if ((wParam >= 0) && (wParam <= 255))						// Is Key (wParam) In A Valid Range?
		{
			window->keys->keyDown[wParam] = TRUE;					// Set The Selected Key (wParam) To True
			if (game_option_wivik)
				window->keys->wivik[wParam] = TRUE;
			
			TCHAR temp[512];
			wsprintf(temp,"%d",wParam);
			//OutputDebugString(temp);								// Pour savoir le code d'une touche pressée
			
			return 0;												// Return
		}
		break;														// Break

	case WM_KEYUP:													// Update Keyboard Buffers For Keys Released
		if ((wParam >= 0) && (wParam <= 255))						// Is Key (wParam) In A Valid Range?
		{
			window->keys->keyDown[wParam] = FALSE;					// Set The Selected Key (wParam) To False
			return 0;												// Return
		}
		break;														// Break

	case WM_LBUTTONDOWN:
		{
			if (game_step != 4) return 0;
			moving = true;											// We are moving the scene
			startx = LOWORD(lParam);								// mouse x
			starty = HIWORD(lParam);								// mouse y

			// largeur de résolution
			int w = g_window->init.width;

			// paliers de résolutions
			int w2;
			if (w >= 1600) w2 = 1600; else if (w >= 1280) w2 = 1280; else if (w >= 1024) w2 = 1024;	else if (w >= 800) w2 = 800; else w2 = 640;
		
			// déplacement equalizer (BOUTON ENFONÇÉ)
			if (game_spectrum && w > 550)
				if ((startx > equalizer_x+w+(-183)*w2/1024 && startx < equalizer_x+w+(-183+147)*w2/1024)
				&& (starty > -equalizer_y+(+238-102)*w2/1024 && starty < -equalizer_y+238*w2/1024))
				{
					equalizer_moving = true;							// l'equalizer est bougé
					SetCursor(cur_hand_closed);
				}
		}
		break;

	case WM_LBUTTONUP:
		{
			// déplacement equalizer (BOUTON RELACHÉ)
			if (equalizer_moving)
				SetCursor(cur_hand);

			moving = false;											// We stop moving the scene
			equalizer_moving = false;								// We stop moving the equalizer
		}
		break;

	case WM_MOUSEMOVE:
		{
			if (game_step != 4) return 0;

			// largeur de résolution
			int w = g_window->init.width;

			// paliers de résolutions
			int w2;
			if (w >= 1600) w2 = 1600; else if (w >= 1280) w2 = 1280; else if (w >= 1024) w2 = 1024;	else if (w >= 800) w2 = 800; else w2 = 640;
		
			// equalizer (MOUSE OVER)
			if (game_spectrum && w > 550)
				if ((startx > equalizer_x+w+(-183)*w2/1024 && startx < equalizer_x+w+(-183+147)*w2/1024)
				&& (starty > -equalizer_y+(+238-102)*w2/1024 && starty < -equalizer_y+238*w2/1024))
					if (equalizer_moving)
						SetCursor(cur_hand_closed);
					else
						SetCursor(cur_hand);
		
			// déplacement equalizer (la souris est déplacée pendant que le bouton est pressé)
			if (equalizer_moving)
			{
				equalizer_x += LOWORD(lParam) - startx;
				equalizer_y += starty - HIWORD(lParam);
			}
			// rotation de la scène (bouton enfonçé mais pas sur l'équalizer)
			else if (moving)
			{
				angle_x += (LOWORD(lParam) - startx) / 2;			// souris axe x (/2 pour diminuer la vitesse)
				angle_y += (HIWORD(lParam) - starty) / 2;			// souris axe y (/2 pour diminuer la vitesse)
			}
			startx = LOWORD(lParam);								// mouse x
			starty = HIWORD(lParam);								// mouse y
		}
		break;

	case WM_TOGGLEFULLSCREEN:									// Toggle FullScreen Mode On/Off
		g_createFullScreen = (g_createFullScreen == TRUE) ? FALSE : TRUE;
		PostMessage(hWnd, WM_QUIT, 0, 0);
		break;													// Break

	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);			// Pass Unhandled Messages To DefWindowProc
}

//////////////////////////////////////////////////////////////////////////////////////
//
// REGISTER WINDOW CLASS
//
//
BOOL RegisterWindowClass(Application* application)						// Register A Window Class For This Application.
{																		// TRUE If Successful
	// Register A Window Class
	WNDCLASSEX windowClass;												// Window Class
	ZeroMemory(&windowClass, sizeof(WNDCLASSEX));						// Make Sure Memory Is Cleared
	windowClass.cbSize			= sizeof(WNDCLASSEX);					// Size Of The windowClass Structure
	windowClass.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraws The Window For Any Movement / Resizing
	windowClass.lpfnWndProc		= (WNDPROC)(WindowProc);				// WindowProc Handles Messages
	windowClass.hInstance		= application->hInstance;				// Set The Instance
	windowClass.hbrBackground	= static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));// Class Background Brush Color
	windowClass.hCursor			= LoadCursor(application->hInstance, (LPCTSTR)IDC_CURSOR_CROSS);// Load The Arrow Pointer
	windowClass.lpszClassName	= application->className;				// Sets The Applications Classname
	windowClass.hIcon			= LoadIcon(application->hInstance, (LPCTSTR)IDI_ICON_APP);

	if (RegisterClassEx(&windowClass) == 0)								// Did Registering The Class Fail?
	{
		// NOTE: Failure, Should Never Happen
		MessageBox(HWND_DESKTOP, "RegisterClassEx Failed!", "Error", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;													// Return False (Failure)
	}
	return TRUE;														// Return True (Success)
}


//////////////////////////////////////////////////////////////////////////////////////
//
// THREAD - ON BAISSE LE VOLUME DE LA MUSIQUE
//
//
void FmodFadingOut(void* dummy)
{
	int volume = 250;

	do
	{
		volume -= 5;
		FMUSIC_SetMasterVolume(my_mod_about,volume);

		Sleep(10);
	}
	while(volume >= 0);

	FMUSIC_StopSong(my_mod_about);
	my_mod_about_on = false;
	my_mod_about_working = false;
	_endthread();
}

//////////////////////////////////////////////////////////////////////////////////////
//
// THREAD - ON AUGMENTE LE VOLUME DE LA MUSIQUE
//
//
void FmodFadingIn(void* dummy)
{
	FMUSIC_SetMasterVolume(my_mod_about, 0);

	if (!FMUSIC_IsPlaying(my_mod_about))
		FMUSIC_PlaySong(my_mod_about);

	my_mod_about_on = true;
	int volume = 0;

	do
	{
		volume += 5;
		FMUSIC_SetMasterVolume(my_mod_about,volume);

		Sleep(10);
	}
	while(volume <= 250);

	my_mod_about_working = false;
	_endthread();
}

//////////////////////////////////////////////////////////////////////////////////////
//
// THREAD - TRANSPARENCE DE LA FENETRE (WIN 2000/XP)
//
//
void DialogTransparency(void* dummy)
{
	Sleep(250);
	int alpha = 0;
	DWORD dwStyle = GetWindowLong(g_hwndDlgAbout, GWL_EXSTYLE);
	SetWindowLong(g_hwndDlgAbout, GWL_EXSTYLE, dwStyle | 0x00080000);	// = WS_EX_LAYERED

	#define LWA_ALPHA		0x00000002
	SLWA_FUNC MySetLayeredWindowAttributes;
	HMODULE hUser32 = GetModuleHandle("USER32.DLL");
	if (!hUser32) _endthread();
	MySetLayeredWindowAttributes = (SLWA_FUNC)GetProcAddress(hUser32,"SetLayeredWindowAttributes");

	do
	{
		if (MySetLayeredWindowAttributes)
			MySetLayeredWindowAttributes(g_hwndDlgAbout,NULL,alpha,LWA_ALPHA);

		alpha += 30;
		Sleep(2);
	}
	while(alpha <= 255);

	// retour au mode normal
	if (MySetLayeredWindowAttributes)
		MySetLayeredWindowAttributes(g_hwndDlgAbout,NULL,255,LWA_ALPHA);
	SetWindowLong(g_hwndDlgAbout,GWL_EXSTYLE,dwStyle & ~0x80000);

	_endthread();
}

//////////////////////////////////////////////////////////////////////////////////////
//
// CALLBACK - DIALOG BOX ABOUT (readme...)
//
//
BOOL CALLBACK DialogAboutProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam) 
{ 
	g_hwndDlgAbout = hwndDlg;									// Global handle
	static HWND edit_handle = NULL;

	switch (message) 
	{ 
	case WM_COMMAND: 
		switch (LOWORD(wParam)) 
		{ 
		case IDCANCEL:											// close (escape) : hidden button (called automatically when the escape key is pressed)
		case IDC_BUTTON_CLOSE:									// close (button)
			SendMessage(hwndDlg, WM_CLOSE, 0, 0);
			return TRUE;

		case IDC_BUTTON_MUTE:
			{
				if (!my_mod_about_working)
				{
					my_mod_about_working = true;

					if (FMUSIC_IsPlaying(my_mod_about)) // stop the music
					{
						// place l'icône "MUSIQUE ON" dans le bouton
						HICON hU = (HICON)LoadImage(window.init.application->hInstance,	MAKEINTRESOURCE(IDB_BUTTON_PLAY), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
						SendMessage(GetDlgItem(hwndDlg,IDC_BUTTON_MUTE), BM_SETIMAGE, IMAGE_BITMAP, (LPARAM) (DWORD)(uintptr_t)hU );

						_beginthread( FmodFadingOut, 0, NULL );
					}
					else								// start the music
					{
						// place l'icône "MUSIQUE OFF" dans le bouton
						HICON hU = (HICON)LoadImage(window.init.application->hInstance,	MAKEINTRESOURCE(IDB_BUTTON_MUTE), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
						SendMessage(GetDlgItem(hwndDlg,IDC_BUTTON_MUTE), BM_SETIMAGE, IMAGE_BITMAP, (LPARAM) (DWORD)(uintptr_t)hU );

						_beginthread( FmodFadingIn, 0, NULL );
					}
				}
			}
			return TRUE;

		case IDC_HOMEPAGE:
			ShellExecute(hwndDlg, "open", "http://spaceinvadersgl.sourceforge.net", NULL, NULL, SW_SHOWNORMAL);
			return TRUE;

		case IDC_MAIL:
			ShellExecute(hwndDlg, "open", "https://sourceforge.net/forum/?group_id=74529", NULL, NULL, SW_SHOWNORMAL);
			return TRUE;
		
		case IDC_CHECK_WIVIK:
			if (IsDlgButtonChecked(hwndDlg,IDC_CHECK_WIVIK))
			{
				EnableWindow( GetDlgItem(hwndDlg,IDC_SLIDER_WIVIK), TRUE );
				EnableWindow( GetDlgItem(hwndDlg,IDC_TEXT_WIVIK), TRUE );
				game_option_wivik = true;
			}
			else
			{
				EnableWindow( GetDlgItem(hwndDlg,IDC_SLIDER_WIVIK), FALSE );
				EnableWindow( GetDlgItem(hwndDlg,IDC_TEXT_WIVIK), FALSE );
				game_option_wivik = false;
			}
			return TRUE;
		}
		
		switch (HIWORD(wParam))
		{
		case BN_CLICKED:
			break;
		}

		return FALSE;

	case WM_HSCROLL:
		{
			// GET SLIDER POSITION FOR WIVIK ON-SCREEN KEYBOARD
			game_option_holddown_wivik = SendMessage(GetDlgItem(hwndDlg,IDC_SLIDER_WIVIK), TBM_GETPOS, 0, 0);
			char message[512]="";
			if (game_lang == 0 || game_lang == 2)
				strcpy(message, "1 click = ");
			else if (game_lang == 1)
				strcpy(message, "1 clic = ");
			char temp[10]="";
			_ltoa(game_option_holddown_wivik, temp, 10);
			strcat(message, temp);
			if (game_lang == 0)
				strcat(message, "ms hold down key");
			else if (game_lang == 1)
				strcat(message, "ms de touche appuyée");
			else if (game_lang == 2)
				strcat(message, "ms pulsación tecla abajo");
			SetWindowText(GetDlgItem(hwndDlg,IDC_TEXT_WIVIK), message);
		}
		return TRUE;

	case WM_CLOSE:												// Close (X, Alt+F4) also called with escape key and close button (see IDCANCEL: and IDC_BUTTON_CLOSE:)
		{
			// SAUVEGARDE DANS LA BASE DE REGISTRE DU DELAI WIVIK ON-SCREEN KEYBOARD
			HKEY key;
			if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\SpaceInvaders-OpenGL\\", 0, KEY_WRITE, &key) == ERROR_SUCCESS)
			{
				// Wivik Activé ?
				if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_WIVIK))
					game_option_wivik = 1;
				else
					game_option_wivik = 0;
				RegSetValueEx(key, "game_option_wivik", 0, REG_BINARY, (LPBYTE) &game_option_wivik, sizeof(bool));

				// Wivik Délai
				RegSetValueEx(key, "game_option_holddown_wivik", 0, REG_DWORD, (LPBYTE) &game_option_holddown_wivik, sizeof(DWORD));
			}
			RegCloseKey(key);

			EndDialog(hwndDlg, TRUE);
		}
		return TRUE;

	case WM_INITDIALOG:
		{
			//// FADING TRANSPARENCE
			//// on met l'opacité à 0
			//DWORD dwStyle = GetWindowLong(hwndDlg, GWL_EXSTYLE);
			//SetWindowLong(hwndDlg, (TRUE ? GWL_EXSTYLE : GWL_STYLE), dwStyle | 0x00080000);
			//#define LWA_ALPHA		0x00000002
			//SLWA_FUNC MySetLayeredWindowAttributes;
			//HMODULE hUser32 = GetModuleHandle("USER32.DLL");
			//if (hUser32)
			//{
			//	MySetLayeredWindowAttributes = (SLWA_FUNC)GetProcAddress(hUser32,"SetLayeredWindowAttributes");
			//	if (MySetLayeredWindowAttributes)
			//		MySetLayeredWindowAttributes(hwndDlg,NULL,0,LWA_ALPHA);
			//}
			//// on lance le thread qui augmente l'opacité
			//_beginthread( DialogTransparency, 0, NULL );

			// SLIDE BAR FOR WIVIK ON-SCREEN KEYBOARD
			SendMessage(GetDlgItem(hwndDlg,IDC_SLIDER_WIVIK), TBM_SETRANGEMIN, TRUE, 100);
			SendMessage(GetDlgItem(hwndDlg,IDC_SLIDER_WIVIK), TBM_SETRANGEMAX, TRUE, 500);
			SendMessage(GetDlgItem(hwndDlg,IDC_SLIDER_WIVIK), TBM_SETPOS, TRUE, game_option_holddown_wivik);
			char message[512]="";
			if (game_lang == 0 || game_lang == 2)
				strcpy(message, "1 click = ");
			else if (game_lang == 1)
				strcpy(message, "1 clic = ");
			char temp[10]="";
			_ltoa(game_option_holddown_wivik, temp, 10);
			strcat(message, temp);
			if (game_lang == 0)
				strcat(message, "ms hold down key");
			else if (game_lang == 1)
				strcat(message, "ms de touche appuyée");
			else if (game_lang == 2)
				strcat(message, "ms pulsación tecla abajo");
			SetWindowText(GetDlgItem(hwndDlg,IDC_TEXT_WIVIK), message);

			if (game_option_wivik)
				CheckDlgButton(hwndDlg, IDC_CHECK_WIVIK, BST_CHECKED);
			else
			{
				EnableWindow( GetDlgItem(hwndDlg,IDC_SLIDER_WIVIK), FALSE );
				EnableWindow( GetDlgItem(hwndDlg,IDC_TEXT_WIVIK), FALSE );
			}

			// HYPERLIENS
			ConvertStaticToHyperlink(hwndDlg, IDC_HOMEPAGE);
			ConvertStaticToHyperlink(hwndDlg, IDC_MAIL);

			// EDIT BOX (fichier readme.txt)
			edit_handle = CreateWindowEx(
				WS_EX_CLIENTEDGE,
				"EDIT",
				NULL,
				WS_CHILD | WS_VSCROLL | ES_AUTOHSCROLL |  ES_AUTOVSCROLL | ES_LEFT | ES_MULTILINE | ES_WANTRETURN,
				10, 50,
				553, 347,
				hwndDlg,
				NULL,
				(HINSTANCE)GetWindowLong(hwndDlg, GWL_HINSTANCE),
				NULL);

			if(edit_handle == NULL)
				MessageBox(NULL, "Could not create edit control!", "Error", MB_OK | MB_ICONERROR);

			// on crée la nouvelle font
			HFONT t_font = CreateFont(12, 0, 0, 0, FW_NORMAL, false, false, false, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, 12, "Lucida Console");
			SendMessage(edit_handle, WM_SETFONT, (WPARAM)t_font, MAKELONG(true, 0));

			// on remplit l'Edit Box par le texte du fichier Readme.txt
			HRSRC hrsrc;
			if (game_lang == 0)
				hrsrc			= FindResource(0, MAKEINTRESOURCE(IDR_README_TXT), "TEXT");
			else if (game_lang == 1)
				hrsrc			= FindResource(0, MAKEINTRESOURCE(IDR_README_FR_TXT), "TEXT");
			else if (game_lang == 2)
				hrsrc			= FindResource(0, MAKEINTRESOURCE(IDR_README_ES_TXT), "TEXT");
			HGLOBAL resource	= LoadResource(0, hrsrc);
			void* buffer		= LockResource(resource);
			unsigned int size	= SizeofResource(NULL, hrsrc);
			char* my_text		= (char*)buffer;
			UnlockResource(resource);
			FreeResource(resource);

			SendMessage(edit_handle, WM_SETTEXT, NULL, (LPARAM)my_text);
			ShowWindow(edit_handle, SW_SHOW);

			// Bouton musique (on/off)
			static bool first = true;
			if (my_mod_about_on || first)
			{
				// place l'icône "MUSIQUE OFF"
				HICON hU = (HICON)LoadImage(window.init.application->hInstance,	MAKEINTRESOURCE(IDB_BUTTON_MUTE), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
				SendMessage(GetDlgItem(hwndDlg,IDC_BUTTON_MUTE), BM_SETIMAGE, IMAGE_BITMAP, (LPARAM) (DWORD)(uintptr_t)hU );
			}
			else
			{
				// place l'icône "MUSIQUE ON"
				HICON hU = (HICON)LoadImage(window.init.application->hInstance,	MAKEINTRESOURCE(IDB_BUTTON_PLAY), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
				SendMessage(GetDlgItem(hwndDlg,IDC_BUTTON_MUTE), BM_SETIMAGE, IMAGE_BITMAP, (LPARAM) (DWORD)(uintptr_t)hU );
			}
			first = false;

			// Translate the DialogBox
			translate(1, hwndDlg);

			return TRUE;
		}

		/*
		// transparence de fenêtre windows à mettre dans WM_PAINT (à voir)

		case WM_CREATE:
		{
		// Transparence de la fenêtre
		// on ajoute le style WS_EX_LAYERED	(0x00080000) à la DialogBox

		DWORD dwStyle,dwNewStyle;

		dwStyle = GetWindowLong(hwndDlg, (TRUE ? GWL_EXSTYLE : GWL_STYLE));
		dwNewStyle = (dwStyle & (~NULL)) | 0x00080000;
		SetWindowLong(hwndDlg, (TRUE ? GWL_EXSTYLE : GWL_STYLE), dwNewStyle);
		SetWindowPos(hwndDlg,NULL,0,0,0,0,SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE);

		// puis on règle l'alpha
		#define WS_EX_LAYERED	0x00080000
		#define LWA_COLORKEY	0x00000001
		#define LWA_ALPHA		0x00000002
		BYTE alpha=200,Anim=1;

		SLWA_FUNC MySetLayeredWindowAttributes;
		HMODULE hUser32 = GetModuleHandle("USER32.DLL");
		if (!hUser32) return FALSE;
		MySetLayeredWindowAttributes = (SLWA_FUNC)GetProcAddress(hUser32,"SetLayeredWindowAttributes");
		if (MySetLayeredWindowAttributes)
		MySetLayeredWindowAttributes(hwndDlg,NULL,alpha,LWA_ALPHA);

		}*/
	} 

	return FALSE; // DialogBox procedure must return FALSE for messages not handled.
} 



//////////////////////////////////////////////////////////////////////////////////////
//
// CALLBACK -> DIALOGBOX CUSTOM STREAM (enter an url)
//
//
LRESULT CALLBACK DialogStreamProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:	// close the DialogBox when <escape> key is pressed
			EndDialog(hWnd, wParam);
			return TRUE;

		case IDC_STREAMING_SITE:
			ShellExecute(NULL, "open", "http://www.shoutcast.com", NULL, NULL, SW_SHOWNORMAL);
			return TRUE;

		case IDOK:
			char string[1024] = "";
			GetDlgItemText(hWnd, IDC_STREAMING_URL, string, 1024);
			strcpy(stream_name_user, string);
			//BOX(stream_name_user);
			EndDialog(hWnd, wParam);
			return TRUE;
		}
		
		return FALSE;

	case WM_INITDIALOG:
		{
			// Get desktop size
			HWND	hDesktop = GetDesktopWindow();
			HDC	hDc = GetDC(hDesktop);
			int desktop_width  = GetDeviceCaps(hDc, PLANES) * GetDeviceCaps(hDc, HORZRES);
			int desktop_height = GetDeviceCaps(hDc, PLANES) * GetDeviceCaps(hDc, VERTRES);
			ReleaseDC(hDesktop, hDc);

			// Set the window position and size
			SetWindowPos(hWnd, HWND_TOPMOST, desktop_width/2 -218, desktop_height/2 -94, 0, 0, SWP_NOSIZE);

			// Hyperlien
			ConvertStaticToHyperlink(hWnd, IDC_STREAMING_SITE);

			// Put custom URL in editbox
			if (strcmp(stream_name_user,"") == 0)
				strcpy(stream_name_user, "http://64.236.34.4:80/stream/1065");
			SetDlgItemText(hWnd, IDC_STREAMING_URL, stream_name_user);
			SendDlgItemMessage(hWnd,IDC_STREAMING_URL, EM_SETSEL, 0, 1024);

			// Translate the DialogBox
			translate(2, hWnd);
		}
		return TRUE;
	}

	return FALSE; // DialogBox procedure must return FALSE for messages not handled.
}

//////////////////////////////////////////////////////////////////////////////////////
//
// CALLBACK - CONTROLS FOR THE STARTUP DIALOGBOX
//            This callback procedure is used for all controls which handle a tooltip.
//
//
LRESULT CALLBACK DialogControlsProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// Static variables for the move/hover/leave messages passed to the controls
	static DWORD time_exit		= timeGetTime();
	static bool  enter			= false;
	static bool  is_shown		= false;
	static POINT cur;
	TRACKMOUSEEVENT tme;

	switch (message)
	{ 
	case WM_MOUSEMOVE:
		{
			// Set the text and title for the new tooltip
			// (either it's the first time we enter the control or we are changing of control)
			if (!enter)
				if (GetDlgItem(GetParent(hwnd), IDC_MIPMAP) == hwnd)
				{
					if (game_lang == 0)
					{
						ti.lpszText = "Creates different size textures for best visual quality.\r\nNeeds a few more memory megabytes.";
						SendMessage(g_hwndTT, TTM_SETTITLE, 1, (LPARAM) (LPCTSTR) "MIPMAP TEXTURES");
					}
					else if (game_lang == 1)
					{
						ti.lpszText = "Créé des textures de tailles différentes pour une meilleure qualité visuelle.\r\nNécessite quelques mega-octets de mémoire en plus.";
						SendMessage(g_hwndTT, TTM_SETTITLE, 1, (LPARAM) (LPCTSTR) "TEXTURES MIPMAP");
					}
					else if (game_lang == 2)
					{
						ti.lpszText = "Crean diferentes tamaños de las texturas para la mejor calidad visual.\r\nNecesita unos pocos Mb's de memoria adicional.";
						SendMessage(g_hwndTT, TTM_SETTITLE, 1, (LPARAM) (LPCTSTR) "TEXTURAS MIPMAP");
					}
				}
				else if (GetDlgItem(GetParent(hwnd), IDC_VSYNC) == hwnd)
				{
					if (game_lang == 0)
					{
						ti.lpszText = "Adjusts frame rate to screen frequency for smoothed movements.\r\nFPS is limited by this frequency.\r\n\r\n<V> key to swap ON/OFF during game.";
						SendMessage(g_hwndTT, TTM_SETTITLE, (WPARAM) 1, (LPARAM) (LPCTSTR) "VERTICAL SYNCHRONIZATION");
					}
					else if (game_lang == 1)
					{
						ti.lpszText = "Ajuste la fréquence d'images à la fréquence de l'écran pour des mouvements plus fluides.\r\nLe nombre d'images par seconde est limité à cette fréquence.\r\n\r\nTouche <V> pour activer/désactiver pendant le jeu.";
						SendMessage(g_hwndTT, TTM_SETTITLE, (WPARAM) 1, (LPARAM) (LPCTSTR) "SYNCHRONISATION VERTICALE");
					}
					else if (game_lang == 2)
					{
						ti.lpszText = "Ajusta el frame rate a la frecuencia de pantalla para los movimientos suavizados.\r\nFPS está limitado por su frecuencia.";
						SendMessage(g_hwndTT, TTM_SETTITLE, (WPARAM) 1, (LPARAM) (LPCTSTR) "SINCRONIZACIÓN VERTICAL");
					}
				}
				else if (GetDlgItem(GetParent(hwnd), IDC_STREAM) == hwnd)
				{
					if (game_lang == 0)
					{
						ti.lpszText = "Enables Internet music streaming.        \r\nChoose from :\r\n- Cable/DSL (128k)\r\n- (Dual)ISDN (56k)\r\n- Dialup (24k)\r\n- Custom (enter an URL)";
						SendMessage(g_hwndTT, TTM_SETTITLE, (WPARAM) 1, (LPARAM) (LPCTSTR) "STREAMED MUSIC");
					}
					else if (game_lang == 1)
					{
						ti.lpszText = "Active le flux sonore par Internet.        \r\nChoisir entre :\r\n- Cable/DSL (128k)\r\n- (Dual)ISDN (56k)\r\n- Dialup (24k)\r\n- Personnalisé (entrer une URL)";
						SendMessage(g_hwndTT, TTM_SETTITLE, (WPARAM) 1, (LPARAM) (LPCTSTR) "MUSIQUE EN STREAMING");
					}
					else if (game_lang == 2)
					{
						ti.lpszText = "Ajusta el ancho de banda de Internet para la música on-line. Elija entre:\r\n- Cable/ADSL (128k)\r\n- (Dual)ISDN (56k)\r\n- Dialup (24k)\r\n- Personalizado (introduzca una URL)";
						SendMessage(g_hwndTT, TTM_SETTITLE, (WPARAM) 1, (LPARAM) (LPCTSTR) "ANCHO BANDA MÚSICA");
					}
				}
				else if (GetDlgItem(GetParent(hwnd), IDC_BILLBOARDING) == hwnd)
				{
					if (game_lang == 0)
					{
						ti.lpszText = "Adjusts the orientation of 2D objects (like particles) so they face the camera for best representation (only in 3D mode).\r\nNeeds more CPU.";
						SendMessage(g_hwndTT, TTM_SETTITLE, (WPARAM) 1, (LPARAM) (LPCTSTR) "BILLBOARDING");
					}
					else if (game_lang == 1)
					{
						ti.lpszText = "Ajuste l'orientation des objets 2D (comme les particules d'explosions) pour qu'ils fassent face à la caméra pour une meilleure représentation (seulement en mode 3D).\r\nNécessite plus de puissance.";
						SendMessage(g_hwndTT, TTM_SETTITLE, (WPARAM) 1, (LPARAM) (LPCTSTR) "BILLBOARDING");
					}
					else if (game_lang == 2)
					{
						ti.lpszText = "Ajusta la orientación de los objetos 2D (como las partículas) y además limita la cámara para el mejor ajuste (sólo en modo 3D).\r\nNecesita más CPU.";
						SendMessage(g_hwndTT, TTM_SETTITLE, (WPARAM) 1, (LPARAM) (LPCTSTR) "BILLBOARDING");
					}
				}
				else if (GetDlgItem(GetParent(hwnd), IDC_ANTI_ALIASING) == hwnd)
				{
					if (game_lang == 0)
					{
						ti.lpszText = "Enables Anti-aliasing, mainly for 3D objects for best visual quality.\r\nNeeds a compatible graphic card and more CPU / graphic card RAM.\r\n\r\n<A> key to swap ON/OFF during game.";
						SendMessage(g_hwndTT, TTM_SETTITLE, (WPARAM) 1, (LPARAM) (LPCTSTR) "ANTI-ALIASING");
					}
					else if (game_lang == 1)
					{
						ti.lpszText = "Active l'anti-crénelage, principalement pour les objets 3D pour une meilleure qualité visuelle.\r\nNécessite une carte vidéo compatible et plus de puissance/mémoire vidéo.\r\n\r\nTouche <A> pour activer/désactiver pendant le jeu.";
						SendMessage(g_hwndTT, TTM_SETTITLE, (WPARAM) 1, (LPARAM) (LPCTSTR) "ANTI-ALIASING");
					}
					else if (game_lang == 2)
					{
						ti.lpszText = "Permite el Anti-aliasing, principalmente para una mejor calidad visual en los objetos 3D.\r\nPresione la tecla <A> durante el juego para habilitar/deshabilitar.\r\nNecesita una tarjeta gráfica compatible y una mayor CPU / memoria RAM gráfica.";
						SendMessage(g_hwndTT, TTM_SETTITLE, (WPARAM) 1, (LPARAM) (LPCTSTR) "ANTI-ALIASING");
					}
				}
				else if (GetDlgItem(GetParent(hwnd), IDC_ANTI_ALIASING_MODE) == hwnd)
				{
					if (game_lang == 0)
					{
						ti.lpszText = "The quality/speed depends strongly on the video card driver.\r\n\r\nx2 (2 samples,2 taps) fastest mode.\r\nx2(Qx) (2,5) ~ speed of previous, quality of next.\r\nx4 (4,4) little nicer than previous but slower.\r\nx4S (4,9) nicer than previous but slower.";
						SendMessage(g_hwndTT, TTM_SETTITLE, (WPARAM) 1, (LPARAM) (LPCTSTR) "ANTI-ALIASING MODE");
					}
					else if (game_lang == 1)
					{
						ti.lpszText = "The quality/speed depends strongly on the video card driver.\r\n\r\nx2 (2 samples,2 taps) fastest mode.\r\nx2(Qx) (2,5) ~ speed of previous, quality of next.\r\nx4 (4,4) little nicer than previous but slower.\r\nx4S (4,9) nicer than previous but slower.";
						SendMessage(g_hwndTT, TTM_SETTITLE, (WPARAM) 1, (LPARAM) (LPCTSTR) "ANTI-ALIASING MODE");
					}
					else if (game_lang == 2)
					{
						ti.lpszText = "The quality/speed depends strongly on the video card driver.\r\n\r\nx2 (2 samples,2 taps) fastest mode.\r\nx2(Qx) (2,5) ~ speed of previous, quality of next.\r\nx4 (4,4) little nicer than previous but slower.\r\nx4S (4,9) nicer than previous but slower.";
						SendMessage(g_hwndTT, TTM_SETTITLE, (WPARAM) 1, (LPARAM) (LPCTSTR) "ANTI-ALIASING MODE");
					}

					// we enter the control
					if(!AntialiasingMode_over)
					{
						AntialiasingMode_over = true;
						InvalidateRect(hwnd, NULL, FALSE);
					}
				}
			 
			if (GetDlgItem(GetParent(hwnd), IDC_ANTI_ALIASING_MODE) == hwnd)
				SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(32649))); // Hand cursor

			// When the mouse enter the control
			if (!enter)
			{
				//OutputDebugString("enter");
				enter = true;
				
				// Update text
				SendMessage(g_hwndTT,TTM_UPDATETIPTEXT, 0, (LPARAM)&ti);			// update text

				// Activate TrackMouseEvent for receiving WM_MOUSEHOVER and WM_MOUSELEAVE messages
				tme.cbSize      = sizeof(TRACKMOUSEEVENT); // size
				tme.dwFlags     = TME_HOVER | TME_LEAVE;   // for hover and leave msgs
				tme.hwndTrack   = hwnd;                    // our window
				tme.dwHoverTime = 500;                     // time before display the tooltip

				if (!_TrackMouseEvent(&tme)) OutputDebugString("sio: TrackMouseEvent Failed");
			}

			// Send position
			if (is_shown)
			{
				GetCursorPos(&cur);
				SendMessage(g_hwndTT, TTM_TRACKPOSITION, 0, (LPARAM) MAKELPARAM(cur.x+20, cur.y+20));
			}

			// if we are changing of control quickly display the new tooltip immediately
			if (!is_shown)
				if (timeGetTime() - time_exit < 200)
				{
					is_shown = true;
					GetCursorPos(&cur);
					SendMessage(g_hwndTT, TTM_TRACKPOSITION, 0, (LPARAM) MAKELPARAM(cur.x+20, cur.y+20));// send position
					SendMessage(g_hwndTT, TTM_TRACKACTIVATE, (WPARAM)TRUE, (LPARAM)&ti);// activate
				}

			break;
		}

	case WM_MOUSEHOVER:
		{
			//OutputDebugString("hover");

			// Send position to the tooltip
			GetCursorPos(&cur);
			SendMessage(g_hwndTT, TTM_TRACKPOSITION, 0, (LPARAM) MAKELPARAM(cur.x+20, cur.y+20));
			
			// activate the tooltip
			if (!is_shown)
			{
				SendMessage(g_hwndTT, TTM_TRACKACTIVATE, (WPARAM)TRUE, (LPARAM)&ti);// activate the tracking tooltip
				//SendMessage(g_hwndTT,TTM_UPDATE, 0, 0);								// force update
				is_shown = true;
			}
		}
		break;
		
	case WM_MOUSELEAVE:
		{
			//OutputDebugString("leave");
			
			// Deactivate the tooltip
			SendMessage(g_hwndTT, TTM_TRACKACTIVATE, (WPARAM)FALSE, (LPARAM)&ti);

			if (is_shown)
				time_exit = timeGetTime();
			
			is_shown = false;
			enter    = false;

			// antialiasing button hover
			if (GetDlgItem(GetParent(hwnd), IDC_ANTI_ALIASING_MODE) == hwnd)
				if (AntialiasingMode_over)
				{
					AntialiasingMode_over = false;
					InvalidateRect(hwnd, NULL, TRUE);
				}
		}
		break;

		case WM_LBUTTONUP:
		//case WM_LBUTTONDBLCLK:
			{
				if (GetDlgItem(GetParent(hwnd), IDC_ANTI_ALIASING_MODE) == hwnd)
				{
					// change mode, resize/redraw the control
					game_option_anti_aliasing_mode++;
					if (game_option_anti_aliasing_mode > 3)
						game_option_anti_aliasing_mode = 0;

					if (game_option_anti_aliasing_mode == 0)
						SetWindowPos(hwnd,0,0,0,18,16,SWP_NOMOVE);
					else if (game_option_anti_aliasing_mode == 1)
						SetWindowPos(hwnd,0,0,0,37,16,SWP_NOMOVE);
					else if (game_option_anti_aliasing_mode == 2)
						SetWindowPos(hwnd,0,0,0,18,16,SWP_NOMOVE);
					else if (game_option_anti_aliasing_mode == 3)
						SetWindowPos(hwnd,0,0,0,23,16,SWP_NOMOVE);
					
					SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(32649))); // Hand cursor
				}
			}
			break;
		
		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK:
			if (GetDlgItem(GetParent(hwnd), IDC_ANTI_ALIASING_MODE) == hwnd)
				SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(32649))); // Hand cursor
			break;
	}

	// Let pass the other messages for the control
	return CallWindowProc(OrigControlProc, hwnd, message, wParam, lParam); 
}

//////////////////////////////////////////////////////////////////////////////////////
//
// CALLBACK - DIALOG BOX STARTUP (options)
//
//
BOOL CALLBACK DialogStartupProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam) 
{ 
	
	g_hwndDlg = hwndDlg;												// Global handle

	switch (message)
	{ 
	case WM_COMMAND: 
		switch (LOWORD(wParam)) 
		{ 
		case IDC_BUTTON_ABOUT:
			DialogBox(window.init.application->hInstance, MAKEINTRESOURCE(IDD_ABOUT), hwndDlg, (DLGPROC)DialogAboutProc);// Runs the About Dialog Box
			return TRUE;

		case IDCANCEL:												// close (escape) : hidden button (called automatically when the escape key is pressed)
			SendMessage(hwndDlg, WM_CLOSE, 0, 0);
			return TRUE;

		case IDC_STREAM:											// streaming
			game_option_stream = !game_option_stream;
			EnableWindow( GetDlgItem(hwndDlg,IDC_BITRATE), game_option_stream );
			if (game_option_stream)
				_beginthread( InternetConnect, 0, NULL );
			return TRUE;

		case IDC_BITRATE:											// bitrate
				game_option_bitrate = SendMessage(GetDlgItem(hwndDlg,IDC_BITRATE), CB_GETCURSEL, 0, 0);
			break; // so we can handle HIWORD(wParam) underneath : CBN_SELCHANGE

		case IDC_FULLSCREEN:										// Fullscreen
			if (IsDlgButtonChecked(hwndDlg, IDC_FULLSCREEN))
			{
				window.init.isFullScreen = TRUE;
				window.init.quickFullScreen = FALSE;
			}
			else
				window.init.isFullScreen = FALSE;
			break; // so we can handle HIWORD(wParam) underneath : BN_CLICKED

		case IDC_QUICK_FULLSCREEN:									// Quick fullscreen
			if (IsDlgButtonChecked(hwndDlg, IDC_QUICK_FULLSCREEN))
			{
				window.init.quickFullScreen = TRUE;
				window.init.isFullScreen = FALSE;
			}
			else
				window.init.quickFullScreen = FALSE;
			break; // so we can handle HIWORD(wParam) underneath : BN_CLICKED
		
		case IDC_WINDOW:											// Window
			if (IsDlgButtonChecked(hwndDlg, IDC_WINDOW))
			{
				window.init.isFullScreen = FALSE;
				window.init.quickFullScreen = FALSE;
			}
			break; // so we can handle HIWORD(wParam) underneath : BN_CLICKED

		case IDC_16BPP:												// 16 bpp
			if (IsDlgButtonChecked(hwndDlg, IDC_16BPP))
				window.init.bitsPerPixel	= 16;
			return TRUE;
		
		case IDC_32BPP:												// 32 bpp
			if (IsDlgButtonChecked(hwndDlg, IDC_32BPP))
				window.init.bitsPerPixel	= 32;
			return TRUE;

		case IDC_640_480:											// 640 x 480
			if (IsDlgButtonChecked(hwndDlg, IDC_640_480))
			{
				window.init.width			= 640;
				window.init.height			= 480;
			}
			return TRUE;
			
		case IDC_800_600:											// 800 x 600
			if (IsDlgButtonChecked(hwndDlg, IDC_800_600))
			{
				window.init.width		= 800;
				window.init.height		= 600;
			}
			return TRUE;

		case IDC_1024_768:											// 1024 x 768
			if (IsDlgButtonChecked(hwndDlg, IDC_1024_768))
			{
				window.init.width		= 1024;
				window.init.height		= 768;
			}
			return TRUE;

		case IDC_MIPMAP:											// mipmap
			if (IsDlgButtonChecked(hwndDlg, IDC_MIPMAP))
				game_option_mipmap = true;
			else game_option_mipmap = false;
			return TRUE;

		case IDC_VSYNC:												// VSync
			if (IsDlgButtonChecked(hwndDlg, IDC_VSYNC))
				game_option_vsync = true;
			else game_option_vsync = false;
			return TRUE;

		case IDC_BILLBOARDING:										// billboarding
			if (IsDlgButtonChecked(hwndDlg, IDC_BILLBOARDING))
				game_option_billboard = true;
			else game_option_billboard = false;
			return TRUE;

		case IDC_ANTI_ALIASING:										// antialiasing
			if (IsDlgButtonChecked(hwndDlg, IDC_ANTI_ALIASING))
			{
				game_option_anti_aliasing = true;
				EnableWindow(GetDlgItem(hwndDlg,IDC_ANTI_ALIASING_MODE), TRUE);
				InvalidateRect(GetDlgItem(hwndDlg,IDC_ANTI_ALIASING_MODE),NULL,TRUE);
			}
			else
			{
				game_option_anti_aliasing = false;
				EnableWindow(GetDlgItem(hwndDlg,IDC_ANTI_ALIASING_MODE), FALSE);
				InvalidateRect(GetDlgItem(hwndDlg,IDC_ANTI_ALIASING_MODE),NULL,TRUE);
			}
			return TRUE;

		case IDOK:
			// Write the options in registry
			set_options(&window);

			EndDialog(hwndDlg, wParam); 
			return TRUE; 
		}
		
		switch (HIWORD(wParam))
		{
		
		case CBN_SELCHANGE:											// combobox selection changed
			{
				// Custom Streaming URL -> Open DialogBox
				if (SendMessage(GetDlgItem(hwndDlg,IDC_BITRATE), CB_GETCURSEL, 0, 0) == 3)
				{
					custom_streaming_dialogbox = true; // for not launching music when DialogStartupProc will receive WM_ENTERIDLE (music only in 'about' DialogBox)
					DialogBoxParam(window.init.application->hInstance, MAKEINTRESOURCE(IDD_STREAMING), hwndDlg, (DLGPROC)DialogStreamProc, (LPARAM)message);
					custom_streaming_dialogbox = false;
				}
			}
			return TRUE;

		case BN_CLICKED:											// control clicked
			{
				// Activate/Deactivate display controls
				if (LOWORD(wParam) == IDC_FULLSCREEN || LOWORD(wParam) == IDC_WINDOW)
				{
					EnableWindow( GetDlgItem(hwndDlg,IDC_640_480) , TRUE );
					EnableWindow( GetDlgItem(hwndDlg,IDC_800_600) , TRUE );
					EnableWindow( GetDlgItem(hwndDlg,IDC_1024_768), TRUE );
					EnableWindow( GetDlgItem(hwndDlg,IDC_16BPP)   , TRUE );
					EnableWindow( GetDlgItem(hwndDlg,IDC_32BPP)   , TRUE );
				}
				if (LOWORD(wParam) == IDC_QUICK_FULLSCREEN)
				{
					EnableWindow( GetDlgItem(hwndDlg,IDC_640_480) , FALSE );
					EnableWindow( GetDlgItem(hwndDlg,IDC_800_600) , FALSE );
					EnableWindow( GetDlgItem(hwndDlg,IDC_1024_768), FALSE );
					EnableWindow( GetDlgItem(hwndDlg,IDC_16BPP)   , FALSE );
					EnableWindow( GetDlgItem(hwndDlg,IDC_32BPP)   , FALSE );
				}
			}
			return TRUE;
		}

		return FALSE;
		
	case WM_DRAWITEM:
		{
			LPDRAWITEMSTRUCT pdis = (LPDRAWITEMSTRUCT) lParam;
			
			switch (pdis->CtlID)
			{
			case IDC_ANTI_ALIASING_MODE:
				{
					RECT rc = {0,0,0,16}; //left,top,right,bottom

					// set text and background colors
					if (!game_option_anti_aliasing)
						SetTextColor(pdis->hDC, RGB(161,161,146));
					else if (pdis->itemState & ODS_SELECTED) // button down
					{
						SetTextColor(pdis->hDC, RGB(255,255,255));
						FillRect(pdis->hDC, &pdis->rcItem, CreateSolidBrush(RGB(30, 30, 30))); // paint the entire area under the control
						SetBkColor(pdis->hDC,RGB(30,30,30)); // paint the background text
					}
					else if (AntialiasingMode_over) 
					{
						SetTextColor(pdis->hDC, RGB(60,60,60));
						FillRect(pdis->hDC, &pdis->rcItem, CreateSolidBrush(RGB(255, 255, 255))); // paint the entire area under the control
						SetBkColor(pdis->hDC,RGB(255,255,255)); // paint the background text
					}
					else  SetTextColor(pdis->hDC, RGB(60,60,60));
					
					// set the text
					if (game_option_anti_aliasing_mode == 0)
					{rc.right=18;TextOut(pdis->hDC, 3, 1, "x2", 2);}
					else if (game_option_anti_aliasing_mode == 1)
					{rc.right=37;TextOut(pdis->hDC, 3, 1, "x2(Qx)", 6);}
					else if (game_option_anti_aliasing_mode == 2)
					{rc.right=18;TextOut(pdis->hDC, 3, 1, "x4", 2);}
					else if (game_option_anti_aliasing_mode == 3)
					{rc.right=23;TextOut(pdis->hDC, 3, 1, "x4S", 3);}

					// draw the frame around the text
					if (AntialiasingMode_over)
						FrameRect(pdis->hDC, &rc, CreateSolidBrush(RGB(45, 45, 45)));
					else
						FrameRect(pdis->hDC, &rc, CreateSolidBrush(RGB(208, 208, 191)));
				}
				break;
			}
		}
		return TRUE;

	case WM_MOUSEMOVE:												// mouse move
		{
			int xPos = LOWORD(lParam);  // horizontal position of cursor 
			int yPos = HIWORD(lParam);  // vertical position of cursor 

			if (xPos>5 && xPos<26 && yPos>94 && yPos<217 && !dialogbox_internet_over)
			{
				//ShowWindow( GetDlgItem(hwndDlg,IDB_STARTUP_INTERNET), FALSE);
				ShowWindow( GetDlgItem(hwndDlg,IDB_STARTUP_INTERNET_OVER), TRUE);
				//InvalidateRect(GetDlgItem(hwndDlg,IDB_STARTUP_INTERNET_OVER), NULL, TRUE);
				dialogbox_internet_over = true;
				SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(32649))); // Hand cursor
			}
			else if ((xPos>5  && xPos<26 && yPos>94 && yPos<217)	 // Link homepage
				  || (xPos>2  && xPos<24 && yPos>3 && yPos<16)		 // French flag
				  || (xPos>25 && xPos<48 && yPos>3 && yPos<16)		 // English flag
				  || (xPos>50 && xPos<72 && yPos>3 && yPos<16))		 // Spanish Flag
				SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(32649))); // Hand cursor
			else if (dialogbox_internet_over)
			{
				//ShowWindow( GetDlgItem(hwndDlg,IDB_STARTUP_INTERNET), TRUE);
				ShowWindow( GetDlgItem(hwndDlg,IDB_STARTUP_INTERNET_OVER), FALSE);
				//InvalidateRect(GetDlgItem(hwndDlg,IDB_STARTUP_INTERNET_OVER), NULL, TRUE);
				dialogbox_internet_over = false;
			}
		}
		return TRUE;

	case WM_LBUTTONDOWN:											// left button down
		{
			int xPos = LOWORD(lParam);  // horizontal position of cursor 
			int yPos = HIWORD(lParam);  // vertical position of cursor 

			if (xPos>5 && xPos<26 && yPos>94 && yPos<217)
			{
				ShowWindow( GetDlgItem(hwndDlg,IDB_STARTUP_INTERNET), TRUE);
				ShowWindow( GetDlgItem(hwndDlg,IDB_STARTUP_INTERNET_OVER), FALSE);
				SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(32649))); // Hand cursor
			}
			else if (xPos>2  && xPos<24 && yPos>3 && yPos<16)		 // French flag
			{
				SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(32649))); // Hand cursor
				if (game_lang == 1) return TRUE;
				game_lang = 1;
				translate(0, hwndDlg);
			}
			else if (xPos>25 && xPos<48 && yPos>3 && yPos<16)		 // English flag
			{
				SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(32649))); // Hand cursor
				if (game_lang == 0) return TRUE;
				game_lang = 0;
				translate(0, hwndDlg);
			}
			else if (xPos>50 && xPos<72 && yPos>3 && yPos<16)		 // Spanish Flag
			{
				SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(32649))); // Hand cursor
				if (game_lang == 2) return TRUE;
				game_lang = 2;
				translate(0, hwndDlg);
			}
			else
				// Drag window by clicking anywhere on it
				PostMessage(hwndDlg, WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( xPos, yPos));
		}
		return TRUE;

	case WM_LBUTTONUP:												// left button up
	case WM_LBUTTONDBLCLK:
		{
			int xPos = LOWORD(lParam);  // horizontal position of cursor 
			int yPos = HIWORD(lParam);  // vertical position of cursor 

			if (xPos>5 && xPos<26 && yPos>94 && yPos<217)
			{
				SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(32649))); // Hand cursor
				ShellExecute(NULL, "open", "http://spaceinvadersgl.sf.net", NULL, NULL, SW_SHOWNORMAL);
			}
			else if ((xPos>2  && xPos<24 && yPos>3 && yPos<16)		 // French flag
				  || (xPos>25 && xPos<48 && yPos>3 && yPos<16)		 // English flag
				  || (xPos>50 && xPos<72 && yPos>3 && yPos<16))		 // Spanish Flag
				SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(32649))); // Hand cursor
		}
		return TRUE;

	case WM_CLOSE:													// close (X, Alt+F4) also called for escape key (see IDCANCEL:)
		{
			FMUSIC_SetMasterVolume(my_mod_about,0);
			FMUSIC_StopSong(my_mod_about);
			FMUSIC_FreeSong(my_mod_about);
			FSOUND_Close();

			// Write the options in registry
			set_options(&window);

			EndDialog(hwndDlg, TRUE);
			exit(0);
		}

	case WM_INITDIALOG:												// init dialog
		{
			// ASSOCIE UNE ICÔNE A LA "WINDOW CLASS" DE LA DIALOGBOX
			// (POUR AFFICHAGE LORS DE <ALT>+<TAB> ET PETITE ICÔNE
			// EN HAUT À GAUCHE DE CETTE FENÊTRE ET DE SES FILLES)
			SetClassLong (hwndDlg, GCL_HICON, (LONG)LoadIcon (window.init.application->hInstance, MAKEINTRESOURCE (IDI_ICON_APP)));
		
			// --- START WIVIK : RETRIEVE GAME_OPTION_HOLDDOWN_WIVIK FROM REGISTRY (FOR WIVIK ON-SCREEN KEYBOARD)
			HKEY key; DWORD disposition;
			int  nombre = 0;
			// check key
			if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\SpaceInvaders-OpenGL\\", 0, KEY_READ, &key) == ERROR_SUCCESS)
			{
				DWORD type = REG_DWORD;	DWORD size = sizeof(DWORD);
				RegQueryValueEx(key, "game_option_holddown_wivik", NULL, &type, (LPBYTE)&nombre, &size);
				size = sizeof(bool);
				RegQueryValueEx(key, "game_option_wivik", NULL, &type, (LPBYTE)&game_option_wivik, &size);
				if (game_option_wivik == NULL) game_option_wivik = 0;
			}
			else
				// create key
				RegCreateKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\SpaceInvaders-OpenGL\\", 0, "", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &key, &disposition);
			RegCloseKey(key);

			if (nombre == 0) game_option_holddown_wivik = 100;
			else game_option_holddown_wivik = nombre;
			// --- END WIVIK ---

			// GET STARTUP WINDOW OPTIONS (FROM REGISTRY)
			get_options(&window);

			// INITIALISATION DES CONTROLES (BOUTONS RADIO, CHECKBOX)
			if (window.init.quickFullScreen)
				CheckRadioButton(hwndDlg, IDC_QUICK_FULLSCREEN, IDC_WINDOW, IDC_QUICK_FULLSCREEN);
			else if (window.init.isFullScreen)
				CheckRadioButton(hwndDlg, IDC_FULLSCREEN, IDC_WINDOW, IDC_FULLSCREEN); // should be quickfullscreen, window, fullscreen ??
			else
				CheckRadioButton(hwndDlg, IDC_QUICK_FULLSCREEN, IDC_WINDOW, IDC_WINDOW);
			
			if (window.init.width == 640)
				CheckRadioButton(hwndDlg, IDC_640_480, IDC_1024_768, IDC_640_480);
			else if (window.init.width == 800)
				CheckRadioButton(hwndDlg, IDC_640_480, IDC_1024_768, IDC_800_600);
			else if (window.init.width == 1024)
				CheckRadioButton(hwndDlg, IDC_640_480, IDC_1024_768, IDC_1024_768);

			if (window.init.bitsPerPixel == 16)
				CheckRadioButton(hwndDlg, IDC_16BPP, IDC_32BPP, IDC_16BPP);
			else
				CheckRadioButton(hwndDlg, IDC_32BPP, IDC_32BPP, IDC_32BPP);

			// To show/hide radio buttons for resolution and bbp
			SendMessage(hwndDlg, WM_COMMAND, MAKELONG(NULL, BN_CLICKED), (LPARAM)hwndDlg);

			CheckDlgButton(hwndDlg, IDC_MIPMAP, game_option_mipmap);
			CheckDlgButton(hwndDlg, IDC_VSYNC, game_option_vsync);
			CheckDlgButton(hwndDlg, IDC_BILLBOARDING, game_option_billboard);
			CheckDlgButton(hwndDlg, IDC_ANTI_ALIASING, game_option_anti_aliasing);
			if (!game_option_anti_aliasing)
				EnableWindow(GetDlgItem(hwndDlg,IDC_ANTI_ALIASING_MODE), FALSE);

			if (game_option_stream)
			{
				CheckDlgButton(hwndDlg, IDC_STREAM, game_option_stream);
				EnableWindow( GetDlgItem(hwndDlg,IDC_BITRATE), TRUE);
				//_beginthread( InternetConnect, 0, NULL );
			}

			SendMessage(GetDlgItem(hwndDlg,IDC_BITRATE), CB_ADDSTRING, 0, (LPARAM) "DSL");
			SendMessage(GetDlgItem(hwndDlg,IDC_BITRATE), CB_ADDSTRING, 0, (LPARAM) "ISDN");
			SendMessage(GetDlgItem(hwndDlg,IDC_BITRATE), CB_ADDSTRING, 0, (LPARAM) "Dialup");
			SendMessage(GetDlgItem(hwndDlg,IDC_BITRATE), CB_ADDSTRING, 0, (LPARAM) "Custom");
			SendMessage(GetDlgItem(hwndDlg,IDC_BITRATE), CB_SETCURSEL, game_option_bitrate, 0);
			
			// CREATION DES TOOLTIPS (INFOS BULLES SUR LES CONTROLES)
/*			g_hwndTT = CreateWindow(TOOLTIPS_CLASS, TEXT(""),
                          WS_POPUP,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          NULL, (HMENU)NULL, window.init.application->hInstance,
                          NULL);
*/
			 g_hwndTT = CreateWindowEx(WS_EX_TOPMOST,
					TOOLTIPS_CLASS,
					NULL,
					WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
					CW_USEDEFAULT,
					CW_USEDEFAULT,
					CW_USEDEFAULT,
					CW_USEDEFAULT,
					hwndDlg,
					NULL, window.init.application->hInstance,
					NULL
					);

			// paramètres
			if (game_lang == 0)
				SendMessage(g_hwndTT, (UINT) TTM_SETMAXTIPWIDTH, 0, 200);			// width of 200 pixels
			else if (game_lang == 1)
				SendMessage(g_hwndTT, (UINT) TTM_SETMAXTIPWIDTH, 0, 220);			// width of 200 pixels
			else if (game_lang == 2)
				SendMessage(g_hwndTT, (UINT) TTM_SETMAXTIPWIDTH, 0, 220);			// width of 200 pixels
//			SendMessage(g_hwndTT, TTM_SETDELAYTIME, (WPARAM)TTDT_AUTOPOP, 500);// duration of X ms
//			SendMessage(g_hwndTT, TTM_SETDELAYTIME, (WPARAM)TTDT_INITIAL, 500);// appears after X ms

//			COLORREF text_color = RGB(255,255,255); SendMessage(g_hwndTT, (UINT) TTM_SETTIPTEXTCOLOR, (COLORREF) text_color, 0);
//			COLORREF text_color = RGB(0,0,0); SendMessage(g_hwndTT, (UINT) TTM_SETTIPTEXTCOLOR, (COLORREF) text_color, 0);
			COLORREF text_color = RGB(50,50,50); SendMessage(g_hwndTT, (UINT) TTM_SETTIPTEXTCOLOR, (COLORREF) text_color, 0);
//			COLORREF bg_color = RGB(255,255,255);   SendMessage(g_hwndTT, (UINT) TTM_SETTIPBKCOLOR, (COLORREF) bg_color, 0);
//			COLORREF bg_color = RGB(15,21,23);   SendMessage(g_hwndTT, (UINT) TTM_SETTIPBKCOLOR, (COLORREF) bg_color, 0);
			RECT margin_size = {-3,-3,-3,-3};       SendMessage(g_hwndTT, (UINT) TTM_SETMARGIN, 0, (LPARAM) (LPRECT) &margin_size);
			// ChildWindowFromPoint()
			// toolinfo
			ti.cbSize = sizeof(TOOLINFO);
			ti.uFlags = TTF_IDISHWND | TTF_TRACK | TTF_ABSOLUTE | TTF_SUBCLASS;
			ti.hwnd   = hwndDlg;
			ti.hinst  = window.init.application->hInstance;
			ti.rect.left = ti.rect.top = ti.rect.bottom = ti.rect.right = 0; 

			// add tooltip
			SendMessage(g_hwndTT, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &ti);

			// we subclass the controls to intercept the messages (DialogControlsProc procedure)
			OrigControlProc = (WNDPROC) SetWindowLong(GetDlgItem(hwndDlg, IDC_MIPMAP), GWL_WNDPROC, (LONG_PTR)DialogControlsProc);
			OrigControlProc = (WNDPROC) SetWindowLong(GetDlgItem(hwndDlg, IDC_VSYNC), GWL_WNDPROC, (LONG_PTR)DialogControlsProc);
			OrigControlProc = (WNDPROC) SetWindowLong(GetDlgItem(hwndDlg, IDC_STREAM), GWL_WNDPROC, (LONG_PTR)DialogControlsProc);
			OrigControlProc = (WNDPROC) SetWindowLong(GetDlgItem(hwndDlg, IDC_BILLBOARDING), GWL_WNDPROC, (LONG_PTR)DialogControlsProc);
			OrigControlProc = (WNDPROC) SetWindowLong(GetDlgItem(hwndDlg, IDC_ANTI_ALIASING), GWL_WNDPROC, (LONG_PTR)DialogControlsProc);
			OrigControlProc = (WNDPROC) SetWindowLong(GetDlgItem(hwndDlg, IDC_ANTI_ALIASING_MODE), GWL_WNDPROC, (LONG_PTR)DialogControlsProc);

			// set the antialiasing_mode button the good size
			HWND hwnd_temp = GetDlgItem(hwndDlg, IDC_ANTI_ALIASING_MODE);
			
			if (game_option_anti_aliasing_mode == 0)
				SetWindowPos(hwnd_temp,0,0,0,18,16,SWP_NOMOVE);
			else if (game_option_anti_aliasing_mode == 1)
				SetWindowPos(hwnd_temp,0,0,0,37,16,SWP_NOMOVE);
			else if (game_option_anti_aliasing_mode == 2)
				SetWindowPos(hwnd_temp,0,0,0,18,16,SWP_NOMOVE);
			else if (game_option_anti_aliasing_mode == 3)
				SetWindowPos(hwnd_temp,0,0,0,23,16,SWP_NOMOVE);

			// Translate the DialogBox
			translate(0, hwndDlg);

			Fps_Init();
		}
		return TRUE;
		
	case WM_ENTERIDLE: // message received when a new dialogbox is shown (about, custom stream)
		if (!my_mod_about && !custom_streaming_dialogbox)
		{
			// extract fmod.dll
			fmod_extract ();								// not to be removed
			
			// start fmod
			if (!FSOUND_Init(44100, 32, 0))
			{
				MessageBox(0, FMOD_ErrorString(FSOUND_GetError()), "Sound Initialization Error", MB_TOPMOST);
				FSOUND_Close();
			}
			
			// extract music from resources
			HRSRC res					= FindResource(NULL, MAKEINTRESOURCE(IDR_SOUND_XM_ABOUT), "WAVE");
			HGLOBAL hres				= LoadResource(NULL, res);
			void * data					= LockResource(hres);
			unsigned long int length	= SizeofResource(NULL, res);

			if (!(my_mod_about = FMUSIC_LoadSongEx((const char*)data, 0, length, FSOUND_LOADMEMORY, NULL, 0)))
				FMOD_ErrorString(FSOUND_GetError());

			UnlockResource(hres);
			FreeResource(hres);

			// fade in music
			my_mod_about_working = true;
			_beginthread( FmodFadingIn, 0, NULL );
			my_mod_about_on = true;
		}
		return TRUE;
	} 
	
	return FALSE; // DialogBox procedure must return FALSE for messages not handled.
} 

//////////////////////////////////////////////////////////////////////////////////////
//
// PROGRAM ENTRY (WINMAIN)
//
//
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Application			application;									// Application Structure"
	Keys				keys;											// Key Structure
	BOOL				isMessagePumpActive;							// Message Pump Active?
	MSG					msg;											// Window Message Structure

	// FMOD ex-DLL INITIALISATION
#ifndef CONFIG_FMOD_EXTRACT_YES
	FMOD_DllMain(GetModuleHandle(NULL), DLL_PROCESS_ATTACH, NULL);
#endif

	// Fill Out Application Data
	application.className = "OpenGL";									// Application Class Name
	application.hInstance = hInstance;									// Application Instance

	// Fill Out Window Data
	ZeroMemory(&window, sizeof(GL_Window));								// Zero 'window' Structure
	window.keys					= &keys;								// Window Key Structure
	window.init.application		= &application;							// Window Application
	window.init.title			= "Space Invaders OpenGL";	            // Window Title
	window.init.width			= 640;									// Default Window Width (modified by startup screen)
	window.init.height			= 480;									// Default Window Height (modified by startup screen)
	window.init.bitsPerPixel	= 16;									// Default Bits Per Pixel (modified by startup screen)
	window.init.isFullScreen	= TRUE;									// Default Fullscreen? (Set To TRUE)
	window.init.quickFullScreen	= TRUE;									// Default quickFullscreen? (Set To TRUE)

	ZeroMemory(&keys, sizeof(Keys));									// Zero 'keys' Structure

	// Initialize Common Controls
	INITCOMMONCONTROLSEX icex;											// Initialize Windows XP Common Controls
	
	// Load the ToolTip class from the DLL
    icex.dwSize = sizeof(icex);
    icex.dwICC  = ICC_BAR_CLASSES;										// For Tooltips

	if(!InitCommonControlsEx(&icex))
	   BOX("InitCommonControlEx() failed.");

	// Register A Class For Our Window To Use
	if (RegisterWindowClass(&application) == FALSE)						// Did Registering A Class Fail?
	{
		// Failure
		MessageBox(HWND_DESKTOP, "Error Registering Window Class!", "Error", MB_OK | MB_ICONEXCLAMATION);
		return -1;														// Terminate Application
	}

	//if (IsCardAccelerated())
	//	MessageBox(0,"carte graphique ok","",0);

	// Runs the Startup DialogBox
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_STARTUP), HWND_DESKTOP, (DLGPROC)DialogStartupProc); 

	OutputDebugString("sio: Space Invaders OpenGL [Start]");

	// Get the resolution for "Quick FullScreen" mode
	if (window.init.quickFullScreen == TRUE && window.init.isFullScreen == FALSE)// If Quick Fullscreen, we catch the screen resolution
	{
		HWND	hDesktop = GetDesktopWindow();
		HDC	hDc = GetDC(hDesktop);
		window.init.width = GetDeviceCaps(hDc, PLANES) * GetDeviceCaps(hDc, HORZRES);
		window.init.height = GetDeviceCaps(hDc, PLANES) * GetDeviceCaps(hDc, VERTRES);
		ReleaseDC(hDesktop, hDc);
	}

	g_isProgramLooping = TRUE;											// Program Looping Is Set To TRUE
	g_createFullScreen = window.init.isFullScreen;						// g_createFullScreen Is Set To User Default
	
	cur_hand		= LoadCursor(application.hInstance, MAKEINTRESOURCE(IDC_CURSOR_HAND));
	cur_hand_closed	= LoadCursor(application.hInstance, MAKEINTRESOURCE(IDC_CURSOR_HAND_CLOSED));

	while (g_isProgramLooping)											// Loop Until WM_QUIT Is Received
	{
		// Create A Window
		window.init.isFullScreen = g_createFullScreen;					// Set Init Param Of Window Creation To Fullscreen?
		if (CreateWindowGL(&window) == TRUE)							// Was Window Creation Successful?
		{
			// At This Point We Should Have A Window That Is Setup To Render OpenGL
			if (Initialize(&window, &keys) == FALSE)					// Call User Intialization
			{
				// Failure
				TerminateApplication(&window);							// Close Window, This Will Handle The Shutdown
			}
			else														// Otherwise (Start The Message Pump)
			{	// Initialize was a success
				isMessagePumpActive = TRUE;								// Set isMessagePumpActive To TRUE

				while (isMessagePumpActive == TRUE)						// While The Message Pump Is Active
				{
					// Success Creating Window.  Check For Window Messages
					if (PeekMessage(&msg, window.hWnd, 0, 0, PM_REMOVE) != 0)
					{
						// Check For WM_QUIT Message
						if (msg.message != WM_QUIT)						// Is The Message A WM_QUIT Message?
						{
							DispatchMessage(&msg);						// If Not, Dispatch The Message
						}												// to WindowProc()
						else											// Otherwise (If Message Is WM_QUIT)
						{
							isMessagePumpActive = FALSE;				// Terminate The Message Pump
						}
					}
					else												// If There Are No Messages
					{
						if (window.isVisible == FALSE)					// If Window Is Not Visible
						{
							WaitMessage();								// Application Is Minimized Wait For A Message (paused)
						}
						else											// If Window Is Visible
						{
							static char debugprint[300]="";
							static int loop=0;

							// ETAPES DU JEU (parties 1,2,3 du chargement - puis le jeu)

							switch (game_step)
							{
							case 1:
								{
									loading_display1();					// Screen fading In
									Update(0, &window);					// Update
								}
								break;

							case 2:
								{
									if (!game_loading)
									{
										// CREATION D'UN THREAD POUR LE CHARGEMENT
										wglMakeCurrent (NULL, NULL);
										h_thread2 = _beginthread( loading, 0, NULL );
										game_loading = true;
									}

									// To avoid making too loops in THIS THREAD while the LOADING THREAD is working,
									// but permit anyway to handle messages to move or close the window during loading.
									// => 400 loops against 2,500,000 on my pc ! much much faster this way !
									// This is because there're 2 threads of course and we don't want THIS
									// THREAD to take too much cpu (the Sleep() function give its time slice so it's ok)
									Sleep(5);

									Update(0, &window);					// Update
								}
								break;

							case 3:
								{
									loading_display3();					// Screen fading Out
									Update(0, &window);					// Update
								}
								break;

							case 4:
								{
									Fps_Update();
	
									Update(0, &window);				// Update
									GameLogic(&window);				// Game Logic
									Draw(&window);					// Draw
								}									
								break;
							}

							if (game_step != 2)							// game_step function (loading) has its own swapbuffers
								SwapBuffers(window.hDC);				// Swap Buffers (Double Buffering)
						}
					}
				}														// Loop While isMessagePumpActive == TRUE
			}															// If (Initialize (...

			// Application Is Finished
			Deinitialize();												// User Defined DeInitialization

			DestroyWindowGL(&window);									// Destroy The Active Window
		}
		else															// If Window Creation Failed
		{
			// Error Creating Window
			MessageBox(HWND_DESKTOP, "Error Creating OpenGL Window", "Error", MB_OK | MB_ICONEXCLAMATION);
			g_isProgramLooping = FALSE;									// Terminate The Loop
		}
	}																	// While (isProgramLooping)

	UnregisterClass(application.className, application.hInstance);		// UnRegister Window Class
	return 0;
}																		// End Of WinMain()
