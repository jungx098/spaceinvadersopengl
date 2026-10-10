// si_Main.h - space invaders opengl

#ifndef _MAIN_H
#define _MAIN_H

#ifdef  _MSC_VER
#pragma warning (disable:4305 4244 4005)
//#pragma warning (disable:4305 4244)	// Disable warnings on precision truncations/conversions
									// 4305 : truncation
									// 4244 : conversion, possible loss of data
#endif

#define PI 3.1415926535

//#include <gl\glaux.h>					// Header File For Some OpenGL Functions
#include <math.h>						// Header File For sin(), cos(), _hypot()
#include <process.h>					// Header File For _beginthread, _endthread 
#include <gl/glut.h>
#include "resource.h"					// Header File For Game Resources
#include "si_Basecode.h"				// Header File For Game
#include "si_Fmod.h"					// Header File For FMOD (sounds & music)
#include "si_Fmod_errors.h"				// Header File For FMOD (sounds & music errors)
#include "si_Fps.h"						// Header File For Frame Per Second, TimerGetTime()
#include "si_Functions.h"				// Header File For Game
#include "si_Particles.h"				// Header File For Game
#include "si_Resources.h"				// Header File For Game
#include "si_World.h"					// Header File For Game
//#include "si_3DS.h"						// Header File For 3DS Objects
#include "si_Font.h"					// Header File For 2D Text

// MACRO : affiche une messagebox avec un message
#define BOX(text) MessageBox(NULL,text,"Info",MB_OK | MB_TOPMOST);

// MACRO : affiche un message d'erreur avec le nom du fichier et la ligne
#define ERR(s) DisplayMsgBoxErrorAndExit(__FILE__, __LINE__, s)

// MACRO : affiche le dernier message d'erreur sur la sortie de Debug
#define LAST_ERROR() DisplayLastErrorDebug()

// MACRO : affiche un *entier* dans le debugger *une seule fois* (si différent de 0)
#define DISPLAY_1INT(var)											\
 		static int my_number = 0;									\
		if (my_number == 0)											\
		{															\
			my_number = var;										\
			char my_string[300]="";									\
			OutputDebugString (_itoa(my_number, my_string, 10));	\
		}															\

// MACRO : affiche un *entier* dans le debugger
#define DISPLAY_INT(var)											\
			{														\
				char my_string[300]="";								\
				OutputDebugString (_itoa(var, my_string, 10));		\
			}														\

// MACRO : affiche une *chaîne* dans le debugger
#define DISPLAY(var)												\
			OutputDebugString (var);								\


//////////////////////////////////////////////////////////////////////////////////////
//
// VARIABLES GLOBALES => Création des structures
//
//

typedef struct							// JOUEUR
{
	bool	alive;						// joueur vivant ?
	bool	explosion;					// explosion en cours ?
	int		explosion_life;				// durée de vie de l'explosion
	int		type;						// type
	float	energy;						// energie
	float	hit_duration;				// durée d'affichage du vaisseau touché
	float	x, y, z;					// coordonnées
	float	yi;							// le vaisseau est secoué (passage à game 3D)
	float	yi_speed;					// le vaisseau est secoué (vitesse)
	float	yi_cos;						// le vaisseau est secoué (variable de cosinus)
	float	zi;							// le vaisseau 'flotte' (coord z, pour mode 3D)
	float	zi_speed;					// le vaisseau 'flotte' (vitesse)
	float	size_x, size_y;				// taille
	float	movement;					// déplacement (pour le glissement)
	bool	laser_left;					// laser gauche vivant ?
	bool	laser_left_explosion;		// laser gauche explosion en cours ?
	float	laser_left_explosion_life;	// laser gauche durée de l'explosion
	bool	laser_right;				// laser droite vivant ?
	bool	laser_right_explosion;		// laser droite explosion en cours ?
	float	laser_right_explosion_life;	// laser droite durée de l'explosion
}
players;
extern players player[10];				// nombre de joueurs

typedef struct							// SUPERSHIP
{
	bool	alive;						// supership vivant ?
	int		health;						// santé
	float	hit_duration;				// durée d'affichage du supership touché
	bool	explosion;					// explosion en cours ?
	int		explosion_life;				// durée de vie de l'explosion
	int		type;						// quel type de supership
	float	speed;						// vitesse
	float	x, y, z;					// coordonnées
	float	size_x, size_y;				// taille
}
superships;
extern superships supership[10];		// nombre de superships

typedef struct							// BUNKERS
{
	bool	active;						// brique active ?
	float	x, y, z;					// coordonnées
}
bricks;
extern bricks brick[500];				// nombre de briques

typedef struct							// SHIPS
{
	bool	alive;						// ship ennemi vivant ?
	int		health;						// santé
	float	hit_duration;				// durée d'affichage d'un ship touché
	bool	explosion;					// explosion en cours ?
	float	explosion_life;				// durée de l'explosion
	int		type;						// 1,2 ou 3 (quel type de vaisseau ennemi)
	double	x, y, z;					// coordonnées
	float	zi;							// le vaisseau 'flotte' (coord z pour mode 3D)
	float	zi_speed;					// le vaisseau 'flotte' (vitesse)
	float	size_x, size_y;				// taille
	float	my_rand;					// nombre aléatoire
}
ships;
extern ships ship[50];					// nombre de ships ennemis

