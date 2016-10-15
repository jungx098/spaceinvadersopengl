// si_Functions.cpp - space invaders opengl

#include "si_Main.h"
#include "si_Loading.h"
#include <Wininet.h>			// Header File For Wininet library

#define LOG0(text) add_log(text, false);		// MACRO de LOG (ajout d'une entrée dans les logs, _NON_ pris en compte pour la progress bar)
#define LOG(text)  add_log(text, true);			// MACRO de LOG (ajout d'une entrée dans les logs, prise en compte pour la progress bar)

//////////////////////////////////////////////////////////////////////////////////////
//
// FONCTIONS => Gère une collision entre 2 rectangles (2 sprites)
//
//
int collision (float pg, float pd, float ph, float pb, float sg, float sd, float sh, float sb)
{
	if (
		( ((pg < sg) && (sg < pd)) || ((pg < sd) && (sd < pd)) )
		&&
		( ((pb < sb) && (sb < ph)) || ((pb < sh) && (sh < ph)) )
		)
		return 1;

	else if (
		( ((sg < pg) && (pg < sd)) || ((sg < pd) && (pd < sd)) )
		&&
		( ((sb < pb) && (pb < sh)) || ((sb < ph) && (ph < sh)) )
		)
		return 1;

	else return 0;
}

//////////////////////////////////////////////////////////////////////////////////////
//
// FONCTIONS => Gère le délai nécessaire entre 2 frappes de touche
//
//
int key_wait(int milliseconds, float & key_timer)		// gère les répétitions de frappes de touches
{
	if ((TimerGetTime() - key_timer) < milliseconds)	// le temps écoulé entre 2 pressions
		return 0;										// d'une touche doit être > à milliseconds
	else
	{
		key_timer = TimerGetTime();
		return 1;
	}
}

// THIS IS FOR COMPATIBILITY WITH WIVIK ON-SCREEN KEYBOARD
int wivik_wait(int milliseconds, float & key_timer)		// gère les répétitions de frappes de touches
{
	static bool registered = false;

	if (TimerGetTime() - key_timer < milliseconds)		// le temps écoulé entre 2 pressions
		return 0;										// d'une touche doit être > à milliseconds
	
	else if (!registered)
	{
		key_timer = TimerGetTime();
		registered = true;
		return 0;
	}
	else
	{
		key_timer = TimerGetTime();
		registered = false;
		return 1;
	}
}



// For use with ParsePlaylist() underneath
char *stristr(char *string2, char *string1)
{
    char *s1 = _strdup(string1);
    char *s2 = _strdup(string2);
    char *ret = 0;
    int i, j;
    int len1 = strlen(string1);
    int len2 = strlen(string2);

    for (i=0;s1[i];i++) s1[i] = tolower(s1[i]);
    for (i=0;s2[i];i++) s2[i] = tolower(s2[i]);

    for (j=0;j < (len2 - len1);j++)
    {
        char *a = s1;
        char *b = &s2[j];

        for (i=0;(i < len1) && *a && *b;a++, b++, i++)
        {
            if (*a != *b)
            {
                break;
            }
        }

        if (i == len1)
        {
            ret = &string2[j];
            break;
        }
    }

    free(s1);
    free(s2);
    return ret;
}

