// si_Loading.cpp - space invaders opengl

//////////////////////////////////////////////////////////////////////////////////////
//
// LOADING => gestion du chargement
//
//

#include <process.h>							// Header File For _beginthread, _endthread 
#include "si_Main.h"							// Header File For Game
#include "si_Loading.h"							// Header File For Game
#include "si_Textures.h"						// Header File For Game
#include "si_3DS.h"								// Header File For Game
extern Model_3DS m;								// objet 3DS
extern Model_3DS m1;							// objet 3DS
extern Model_3DS m2;							// objet 3DS
extern Model_3DS m3;							// objet 3DS
//extern Model_3DS m4;							// objet 3DS
extern Model_3DS m5;							// objet 3DS

#define DEBUG_MUSIC								// DEBUGGAGE (aussi dans siMain.cpp)
#define DEBUG_BUNKERS							// DEBUGGAGE (aussi dans siMain.cpp)
#define LOG0(text) add_log(text, false);		// MACRO de LOG (ajout d'une entrée dans les logs, _NON_ pris en compte pour la progress bar)
#define LOG(text)  add_log(text, true);			// MACRO de LOG (ajout d'une entrée dans les logs, prise en compte pour la progress bar)

// LOG SYSTEM
char	LogSystem[50][50];						// 50 logs de 50 caractères disponibles
int		LogSystem_index = 0;					// taille effective du tableau des logs

// BARRE DE PROGRESSION
int	progress_bar_total = 115;//115
int	progress_bar_index = 0;

//////////////////////////////////////////////////////////////////////////////////////
//
// CHARGEMENT => GLOBAL (appel de toutes les fonctions de chargement)
//
//
void loading (void *dummy)
{

	if (wglMakeCurrent(g_window->hDC, g_window->hRC) == FALSE)			// contexte de thread
	{
		LPVOID lpMsgBuf;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf, 0, NULL);
		MessageBox(NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION);
		LocalFree(lpMsgBuf);
	}
	
	// VERSION
	char temp[100]="SPACE INVADERS OPENGL v";
	strcat(temp, game_version);
	LOG0(temp)

	// DATE DE BUILD
	strcpy(temp, "Built: ");
	strcat(temp, __DATE__);
	strcat(temp, ", ");
	strcat(temp, __TIME__);
	LOG0(temp)
	LOG0("")

	// CHARGEMENT DES SONS
#ifdef DEBUG_MUSIC
	LOG("Loading Sounds...")
	LoadSounds();											// MUSIC & SOUNDS
#endif

	// CHARGEMENT DES TEXTURES
	LOG("Loading Textures...")
	LoadTextures()	;										// TEXTURES

	// CHARGEMENT DES OBJETS 3D
	LOG("Loading Elements...")
	try
	{
		// 1) from files
		
		// char Path[MAX_PATH+1];
		//
		// a) situé dans le répertoire temp. de l'utilisateur
		// GetTempPath(sizeof Path, Path);
		// strcat(Path, "\\SpaceInvaders-OpenGL");
		//
		// b) situé dans le répertoire du jeu
		// GetCurrentDirectory(sizeof Path, Path);
		//
		// m.LoadFromFile(strcat(Path,  "\\player.3ds"));				// JOUEUR

		// 2) from resources
		
		m.LoadFromResource(IDR_BINARY_PLAYER_3DS);				// JOUEUR
		m1.LoadFromResource(IDR_BINARY_SHIP_3DS);				// SHIPS
		m2.LoadFromResource(IDR_BINARY_SUPERSHIP_3DS);			// SUPERSHIP
		m3.LoadFromResource(IDR_BINARY_FIGHTER_LASER_3DS);		// JOUEUR LASER
		//m4.LoadFromResource(IDR_BINARY_MISSILE_3DS);			// MISSILE
		m5.LoadFromResource(IDR_BINARY_SPHERE_OGIVE_3DS);		// SPHERE OGIVE

		// Enable Compiled Vertex Array for the 3DS ships models.
		// (useful because there's plenty of this model displayed at the same time)
		m1.compiled_vertex_array = true;
	}
	catch(...)
	{
		ERR("Exception: There was a problem during loading of the 3D objects.");
	}

	// INITIALISATION DU JEU
	LOG("Initialization...")
	init_ships();											// VAISSEAUX
	loading_display2(true);
	init_player();											// PLAYER
	loading_display2(true);
	init_supership();										// SUPERSHIP
	loading_display2(true);
	init_shoots();											// TIRS
	loading_display2(true);
