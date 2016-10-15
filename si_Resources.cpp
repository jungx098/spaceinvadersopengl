// si_Resources.cpp - space invaders opengl

#include "si_Main.h"
#include <delayimp.h>					// For unloading dll (__FUnloadDelayLoadedDLL2(),...)

//////////////////////////////////////////////////////////////////////////////////////
//
// RESOURCES => Extraction d'une ressource dans le rép. temp. de l'utilisateur
//
//
void resource_extract(char* name)
{
	// on créé un sous-répertoire dans le répertoire temporaire de l'utilisateur
	char Path2[MAX_PATH+1];
	GetTempPath(sizeof Path2, Path2);
	strcat(Path2, "\\SpaceInvaders-OpenGL");

	// déclarations et initialisations
	char		m_szFilename2[MAX_PATH];
	HINSTANCE	m_hModule2	= NULL;
	char		m_szType2[MAX_PATH];

	LPCSTR szType2		= "BINARY";
	LPCSTR szFilename2	= strcat(strcat(Path2, "\\"),name);

	memset(m_szType2,0,sizeof m_szType2);
	memcpy(m_szType2,(void*)szType2,strlen(szType2));

	memset(m_szFilename2,0,sizeof m_szFilename2);
	memcpy(m_szFilename2,szFilename2,strlen(szFilename2));

	// extraction de la ressource
	HRSRC hRes2;
	if (strcmp(name,"font.tga") == NULL)
		hRes2 = FindResource(m_hModule2, MAKEINTRESOURCE(IDR_BINARY_FONT_TGA), m_szType2);

	else if (strcmp(name,"player.3ds") == NULL)
		hRes2 = FindResource(m_hModule2, MAKEINTRESOURCE(IDR_BINARY_PLAYER_3DS), m_szType2);

	else if (strcmp(name,"ship.3ds") == NULL)
		hRes2 = FindResource(m_hModule2, MAKEINTRESOURCE(IDR_BINARY_SHIP_3DS), m_szType2);

	else if (strcmp(name,"sphere_ogive.3ds") == NULL)
		hRes2 = FindResource(m_hModule2, MAKEINTRESOURCE(IDR_BINARY_SPHERE_OGIVE_3DS), m_szType2);

	else if (strcmp(name,"supership.3ds") == NULL)
		hRes2 = FindResource(m_hModule2, MAKEINTRESOURCE(IDR_BINARY_SUPERSHIP_3DS), m_szType2);

	else if (strcmp(name,"fighter_laser.3ds") == NULL)
		hRes2 = FindResource(m_hModule2, MAKEINTRESOURCE(IDR_BINARY_FIGHTER_LASER_3DS), m_szType2);

	else return;

	DWORD	dwDataSize2	= SizeofResource(m_hModule2,hRes2);
	HGLOBAL	hGlob2		= LoadResource(m_hModule2,hRes2);
	LPVOID	pData2		= LockResource(hGlob2);

	// création du fichier
	HANDLE	hFile2 = CreateFile (m_szFilename2, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if( hFile2 == INVALID_HANDLE_VALUE )
	{
		UnlockResource(hGlob2);
		FreeResource(hGlob2);	
	}

	DWORD	dwBytesWritten2 = 0;

	if( !WriteFile(hFile2,pData2,dwDataSize2,&dwBytesWritten2,NULL) 
		||
		dwBytesWritten2 != dwDataSize2)
	{
		CloseHandle(hFile2);
		UnlockResource(hGlob2);
		FreeResource(hGlob2);	
		if(DeleteFile(m_szFilename2))
			memset(m_szFilename2,0,sizeof m_szFilename2);
	}

	CloseHandle(hFile2);
	UnlockResource(hGlob2);
	FreeResource(hGlob2);
}

//////////////////////////////////////////////////////////////////////////////////////
//
// RESOURCES => Extraction des ressources en fichiers dans le rép. temporaire
//
//
void resources_extract (void)
{
	// on créé un sous-répertoire dans le répertoire temporaire de l'utilisateur
	char Path2[MAX_PATH+1];
	GetTempPath(sizeof Path2, Path2);
	strcat(Path2, "\\SpaceInvaders-OpenGL");
	CreateDirectory(Path2, NULL);

	resource_extract("font.tga");
	//resource_extract("player.3ds");
	//resource_extract("ship.3ds");
	//resource_extract("sphere_ogive.3ds");
	//resource_extract("supership.3ds");
	//resource_extract("fighter_laser.3ds");
}

//////////////////////////////////////////////////////////////////////////////////////
//
// RESOURCES => Extraction de fmod.dll (en run-time grâce à l'option /DELAYLOAD:fmod.dll)
//              le déchargement éventuel de l'ancienne version nécessite /DELAY:UNLOAD
//
void fmod_extract (void)
{
	// on vérifie si fmod.dll est présent dans le répertoire système de windows
	// (/system pour win9x/ME, /system32 pour win2000/XP)

	char Path[MAX_PATH+1];
	GetSystemDirectory ( Path, MAX_PATH+1 );

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	char searchFile[MAX_PATH+1];
	strcpy(searchFile, Path);
	strcat(searchFile, "\\fmod.dll");

	hFind = FindFirstFile(searchFile, &FindFileData);

	// la version est-elle à jour ?
	// si la version trouvée est inférieure à la version en ressource interne
	// nous devons décharger de la mémoire la première pour la remplacer par la
	// deuxième. Car l'appel de FSOUND_GetVersion() ci-dessous charge
	// automatiquement l'ancienne version pour toute la durée d'exécution.

	bool fmodIsUptodate = false;
	if ((hFind != INVALID_HANDLE_VALUE))						// si le fichier existe
		if (FSOUND_GetVersion() < FMOD_VERSION)					// si la version est inférieure
		{
			fmodIsUptodate = false;
			// décharge l'ancienne dll			
			#if _MSC_VER >= 1300
				if (FAILED(__FUnloadDelayLoadedDLL2("fmod.dll")))	// MSVC++ 7.0 et +
			#else
				//if (FAILED(__FUnloadDelayLoadedDLL("fmod.dll")))	// MSVC++ 6.0 et -
			#endif
				ERR("Error unloading the old fmod.dll.\nTry to delete %system%/fmod.dll and restart the game.\nIt will extract a new version automatically.\n");
		}
		else
			fmodIsUptodate = true;

	// s'il n'est pas présent ou la version plus ancienne on le crée
	if ((hFind == INVALID_HANDLE_VALUE) || (hFind != INVALID_HANDLE_VALUE && !fmodIsUptodate))
	{
		// déclarations et initialisations
		char		m_szFilename[MAX_PATH];
		HINSTANCE	m_hModule	= NULL;
		char		m_szType[MAX_PATH];
		DWORD		m_dwID;

		DWORD		dwID		= IDR_BINARY_FMOD;
		LPCSTR		szType		= "BINARY";
		LPCSTR		szFilename	= strcat(Path, "\\fmod.dll");

		memset(m_szType,0,sizeof m_szType);
		memcpy(m_szType,(void*)szType,strlen(szType));

		memset(m_szFilename,0,sizeof m_szFilename);
		memcpy(m_szFilename,szFilename,strlen(szFilename));

		m_dwID = dwID;

		// extraction de la ressource
		HRSRC	hRes		= FindResource(m_hModule, MAKEINTRESOURCE(m_dwID), m_szType);
		DWORD	dwDataSize	= SizeofResource(m_hModule,hRes);
		HGLOBAL	hGlob		= LoadResource(m_hModule,hRes);
		LPVOID	pData		= LockResource(hGlob);

		// création du fichier
		HANDLE hFile = CreateFile (m_szFilename, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if( hFile == INVALID_HANDLE_VALUE )
		{
			UnlockResource(hGlob);
			FreeResource(hGlob);	
		}

		DWORD	dwBytesWritten=0;

		if( !WriteFile(hFile,pData,dwDataSize,&dwBytesWritten,NULL) 
			||
			dwBytesWritten != dwDataSize)
		{
			CloseHandle(hFile);
			UnlockResource(hGlob);
			FreeResource(hGlob);	
			if(DeleteFile(m_szFilename))
				memset(m_szFilename,0,sizeof m_szFilename);
		}

		CloseHandle(hFile);
		UnlockResource(hGlob);
		FreeResource(hGlob);
	}

	FindClose (hFind);
}
