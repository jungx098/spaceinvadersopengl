// si_Textures.h - space invaders opengl

#ifndef _TEXTURES_H
#define _TEXTURES_H

#include <olectl.h>		// Header File For The OLE Controls Library

//////////////////////////////////////////////////////////////////////////////////////
//
// TEXTURES => Fonctions
//
//

BOOL LoadTexture        (char* file, USHORT resource_id, int num_texture);
BOOL LoadTextureJPG_GIF (char* file, USHORT resource_id, int num_texture);
BOOL LoadTextureBMP     (char* file, USHORT resource_id, int num_texture);
BOOL LoadTextureTGA     (char* file, USHORT resource_id, int num_texture);
BOOL LoadTexturePNG     (char* file, USHORT resource_id, int num_texture);

#endif