#ifdef DEBUG_BUNKERS
	init_bunker(-5.0f, -4.5f, 0.05f);						// BUNKER 1
	loading_display2(true);
	init_bunker(-2.0f, -4.5f, 0.05f);						// BUNKER 2
	loading_display2(true);
	init_bunker( 1.0f, -4.5f, 0.05f);						// BUNKER 3
	loading_display2(true);
	init_bunker( 4.0f, -4.5f, 0.05f);						// BUNKER 4
	loading_display2(true);
#endif
//	init_particle();										// PARTICLES
	loading_display2(true);
	loading_display2(true);

	// RECUPERATION DU HISCORE
	hiscore = get_hiscore();

	LOG(" ")												// ABSORBE 1 CLIGNOTEMENT SUR LE DERNIER ELEMENT AFFICHE

	// RENITIALISATION DU FPS POUR NE PAS PRENDRE EN COMPTE LE DELAI PRIS PAR LE CHARGEMENT
	Fps_Reinitialize();										// To Avoid Jumped Movement After Loading
	
	game_step++;

	wglMakeCurrent(NULL, NULL);

}

//////////////////////////////////////////////////////////////////////////////////////
//
// CHARGEMENT => LOG SYSTEM (Ajout d'une entrée)
//
//
void add_log (char* text, bool addToProgressBar)
{
	static bool textures_step = false;
	static int textures_nb = 0;

	// LOG des textures
	if (strcmp(text, "Loading Textures...") == 0)
	{
		if (textures_step)
			LogSystem_index--;
		else
		{
			textures_step = true;
			strcpy(LogSystem[LogSystem_index], text);	// on ajoute l'entrée dans le tableau des logs
		}

		char string_textures_nb[20] = "";
		_itoa(textures_nb,string_textures_nb, 10);
		strcpy(LogSystem[LogSystem_index], "Loading Textures... (");
		strcat(LogSystem[LogSystem_index], string_textures_nb);
		strcat(LogSystem[LogSystem_index], ")");

		textures_nb++;
	}

	// LOG des autres éléments
	else
	{
		if (textures_step)
		{
			strcpy(LogSystem[LogSystem_index-1], "Loading Textures..."); // on enlève le numéro de texture une fois le chargement des textures effectué
			textures_step = false;
		}
		strcpy(LogSystem[LogSystem_index], text);		// on ajoute l'entrée dans le tableau des logs
	}


	LogSystem_index++;									// l'index du tableau des logs est incrémenté de 1
	loading_display2(addToProgressBar);					// on affiche les logs
}

//////////////////////////////////////////////////////////////////////////////////////
//
// CHARGEMENT => BARRE DE PROGRESSION
//
//
void progress_bar ()
{
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	// cadre
	glColor4f (0.5f, 0.5f, 0.5f, 1.0f);
	glLineWidth(2.0f);
	glBegin(GL_LINE_LOOP);
		glVertex2f(-5.0f ,+0.0f);
		glVertex2f( 5.0f ,+0.0f);
		glVertex2f( 5.0f ,-0.8f);
		glVertex2f(-5.0f ,-0.8f);
	glEnd();

	// rectangle intérieur
	float right = ((float)progress_bar_index / (float)progress_bar_total) * (4.9*2) - 4.9;
	glColor4f (0.97f, 0.97f, 0.85f, 1.0f);
	glBegin(GL_QUADS);
		glVertex2f( -4.9f  , -0.1f);
		glVertex2f(  right , -0.1f);
		glVertex2f(  right , -0.7f);
		glVertex2f( -4.9f  , -0.7f);
	glEnd();

	progress_bar_index++;

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	// ci-dessous pour savoir le nombre de fractions de la barre de progression
	//char my_string[300]="";
	//OutputDebugString(_itoa(progress_bar_index, my_string, 10)); // initialiser progress_bar_total à cette valeur - 1
}

