/*
  si_Main.cpp                                                                 May 2004

                         Space Invaders OpenGL - version 0.6.1
                 (2003) Mathieu Vidalinc <maya75@users.sourceforge.net>
                         http://spaceinvadersgl.sourceforge.net

______________________________________________________________________________________

  The GNU General Public License (GPL)
	
  This program is free software; you can redistribute it and/or modify it under the
  terms of the GNU General Public License as published by the Free Software Foundation;
  either version 2 of the License, or (at your option) any later version.
 
  This program is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
  PARTICULAR PURPOSE. See the GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along with this
  program; if not, write to the Free Software Foundation, Inc., 59 Temple Place,
  Suite 330, Boston, MA 02111-1307 USA
______________________________________________________________________________________

  NOTES
  =====

  - If you use Visual Studio > 6.0 (i.e. .NET) or have installed the Platform SDK,
  delete the two first #define of resource.h which are already included by the compiler.

  - If you want to run previous releases (version <= 0.5.5), please delete fmod.dll
  (version >= 3.70) from system directory, since these releases use fmod.dll v3.63
  which is NOT compatible with (and which will not overwrite) the latest version
  of fmod.dll.

  - I added _WIN32_WINDOWS=0x0410 (for targeting Windows 98) as preprocessor definition
  in Project Settings for using the Platform SDK.

  - I compile the releases with MSVC++ 6.0 because MSVC++ 7.0 links the program with
  msvcr70.dll, which is not present on all systems. Doing that i have a program linked
  with msvcrt.dll which is present since Win95 OSR2.5. Prjconverter was used to convert
  project from VC++7 to VC++6 (http://www.codeproject.com/tools/prjconverter.asp)

  - I reduce the executable file size using ASPack Compressor (http://www.aspack.com)
  
  Hope you'll find something interesting here!
  
  For your code editor: Tab size: 4, Ident size: 4 (default in VC++)
______________________________________________________________________________________
 
  LINKED LIBRARIES
  ================

  LIBRARY			PURPOSE								FUNCTIONS USED

  opengl32.lib		OpenGL API							glEnable(), glBindTexture()...
  glu32.lib			OpenGL Utilities					gluBuild2DMipmaps(), gluLookAt()...
  
  msvcrt.lib		Multithreaded runtime library		fread(), strlen()...
  comctl32.lib		Windows XP Common Controls			InitCommonControls()
  winmm.lib			Windows Multimedia Functions		timeGetTime()
  shell32.lib		Shell Operations					ShellExecute()
  advapi32.lib		Win32 Advanced API					RegOpenKeyEx(), RegQueryValueEx()...
  wininet.lib		Internet Functions					InternetOpen()...

  ole32.lib			For JPG/GIF loading					CreateStreamOnHGlobal()
  oleaut32.lib		For JPG/GIF loading					OleLoadPicture()
  gdi32.lib			For JPG/GIF loading					SelectObject()...
  uuid.lib			For JPG/GIF loading					IID_IPicture
    
  fmodvc.lib		FMOD Sound System					FSOUND_Init()...
  delayimp.lib		Run-Time DLL loading (fmod.dll)		needs /delayload:"fmod.dll" switch

  any of /align:4096 /filealign:512 /opt:nowin98 for reduce file size
______________________________________________________________________________________

  CONTROLS
  ========
  
     [LEFT], [RIGHT]       -> Déplacements
     [UP], [CTRL]          -> Tir
     [ESC]                 -> Quitter
     [+], [-]              -> Zoom
     [SPACE]               -> Change la musique (si plusieurs mp3)
     [B]                   -> Change le fond
     [M]                   -> Musique on/off
     [R]                   -> Réinitialise la caméra : angle, zoom
     [A]                   -> Anti-aliasing on/off
     (MOUSE)               -> Rotation de la scène

  TODO:wheel,joystick,win-tab key,weapons, block-accel
______________________________________________________________________________________*/

#define WIN32_LEAN_AND_MEAN				// Compile faster (excludes automatic processing of some rarely-used headers)

#include <windows.h>					// Header File For Windows
#include <Mmsystem.h>					// Header File For Multimedia
#include <stdio.h>						// Header File For Input/Outout
#include <stdlib.h>						// Header File For Standard Library
#include <string.h>						// Header File For The String Library
#include <gl/gl.h>						// Header File For The OpenGL32 Library
#include <gl/glu.h>						// Header File For The GLu32 Library
#include "si_Main.h"					// Header File For game
#include "si_3DS.h"						// Header File For Game

#ifndef CDS_FULLSCREEN					// CDS_FULLSCREEN Is Not Defined By Some
#define CDS_FULLSCREEN 4				// Compilers. By Defining It This Way,
#endif									// We Can Avoid Errors

// GLOBAL WINDOWS AND KEYS INFOS
GL_Window*	g_window;					// Infos de fenêtre en variable globale
Keys*		g_keys;						// Infos de touches en variable globale

// DEBUG TOOLS
//#define	DEBUG						// DEBUGGAGE
//#define	DEBUG_PASSAGE_3D			// DEBUGGAGE (aussi dans si_world.cpp)
#define DEBUG_MORTAL					// DEBUGGAGE
#define DEBUG_MUSIC						// DEBUGGAGE (aussi dans si_Loading.cpp)
#define DEBUG_BUNKERS					// DEBUGGAGE (aussi dans si_Loading.cpp)
#define DEBUG_FPS						// DEBUGGAGE
int		fp10s_nb				= 0;	// DEBUGGAGE
int		fp10s_count				= 0;	// DEBUGGAGE
int		fp10s_timer				= 0;	// DEBUGGAGE
int		nb_tir_index			= 0;	// DEBUGGAGE

// FPS METHODE N°2
bool	Fps2_firstpass = true;			// FPS Méthode n°2
double	sec_start				= 0;	// FPS Méthode n°2
double	sec_end					= 0;	// FPS Méthode n°2
double	sec_frame				= 0;	// FPS Méthode n°2

// PARAMETRES DU JEU ET CHARGEMENT		// à changer par une structure (à voir)
const char	game_version[6]		= "0.6.1";// VERSION
int		game_step				= 1;	// ETAPE (chargement : etape = 1, etc.)
int		game_level				= 1;	// LEVEL
bool	game_3d					= false;// 3D GAME ACTIVE ?
bool	game_3d_lines			= false;// 3D GAME - MODE GL_LINES ?
bool	game_option_mipmap		= true; // MIPMAP ?
bool	game_option_vsync		= true; // VSYNC ?
bool	game_option_billboard	= true; // BILLBOARDS ?
bool	game_option_anti_aliasing = true;// ANTI-ALIASING ?
int		game_option_anti_aliasing_mode = 0;// ANTI-ALIASING MODE (0:x2, 1:x2-Quincunx, 2:x4, 3:x4S))
bool	game_option_stream		= false;// MUSIC STREAMING ?
int		game_option_bitrate		= 0;	// MUSIC BITRATE ?
bool	game_option_wivik		= false;// WIVIK KEYBOARD HANDLING ACTIVATED
int		game_option_holddown_wivik = 100;// HOLD DOWN KEY DURATION IN MS FOR WIVIK KEYBOARD

bool	game_2d_text			= true;	// 2D TEXT AFFICHE ?
char	game_message[300]		= "";	// MESSAGE UTILISATEUR
bool	game_spectrum			= false;// SPECTRUM (BEAT DETECTION)?
bool	game_console			= false;// CONSOLE ACTIVE ?
bool	game_console_come_in	= true;	// CONSOLE APPARAIT ?
bool	game_console_moving		= false;// CONSOLE EN DEPLACEMENT ?
bool	game_console_start		= false;// CONSOLE EN DEPLACEMENT ?
bool	game_immortal			= false;// JOUEUR IMMORTEL ?
bool	game_antialiasing_active= true;	// ANTI-ALIASING ACTIVE ?
bool	game_vsync_active		= true;	// VSYNC ACTIVE ?
int		game_lang				= 0;	// LANGUAGE (0 english, 1 french, 2 spanish)

int		frame_start				= 0;	// TIMING FUNCTION
int		frame_oldtime			= 0;	// TIMING FUNCTION
int		equalizer_x				= 0;	// EQUALIZER COORD X
int		equalizer_y				= 0;	// EQUALIZER COORD Y

// ARB-MULTISAMPLE Anti-aliasing (OpenGL extension)
bool	arbMultisampleSupported	= false;// EXTENSION "ARBMULTISAMPLE" DISPO SUR LA CARTE 3D ?
int		arbMultisampleFormat	= 0;	// PIXEL FORMAT UTILISÉ SI L'EXTENSION EST DISPO

// CONSOLE
char	console_text[50][50]	= {"",""};// CONSOLE TEXTE UTILISATEUR
int		console_line			= 0;	// CONSOLE INDEX DE CONSOLE_TEXT[][]
char	console_command[40]		= "";	// CONSOLE 1ERE COMMANDE
char	console_command2[40]	= "";	// CONSOLE 2EME COMMANDE
char	console_output[40]		= "";	// CONSOLE 1ERE SORTIE
char	console_output2[40]		= "";	// CONSOLE 2EME SORTIE
float	viewport_push			= -999;	// CONSOLE VIEWPORT MINI-SCREEN

void Fullscreen(void);

// FONTS
int		fontsize				= 0;	// FONT SIZE
int		fontspace				= 0;	// FONT VERTICAL SPACE

// INTERMEDE							// à changer par une structure (à voir)
bool	intermede				= false;// INTERMEDE animation à faire ?
bool	intermede_fin			= false;// INTERMEDE FIN de l'animation ?
double	intermede_angle			= 0;	// INTERMEDE ANGLE pour l'animation de l'intermede
float	intermede_angle_total	= 0;	// INTERMEDE ANGLE TOTAL pour l'animation de l'intermede
bool	intermede_angle_moitie	= false;// INTERMEDE MOITIE moitie du chemin parcouru pour l'accélération et la décélération
bool	intermede_sound			= false;// INTERMEDE SOUND si = true, lance la musique de l'intermede
bool	active_splash			= false;// SPLASH SCREEN activé après l'intermède
bool	powerup					= false;// POWERUP (passage à game 3D)

// FPS ET INTERMEDE
float	my_fps;							// FPS pour gérer l'affichage du fps
float	slowdown_fps			= 0;	// FPS idem
float	blink_intermede			= 0;	// INTERMEDE le texte de l'intermede apparaît
int		blink_intermede_nb		= 0;	// INTERMEDE (clignote 3 fois)

// VAISSEAUX							// à changer par une structure (à voir)
float	ships_move				= -1;	// DEPLACEMENT (GAUCHE, DROITE)
float	ships_speed				= 1;	// VITESSE
bool	ships_vers_la_gauche	= true;	// SENS DE DEPLACEMENT (en dur)
float	ships_balancier			= 5;	// QUAND ON CHANGE DE SENS (de 9 à 0 pour le ship le + à droite par exemple, puis de 0 à 9)
float	ship_anim				= 0;	// ANIMATION DES SHIPS
int		ships_choix_anim		= 0;	// ANIM SHIPS 0 = 1ère texture 1 (car id texture = 1,2,3), 10 = 2ème texture (car id texture = 11,12,13)
float	ships_acc_shoots		= 0;	// TIRS PLUS RAPIDE quand on change de niveau

// STRUCTURES
players			player[10];				// 10 PLAYERS DISPO
superships		supership[10];			// 10 SUPERSHIPS DISPO
tirs			tir[51];				// (max_tirs+1) TIRS DISPO
const int		max_tirs		= 50;	// NOMBRE DE TIRS maximum affichés en même temps
int				id_tir			= 0;	// NOMBRE DE TIRS index
int				shoot_type		= 0;	// TYPE DE TIR 3D DU JOUEUR
ships			ship[50];				// 50 SHIPS DISPO
bricks			brick[500];				// 500 BRIQUES DE BUNKERS DISPO
int				id_brick		= 0;	// INDEX DES DU TABLEAU DES BRIQUES DES BUNKERS
explosions		explosion[60];			// 60 EXPLOSIONS DISPO
//particles2		particle2[MAX_PARTICLES2];// TIRS 3D DU JOUEUR
bonuses			bonus[10];				// BONUS

//Model_3DS m, m1, m2, m3, m5; //m4					// Instanciation des objets 3DS du jeu
extern Model_3DS m;						// objet 3DS
extern Model_3DS m1;					// objet 3DS
extern Model_3DS m2;					// objet 3DS
extern Model_3DS m3;					// objet 3DS
//extern Model_3DS m4;					// objet 3DS
extern Model_3DS m5;					// objet 3DS
GLUquadricObj*	quadratic;				// Storage For Our Quadratic Objects

// CAMERA PATH (COURBE DE BEZIER)
point3f			control_points[6];		// POINTS DE CONTROLES POUR LA COURBE DE BEZIER (MOUVEMENT DE CAMERA)
point3f			pcamera;				// COORDONNEES CAMERA
float			ciblex			= 0;	// COORD X CIBLE
float			cibley			= 0;	// COORD Y CIBLE
float			t = 0;					// TEMPS (COURBE DE BEZIER)

// FLOTTEMENT CAMERA
point3f			pcamera2;				// COORDONNEES CAMERA

// SCORES								// à changer par une structure (structure game ?)(à voir)
unsigned int	score	 		= 0;	// SCORE
unsigned int	hiscore			= 0;	// HISCORE (lu et écrit dans la base de registre)
int		lives 					= 3;	// LIVES
int		partie_gagnee			= 0;	// PARTIE GAGNE (0 = en cours, 1 = gagnée, -1 = perdue)
bool	vie_perdue				= false;// VIE PERDUE

// SCENE ZOOM
GLfloat zoom					= 0;	// ZOOM
GLfloat zoom_total				= 0;	// ZOOM TOTAL
GLfloat zoom_total_bonus		= 0;	// ZOOM TOTAL PENDANT LE PASSAGE A MODE 3D

// SCENE ROTATION
bool	moving;							// SOURIS (ROTATION SCENE)
double	startx, starty;					// SOURIS (ROTATION SCENE)
double	angle_x					 = 0;	// ANGLE X DE LA SCENE (SOURIS)
double	angle_y 				 = 0;	// ANGLE Y DE LA SCENE (SOURIS)
double	angle_x_start			 = 0;	// ANGLE X DE LA SCENE (MEMORISATION AVANT PASSAGE 3D)
double	angle_y_start			 = 0;	// ANGLE Y DE LA SCENE (MEMORISATION AVANT PASSAGE 3D)

// COLLISIONS
float g1,d1,h1,b1,g2,d2,h2,b2;			// COLLISIONS (1 = 1er sprite, 2 = 2ème sprite; g, d, h, b = gauche, droite, haut, bas)

// SONS ET MUSIQUES
bool	music_on				= true; // MUSIQUE ON/OFF
int		music_type				= 0;	// MUSIQUE TYPE (XM=1, MP3=2, STREAMING=3))
int		int_sound				= 0,	// NUMERO CHANNEL (bruitages et musique)
		int_sound_explosion		= 0,	// IDEM
		int_sound_mp3			= 0,	// IDEM
		int_sound_mod			= 0;	// IDEM