//////////////////////////////////////////////////////////////////////////////////////
//
// FONCTIONS => Parse un fichier .pls (Winamp playlist)
//
//
Playlist *ParsePlaylist(char *name)
{
    char        *filebuf, *p;
    Playlist    *playlist = 0;
	int          count, i;

	// Check if URL null
    if (!name)
    {
        BOX("Streaming URL is empty");
		return 0;
    }
		
	// Check if URL is a *.pls file
	if (_stricmp((const char *)".pls", (const char *)(&name[strlen(name) - 4])))
    {
		return 0;
    }

    // Create a WinInet session
    HINTERNET hSession = InternetOpen(_T(""), INTERNET_OPEN_TYPE_PRECONFIG, 0, 0, 0);

    // Prepare a request
	HINTERNET hRequest = InternetOpenUrl(hSession, name, 0, 0, 0, 0);

    // Send the HTTP request.
    HttpSendRequest(hRequest, 0, 0, 0, 0);

    // Read the response data.
    DWORD dwNumberOfBytesAvailable = 0;

    do
    {
        // Retrieve size of remote file
		InternetQueryDataAvailable(hRequest, &dwNumberOfBytesAvailable, 0, 0);

        if (0 < dwNumberOfBytesAvailable)
        {
			// Allocate buffer memory
			filebuf = new char[dwNumberOfBytesAvailable + 1];
            filebuf[dwNumberOfBytesAvailable] = 0;

            DWORD dwBytesRead = 0;

            // Read remote file and put in buffer
			InternetReadFile(hRequest,
                               filebuf,
                               dwNumberOfBytesAvailable,
                               &dwBytesRead);

			//BOX(filebuf);
        }
    }
    while (0 < dwNumberOfBytesAvailable);

    // Close the request handle.
    InternetCloseHandle(hRequest);

    // Close the WinInet session.
    InternetCloseHandle(hSession);

	// Parse the pls file (from Fmod sample)
    p = stristr(filebuf, "NumberOfEntries=");
    if (!p)
    {
		BOX("Streaming : NumberOfEntries in pls file not found");
		return 0;
    }

    p += 16;
    count = atoi(p);

    if (!count)
    {
		BOX("Streaming : numeric value for NumberOfEntries not found");
		return 0;
    }

    playlist = (Playlist *)calloc(sizeof(Playlist), 1);
    if (!playlist)
    {
		BOX("Streaming : can't allocate memory for pls file (playlist)");
		return 0;
    }

    playlist->count = count;

	playlist->name = (void **)calloc(sizeof(char *) * count, 1);
    if (!playlist->name)
    {
		BOX("Streaming : can't allocate memory for pls file (playlist->name)");
		return 0;
    }

    playlist->displayname = (void **)calloc(sizeof(char *) * count, 1);
    if (!playlist->displayname)
    {
		BOX("Streaming : can't allocate memory for pls file (playlist->displayname)");
		return 0;
    }

    for (i=0;i < count;i++)
    {
        char tmp[32];
        char *filename, *displayname, *t;

        filename = displayname = 0;

        sprintf(tmp, "File%d=", i + 1);
        p = strstr(filebuf, tmp);
        if (p)
        {
            p += strlen(tmp);
            t = p;
            for (;*t && (*t != 0xa) && (*t != 0xd);t++);
            if (*t)
            {
                char tmpc = *t;
                *t = 0;
                filename = _strdup(p);
                *t = tmpc;
            }
        }

        if (strncmp(filename, "http://", 7) && strncmp(filename, "http:\\\\", 7))
        {
            char *tmpname = _strdup(filename);
            p = &tmpname[strlen(tmpname) - 1];
            for (;(p > tmpname) && (*p != '\\') && (*p != '/');p--);
            if ((*p == '\\') || (*p == '/'))
            {
                p++;
            }
            displayname = _strdup(p);
            free(tmpname);
        }

        playlist->name[i] = filename;
        playlist->displayname[i] = displayname;
    }

    free(filebuf);
    return playlist;

}