//////////////////////////////////////////////////////////////////////////////////////
//
// CHARGEMENT => D'UN SON, appelée par LoadSounds()
//
//
FSOUND_SAMPLE * LoadSound (int IntResource)
{
	HRSRC res					= FindResource(NULL, MAKEINTRESOURCE(IntResource), "WAVE");
	HGLOBAL hres				= LoadResource(NULL, res);

	void * data					= LockResource(hres);
	unsigned long int length	= SizeofResource(NULL, res);

	FSOUND_SAMPLE * return_FSOUND_SAMPLE = FSOUND_Sample_Load(FSOUND_FREE, (char *)data, FSOUND_LOADMEMORY, 0, length);

	UnlockResource(hres);
	FreeResource(hres);

	loading_display2(true);

	return return_FSOUND_SAMPLE;
}

//////////////////////////////////////////////////////////////////////////////////////
//
// CHARGEMENT => DE LA MUSIQUE ET DES SONS
//
//
void LoadSounds()
{
	// RECHERCHE DE FICHIERS MP3 dans le répertoire du jeu
	int	i=1;
	if (game_option_stream)
		i=2;													// car MOD = 1, stream = 2, les mp3 = 3,4,...
	BOOL fFinished;
	HANDLE hList;
	WIN32_FIND_DATA FileData;

	hList = FindFirstFile("*", &FileData);						// on cherche le premier fichier
	if (hList != INVALID_HANDLE_VALUE)
	{
		fFinished = FALSE;
		while (!fFinished)
		{
			if (strstr(_strlwr(_strdup(FileData.cFileName)), ".mp3") != NULL)// si le fichier est un fichier mp3
			{
				strncpy(music[i].fichier, FileData.cFileName, 255);	// on l'ajoute dans le tableau des musiques
				music[i].fichier[255] = '\0';					// pour faire une chaîne correcte si dépasse la limite
				i++;
			}

			if (!FindNextFile(hList, &FileData))				// y a t'il d'autres fichiers dans le répertoire ?
				if (GetLastError() == ERROR_NO_MORE_FILES)
					fFinished = TRUE;

			if (i == 300)										// nombre de fichiers mp3 limité à ...
				fFinished = TRUE;
		}
	}
	FindClose(hList);

	strcpy(music[0].fichier, "1 000 000 miles by ronny / teklords");// la première musique est le mod XM

	// MUSIQUE XM : CHARGEMENT EN MEMOIRE
    HRSRC res					= FindResource(NULL, MAKEINTRESOURCE(IDR_SOUND_XM), "WAVE");
	HGLOBAL hres				= LoadResource(NULL, res);
	void * data					= LockResource(hres);
	unsigned long int length	= SizeofResource(NULL, res);

	if (!(my_mod = FMUSIC_LoadSongEx((const char*)data, 0, length, FSOUND_LOADMEMORY | FSOUND_LOOP_NORMAL , NULL, 0)))
		FMOD_ErrorString(FSOUND_GetError());

	UnlockResource(hres);
	FreeResource(hres);

	// MUSIQUE XM (SI STREAMING NON SÉLECTIONNÉ)
	if (!game_option_stream)
	{
		music_type = 0;
		strcpy(music_en_cours, music[0].fichier);
		strcpy(music_affichee, music[0].fichier);
		strcat(music_affichee, "     ");							// pour affichage correct derniers caractères

		if (music_on)
		{
			int_sound_mod = FMUSIC_PlaySong(my_mod);
			FMUSIC_SetMasterVolume(my_mod,240);
		}
	}
	
	// MUSIQUE STREAMING (SI SÉLECTIONNÉE AU DÉMARRAGE)
	else
	{
		music_type = 2;
		id_my_music = 1;
		strcpy(music[1].fichier, "Streaming");						// le streaming est en deuxième position
		strcpy(music_affichee, "Streaming     ");
		strcpy(stream_name, "Streaming     ");

		if (game_option_bitrate == 0)
			strcpy(music_en_cours, "http://www.di.fm/mp3/classictechno96k.pls"); // DSL
		else if (game_option_bitrate == 1)
			strcpy(music_en_cours, "http://www.di.fm/mp3/classictechno56k.pls"); // ISDN
		else if (game_option_bitrate == 2)
			strcpy(music_en_cours, "http://www.di.fm/mp3/trance24k.pls");        // Dialup
		else if (game_option_bitrate == 3)
			strcpy(music_en_cours, stream_name_user);                            // Custom

		// Parse the remote file if it's a Shoutcast playlist (*.pls)
		if (_stricmp((const char *)".pls", (const char *)(&music_en_cours[strlen(music_en_cours) - 4])) == 0)
		{
			LOG0("Waiting for playlist...")
			my_playlist = ParsePlaylist(music_en_cours);

			if (my_playlist)
				strcpy(music_en_cours, (char*) my_playlist->name[0]);
		}
		
		//BOX(music_en_cours);
		
		my_stream = FSOUND_Stream_Open(music_en_cours, FSOUND_NORMAL | FSOUND_NONBLOCKING, 0, 0);
		//int_sound_mp3 = FSOUND_Stream_Play(FSOUND_FREE, my_stream);
		int_sound_mp3 = FSOUND_Stream_PlayEx(FSOUND_FREE, my_stream, dspUnitSpectrum, false);
		FSOUND_SetVolume(int_sound_mp3, 240);
	}

	// CHARGEMENT DES SONS
	sound_tir		= LoadSound ( IDR_SOUND_TIR	          );
	sound_tir3d		= LoadSound ( IDR_SOUND_TIR3D         );
	sound_touche	= LoadSound ( IDR_SOUND_EXPLOSION     );
	sound_descente	= LoadSound ( IDR_SOUND_DESCENTE      );
	sound_applause	= LoadSound ( IDR_SOUND_APPLAUSE      );
	sound_shield	= LoadSound ( IDR_SOUND_SHIELD_CHARGE );
	sound_cross		= LoadSound ( IDR_SOUND_CROSS         );
	sound_bonus		= LoadSound ( IDR_SOUND_BONUS         );
	sound_key		= LoadSound ( IDR_SOUND_KEY           );

}