int		id_my_music				= 0;	// IDENTIFIANT (musique en cours : music[id_my_music].titre)
musics	music[300];						// 300 MUSIQUES DISPO
char	stream_name[1024]		= "";	// STREAMED MUSIC NAME
char	stream_name_user[1024]	= "";	// STREAMED MUSIC USER
char	music_en_cours[1024]	= "";	// MUSIQUE EN COURS
char	music_affichee[1024]	= "";	// MUSIQUE AFFICHEE (LONGUEUR LIMITEE)
Playlist* my_playlist;					// SHOUTCAST PLAYLIST
int		timer_1s				= 0;	// TIMER POUR RECHERCHER LE NOM DE L'ARTISTE ET DE LA MUSIQUE

// FMOD CALLBACKS
signed short *OscBuffer = NULL;
       int    OscBlock = 0;

void * F_CALLBACKAPI dspCallbackSounds(void *originalbuffer, void *newbuffer, int length, int param)
{return newbuffer;}

void * F_CALLBACKAPI dspCallbackSpectrum(void *originalbuffer, void *newbuffer, int length, int param)
{
	int             mixertype = FSOUND_GetMixer();
    int             count;
	int             totalblocks; 
    signed short    *dest;

	totalblocks = FSOUND_DSP_GetBufferLengthTotal() / FSOUND_DSP_GetBufferLength(); 
	
    // Convert and downmix into a mono short int buffer.

    dest = &OscBuffer[OscBlock * FSOUND_DSP_GetBufferLength()];

	//char tempstr[255]="";
	//_itoa(*OscBuffer, tempstr, 10);
	//OutputDebugString(tempstr);

    //if (mixertype == FSOUND_MIXER_QUALITY_FPU)
	if (1)
    {
        float *src = (float *)newbuffer;

        for (count=0; count < length; count++)
        {
            //dest[count] = (signed short)(src[count << 1] + src[(count << 1) + 1] * 0.5f);
			dest[count] = (signed short)(src[count]);
        }
    }
    else
    {
        signed short *src = (signed short *)newbuffer;

        for (count=0; count < length; count++)
        {
            dest[count] = (signed short)(((int)src[count << 1] + (int)src[(count << 1) + 1]) >> 1);
        }
    }    

    OscBlock++;
    if (OscBlock >= totalblocks)
    {
        OscBlock = 0;
    }

	return newbuffer;
}

char	artist[256]				= "";
char	title[256]				= "";
int		metanum					= 0;
signed char F_CALLBACKAPI metacallback(char *name, char *value, int userdata)
{
    if (!strcmp("ARTIST", name))
    {
        strcpy(artist, value);
        return TRUE;
    }

    if (!strcmp("TITLE", name))
    {
        strcpy(title, value);
        metanum++;
        return TRUE;
    }
    return TRUE;
}

FSOUND_DSPUNIT* dspUnitSounds;			// DSP SOUNDS (UNIT)
FSOUND_DSPUNIT* dspUnitSpectrum;		// DSP SPECTRUM (UNIT)
FSOUND_STREAM * my_stream;				// MUSIQUE STREAMING
FSOUND_STREAM * my_mp3;					// MUSIQUE MP3
FMUSIC_MODULE * my_mod;					// MUSIQUE MOD
FMUSIC_MODULE * my_mod_about;			// MUSIQUE MOD FENETRE -ABOUT-
FSOUND_SAMPLE	* sound_tir,			// TIR JOUEUR
				* sound_tir3d,			// TIR 3D JOUEUR
				* sound_touche,			// TIR JOUEUR
				* sound_descente,		// SHIPS DESCENDENT D'UN CRAN
				* sound_applause,		// APLAUDISSEMENTS
				* sound_shield,			// BOUCLIER PASSAGE 3D
				* sound_cross,			// SON POUR LE PASSAGE 2D -> 3D
				* sound_bonus,			// BONUS ARME PRIS
				* sound_key;			// TOUCHE
bool	sound_cross_passed		= false;// SON CROSS PASSAGE 2D->3D

// SCROLL MUSIC TEXT
float	scroll_music_x			= 0;	// SCROLL COORD X
int		scroll_music_sens		= 1;	// SCROLL SENS DE DEPLACEMENT
float	scroll_music_total		= 0;	// SCROLL TOTAL DEPLACEMENT

// TEXTURES
GLuint	texture[100];					// TEXTURES[NOMBRE DE TEXTURES MAXIMUM]

// DEBUG : AFFICHAGE D'UNE ERREUR OPENGL
#define DEBUG_GL(command) command;fprintf(stderr,#command " ==> %s\n",gluErrorString(glGetError()));


/*___________________________________________________________________
|                                                                    |
|                           INITIALISATION                           |
|____________________________________________________________________| 
*/

//////////////////////////////////////////////////////////////////////////////////////
//
// Initialisation du jeu (fonctions GL et jeu)
//
//
BOOL Initialize (GL_Window* window, Keys* keys)					// Any GL Init Code & User Initialization Goes Here
{
	g_window	= window;										// infos de fenêtre en variable globale
	g_keys		= keys;											// infos de touches en variable globale

	// GAME OPTION -> SYNCHRO VERTICALE (VSYNC) SI DISPONIBLE
	typedef BOOL (WINAPI *PFNWGLSWAPINTERVALEXTPROC)(int interval);
	PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;
	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
	wglSwapIntervalEXT = ( BOOL (WINAPI *)(int)) wglGetProcAddress( "wglSwapIntervalEXT" );

	if (wglSwapIntervalEXT)
		if (game_option_vsync)
		{
			wglSwapIntervalEXT(1);
			game_vsync_active = true;
		}
		else
		{
			wglSwapIntervalEXT(0);
			game_vsync_active = false;
		}

	// GAME OPTION -> ANTI-ALIASING
	if (game_option_anti_aliasing)
	{
		//glEnable(GL_MULTISAMPLE_ARB);
		game_antialiasing_active = true;
	}
	else game_antialiasing_active = false;
	
	// MATHS -> INITIALISATION POUR LA FONCTION RANDOMIZE : RAND()
	srand((unsigned)timeGetTime());

	// SONS, MUSIQUE -> INITIALISATION FMOD
	if (!my_mod_about)	// si n'a pas déja été initialisé (càd l'utilisateur n'a pas ouvert la fenêtre -About-)
	{
		fmod_extract ();										// not to be removed

		if (!FSOUND_Init(44100, 32, 0))
		{
			MessageBox(0, FMOD_ErrorString(FSOUND_GetError()), "Sound Initialization Error", MB_TOPMOST);
			FSOUND_Close();
		}
	}

	// MUSIQUE -> DÉCALAGE DU SPECTRUM DÛ AU BUFFER DU DSP
	//int temp = FSOUND_DSP_GetBufferLengthTotal();
	//char temp2[200]="";
	//MessageBox(NULL,_itoa(temp, temp2, 10) , "", NULL); // => 4096 samples x 8 octets

	// MUSIQUE -> SPECTRUM ACTIVÉ
//	FSOUND_DSP_SetActive(FSOUND_DSP_GetFFTUnit(), TRUE);
//	FSOUND_DSP_SetActive(DspUnit, TRUE);
	game_spectrum = true;

	// SONS -> CREATION DSP
	// on créé un DSP pour y placer les sons du jeu (hors musique), avec une priorité
	// de 950, donc supérieure à celle du DSP FFT (utilisé pour le spectrum) qui est
	// de 900.	Ainsi les bruitages du jeu n'affecteront pas le spectrum, les DSP
	// étant exécutés dans l'ordre des priorités croissantes, de 0 à 1000.
	dspUnitSounds	= FSOUND_DSP_Create(&dspCallbackSounds, 950, 0);
	FSOUND_DSP_SetActive(dspUnitSounds, TRUE);

//	dspUnitSpectrum	= FSOUND_DSP_Create(&dspCallbackSpectrum, 850,	0);
//	OscBuffer		= (short*) calloc(FSOUND_DSP_GetBufferLengthTotal() + 16, 2); // *2 for mono 16bit buffer
//	FSOUND_DSP_SetActive(dspUnitSpectrum, TRUE);

	// 2D TEXT -> CHARGEMENT DE L'IMAGE DES FONTS
	resources_extract();										// extraction du fichier font.tga
	char Path[MAX_PATH+1];
	GetTempPath(sizeof Path, Path);
	strcat(Path, "\\SpaceInvaders-OpenGL\\font.tga");
	fontLoad (Path);											// GlTexFont (texte 2D)

	// OPENGL -> INITIALISATION
	glClearColor (0.0f, 0.0f, 0.0f, 0.0f);						// Black Background
	glClearDepth (1.0f);										// Depth Buffer Setup
	glEnable(GL_TEXTURE_2D);									// Enable Texture Mapping
	glDisable(GL_CULL_FACE);									// Front and Back faces of polygons are drawn

	glDepthFunc(GL_LEQUAL);										// Define Depth
	glDisable(GL_DEPTH_TEST);									// No depth test

	glBlendFunc(GL_SRC_ALPHA,GL_ONE);							// Define Alpha Blending
	glEnable(GL_BLEND);											// Enable Alpha Blending
	
	// OPENGL -> QUALITÉ
	glShadeModel (GL_SMOOTH);									// Smooth Shading for lines and polygons (for 3D objects)
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// Hint for perspective calculations (most accurate)
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);						// Hint for lines rasterization (progress bar, 3D objects in game_3d_lines mode...)
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);					// Hint for polygon rasterization (3D objects...)

	// OPENGL -> LUMIERES (UNIQUEMENT EN MODE 3D)

	static float ambient[] =			{0.9, 0.9, 0.9, 1.0};
	//static float ambient[] =			{1.0, 1.0, 1.0, 1.0};
	//static float ambient[] =			{0.8, 0.8, 0.8, 1.0};
	static float diffuse[] =			{0.25, 0.25, 0.25, 1.0};
	//static float diffuse[] =			{1.0, 1.0, 1.0, 1.0};
	static float specular[] =			{1.0, 1.0, 1.0, 1.0};
	
	static float ambient2[] =			{0.2, 0.2, 0.2, 1.0};
	static float diffuse2[] =			{0.05, 0.05, 0.05, 0.05};

	static float lmodel_ambient[] =		{0.1, 0.1, 0.1, 1.0};
	static float lmodel_localviewer[] =	{1.0};

	static float mat_shininess[] =		{10.0};
	static float mat_specular[] =		{0.8, 0.8, 0.8, 1.0};
	static float mat_ambient[] =		{0.6, 0.6, 0.6, 1.0};
	//static float mat_ambient[] =		{0.5, 0.5, 0.5, 1.0};
	static float mat_diffuse[] =		{0.4, 0.4, 0.4, 1.0};

	static float spot_direction[] =		{0.0, -1.0, 0.0};

	// lumière n°0, pour le vaisseau du joueur (mode 3D)
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	// lumière n°1, pour les vaisseaux enemis (mode 3D)
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	// lumière n°2, pour les vaisseaux enemis (mode 3D)
	glLightfv(GL_LIGHT2, GL_AMBIENT, ambient2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse2);

	// lumière (réfléchie sur les matériaux)
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);

	// lumière sur objet "TEXT 3D"
//	static float ambient2[] =			{0.7, 0.7, 0.7, 1.0};
//	static float diffuse2[] =			{0.5, 0.5, 0.5, 1.0};
//	static float spot_exponent2[] =		{110.0};
//	static float spot_cutoff2[] =		{35.0};

//	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse2);
//	glLightfv(GL_LIGHT2, GL_SPOT_EXPONENT, spot_exponent2);
//	glLightfv(GL_LIGHT2, GL_SPOT_CUTOFF, spot_cutoff2);

	// PASSAGE 2D à 3D -> camera path, courbe de Bézier (mouvement de caméra pour le passage au mode 3D)
	pcamera.x  =  0; pcamera.y = 0; pcamera.z  = 20;
	control_points[0].x =  0; control_points[0].y = 0; control_points[0].z = 20;
	control_points[1].x = -8; control_points[1].y = 0; control_points[1].z = 18;//15
	control_points[2].x = -2; control_points[2].y = 0; control_points[2].z =  5;//5
	control_points[3].x =  2; control_points[3].y = 0; control_points[3].z = -7;//3, -2
	control_points[4].x =  5; control_points[4].y = 0; control_points[4].z = 18;//15
	control_points[5].x =  0; control_points[5].y = 0; control_points[5].z = 20;

	// flottement caméra
	pcamera2.x  =  0; pcamera2.y = 0; pcamera2.z  = 0;

	// SPHERE
	quadratic=gluNewQuadric();							// Create A Pointer To The Quadric Object (Return 0 If No Memory)
	gluQuadricNormals(quadratic, GLU_SMOOTH);			// Create Smooth Normals 
	gluQuadricTexture(quadratic, GL_TRUE);				// Create Texture Coords 

	// LOADING -> vue orthogonale 2D
	glViewport(0, 0, (GLsizei)(g_window->init.width), (GLsizei)(g_window->init.height));
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0f, (GLfloat)(g_window->init.width)/ (GLfloat)(g_window->init.height), 0.1f, 500.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(	0.0, 0.0, 10.0,									// l'oeil
				0.0, 0.0,  0.0,									// le centre
				0.0, 1.0,  0.0	);								// le haut


	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////
//
// Désinitialisation => clic sur bouton Fermer de la fenêtre
//
//
void Deinitialize (void)
{
	set_hiscore(score);

	// FMOD ex-DLL DESINITIALISATION
	#ifndef CONFIG_FMOD_EXTRACT_YES
	FMOD_DllMain(GetModuleHandle(NULL), DLL_PROCESS_DETACH, NULL);
	#endif

}

/*___________________________________________________________________
|                                                                    |
|           U P D A T E  (K E Y B O A R D  H A N D L I N G)          |
|____________________________________________________________________| 
*/