//////////////////////////////////////////////////////////////////////////////////////
//
// TRANSLATE GUI WhichDialogBox : 0=main startup, 1=about, 2=custom streaming
//
// 
void translate(int WhichDialogBox, HWND hwndDlg)
{
	if (game_lang == 0) // english
	{
		if (WhichDialogBox == 0)
		{
			SetDlgItemText(hwndDlg, IDC_STATIC_DISPLAY, "Display");
			SetDlgItemText(hwndDlg, IDC_QUICK_FULLSCREEN, "Quick FullScreen (current mode)");
			SetDlgItemText(hwndDlg, IDC_FULLSCREEN, "FullScreen");
			SetDlgItemText(hwndDlg, IDC_WINDOW, "Window");
			SetDlgItemText(hwndDlg, IDC_STATIC_MISC, "Misc.");
			SetDlgItemText(hwndDlg, IDC_VSYNC, "VSync");
			SetDlgItemText(hwndDlg, IDC_STREAM, "Streaming");
			SetDlgItemText(hwndDlg, IDC_STATIC_QUALITY, "Quality");
			SetDlgItemText(hwndDlg, IDC_ANTI_ALIASING, "Antialiasing");
			SetDlgItemText(hwndDlg, IDC_BILLBOARDING, "Billboarding");
			SetDlgItemText(hwndDlg, IDC_MIPMAP, "Mipmap Textures");
			SetDlgItemText(hwndDlg, IDOK, "Start");
			SendMessage(GetDlgItem(hwndDlg,IDC_BITRATE), CB_DELETESTRING, 3, 0);
			SendMessage(GetDlgItem(hwndDlg,IDC_BITRATE), CB_DELETESTRING, 2, 0);
			SendMessage(GetDlgItem(hwndDlg,IDC_BITRATE), CB_ADDSTRING, 0, (LPARAM) "Dialup");
			SendMessage(GetDlgItem(hwndDlg,IDC_BITRATE), CB_ADDSTRING, 0, (LPARAM) "Custom");
			SendMessage(GetDlgItem(hwndDlg,IDC_BITRATE), CB_SETCURSEL, game_option_bitrate, 0);
		}
		return;
	}
	else if (game_lang == 1) // french
	{
		if (WhichDialogBox == 0)
		{
			SetDlgItemText(hwndDlg, IDC_STATIC_DISPLAY, "Affichage");
			SetDlgItemText(hwndDlg, IDC_QUICK_FULLSCREEN, "Plein Ecran Rapide (mode en cours)");
			SetDlgItemText(hwndDlg, IDC_FULLSCREEN, "Plein Ecran");
			SetDlgItemText(hwndDlg, IDC_WINDOW, "Fenêtre");
			SetDlgItemText(hwndDlg, IDC_STATIC_MISC, "Autres");
			SetDlgItemText(hwndDlg, IDC_VSYNC, "VSync");
			SetDlgItemText(hwndDlg, IDC_STREAM, "Streaming");
			SetDlgItemText(hwndDlg, IDC_STATIC_QUALITY, "Qualité");
			SetDlgItemText(hwndDlg, IDC_ANTI_ALIASING, "Antialiasing");
			SetDlgItemText(hwndDlg, IDC_BILLBOARDING, "Billboarding");
			SetDlgItemText(hwndDlg, IDC_MIPMAP, "Textures Mipmap");
			SetDlgItemText(hwndDlg, IDOK, "Démarrer");
			SendMessage(GetDlgItem(hwndDlg,IDC_BITRATE), CB_DELETESTRING, 3, 0);
			SendMessage(GetDlgItem(hwndDlg,IDC_BITRATE), CB_DELETESTRING, 2, 0);
			SendMessage(GetDlgItem(hwndDlg,IDC_BITRATE), CB_ADDSTRING, 0, (LPARAM) "Modem");
			SendMessage(GetDlgItem(hwndDlg,IDC_BITRATE), CB_ADDSTRING, 0, (LPARAM) "Adresse");
			SendMessage(GetDlgItem(hwndDlg,IDC_BITRATE), CB_SETCURSEL, game_option_bitrate, 0);
		}
		else if (WhichDialogBox == 1)
		{
			SetWindowText(hwndDlg, "Space Invaders OpenGL - A Propos -");
			SetDlgItemText(hwndDlg, IDC_STATIC_HOMEPAGE, "Page Web");
			SetDlgItemText(hwndDlg, IDC_STATIC_FORUM, "Forum");
			SetDlgItemText(hwndDlg, IDC_CHECK_WIVIK, "Clavier Virtuel d'aide à l'écran Wivik");
			SetDlgItemText(hwndDlg, IDC_BUTTON_CLOSE, "Fermer la fenêtre");
		}
		else if (WhichDialogBox == 2)
		{
			SetWindowText(hwndDlg, "Adresse URL de streaming");
			SetDlgItemText(hwndDlg, IDC_STATIC_CUSTOM1, "Veuillez entrer l'URL d'une playlist Shoutcast (*.pls), d'un flux Shoutcast,");
			SetDlgItemText(hwndDlg, IDC_STATIC_CUSTOM2, "ou d'un flux MP3. Voir :");
		}
		return;
	}
	else if (game_lang == 2) // spanish
	{
		if (WhichDialogBox == 0)
		{
			SetDlgItemText(hwndDlg, IDC_STATIC_DISPLAY, "Display");
			SetDlgItemText(hwndDlg, IDC_QUICK_FULLSCREEN, "Pantalla Completa (modo actual)");
			SetDlgItemText(hwndDlg, IDC_FULLSCREEN, "Pantalla Completa");
			SetDlgItemText(hwndDlg, IDC_WINDOW, "Ventana");
			SetDlgItemText(hwndDlg, IDC_STATIC_MISC, "Misc.");
			SetDlgItemText(hwndDlg, IDC_VSYNC, "VSync");
			SetDlgItemText(hwndDlg, IDC_STREAM, "Ancho banda");
			SetDlgItemText(hwndDlg, IDC_STATIC_QUALITY, "Quality");
			SetDlgItemText(hwndDlg, IDC_ANTI_ALIASING, "Antialiasing");
			SetDlgItemText(hwndDlg, IDC_BILLBOARDING, "Billboarding");
			SetDlgItemText(hwndDlg, IDC_MIPMAP, "Texturas Mipmap");
			SetDlgItemText(hwndDlg, IDOK, "Empezar");
			SendMessage(GetDlgItem(hwndDlg,IDC_BITRATE), CB_DELETESTRING, 3, 0);
			SendMessage(GetDlgItem(hwndDlg,IDC_BITRATE), CB_DELETESTRING, 2, 0);
			SendMessage(GetDlgItem(hwndDlg,IDC_BITRATE), CB_ADDSTRING, 0, (LPARAM) "Marcado");
			SendMessage(GetDlgItem(hwndDlg,IDC_BITRATE), CB_ADDSTRING, 0, (LPARAM) "Personalizado");
			SendMessage(GetDlgItem(hwndDlg,IDC_BITRATE), CB_SETCURSEL, game_option_bitrate, 0);
		}
		else if (WhichDialogBox == 1)
		{
			SetWindowText(hwndDlg, "Space Invaders OpenGL - About -");
			SetDlgItemText(hwndDlg, IDC_STATIC_HOMEPAGE, "Página Web");
			SetDlgItemText(hwndDlg, IDC_STATIC_FORUM, "Foro");
			SetDlgItemText(hwndDlg, IDC_CHECK_WIVIK, "Wikik pantalla Teclado virtual");
			SetDlgItemText(hwndDlg, IDC_BUTTON_CLOSE, "Cerrar ventana");
		}
		else if (WhichDialogBox == 2)
		{
			SetWindowText(hwndDlg, "Personalizado con URL");
			SetDlgItemText(hwndDlg, IDC_STATIC_CUSTOM1, "Introduzca una URL para una lista de reproducción (*.pls) de una estación,");
			SetDlgItemText(hwndDlg, IDC_STATIC_CUSTOM2, "una emisora, o un MP3. Chequee");
		}
		return;
	}
}