//////////////////////////////////////////////////////////////////////////////////////
//
// CHARGEMENT => TEXTURES
//
//
void LoadTextures()
{
	// Les ID des ressources à charger
	//
	// IDT -> TGA, IDB -> BMP, IDG -> GIF
	// IDJ -> JPG, IDP -> PNG
	//
	USHORT resource_id[]={
		IDT_00, IDB_01, IDB_02, IDB_03, IDT_04, IDB_05, IDB_06, IDG_07,
		IDB_08, IDJ_09, IDB_10, IDB_11, IDB_12, IDB_13, IDB_14, IDB_15,
		IDB_16, IDB_17, IDB_18, IDB_19, IDB_20, IDB_21, IDB_22, IDB_23,
		IDB_24, IDB_25, IDB_26, IDB_27, IDJ_28, IDB_PARTICLE, IDB_30,
		IDJ_31, IDJ_32, IDJ_CADRE_CENTRE, IDJ_CADRE_CENTRE_LONG,
		IDP_35, IDG_36, IDJ_37, IDJ_38, IDB_39, IDT_40, IDJ_41, IDJ_42,
		IDG_43, IDJ_44, IDJ_45, IDG_46, IDG_47, IDP_48, IDP_49, IDP_50,
		IDP_51, IDG_52, IDJ_53};

	glGenTextures((sizeof(resource_id)/2), &texture[0]);		// génère "sizeof(Texture)/2" textures = 2 octets (pour USHORT) *nbre de textures /2 = nbre de textures
	for (int loop=0; loop<sizeof(resource_id)/2; loop++)		// pour chaque texture...
	{
		LOG("Loading Textures...")								// ...on ajoute un LOG
		LoadTexture(NULL, resource_id[loop], loop);				// et on la charge
	}
}

// ID DES TEXTURES
//
// 0  PLAYER.TGA
// 1  SHIP 1 animation n°1
// 2  SHIP 2 animation n°1
// 3  SHIP 3 animation n°1
// 4  SUPERSHIP.TGA
// 5  FOND SKYBOX2
// 6  TIR d'un ship
// 7  FOND ESPACE (GIF)
// 8  TIR du joueur
// 9  FOND SKYBOX
// 10 BARRE du bas
// 11 SHIP 1 animation n°2
// 12 SHIP 2 animation n°2
// 13 SHIP 3 animation n°2
// 14 CHIFFRE 0
// 15 CHIFFRE 1
// 16 CHIFFRE 2
// 17 CHIFFRE 3
// 18 CHIFFRE 4
// 19 CHIFFRE 5
// 20 CHIFFRE 6
// 21 CHIFFRE 7
// 22 CHIFFRE 8
// 23 CHIFFRE 9
// 24 TEXTE "SCORE"
// 25 TEXTE "HISCORE"
// 26 TEXTE "LIVES"
// 27 BUNKER (PIXEL VERT)
// 28 FOND PLANETE (JPG)
// 29 PARTICLE
// 30 BUNKER (PIXEL VERT 3D)