//////////////////////////////////////////////////////////////////////////////////////
//
// Fonction UPDATE (gère entre autres les touches clavier)
//
//
void Update (DWORD milliseconds, GL_Window* window)
{
	// Lecture d'une musique en "FSOUND_NONBLOCKING" (streaming ou mp3) lorsqu'elle est prête
	if (music_type != 0 && int_sound_mp3 < 0)
	{
		try{
			if (music_type == 2)
				//int_sound_mp3 = FSOUND_Stream_Play(FSOUND_FREE, my_stream);
				int_sound_mp3 = FSOUND_Stream_PlayEx(FSOUND_FREE, my_stream, dspUnitSpectrum, false);
			else if (music_type == 1)
				//int_sound_mp3 = FSOUND_Stream_Play(FSOUND_FREE, my_mp3);
				int_sound_mp3 = FSOUND_Stream_PlayEx(FSOUND_FREE, my_mp3, dspUnitSpectrum, false);
		}
		catch(...)
		{
			ERR("Erreur 22b");
		}

		if (int_sound_mp3 >= 0)
		{
			// pour un streaming
			if (game_option_stream && id_my_music == 1)
			{
				FSOUND_SetVolume(int_sound_mp3, 0);
				_beginthread( FmodStreamFadingIn, 0, NULL );

				try{
					// le nom de la radio dans un premier temps
					void* temp;
					if (FSOUND_Stream_FindTagField(my_stream, FSOUND_TAGFIELD_SHOUTCAST, "icy-name", &temp, NULL))
					{
						strcpy(stream_name, (LPCTSTR) temp);
						strcpy(music_en_cours, stream_name);
						strcpy(music_affichee, stream_name);
						//OutputDebugString("aff1");
						strcat(music_affichee, "     ");							// pour affichage correct derniers caractères
					}

					// puis pour le nom de l'artiste et de la musique (récupérés plus bas)
					FSOUND_Stream_Net_SetMetadataCallback(my_stream, metacallback, 0);
				}
				catch(...)
				{
					ERR("Erreur 22e");
				}
			}
			
			// pour un mp3 en local
			else if (music_type == 1)
			{
				try{
					FSOUND_SetVolume(int_sound_mp3, 240);
				}
				catch(...){
					ERR("erreur 22f");
				}
			}
		}
	}

	// on récupère le nom de l'artiste et de la musique pour un stream
	if (music_type == 2)
		if ((TimerGetTime() - timer_1s) > 1000)						// chaque seconde on vérifie
		{
			timer_1s = TimerGetTime();
			if (metanum)
			{
				// nouvel artiste et nom de musique
				char temp[1024] = "";
				strcpy(temp, artist);
				strcat(temp, " - ");
				strcat(temp, title);
				//BOX(temp);
				
				// si le nom a changé on met à jour
				if (strcmp(temp, stream_name))
				{
					strcpy(stream_name, temp);
					strcpy(music_en_cours, stream_name);
					strcpy(music_affichee, stream_name);
					strcat(music_affichee, "     ");				// pour affichage correct derniers caractères

					// scrolling du titre -> réinitialisation
					scroll_music_x			= 0;
					scroll_music_sens		= 1;
					scroll_music_total		= 0;
				}
				metanum = 0;
			}
		}


	// ON QUITTE LE JEU
	if (g_keys->keyDown [VK_ESCAPE] == TRUE					// TOUCHE <ESC>
		|| window->keys->wivik[VK_ESCAPE] == TRUE)
	{
		// on enregistre le score si + grand que le hiscore
		set_hiscore(score);

		// on libère les ressources sonores
		FSOUND_Close();										// on ferme FMOD

		// on efface le répertoire temporaire et les fichiers temporaires
		char Path[MAX_PATH+1];
		char File[MAX_PATH+1];

		GetTempPath(sizeof Path, Path);
		strcat(Path, "\\SpaceInvaders-OpenGL");

		strcpy(File, Path);
		strcat(File, "\\font.tga");
		DeleteFile(File);

		RemoveDirectory(Path);

		// on termine l'application
		TerminateApplication (g_window);
	}

	// THIS IS FOR COMPATIBILITY WITH WIVIK ON-SCREEN KEYBOARD
	if (game_option_wivik)
		for (int i=0; i<=255; i++)
		{
			if (window->keys->wivik[i] == TRUE
				&& window->keys->keyDown[i] == TRUE)
				window->keys->wivik[i] = FALSE;

			if (window->keys->wivik[i] == TRUE
				&& window->keys->keyDown[i] == FALSE)
			{
				static float key_timer_wivik_left[255];
				if (i == VK_LEFT || i == VK_RIGHT || i == VK_UP)
				{
					if (wivik_wait(game_option_holddown_wivik, key_timer_wivik_left[i])) // xxx ms between change
						window->keys->wivik[i] = FALSE;
				}
				else
					if (wivik_wait(100, key_timer_wivik_left[i])) // 100 ms between change
						window->keys->wivik[i] = FALSE;
			}
		}

	// TOUCHE WINDOWS -> REDUCTION DE LA FENÊTRE SI MODE QUICK-FULLSCREEN
	if (g_keys->keyDown [VK_LWIN] == TRUE						// TOUCHE <WINDOWS>
		|| g_keys->keyDown [VK_RWIN] == TRUE
		|| g_keys->keyDown [VK_RETURN] == TRUE)
		if (window->init.quickFullScreen == TRUE)
		{
			//SendMessage(window->hWnd, WM_SIZE, SIZE_MINIMIZED, NULL);
			//SendMessage(window->hWnd, WM_ACTIVATE, SIZE_MINIMIZED, NULL);
			//ShowWindow(window->hWnd, SW_MINIMIZE);
			//SendMessage(window->hWnd, WM_SHOWWINDOW, SW_MINIMIZE, 0);
		
		}

	// DEPLACEMENT DU JOUEUR
	if (g_keys->keyDown [VK_LEFT] == TRUE						// TOUCHE <GAUCHE>
		|| window->keys->wivik[VK_LEFT] == TRUE)
	{
		//OutputDebugString("main keyDown: left DOWN");

		if (fps != 0 && player[0].alive)
		{
			if (!game_3d)
				player[0].x -= 3.0 / fps;
			else
			{
				player[0].movement -= 0.0005 *ms/10;//0.0005

				if (player[0].movement < - 0.005)
					player[0].movement = - 0.005;
			}
		}
	}

	if (g_keys->keyDown [VK_RIGHT] == TRUE						// TOUCHE <DROITE>
		|| window->keys->wivik[VK_RIGHT] == TRUE)
		if (fps != 0 && player[0].alive)
		{
			if (!game_3d)
				player[0].x += 3.0 / fps;
			else
			{
				player[0].movement += 0.0005 *ms/10;
				if (player[0].movement > + 0.005)
					player[0].movement = + 0.005;
			}
		}

	// TIR DU JOUEUR
	if (g_keys->keyDown [VK_UP] == TRUE
		|| g_keys->keyDown [VK_CONTROL] == TRUE					// TOUCHE <HAUT> OU <CTRL>
		|| window->keys->wivik[VK_UP] == TRUE)
	{
		if (player[0].alive != true) return;

		static float key_timer_tir;
		if (key_wait(100, key_timer_tir))						// 100 ms between change
		{
			nb_tir_index++;
			if (!game_3d)
			{
				int_sound = FSOUND_PlaySoundEx(FSOUND_FREE,sound_tir, dspUnitSounds, false);
				FSOUND_SetVolume(int_sound,40);//50 40
			}
			else
			{
				int_sound = FSOUND_PlaySoundEx(FSOUND_FREE,sound_tir3d, dspUnitSounds, false);
				FSOUND_SetVolume(int_sound,20);//22
			}
			tir[id_tir].type	= shoot_type;
			tir[id_tir].active	= true;
			tir[id_tir].x		= player[0].x;
			if (shoot_type == 2)
				tir[id_tir].tail	= true;
			else tir[id_tir].tail	= false;
			
			if (game_3d && (shoot_type == 0 || shoot_type == 1))
				tir[id_tir].y		= player[0].y + 1.5f;
			else tir[id_tir].y		= player[0].y + .5f;

			if (!game_3d)
				tir[id_tir].z	= player[0].z;
			else
			{
				tir[id_tir].z	= m.pos.z;
				//if (shoot_type == 2)
				//	init_tir3d(id_tir);
			}
			
			id_tir++;
			if (id_tir > max_tirs)							// x tirs au maximum en même temps
				id_tir = 0;									// remise à zéro


			if (player[0].laser_left)						// le laser de gauche tire
			{
				tir[id_tir].type	= 5;
				tir[id_tir].x		= player[0].x - .9f;
				tir[id_tir].y		= player[0].y + .8f;
				tir[id_tir].z		= m.pos.z;
				tir[id_tir].active	= true;
				tir[id_tir].tail	= false;
				id_tir++;
				if (id_tir > max_tirs)						// x tirs au maximum en même temps
					id_tir = 0;								// remise à zéro

			}
			if (player[0].laser_right)						// le laser de droite tire
			{
				tir[id_tir].type	= 5;
				tir[id_tir].x		= player[0].x + .9f;
				tir[id_tir].y		= player[0].y + .8f;
				tir[id_tir].z		= m.pos.z;
				tir[id_tir].active	= true;
				tir[id_tir].tail	= false;
				id_tir++;
				if (id_tir > max_tirs)						// x tirs au maximum en même temps
					id_tir = 0;								// remise à zéro
			}

		}
	}

	// ZOOM +
	if (g_keys->keyDown [VK_ADD] == TRUE						// TOUCHE <+>
		|| window->keys->wivik[VK_ADD] == TRUE)
	{
		zoom =  0.02f * ms;
		zoom_total += zoom;
	}

	// ZOOM -
	if (g_keys->keyDown [VK_SUBTRACT] == TRUE					// TOUCHE <->
		|| window->keys->wivik[VK_SUBTRACT] == TRUE)
	{
		zoom = -0.02f * ms;
		zoom_total += zoom;
	}

	// POUR TESTS
	if (g_keys->keyDown [VK_F4] == TRUE							// TOUCHE <F4>
		|| window->keys->wivik[VK_F4] == TRUE)
	{
		static float key_timer_temp;
		if (!key_wait(200, key_timer_temp)) return;				// 200 ms between change

	}

	// CHANGEMENT DE MODE (GAME 2D / 3D)
	if (g_keys->keyDown [VK_F1] == TRUE							// TOUCHE <F1>
		|| window->keys->wivik[VK_F1] == TRUE)
	{
		static float key_timer_game_3d;
		if (!key_wait(200, key_timer_game_3d)) return;			// 200 ms between change

		game_3d = !game_3d;										// on change de mode

		key_sound();

		// paramètres pour la 2D
		if (!game_3d)
		{
			glDisable(GL_CULL_FACE);
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);

			player[0].y	= -6.15;
			shoot_type = 0;
			player[0].laser_left = false;
			player[0].laser_right = false;
		}
		
		// paramètres pour la 3D
		else
		{
			player[0].y	= -7;
			player[0].movement = 0;
		}

		ReshapeGL(window->init.width, window->init.height);
	}

	// MODE MORTEL/IMMORTEL
	if ( (g_keys->keyDown [VK_F5] == TRUE && g_keys->keyDown [VK_F6] == TRUE) // TOUCHES <F5> + <F6>
		|| (window->keys->wivik[VK_F5] == TRUE && window->keys->wivik[VK_F6] == TRUE)
		|| window->keys->wivik[VK_F10] == TRUE )
	{
		static float key_timer_game_immortal;
		if (!key_wait(200, key_timer_game_immortal)) return;	// 200 ms between change

		key_sound();
		
		game_immortal = !game_immortal;
		if (game_immortal)
			strcpy(game_message, "Enter immortal mode.");
		else
			strcpy(game_message, "Enter mortal mode.");
	}

	// CHANGEMENT BACKGROUND/LEVEL
	if (g_keys->keyDown [0x42] == TRUE || g_keys->keyDown [VK_F2] == TRUE// TOUCHE <B> ou <F2>
		|| window->keys->wivik[0x42] == TRUE || window->keys->wivik[VK_F2] == TRUE)
	{
		static float key_timer_game_level;
		if (!key_wait(200, key_timer_game_level)) return;		// 200 ms between change

		key_sound();

		if (game_level++ == 5)
			game_level = 1;

		if (game_3d)
			game_3d_lines = !game_3d_lines;
	}

	// LASERS JOUEUR ACTIVÉS OU PAS
	if (g_keys->keyDown [VK_F3] == TRUE						// TOUCHE <F3>
		|| window->keys->wivik[VK_F3] == TRUE)
	{
		static float key_timer_game_lasers;
		if (!key_wait(200, key_timer_game_lasers)) return;		// 200 ms between change

		key_sound();

		if (!player[0].laser_left || !player[0].laser_right)
		{
			player[0].laser_left = true;
			player[0].laser_right = true;
		}
		else
		{
			player[0].laser_left = false;
			player[0].laser_right = false;
		}
	}

	// ACTIVE/DESACTIVE LA CONSOLE
	if (g_keys->keyDown [222] == TRUE						    // TOUCHE <²>
		|| window->keys->wivik[222] == TRUE)
	{
		static float key_timer_console;
		if (!key_wait(200, key_timer_console)) return;			// 200 ms between change

		key_sound();

		if (!game_console_moving)
		{
			game_console_start = true;
			game_console_moving = true;
		}
	}

	// TAPE UN CARACTERE DANS LA CONSOLE, SI UNE TOUCHE RESERVEE [LETTRE, BARRE D'ESPACE...] N'A PAS ÉTÉ TAPÉE
	if (game_console)
		if (g_keys->keyDown[VK_LEFT]     != TRUE && g_keys->keyDown[VK_RIGHT] != TRUE
		 && g_keys->keyDown[VK_UP]       != TRUE && g_keys->keyDown[VK_F1]    != TRUE
		 && g_keys->keyDown[VK_F2]		 != TRUE && g_keys->keyDown[VK_F3]    != TRUE
		 && g_keys->keyDown[VK_F5]       != TRUE && g_keys->keyDown[VK_F6]    != TRUE
		 && g_keys->keyDown[222]         != TRUE && g_keys->keyDown[VK_ADD]   != TRUE
		 && g_keys->keyDown[VK_SUBTRACT] != TRUE)
		{
			static float key_timer_console_text;
			static int last_key = 0;
			static bool last_key_released = false;
			bool first_key_displayed = false;
		
			if (g_keys->keyDown[last_key] == FALSE)
				last_key_released = true;
		
			if (!last_key_released)
				if (!key_wait(300, key_timer_console_text) && g_keys->keyDown[last_key] == TRUE && last_key != 0)
					return;

			int key_pressed = 0;
		
			for(int i=0; i<=255; i++)
			{
				if (g_keys->keyDown [i] == TRUE && !first_key_displayed)
				{
					key_pressed = i;
					last_key = key_pressed;
					last_key_released = false;
	
					char new_char[2] = "";
					new_char[0] = key_pressed;
					new_char[1] = '\0';

					if (key_pressed == VK_BACK)					// BACKSPACE (=8)
						console_text[console_line][strlen(console_text[console_line])-1] = '\0';
					else if (key_pressed == VK_TAB)				// TAB (=9)
						strcpy(console_text[console_line], console_command);
					else if (key_pressed == VK_RETURN)			// RETURN (=13)
						{
							if (strcmp(console_text[console_line], "CLS") == 0)	// clear screen
							{
								strcpy(console_command, "");
								strcpy(console_command2, "");
								strcpy(console_output, "");
								strcpy(console_output2, "");
								strcpy(console_text[console_line], "");
							}
							else
							{
								strcpy(console_output2, console_output);
								strcpy(console_command2, console_command);
								strcpy(console_command, console_text[console_line]);
								strcpy(console_text[console_line], "");
							}
						}
					else
						if (strlen(console_text[console_line]) < 38)
							strcat(console_text[console_line], new_char);

					first_key_displayed = true;
				}
				g_keys->keyDown [i] = FALSE;
			}
			return;
		}
		else // console (²) + touche réservée appuyée
			return;


	// REPLACE LA CAMERA CORRECTEMENT
	if (g_keys->keyDown [0x52] == TRUE							// TOUCHE <R> (RESHAPE)
		|| window->keys->wivik[0x52] == TRUE)
	{
		ReshapeGL(window->init.width, window->init.height);
	}

	// MUSIQUE ON/OFF
	if (g_keys->keyDown [0x4d] == TRUE							// TOUCHE <M>
		|| window->keys->wivik[0x4d] == TRUE)
	{
		static float key_timer_music_on_off;
		if (!key_wait(200, key_timer_music_on_off)) return;		// 200 ms between change

		if (music_on)
		{
			if (int_sound_mp3 >= 0)
			{
				try{
					FSOUND_SetVolume(int_sound_mp3, 0);
				}
				catch(...){
					ERR("Erreur 11a");
				}
			}

			try{
				FMUSIC_SetMasterVolume(my_mod, 0);
				music_on = false;
			}
			catch(...){
				ERR("Erreur 11b");
			}
		}
		else
		{
			if (int_sound_mp3 >= 0)
			{
				try{
					FSOUND_SetVolume(int_sound_mp3, 240);
				}
				catch(...){
					ERR("Erreur 11c");
				}
			}

			try{
				FMUSIC_SetMasterVolume(my_mod, 240);
				music_on = true;
			}
			catch(...){
				ERR("Erreur 11d");
			}
		}
	}

	// AFFICHE / CACHE LES TEXTES 2D
	if (g_keys->keyDown [0x54] == TRUE							// TOUCHE <T>
		|| window->keys->wivik[0x54] == TRUE)
	{
		static float key_timer_text2d;
		if (!key_wait(200, key_timer_text2d)) return;			// 200 ms between change

		key_sound();

		game_2d_text = !game_2d_text;
	}

	// PASSE INTRO / MUSIQUE AVANCE
	// ON LIT LE MP3 SUIVANT DU REPERTOIRE COURANT
	if (g_keys->keyDown [VK_SPACE] == TRUE						// TOUCHE <ESPACE>
		|| window->keys->wivik[VK_SPACE] == TRUE)
	{
		if (game_step != 4 || !music_on)
			//|| (my_stream && FSOUND_Stream_GetOpenState(my_stream) != 0
			//	&& !(game_option_stream && id_my_music == 1)) )	// en mode FSOUND_NONBLOCKING le stream internet peut-être interrompu mais pas les mp3 locaux
			return;

		//if (my_stream && FSOUND_Stream_GetOpenState(my_stream) != 0)
		// la musique doît être prête sinon on retourne (à cause du tag FSOUND_NONBLOCKING utilisé)
		//if (int_sound_mp3 < 0)
		if (music_type == 1 && int_sound_mp3 < 0)
		{
			return; // temporaire (sinon bug si barre d'espace laissée appuyée)
			//if (game_option_stream)
			//{
			//	if (id_my_music != 1)
			//		return;
			//}
			//else return;
		}

		try{
			static float key_timer_music_next;
			if (!key_wait(200, key_timer_music_next)) return;	// 200 ms between change
		}
		catch(...){
			ERR("erreur 00a1");
		}
		
		// a-t'on des musiques mp3 dans la liste des noms de fichiers musicaux ? (càd dans le répertoire du jeu)
		try{
		if (strstr(music[1].fichier, ".mp3") == NULL && !game_option_stream)
			if (!FMUSIC_IsFinished(my_mod))
			{
				switch(game_lang)
				{
					case 0:
						strcpy(game_message, "Please put some mp3 files in game directory.");
						break;
					case 1:
						strcpy(game_message, "Veuillez placer quelques mp3 dans le rep. du jeu svp.");
						break;
					case 2:
						strcpy(game_message, "Por favor, copie algunos archivos MP3 al directorio del juego  ");
						break;
				}
				return;
			}
		}
		catch(...){
			ERR("erreur 00a3");
		}

		// on arrête et ferme la musique en cours pour démarrer la suivante
		try{
			if (music_type == 0)
				FMUSIC_StopSong(my_mod);
		}
		catch(...)
		{
			ERR("erreur 00b");
		}

		try{
			if (music_type == 1)
			{
				FSOUND_Stream_Stop(my_mp3);
				FSOUND_Stream_Close(my_mp3);	// free memory
				my_mp3 = NULL;
			}
		}
		catch(...)
		{
			ERR("erreur 00c");
		}

		try{
			if (music_type == 2 && my_stream)
			{
					FSOUND_Stream_Close(my_stream);
					my_stream = NULL;
			}
		}
		catch(...)
		{
			ERR("erreur 00d");
		}
			
		// on passe à la musique suivante
		try{
			id_my_music++;
			if ( strstr(music[id_my_music].fichier, ".mp3") == NULL
				&& strstr(music[id_my_music].fichier, "Streaming") == NULL )// est-on à la fin de la liste
			{
				id_my_music = 0;									// si oui on repart du premier morceau (XM)
			}
		}
		catch(...){
			ERR("erreur 00e");
		}
		
		// on récupère le nom de la musique
		try{
			strcpy(music_en_cours, music[id_my_music].fichier);
			strcpy(music_affichee, music[id_my_music].fichier);
			//OutputDebugString("aff2");
			strcat(music_affichee, "     ");						// pour affichage correct derniers caractères
		}
		catch(...){
			ERR("erreur 00f");
		}

		// TROIS TYPES DE MUSIQUE
		// ----------------------

		// 1) LECTURE DE LA MUSIQUE - MOD -
		if (strstr(music[id_my_music].fichier, "1 000 000 miles by ronny / teklords") != NULL)
		{
			try{
				music_type = 0;
				int_sound_mod = FMUSIC_PlaySong(my_mod);
				FMUSIC_SetMasterVolume(my_mod,240);
				//OutputDebugString("aff3");
			}
			catch(...){
				ERR("erreur 00g");
			}
		}
		
		// 2) LECTURE D'UNE MUSIQUE - EN STREAMING -
		else if (strstr(music[id_my_music].fichier, "Streaming") != NULL)
			try{
				music_type = 2;

				if (my_playlist)
					strcpy(music_en_cours, (char*) my_playlist->name[0]);
				else
					strcpy(music_en_cours, stream_name_user);

				my_stream = FSOUND_Stream_Open(music_en_cours, FSOUND_NORMAL | FSOUND_NONBLOCKING, 0, 0);
				//int_sound_mp3 = FSOUND_Stream_Play(FSOUND_FREE, my_stream);
				int_sound_mp3 = FSOUND_Stream_PlayEx(FSOUND_FREE, my_stream, dspUnitSpectrum, false);
			}
			catch(...){
				ERR("erreur 00h");
			}
		
		// 3) LECTURE D'UNE MUSIQUE - MP3 -
		else
		{
			try{
				music_type = 1;
				my_mp3 = FSOUND_Stream_Open(music_en_cours, FSOUND_NORMAL | FSOUND_NONBLOCKING | FSOUND_LOOP_OFF, 0, 0);
				//int_sound_mp3 = FSOUND_Stream_Play(FSOUND_FREE, my_mp3);
				int_sound_mp3 = FSOUND_Stream_PlayEx(FSOUND_FREE, my_mp3, dspUnitSpectrum, false);
			}
			catch(...){
				ERR("erreur 00i");
			}
		}

		// SCROLLING DU TITRE -> RÉINITIALISATION
		scroll_music_x			= 0;
		scroll_music_sens		= 1;
		scroll_music_total		= 0;
	}

	// ACTIVE/DESACTIVE LE SPECTRUM
	if (g_keys->keyDown [0x53] == TRUE							// TOUCHE <S>
		|| window->keys->wivik[0x53] == TRUE)
	{
		static float key_timer_spectrum;
		if (!key_wait(200, key_timer_spectrum)) return;			// 200 ms between change

		key_sound();

		game_spectrum = !game_spectrum;							// on change de mode

		if (game_spectrum)
			FSOUND_DSP_SetActive(FSOUND_DSP_GetFFTUnit(), TRUE);
		else
			FSOUND_DSP_SetActive(FSOUND_DSP_GetFFTUnit(), FALSE);
	}

	// ANTI-ALIASING ON/OFF
	if (g_keys->keyDown [0x41] == TRUE							// TOUCHES <A>
		|| window->keys->wivik[0x41] == TRUE)
	{
		static float key_timer_aa;
		if (!key_wait(200, key_timer_aa)) return;				// 200 ms between change

		key_sound();
		
		if (!game_option_anti_aliasing)
		{
			switch(game_lang)
			{
				case 0:
					strcpy(game_message, "Please choose Anti-aliasing option on start.");
					break;
				case 1:
					strcpy(game_message, "Veuillez choisir l'option Antialiasing au demarrage.");
					break;
				case 2:
					strcpy(game_message, "Por favor, elija la opcion de Anti-aliasing al comienzo");
					break;
			}
			return;
		}
		else if (game_3d_lines && game_3d)
		{
			switch(game_lang)
			{
				case 0:
					strcpy(game_message, "Anti-aliasing not used in 3D Line mode.");
					break;
				case 1:
					strcpy(game_message, "L'anti-aliasing n'est pas utilise dans ce mode.");
					break;
				case 2:
					strcpy(game_message, "Anti-aliasing sin usarse en el modo lineal 3D");
					break;
			}
			return;
		}

		game_antialiasing_active = !game_antialiasing_active;
		
		if (game_antialiasing_active)
			strcpy(game_message, "Anti-aliasing : ON");
		else
			strcpy(game_message, "Anti-aliasing : OFF");
	}

	// VSYNC ON/OFF
	if (g_keys->keyDown [0x56] == TRUE							// TOUCHES <V>
		|| window->keys->wivik[0x56] == TRUE)
	{
		static float key_timer_vsync;
		if (!key_wait(200, key_timer_vsync)) return;			// 200 ms between change

		key_sound();

		// get function
		typedef BOOL (WINAPI *PFNWGLSWAPINTERVALEXTPROC)(int interval);
		PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;
		wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
		wglSwapIntervalEXT = ( BOOL (WINAPI *)(int)) wglGetProcAddress( "wglSwapIntervalEXT" );

		// swap on/off
		if (wglSwapIntervalEXT)
		{
			game_vsync_active = !game_vsync_active;
			
			if (game_vsync_active)
				wglSwapIntervalEXT(1);
			else
				wglSwapIntervalEXT(0);
		}
		else
		{
			strcpy(game_message, "Sorry, SwapIntervalExt() not available.");
			return;
		}

		// game message
		if (game_vsync_active)
			strcpy(game_message, "Vertical Synchronization : ON");
		else
			strcpy(game_message, "Vertical Synchronization : OFF");
	}

	// FULLSCREEN / WINDOW
	if (g_keys->keyDown [VK_F12] == TRUE)					  // TOUCHE <>
	{
		//MessageBox(0,"","",0);
		Fullscreen();
	}
	

	static float fov = 45;
	// TESTS
	if (g_keys->keyDown [0x57] == TRUE)							// TOUCHE <W>
	{
		fov += 1;
		glViewport(0, 0, (GLsizei)(g_window->init.width), (GLsizei)(g_window->init.height));
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(fov, (GLfloat)(g_window->init.width) / (GLfloat)(g_window->init.height), 0.1f, 500.0f);
		glMatrixMode(GL_MODELVIEW);
	}

	// TESTS
	if (g_keys->keyDown [0x58] == TRUE)							// TOUCHE <X>
	{
		fov -= 1;
		glViewport(0, 0, (GLsizei)(g_window->init.width), (GLsizei)(g_window->init.height));
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(fov, (GLfloat)(g_window->init.width) / (GLfloat)(g_window->init.height), 0.1f, 500.0f);
		glMatrixMode(GL_MODELVIEW);
	}
}