//////////////////////////////////////////////////////////////////////////////////////
//
// FONCTIONS => Courbe générale de Bézier. Retourne le point (donc les coordonnées) situé sur
//              la courbe formée des n points de contrôles situés à l'adresse p à l'intant mu.
//
// FORMAT    => la structure d'un point:    typedef struct {float x;float y;float z;}point3f;
//              les points de contrôles:    déclarés par: point3f control_points[n+1];
//                                          définis par:  control_points[0].x=... jusqu'à control_points[n].z=...
//              le point retourné:          point3f return_point;
//              le temps:                   mu compris entre 0 et 1 (0 <= mu <= 1)
//
// APPEL     => return_point = Bezier(control_points, 5, t); (exemple pour 6 points de contrôle)
//
point3f Bezier(point3f *p,int n,double mu)
{
   int k,kn,nn,nkn;
   double blend,muk,munk;
   point3f b = {0.0,0.0,0.0};

   // cas particulier pour mu = 1
   if (mu == 1)
   {
	b.x = p[n].x;
	b.y = p[n].y;
	b.z = p[n].z;
	return(b);
   }

   // cas général   
   muk = 1;
   munk = pow(1-mu,(double)n);

   for (k=0;k<=n;k++) {
      nn = n;
      kn = k;
      nkn = n - k;
      blend = muk * munk;
      muk *= mu;
      munk /= (1-mu);
      while (nn >= 1) {
         blend *= nn;
         nn--;
         if (kn > 1) {
            blend /= (double)kn;
            kn--;
         }
         if (nkn > 1) {
            blend /= (double)nkn;
            nkn--;
         }
      }
      b.x += p[k].x * blend;
      b.y += p[k].y * blend;
      b.z += p[k].z * blend;
   }

   return(b);
}

//////////////////////////////////////////////////////////////////////////////////////
//
// FONCTIONS => Retourne le hiscore
//
//
int get_hiscore()
{
	// LECTURE DU HISCORE DANS LA BASE DE REGISTRE
	HKEY key;
	DWORD disposition;

	// check key
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\SpaceInvaders-OpenGL\\", 0, KEY_READ, &key) == ERROR_SUCCESS)
	{
		// read key value
		int  nombre = 0;

		DWORD type = REG_DWORD;
		DWORD size = sizeof(DWORD);

		RegQueryValueEx(key, "hiscore", NULL, &type, (LPBYTE)&nombre, &size);
		return nombre;
	}
	else
	{
		// create key
		RegCreateKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\SpaceInvaders-OpenGL\\", 0, "", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &key, &disposition);
		return 0;
	}
	RegCloseKey(key);
}

//////////////////////////////////////////////////////////////////////////////////////
//
// FONCTIONS => Inscrit le hiscore
//
//
void set_hiscore(int score)
{
	// SAUVEGARDE DU HISCORE DANS LA BASE DE REGISTRE
	HKEY key;	

	int current_hiscore = 0;
	current_hiscore = get_hiscore();

	if (score < current_hiscore)
		return;

	// check key
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\SpaceInvaders-OpenGL\\", 0, KEY_WRITE, &key) == ERROR_SUCCESS)
	{
		RegSetValueEx(key, "hiscore", 0, REG_DWORD, (LPBYTE) &score, sizeof(DWORD));
	}
	RegCloseKey(key);
}

