// si_World.cpp - space invaders opengl

//#define	DEBUG_PASSAGE_3D			// DEBUGGAGE (aussi dans si_main.cpp)
#include "si_Main.h"
#include "si_3DS.h"								// Header File For Game
extern Model_3DS m;						// objet 3DS
extern Model_3DS m1;					// objet 3DS
extern Model_3DS m2;					// objet 3DS
extern Model_3DS m3;					// objet 3DS
extern Model_3DS m4;					// objet 3DS
extern Model_3DS m5;					// objet 3DS

int design_tex = -2;					// essais

//////////////////////////////////////////////////////////////////////////////////////
//
// Initialisation des ships
//
//
void init_ships(){

	int i;

	// ships 1ère ligne (en haut)
	for (i=0;i<=9;i++)
	{
		ship[i].alive		= true;			// vivant ?
		ship[i].type		= 1;			// type de vaisseau
		ship[i].x			= 8 - 1 - i;	// coord x (soit 7 pour le 1er, 6 pour le 2eme, etc.)
		ship[i].y			= 2;			// coord y
		ship[i].z			= 0.05f;		// coord z
		ship[i].size_x		= 0.2f;			// taille en largeur
		ship[i].size_y		= 0.2f;			// taille en hauteur
	}

	// ships 2ème ligne
	for (i=10;i<=19;i++)
	{
		ship[i].alive		= true;
		ship[i].type		= 1;
		ship[i].x			= 8 - 1 - i + 10;// +10 car on part de 10 à 19 !
		ship[i].y			= 1.2f;
		ship[i].z			= 0.05f;
		ship[i].size_x		= 0.2f;
		ship[i].size_y		= 0.2f;
	}

	// ships 3ème ligne
	for (i=20;i<=29;i++)
	{
		ship[i].alive		= true;
		ship[i].type		= 1;
		ship[i].x			= 8 - 1 - i + 20;
		ship[i].y			= 0.4f;
		ship[i].z			= 0.05f;
		ship[i].size_x		= 0.2f;
		ship[i].size_y		= 0.2f;
	}

	// ships 4ème ligne
	for (i=30;i<=39;i++)
	{
		ship[i].alive		= true;
		ship[i].type		= 2;
		ship[i].x			= 8 - 1 - i + 30;
		ship[i].y			= -.4f;
		ship[i].z			= 0.05f;
		ship[i].size_x		= 0.3f;
		ship[i].size_y		= 0.2f;
	}

	// ships 5ème ligne (en bas)
	for (i=40;i<=49;i++)
	{
		ship[i].alive		= true;
		ship[i].type		= 3;
		ship[i].x			= 8 - 1 - i + 40;
		ship[i].y			= -1.2f;
		ship[i].z			= 0.05f;
		ship[i].size_x		= 0.4f;
		ship[i].size_y		= 0.2f;
	}

	// pour tous les ships
	for (i=0;i<=49;i++)
	{
		ship[i].health			= 5;//10
		ship[i].hit_duration	= 500;
		ship[i].zi				= 0.00;
		ship[i].zi_speed		= (float)(rand()%8+3) * 0.001;
		ship[i].explosion		= false;
		ship[i].explosion_life	= 10000;
	}

}

//////////////////////////////////////////////////////////////////////////////////////
//
// Initialisation du player
//
//
void init_player()
{
	player[0].alive			=	true;
	player[0].type			=	0;
	player[0].energy		=	20;
	player[0].hit_duration	=	500;
	player[0].x				=	0;
	player[0].y				=  -6.15;
	player[0].yi			=	0.00;
	player[0].yi_speed		=	0.01;
	player[0].yi_cos		=	0;
	player[0].z				=	0.05;
	player[0].zi			=	0.00;
	player[0].zi_speed		=	0.01;
	player[0].size_x		=	0.3;
	player[0].size_y		=	0.25;
	player[0].movement		=	0;
	player[0].laser_left	=	false;
	player[0].laser_right	=	false;
}

//////////////////////////////////////////////////////////////////////////////////////
//
// Initialisation du supership
//
//
void init_supership()
{
	supership[0].alive		=	true;
	supership[0].health		=	25;
	supership[0].hit_duration	=	500;
	supership[0].type		=	0;
	supership[0].speed		=	2;
	supership[0].x			=	0;
	supership[0].y			=	4;
	supership[0].z			=	0.05;
	supership[0].size_x		=	0.5;
	supership[0].size_y		=	0.3;
}

//////////////////////////////////////////////////////////////////////////////////////
//
// Initialisation des tirs
//
//
void init_shoots()
{
	for(int i=0; i<=max_tirs; i++)
	{
		tir[i].type = 0;
		tir[i].active = false;
		tir[i].tail = false;
	}
}

//////////////////////////////////////////////////////////////////////////////////////
//
// Initialisation des bonus
//
//
void init_bonuses(bool game_3d_local)
{
	if (!game_3d_local)
	{
		// BONUS N°1 => Power up (passage à jeu 3D)
		bonus[1].type = 0;
		bonus[1].active = true;
		bonus[1].explosion = false;
		bonus[1].explosion_life = 4;
		bonus[1].x = float (rand()%10) - 5;
		bonus[1].y = 10;
		bonus[1].z = 0.05;
		bonus[1].shake1 = 1;
		bonus[1].shake2 = 0;
	}
	else
	{
		if (!player[0].laser_left || !player[0].laser_right)
            // BONUS N°2 => lasers joueur
			bonus[2].type = 1;
		else
            // BONUS N°3 => weapon 1 en mode 3D
			bonus[2].type = 2;
		bonus[2].active = true;
		bonus[2].explosion = false;
		bonus[2].explosion_life = 4;
		bonus[2].x = float (rand()%10) - 5;
		bonus[2].y = 20;
		bonus[2].z = 0.05;
		bonus[2].shake1 = 1;
		bonus[2].shake2 = 0;
	}


	/*
	// BONUS N°2 => '3D' text object
	bonus[0].type = 0;
	bonus[0].active = true;
	bonus[0].x = float (rand()%10) - 5;
	bonus[0].y = 6;
	bonus[0].z = 0.05;
	*/

}

//////////////////////////////////////////////////////////////////////////////////////
//
// Initialisation d'un bunker
//
//
void init_bunker(float x, float y, float z)
{

	// 1ère ligne du bunker
	float position = 0;
	float i = 0, j = 0;

	for (i=0; i<= 9; i++)
	{
		brick[id_brick].active = true;
		brick[id_brick].x = x + position;
		brick[id_brick].y = y;
		brick[id_brick].z = z;
		id_brick++;
		position += 0.1f;
	}

	// 2ème ligne du bunker
	position = 0;
	for (i=0; i<= 11; i++)
	{
		brick[id_brick].active = true;
		brick[id_brick].x = x + position - 0.1f;
		brick[id_brick].y = y - 0.1f;
		brick[id_brick].z = z;
		id_brick++;
		position += 0.1f;
	}

	// 3, 4, 5ème ligne du bunker
	for (j=0; j<=2; j++)
	{
		position = 0;
		for (i=0; i<= 13; i++)
		{
			brick[id_brick].active = true;
			brick[id_brick].x = x + position - 0.2f;
			brick[id_brick].y = y - 0.2f - j/10;
			brick[id_brick].z = z;
			id_brick++;
			position += 0.1f;
		}
	}

	// 6ème ligne
	position = 0;
	for (i=0; i<= 13; i++)
	{
		if ((i < 5) || (i > 8))
		{
			brick[id_brick].active = true;
			brick[id_brick].x = x + position - 0.2f;
			brick[id_brick].y = y - 0.5f;//0.6
			brick[id_brick].z = z;
			id_brick++;
			position += 0.1f;
		}
		else position += 0.1f;
	}

	// 7ème ligne
	position = 0;
	for (i=0; i<= 13; i++)
	{
		if ((i < 4) || (i > 9))
		{
			brick[id_brick].active = true;
			brick[id_brick].x = x + position - 0.2f;
			brick[id_brick].y = y - 0.6f;
			brick[id_brick].z = z;
			id_brick++;
			position += 0.1f;
		}
		else position += 0.1f;
	}

	// 8ème ligne
	position = 0;
	for (i=0; i<= 13; i++)
	{
		if ((i < 3) || (i > 10))
		{
			brick[id_brick].active = true;
			brick[id_brick].x = x + position - 0.2f;
			brick[id_brick].y = y - 0.7f;
			brick[id_brick].z = z;
			id_brick++;
			position += 0.1f;
		}
		else position += 0.1f;
	}

	// 9ème ligne
	position = 0;
	for (i=0; i<= 13; i++)
	{
		if ((i < 3) || (i > 10))
		{
			brick[id_brick].active = true;
			brick[id_brick].x = x + position - 0.2f;
			brick[id_brick].y = y - 0.8f;
			brick[id_brick].z = z;
			id_brick++;
			position += 0.1f;
		}
		else position += 0.1f;
	}

	// après l'initialisation du dernier bunker,
	// toutes les briques restantes sont inactives
	if (id_brick > 400)
		for (int i=id_brick; i<= 499; i++)
			brick[i].active = false;

	// => 102 briques par bunker
}