/*___________________________________________________________________
|                                                                    |
|        D R A W  (D I S P L A Y  2 D / 3 D  E L E M E N T S)        |
|____________________________________________________________________| 
*/

//////////////////////////////////////////////////////////////////////////////////////
//
// Fonction DRAW (affichage)
//
//
void Draw (GL_Window* window)
{
	// --------------- TEST : BUG PASSAGE 3D ----------------
	#ifdef DEBUG_PASSAGE_3D
	
	// retour à 2D
	g_keys->keyDown [VK_F1] = FALSE;
	if (bonus[1].explosion == false && intermede == false && game_3d)
		g_keys->keyDown [VK_F1] = TRUE;
	// tirs non-stop
	g_keys->keyDown [VK_UP] = TRUE;

	#endif // ----------------------------------------------
	
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);							// efface les buffers
	glEnable (GL_TEXTURE_2D);

	// VIEWPORT PRINCIPAL (LE JEU)
	glViewport(0, 0, (GLsizei)(window->init.width), (GLsizei)(window->init.height));// Reset The Current Viewport
	glMatrixMode(GL_PROJECTION);													// Select The Projection Matrix
	glLoadIdentity();																// Reset The Projection Matrix
	gluPerspective(45.0f, (GLfloat)(window->init.width)/							// Calculate The Aspect Ratio Of The Window
		(GLfloat)(window->init.height),	0.1f, 500.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// TIMING METHODE 2
	sec_start = GetSysTimeSeconds();
	if (Fps2_firstpass)
		sec_frame = 0.010;
	else
		sec_frame = (sec_start - sec_end);
	sec_end = GetSysTimeSeconds();
	if (sec_frame > 0.1)
		sec_frame = 0.1;															// on met une limite à 100ms
	Fps2_firstpass = false;

	static char debugprint[]="";
	static int decimal, sign;
	//OutputDebugString(_itoa((int)_fps, debugprint, 10));
	//OutputDebugString(_ecvt( my_seconds, 10, &decimal, &sign ));

	// l'energy du joueur se régénère lentement
	if (player[0].energy < 20)
		player[0].energy += ms10/5000;
	if (player[0].energy > 20)
		player[0].energy = 20;

	// PASSAGE AU MODE 3D (MOUVEMENT DE CAMERA - COURBES DE BEZIER)
	//
	// Le mouvement de caméra chevauche 2 parties : 2D et 3D (voir ci-dessous)
	// Pour chaque partie il faut normaliser t (le temps), car la fonction Bezier() requiert 0 <= t <= 1
	// puis on utilise ce t pour modifier d'autres paramètres comme les angles, le zoom, ...
	//
	if (bonus[1].explosion)
	{
		// on se rapproche du joueur 2D (de 4 à 2)
		if (bonus[1].explosion_life >= 2.0)
		{
			t =( bonus[1].explosion_life -4 ) /-2 /2;				// t de 0 à 0.5
			angle_x = angle_x_start * (1 - t*2);					// de angle_x_start à 0
			angle_y = angle_y_start * (1 - t*2);					// de angle_y_start à 0
			zoom_total_bonus = zoom_total * (1 - t*2);				// de zoom_total à 0
		}
		// puis on s'éloigne du joueur 3D (de 2 à 0)
		else
		{
			t =( bonus[1].explosion_life -2 ) / -4 +0.5;			// t de 0.5 à 1
			angle_y = ( t -0.5 )*-40 *2;							// de 0 à -40
			player[0].y = (t -0.5)*2 *-0.85 -6.15;					// de -6.15 à -7
			zoom_total = zoom_total_bonus;							// zoom_total devient zoom_total_bonus pour l'après passage 3D car un léger décalage existe
		}
		
		pcamera = Bezier(control_points, 5, t);					// on récupère les coordonnées de la caméra

		// SINUS : FONCTION D'ACCÉLÉRATION ET DÉCÉLÉRATION PROGRESSIVES
		//
		// En fonction de t (de 0 à 1) retourne une valeur de 0 à 1 puis de nouveau à 0.
		// Pour cela on utilise la courbe de sinus qui, de 270° à 630°, retourne une
		// valeur de -1 à 1 puis à -1. il suffit alors de normaliser avec : (x+1)/2
		// Remarque : pour une valeur retournée de 0 à 1, remplacer rad630 par rad450 = 5*PI/2;
		//
		static float rad270 = 3*PI/2;								// 270° en radians
		static float rad630 = 7*PI/2;								// 630° en radians
		float sinus = (sin((t*rad630)/(-rad630/(rad630 - rad270)) +rad270)+1) /2;

		pcamera.y = sinus * -12;									// de 0 à -12 puis à 0 (avec accélération et décélération)
		ciblex = sinus * player[0].x /2;							// de 0 à player[0].x/2 puis à 0 (avec accélération et décélération)
		cibley = sinus * player[0].y * 0.8;							// de 0 à player[0].y*0.8 puis à 0 (avec accélération et décélération)

		// son CROSS (2D -> 3D)
/*		if (!sound_cross_passed && bonus[1].explosion_life <= 2.5 && bonus[1].explosion_life > 0.0)//...3.0...
		{
			sound_cross_passed = true;
			int_sound = FSOUND_PlaySoundEx(FSOUND_FREE,sound_cross, dspUnitSounds, false);
			FSOUND_SetVolume(int_sound,255);
		}
*/
	}

	// FLOTTEMENT CAMERA
/*	if (game_3d && !bonus[1].explosion)
	{
		static float tx = 0;
		static float ty = 0;
		static float tz = 0;

		// à modifier
		tx += 0.0002 * ms10;
		ty += 0.00025 * ms10;
		tz += 0.00015 * ms10;

		if (tx > 1) tx -= 1;
		if (ty > 1) ty -= 1;
		if (tz > 1) tz -= 1;

//		pcamera2.x = 1.0 *cos(tx * 2*PI);
//		pcamera2.y = 0.7 *sin(ty * 2*PI);
//		pcamera2.z = 0.6 *sin(tz * 2*PI);

		pcamera2.x = 1.0 *cos(tx * 2*PI);
		pcamera2.y = 0.7 *sin(ty * 2*PI);
		pcamera2.z = 0.6 *sin(tz * 2*PI);
	}*/

	//
	// POSITIONNEMENT CAMERA (OPERATIONS SUR LA MATRICE MODELVIEW)
	//

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();												// position initiale

	// CAMERA -> position par défaut du jeu
	gluLookAt(	pcamera.x+pcamera2.x, pcamera.y+pcamera2.y, pcamera.z+pcamera2.z,// la caméra
				ciblex, cibley, 0,									// la cible
				0.0, 1.0,  0.0	);									// le haut

	// CAMERA -> modification du zoom pendant le passage 2D->3D
	if (bonus[1].explosion)
		glTranslatef ( 0.0, 0.0, zoom_total_bonus);					// zoom pendant le passage à mode 3D
	else
		glTranslatef ( 0.0, 0.0, zoom_total );						// zoom utilisateur
	
	// CAMERA -> modification pendant l'intermède (rotation de la scène)
	//
	// la rotation de l'intermède ne se fait pas directement avec un angle incrémenté
	// pour éviter des opérations trop nombreuses sur la matrice MODEL_VIEW, mais sur
	// un angle global après la réinitialisation de la matrice par glLoadIdentity() (ci-dessus)
	glRotatef(intermede_angle_total, -0.3f, 0.55f, 1.0f);			// rotation de l'intermède -.3 .5 1.0

	// CAMERA -> modification utilisateur (clic et déplacement de la souris)
	glRotatef(angle_y, 1.0, 0.0, 0.0);								// rotation utilisateur (par rapport à l'axe des x)
	glRotatef(angle_x, 0.0, 1.0, 0.0);								// rotation utilisateur (par rapport à l'axe des y)

	// LUMIERES -> POSITION
	static float position_player[] =		{-10.0, 20.0, 1.0, 0.0};// for player
	static float position_ships[] =			{-10.0, 10.0, 5.0, 0.0};// for ships
	static float position_ships2[] =		{ 10.0, 0.0, -20.0, 0.0};// for ships
	glLightfv(GL_LIGHT0, GL_POSITION, position_player);
	glLightfv(GL_LIGHT1, GL_POSITION, position_ships);
	glLightfv(GL_LIGHT2, GL_POSITION, position_ships2);

	// AFFICHAGE DE DIVERS ELEMENTS DU JEU

	display_splashscreen();										// Splash screen
	display_background(false);										// Fond	
	display_bonus_2d_to_3d();										// Bonus 2D to 3D
	display_bonus_3d();												// Bonus 3D
	display_below_lives();											// Nombre de vies
	display_score_hiscore_lives();									// Score, Hiscore, Lives
	display_explosions(false);										// Explosions des ships
	#ifdef DEBUG_BUNKERS
	display_bunkers();												// Bunkers
	#endif
	display_console();												// Console

	// BONUS -> CREATION ALEATOIRE D'UN BONUS - 2D -
	#ifdef DEBUG_PASSAGE_3D
	if ((bonus[1].active == false) && (floor((rand()%100)/ms10) == 1) && intermede == false && !game_3d)
	#else
//	if ((bonus[1].active == false) && (floor((rand()%10000)/ms10) == 1) && intermede == false && !game_3d)
	if ((bonus[1].active == false) && intermede == false)
	#endif
		init_bonuses(false);

	// BONUS -> CREATION ALEATOIRE D'UN BONUS - 3D -
	if (!player[0].laser_left || !player[0].laser_right)
	{
		if ((bonus[2].active == false) && (floor((rand()%15000)/ms10) == 1) && intermede == false && game_3d)
			init_bonuses(true);
	}
	else if (shoot_type < 1)
			if ((bonus[2].active == false) && (floor((rand()%300000)/ms10) == 1) && intermede == false && game_3d)
				init_bonuses(true);

	//
	// BONUS -> GESTION DES BONUS EN MODE 2D
	//
	if (bonus[1].active && bonus[1].explosion == false)
	{
		// LE JOUEUR PRENDS LE BONUS (MODE 2D)
		if (player[0].alive && !game_3d)
		{
			g1 = player[0].x - player[0].size_x;
			d1 = player[0].x + player[0].size_x;
			h1 = player[0].y + player[0].size_y;
			b1 = player[0].y - player[0].size_y;

			g2 = bonus[1].x - .4;
			d2 = bonus[1].x + .4;
			h2 = bonus[1].y + .4;
			b2 = bonus[1].y - .0;
		
			if (collision(g1, d1, h1, b1, g2, d2, h2, b2))
			{
				int_sound = FSOUND_PlaySoundEx(FSOUND_FREE,sound_shield, dspUnitSounds, false);
				FSOUND_SetCurrentPosition(int_sound, 1000);
				FSOUND_SetVolume(int_sound,200);
				bonus[1].explosion = true;

				angle_x_start = fmod(angle_x, 360);				// on veut un angle de -360 à 360 (le reste du modulo)
				angle_y_start = fmod(angle_y, 360);				// idem
			}
		}

		// déplacement vers le bas
		if (bonus[1].explosion == false)
			#ifdef DEBUG_PASSAGE_3D
			bonus[1].y -= 0.02 * ms;
			bonus[1].x = player[0].x;
			#else
			bonus[1].y -= 0.002 * ms;
			#endif

		// devient inactif s'il sort de l'écran
		if (bonus[1].y < -10)
			bonus[1].active = false;
	}

	//
	// BONUS -> GESTION DES BONUS EN MODE 3D
	//
	if (bonus[2].active && bonus[2].explosion == false)
	{
		// LE JOUEUR PRENDS LE BONUS (MODE 3D)
		if (player[0].alive && game_3d)
		{
			g1 = player[0].x - player[0].size_x;
			d1 = player[0].x + player[0].size_x;
			h1 = player[0].y + player[0].size_y + 0.7;
			b1 = player[0].y - player[0].size_y;

			g2 = bonus[2].x - 1.0;
			d2 = bonus[2].x + 1.0;
			h2 = bonus[2].y + .4;
			b2 = bonus[2].y - .0;
		
			if (collision(g1, d1, h1, b1, g2, d2, h2, b2))
			{
				// on lance le son 2 fois pour un volume 2 fois plus fort
				int_sound = FSOUND_PlaySoundEx(FSOUND_FREE,sound_bonus, dspUnitSounds, false);
				FSOUND_SetVolume(int_sound,255);
				int_sound = FSOUND_PlaySoundEx(FSOUND_FREE,sound_bonus, dspUnitSounds, false);
				FSOUND_SetVolume(int_sound,255);
				//bonus[2].explosion = true;
				bonus[2].active = false;
				
				// BONUS lasers joueur
				if (bonus[2].type == 1)
				{
					player[0].laser_left = true;
					player[0].laser_left_explosion = false;
					player[0].laser_right = true;
					player[0].laser_right_explosion = false;
				}
				// BONUS nouvelle arme
				else if (bonus[2].type == 2)
				{
					shoot_type++;
					if (shoot_type == 2)//3
						shoot_type = 0;
				}
			}
		}

		// déplacement vers le bas du bonus
		bonus[2].y -= 0.0025 * ms;//0.002

		// devient inactif s'il sort de l'écran
		if (bonus[2].y < -10)
			bonus[2].active = false;
	}

	//
	// TIRS -> AFFICHAGE ET DÉPLACEMENTS DES TIRS JOUEUR ET ENEMIS
	//
	for (int k=0; k <= max_tirs; k++)							// pour chaque tir de la scène
	{
		if (tir[k].active)										// si il est actif...
		{
			//----------------------
			display_shoot(k, false);							// on l'affiche
			//----------------------

			// TIR PLAYER
			if (tir[k].type < 10)								// si le tir est du player
			{
				if (!game_3d)
					tir[k].y += 0.008 * ms10;					// il se déplace vers le haut
				else
					//tir[k].y += 0.0005 * ms10;//0.017
					tir[k].y += 0.017 * ms10;
			}
			// TIR ENEMI (SHIP)
			else if (tir[k].type == 10)							// si le tir est d'un ship
			{
				tir[k].y -= 0.004 * ms10;						// il se déplace vers le bas
			}
		}

		// AFFICHE LES PARTICULES (LA TRAINEE) DU TIR 3D LORSQUE LE TIR EST DEVENU INACTIF
		else if (game_3d && tir[k].tail)
		{
			// ---------------------
			display_shoot(k, false);
			// ---------------------
		}
	}


	// 3D WRAPPER (DEBUT)
	if (game_3d)
	{
		glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
		glEnable(GL_CULL_FACE);		// on n'affiche pas les faces cachées
		glFrontFace(GL_CCW);		// les faces frontales (non cachées) sont dans le sens CCW
		glEnable(GL_DEPTH_TEST);	// active le test de profondeur (les faces situées derrière d'autres ne seront pas affichées)
		glDisable(GL_BLEND);		// désactive le mélange de couleurs (ici transparence)
	}

	// LUMIERE PLAYER (DEBUT)
	if (game_3d)
	{
		glEnable(GL_LIGHTING);		// active les lumières
		glEnable(GL_LIGHT0);		// allume la lumière player
	}

	// JOUEUR -> DÉPLACEMENT NON-INTERACTIF ET AFFICHAGE
	if (player[0].alive)
	{
		// DEPLACEMENTS NON-INTERACTIF DU JOUEUR EN 3D (GLISSEMENT, FLOTTEMENT, PENCHEMENT)
		if (game_3d)
		{
			// 1) sliding
			player[0].movement *= 0.963; // 0.958
//			if ((1000/ms) > 85)
//				player[0].movement *= 0.958 * ((1000/ms)/85);
//			player[0].movement *= 1 - ms/11.79;
/*			if (player[0].movement < 0)
			{
				player[0].movement += 0.0002 * ms/10;
				if (player[0].movement > 0)
					player[0].movement = 0;
			}
			else if (player[0].movement > 0)
			{
				player[0].movement -= 0.0002 * ms/10;
				if (player[0].movement < 0)
					player[0].movement = 0;
			}*/
			player[0].x += player[0].movement * 1.05 * ms;//speed of player
			//player[0].x += player[0].movement;

			// 2) floating
			player[0].zi += player[0].zi_speed * ms / 15;
			m.pos.z = cosf(player[0].zi * 5) / 10 - 0.05;	// cosf(a * b) / c - d : a=de -3.14 à +3.14, b=vitesse, c=amplitude, d=ajustement vertical

			// 3) inclination
			m.rot.z = - player[0].movement * 11000;//speed of inclination
			if (m.rot.z > 50) m.rot.z = 50; else if (m.rot.z < -50) m.rot.z = -50; // limits of inclination : [-50,+50]

			// player lasers (bonuses)
			if (player[0].laser_left || player[0].laser_right)
			{
				m3.pos.z = m.pos.z / 1.5;	// floating
				m3.rot.z = m.rot.z / 1.7;	// inclination
			}
		}

		// --------------
		display_player();
		display_ring_2d_to_3d();
		// --------------
	}


	// LUMIERE DES SHIPS (DEBUT) ET PLAYER (FIN)
	if (game_3d)
	{
		glEnable(GL_LIGHTING);	// active les lumières
		glDisable(GL_LIGHT0);	// éteint la lumière player
		glEnable(GL_LIGHT1);	// allume la lumière ships
		//glEnable(GL_LIGHT2);	// allume la lumière ships
		glDisable(GL_BLEND);	// pas de blending pour les ships
	}

	// SHIPS -> DÉPLACEMENTS ET AFFICHAGE
	if (ships_move < 0)
		//	ships_move = - 0.001 * ms;
		//else ships_move =  0.001 * ms;
		ships_move = -  sec_frame * ships_speed;
	else ships_move =   sec_frame * ships_speed;

	ships_balancier += ships_move;					// sert à savoir quand on change de sens

	if (game_antialiasing_active && game_3d && !game_3d_lines) // Anti-aliasing (DEBUT)
		glEnable(GL_MULTISAMPLE_ARB);

	for (int i=0; i<=49; i++)						// pour chaque ship...
	{
		if (ship[i].alive)							// est-t'il vivant ?
		{
			ship[i].x += ships_move;				// si oui il se déplace
			// -------------
			display_ship(i);
			// -------------
		}
	}

	if (game_antialiasing_active && game_3d && !game_3d_lines) // Anti-aliasing (FIN)
		glDisable(GL_MULTISAMPLE_ARB);


	// SUPERSHIP -> DÉPLACEMENT ET AFFICHAGE
	if (supership[0].alive)
	{
		supership[0].x += supership[0].speed * 0.001 * ms10;
		if (supership[0].x < -3)									// changement de sens
		{
			supership[0].speed *= -1;
			supership[0].x += supership[0].speed * 0.001 * ms10;
		}
		else if (supership[0].x > 3)								// changement de sens
		{
			supership[0].speed *= -1;
			supership[0].x += supership[0].speed * 0.001 * ms10;
		}

		// -----------------
		if (game_antialiasing_active && game_3d && !game_3d_lines) // Anti-aliasing (DEBUT)
			glEnable(GL_MULTISAMPLE_ARB);

		display_supership();

		if (game_antialiasing_active && game_3d && !game_3d_lines) // Anti-aliasing (FIN)
			glDisable(GL_MULTISAMPLE_ARB);
		// -----------------
	}

	// SPECTRUM 3D (ANCIEN)
	//if (game_spectrum)
	//	display_spectrum();

	// LUMIERE DES SHIPS (FIN)
	if (game_3d)
	{
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT2);
		glDisable(GL_LIGHTING);
	}

	// 3D WRAPPER (FIN)
	if (game_3d)
	{
		glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
		glDisable(GL_DEPTH_TEST);		// désactive le test de profondeur
		glEnable(GL_BLEND);				// active le mélange de couleurs (ici transparence)
		glDisable(GL_CULL_FACE);		// on affiche les faces "cachées" (le dos des éléments en mode 2D)
	}

	if (game_spectrum)
		// -----------------
		display_equalizer();
		// -----------------


	/*___________________________________________________________________
	|                                                                    |
	|                       AFFICHAGE DES TEXTES                         |
	|____________________________________________________________________| 
	*/
	int text_align_left, text_align_left_up, text_align_right, text_align_right_up;
	int text_align_right_equalizer, text_align_up_equalizer;
	
	if (window->init.width >= 550 && game_2d_text)				// on n'affiche les textes que si la résolution >= 550 et l'option game_2d_text = true
	{
		display_text_frames();
		display_energy();

		// variables pour le placement correct des textes (en fonction de la résolution)
		if (window->init.width >= 1600)
		{
			fontsize = 15;
			fontspace = 17;
			text_align_left = 45;
			text_align_left_up = -40;
			text_align_right = -132;
			text_align_right_up = -52;
			
			text_align_right_equalizer = -287;
			text_align_up_equalizer = -390;
		}
		else if (window->init.width >= 1280)
		{
			fontsize = 12;
			fontspace = 14;
			text_align_left = 35;
			text_align_left_up = -31;
			text_align_right = -72;
			text_align_right_equalizer = -72;
			text_align_right_up = -39;

			text_align_right_equalizer = -227;
			text_align_up_equalizer = -314;
		}
		else if (window->init.width >= 1024)
		{
			fontsize = 10;
			fontspace = 12;
			text_align_left = 25;
			text_align_left_up = -23;
			text_align_right = -30;
			text_align_right_equalizer = -30;
			text_align_right_up = -28;

			text_align_right_equalizer = -185;
			text_align_up_equalizer = -250;
		}
		else if (window->init.width >= 800)
		{
			fontsize = 8;
			fontspace = 10;
			text_align_left = 18;
			text_align_left_up = -18;
			text_align_right = 10;
			text_align_right_up = -20;

			text_align_right_equalizer = -143;
			text_align_up_equalizer = -195;
		}
		else
		{
			fontsize = 6;
			fontspace = 8;
			text_align_left = 15;
			text_align_left_up = -14;
			text_align_right = 40;
			text_align_right_up = -16;

			text_align_right_equalizer = -110;
			text_align_up_equalizer = -157;
		}

		// EN HAUT A GAUCHE
		fontSize (fontsize); fontColor();fontDrawString (5 + text_align_left, window->init.height - fontspace*2 + text_align_left_up-1, FSOUND_GetCPUUsage()<10 ? game_lang == 2 ? "sonido=  %2.2f%%" :"audio =  %2.2f%%" : game_lang == 2 ? "sonido= %2.2f%%" :"audio = %2.2f%%",  FSOUND_GetCPUUsage());
		slowdown_fps += 0.02 * ms10;	if (slowdown_fps > 1) slowdown_fps = 0;	if (slowdown_fps == 0) my_fps=fps;
		fontSize (fontsize); fontColor(); fontDrawString (5 + text_align_left, window->init.height - fontspace*3 + text_align_left_up-2, "fps   = %2.2f%", my_fps);

		fontSize (fontsize); fontColor (0.952f, 0.937f, 0.804f); fontColor();
		fontDrawString (5 + text_align_left, window->init.height - fontspace*5.3 + text_align_left_up, "angle_x = %2.4f", angle_x - pcamera.x + ciblex); // %2.0f
		fontSize (fontsize); fontColor (0.952f, 0.937f, 0.804f); fontColor();
		fontDrawString (5 + text_align_left, window->init.height - fontspace*6.3 + text_align_left_up, "angle_y = %2.4f", angle_y - pcamera.y + cibley);
		fontSize (fontsize); fontColor (0.952f, 0.937f, 0.804f); fontColor();
		fontDrawString (5 + text_align_left, window->init.height - fontspace*7.3 + text_align_left_up, "zoom    = %2.4f", zoom_total);
		fontSize (fontsize); fontColor (0.952f, 0.937f, 0.804f); fontColor();
		fontDrawString (5 + text_align_left, window->init.height - fontspace*8.3 + text_align_left_up, "backgrd = %d",game_level);
		fontSize (fontsize); fontColor (0.952f, 0.937f, 0.804f); fontColor();
		fontDrawString (5 + text_align_left, window->init.height - fontspace*9.3 + text_align_left_up, "inter   = %3.2f", intermede_angle);

		#ifdef DEBUG_FPS
		fp10s_count++;
		if ((TimerGetTime() - fp10s_timer) > 10000)
		{
			fp10s_timer = TimerGetTime();
			fp10s_nb    = fp10s_count;
			fp10s_count = 0;
		}
		fontSize (fontsize); fontColor (0.952f, 0.937f, 0.804f); fontColor();
		fontDrawString (5 + text_align_left, window->init.height - fontspace*10.3 + text_align_left_up, "fp10s   = %d", fp10s_nb);
		#endif
	
		#ifdef DEBUG_PASSAGE_3D
		fontSize (fontsize); fontColor (1,1,1); fontColor();
		fontDrawString (5 + text_align_left, window->init.height - fontspace*12 + text_align_left_up, " tir index : %d", id_tir);
		#endif

		#ifdef DEBUG
		fontDrawString (5 + text_align_left, window->init.height - fontspace*10 + text_align_left_up, "inter angle total = %2.0f", intermede_angle_total);
		fontDrawString (5 + text_align_left, window->init.height - fontspace*11 + text_align_left_up, "inter angl        = %2.2f", intermede_angle);
		fontDrawString (5 + text_align_left, window->init.height - fontspace*12 + text_align_left_up, "frame_time        = %d", timeGetTime());
		fontDrawString (5 + text_align_left, window->init.height - fontspace*13 + text_align_left_up, "frame_oldtime     = %d", frame_oldtime );
		fontDrawString (5 + text_align_left, window->init.height - fontspace*14 + text_align_left_up, "frame_start       = %d", frame_start);
		fontDrawString (5 + text_align_left, window->init.height - fontspace*15 + text_align_left_up, "diff              = %d", timeGetTime() - frame_oldtime);
		#endif

		// EN HAUT A DROITE
		//if (game_lang == 1) text_align_right -= 3;
		//else if (game_lang == 2) text_align_right -= 9;
		fontSize (fontsize); fontColor(); fontDrawString (window->init.width - 155 + text_align_right, window->init.height - fontspace + text_align_right_up, game_lang == 0 ? "ESC Quit" : game_lang == 1 ? "ESC Quitter" : "Salir ESC");
		fontSize (fontsize); fontColor(); fontDrawString (window->init.width - 155 + text_align_right, window->init.height - fontspace*2 + text_align_right_up, game_lang == 0 ? "MOUSE Rotation" : game_lang == 1 ? "Souris Rotation" : "Rotacion RATON");
		fontSize (fontsize); fontColor(); fontDrawString (window->init.width - 155 + text_align_right, window->init.height - fontspace*3 + text_align_right_up, game_lang == 0 ? music_on ? "M \r Music (on)" : "M \r Music (off)" : game_lang == 1 ? music_on ? "M \r Musique(on)" : "M \r Musique(off)" : music_on ? "M \r Music (si)" : "M \r Music (no)");
		fontSize (fontsize); fontColor(); fontDrawString (window->init.width - 155 + text_align_right, window->init.height - fontspace*4 + text_align_right_up, game_lang == 0 ? "B \r Background" : game_lang == 1 ? "B \r Fond" : "B \r Fondo");
		fontSize (fontsize); fontColor(); fontDrawString (window->init.width - 155 + text_align_right, window->init.height - fontspace*5 + text_align_right_up, game_lang == 0 ? "R \r Replace cam" : game_lang == 1 ? "R \r Recentre cam" : "R \r Reemplazo cam"); // Reemplazo camara
		fontSize (fontsize); fontColor(); fontDrawString (window->init.width - 155 + text_align_right, window->init.height - fontspace*6 + text_align_right_up, game_lang == 0 ? "T \r Hide Text" : game_lang == 1 ? "T \r Cache Texte" : "T \r Ocultar Texto");
		fontSize (fontsize); fontColor(); fontDrawString (window->init.width - 155 + text_align_right, window->init.height - fontspace*7 + text_align_right_up, "+/- Zoom");
		if (game_spectrum)
		{
			fontSize (fontsize-1); fontColor (0.952f, 0.937f, 0.804f); fontColor();
			fontDrawString (equalizer_x + window->init.width + text_align_right_equalizer, equalizer_y + window->init.height + text_align_up_equalizer, "  S \r Spectrum");
		}

		// EN BAS A GAUCHE
		fontSize (fontsize); fontColor(); fontDrawString (5 + text_align_left, fontspace*1*1.3, game_lang == 0 ? "Height = %d" : game_lang == 1 ? "Hauteur = %d" : "Alto  = %d", window->init.height);
		fontSize (fontsize); fontColor(); fontDrawString (5 + text_align_left, fontspace*2*1.3, game_lang == 0 ? "Width  = %d" : game_lang == 1 ? "Largeur = %d" : "Ancho = %d", window->init.width);
		fontSize (fontsize); fontColor();
		if (game_lang == 0)
			fontDrawString (window->init.width/2 - (35*fontsize)/2, 5, "< Press space key to change music >");
		else if (game_lang == 1)
			fontDrawString (window->init.width/2 - (46*fontsize)/2, 5, "< Appuyer sur espace pour changer la musique >");
		else if (game_lang == 2)
			fontDrawString (window->init.width/2 - (56*fontsize)/2, 5, "< Presione la barra espaciadora para cambiar la musica >");
		//fontDrawString (window->init.width/2 - (35*fontsize)/2, 35, "t=%f zoom_total_bonus=%f", t, zoom_total_bonus);
		//fontDrawString (window->init.width/2 - (35*fontsize)/2, 25, "ciblex=%f cibley=%f", ciblex, cibley);
		//fontDrawString (window->init.width/2 - (35*fontsize)/2, 15, "pcamera.x=%f pcamera.y=%f pcamera.z=%f",pcamera.x,pcamera.y,pcamera.z);
		//fontDrawString (window->init.width/2-(9*fontsize)/2, (fontspace+1)*4, "var = %i ", bonus[1].explosion);
				

		// MESSAGE UTILISATEUR (AU CENTRE EN BAS)
		if (strlen(game_message) > 0)
		{
			static float game_message_time = 0;
			game_message_time += (3 * ms10) / 40 ;
			if (game_message_time < 250 && (window->init.width == 640 || window->init.width == 800 || window->init.width == 1024 || window->init.width == 1280 || window->init.width == 1600))
			{
				if (game_lang == 2) // game_message smaller for spanish language
				{
					fontSize (fontsize+1);
					fontDrawString (window->init.width/2 - (strlen(game_message)*(fontsize+1))/2, (fontspace+1) * 2, "%s", game_message );
				}
				else
				{
					fontSize (fontsize+2);
					fontDrawString (window->init.width/2 - (strlen(game_message)*(fontsize+2))/2, (fontspace+1) * 2, "%s", game_message );
				}
			}
			else
			{
				game_message_time = 0;
				strcpy(game_message,"");
			}
		}
	}



	/*___________________________________________________________________
	|                                                                    |
	|          L'INTERMEDE (LA CAMERA TOURNE AUTOUR DE LA SCENE)         |
	|____________________________________________________________________| 
	*/

	if (intermede && !intermede_fin)
	{
		// le texte clignote
		blink_intermede += (3*ms10) /40;

		if (blink_intermede > 90)
		{
			blink_intermede = 0;
			blink_intermede_nb++;
		}

		// affichage du texte "YOU WIN", "YOU LOSE" ou "GAME OVER", 3 fois maximum
		if ((blink_intermede < 60) && (blink_intermede_nb < 3))
		{
			if (partie_gagnee == 1)
			{
				fontSize ((int) window->init.width / 25);fontColor (0.0f, 0.0f, 0.0f);
				fontDrawString ((int)(window->init.width * .34), (int)(window->init.height / 2) -2, "YOU WIN !");
				fontSize ((int) window->init.width / 25);fontColor (1.0f, 1.0f, 1.0f);
				fontDrawString ((int)(window->init.width * .34), (int)(window->init.height / 2),    "YOU WIN !"); // pour l'effet d'ombre
			}
			else if (partie_gagnee == -1)
			{
				fontSize ((int) window->init.width / 25);fontColor (0.0f, 0.0f, 0.0f);
				fontDrawString ((int)(window->init.width * .34), (int)(window->init.height / 2) -2, "GAME OVER !");
				fontSize ((int) window->init.width / 25);fontColor (1.0f, 0.2f, 0.2f);
				fontDrawString ((int)(window->init.width * .34), (int)(window->init.height / 2)   , "GAME OVER !"); // pour l'effet d'ombre
			}
			else
			{
				fontSize ((int) window->init.width / 25);fontColor (0.0f, 0.0f, 0.0f);
				fontDrawString ((int)(window->init.width * .34), (int)(window->init.height / 2) -2, "YOU LOSE !");
				fontSize ((int) window->init.width / 25);fontColor (1.0f, 0.2f, 0.2f);
				fontDrawString ((int)(window->init.width * .34), (int)(window->init.height / 2),    "YOU LOSE !"); // pour l'effet d'ombre
			}
		}

		//
		// ANIMATION DE L'INTERMEDE (coef en fonction du nombre de fps : (3 * ms10) / 40)
		//

		// on incrémente ou décrémente l'angle de l'animation (on élève à la puissance 3 => bidouillage)
		/*
		if (intermede_angle_moitie == false)
		intermede_angle += sqrt(timeGetTime() - frame_oldtime) * 0.001;
		else
		intermede_angle -= sqrt(timeGetTime() - frame_oldtime) * 0.001;
		*/
		//fpsavr
		/*		// ESSAI AVEC METHODE FPS N°2		
		if (GetSysTimeSeconds() - frame_oldtime >= 0.005)
		{
		if (intermede_angle_moitie == false)
		intermede_acceleration += 1 * sec_frame;
		else
		intermede_acceleration -= 1 * sec_frame;

		//else
		//	intermede_angle -= 10 * ((1 * ms10) / 40);

		frame_oldtime = GetSysTimeSeconds();

		// cas particulier (évite un bug qui fait passer l'angle en négatif)
		if ((intermede_acceleration <= 0.2) && (intermede_angle > 180))
		intermede_acceleration = 0.2;

		intermede_angle += intermede_acceleration;
		}



		// rotation de la scène (animation de l'intermède)
		// ROTATIONS DE LA SCENE (SOURIS)
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(	0.0, 0.0, 20.0,							// l'oeil
		0.0, 0.0,  0.0,							// le centre
		0.0, 1.0,  0.0	);						// le haut
		glRotatef(angle_x, 1.0, 0.0, 0.0);						// souris axe y
		glRotatef(angle_y, 0.0, 1.0, 0.0);						// souris axe x
		glTranslatef ( 0.0, 0.0, zoom_total );
		glRotatef(intermede_angle, -0.3f, 0.5f, 1.0f);

		// on met à jour l'angle total (sert pour le test au dessous)
		//		intermede_angle_total += intermede_angle;

		// si on dépasse la moitié on le spécifie dans une variable (sert aussi pour le test au dessus)
		if (intermede_angle >= 180)
		intermede_angle_moitie = true;
		*/

//		if (timeGetTime() - frame_oldtime >= 2) //25 //2
//		{ 
			if (intermede_angle_moitie == false) 
				intermede_angle += 0.006 * ((3 * ms) / 40);				// 0.1
			else
				intermede_angle -= 0.006 * ((3 * ms) / 40);

			// cas particulier (évite un bug qui fait passer de temps en temps l'angle en négatif) 
			if ((intermede_angle <= 0.2) && (intermede_angle_total > 180)) 
				intermede_angle = 0.2;

//			frame_oldtime = timeGetTime();
//		}

		// accélération / décélération
		if (intermede_angle_total < 180)
			intermede_angle *= 1 + (ms/1000);
		else
			intermede_angle *= 1 - (ms/1000);

		// on calcule l'angle total effectif (celui-ci est utilisé lors des opérations
		// sur la matrice MODELVIEW, en début de fonction)
		intermede_angle_total += intermede_angle;

		if (intermede_angle_total >= 180) 
			intermede_angle_moitie = true; 

		//
		// APRES L'INTERMEDE ON RÉINITIALISE LES DIVERS ELEMENTS DU JEU
		//

		if (intermede_angle_total >= 360 && intermede_angle_moitie)
		{
			//glRotatef(-intermede_angle_total, -0.3f, 0.5f, 1.0f);	// on revient précisemment à la position avant l'intermède
			glRotatef(-intermede_angle_total, -0.3f, 0.55f, 1.0f);	// on revient précisemment à la position avant l'intermède

			// SI UNE VIE PERDUE...
			if (vie_perdue)						// test indépendant des 2 ci-dessous car le joueur peut perdre une vie et juste après ou avant gagner la partie avec les tirs actifs restant dans la scène
			{
				init_player();					// initialisation du joueur
				init_shoots();					// initialisation des tirs
				shoot_type			= 0;		// tir de base
				lives--;						// on perd une vie
				ships_move			= -1;		// déplacement des ships
				ships_speed			= 1;		// vitesse des ships
				ships_acc_shoots	= 0;		// fréquence des tirs
				game_level			= 1;		// retour au niveau 1
				game_3d				= false;	// on recommence le jeu en game 2D
				game_3d_lines		= false;
				ReshapeGL(window->init.width, window->init.height);
				if (lives < 1)
					partie_gagnee = -1;
			}

			// SI PARTIE GAGNEE...
			if (partie_gagnee == 1)
			{
				partie_gagnee		= 0;		// partie en cours
				ships_move			= -1;		// déplacement des ships
				ships_speed			*= 1.05;	// vitesse des ships				
				
				if (ships_acc_shoots < 5)
					ships_acc_shoots += 0.2;	// fréquence des tirs
				
				if (bonus[1].explosion_life >= 2.0 || !bonus[1].explosion_life)
				{
					if (game_level++ == 5)		// niveau suivant
						game_level = 1;

					if (!game_3d_lines)			// mode lines ou non
						game_3d_lines = true;
					else
						game_3d_lines = false;
				}
			}

			// SI PARTIE PERDUE...
			else if (partie_gagnee == -1)
			{
				init_player();					// initialisation du joueur
				init_shoots();					// initialisation des tirs
				shoot_type			= 0;		// tir de base
				partie_gagnee		= 0;		// partie en cours
				hiscore = get_hiscore();		// récupère le hiscore
				lives				= 3;		// on remet 3 vies
				score				= 0;		// réinitialise le score
				ships_move			= -1;		// déplacement des ships
				ships_speed			= 1;		// vitesse des ships
				ships_acc_shoots	= 0;		// fréquence des tirs
				game_level			= 1;		// retour au niveau 1
				game_3d				= false;	// on recommence le jeu en game 2D
				game_3d_lines		= false;	// pour le mode 3D, le mode lines est off
				ReshapeGL(window->init.width, window->init.height);
			}


			// DANS TOUS LES CAS...

			// ENREGISTREMENT DU SCORE SI + GRAND QUE LE HISCORE
			set_hiscore(score);

			// REINITIALISATION DES VARIABLES GLOBALES ET DES ELEMENTS DU JEU
			vie_perdue				= false;	// on n'est pas en train de perdre une vie
			intermede				= false;	// on n'est plus dans l'intermede
			intermede_fin			= true;		// la fin de l'intermede est passée
			intermede_angle			= 0;		// l'angle de l'intermede est remis à zéro
			intermede_angle_total	= 0;		// l'angle total de l'intermede est remis à zéro
			intermede_angle_moitie	= false;	// la moitié de l'animation n'est pas passée
			blink_intermede			= 0;		// pour le texte de l'intermede
			blink_intermede_nb		= 0;		// pour le texte de l'intermede
			intermede_sound			= false;	// la musique de l'intermede est finie

			ships_balancier			= 5;		// les ships démarrent en haut à droite
			ships_vers_la_gauche	= true;		// le sens des ships est vers la gauche
			init_ships();						// initialisation des vaisseaux
			init_supership();					// initialisation du supership
			id_brick				= 0;		// l'index des briques des bunkers est remis à zéro

			#ifdef DEBUG_BUNKERS
			init_bunker(-5.0f, -4.5f, 0.05f);	// initialisation du premier bunker
			init_bunker(-2.0f, -4.5f, 0.05f);	// initialisation du deuxième bunker
			init_bunker( 1.0f, -4.5f, 0.05f);	// initialisation du troisième bunker
			init_bunker( 4.0f, -4.5f, 0.05f);	// initialisation du quatrième bunker
			#endif

			active_splash			= true;		// active le splash screen avant de commencer la nouvelle partie
		}
	}

	// MUSIC TITLE SCROLLING (DANS UN NOUVEAU VIEWPORT)
	if (window->init.width >= 550 && game_2d_text)
	{
		fontSize (fontsize); fontColor(); fontDrawString (8 + text_align_left, window->init.height - fontspace + text_align_left_up, "\r");
	}
	unsigned int char_max = 25;
	if  (
		window->init.width >= 550 && game_2d_text
		&& (strstr(music[id_my_music].fichier, ".mp3") != NULL
			|| (game_option_stream && id_my_music == 1 && strlen(stream_name) > char_max )
			|| (id_my_music == 0 && strlen(music_affichee) > char_max ))
		)
	{
		glViewport(7 + ceil(1.5f*fontsize) + text_align_left, 0, char_max * fontsize, g_window->init.width);
		unsigned int taille_titre = strlen(music_affichee);

		if (taille_titre > char_max)
		{
			if (taille_titre < (char_max +1 + scroll_music_total))
			{
				scroll_music_sens *= -1;
				scroll_music_total = 0;
			}

			scroll_music_total += (0.02 * ms / fontsize);

			if (scroll_music_total > 4.0 && scroll_music_total < fontsize * (taille_titre - 2))
				scroll_music_x -= scroll_music_sens * 0.02 * ms;
		}
		fontSize (fontsize); fontColor(); fontDrawString (ceil(scroll_music_x), window->init.height - fontspace + text_align_left_up, "%s", music_affichee);
	}
	else if (window->init.width >= 550 && game_2d_text) // si <= char_max caractères => pas de défilement
	{
		fontSize (fontsize); fontColor(); fontDrawString (8 + fontsize*0.4 + 0.5 + text_align_left, window->init.height - fontspace + text_align_left_up, " %s", music_affichee); // + 0.5 sert à faire l'arrondi
	}

	// SMALL SCREEN (DANS UN NOUVEAU VIEWPORT)
	display_console_mini_screen();						// affiche le mini-écran si la console est activée

	glFlush ();									// Flush The GL Rendering Pipeline
}