//////////////////////////////////////////////////////////////////////////////////////
//
// FONCTIONS => Récupère les options
//
//
int get_options(GL_Window* window)
{
	// LECTURE DANS LA BASE DE REGISTRE
	HKEY key;
	DWORD disposition;

	// check key
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\SpaceInvaders-OpenGL\\", 0, KEY_READ, &key) == ERROR_SUCCESS)
	{
		// read bool values
		DWORD type, size;

		size = sizeof(bool);RegQueryValueEx(key, "quickFullScreen",              NULL, &type, (LPBYTE)&window->init.quickFullScreen, &size);		
		size = sizeof(bool);RegQueryValueEx(key, "isFullScreen",                 NULL, &type, (LPBYTE)&window->init.isFullScreen,    &size);
		size = sizeof(bool);RegQueryValueEx(key, "game_option_vsync",            NULL, &type, (LPBYTE)&game_option_vsync,            &size);
		size = sizeof(bool);RegQueryValueEx(key, "game_option_stream",           NULL, &type, (LPBYTE)&game_option_stream,           &size);
		size = sizeof(bool);RegQueryValueEx(key, "game_option_anti_aliasing",    NULL, &type, (LPBYTE)&game_option_anti_aliasing,    &size);
		size = sizeof(bool);RegQueryValueEx(key, "game_option_billboard",        NULL, &type, (LPBYTE)&game_option_billboard,        &size);
		size = sizeof(bool);RegQueryValueEx(key, "game_option_mipmap",           NULL, &type, (LPBYTE)&game_option_mipmap,           &size);

		// read string values
		size=1024;RegQueryValueEx(key, "stream_name_user", NULL, &type, (LPBYTE)&stream_name_user, &size);size=1024;
		
		// read int values
		type = REG_DWORD;
		size = sizeof(DWORD);RegQueryValueEx(key, "game_option_bitrate",         NULL, &type, (LPBYTE)&game_option_bitrate,          &size);
		size = sizeof(DWORD);RegQueryValueEx(key, "game_option_anti_aliasing_mode",NULL, &type, (LPBYTE)&game_option_anti_aliasing_mode, &size);
		size = sizeof(DWORD);RegQueryValueEx(key, "bitsPerPixel",                NULL, &type, (LPBYTE)&window->init.bitsPerPixel,    &size);
		size = sizeof(DWORD);RegQueryValueEx(key, "width",                       NULL, &type, (LPBYTE)&window->init.width,           &size);
		size = sizeof(DWORD);RegQueryValueEx(key, "game_lang",                   NULL, &type, (LPBYTE)&game_lang,                    &size);
		
		// some verifications
		if (window->init.quickFullScreen && window->init.isFullScreen)
			window->init.isFullScreen = false;

		return 1;
	}
	else
	{
		// create key
		RegCreateKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\SpaceInvaders-OpenGL\\", 0, "", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &key, &disposition);
		return 0;
	}
	RegCloseKey(key);
}

//////////////////////////////////////////////////////////////////////////////////////
//
// FONCTIONS => Inscrit les options
//
//
void set_options(GL_Window* window)
{
	// SAUVEGARDE DANS LA BASE DE REGISTRE
	HKEY key;

	// check key
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\SpaceInvaders-OpenGL\\", 0, KEY_WRITE, &key) == ERROR_SUCCESS)
	{
		// set bool values
		RegSetValueEx(key, "quickFullScreen",              0, REG_BINARY, (LPBYTE) &window->init.quickFullScreen, sizeof(bool));
		RegSetValueEx(key, "isFullScreen",                 0, REG_BINARY, (LPBYTE) &window->init.isFullScreen,    sizeof(bool));
		RegSetValueEx(key, "game_option_vsync",            0, REG_BINARY, (LPBYTE) &game_option_vsync,            sizeof(bool));
		RegSetValueEx(key, "game_option_stream",           0, REG_BINARY, (LPBYTE) &game_option_stream,           sizeof(bool));
		RegSetValueEx(key, "game_option_anti_aliasing",    0, REG_BINARY, (LPBYTE) &game_option_anti_aliasing,    sizeof(bool));
		RegSetValueEx(key, "game_option_billboard",        0, REG_BINARY, (LPBYTE) &game_option_billboard,        sizeof(bool));
		RegSetValueEx(key, "game_option_mipmap",           0, REG_BINARY, (LPBYTE) &game_option_mipmap,           sizeof(bool));

		// set int values
		RegSetValueEx(key, "game_option_bitrate",          0, REG_DWORD, (LPBYTE) &game_option_bitrate,       sizeof(DWORD));
		RegSetValueEx(key, "game_option_anti_aliasing_mode", 0, REG_DWORD, (LPBYTE) &game_option_anti_aliasing_mode, sizeof(DWORD));
		RegSetValueEx(key, "bitsPerPixel",                 0, REG_DWORD, (LPBYTE) &window->init.bitsPerPixel, sizeof(DWORD));
		RegSetValueEx(key, "width",                        0, REG_DWORD, (LPBYTE) &window->init.width,        sizeof(DWORD));
		RegSetValueEx(key, "game_lang",                    0, REG_DWORD, (LPBYTE) &game_lang,                 sizeof(DWORD));
				
		// set string values
		RegSetValueEx(key, "stream_name_user",             0, REG_SZ, (const unsigned char*) stream_name_user, strlen(stream_name_user));
	}
	RegCloseKey(key);
}

//////////////////////////////////////////////////////////////////////////////////////
//
// FONCTIONS => La carte graphique est-elle accélérée ?
//
//
bool IsCardAccelerated()
{
   //structure that holds info about a pixel format
   PIXELFORMATDESCRIPTOR pfd;
   //HW accelerated yes or not
   bool isAccelerated = false;
  
   //Very important to set this variable.. Windows seems
   //not to be able to do this itself;)
   pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
   
   //Device Context of the window you want to draw in
   //must be the same as the one you use to draw GL
   //HDC hDC = GetDC(g_window->hWnd);
   HDC hDC = GetDC(HWND_DESKTOP);
 
   //number of available pixel formats
   int nFormatCount = DescribePixelFormat(hDC, 1, 0, NULL);
 
   //Go through all available pixel formats and check..
   for(int i = 1; i <= nFormatCount; i++)
   {
      //Get description of pixel format
      DescribePixelFormat(hDC, i, pfd.nSize, &pfd);
 
      //Not generic?
      if (!(pfd.dwFlags & PFD_GENERIC_FORMAT))
      {
         //It's HW accelerated!
         isAccelerated = true;
         //We can stop here, as there is at least one acc. pixel format
         break;
      }
   }
  
   return isAccelerated;
}