//////////////////////////////////////////////////////////////////////////////////////
//
// Affichage de l'equalizer
//
//
void display_equalizer(void)
{
	if (g_window->init.width < 550) return;

	// mode ortho 2D
	glMatrixMode(GL_PROJECTION);
	glPushMatrix ();
	glLoadIdentity();
	glOrtho(0.0, (GLdouble)g_window->init.width, 0.0, (GLdouble)g_window->init.height, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix ();
	glLoadIdentity();

		// EQUALIZER

		// alpha test + blend
		glDisable(GL_ALPHA_TEST);									// alpha test
		//glAlphaFunc(GL_GREATER, 0.9f);								// alpha test
		glEnable(GL_BLEND);											// blending
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			// for anti-aliasing

		// résolution
		int w = g_window->init.width;
		int h = g_window->init.height;

		// paliers de résolutions
		int w2;
		if (w >= 1600) w2 = 1600; else if (w >= 1280) w2 = 1280; else if (w >= 1024) w2 = 1024;	else if (w >= 800) w2 = 800; else w2 = 640;

		// on affiche le cadre de l'equalizer
		glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
		glBindTexture (GL_TEXTURE_2D, texture[35]);
		glBegin (GL_QUADS);
			glTexCoord2f (0,1);	glVertex2f (equalizer_x+w+(-183      )*w2/1024, equalizer_y+h+(-238      )*w2/1024);	//102
			glTexCoord2f (0,0);	glVertex2f (equalizer_x+w+(-183      )*w2/1024, equalizer_y+h+(-238 + 128)*w2/1024);
			glTexCoord2f (1,0);	glVertex2f (equalizer_x+w+(-183 + 256)*w2/1024, equalizer_y+h+(-238 + 128)*w2/1024);	//147
			glTexCoord2f (1,1);	glVertex2f (equalizer_x+w+(-183 + 256)*w2/1024, equalizer_y+h+(-238      )*w2/1024);
		glEnd ();

		// BARRES DE L'EQUALIZER
		
		// on récupère les données du spectrum
		static float barre1[8] = {0,0,0,0,0,0,0,0};
		static float barre2[8] = {0,0,0,0,0,0,0,0};
		float temp[8] = {0,0,0,0,0,0,0,0};

		float* spec = FSOUND_DSP_GetSpectrum();

		// WORK ABOUT SPECTRUM ANALYSIS
/*
		int offset;
		float xoff, step;
		signed short *src;

		//The next pcmblock (Oscblock + 1) is the one that is audible.
		offset = (OscBlock + 1) * FSOUND_DSP_GetBufferLength();
		if (offset >= FSOUND_DSP_GetBufferLengthTotal())
			offset -= FSOUND_DSP_GetBufferLengthTotal();

		src = &OscBuffer[offset];

		char tempstr[255]="";
		_itoa(*src, tempstr, 10);
		OutputDebugString(tempstr);


		// xoff is the x position that is scaled lookup of the dsp block according to the graphical window size.
		xoff = 0;
		step = (float)FSOUND_DSP_GetBufferLength() / (float)160;

		//char tempstr[255]="";
		//_itoa(step, tempstr, 10);
		//OutputDebugString(tempstr);
*/	
		// on adapte les données aux barres de l'equalizer
		if (spec)
		//if (OscBuffer)
			for (int i=0; i<=7; i++)							// pour les 8 barres de l'equalizer
			{
				barre1[i] = barre2[i];							// les barres affichées (barre1[]) sont celles
																// calculées la frame précédente (barre2[])

				for (int j=0; j<=19; j++)						// on récupère les nouvelles valeurs (par tranches de 20, 1 barre = 20 valeurs)
					temp[i] += spec[(i*20)+j];
					//temp[i] += src[(i*20)+j];

				temp[i] /= 4;//3								// on redimensionne la hauteur
				//temp[i] /= 100000/3;//3								// on redimensionne la hauteur

				if (temp[i] > barre2[i])						// si la nouvelle valeur est + grande on met à jour la barre
				{				
					barre2[i] = temp[i];
					if (i != 0 && i != 1 && barre2[i] > 1)		// on limite à la taille de la texture (de 0 à 1)
					//if (barre2[i] > 1)		// on limite à la taille de la texture (de 0 à 1)
						barre2[i] = 1;							// sauf les deux 1ères barres (les basses) qui sont customisées plus bas
				}
				else											// sinon elle décroît
				{
					if (barre2[i] > 0.002*ms)
						barre2[i] -= 0.002*ms;
					else
						barre2[i] = 0;
				}
			}

		// customization of some strips for better looking

		barre2[0] = (barre2[0])*0.3;	// customisation pour les sons graves (les deux 1ères barres)
		//barre2[0] = (barre2[0] - 0.4)*0.7;//0.3	// customisation pour les sons graves (les deux 1ères barres)
//		barre2[0] = (barre2[0] -0.1)*0.5;//0.3	// customisation pour les sons graves (les deux 1ères barres)
		if (barre2[0] > 1)
			barre2[0] = 1;

//		barre2[1] = (barre2[1] - 0.0)* 1.0;//0.7
		barre2[1] = barre2[1]*0.5; //0.7 0.5
		barre2[1] += barre2[0]*0.3;//0.2 0.3
		if (barre2[1] > 1)
			barre2[1] = 1;

		if (barre2[2] > 1)
			barre2[2] = 1;

//		barre2[2] += barre2[0]*0.1;

		// on affiche les 8 barres de l'equalizer
		glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
		glBindTexture (GL_TEXTURE_2D, texture[36]);
		glBegin (GL_QUADS);
			for(int i=0; i<=7; i++)
			{
				glTexCoord2f (0,barre2[i]); glVertex2f (equalizer_x+w+(-176      + i*17)*w2/1024, equalizer_y+h+(-229 + 128 *barre2[i])*w2/1024);	//86
				glTexCoord2f (0,0);	        glVertex2f (equalizer_x+w+(-176      + i*17)*w2/1024, equalizer_y+h+(-229                 )*w2/1024);
				glTexCoord2f (1,0);         glVertex2f (equalizer_x+w+(-176 + 16 + i*17)*w2/1024, equalizer_y+h+(-229                 )*w2/1024);	//14
				glTexCoord2f (1,barre2[i]); glVertex2f (equalizer_x+w+(-176 + 16 + i*17)*w2/1024, equalizer_y+h+(-229 + 128 *barre2[i])*w2/1024);
			}
		glEnd ();

		// retour aux anciens paramètres (alpha test et blending)
		glDisable(GL_ALPHA_TEST);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);

	glMatrixMode (GL_PROJECTION);
	glPopMatrix ();
	glMatrixMode (GL_MODELVIEW);
	glPopMatrix ();

	// active/désactive le DSP si la touche <M> a été appuyée
	if (!music_on && FSOUND_DSP_GetActive(FSOUND_DSP_GetFFTUnit()))
		for (int i=0; i<=7; i++)
		{
			if (barre2[i] > 0)
				break;
			FSOUND_DSP_SetActive(FSOUND_DSP_GetFFTUnit(), FALSE);
		}
	else if (music_on && !FSOUND_DSP_GetActive(FSOUND_DSP_GetFFTUnit()))
		FSOUND_DSP_SetActive(FSOUND_DSP_GetFFTUnit(), TRUE);



	//glPushMatrix();
	//	glLoadIdentity ();
	//	gluLookAt(	0.0, 0.0, 20.0,									// l'oeil
	//				0.0, 0.0,  0.0,									// le centre
	//				0.0, 1.0,  0.0	);								// le haut

	//	// equalizer
	//	glColor4f (0.6f, 0.6f, 0.6f, 1.0f);
	//	glBindTexture (GL_TEXTURE_2D, texture[35]);

	//	// ALPHA TEST + BLEND
	//	glAlphaFunc(GL_GREATER, 0.9f);								// for TGA alpha test
	//	glEnable(GL_ALPHA_TEST);									// for TGA alpha test
	//	
	//	glDisable(GL_BLEND);
	//	//glDisable(GL_DEPTH_TEST);
	//	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			// for anti-aliasing

	//	float ratio_x = 22.0 / g_window->init.width;					// 22.0 = largeur écran
	//	float ratio_y = 16.5 / g_window->init.height;					// 16.5 = hauteur écran
	//	
	//	glBegin (GL_QUADS);
	//		glTexCoord2f (0,1);	glVertex2f (8.7f - 1.6f              , 3.2f + 102*ratio_y);
	//		glTexCoord2f (0,0);	glVertex2f (8.7f - 1.6f              , 3.2f + 0.00f);
	//		glTexCoord2f (1,0);	glVertex2f (8.7f - 1.6f + 147*ratio_x, 3.2f + 0.00f);
	//		glTexCoord2f (1,1);	glVertex2f (8.7f - 1.6f + 147*ratio_x, 3.2f + 102*ratio_y);
	//	glEnd ();

	//	// BACK TO NON ALPHA TEST + PREVIOUS BLEND
	//	glDisable(GL_ALPHA_TEST);
	//	
	//	// EQUALIZER BARRES
	//	glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
	//	glBindTexture (GL_TEXTURE_2D, texture[36]);
	//	for(int i=0; i<=7; i++)
	//	{
	//		glBegin (GL_QUADS);
	//			glTexCoord2f (0,1);	glVertex2f (8.7f - 1.45f              + i*17*ratio_x, 3.37f + 86*ratio_y);
	//			glTexCoord2f (0,0);	glVertex2f (8.7f - 1.45f              + i*17*ratio_x, 3.37f + 0.00f);
	//			glTexCoord2f (1,0);	glVertex2f (8.7f - 1.45f + 14*ratio_x + i*17*ratio_x, 3.37f + 0.00f);
	//			glTexCoord2f (1,1);	glVertex2f (8.7f - 1.45f + 14*ratio_x + i*17*ratio_x, 3.37f + 86*ratio_y);
	//		glEnd ();
	//	}

	//	// BACK TO PREVIOUS BLEND
	//	glEnable(GL_BLEND);
	//	glBlendFunc(GL_SRC_ALPHA,GL_ONE);

	//glPopMatrix();

}

//////////////////////////////////////////////////////////////////////////////////////
//
// FONCTIONS => Spectrum 3D
//
//
void display_spectrum(void)
{
/*
	static int		choix_anim		= 0;
	float			beat_detection	= 0;
	static float	beat_scale		= 0;
	float			beat_sum		= 0;
	static float	xrot			= 0;
	static float	yrot			= 0;
	static float	zrot			= 0;

	// on récupère le spectrum
	float* spec = FSOUND_DSP_GetSpectrum();

	for (int i=0;i<=70;i++)
		beat_detection += spec[i];
	beat_detection /= 70 - 0.0;

	if (beat_detection > beat_scale + 0.0)
		beat_scale = beat_detection;

	beat_scale -= 0.007;

	char my_string[300]="";
	_ltoa(beat_scale, my_string, 10);
	//OutputDebugString(_ltoa(beat_scale, my_string, 10));
	//OutputDebugString(_ltoa(10.0, my_string, 10));
	//fontDrawString (100, 300, "beat float =  %f, beat string = %s", beat_scale,my_string);

	if (game_3d && choix_anim == 0)
	{
		glPushMatrix();

		glLoadIdentity ();
		gluLookAt(	0.0, 0.0, 20.0,										// l'oeil
					0.0, 0.0,  0.0,										// le centre
					0.0, 1.0,  0.0	);									// le haut

		glTranslatef ( 0.0, 0.0, 8 );

		beat_sum = 0.012f + beat_scale * 0.07f;

		m.scale = beat_sum;

		// textes
		fontSize(fontsize);fontDrawString (260, g_window->init.height - fontspace*2.9 , "beat detec =  %f", beat_detection);
		fontSize(fontsize);fontDrawString (260, g_window->init.height - fontspace*4.2 , "beat scale =  %f", beat_scale);
		fontSize(fontsize);fontDrawString (260, g_window->init.height - fontspace*5.5 , "beat sum   =  %f", beat_sum);
	
		// tambourin
		m.pos.x =  5.2;
		m.pos.y = -3.7;

		m.rot.x +=  0.8f;
		m.rot.y +=  0.4f;
		m.rot.z +=  0.6f;

		// LUMIERE
		static float ambient[] =			{0.8, 0.8, 0.8, 1.0}; //0.9
		static float diffuse[] =			{0.2, 0.2, 0.2, 1.0}; //0.5
		glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);

		// LUMIERE (MATERIAU)
		static float mat_ambient[] = {0.5, 0.5, 0.5, 1.0}; //0.7 ou 0.8
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
		static float mat_diffuse_ships[] = {0.1, 0.1, 0.1, 1.0};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse_ships);

		// LUMIERE (POSITION)
		static float position_player[] = {1.0, 1.0, 15.0, 0.0};
		glLightfv(GL_LIGHT1, GL_POSITION, position_player);

		m.Draw();

		glPopMatrix();
	}
*/
}

//////////////////////////////////////////////////////////////////////////////////////
//
// Affichage lorsqu'un ship est touché
//
//
void display_hit_ship(Model_3DS &model, int i)
{

	ship[i].hit_duration -= 10 * ms;
	if (ship[i].hit_duration <= 0)
		ship[i].hit_duration = 500;

	model.Draw(-1, GL_TRIANGLES);						// ship non texturé, blanc

}

//////////////////////////////////////////////////////////////////////////////////////
//
// Affiche un ship 3DS, appelée par display_ship()
//
//
void display_ship_element(Model_3DS &model, int i)
{
	model.scale = 0.0015f;

	model.rot.x = 90.0f;
	model.rot.y = 0.0f;
	model.rot.z = 0.0f;

	model.pos.x = ship[i].x;
	model.pos.y = ship[i].y;
	model.pos.z = cosf(ship[i].zi * 4) / 15;			// amplitude : de -1/var à 1/var

	// affiche le ship touché
	if (ship[i].hit_duration < 500)
		display_hit_ship(model, i);
	else
	// affiche le ship normal
		if (game_level == 6)
			model.Draw(4, GL_TRIANGLES);
		else
			model.Draw();

////tests stencil buffer (DEBUT)
//if (!game_3d_lines)
//		model.Draw();
//else
//{
//	 game_3d_lines = false;
//     glClearStencil(0);
//     glClear(GL_STENCIL_BUFFER_BIT);
//
//     // Render the mesh into the stencil buffer.
// 
//     glEnable(GL_STENCIL_TEST);
//
//     glStencilFunc(GL_ALWAYS, 1, -1);
//     glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
//
//     glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//
//     //m.Draw();
//
//     // Render the thick wireframe version.
//
//     glStencilFunc(GL_NOTEQUAL, 1, -1);
//     glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
//
//     glLineWidth(1);
//     glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
// 
//     //m.Draw();
//	 model.Draw();
//
//	 glDisable(GL_STENCIL_TEST);
//	 glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//	 glLineWidth(1);
//	 game_3d_lines = true;
//}
////tests stencil buffer (FIN)
		
}