/*___________________________________________________________________
|                                                                    |
|            G A M E   L O G I C  (C O L L I S I O N S, ...)         |
|____________________________________________________________________| 
*/

//////////////////////////////////////////////////////////////////////////////////////
//
// Fonction GAMELOGIC (gère le déroulement du jeu)
//
//
void GameLogic (GL_Window* window)
{

	//
	// si la musique est terminée on passe à la suivante
	//
	static bool music_next = false;
	if (music_next)
	{
		window->keys->keyDown[VK_SPACE] = FALSE;
		music_next = false;
	}

	try
	{
	if (!music_next)
		if ( (strstr(music[id_my_music].fichier, "1 000 000 miles by ronny / teklords") != NULL && FMUSIC_IsFinished(my_mod) )
			|| (strstr(music[id_my_music].fichier, ".mp3") != NULL && !FSOUND_IsPlaying(int_sound_mp3)) )
			{
				// on lance la prochaine musique en simulant un appui sur la barre d'espace
				window->keys->keyDown[VK_SPACE] = TRUE;
				music_next = true;
			}
	}
	catch(...)
	{
		ERR("erreur dans le chgt de zik");
	}


		/*___________________________________________________________________
		|                                                                    |
		|         GESTION DES DEPLACEMENTS, DES TIRS, DES COLLISIONS         |
		|____________________________________________________________________| 
		*/


		// ANIMATION DES SHIPS (TEXTURE 1 OU 2)
		ship_anim += ms10/10;
		if (ship_anim < 100)							// pendant 100 frames on affiche 1 sprite
			ships_choix_anim = 0;
		else											// le reste du temps le 2ème sprite
			ships_choix_anim = 10;
		if (ship_anim > 200)
			ship_anim = 0;

		// GESTION DES SHIPS

		for (int i=0; i<=49; i++)						// pour chaque ship...
		{
			if (ship[i].alive)							// est-t'il vivant ?
			{
				// LE SHIP TOUCHE UN BORD ? SI OUI TOUS LES SHIPS DESCENDENT D'UN CRAN
				if ( ((ships_balancier < 0) && (ships_vers_la_gauche)) || ((ships_balancier > 5) && (!ships_vers_la_gauche)) )
				{
					if (!game_3d)
					{
						int_sound = FSOUND_PlaySoundEx(FSOUND_FREE,sound_descente, dspUnitSounds, false);
						FSOUND_SetVolume(int_sound,160);
					}
					if (ships_balancier < 0)
						ships_vers_la_gauche = false;
					else
						ships_vers_la_gauche = true;	// on change de sens
					if (fabs(ships_move) < 0.02) ships_move *= -1.05f; else ships_move *= -1.0f;			// limite de vitesse (fabs = valeur absolue d'un float)
					for (int j=0;j<=49;j++)				// et pour tous les vaisseaux...
						ship[j].y -= .3f;				// on descend d'un cran
				}

				// LE SHIP TIRE OU PAS (ALEATOIRE)
				if (( floor((rand()%100000/(1+ships_acc_shoots))/ms10)) == 1)
				{
					tir[id_tir].type = 10;
					tir[id_tir].active = true;
					tir[id_tir].tail = false;
					tir[id_tir].x = ship[i].x;
					tir[id_tir].y = ship[i].y - .3f;
					tir[id_tir].z = .05f;
					
					id_tir++;
					if (id_tir > max_tirs)						// x tirs au maximum en même temps
						id_tir = 0;								// remise à zéro
				}

				// LE SHIP TOUCHE LE JOUEUR (COLLISION DES VAISSEAUX)
				g1 = player[0].x - player[0].size_x;
				d1 = player[0].x + player[0].size_x;
				h1 = player[0].y + player[0].size_y;
				b1 = player[0].y - player[0].size_y;

				g2 = ship[i].x - ship[i].size_x;
				d2 = ship[i].x + ship[i].size_x;
				h2 = ship[i].y + ship[i].size_y;
				b2 = ship[i].y - ship[i].size_y;

				if (player[0].alive)
					if (collision(g1, d1, h1, b1, g2, d2, h2, b2) && !game_immortal)
					{
						#ifdef DEBUG_MORTAL
						int_sound = FSOUND_PlaySoundEx(FSOUND_FREE,sound_touche, dspUnitSounds, false);
						FSOUND_SetCurrentPosition(int_sound, 2000);
						FSOUND_SetVolume(int_sound,180);
						exploded(i);
						if (bonus[1].explosion)
							break;									// le joueur est protégé pendant le passage 3D
						vie_perdue = true;
						player[0].energy = 0;
						if (player[0].laser_left)
							exploded(50, player[0].x-0.9, player[0].y, player[0].z);
						if (player[0].laser_right)
							exploded(51, player[0].x+0.9, player[0].y, player[0].z);
						exploded(52, player[0].x, player[0].y, player[0].z);
						#endif
					}

					// LE SHIP ARRIVE EN BAS
					if (ship[i].y < - 6.5)									// s'il dépasse la ligne verte en bas...
						vie_perdue = true;									// ...le joueur a perdu
			}
		}


		//
		// GESTION DES TIRS (du player et des ships)
		//

		for (int k=0; k <= max_tirs; k++)								// pour tous les tirs
		{
			if (tir[k].active)											// si il est actif...
			{
				if (tir[k].type == 0 || (game_3d && tir[k].type < 10))	// si le tir est du player
				{
					if (tir[k].y > 17)									// si le tir sort de l'écran en haut...
					{
						tir[k].active = false;							// ...il devient inactif
						if (tir[k].type != 2)
							tir[k].tail = false;
					}

					// LE TIR DU JOUEUR TOUCHE UNE BRIQUE
					if (tir[k].y < -3.00)								// on ne regarde qu'au niveau des bunkers (optimisation)
					{
						for (int i=0; i<= id_brick -1; i++)				// pour toutes les briques...
						{
							if (brick[i].active)
							{
								g1 = brick[i].x - .05;
								d1 = brick[i].x + .05;
								h1 = brick[i].y + .3;
								b1 = brick[i].y - .3;
								
								if (!game_3d)
								{
									g2 = tir[k].x - .05;
									d2 = tir[k].x + .05;
									h2 = tir[k].y - .1;
									b2 = tir[k].y - .7;
								}
								else
								{
									if (tir[k].type == 2)	// tir 3D joueur particules
									{
										g2 = tir[k].x - .05 - .05;
										d2 = tir[k].x + .05 + .05;
										h2 = tir[k].y - .25;//- .4  - .0;
										b2 = tir[k].y - .5;//- 1.0 - .0;
									}
									else if (tir[k].type == 0 || tir[k].type == 1) // tir de base
									{
										g2 = tir[k].x - .05 - .05;
										d2 = tir[k].x + .05 + .05;
										h2 = tir[k].y - .5;
										b2 = tir[k].y - .75;
									}
									else if (tir[k].type == 5) // tir de laser (mini ship)
									{
										g2 = tir[k].x - .03;
										d2 = tir[k].x + .03;
										h2 = tir[k].y - .25;
										b2 = tir[k].y - .5;
									}
								}

								if (collision(g1, d1, h1, b1, g2, d2, h2, b2))
								{
									int_sound = FSOUND_PlaySoundEx(FSOUND_FREE,sound_touche, dspUnitSounds, false);
									FSOUND_SetCurrentPosition(int_sound, 2000);
									FSOUND_SetVolume(int_sound,12);//25
									tir[k].active = false;
									if (tir[k].type != 2)
										tir[k].tail = false;
									brick[i].active = false;
								}
							}
						}
					}


					// LE TIR DU JOUEUR TOUCHE UN SHIP
					for (int i=0; i<= 49; i++)
					{
						if (ship[i].alive)
						{
							g1 = ship[i].x - ship[i].size_x;
							d1 = ship[i].x + ship[i].size_x;
							h1 = ship[i].y + ship[i].size_y;
							b1 = ship[i].y - ship[i].size_y;

							if (!game_3d)
							{
								g2 = tir[k].x - .05;
								d2 = tir[k].x + .05;
								h2 = tir[k].y + .3;
								b2 = tir[k].y - .3;
							}
							else
							{
								if (tir[k].type < 5) // tir player non laser
								{
									g2 = tir[k].x - .05 - .1;
									d2 = tir[k].x + .05 + .1;
									h2 = tir[k].y + .3  - .2;
									b2 = tir[k].y - .3  - .8;
								}
								else if (tir[k].type == 5) // tir de laser (mini ship)
								{
									g2 = tir[k].x - .03;
									d2 = tir[k].x + .03;
									h2 = tir[k].y - .25;
									b2 = tir[k].y - .5;
								}

							}

							if (collision(g1, d1, h1, b1, g2, d2, h2, b2))
							{
								if (!int_sound_explosion)
									int_sound_explosion = FSOUND_PlaySoundEx(FSOUND_FREE,sound_touche, dspUnitSounds, false);
								else
								{
									FSOUND_SetVolume(int_sound_explosion,0);	// évite à la fois l'addition des volumes et/ou le craquement
									int_sound_explosion = FSOUND_PlaySoundEx(FSOUND_FREE,sound_touche, dspUnitSounds, false);
								}
								
								if (!game_3d)
								{
									FSOUND_SetVolume(int_sound_explosion,120);//130
								}
								else
								{
									if (tir[k].type == 0)
										ship[i].health -= 4;
									else if (tir[k].type == 1)
										ship[i].health -= 5;
									else if (tir[k].type == 2)
										ship[i].health -= 10;
									else if (tir[k].type == 5)
										ship[i].health -= 3;

									ship[i].hit_duration--;
									FSOUND_SetVolume(int_sound_explosion,90);//118
									if (ship[i].health <= 0)
										ship[i].alive = false;
								}
								// le ship meurt
								if (!ship[i].alive || !game_3d)
								{
									exploded(i);						// le ship i meurt
									score += (4 - ship[i].type) * 10;
								}
								FSOUND_SetCurrentPosition(int_sound_explosion, 2100);
								tir[k].active = false;					// le tir k aussi
								if (tir[k].type != 2)
									tir[k].tail = false;
							}
						}
					}

					// LE TIR DU JOUEUR TOUCHE LE SUPERSHIP
					if (supership[0].alive)
					{
						g1 = supership[0].x - supership[0].size_x;
						d1 = supership[0].x + supership[0].size_x;
						h1 = supership[0].y + supership[0].size_y;
						b1 = supership[0].y - supership[0].size_y;

						if (!game_3d)
						{
							g2 = tir[k].x - .05;
							d2 = tir[k].x + .05;
							h2 = tir[k].y + .3;
							b2 = tir[k].y - .3;
						}
						else
						{
							if (tir[k].type < 5) // tir player
							{
								g2 = tir[k].x - .05 -.5;
								d2 = tir[k].x + .05 +.5;
								h2 = tir[k].y + .3;
								b2 = tir[k].y - .3;
							}
							else if (tir[k].type == 5) // tir de laser (mini ship)
							{
								g2 = tir[k].x - .03;
								d2 = tir[k].x + .03;
								h2 = tir[k].y - .25;
								b2 = tir[k].y - .5;
							}
						}

						if (collision(g1, d1, h1, b1, g2, d2, h2, b2)) // tir player sur supership
						{
							if (!int_sound_explosion)
								int_sound_explosion = FSOUND_PlaySoundEx(FSOUND_FREE,sound_touche, dspUnitSounds, false);
							else
							{
								FSOUND_SetVolume(int_sound_explosion,0);	// évite à la fois l'addition des volumes et/ou le craquement
								int_sound_explosion = FSOUND_PlaySoundEx(FSOUND_FREE,sound_touche, dspUnitSounds, false);
							}
							
							if (!game_3d)
							{
								FSOUND_SetVolume(int_sound_explosion,115);//115
							}
							else
							{
								if (tir[k].type == 0)
									supership[0].health -= 4;
								else if (tir[k].type == 1)
									supership[0].health -= 5;
								else if (tir[k].type == 2)
									supership[0].health -= 10;
								else if (tir[k].type == 5)
									supership[0].health -= 3;

								supership[0].hit_duration--;
								FSOUND_SetVolume(int_sound_explosion,115);
								if (supership[0].health <= 0)
									supership[0].alive = false;
							}
							// le supership meurt
							if (!supership[0].alive || !game_3d)
							{
								exploded(53, supership[0].x, supership[0].y, supership[0].z);
								score += (int)ceil((float)rand() / 100 );				// = de 1 à 32767/100 soit de 1 à 327
							}
							FSOUND_SetCurrentPosition(int_sound_explosion, 2100);
							tir[k].active = false;
							if (tir[k].type != 2)
								tir[k].tail = false;
						}
					}
				}

				// TIR ENEMI (SHIP)
				else if (tir[k].type == 10)									// si le tir est d'un ship
				{
					if (tir[k].y < -10)										// si le tir sort de l'écran en bas...
					{
						tir[k].active = false;								// ...il devient inactif
						tir[k].tail = false;
					}

					// LE TIR ENEMI TOUCHE LE JOUEUR
					if (!game_3d)
					{
						g1 = player[0].x - player[0].size_x;
						d1 = player[0].x + player[0].size_x;
						h1 = player[0].y + player[0].size_y;
						b1 = player[0].y - player[0].size_y;
					}
					else
					{
						g1 = player[0].x - player[0].size_x - 0.25 + m.rot.z/250; // m.rot.z (de -48.xx à 48.xx) correspond au penchement du joueur donc sa taille projetée en 2D est plus faible (la gestion de collision est en 2D)
						d1 = player[0].x + player[0].size_x + 0.25 - m.rot.z/250; // idem
						h1 = player[0].y + player[0].size_y + 0.4;
						b1 = player[0].y - player[0].size_y;
					}

					g2 = tir[k].x - .033;
					d2 = tir[k].x + .033;
					h2 = tir[k].y + .3;
					b2 = tir[k].y - .3;

					if (player[0].alive)
						if (collision(g1, d1, h1, b1, g2, d2, h2, b2) && !game_immortal)
						{
							#ifdef DEBUG_MORTAL
							tir[k].active = false;
							tir[k].tail = false;
							if (bonus[1].explosion)
								break;									// le joueur est protégé pendant le passage 3D

							int_sound = FSOUND_PlaySoundEx(FSOUND_FREE,sound_touche, dspUnitSounds, false);
							FSOUND_SetCurrentPosition(int_sound, 2000);
							FSOUND_SetVolume(int_sound,160);

							player[0].energy -= 3.0f;
							player[0].hit_duration--;
							if (player[0].energy <= 0)
							{
								FSOUND_SetVolume(int_sound,170);
								vie_perdue = true;
								if (player[0].laser_left)
									exploded(50, player[0].x-0.9, player[0].y, player[0].z);
								if (player[0].laser_right)
									exploded(51, player[0].x+0.9, player[0].y, player[0].z);
								exploded(52, player[0].x, player[0].y, player[0].z);
							}
							#endif
						}

					// LE TIR ENEMI TOUCHE LE LASER JOUEUR * DE GAUCHE *
					if (player[0].laser_left)
					{
						g1 = player[0].x - player[0].size_x -0.9 +0.10 + m3.rot.z/500; // m.rot.z (de -24.xx à 24.xx) correspond au penchement du laser joueur donc sa taille projetée en 2D est plus faible (la gestion de collision est en 2D)
						d1 = player[0].x + player[0].size_x -0.9 -0.10 - m3.rot.z/500; // idem
						h1 = player[0].y + player[0].size_y;
						b1 = player[0].y - player[0].size_y;

						g2 = tir[k].x - .033;
						d2 = tir[k].x + .033;
						h2 = tir[k].y + .3;
						b2 = tir[k].y - .3;

						if (collision(g1, d1, h1, b1, g2, d2, h2, b2) && !game_immortal)
						{
							#ifdef DEBUG_MORTAL
							int_sound = FSOUND_PlaySoundEx(FSOUND_FREE,sound_touche, dspUnitSounds, false);
							FSOUND_SetCurrentPosition(int_sound, 2000);
							FSOUND_SetVolume(int_sound,170);
							tir[k].active = false;
							tir[k].tail = false;
							if (bonus[1].explosion)
								break;		// protégé par l'anneau du bonus
							exploded(50, player[0].x-0.9, player[0].y, player[0].z);
							#endif
						}
					}

					// LE TIR ENEMI TOUCHE LE LASER JOUEUR * DE DROITE *
					if (player[0].laser_right)
					{
						g1 = player[0].x - player[0].size_x +0.9 +0.10 + m3.rot.z/500; // m.rot.z (de -24.xx à 24.xx) correspond au penchement du laser joueur donc sa taille projetée en 2D est plus faible (la gestion de collision est en 2D)
						d1 = player[0].x + player[0].size_x +0.9 -0.10 - m3.rot.z/500; // idem
						h1 = player[0].y + player[0].size_y;
						b1 = player[0].y - player[0].size_y;

						g2 = tir[k].x - .033;
						d2 = tir[k].x + .033;
						h2 = tir[k].y + .3;
						b2 = tir[k].y - .3;

						if (collision(g1, d1, h1, b1, g2, d2, h2, b2) && !game_immortal)
						{
							#ifdef DEBUG_MORTAL
							int_sound = FSOUND_PlaySoundEx(FSOUND_FREE,sound_touche, dspUnitSounds, false);
							FSOUND_SetCurrentPosition(int_sound, 2000);
							FSOUND_SetVolume(int_sound,170);
							tir[k].active = false;
							tir[k].tail = false;
							if (bonus[1].explosion)
								break;		// le joueur est protégé par l'anneau du bonus
							exploded(51, player[0].x+0.9, player[0].y, player[0].z);
							#endif
						}
					}

					if (tir[k].y < -3.00)									// au niveau des bunkers (optimisation)
					{
						for (int i=0; i<= id_brick -1; i++)					// pour toutes les briques...
						{
							if (brick[i].active)
							{
								// LE TIR ENEMI TOUCHE UNE BRIQUE D'UN BUNKER
								g1 = brick[i].x - .05;
								d1 = brick[i].x + .05;
								h1 = brick[i].y + .05;
								b1 = brick[i].y - .05;

								g2 = tir[k].x - .033;
								d2 = tir[k].x + .033;
								h2 = tir[k].y + .3;
								b2 = tir[k].y - .3;

								if (collision(g1, d1, h1, b1, g2, d2, h2, b2))
								{
									if (!int_sound_explosion)
										int_sound_explosion = FSOUND_PlaySoundEx(FSOUND_FREE,sound_touche, dspUnitSounds, false);
									else
									{
										FSOUND_SetVolume(int_sound_explosion,0);	// évite à la fois l'addition des volumes et/ou le craquement
										int_sound_explosion = FSOUND_PlaySoundEx(FSOUND_FREE,sound_touche, dspUnitSounds, false);
									}
									FSOUND_SetVolume(int_sound_explosion,50);//60
									FSOUND_SetCurrentPosition(int_sound_explosion, 2000);

									tir[k].active = false;
									tir[k].tail = false;
									brick[i].active = false;
								}
							}
						}
					}
				}
			}
		}


		// PARTIE GAGNEE ? (SI IL N'Y A PLUS AUCUN VAISSEAU)
		if (partie_gagnee == 0 && !intermede)											// on fait le test si la partie est en cours
		{
			partie_gagnee = 1;
			for (int i=0;i<=49;i++)										// pour tous les ships...
			{
				if (ship[i].alive)										// si il reste encore au moins 1 ship vivant...
					partie_gagnee = 0;									// ...alors la partie est en cours
			}
			if (supership[0].alive)										// idem pour le super ship
				partie_gagnee = 0;
		}


		/*___________________________________________________________________
		|                                                                    |
		|                        GESTION DES PARTIES                         |
		|____________________________________________________________________| 
		*/

		// SI LA PARTIE (EN FAIT JUSTE LE NIVEAU) EST GAGNEE
		if (partie_gagnee == 1 && !intermede)
		{
			intermede		= true;
			intermede_fin	= false;
			frame_start		= GetSysTimeSeconds();
			frame_oldtime	= GetSysTimeSeconds();

			int_sound = FSOUND_PlaySoundEx(FSOUND_FREE,sound_applause, dspUnitSounds, false);
			FSOUND_SetVolume(int_sound,250);
			intermede_sound	= true;
		}

		// SI UNE VIE EST PERDUE ET QU'IL EN RESTE
		else if (vie_perdue && (lives > 1) && !intermede)
		{

			intermede		= true;
			intermede_fin	= false;
			frame_start		= GetSysTimeSeconds();
			frame_oldtime	= GetSysTimeSeconds();
		}

		// SI UN VIE EST PERDUE ET QU'IL N'EN RESTE PLUS => LE JEU EST PERDU
		else if (vie_perdue && (lives <= 1) && !intermede)
		{
			partie_gagnee	= -1;
			intermede		= true;
			intermede_fin	= false;
			frame_start		= GetSysTimeSeconds();
			frame_oldtime	= GetSysTimeSeconds();
		}
}