//////////////////////////////////////////////////////////////////////////////////////
//
// FONCTIONS => Hyperliens (Copyright 2002 Neal Stublen, http://www.awesoftware.com)
//              Appel par : ConvertStaticToHyperlink(hwndDlg, IDC_MY_CONTROL); dans WM_INITDIALOG
//              et ShellExecute() dans case IDC_MY_CONTROL:
//
#define PROP_ORIGINAL_FONT		TEXT("_Hyperlink_Original_Font_")
#define PROP_ORIGINAL_PROC		TEXT("_Hyperlink_Original_Proc_")
#define PROP_STATIC_HYPERLINK	TEXT("_Hyperlink_From_Static_")
#define PROP_UNDERLINE_FONT		TEXT("_Hyperlink_Underline_Font_")

LRESULT CALLBACK _HyperlinkParentProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	WNDPROC pfnOrigProc = (WNDPROC) GetProp(hwnd, PROP_ORIGINAL_PROC);

	switch (message)
	{
	case WM_CTLCOLORSTATIC:
		{
			HDC hdc = (HDC) wParam;
			HWND hwndCtl = (HWND) lParam;

			BOOL fHyperlink = (NULL != GetProp(hwndCtl, PROP_STATIC_HYPERLINK));
			if (fHyperlink)
			{
				LRESULT lr = CallWindowProc(pfnOrigProc, hwnd, message, wParam, lParam);
				SetTextColor(hdc, RGB(0, 0, 0));
				return lr;
			}

			break;
		}
	case WM_DESTROY:
		{
			SetWindowLong(hwnd, GWL_WNDPROC, (LONG) pfnOrigProc);
			RemoveProp(hwnd, PROP_ORIGINAL_PROC);
			break;
		}
	}
	return CallWindowProc(pfnOrigProc, hwnd, message, wParam, lParam);
}

LRESULT CALLBACK _HyperlinkProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	WNDPROC pfnOrigProc = (WNDPROC) GetProp(hwnd, PROP_ORIGINAL_PROC);

	switch (message)
	{
	case WM_DESTROY:
		{
			SetWindowLong(hwnd, GWL_WNDPROC, (LONG) pfnOrigProc);
			RemoveProp(hwnd, PROP_ORIGINAL_PROC);

			HFONT hOrigFont = (HFONT) GetProp(hwnd, PROP_ORIGINAL_FONT);
			SendMessage(hwnd, WM_SETFONT, (WPARAM) hOrigFont, 0);
			RemoveProp(hwnd, PROP_ORIGINAL_FONT);

			HFONT hFont = (HFONT) GetProp(hwnd, PROP_UNDERLINE_FONT);
			DeleteObject(hFont);
			RemoveProp(hwnd, PROP_UNDERLINE_FONT);

			RemoveProp(hwnd, PROP_STATIC_HYPERLINK);

			break;
		}
	case WM_MOUSEMOVE:
		{
			if (GetCapture() != hwnd)
			{
				HFONT hFont = (HFONT) GetProp(hwnd, PROP_UNDERLINE_FONT);
				SendMessage(hwnd, WM_SETFONT, (WPARAM) hFont, FALSE);
				InvalidateRect(hwnd, NULL, FALSE);
				SetCapture(hwnd);
			}
			else
			{
				RECT rect;
				GetWindowRect(hwnd, &rect);

				POINT pt = { LOWORD(lParam), HIWORD(lParam) };
				ClientToScreen(hwnd, &pt);

				if (!PtInRect(&rect, pt))
				{
					HFONT hFont = (HFONT) GetProp(hwnd, PROP_ORIGINAL_FONT);
					SendMessage(hwnd, WM_SETFONT, (WPARAM) hFont, FALSE);
					InvalidateRect(hwnd, NULL, FALSE);
					ReleaseCapture();
				}
			}
			break;
		}
	case WM_SETCURSOR:
		{
			// Since IDC_HAND is not available on all operating systems,
			// we will load the arrow cursor if IDC_HAND is not present.
			HCURSOR hCursor = LoadCursor(NULL, MAKEINTRESOURCE(32649)); // Hand cursor
			if (NULL == hCursor)
			{
				hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
			}
			SetCursor(hCursor);
			return TRUE;
		}
	}

	return CallWindowProc(pfnOrigProc, hwnd, message, wParam, lParam);
}