typedef struct							// TIRS (JOUEUR ET ENNEMIS)
{
	int		type;						// type (0 1 2 = tir player, 10 = tir ships)
	bool	active;						// tir actif ?
	bool	tail;						// trainée pour les particules (quand le tir a touché un impact, la trainée doit être toujours présente jusqu'à dissipation)
	float	x, y, z;					// coordonnées
	//particles2 particle2[MAX_PARTICLES2];// tir 3D
}
tirs;
extern tirs	tir[51];					// nombre de tirs permis au même moment (index remis à zéro si limite dépassée)

typedef struct							// BONUS
{
	int		type;						// type
	bool	active;						// actif ?
	bool	explosion;					// explosion en cours ?
	float	explosion_life;				// durée de l'explosion
	float	x, y, z;					// coordonnées
	float	shake1, shake2;				// la scène tremble
}
bonuses;
extern bonuses	bonus[10];				// nombre de bonus


//////////////////////////////////////////////////////////////////////////////////////
//
// VARIABLES GLOBALES => Autres Variables
//
//

// Application - Options
extern GL_Window* g_window;				// infos de la fenêtre (variable globale)
extern const char game_version[6];		// version du jeu (pour gestion des erreurs)
extern bool		game_3d;				// 3D activée ?
extern bool		game_3d_lines;			// mode GL_LINES (en mode 3D) ?
extern bool		game_option_mipmap;		// mipmapping ?
extern bool		game_option_vsync;		// VSync ?
extern bool		game_option_billboard;	// billboarding ?
extern bool		game_option_anti_aliasing;// anti-aliasing ?
extern int		game_option_anti_aliasing_mode;// anti-aliasing mode
extern bool		game_option_stream;		// music streaming ?
extern int		game_option_bitrate;	// music streaming bitrate
extern bool		game_option_wivik;		// wivik keyboard handling activated
extern int		game_option_holddown_wivik;// hold down key duration in ms for wivik keyboard
extern bool		game_antialiasing_active;// anti-aliasing active ?
extern bool		game_vsync_active;		// vsync active ?
extern int		game_lang;				// language (0 english, 1 french, 2 spanish)

// Console
extern bool		game_console;
extern char		console_command[40];
extern char		console_command2[40];
extern char		console_output[40];
extern char		console_output2[40];
extern bool		game_console_come_in;
extern bool		game_console_start;
extern float	viewport_push;
extern bool		game_console_moving;

// Musique
extern char		stream_name[1024];		// streamed music name
extern char		stream_name_user[1024];	// streamed music user
extern char		music_en_cours[1024];	// musique en cours
extern char		music_affichee[1024];	// musique affichee (longueur limitee)
extern Playlist* my_playlist;			// Shoutcast playlist

extern int		int_sound,				// numero channel pour les bruitages
				int_sound_mp3,			// numero channel pour les musiques mp3
				int_sound_mod;			// numero channel pour la musique mod
extern bool		music_on;				// musique mp3 on/off
extern int		music_type;				// musique type
extern int		id_my_music;			// identifiant pour la musique en cours : music[id_my_music].titre (0="." et 1="..")
extern FSOUND_DSPUNIT * dspUnitSounds;	// DSP utilisé pour les sons non "intégrés" dans le spectrum
extern FSOUND_DSPUNIT * dspUnitSpectrum;// DSP utilisé pour le spectrum (musiques streams, mp3 ...)
extern signed short *OscBuffer;
extern int           OscBlock;
extern FSOUND_STREAM * my_stream;		// musique streaming
extern FSOUND_STREAM * my_mp3;			// musique mp3
extern FMUSIC_MODULE * my_mod;			// musique mod
extern FMUSIC_MODULE * my_mod_about;	// musique mod about-dialogbox
extern FSOUND_SAMPLE * sound_tir,		// sons wav et mp3
				* sound_tir3d,
				* sound_touche,
				* sound_descente,
				* sound_applause,
				* sound_shield,
				* sound_cross,
				* sound_bonus,
				* sound_key;
extern bool		sound_cross_passed;		// son cross passage 2d->3d
typedef struct {char fichier[256];}		// nom de fichier d'une musique mp3
musics;
extern musics	music[300];				// 300 struct music
/*
typedef struct
{
    int    count;
    void **name;
    void **displayname;
} Playlist;								// Winamp playlist
*/

extern int		game_step;				// on importe la variable etape du jeu
extern float	game_3d_ajust;			// 3D adjust
extern char		console_text[50][50];	// console text
extern int		console_line;			// console line
extern int		id_brick;				// index des briques
extern GLuint	texture[100];			// tableau des textures
extern int		game_level;				// fond du jeu
extern int		ships_choix_anim;		// anim des ships ennemis
extern const int max_tirs;				// nombre de tirs maximum affichés en même temps
extern unsigned int	score;				// score
extern unsigned int	hiscore;			// highscore
extern int		lives;					// vies
extern bool		powerup;				// powerup (passage à game 3D)
extern bool		Fps2_firstpass;			// FPS Méthode n°2
extern double	angle_x_start;			// angle x utilisateur
extern double	angle_y_start;			// angle y utilisateur
extern int		fontsize;				// font size
extern int		fontspace;				// space between fonts
extern int		equalizer_x;			// equalizer coord x
extern int		equalizer_y;			// equalizer coord y
extern int		shoot_type;

/*
extern Model_3DS m;						// objet 3DS
extern Model_3DS m1;					// objet 3DS
extern Model_3DS m2;					// objet 3DS
extern Model_3DS m3;					// objet 3DS
//extern Model_3DS m4;					// objet 3DS
extern Model_3DS m5;					// objet 3DS
*/
extern GLUquadricObj*	quadratic;		// Storage For Our Quadratic Objects

#endif