//////////////////////////////////////////////////////////////////////////////////////
//
// Affiche un ship
//
//
void display_ship(int i)
{
	// GAME 2D
	if (!game_3d)
	{
		glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
		glBindTexture (GL_TEXTURE_2D, texture[ship[i].type + ships_choix_anim]);// attention à faire correspondre les id des textures (1, 2, 3)

		glBegin (GL_QUADS);
			glTexCoord2f (0,1);	glVertex2f (ship[i].x - ship[i].size_x, ship[i].y + ship[i].size_y);
			glTexCoord2f (0,0);	glVertex2f (ship[i].x - ship[i].size_x, ship[i].y - ship[i].size_y);
			glTexCoord2f (1,0);	glVertex2f (ship[i].x + ship[i].size_x, ship[i].y - ship[i].size_y);
			glTexCoord2f (1,1);	glVertex2f (ship[i].x + ship[i].size_x, ship[i].y + ship[i].size_y);
		glEnd ();
	}

	// GAME 3D - affiche ship
	else
	{
		// le ship 'flotte'
		ship[i].zi += ship[i].zi_speed * ms / 15;

		// ship type 1
		if (ship[i].type == 1)											// ships du haut
		{
			if (0 <= i && i <= 9)
				glColor4f (.5f, .5f, .5f, 1.0f);
			else if (10 <= i && i <= 19)
				glColor4f (.6f, .6f, .6f, 1.0f);
			else if (20 <= i && i <= 29)
				glColor4f (.7f, .7f, .7f, 1.0f);
			display_ship_element(m1, i);
		}

		// ship type 2
		else if (ship[i].type == 2)										// ships du milieu
		{
			glColor4f (.85f, .85f, .85f, 1.0f);
			display_ship_element(m1, i);
		}

		// ship type 3
		else if (ship[i].type == 3)										// ships du bas
		{
			glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
			display_ship_element(m1, i);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////
//
// Affiche le player
//
//
void display_player()
{
	// GAME 2D
	if (!game_3d)
	{
		glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
		glBindTexture (GL_TEXTURE_2D, texture[0]);

		// ALPHA TEST + BLEND
		glAlphaFunc(GL_GREATER, 0.3f);								// for TGA alpha test
		glEnable(GL_ALPHA_TEST);									// for TGA alpha test
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			// for anti-aliasing

		glBegin (GL_QUADS);
			glTexCoord2f (0,1);	glVertex2f (player[0].x - .3, player[0].y + player[0].yi +.25);
			glTexCoord2f (0,0);	glVertex2f (player[0].x - .3, player[0].y + player[0].yi -.25);
			glTexCoord2f (1,0);	glVertex2f (player[0].x + .3, player[0].y + player[0].yi -.25);
			glTexCoord2f (1,1);	glVertex2f (player[0].x + .3, player[0].y + player[0].yi +.25);
		glEnd ();

		// BACK TO NON ALPHA TEST + PREVIOUS BLEND
		glDisable(GL_ALPHA_TEST);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	}
	
	// GAME 3D (affiche joueur)
	else
	{
		glColor4f (1.0f, 1.0f, 1.0f, 1.0f);

		// joueur
		m.rot.x =  -270.0f;
		m.scale = 0.15f;
		m.pos.x = player[0].x;
		if (bonus[1].explosion == false)							// car si passage 3D c'est display_bonus_2d_to_3d qui décide du y du player
			m.pos.y = player[0].y;

		// Anti-aliasing (DEBUT)
		if (game_antialiasing_active && !game_3d_lines)
			glEnable(GL_MULTISAMPLE_ARB);

		//m.Draw(0, GL_LINES);
		//game_3d_lines = false;
		//m.lit = false;
		//glColor4f (0.0f, 0.0f, 0.0f, 1.0f);

		//if (game_level == 6)
			//m.Draw(design_tex, GL_TRIANGLES;
		//	m.Draw(4, GL_TRIANGLES);
		//else

		// joueur touché
		if (player[0].hit_duration < 500)
		{
			player[0].hit_duration -= 20 * ms;
			if (player[0].hit_duration <= 0)
				player[0].hit_duration = 500;

			m.Draw(-1, GL_TRIANGLES);	// vaisseau non texturé, blanc
		}
		
		// joueur normal
		else
			m.Draw();

////tests stencil buffer (DEBUT)
//if (!game_3d_lines)
//		m.Draw();
//else
//{
//	 game_3d_lines = false;
//     glClearStencil(0);
//     glClear(GL_STENCIL_BUFFER_BIT);
//     // Render the mesh into the stencil buffer.
// 
//     glEnable(GL_STENCIL_TEST);
//     glStencilFunc(GL_ALWAYS, 1, -1);
//     glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
//     glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//     //m.Draw();
//     // Render the thick wireframe version.
//     glStencilFunc(GL_NOTEQUAL, 1, -1);
//     glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
//     glLineWidth(1);
//     glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
// 
//     //m.Draw();
//	 m.Draw(-1, GL_TRIANGLES);
//	 glDisable(GL_STENCIL_TEST);
//	 glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//	 glLineWidth(1);
//	 game_3d_lines = true;
//}
////tests stencil buffer (FIN)


			//m.Draw(-1, GL_TRIANGLES);
		//game_3d_lines = true;
		//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		//glEnable(GL_POLYGON_OFFSET_LINE);
		//glPolygonOffset(-1.0f,-1.0f);
		//m.lit = true;
		//glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
		//m.Draw(4, GL_LINES);
		//glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		//glDisable(GL_POLYGON_OFFSET_LINE);
		//game_3d_lines = true;
	

		// laser gauche
		if (player[0].laser_left)
		{
			m3.rot.x =  -270.0f;
			m3.scale = 0.15f;
			m3.pos.x = player[0].x - 0.9;
			if (bonus[1].explosion == false)						// car si passage 3D c'est display_bonus_2d_to_3d qui décide du y du player
				m3.pos.y = player[0].y;
			m3.Draw();
		}
		
		// laser droite
		if (player[0].laser_right)
		{
			m3.rot.x =  -270.0f;
			m3.scale = 0.15f;
			m3.pos.x = player[0].x + 0.9;
			if (bonus[1].explosion == false)						// car si passage 3D c'est display_bonus_2d_to_3d qui décide du y du player
				m3.pos.y = player[0].y;
			m3.Draw();
		}

		// Anti-aliasing (FIN)
		if (game_antialiasing_active && !game_3d_lines)
			glDisable(GL_MULTISAMPLE_ARB);

		// Pas de lumière sur les réacteurs
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);

		// REACTEURS DU PLAYER
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		glFrontFace(GL_CCW);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glDisable(GL_CULL_FACE);

		glBindTexture (GL_TEXTURE_2D, texture[29]);					// 29 (chargée) ou 91 (générée)
		
		// alpha des réacteurs
		static float alpha = 0.83f;
		static bool alpha_down = true;
		if (alpha > 0.83 && alpha_down)
			alpha -= 0.005*ms;
		else if (alpha < 1.1 && !alpha_down)
			alpha += 0.005*ms;
		else if (alpha < 0.83 && alpha_down)
			alpha_down = false;
		else if (alpha > 0.83 && !alpha_down)
			alpha_down = true;

		//glColor4f (0.953f, 0.340f, 0.120f, 0.9f);					// rouge
		glColor4f (0.953f, 0.340f, 0.120f, alpha);					// rouge

		float ln = 0.00;											// particules externes des lasers + grandes pour le mode 3D_LINES
		float ln2 = 0.00;											// particules internes des réacteurs + petites pour le mode *non* 3D_LINES
		if (game_3d_lines)
			ln = 0.20;
		else ln2 = 0.02;

		// réacteurs du vaisseau
		glPushMatrix();

		// on va à la position du vaisseau
		glTranslatef(m.pos.x, m.pos.y, m.pos.z);
		glRotatef(m.rot.x, 1.0f, 0.0f, 0.0f);
		glRotatef(m.rot.y, 0.0f, 1.0f, 0.0f);
		glRotatef(m.rot.z, 0.0f, 0.0f, 1.0f);

		float modelview[16];
		int i,k,l;

			// réacteur gauche (avec billboarding)
			glPushMatrix();
				if (!game_3d_lines)
					glTranslatef(- 0.23, 0.03, 0.33); // -0.23 0.03 0.35
				else glTranslatef(- 0.23, 0.03, 0.35);

				// get the current modelview matrix
				glGetFloatv(GL_MODELVIEW_MATRIX , modelview);

				// undo all rotations (beware all scaling is lost as well)
				for( k=0; k<3; k++ ) 
					for( l=0; l<3; l++ ) {
						if ( k==l )
							modelview[k*4+l] = 1.0;
						else
							modelview[k*4+l] = 0.0;
					}

				// set the modelview with no rotations and scaling
				glLoadMatrixf(modelview);

				for (i=0; i<=2; i++)
				{
					glBegin (GL_QUADS);
						glTexCoord2f (0,1);	glVertex2f ( - .30+ln2, +.30-ln2);
						glTexCoord2f (0,0);	glVertex2f ( - .30+ln2, -.30+ln2);
						glTexCoord2f (1,0);	glVertex2f ( + .30-ln2, -.30+ln2);
						glTexCoord2f (1,1);	glVertex2f ( + .30-ln2, +.30-ln2);
					glEnd ();
				}
				glBindTexture (GL_TEXTURE_2D, texture[44]);
				for (i=0; i<=0; i++)
				{
					glBegin (GL_QUADS);
						glTexCoord2f (0,1);	glVertex2f ( - .70, +.70);
						glTexCoord2f (0,0);	glVertex2f ( - .70, -.70);
						glTexCoord2f (1,0);	glVertex2f ( + .70, -.70);
						glTexCoord2f (1,1);	glVertex2f ( + .70, +.70);
					glEnd ();
				}
			glPopMatrix();

			// réacteur droite (avec billboarding)
			glPushMatrix();
				glBindTexture (GL_TEXTURE_2D, texture[29]);
				if (!game_3d_lines)
					glTranslatef(+ 0.23, 0.03, 0.33); // +0.23 0.03 0.35
				else glTranslatef(+ 0.23, 0.03, 0.35);

				// get the current modelview matrix
				glGetFloatv(GL_MODELVIEW_MATRIX , modelview);

				// undo all rotations (beware all scaling is lost as well)
				for( k=0; k<3; k++ ) 
					for( l=0; l<3; l++ ) {
						if ( k==l )
							modelview[k*4+l] = 1.0;
						else
							modelview[k*4+l] = 0.0;
					}

				// set the modelview with no rotations and scaling
				glLoadMatrixf(modelview);

				for (i=0; i<=2; i++)
				{
					glBegin (GL_QUADS);
						glTexCoord2f (0,1);	glVertex2f ( - .30+ln2, +.30-ln2);
						glTexCoord2f (0,0);	glVertex2f ( - .30+ln2, -.30+ln2);
						glTexCoord2f (1,0);	glVertex2f ( + .30-ln2, -.30+ln2);
						glTexCoord2f (1,1);	glVertex2f ( + .30-ln2, +.30-ln2);
					glEnd ();
				}
				glBindTexture (GL_TEXTURE_2D, texture[44]);
				glBegin (GL_QUADS);
					glTexCoord2f (0,1);	glVertex2f ( - .70, +.70);
					glTexCoord2f (0,0);	glVertex2f ( - .70, -.70);
					glTexCoord2f (1,0);	glVertex2f ( + .70, -.70);
					glTexCoord2f (1,1);	glVertex2f ( + .70, +.70);
				glEnd ();
			glPopMatrix();

		glPopMatrix();

		// réacteur du laser gauche
		if (player[0].laser_left)
		{
			glBindTexture (GL_TEXTURE_2D, texture[29]);
			glColor4f (0.120f, 0.340f, 0.953f, alpha);					// bleu
			glPushMatrix();

				// on va à la position du laser
				m3.pos.x = player[0].x - 0.9;	
				glTranslatef(m3.pos.x, m3.pos.y, m3.pos.z);
				glRotatef(m3.rot.x, 1.0f, 0.0f, 0.0f);
				glRotatef(m3.rot.y, 0.0f, 1.0f, 0.0f);
				glRotatef(m3.rot.z, 0.0f, 0.0f, 1.0f);

				glTranslatef( 0.0, 0.03, 0.12);

				// get the current modelview matrix
				glGetFloatv(GL_MODELVIEW_MATRIX , modelview);

				// undo all rotations (beware all scaling is lost as well)
				for( k=0; k<3; k++ ) 
					for( l=0; l<3; l++ ) {
						if ( k==l )
							modelview[k*4+l] = 1.0;
						else
							modelview[k*4+l] = 0.0;
					}

				// set the modelview with no rotations and scaling
				glLoadMatrixf(modelview);

				for (i=0; i<=2; i++)
				{
					glBegin (GL_QUADS);
						glTexCoord2f (0,1);	glVertex2f ( - .30, +.30);
						glTexCoord2f (0,0);	glVertex2f ( - .30, -.30);
						glTexCoord2f (1,0);	glVertex2f ( + .30, -.30);
						glTexCoord2f (1,1);	glVertex2f ( + .30, +.30);
					glEnd ();
				}
				glBindTexture (GL_TEXTURE_2D, texture[44]);
				glBegin (GL_QUADS);
					glTexCoord2f (0,1);	glVertex2f ( - .60-ln, +.60+ln);
					glTexCoord2f (0,0);	glVertex2f ( - .60-ln, -.60-ln);
					glTexCoord2f (1,0);	glVertex2f ( + .60+ln, -.60-ln);
					glTexCoord2f (1,1);	glVertex2f ( + .60+ln, +.60+ln);
				glEnd ();

			glPopMatrix();
		}

		// réacteur du laser droit
		if (player[0].laser_right)
		{
			glBindTexture (GL_TEXTURE_2D, texture[29]);
			glColor4f (0.120f, 0.340f, 0.953f, alpha);					// bleu
			glPushMatrix();

				// on va à la position du laser
				m3.pos.x = player[0].x + 0.9;	
				glTranslatef(m3.pos.x, m3.pos.y, m3.pos.z);
				glRotatef(m3.rot.x, 1.0f, 0.0f, 0.0f);
				glRotatef(m3.rot.y, 0.0f, 1.0f, 0.0f);
				glRotatef(m3.rot.z, 0.0f, 0.0f, 1.0f);

				glTranslatef( 0.0, 0.03, 0.12);

				// get the current modelview matrix
				glGetFloatv(GL_MODELVIEW_MATRIX , modelview);

				// undo all rotations (beware all scaling is lost as well)
				for( k=0; k<3; k++ ) 
					for( l=0; l<3; l++ ) {
						if ( k==l )
							modelview[k*4+l] = 1.0;
						else
							modelview[k*4+l] = 0.0;
					}

				// set the modelview with no rotations and scaling
				glLoadMatrixf(modelview);

				for (i=0; i<=2; i++)
				{
					glBegin (GL_QUADS);
						glTexCoord2f (0,1);	glVertex2f ( - .30, +.30);
						glTexCoord2f (0,0);	glVertex2f ( - .30, -.30);
						glTexCoord2f (1,0);	glVertex2f ( + .30, -.30);
						glTexCoord2f (1,1);	glVertex2f ( + .30, +.30);
					glEnd ();
				}
				glBindTexture (GL_TEXTURE_2D, texture[44]);
				glBegin (GL_QUADS);
					glTexCoord2f (0,1);	glVertex2f ( - .60-ln, +.60+ln);
					glTexCoord2f (0,0);	glVertex2f ( - .60-ln, -.60-ln);
					glTexCoord2f (1,0);	glVertex2f ( + .60+ln, -.60-ln);
					glTexCoord2f (1,1);	glVertex2f ( + .60+ln, +.60+ln);
				glEnd ();

			glPopMatrix();
		}


		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		//glDisable(GL_BLEND);

	}
}

//////////////////////////////////////////////////////////////////////////////////////
//
// Affiche le supership
//
//
void display_supership()
{
	// GAME 2D
	if (!game_3d)
	{
		glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
		glBindTexture (GL_TEXTURE_2D, texture[4]);

		// ALPHA TEST + BLEND
		glAlphaFunc(GL_GREATER, 0.4f);								// for TGA alpha test
		glEnable(GL_ALPHA_TEST);									// for TGA alpha test
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			// for anti-aliasing

		glBegin (GL_QUADS);
			glTexCoord2f (0,1);	glVertex2f (supership[0].x - .5, supership[0].y +.3);
			glTexCoord2f (0,0);	glVertex2f (supership[0].x - .5, supership[0].y -.3);
			glTexCoord2f (1,0);	glVertex2f (supership[0].x + .5, supership[0].y -.3);
			glTexCoord2f (1,1);	glVertex2f (supership[0].x + .5, supership[0].y +.3);
		glEnd ();

		// BACK TO NON ALPHA TEST + PREVIOUS BLEND
		glDisable(GL_ALPHA_TEST);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	}

	// GAME 3D
	else
	{
		glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
		m2.pos.x = supership[0].x;
		m2.pos.y = supership[0].y;
		m2.pos.z = supership[0].z;

		m2.rot.x =  90.0;
		m2.scale = 0.005;
		
		// supership touché
		if (supership[0].hit_duration < 500)
		{
			supership[0].hit_duration -= 10 * ms;
			if (supership[0].hit_duration <= 0)
				supership[0].hit_duration = 500;

			m2.Draw(-1, GL_TRIANGLES);	// supership non texturé, blanc
		}
		
		// supership normal
		else
			if (game_level == 6)
				//m2.Draw(design_tex, GL_TRIANGLES);
				m2.Draw(16, GL_TRIANGLES);
			else
				m2.Draw();
	}
}

//////////////////////////////////////////////////////////////////////////////////////
//
// Affiche un tir (du player ou d'un ship)
//
//
void display_shoot(int k, bool viewport2)	// tir n°k, dans la console (true) ou l'écran principal (false)
{
	//glAlphaFunc(GL_GREATER, 0.0f);
	//glDisable(GL_ALPHA_TEST);
//	glEnable(GL_ALPHA_TEST);
//	glAlphaFunc(GL_GREATER, 0.9f);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glEnable(GL_BLEND);

//	glAlphaFunc(GL_GREATER, 0.3f);								// for TGA alpha test
//	glEnable(GL_ALPHA_TEST);									// for TGA alpha test
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			// for anti-aliasing

	// GAME 2D
	if (!game_3d)
	{
		if (tir[k].type == 0)                 // tir du joueur
		{
			glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
			glBindTexture (GL_TEXTURE_2D, texture[8]);
			glBegin (GL_QUADS);
				glTexCoord2f (0,1);	glVertex2f (tir[k].x - .05, tir[k].y +.3);
				glTexCoord2f (0,0);	glVertex2f (tir[k].x - .05, tir[k].y -.4);
				glTexCoord2f (1,0);	glVertex2f (tir[k].x + .05, tir[k].y -.4);
				glTexCoord2f (1,1);	glVertex2f (tir[k].x + .05, tir[k].y +.3);
			glEnd ();
		}
		else if (tir[k].type == 10)            // tir d'un ship
		{
			glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
			glBindTexture (GL_TEXTURE_2D, texture[6]);
			float top = 0;
			if (!game_3d)
				top = 0.3;
			else
				top = 0.6;
			glBegin (GL_QUADS);
				glTexCoord2f (0,1);	glVertex2f (tir[k].x - .033, tir[k].y + top);
				glTexCoord2f (0,0);	glVertex2f (tir[k].x - .033, tir[k].y -.3);
				glTexCoord2f (1,0);	glVertex2f (tir[k].x + .033, tir[k].y -.3);
				glTexCoord2f (1,1);	glVertex2f (tir[k].x + .033, tir[k].y + top);
			glEnd ();
		}
	}

	// GAME 3D
	else
		if (tir[k].type < 5)				// tir du joueur
		{
			if (tir[k].type == 0)				// tir 3D double
			{
				// alpha test + blend
				//glEnable(GL_ALPHA_TEST);									// alpha test
				//glAlphaFunc(GL_GREATER, 0.9f);								// alpha test
				//glEnable(GL_BLEND);											// blending
				//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			// for anti-aliasing
				//glBlendFunc(GL_SRC_ALPHA,GL_ONE);
				//glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE);
				//glDisable(GL_DEPTH_TEST);
				//glBlendFunc(GL_SRC_ALPHA,GL_ONE);

				//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
				//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//normal
				//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
				 //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
//				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
				
				//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
				//glTexEnvi(GL_TEXTURE_ENV,GL_COMBINE_RGB_EXT, GL_INTERPOLATE);

				glColor4f (1.0f, 1.0f, 1.0f, 1.0f); //blanc
				//glColor4f (1.0f, 0.0f, 0.0f, 1.0f); //rouge
				//if (game_3d_lines)
					glBindTexture (GL_TEXTURE_2D, texture[45]);
				//else glBindTexture (GL_TEXTURE_2D, texture[45]);
				int nb = 1;
				if (!game_3d_lines)
					nb = 2;
				for (int i=0; i<=nb; i++)
				{
					glBegin (GL_QUADS);
						glTexCoord2f (0,1);	glVertex2f (tir[k].x - .15, tir[k].y +.1);
						glTexCoord2f (0,0);	glVertex2f (tir[k].x - .15, tir[k].y -1.0);
						glTexCoord2f (1,0);	glVertex2f (tir[k].x + .15, tir[k].y -1.0);
						glTexCoord2f (1,1);	glVertex2f (tir[k].x + .15, tir[k].y +.1);
					glEnd ();
				}
				// retour aux anciens paramètres (alpha test et blending)
				//glDisable(GL_ALPHA_TEST);
				//glEnable(GL_DEPTH_TEST);
				//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
//				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//normal

/*				m4.scale = 0.0003f;//0.0003

				m4.rot.x = 90.0f;
//				m4.rot.y = 45.0f;
				m4.rot.z += 1.0f;

				m4.pos.x = tir[k].x;
				m4.pos.y = tir[k].y;
				m4.pos.z = 0;				// amplitude : de -1/var à 1/var

			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);

				glDisable(GL_BLEND);
				glEnable(GL_DEPTH_TEST);
				glFrontFace(GL_CW);
				m4.Draw();

			glDisable(GL_LIGHTING);
			glDisable(GL_LIGHT0);
*/
			}
			else if (tir[k].type == 1)	// tir 3D "chevron"
			{
				glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
				if (game_3d_lines)
					glBindTexture (GL_TEXTURE_2D, texture[32]);
				else glBindTexture (GL_TEXTURE_2D, texture[31]);
				int nb = 1;
				if (!game_3d_lines)
					nb = 2;
				for (int i=0; i<=nb; i++)
				{
					glBegin (GL_QUADS);
						glTexCoord2f (0,1);	glVertex2f (tir[k].x - .15, tir[k].y +.1);
						glTexCoord2f (0,0);	glVertex2f (tir[k].x - .15, tir[k].y -1.0);
						glTexCoord2f (1,0);	glVertex2f (tir[k].x + .15, tir[k].y -1.0);
						glTexCoord2f (1,1);	glVertex2f (tir[k].x + .15, tir[k].y +.1);
					glEnd ();
				}
			}
			else if (tir[k].type == 2)	// tir 3D en particules
				//display_shoot3d(k, viewport2);
				return;
		}

		else if (tir[k].type == 10)            // tir d'un ship
		{
			glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
			glBindTexture (GL_TEXTURE_2D, texture[6]);
			glBegin (GL_QUADS);
				glTexCoord2f (0,1);	glVertex2f (tir[k].x - .03, tir[k].y +.3);
				glTexCoord2f (0,0);	glVertex2f (tir[k].x - .03, tir[k].y -.3);
				glTexCoord2f (1,0);	glVertex2f (tir[k].x + .03, tir[k].y -.3);
				glTexCoord2f (1,1);	glVertex2f (tir[k].x + .03, tir[k].y +.3);
			glEnd ();
		}
		else if (tir[k].type == 5)            // tir d'un laser du joueur
		{
			glBindTexture (GL_TEXTURE_2D, texture[6]);
			int nb = 0;
			if (game_3d_lines)
			{
				nb = 1;
				glColor4f (1.0f, 1.0f, 1.0f, 0.7f);
			}
			else glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
			for (int i=0; i<=nb; i++)
			{
				glBegin (GL_QUADS);
					glTexCoord2f (0,1);	glVertex2f (tir[k].x - .03, tir[k].y +.3);
					glTexCoord2f (0,0);	glVertex2f (tir[k].x - .03, tir[k].y -.3);
					glTexCoord2f (1,0);	glVertex2f (tir[k].x + .03, tir[k].y -.3);
					glTexCoord2f (1,1);	glVertex2f (tir[k].x + .03, tir[k].y +.3);
				glEnd ();
			}
		}
}

//////////////////////////////////////////////////////////////////////////////////////
//
// Affiche le bonus (mode 3D)
//
//
void display_bonus_3d()
{

	if ((bonus[2].active == true) && (bonus[2].explosion == false) && game_3d)
	{
		static float alpha = 0.9f;
		static bool decrease = true;
		if (decrease)
			alpha -= 0.0005 * ms;
		else alpha += 0.0005 * ms;
		if (alpha <= 0)
			decrease = false;
		else if (alpha >= 0.9)
			decrease = true;

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		
		glColor4f (1.0f, 1.0f, 1.0f, alpha);
		if (bonus[2].type == 1)
			glBindTexture (GL_TEXTURE_2D, texture[47]);
		else if (bonus[2].type == 2)
			glBindTexture (GL_TEXTURE_2D, texture[46]);
		int nb = 2;
		if (game_3d_lines)
			nb = 2;
		for (int i=0; i<=nb; i++)
		{
			glBegin (GL_QUADS);
				glTexCoord2f (0,1);	glVertex3f (bonus[2].x - 1.6f, bonus[2].y + 0.4f, .05f);
				glTexCoord2f (1,1);	glVertex3f (bonus[2].x + 1.6f, bonus[2].y + 0.4f, .05f);
				glTexCoord2f (1,0);	glVertex3f (bonus[2].x + 1.6f, bonus[2].y - 0.4f, .05f);
				glTexCoord2f (0,0);	glVertex3f (bonus[2].x - 1.6f, bonus[2].y - 0.4f, .05f);
			glEnd ();
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////
//
// Affiche les bonus
//
//
void display_bonus_2d_to_3d()
{

	// on affiche le powerup
	if ((bonus[1].active == true) && (bonus[1].explosion == false) && !game_3d)
	{

	glPushMatrix();
		static float angle_bonus = 0;
		angle_bonus += ms10/20;

		//bonus[1].x = 0; //2
		//bonus[1].y = 0;
		
		glTranslatef(bonus[1].x, bonus[1].y, .05);
//		glRotatef(angle_bonus, 0.0, 0.0, 1.0);
		
		glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindTexture (GL_TEXTURE_2D, texture[42]);//42 44 50
				
		float tx = 0.0;
		//for(int i=0;i<=1;i++)
		{
		glBegin (GL_QUADS);
/*			glTexCoord2f (0,1);	glVertex3f (bonus[1].x-tx - 0.4f, bonus[1].y+tx + 0.4f, .05f);
			glTexCoord2f (1,1);	glVertex3f (bonus[1].x+tx + 0.4f, bonus[1].y+tx + 0.4f, .05f);
			glTexCoord2f (1,0);	glVertex3f (bonus[1].x+tx + 0.4f, bonus[1].y-tx - 0.4f, .05f);
			glTexCoord2f (0,0);	glVertex3f (bonus[1].x-tx - 0.4f, bonus[1].y-tx - 0.4f, .05f);
*/
			glTexCoord2f (0,1);	glVertex3f (-tx - 0.4f, +tx + 0.4f, .05f);
			glTexCoord2f (1,1);	glVertex3f (+tx + 0.4f, +tx + 0.4f, .05f);
			glTexCoord2f (1,0);	glVertex3f (+tx + 0.4f, -tx - 0.4f, .05f);
			glTexCoord2f (0,0);	glVertex3f (-tx - 0.4f, -tx - 0.4f, .05f);
		glEnd ();
		}

	glPopMatrix();
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	}
}

void display_ring_2d_to_3d()
{
	// le powerup est pris => on affiche le cercle, la scène tremble, etc.
	if ((bonus[1].active == true) && (bonus[1].explosion == true))
	
	{
		static bool sound_cross_passed = false;

		// on gère le tremblement du vaisseau du joueur et du cercle
		static float player_yi_acc = 0.05;

		if (bonus[1].explosion_life > 2)
			player[0].yi_speed += player_yi_acc;
		else player[0].yi_speed -= player_yi_acc;

		if (player[0].yi_speed >= 0.6)
			player[0].yi_speed = 0.6;

		else if (player[0].yi_speed <= -0.4 && game_3d)
			player[0].yi_speed = -0.4;

		player[0].yi_cos += player[0].yi_speed;

		if (player[0].yi_cos >= 3.14 || player[0].yi_cos <= -3.14)
			player[0].yi_speed *= -1;

		if (bonus[1].explosion_life >= 1)
			player[0].yi = cosf(player[0].yi_cos * 50) / 40;	// cosf(a * b) / c - d : c=amplitude, d=position haut-bas
		else player[0].yi = cosf(player[0].yi_cos * 50) / 60;	// l'amplitude est réduite vers la fin

		m.pos.y = player[0].y + player[0].yi;
		m3.pos.y = m.pos.y;

		// Affichage du cercle
		static float x, y, z;
		
		x = player[0].x;
		y = player[0].y + player[0].yi;
		z = player[0].z;

		float tx = 0;

		// cercle game 2D
		if (!game_3d)
		{
			// ring
			glPushMatrix();
				tx = 0.05; //0.1
				glTranslatef(x, y, 0.01);
				glBindTexture (GL_TEXTURE_2D, texture[43]); //43(gif) 51(png)
				glColor4f (1.0f, 1.0f, 1.0f, 0.6f);
				{
				glBegin (GL_QUADS);
					glTexCoord2f (0,1);glVertex2f (-tx - 0.5f, +tx + 0.5f);
					glTexCoord2f (1,1);glVertex2f (+tx + 0.5f, +tx + 0.5f);
					glTexCoord2f (1,0);glVertex2f (+tx + 0.5f, -tx - 0.5f);
					glTexCoord2f (0,0);glVertex2f (-tx - 0.5f, -tx - 0.5f);
				glEnd ();
				}
			glPopMatrix();
		}

		// cercle game 3D
		else
		{
/*			// sphere
			glPushMatrix();
				glEnable(GL_CULL_FACE);
				glFrontFace(GL_CCW);
				glTranslatef(m.pos.x , m.pos.y, m.pos.z);
				glTranslatef(0, 0.2, 0);
				glRotatef(90, 0, 1, 0);
				static float angle_sphere = 0;
				angle_sphere += 1;
				glRotatef(angle_sphere, 1, 0, 0);
				glBindTexture (GL_TEXTURE_2D, texture[53]);
				//glColor4f (1.0f, 1.0f, 1.0f, bonus[1].explosion_life);
				glColor4f (1.0f, 1.0f, 1.0f, bonus[1].explosion_life > 0.2 ? 0.2 : bonus[1].explosion_life);
				//gluSphere(quadratic,0.73f,2,32); //0.78 0.75
				gluSphere(quadratic,0.78f,32,32);//0.71
			glPopMatrix();
*/
			// ring
			static float angle_ring = 0;
			if (bonus[1].explosion_life > 1.5)
				angle_ring += ms10*1.5; // /1.5
			else if (bonus[1].explosion_life > 1)
				//angle_ring += ms10 * bonus[1].explosion_life/1.5;
				angle_ring += ms10*1.0; // /5
			else if (bonus[1].explosion_life > 0.5)
				angle_ring += ms10 /5; // /10
			else
				angle_ring += ms10 /8;// /15
			
			if (angle_ring > 360)
				angle_ring = fmod(angle_ring, 360);
/*			for (int i=0; i<=5; i++)
			{
			glPushMatrix();
				glDisable(GL_DEPTH_TEST);
				tx = -0.05; //0.08
				glDisable(GL_CULL_FACE);
				glTranslatef(m.pos.x , m.pos.y, m.pos.z);
				glTranslatef(0, 0.2, 0);
				//glRotatef(angle_ring*3, 1, 1.2, 1.3);
				//glRotatef(90, 0, 0, 1);
				if (i < 10)
					glRotatef(angle_ring+i*30, 1, 0, 0); //60
					//glRotatef(angle_ring, 1, 1, 0);
				else if (i == 1)
					//glRotatef(angle_ring/2, -1.1, -1.1, -1.1);
					glRotatef(angle_ring+45, -1, -1, 0);
				else if (i == 2)
					//glRotatef(angle_ring/3, -1.2, 1, 1);
					glRotatef(angle_ring+90, 1, -1, 0);
				else if (i == 3)
					//glRotatef(angle_ring/4, 1, -1.1, -1.1);
					glRotatef(angle_ring+135, -1, 1, 0);

				glBindTexture (GL_TEXTURE_2D, texture[52]);//52 43
				//glColor4f (1.0f, 1.0f, 1.0f, bonus[1].explosion_life*1);
				//glColor4f (1.0f, 1.0f, 1.0f, bonus[1].explosion_life > 0.15 ? 0.15 : bonus[1].explosion_life);
				if (bonus[1].explosion_life > 2)
					glColor4f (1.0f, 1.0f, 1.0f, 0.3);
				else
					glColor4f (1.0f, 1.0f, 1.0f, bonus[1].explosion_life *0.15);
				{
				glBegin (GL_QUADS);
					glTexCoord2f (0,1);glVertex2f (-tx - 1.0f, +tx + 1.0f);//1.2
					glTexCoord2f (1,1);glVertex2f (+tx + 1.0f, +tx + 1.0f);//1.2
					glTexCoord2f (1,0);glVertex2f (+tx + 1.0f, -tx - 1.0f);//0.8
					glTexCoord2f (0,0);glVertex2f (-tx - 1.0f, -tx - 1.0f);//0.8
				glEnd ();
				glEnable(GL_DEPTH_TEST);
				}
			glPopMatrix();
			}*/

			// ring
			glPushMatrix();
				glDisable(GL_DEPTH_TEST);
				tx = 0.03; //0.08
				glDisable(GL_CULL_FACE);
				glTranslatef(m.pos.x , m.pos.y, m.pos.z);
				glTranslatef(0, 0.2, 0);

				glBindTexture (GL_TEXTURE_2D, texture[52]);//52 43
				if (bonus[1].explosion_life > 2)
					glColor4f (1.0f, 1.0f, 1.0f, 0.3);
				else
					glColor4f (1.0f, 1.0f, 1.0f, bonus[1].explosion_life *0.15);
				//glColor4f (1.0f, 1.0f, 1.0f, bonus[1].explosion_life);
				glColor4f (1.0f, 1.0f, 1.0f, bonus[1].explosion_life > 0.4 ? 0.4 : bonus[1].explosion_life);
				{
				glBegin (GL_QUADS);
					glTexCoord2f (0,1);glVertex2f (-tx - 1.0f, +tx + 1.0f);//1.2
					glTexCoord2f (1,1);glVertex2f (+tx + 1.0f, +tx + 1.0f);//1.2
					glTexCoord2f (1,0);glVertex2f (+tx + 1.0f, -tx - 1.0f);//0.8
					glTexCoord2f (0,0);glVertex2f (-tx - 1.0f, -tx - 1.0f);//0.8
				glEnd ();
				glEnable(GL_DEPTH_TEST);
				}
			glPopMatrix();

			// sphere
			// Anti-aliasing (DEBUT)
/*			if (game_antialiasing_active)
				glEnable(GL_MULTISAMPLE_ARB);
			glPushMatrix();
				glEnable(GL_CULL_FACE);
				glFrontFace(GL_CCW);
				glTranslatef(m.pos.x , m.pos.y, m.pos.z);
				glTranslatef(0, 0.2, 0);
				glRotatef(90, 0, 1, 0);
				static float angle_sphere = 0;
				angle_sphere += 1;
				glRotatef(angle_sphere, 1, 0, 0);
				//glRotatef(angle_ring, 1, 0, 0);
				glBindTexture (GL_TEXTURE_2D, texture[53]);
				glColor4f (1.0f, 1.0f, 1.0f, bonus[1].explosion_life);
				//glColor4f (1.0f, 1.0f, 1.0f, bonus[1].explosion_life > 0.4 ? 0.4 : bonus[1].explosion_life);
				gluSphere(quadratic,0.78f,32,32); //0.78 0.75
			glPopMatrix();
				glDisable(GL_CULL_FACE);
			// Anti-aliasing (FIN)
			if (game_antialiasing_active)
				glDisable(GL_MULTISAMPLE_ARB);*/

		}

		// on réinitialise à la fin
		if (bonus[1].explosion_life == 0)
		{
			bonus[1].active		= false;
			bonus[1].explosion	= false;
			player[0].yi		= 0.00;
			player[0].yi_speed	= 0.01;
			player[0].yi_cos	= 0;

			sound_cross_passed = false;
		}

		// vers la moitié on passe en game 3D
		else if (bonus[1].explosion_life < 2.0 && !game_3d) //1.5 2.5 =>2.0 (à voir, et changer sound_cross)
		{
			try
			{
				game_3d = true;
				game_3d_lines = false;
				game_level = 1;
				//m.pos.x = player[0].x;									// spécifie la position du player 3D car sinon l'anneau serait affiché au centre lors du passage dans la prochaine boucle.
				player[0].movement = 0;
			}
			catch(...)
			{
				ERR("erreur moitie 3D.01 (passage 3D)");
			}
		}

		// la scène tremble
/*		if (bonus[1].explosion_life <= 3.5 && bonus[1].explosion_life >=0.5)
		{
			//glRotatef(-bonus[1].shake2, -0.3f, 0.5f, 1.0f);

			if (bonus[1].explosion_life >=1.5)
			{
				bonus[1].shake2 += bonus[1].shake1;
				bonus[1].shake1 *= 1.05;
			}
			else
			{
				bonus[1].shake2 -= bonus[1].shake1;
				bonus[1].shake1 *= 0.99;
			}

			if (bonus[1].shake1 >= 1.2)
				bonus[1].shake1 = 1.2;

			else if (bonus[1].shake1 <= 0.5)
				bonus[1].shake1 = 0.5;

			bonus[1].shake2 *= rand()%4*0.1 - 0.2;

			//glRotatef(bonus[1].shake2, -0.3f, 0.5f, 1.0f);
		}
*/
		// on décrémente la vie de "l'explosion" du bonus
#ifdef DEBUG_PASSAGE_3D
		if (bonus[1].explosion_life - 0.01*ms <= 0)			// empêche de dépasser la limite de 0
			bonus[1].explosion_life = 0;						// on cale à 0
		else
			bonus[1].explosion_life -= 0.01*ms;
#else
		if (bonus[1].explosion_life - 0.001*ms <= 0)			// empêche de dépasser la limite de 0
			bonus[1].explosion_life = 0;						// on cale à 0
		else
			bonus[1].explosion_life -= 0.001*ms;
#endif
		// son CROSS (2D -> 3D)
		if (!sound_cross_passed && bonus[1].explosion_life <= 2.4 && bonus[1].explosion_life > 0.0)//...3.0...
		{
			sound_cross_passed = true;
			int int_sound = FSOUND_PlaySoundEx(FSOUND_FREE,sound_cross, dspUnitSounds, false);
			FSOUND_SetVolume(int_sound,255);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////
//
// Affiche les bunkers
//
//
void display_bunkers()
{
	// GAME 2D
	if (!game_3d)
	{
		glDisable(GL_BLEND);

		glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
		glEnable (GL_TEXTURE_2D);
		glBindTexture (GL_TEXTURE_2D, texture[27]);

		for (int i=0; i<= id_brick -1; i++) 
		{
			if (brick[i].active)
			{
				glPushMatrix();
				glTranslatef ( brick[i].x, brick[i].y, brick[i].z );

				glBegin (GL_QUADS);					// callList essayé => fps / 2. à voir plus en détails...
					glTexCoord2f (0,1);	glVertex3f (-0.05f, -0.05f, .05f);  // -.05 pour éviter le z-fighting
					glTexCoord2f (0,0);	glVertex3f (-0.05f,  0.05f, .05f);
					glTexCoord2f (1,0);	glVertex3f ( 0.05f,  0.05f, .05f);
					glTexCoord2f (1,1);	glVertex3f ( 0.05f, -0.05f, .05f);
				glEnd ();

				glPopMatrix();
			}
		}
		glEnable(GL_BLEND);
	}

	// GAME 3D
	else
	{
		glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
		glDisable(GL_BLEND);
		glEnable (GL_TEXTURE_2D);
		if (game_level == 6)
		{
			//if (design_tex != -2 && design_tex != -1)
				//glBindTexture (GL_TEXTURE_2D, texture[design_tex]);
				glBindTexture (GL_TEXTURE_2D, texture[28]);
		}
		else
			glBindTexture (GL_TEXTURE_2D, texture[30]);
		for (int i=0; i<= id_brick -1; i++) 
		{
			if (brick[i].active)
			{
				// correct a pixel bug which display a brick anywhere - needs to be fixed (à voir)
				if (brick[i].z != 0.05f || brick[i].y > -3.00f)
				{
					brick[i].active = false;
					continue;
				}

				glBegin (GL_QUADS);
					glTexCoord2f (0,1);	glVertex3f (brick[i].x-0.05f, brick[i].y-0.05f, brick[i].z+.05f);
					glTexCoord2f (0,0);	glVertex3f (brick[i].x-0.05f, brick[i].y+0.05f, brick[i].z+.05f);
					glTexCoord2f (1,0);	glVertex3f (brick[i].x+0.05f, brick[i].y+0.05f, brick[i].z+.05f);
					glTexCoord2f (1,1);	glVertex3f (brick[i].x+0.05f, brick[i].y-0.05f, brick[i].z+.05f);
				glEnd ();
			}
		}
		glEnable(GL_BLEND);
	}
}

//////////////////////////////////////////////////////////////////////////////////////
//
// Affiche le score, le hiscore, les vies
//
//
void display_score_hiscore_lives()
{
	int i;

	// SCORE
	glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
	glBindTexture (GL_TEXTURE_2D, texture[24]);
	glBegin (GL_QUADS);
		glTexCoord2f (0,1);	glVertex2f ((-5.0f - 1.0f), (5.7f +.3f));
		glTexCoord2f (0,0);	glVertex2f ((-5.0f - 1.0f), (5.7f -.3f));
		glTexCoord2f (1,0);	glVertex2f ((-5.0f + 1.0f), (5.7f -.3f));
		glTexCoord2f (1,1);	glVertex2f ((-5.0f + 1.0f), (5.7f +.3f));
	glEnd ();

	char str_chif[7] = "";

	_itoa(score, str_chif, 10);        // integer to string, en base 10

	int long_chif = strlen(str_chif);
	int long_0 = 6 - long_chif;

	for (i=0; i<= long_0 - 1; i++)
	{
		glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
		glBindTexture (GL_TEXTURE_2D, texture[14]);
		glBegin (GL_QUADS);
			glTexCoord2f (0,1);	glVertex2f (-6.3f +i/1.9f - .3f, 5.0f +.3f);
			glTexCoord2f (0,0);	glVertex2f (-6.3f +i/1.9f - .3f, 5.0f -.3f);
			glTexCoord2f (1,0);	glVertex2f (-6.3f +i/1.9f + .3f, 5.0f -.3f);
			glTexCoord2f (1,1);	glVertex2f (-6.3f +i/1.9f + .3f, 5.0f +.3f);
		glEnd ();
	}
	for (i=long_0 ; i<= 5 ; i++)
	{
		int chif_texture = (int)(str_chif[i - long_0]) - 48; // car "0" = 48 dans la table ASCII et le cast (char *) veut pas marcher pour atoi()

		glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
		glBindTexture (GL_TEXTURE_2D, texture[chif_texture+14]);
		glBegin (GL_QUADS);
			glTexCoord2f (0,1);	glVertex2f (-6.3f +i/1.9f - .3f, 5.0f +.3f);
			glTexCoord2f (0,0);	glVertex2f (-6.3f +i/1.9f - .3f, 5.0f -.3f);
			glTexCoord2f (1,0);	glVertex2f (-6.3f +i/1.9f + .3f, 5.0f -.3f);
			glTexCoord2f (1,1);	glVertex2f (-6.3f +i/1.9f + .3f, 5.0f +.3f);
		glEnd ();
	}

	// HISCORE
	glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
	glBindTexture (GL_TEXTURE_2D, texture[25]);
	glBegin (GL_QUADS);
		glTexCoord2f (0,1);	glVertex2f (-0.1f - 2.0f, 5.7f +.3f);
		glTexCoord2f (0,0);	glVertex2f (-0.1f - 2.0f, 5.7f -.3f);
		glTexCoord2f (1,0);	glVertex2f (-0.1f + 2.0f, 5.7f -.3f);
		glTexCoord2f (1,1);	glVertex2f (-0.1f + 2.0f, 5.7f +.3f);
	glEnd ();

	if (hiscore > score)
		_itoa(hiscore, str_chif, 10);			// integer to string, en base 10
	else
		_itoa(score, str_chif, 10);				// integer to string, en base 10

	long_chif = strlen(str_chif);
	long_0 = 6 - long_chif;


	for (i=0; i<= long_0 - 1; i++)
	{
		glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
		glBindTexture (GL_TEXTURE_2D, texture[14]);
		glBegin (GL_QUADS);
			glTexCoord2f (0,1);	glVertex2f (-1.5f +i/1.9f - .3f, 5.0f +.3f);
			glTexCoord2f (0,0);	glVertex2f (-1.5f +i/1.9f - .3f, 5.0f -.3f);
			glTexCoord2f (1,0);	glVertex2f (-1.5f +i/1.9f + .3f, 5.0f -.3f);
			glTexCoord2f (1,1);	glVertex2f (-1.5f +i/1.9f + .3f, 5.0f +.3f);
		glEnd ();
	}
	for (i=long_0 ; i<= 5 ; i++)
	{
		int chif_texture = (int)(str_chif[i - long_0]) - 48; // car "0" = 48 dans la table ASCII et le casting (char *) veut pas marcher pour atoi()

		glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
		glBindTexture (GL_TEXTURE_2D, texture[chif_texture+14]);
		glBegin (GL_QUADS);
			glTexCoord2f (0,1);	glVertex2f (-1.5f +i/1.9f - .3f, 5.0f +.3f);
			glTexCoord2f (0,0);	glVertex2f (-1.5f +i/1.9f - .3f, 5.0f -.3f);
			glTexCoord2f (1,0);	glVertex2f (-1.5f +i/1.9f + .3f, 5.0f -.3f);
			glTexCoord2f (1,1);	glVertex2f (-1.5f +i/1.9f + .3f, 5.0f +.3f);
		glEnd ();
	}

	// LIVES
	glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
	glBindTexture (GL_TEXTURE_2D, texture[26]);
	glBegin (GL_QUADS);
		glTexCoord2f (0,1);	glVertex2f (4.55f - 1.0f, 5.7f +.3f);
		glTexCoord2f (0,0);	glVertex2f (4.55f - 1.0f, 5.7f -.3f);
		glTexCoord2f (1,0);	glVertex2f (4.55f + 1.0f, 5.7f -.3f);
		glTexCoord2f (1,1);	glVertex2f (4.55f + 1.0f, 5.7f +.3f);
	glEnd ();

	_itoa(lives, str_chif, 10);        // integer to string, en base 10

	long_chif = strlen(str_chif);
	long_0 = 3 - long_chif;

	for (i=0; i<= long_0 - 1; i++)
	{
		glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
		glBindTexture (GL_TEXTURE_2D, texture[14]);
		glBegin (GL_QUADS);
			glTexCoord2f (0,1);	glVertex2f (4.0f +i/1.9f - .3f, 5.0f +.3f);
			glTexCoord2f (0,0);	glVertex2f (4.0f +i/1.9f - .3f, 5.0f -.3f);
			glTexCoord2f (1,0);	glVertex2f (4.0f +i/1.9f + .3f, 5.0f -.3f);
			glTexCoord2f (1,1);	glVertex2f (4.0f +i/1.9f + .3f, 5.0f +.3f);
		glEnd ();
	}
	for (i=long_0 ; i<= 2 ; i++)
	{
		int chif_texture = (int)(str_chif[i - long_0]) - 48; // car "0" = 48 dans la table ASCII et le casting (char *) veut pas marcher pour atoi()

		glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
		glBindTexture (GL_TEXTURE_2D, texture[chif_texture+14]);
		glBegin (GL_QUADS);
			glTexCoord2f (0,1);	glVertex2f (4.0f +i/1.9f - .3f, 5.0f +.3f);
			glTexCoord2f (0,0);	glVertex2f (4.0f +i/1.9f - .3f, 5.0f -.3f);
			glTexCoord2f (1,0);	glVertex2f (4.0f +i/1.9f + .3f, 5.0f -.3f);
			glTexCoord2f (1,1);	glVertex2f (4.0f +i/1.9f + .3f, 5.0f +.3f);
		glEnd ();
	}
}

//////////////////////////////////////////////////////////////////////////////////////
//
// Affiche le nombre de vies en bas (chiffre + images du vaisseau du joueur)
//
//
void display_below_lives()
{
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	
	glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
	// chiffre 0, 1, 2 ou 3 -> id texture 14(0) à 17(3)
	glBindTexture (GL_TEXTURE_2D, texture[14 + lives]);
	glBegin (GL_QUADS);
		glTexCoord2f (0,1);	glVertex2f (-7.5f  - .4f, -6.9f +.25f);
		glTexCoord2f (0,0);	glVertex2f (-7.5f  - .4f, -6.9f -.25f);
		glTexCoord2f (1,0);	glVertex2f (-7.5f  + .4f, -6.9f -.25f);
		glTexCoord2f (1,1);	glVertex2f (-7.5f  + .4f, -6.9f +.25f);
	glEnd ();

	// ships verts en bas à droite
	for (int k=1; k <= lives ; k++)
	{
		glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
		if (!game_3d)
			glBindTexture (GL_TEXTURE_2D, texture[0]);
		else
			glBindTexture (GL_TEXTURE_2D, texture[40]);

		// ALPHA TEST + BLEND
		glAlphaFunc(GL_GREATER, 0.3f);								// for TGA alpha test
		glEnable(GL_ALPHA_TEST);									// for TGA alpha test
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			// for anti-aliasing

		glBegin (GL_QUADS);
			glTexCoord2f (0,1);	glVertex2f (-7.5f + k/1.3f - .3f, -6.9f +.25f);
			glTexCoord2f (0,0);	glVertex2f (-7.5f + k/1.3f - .3f, -6.9f -.25f);
			glTexCoord2f (1,0);	glVertex2f (-7.5f + k/1.3f + .3f, -6.9f -.25f);
			glTexCoord2f (1,1);	glVertex2f (-7.5f + k/1.3f + .3f, -6.9f +.25f);
		glEnd ();

		// BACK TO NON ALPHA TEST + PREVIOUS BLEND
		glDisable(GL_ALPHA_TEST);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	}
}

//////////////////////////////////////////////////////////////////////////////////////
//
// Affiche le fond du jeu
//
//
void display_background(bool viewport2)
{
	// GAME 2D
	if (game_3d == false)
	{
		glColor4f (1.0f, 1.0f, 1.0f, 1.0f);

		if (game_level  == 1) // fond étoilé
		{
			glBindTexture (GL_TEXTURE_2D, texture[7]);
			glBegin (GL_QUADS);
				glTexCoord2f (0,0);	glVertex3f (-12,  12, -.05f); // -.05 pour éviter le z-fighting
				glTexCoord2f (1,0);	glVertex3f (-12, -12, -.05f);
				glTexCoord2f (1,1);	glVertex3f ( 12, -12, -.05f);
				glTexCoord2f (0,1);	glVertex3f ( 12,  12, -.05f);
			glEnd ();
		}
		else if (game_level == 2) // planète
		{
			glPushMatrix();

			glTranslatef(0.0f, 0.0f, -5.0f);
			glBindTexture (GL_TEXTURE_2D, texture[28]);
			glBegin ( GL_QUADS );
				glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.00f, -5.0f,  -.05f);
				glTexCoord2f(1.0f, 0.0f); glVertex3f( 10.00f, -5.0f,  -.05f);
				glTexCoord2f(1.0f, 1.0f); glVertex3f( 10.00f,  5.0f,  -.05f);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-10.00f,  5.0f,  -.05f);
			glEnd();

			glPopMatrix();
		}
		else if (game_level == 3) // earth-moon
		{
			glPushMatrix();

			glTranslatef(0.0f, 0.0f, -15.0f);
			glBindTexture (GL_TEXTURE_2D, texture[37]);
			glBegin ( GL_QUADS );
				glTexCoord2f(0.0f, 0.0f); glVertex3f(-13.46f, -08.0f,  -.05f);
				glTexCoord2f(1.0f, 0.0f); glVertex3f( 13.46f, -08.0f,  -.05f);
				glTexCoord2f(1.0f, 1.0f); glVertex3f( 13.46f,  12.0f,  -.05f);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-13.46f,  12.0f,  -.05f);
			glEnd();

			glPopMatrix();
		}
		else if (game_level == 4) // galaxy1
		{
			glPushMatrix();

			glBindTexture (GL_TEXTURE_2D, texture[38]);
			glBegin ( GL_QUADS );
				glTexCoord2f(0.0f, 0.0f); glVertex3f(-08.00f, -07.36f,  -.05f);
				glTexCoord2f(1.0f, 0.0f); glVertex3f( 08.00f, -07.36f,  -.05f);
				glTexCoord2f(1.0f, 1.0f); glVertex3f( 08.00f,  07.36f,  -.05f);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-08.00f,  07.36f,  -.05f);
			glEnd();

			glPopMatrix();
		}
		else if (game_level == 5) // skybox
		{
			glBindTexture ( GL_TEXTURE_2D, texture[5] ); // 5 = vaisseau rouge
			glBegin ( GL_QUADS );
				// Back Face
				glTexCoord2f(1.0f, 0.0f); glVertex3f(-15.0f, -15.0f, -51.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex3f(-15.0f,  15.0f, -51.0f);
				glTexCoord2f(0.0f, 1.0f); glVertex3f( 15.0f,  15.0f, -51.0f);
				glTexCoord2f(0.0f, 0.0f); glVertex3f( 15.0f, -15.0f, -51.0f);
			glEnd();
			glBindTexture ( GL_TEXTURE_2D, texture[9] );
			glBegin ( GL_QUADS );
				// Top Face
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-15.0f,  15.0f, -51.0f);
				glTexCoord2f(0.0f, 0.0f); glVertex3f(-15.0f,  15.0f,  -21.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex3f( 15.0f,  15.0f,  -21.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex3f( 15.0f,  15.0f, -51.0f);
			glEnd();
			glBindTexture ( GL_TEXTURE_2D, texture[9] );
			glBegin ( GL_QUADS );
				// Bottom Face
				glTexCoord2f(1.0f, 1.0f); glVertex3f(-15.0f, -15.0f, -51.0f);
				glTexCoord2f(0.0f, 1.0f); glVertex3f( 15.0f, -15.0f, -51.0f);
				glTexCoord2f(0.0f, 0.0f); glVertex3f( 15.0f, -15.0f,  -21.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex3f(-15.0f, -15.0f,  -21.0f);
			glEnd();
			glBindTexture ( GL_TEXTURE_2D, texture[9] );
			glBegin ( GL_QUADS );
				// Right face
				glTexCoord2f(1.0f, 0.0f); glVertex3f( 15.0f, -15.0f, -51.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex3f( 15.0f,  15.0f, -51.0f);
				glTexCoord2f(0.0f, 1.0f); glVertex3f( 15.0f,  15.0f,  -21.0f);
				glTexCoord2f(0.0f, 0.0f); glVertex3f( 15.0f, -15.0f,  -21.0f);
			glEnd();
			glBindTexture ( GL_TEXTURE_2D, texture[9] );
			glBegin ( GL_QUADS );
				// Left Face
				glTexCoord2f(0.0f, 0.0f); glVertex3f(-15.0f, -15.0f, -51.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex3f(-15.0f, -15.0f,  -21.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex3f(-15.0f,  15.0f,  -21.0f);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-15.0f,  15.0f, -51.0f);
			glEnd();
		}

		// texture barre du bas
		glBindTexture (GL_TEXTURE_2D, texture[10]);
	}
	
	// GAME 3D (background)
	else
	{
//		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);
		glDisable(GL_BLEND);
		if (!viewport2)
			glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
		else
			glColor4f (1.0f, 1.0f, 1.0f, 0.7f);

		//if (game_level == 2)
		//	glEnable(GL_LIGHTING); // fond noir (à voir)

		m5.rot.x = 45;
		m5.rot.y = 90;

		if (!viewport2)
			m5.rot.z -= 0.07 * ms;	// rotation effectuée qu'une seule fois, en mode normal (pas dans la console)

		// à voir : GL_QUADS
		if (game_level == 6)
		{
			//glClearColor (0.2f, 0.0f, 0.0f, 0.0f);
			//glClearColor (0.211f, 0.043f, 0.043f, 0.0f);
			////glClearColor (0.5f, 0.1f, 0.1f, 0.0f);
			//glClearColor (0.784f, 0.196f, 0.196f, 0.0f);
			//m5.Draw(design_tex, GL_TRIANGLES);
			glEnable(GL_BLEND);
			glColor4f(1.0,1.0,1.0,0.4);
			//glColor4f(0.5,0.5,0.5,0.8);
			//glColor4f(0.9,0.9,0.9,1.0);
			m5.Draw(-2, GL_TRIANGLES);//8 4
			glDisable(GL_BLEND);
		}
		else
			m5.Draw();

		//if (game_level == 2)
		//	glDisable(GL_LIGHTING); // fond noir

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		// texture barre du bas
		glBindTexture (GL_TEXTURE_2D, texture[39]);
	}

	// Anti-aliasing (DEBUT)
	if (game_antialiasing_active)
		glEnable(GL_MULTISAMPLE_ARB);
	
	// la ligne du bas
	glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
	glBegin (GL_QUADS);
		glTexCoord2f (0,1);	glVertex3f (-8, -6.5, -.05f);
		glTexCoord2f (0,0);	glVertex3f (-8, -6.6, -.05f);
		glTexCoord2f (1,0);	glVertex3f ( 8, -6.6, -.05f);
		glTexCoord2f (1,1);	glVertex3f ( 8, -6.5, -.05f);
	glEnd ();

	// Anti-aliasing (FIN)
	if (game_antialiasing_active)
		glDisable(GL_MULTISAMPLE_ARB);
	
	if (game_3d)
		glEnable(GL_BLEND);
}

//////////////////////////////////////////////////////////////////////////////////////
//
// Affiche une image pour le fond du jeu
//
//
void display_image()
{
	glEnable (GL_TEXTURE_2D);
	glColor4f (.3f, .3f, .3f, 1.0f);

	// on applique la texture
	glBindTexture ( GL_TEXTURE_2D, texture[28] );

	// puis on dessine
	glBegin ( GL_QUADS );
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-120.0f, -60.0f,  -200.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 120.0f, -60.0f,  -200.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 120.0f,  60.0f,  -200.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-120.0f,  60.0f,  -200.0f);
	glEnd();
}

//////////////////////////////////////////////////////////////////////////////////////
//
// Affiche un cube
//
//
void display_cube(int texture_id, float size)
{
	glPushMatrix();

	//glTranslatef ( 0.0, 0.0, -700.0 );
	//glRotatef ( xrot, 1.0, 0.0, 0.0 );
	//glRotatef ( yrot, 0.0, 1.0, 0.0 );
	//glRotatef ( zrot, 0.0, 0.0, 1.0 );

	//glEnable (GL_TEXTURE_2D);
	glColor4f (1.0f, 1.0f, 1.0f, 1.0f);

	// on applique la texture[4]...
	glBindTexture ( GL_TEXTURE_2D, texture[texture_id] ); // 9

	// puis on dessine...
	glBegin ( GL_QUADS );
		// Front Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size,  size);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( size, -size,  size);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( size,  size,  size);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-size,  size,  size);
	glEnd();
	glBindTexture ( GL_TEXTURE_2D, texture[texture_id] ); // 5 = vaisseau rouge
	glBegin ( GL_QUADS );
		// Back Face
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, -size, -size);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-size,  size, -size);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( size,  size, -size);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( size, -size, -size);
	glEnd();
	glBindTexture ( GL_TEXTURE_2D, texture[texture_id] );
	glBegin ( GL_QUADS );
		// Top Face
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-size,  size, -size);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-size,  size,  size);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( size,  size,  size);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( size,  size, -size);
	glEnd();
	glBindTexture ( GL_TEXTURE_2D, texture[texture_id] );
	glBegin ( GL_QUADS );
		// Bottom Face
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, -size, -size);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( size, -size, -size);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( size, -size,  size);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, -size,  size);
	glEnd();
	glBindTexture ( GL_TEXTURE_2D, texture[texture_id] );
	glBegin ( GL_QUADS );
		// Right face
		glTexCoord2f(1.0f, 0.0f); glVertex3f( size, -size, -size);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( size,  size, -size);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( size,  size,  size);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( size, -size,  size);
	glEnd();
	glBindTexture ( GL_TEXTURE_2D, texture[texture_id] );
	glBegin ( GL_QUADS );
		// Left Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size, -size);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, -size,  size);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-size,  size,  size);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-size,  size, -size);
	glEnd();
	glPopMatrix();

	//xrot+=0.2f;
	//yrot+=0.1f;
	//zrot+=0.3f;
}

//////////////////////////////////////////////////////////////////////////////////////
//
// Affiche et gère le splash screen (en début de nouvelle partie)
//
//
void display_splashscreen()
{
	if (!active_splash)
		return;

	static float splash_alpha = 1;

	//glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA); // doesn't work with ARB_MULTISAMLE
	glDisable(GL_TEXTURE_2D);
	glColor4f(1.0, 1.0, 1.0, splash_alpha);

	// Dessine le  splash screen
	glBegin(GL_QUADS);
		glVertex3f (-20, -20, 1);// -12 à 12
		glVertex3f ( 20, -20, 1);
		glVertex3f ( 20,  20, 1);
		glVertex3f (-20,  20, 1);
	glEnd();

	glEnable(GL_TEXTURE_2D);
	glColor4f (1, 1, 1, 1);

	splash_alpha -= 0.0025 * ms10;
	if (splash_alpha <= 0)
	{
		splash_alpha = 1.0;
		active_splash = false;
	}
}


//////////////////////////////////////////////////////////////////////////////////////
//
// FONCTIONS => Affiche et gère la console
//
//
void display_console()
{
	// conditions nécessaires
	if (g_window->init.width != 1024 || g_window->init.height != 768)
		return;
	if (!game_console_start && !game_console_moving && !game_console)	
		return;
	
	glDisable(GL_TEXTURE_2D);

	// couleurs (avec transparence)
	if (!game_3d)
		glColor4f (1.0f, 1.0f, 1.0f, 0.05f);
	else
		glColor4f (1.0f, 1.0f, 1.0f, 0.05f);
	
	// le panneau coulisse de gauche à droite (et de droite à gauche)
	static float panel_push;

	if (game_console_come_in && game_console_moving)
	{
		if (game_console_start)
			panel_push = 5.3;
		if (panel_push - (0.1 *ms10/2) > 0)							// empêche de dépasser la position à atteindre
			panel_push -= 0.1 *ms10/2;
		else
			panel_push = 0;											// on arrive à 0
	}
	else if (!game_console_come_in && game_console_moving)
	{
		if (game_console_start)
			panel_push = 0;
		if (panel_push < 5.3)
			panel_push += 0.1 *ms10/2;
	}

	// affichage du panneau
	glPushMatrix();
		glLoadIdentity ();
		gluLookAt(	panel_push, 0.0, 20.0,							// l'oeil
					panel_push, 0.0,  0.0,							// le centre
					0.0, 1.0,  0.0	);								// le haut est l'axe y en positif

		// panneau fond
		if (!game_3d_lines)
			glColor4f (1.0f, 1.0f, 1.0f, 0.03f);//0.15 0.05
		else
			glColor4f (0.7f, 0.7f, 1.0f, 0.03f);
		glBegin (GL_QUADS);
			glTexCoord2f (0,1);	glVertex2f (-11.50,  11.50);
			glTexCoord2f (0,0);	glVertex2f (-11.50, -11.50);
			glTexCoord2f (1,0);	glVertex2f (- 5.80, -11.50);
			glTexCoord2f (1,1);	glVertex2f (- 5.80,  11.50);
		glEnd ();

		// console fond
		glDisable(GL_BLEND);
		if (!game_3d)
			glColor4f (0.04f, 0.04f, 0.03f, 1.0f);//0.05
		else
			if (!game_3d_lines)
				glColor4f (0.00f, 0.384f, 0.651f, 0.05f);//0.05
			else
				glColor4f (0.01f, 0.01f, 0.01f, 1.0f);
		glBegin (GL_QUADS);
			glTexCoord2f (0,1);	glVertex2f (-10.50, 4.65);
			glTexCoord2f (0,0);	glVertex2f (-10.50, 2.50);
			glTexCoord2f (1,0);	glVertex2f (- 6.30, 2.50);
			glTexCoord2f (1,1);	glVertex2f (- 6.30, 4.65);
		glEnd ();
		
		// bordures
		glEnable(GL_BLEND);
		glLineWidth( 2.0f);
		glEnable(GL_LINE_STIPPLE);
		glLineStipple (3, 0xADAD);

		// console bordure
		if (!game_3d)
			glColor4f (0.5f, 0.5f, 0.5f, 0.2f);//1.0
		else
			if (!game_3d_lines)
				glColor4f (0.9f, 0.9f, 0.9f, 0.1f);//1.0
			else
				glColor4f (0.7f, 0.7f, 0.8f, 0.15f);
		glBegin(GL_LINE_LOOP);
			glVertex2f (-10.50, 4.65);
			glVertex2f (-10.50, 2.50);
			glVertex2f (- 6.30, 2.50);
			glVertex2f (- 6.30, 4.65);
		glEnd();
		// mini-screen bordure (viewport2)
		glBegin(GL_LINE_LOOP);
			glVertex2f (-10.50, -5.58);
			glVertex2f (-10.50, -2.45);
			glVertex2f (- 6.30, -2.45);
			glVertex2f (- 6.30, -5.58);
		glEnd();
		// panneau bordure
		if (!game_3d)
			glColor4f (0.3f, 0.3f, 0.3f, 0.2f);//0.2
		else if (!game_3d_lines)
			glColor4f (0.9f, 0.9f, 0.9f, 0.05f);//0.05
		glBegin(GL_LINES);
			glVertex2f (-5.80,  11.50);
			glVertex2f (-5.80, -11.50);
		glEnd();

	glPopMatrix();

	float fontspace_temp = fontspace *1.1;									// interligne augmentée

	// INPUT UTILISATEUR (LA LIGNE DE COMMANDE)
	// input ligne 1
	char temp[21];
	char line1[21];
	strncpy(temp, console_text[console_line], 19);
	temp[19] = '\0';
	strcpy(line1, ">");
	strcat(line1, temp);

	// curseur clignotant
	double integer;
	double blink = modf((double)timeGetTime()/1000, &integer);
	if (blink > 0.3)
		if (strlen(console_text[console_line]) < 19)
			strcat(line1, "_");
	
	fontSize(fontsize-1); fontDrawString(30-viewport_push, 580+console_line*fontspace_temp, line1);

	// input ligne 2
	if (strlen(console_text[console_line]) >= 19)
	{
		char* temp;
		char line2[21];
		temp = &console_text[console_line][19];
		strcpy(line2, temp);
		if (blink > 0.3)
			strcat(line2, "_");
		fontSize(fontsize-1); fontDrawString(30-viewport_push, 580-1*fontspace_temp, line2);
	}

	// ÉVALUATION DE LA COMMANDE 1
	if (strlen(console_command) > 0)
	{
		// console_command (1)
		char temp[45] = "";
		strcat(temp, "\r");
		strcat(temp, console_command);
		temp[19]='\0';
		if (!game_3d)
			fontColor(0.7,0.7,0.7);
		else
			fontColor(0.9,0.9,0.9);
		fontSize(fontsize-1); fontDrawString(30-viewport_push, 580-2*fontspace_temp, temp);

		// console_output (1)
		if (!game_3d)
			fontColor(0.7,0.5,0.5);
		else
			fontColor (0.952f, 0.937f, 0.804f);

		if (strcmp(console_command, "HELP") == 0)					// help
			strcpy(console_output, "Type a variable name\0");
		else if (strcmp(console_command, "CAMERAX") == 0)			// variables
			_ltoa(pcamera.x, console_output, 10);
		else if (strcmp(console_command, "CAMERAY") == 0)
			_ltoa(pcamera.y, console_output, 10);
		else if (strcmp(console_command, "CAMERAZ") == 0)
			_ltoa(pcamera.z, console_output, 10);
		else if (strcmp(console_command, "ANGLEX") == 0)
			_ltoa(angle_x, console_output, 10);
		else if (strcmp(console_command, "ANGLEY") == 0)
			_ltoa(angle_y, console_output, 10);
		else if (strcmp(console_command, "MUSICTYPE") == 0)
			_ltoa(music_type, console_output, 10);
		else if (strcmp(console_command, "MUSICENCOURS") == 0)
			strncpy(console_output, music_en_cours, 39);
		else if (strcmp(console_command, "MUSICAFFICHEE") == 0)
			strncpy(console_output, music_affichee, 39);
		else if (strcmp(console_command, "STREAMNAME") == 0)
			strncpy(console_output, stream_name, 39);
		else if (strcmp(console_command, "IDMYMUSIC") == 0)
			_itoa(id_my_music, console_output, 10);
		else														// invalid command
			strcpy(console_output, "Invalid command");
		
		fontSize(fontsize-1); fontDrawString(30-viewport_push, 580-3*fontspace_temp, console_output);
	}

	// ÉVALUATION DE LA COMMANDE 2
	if (strlen(console_command2) > 0)
	{
		// console_command2
		char temp[45] = "";
		strcat(temp, "\r");
		strcat(temp, console_command2);
		temp[19]='\0';
		if (!game_3d)
			fontColor(0.7,0.7,0.7);
		else
			fontColor(0.9,0.9,0.9);
		fontSize(fontsize-1); fontDrawString(30-viewport_push, 580-4*fontspace_temp, temp);
		
		// console_output2
		if (!game_3d)
			fontColor(0.7,0.5,0.5);
		else
			fontColor (0.952f, 0.937f, 0.804f);

		if (strcmp(console_command2, "CAMERAX") == 0)				// variables
			_ltoa(pcamera.x, console_output2, 10);
		else if (strcmp(console_command2, "CAMERAY") == 0)
			_ltoa(pcamera.y, console_output2, 10);
		else if (strcmp(console_command2, "CAMERAZ") == 0)
			_ltoa(pcamera.z, console_output2, 10);
		else if (strcmp(console_command2, "ANGLEX") == 0)
			_ltoa(angle_x, console_output2, 10);
		else if (strcmp(console_command2, "ANGLEY") == 0)
			_ltoa(angle_y, console_output2, 10);
		else if (strcmp(console_command2, "MUSICTYPE") == 0)
			_ltoa(music_type, console_output2, 10);
		else if (strcmp(console_command2, "MUSICENCOURS") == 0)
			strncpy(console_output2, music_en_cours, 39);
		else if (strcmp(console_command2, "MUSICAFFICHEE") == 0)
			strncpy(console_output2, music_affichee, 39);
		else if (strcmp(console_command2, "IDMYMUSIC") == 0)
			_itoa(id_my_music, console_output2, 10);

		fontSize(fontsize-1); fontDrawString(30-viewport_push, 580-5*fontspace_temp, console_output2);
	}

	glEnable(GL_TEXTURE_2D);

}

//////////////////////////////////////////////////////////////////////////////////////
//
// FONCTIONS => Affiche le mini-écran (viewport n°2) si la console est activée
//              (nécessite les mêmes variables externes que display_console() ci-dessus
//
//
void display_console_mini_screen()
{
	extern float ciblex;
	extern float cibley;

	// conditions nécessaires
	if (g_window->init.width != 1024 || g_window->init.height != 768)
		return;
	
	if (game_console_start || game_console_moving || game_console)
	{
		// activation : il coulisse de gauche à droite
		if (game_console_come_in && game_console_moving)
		{
			if (game_console_start)
			{
				viewport_push = (26+193+25.8);
				game_console_start = false;
			}
			if (viewport_push - (((26+193+26.0)/53) *ms10/2) > 0)		// empêche de dépasser la position à atteindre
				viewport_push -= ((26+193+26.0)/53) *ms10/2;
			else
			{
				game_console_moving = false;
				game_console = true;
				game_console_come_in = false;
				viewport_push = 0;										// on arrive à 0
			}
		}
		// désactivation : il coulisse de droite à gauche
		else if (!game_console_come_in && game_console_moving)
		{
			if (game_console_start)
			{
				viewport_push = 0.0;
				game_console_start = false;
			}
			if (viewport_push < (26+193+25.8))
				viewport_push += ((26+193+26.0)/53) *ms10/2;
			else
			{
				game_console_moving = false;
				game_console = false;
				game_console_come_in = true;
			}
		}

		// affichage du mini-screen
		glViewport (26 - ceil(viewport_push), 126, 193, 191*3/4); //26
		glMatrixMode (GL_PROJECTION);									// Select The Projection Matrix
		glLoadIdentity ();												// Reset The Projection Matrix
		gluPerspective( 45.0, (GLfloat)(1024)/(GLfloat)(768), 0.1f, 500.0 ); // Set Up Perspective Mode To Fit 1/4 The Screen (Size Of A Viewport)
		glMatrixMode (GL_MODELVIEW);									// Select The Modelview Matrix
		glLoadIdentity ();												// Reset The Modelview Matrix
		gluLookAt(	pcamera.x, pcamera.y, pcamera.z,					// la caméra
					ciblex, cibley, 0,									// la cible
					0.0, 1.0,  0.0	);									// le haut
		glClear (GL_DEPTH_BUFFER_BIT);

		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);

		// mini-écran : fond noir sans transparence (couche 1)
		glColor4f (0.0f, 0.0f, 0.0f, 1.0f);
		glBegin (GL_QUADS);
			glTexCoord2f (0,0);	glVertex3f (-12,  12, -.15f);
			glTexCoord2f (1,0);	glVertex3f (-12, -12, -.15f);
			glTexCoord2f (1,1);	glVertex3f ( 12, -12, -.15f);
			glTexCoord2f (0,1);	glVertex3f ( 12,  12, -.15f);
		glEnd ();

		// mini-écran : fond gris avec transparence (couche 2)
		glEnable(GL_BLEND);
		glColor4f (1.0f, 1.0f, 1.0f, 0.0f);//0.15 0.05
		glBegin (GL_QUADS);
			glTexCoord2f (0,0);	glVertex3f (-12,  12, -.15f);
			glTexCoord2f (1,0);	glVertex3f (-12, -12, -.15f);
			glTexCoord2f (1,1);	glVertex3f ( 12, -12, -.15f);
			glTexCoord2f (0,1);	glVertex3f ( 12,  12, -.15f);
		glEnd ();

		glRotatef(angle_y, 1.0, 0.0, 0.0);								// rotation utilisateur (par rapport à l'axe des x)
		glRotatef(angle_x, 0.0, 1.0, 0.0);								// rotation utilisateur (par rapport à l'axe des y)

		glEnable(GL_TEXTURE_2D);

		display_background(true);
        display_below_lives();											// Nombre de vies
		display_score_hiscore_lives();									// Score, Hiscore, Lives
		display_explosions(true);										// Explosions des ships
		display_bunkers();												// Bunkers

		for (int k=0; k <= max_tirs; k++)
			if (tir[k].active)
				display_shoot(k, true);

		// PARAMETRES 3D (DEBUT)
		if (game_3d)
		{
			glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
			glEnable(GL_CULL_FACE);
			glFrontFace(GL_CCW);
			glEnable(GL_DEPTH_TEST);
			glDisable(GL_BLEND);
		}
		// LUMIERE DES SHIPS (DEBUT)
		if (game_3d)
		{
			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT1);
		}

		// DISPLAY SHIPS, SUPERSHIP, PLAYER
		for (int i=0; i<=49; i++)
			if (ship[i].alive)
				display_ship(i);

		if (supership[0].alive)
			display_supership();

		if (player[0].alive)
			display_player();

		// LUMIERE DES SHIPS (FIN)
		if (game_3d)
		{
			glDisable(GL_LIGHT1);
			glDisable(GL_LIGHTING);
		}

		// PARAMETRES 3D (FIN)
		if (game_3d)
		{
			glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glDisable(GL_CULL_FACE);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////
//
// Affiche les cadres pour les textes
//
//
void display_text_frames()
{
	// si la résolution est non standard on n'affiche pas les cadres
	if (   (g_window->init.width != 1600 || g_window->init.height != 1200)
		&& (g_window->init.width != 1280 || g_window->init.height != 960)
		&& (g_window->init.width != 1024 || g_window->init.height != 768)
		&& (g_window->init.width !=  800 || g_window->init.height != 600)
		&& (g_window->init.width !=  640 || g_window->init.height != 480) )
		return;

	glPushMatrix();
	glLoadIdentity ();
	gluLookAt(	0.0, 0.0, 20.0,					// l'oeil
				0.0, 0.0,  0.0,					// le centre
				0.0, 1.0,  0.0	);				// le haut est l'axe y en positif

	glDisable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glColor4f (1.0f, 1.0f, 1.0f, 1.0f);

	// CADRE HAUT-DROITE  ¯|
	glBindTexture (GL_TEXTURE_2D, texture[33]);
	float lw = 0;								// lang width : must be resized depending on the language
	if (game_lang == 1)	lw = 0.2; else if (game_lang == 2) lw = 0.4;
	float lx = 0;									// lang x
//	if (game_lang == 1)	lx = -0.1; else if (game_lang == 2) lx = -0.2;
	glBegin (GL_QUADS);
		glTexCoord2f (0,1);	glVertex2f (8.7f +lx - 1.8f, +6.6f +1.20f);
		glTexCoord2f (0,0);	glVertex2f (8.7f +lx - 1.8f, +6.6f -0.9f);
		glTexCoord2f (1,0);	glVertex2f (8.7f +lx + 1.8f + lw, +6.6f -0.9f);
		glTexCoord2f (1,1);	glVertex2f (8.7f +lx + 1.8f + lw, +6.6f +1.20f);
	glEnd ();

	// CADRE BAS-GAUCHE  |_
	if (game_lang == 1)	lw = 0.18; else if (game_lang == 2) lw = -0.25; else lw = 0;
	glBegin (GL_QUADS);
		glTexCoord2f (0,1);	glVertex2f (-8.7f - 1.80f, -8.4f +1.13f);
		glTexCoord2f (0,0);	glVertex2f (-8.7f - 1.80f, -8.4f +0.30f);
		glTexCoord2f (1,0);	glVertex2f (-8.7f + 1.25f + lw, -8.4f +0.30f);
		glTexCoord2f (1,1);	glVertex2f (-8.7f + 1.25f + lw, -8.4f +1.13f);
	glEnd ();

	// CADRE HAUT-GAUCHE  |¯
	glBindTexture (GL_TEXTURE_2D, texture[34]);
	glBegin (GL_QUADS);
		glTexCoord2f (0,1);	glVertex2f (-8.7f - 1.8f, +6.6f +1.30f);
		glTexCoord2f (0,0);	glVertex2f (-8.7f - 1.8f, +6.6f +0.20f);
		glTexCoord2f (1,0);	glVertex2f (-8.7f + 5.4f, +6.6f +0.20f);
		glTexCoord2f (1,1);	glVertex2f (-8.7f + 5.4f, +6.6f +1.30f);
	glEnd ();

	glPopMatrix();
}

//////////////////////////////////////////////////////////////////////////////////////
//
// Affiche l'energie du joueur
//
//
void display_energy()
{
	// mode ortho 2D
	glMatrixMode(GL_PROJECTION);
	glPushMatrix ();
	glLoadIdentity();
	glOrtho(0.0, (GLdouble)g_window->init.width, 0.0, (GLdouble)g_window->init.height, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix ();
	glLoadIdentity();
	
		// resolution
		int w = g_window->init.width;
		int h = g_window->init.height;

		// correct face
		glFrontFace(GL_CW); //(pour les textures png, car inversées)
		glEnable(GL_CULL_FACE);

		// blending
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// draw energy outline
		// (texture width = 256, texture height = 64)
		glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
		glBindTexture (GL_TEXTURE_2D, texture[48]);
		glBegin (GL_QUADS);
			glTexCoord2f (0,0); glVertex2f (w+(-180      )*w/1024, (10 + 64)*h/768 );
			glTexCoord2f (1,0); glVertex2f (w+(-180 + 256)*w/1024, (10 + 64)*h/768 );
			glTexCoord2f (1,1); glVertex2f (w+(-180 + 256)*w/1024, (10     )*h/768 );
			glTexCoord2f (0,1); glVertex2f (w+(-180      )*w/1024, (10     )*h/768 );
		glEnd ();

		// draw energy center
		// (valeur max de player[0].energy = 20)
		float r = 0.57; //ratio real image size / texture size (because part of it is transparent)
		glBindTexture (GL_TEXTURE_2D, texture[49]);
		glBegin (GL_QUADS);
			glTexCoord2f (0,0);                     glVertex2f (w+(-180                            )*w/1024, (10 + 64)*h/768 );
			glTexCoord2f (player[0].energy*r/20,0); glVertex2f (w+(-180 + 256*player[0].energy*r/20)*w/1024, (10 + 64)*h/768 );
			glTexCoord2f (player[0].energy*r/20,1); glVertex2f (w+(-180 + 256*player[0].energy*r/20)*w/1024, (10     )*h/768 );
			glTexCoord2f (0,1);                     glVertex2f (w+(-180                            )*w/1024, (10     )*h/768 );
		glEnd ();

		glDisable(GL_CULL_FACE);

	glMatrixMode (GL_PROJECTION);
	glPopMatrix ();
	glMatrixMode (GL_MODELVIEW);
	glPopMatrix ();

	// retour aux anciens paramètres
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
}