BOOL ConvertStaticToHyperlink(HWND hwndCtl)
{
	// Subclass the parent so we can color the controls as we desire.

	HWND hwndParent = GetParent(hwndCtl);
	if (NULL != hwndParent)
	{
		WNDPROC pfnOrigProc = (WNDPROC) GetWindowLong(hwndParent, GWL_WNDPROC);
		if (pfnOrigProc != _HyperlinkParentProc)
		{
			SetProp(hwndParent, PROP_ORIGINAL_PROC, (HANDLE) pfnOrigProc);
			SetWindowLong(hwndParent, GWL_WNDPROC, (LONG) (WNDPROC) _HyperlinkParentProc);
		}
	}

	// Make sure the control will send notifications.

	DWORD dwStyle = GetWindowLong(hwndCtl, GWL_STYLE);
	SetWindowLong(hwndCtl, GWL_STYLE, dwStyle | SS_NOTIFY);

	// Subclass the existing control.

	WNDPROC pfnOrigProc = (WNDPROC) GetWindowLong(hwndCtl, GWL_WNDPROC);
	SetProp(hwndCtl, PROP_ORIGINAL_PROC, (HANDLE) pfnOrigProc);
	SetWindowLong(hwndCtl, GWL_WNDPROC, (LONG) (WNDPROC) _HyperlinkProc);

	// Create an updated font by adding an underline.

	HFONT hOrigFont = (HFONT) SendMessage(hwndCtl, WM_GETFONT, 0, 0);
	SetProp(hwndCtl, PROP_ORIGINAL_FONT, (HANDLE) hOrigFont);

	LOGFONT lf;
	GetObject(hOrigFont, sizeof(lf), &lf);
	lf.lfUnderline = TRUE;

	HFONT hFont = CreateFontIndirect(&lf);
	SetProp(hwndCtl, PROP_UNDERLINE_FONT, (HANDLE) hFont);

	// Set a flag on the control so we know what color it should be.

	SetProp(hwndCtl, PROP_STATIC_HYPERLINK, (HANDLE) 1);

	return TRUE;
}

BOOL ConvertStaticToHyperlink(HWND hwndParent, UINT uiCtlId)
{
	return ConvertStaticToHyperlink(GetDlgItem(hwndParent, uiCtlId));
}

void fontColor()
{
	if (game_3d)
	{
		//fontColor(0,0,0);
		//fontColor(0.5,0.5,0.5);
		//fontColor(1,1,1);

		//fontShadowColor(1,1,1);
		//fontShadowColor(0.5,0.5,0.5);
		//fontShadowColor(1,0.6,0.6);
		
		//fontGradientColor(0,0,0);
		//fontGradientColor(1,1,1);
		
		//fontShadow();
		//fontGradient();
	}
}

//////////////////////////////////////////////////////////////////////////////////////
//
// NOUVEAU THREAD - LE VOLUME DE LA MUSIQUE AUGMENTE
//
//
void FmodStreamFadingIn(void* dummy)
{
	int volume = 0;

	do
	{
		volume += 5;
		FSOUND_SetVolume(int_sound_mp3, volume);
		Sleep(30);
	}
	while(volume <= 255);

//	_endthread();
}


//////////////////////////////////////////////////////////////////////////////////////
//
// SON LORS DE L'APPUI SUR UNE TOUCHE D'OPTION
//
//
void key_sound()
{
	int_sound = FSOUND_PlaySoundEx(FSOUND_FREE,sound_key, dspUnitSounds, false);
	FSOUND_SetVolume(int_sound,200);
}

//////////////////////////////////////////////////////////////////////////////////////
//
// Check an OpenGL Extension. By Henry Goffin.
//
//
// WGLisExtensionSupported: This Is A Form Of The Extension For WGL
bool WGLisExtensionSupported(const char *extension)
{
	const size_t extlen = strlen(extension);
	const char *supported = NULL;

	// Try To Use wglGetExtensionStringARB On Current DC, If Possible
	PROC wglGetExtString = wglGetProcAddress("wglGetExtensionsStringARB");

	if (wglGetExtString)
		supported = ((char*(__stdcall*)(HDC))wglGetExtString)(wglGetCurrentDC());

	// If That Failed, Try Standard Opengl Extensions String
	if (supported == NULL)
		supported = (char*)glGetString(GL_EXTENSIONS);

	// If That Failed Too, Must Be No Extensions Supported
	if (supported == NULL)
		return false;

	// Begin Examination At Start Of String, Increment By 1 On False Match
	for (const char* p = supported; ; p++)
	{
		// Advance p Up To The Next Possible Match
		p = strstr(p, extension);

		if (p == NULL)
			return false;															// No Match

		// Make Sure That Match Is At The Start Of The String Or That
		// The Previous Char Is A Space, Or Else We Could Accidentally
		// Match "wglFunkywglExtension" With "wglExtension"

		// Also, Make Sure That The Following Character Is Space Or NULL
		// Or Else "wglExtensionTwo" Might Match "wglExtension"
		if ((p==supported || p[-1]==' ') && (p[extlen]=='\0' || p[extlen]==' '))
			return true;															// Match
	}
}