// 31 TIR 3D BASE
// 32 TIR 3D BASE DARK BACKGRD
// 33 CADRE CENTRE
// 34 CADRE CENTRE LONG

// 35 EQUALIZER (PNG)
// 36 EQUALIZER BARRE

// 37 FOND EARTH_MOON (JPG)
// 38 FOND GALAXY1 (JPG)
// 39 BARRE 3D
// 40 PLAYER3D.TGA
// 41 PARTICLE 2 (EXPLOSION)
// 42 POWERUP (JPG)
// 43 PARTICLE 3 (GIF) (PASSAGE 3D)
// 44 PARTICLE 4 (JPG) (REACTEURS)

// 45 TIR 3D BASE DOUBLE
// 46 TEXT WEAPON
// 47 TEXT BONUS

// 48 CADRE ENERGY CONTOUR (PNG)
// 49 CADRE ENERGY (PNG)
// 50 BONUS 1 (PNG)
// 51 PARTICLE 3 (PNG) (PASSAGE 3D)
// 52 PARTICLE 3_3D (GIF) (PASSAGE 3D)
// 53 SPHERE (JPG)

// 90 LOADING
// 91 PARTICLE (texture créée dans init_particle())
// 92 MOTION BLUR


//////////////////////////////////////////////////////////////////////////////////////
//
// CHARGEMENT => AFICHAGE PARTIE 1 (Fading In)
//
//
void loading_display1 (void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				// efface les buffers

	static float fading = 0.0f;											// la luminosité est au minimum
	Fps_Update();

	// diminution du volume de la musique de la dialog box 'About'
	FMUSIC_SetMasterVolume(my_mod_about,(int)((1-fading)*255));

	// on génère la texture "loading..."
	if (!texture[90])
	{
		HBITMAP	hBMP;													// Handle Of The Bitmap
		BITMAP	BMP;													// Bitmap Structure

		glGenTextures(1, &texture[90]);									// Generate 1 texture
		hBMP=(HBITMAP)LoadImage(GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_LOADING), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
		if (hBMP)														// Does The Bitmap Exist?
		{																// If So...
			GetObject(hBMP,sizeof(BMP), &BMP);							// Get The Object
			glPixelStorei(GL_UNPACK_ALIGNMENT,4);						// Pixel Storage Mode (Word Alignment / 4 Bytes)
			glBindTexture(GL_TEXTURE_2D, texture[90]);					// Bind Our Texture

			// Texturing
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	// Linear Filtering
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // Linear Filtering

			// Generate Mipmapped Texture (3 Bytes, Width, Height And Data From The BMP)
			gluBuild2DMipmaps(GL_TEXTURE_2D, 3, BMP.bmWidth, BMP.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);
			DeleteObject(hBMP);											// Delete The Bitmap Object
		}
	}

	// on affiche la texture "loading..."
	float j = 0.3+sqrt(sqrt(1/fading));

	glColor4f (1.0f, 1.0f, 1.0f, fading);
	glBindTexture ( GL_TEXTURE_2D, texture[90] );
	glBegin (GL_QUADS);
		glTexCoord2f (0,0);glVertex3f ( -3.0*j, 0.10*j, .05f);
		glTexCoord2f (1,0);glVertex3f (  3.0*j, 0.10*j, .05f);
		glTexCoord2f (1,1);glVertex3f (  3.0*j, 1.30*j, .05f);
		glTexCoord2f (0,1);glVertex3f ( -3.0*j, 1.30*j, .05f);
	glEnd ();

	// on gère l'éclaircissement de l'image "loading..."
	if (fading <= 1.0)
		fading+= 0.0009 * ms10;//0.0007
	else 
	{
		FMUSIC_StopSong(my_mod_about);
		FMUSIC_FreeSong(my_mod_about);
		game_step++;
	}

	glFlush ();															// flush the GL rendering pipeline
}

