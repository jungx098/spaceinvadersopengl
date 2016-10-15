// si_World.h - space invaders opengl

#ifndef _WORLD_H
#define _WORLD_H

//////////////////////////////////////////////////////////////////////////////////////
//
// WOLRD => Les fonctions d'initialisation et d'affichage des éléments du jeu
//
//

// Initialisation des éléments

void init_ships();
void init_player();
void init_supership();
void init_shoots();
void init_bonuses(bool game_3d_local);
void init_bunker(float x, float y, float z);

// Affichage des éléments

void display_ship(int i);
void display_player();
void display_supership();
void display_shoot(int i, bool viewport2);
void display_bonus_2d_to_3d();
void display_ring_2d_to_3d();
void display_bonus_3d();
void display_bunkers();
void display_score_hiscore_lives();
void display_below_lives();
void display_background(bool viewport2);
void display_image();
void display_cube(int texture_id, float size);
void display_console();
void display_console_mini_screen();
void display_text_frames();
void display_energy();
void display_spectrum();
void display_equalizer();

//////////////////////////////////////////////////////////////////////////////////////
//
// WORLD => Le splash screen
//
//
void display_splashscreen();
extern bool active_splash;
extern bool active_reverse_splash;

#endif