//////////////////////////////////////////////////////////////////////////////////////
//
// Anti-aliasing (ARB Multi Sample) Author: Colt "MainRoach" McAnlis
//
//
// InitMultisample: Used To Query The Multisample Frequencies

#define WGL_SAMPLE_BUFFERS_ARB		 0x2041
#define WGL_SAMPLES_ARB			     0x2042

bool InitMultisample(HINSTANCE hInstance,HWND hWnd,PIXELFORMATDESCRIPTOR pfd)
{  

	// See If The String Exists In WGL!
	if (!WGLisExtensionSupported("WGL_ARB_multisample"))
	{
		arbMultisampleSupported=false;
		return false;
	}

	// Get Our Pixel Format
	PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");	
	if (!wglChoosePixelFormatARB) 
	{
		arbMultisampleSupported=false;
		return false;
	}

	// Get Our Current Device Context
	HDC hDC = GetDC(hWnd);

	int		pixelFormat;
	int		valid;
	UINT	numFormats;
	float	fAttributes[] = {0,0};

	// These Attributes Are The Bits We Want To Test For In Our Sample
	// Everything Is Pretty Standard, The Only One We Want To 
	// Really Focus On Is The SAMPLE BUFFERS ARB And WGL SAMPLES
	// These Two Are Going To Do The Main Testing For Whether Or Not
	// We Support Multisampling On This Hardware.
	int iAttributes[] =
	{
		WGL_DRAW_TO_WINDOW_ARB,GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB,GL_TRUE,
		WGL_ACCELERATION_ARB,WGL_FULL_ACCELERATION_ARB,
		WGL_COLOR_BITS_ARB,32,	//24, 32 work
		WGL_ALPHA_BITS_ARB,8,	//8
		WGL_DEPTH_BITS_ARB,16,	//8, 16 work
		WGL_STENCIL_BITS_ARB,0,
		WGL_DOUBLE_BUFFER_ARB,GL_TRUE,
		WGL_SAMPLE_BUFFERS_ARB,GL_TRUE,
		WGL_SAMPLES_ARB,4,
		0,0
	};
	
	// samples
	if (game_option_anti_aliasing_mode == 0 || game_option_anti_aliasing_mode == 1)
		iAttributes[19] = 2;
	else if (game_option_anti_aliasing_mode == 2 || game_option_anti_aliasing_mode == 3)
		iAttributes[19] = 4;

	// First We Check To See If We Can Get A Pixel Format For 4 Samples
	valid = wglChoosePixelFormatARB(hDC,iAttributes,fAttributes,1,&pixelFormat,&numFormats);
 
	// If We Returned True, And Our Format Count Is Greater Than 1
	if (valid && numFormats >= 1)
	{
		arbMultisampleSupported = true;
		arbMultisampleFormat = pixelFormat;
//		LOG0("Anti-aliasing")
//		add_log("blblbl", false);
		return arbMultisampleSupported;
	}

	// Our Pixel Format With 4 Samples Failed, Test For 2 Samples
	iAttributes[19] = 2;
	valid = wglChoosePixelFormatARB(hDC,iAttributes,fAttributes,1,&pixelFormat,&numFormats);
	if (valid && numFormats >= 1)
	{
		arbMultisampleSupported = true;
		arbMultisampleFormat = pixelFormat;	 
		return arbMultisampleSupported;
	}
	  
	// Return The Valid Format
	return  arbMultisampleSupported;
}

//////////////////////////////////////////////////////////////////////////////////////
//
// FONCTIONS => Affiche une erreur dans une MessageBox et quitte le programme
//
//
void DisplayMsgBoxErrorAndExit(LPCTSTR File, int Line, LPCTSTR Message)
{
	TCHAR ErrorBuf[512];

	// Create the string.
	wsprintf(ErrorBuf, "Sorry, an error has occurred...\n\n%s\nFile %s, line %d, version %s\n\nPlease check http://spaceinvadersgl.sourceforge.net to grab the last release.\nIf the error persists email mathieu at maya75@users.sourceforge.net.", 
		Message, File, Line, game_version);

	// Dump the error to the debugger, if present.
	OutputDebugString(ErrorBuf);

	// Display the error using MessageBox.
	if (Message != "")
		MessageBox(NULL, ErrorBuf, "Message", MB_OK | MB_TOPMOST);

	//exit(1);
}

//////////////////////////////////////////////////////////////////////////////////////
//
// FONCTIONS => Affiche le dernier message d'erreur sur la sortie de Debug
//
//
void DisplayLastErrorDebug(void)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf, 0, NULL);
	OutputDebugString((LPCTSTR)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

//////////////////////////////////////////////////////////////////////////////////////
//
// FONCTIONS => Affiche le dernier message d'erreur dans une MsgBox
//
//
void DisplayLastErrorBox(void)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf, 0, NULL);
	BOX((LPCTSTR)lpMsgBuf);
	LocalFree(lpMsgBuf);
}