//////////////////////////////////////////////////////////////////////////////////////
//
// CHARGEMENT => AFFICHAGE PARTIE 2 (Logs)
//
//
void loading_display2(bool addToProgressBar)	
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				// efface les buffers

	// on affiche toutes les entrées du log
	int fontsize = 8;													// taille du texte des logs
	if (g_window->init.width >= 800)									// logs affichés si résolution >= 800x600
		for(int i=0; i<=LogSystem_index; i++)							// pour toutes les entrées de log
		{
			fontSize (fontsize);if (g_window->init.width >= 1024) fontColor (1.0, 1.0, 1.0); else fontColor (0.8, 0.8, 0.8);
			fontDrawString (10, g_window->init.height -30 -(fontsize+2)*i, "%s",  LogSystem[i]);
		}

		// on affiche l'image "loading..."
		float j = 1.3;
		glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
		glBindTexture ( GL_TEXTURE_2D, texture[90] );
		glBegin (GL_QUADS);
			glTexCoord2f (0,0);glVertex3f ( -3.0*j, 0.10*j, .05f);
			glTexCoord2f (1,0);glVertex3f (  3.0*j, 0.10*j, .05f);
			glTexCoord2f (1,1);glVertex3f (  3.0*j, 1.30*j, .05f);
			glTexCoord2f (0,1);glVertex3f ( -3.0*j, 1.30*j, .05f);
		glEnd ();

		// MAJ de la barre de progression
		if (addToProgressBar)
			progress_bar();

		// on affiche le résultat explicitement car on ne passe pas par la boucle principale (program loop)
		glFlush ();													// Flush The GL Rendering Pipeline
		SwapBuffers(g_window->hDC);									// Swap Buffers (Double Buffering)
}

//////////////////////////////////////////////////////////////////////////////////////
//
// CHARGEMENT => AFFICHAGE PARTIE 3 (Fading out)
//
//
void loading_display3(void)
{
	if (wglGetCurrentContext() == NULL)									// on récupère le contexte
		if (!wglMakeCurrent(g_window->hDC, g_window->hRC))
			OutputDebugString("sio: wglMakeCurrent() failed in loading_display3()");

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				// efface les buffers

	static float fading = 1.0f;											// la luminosité est au maximum
	static float rotation_angle_total = 0;

	Fps_Update();														// MAJ du FPS pour le fading

	// rotation du l'image loading
	rotation_angle_total = (pow((float)1-fading, (float)1.3)) * 90;

	glLoadIdentity();
	gluLookAt(	0.0, 0.0, 10.0,									// l'oeil
				0.0, 0.0,  0.0,									// le centre
				0.0, 1.0,  0.0	);								// le haut


	glRotatef(rotation_angle_total, 0.0f, 1.0f, 1.0f);

	// diminution de la luminosité de l'écran
	glColor4f (1.0f, 1.0f, 1.0f, fading);
	if (fading > 0.0)
		fading-= 0.0015 * ms10;//0.0014
	else
	{
		ReshapeGL(g_window->init.width, g_window->init.height);			// on repasse en vue non-orthogonale
		OutputDebugString("sio: Space Invaders OpenGL [End loading]");
		if (game_option_anti_aliasing_mode == 2 || game_option_anti_aliasing_mode == 3)
			glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
		game_step++;
	}

	// on affiche toutes les entrées du log
	int fontsize = 8;													// taille du texte des logs
	if (g_window->init.width >= 800)									// logs affichés si résolution >= 800x600
		for(int i=0; i<=LogSystem_index; i++)							// pour toutes les entrées de log
		{
			fontSize (fontsize);if (g_window->init.width >= 1024) fontColor (fading, fading, fading); else fontColor (fading-0.2, fading-0.2, fading-0.2);
			fontDrawString (10, g_window->init.height -30 -(fontsize+2)*i, "%s",  LogSystem[i]);
		}

		// on affiche l'image "loading..."
		float j = 0.3 + sqrt(fading);
		glColor4f (1.0f, 1.0f, 1.0f, fading);
		glBindTexture ( GL_TEXTURE_2D, texture[90] );
		glBegin (GL_QUADS);
			glTexCoord2f (0,0);glVertex3f ( -3.0*j, 0.10*j, .05f);
			glTexCoord2f (1,0);glVertex3f (  3.0*j, 0.10*j, .05f);
			glTexCoord2f (1,1);glVertex3f (  3.0*j, 1.30*j, .05f);
			glTexCoord2f (0,1);glVertex3f ( -3.0*j, 1.30*j, .05f);
		glEnd ();

		glFlush ();															// flush the GL rendering pipeline
}



