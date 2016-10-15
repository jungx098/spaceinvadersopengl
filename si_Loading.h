// si_Functions.h - space invaders opengl

#ifndef _LOADING_H
#define _LOADING_H

//#define LOG0(text) add_log(text, false);		// MACRO de LOG (ajout d'une entrée dans les logs, _NON_ pris en compte pour la progress bar)
//#define LOG(text)  add_log(text, true);			// MACRO de LOG (ajout d'une entrée dans les logs, prise en compte pour la progress bar)

//////////////////////////////////////////////////////////////////////////////////////
//
// LOADING => Fonctions
//
//

// CHARGEMENT => GLOBAL
void loading (void *dummy);

// CHARGEMENT => LOG SYSTEM
void add_log (char* text, bool addToProgressBar);

// CHARGEMENT => PROGRESS BAR
void progress_bar ();

// CHARGEMENT => MUSIQUE ET SONS
//FSOUND_SAMPLE * LoadSound (int IntResource);
void LoadSounds();

// CHARGEMENT => TEXTURES
void LoadTextures();

// CHARGEMENT => AFFICHAGE
void loading_display1 (void);
void loading_display2 (bool addToProgressBar);
void loading_display3 (void);

// 2ème FPS System
BOOL initftime(void);
double ftime(void);
extern float framestart;
extern float first;

#endif