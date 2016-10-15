// si_Functions.h - space invaders opengl

#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

//////////////////////////////////////////////////////////////////////////////////////
//
// FONCTIONS => Divers
//
//

// COLLISIONS (entre deux rectangles)
int collision (float pg, float pd, float ph, float pb, float sg, float sd, float sh, float sb);

// COURBE GENERALE DE BEZIER
typedef struct
{
	float x;
	float y;
	float z;
}
point3f;
extern point3f control_points[6];
extern point3f pcamera;

// *.PLS (WINAMP PLAYLIST) PARSER FOR STREAMS
typedef struct
{
    int    count;
    void **name;
    void **displayname;
} Playlist;
Playlist *ParsePlaylist(char *name);

// TRANSLATE GUI
void translate(int WhichDialogBox, HWND hwndDlg);

// BEZIER CURVE FOR CAMERA MOVEMENT TO 3D
point3f Bezier(point3f *p,int n,double mu);

// KEY_WAIT (gère le délai entre deux frappes de touches)
int key_wait(int milliseconds, float & key_timer);
int wivik_wait(int milliseconds, float & key_timer);

// HISCORE
int  get_hiscore();
void set_hiscore(int score);

// OPTIONS
int get_options(GL_Window* g_window);
void set_options(GL_Window* g_window);

// CARTE GRAPHIQUE ACCÉLÉRÉE ?
bool IsCardAccelerated();

// HYPERLIENS
BOOL ConvertStaticToHyperlink(HWND hwndCtl);
BOOL ConvertStaticToHyperlink(HWND hwndParent, UINT uiCtlId);

void fontColor();

// SON LORS DE L'APPUI SUR UNE TOUCHE D'OPTION
void key_sound();

// INTERNET AUDIO STREAMING => FADING IN (VOLUME UP)
void FmodStreamFadingIn(void* dummy);

// ARB MUTISAMPLE (DEBUT) ------------------------
#include "gl/wglext.h"	//WGL extensions
#include "gl/glext.h"	//GL extensions

//Globals
extern bool	arbMultisampleSupported;
extern int arbMultisampleFormat;

//to check for our sampling
bool InitMultisample(HINSTANCE hInstance,HWND hWnd,PIXELFORMATDESCRIPTOR pfd);
// ARB MULTISAMPLE (FIN) -------------------------

// ERROR HANDLING
void DisplayMsgBoxErrorAndExit(LPCTSTR szFile, int nLine, LPCTSTR lpMessage);
void DisplayLastErrorDebug(void);
void DisplayLastErrorBox(void);

#endif