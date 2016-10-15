// si_Resources.h - space invaders opengl

#ifndef _RESOURCES_H
#define _RESOURCES_H

//////////////////////////////////////////////////////////////////////////////////////
//
// RESSOURCES => Extraction de certaines ressources en fichiers
//
//

// extraction d'une ressource en fichiers (destination : répertoire temporaire de l'utilisateur)
void resource_extract (char*);

// extraction de toutes les ressources (lance les appels à la fonction ci-dessus)
void resources_extract (void);

// extraction de fmod.dll
void fmod_extract (void);

#endif