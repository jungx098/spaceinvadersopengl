//////////////////////////////////////////////////////////////////////
//
// 3D Studio Model Class by Matthew Fairfax
//
// Additional code by Mathieu Vidalinc:
//
// - Loading from resource facility (use of the Cxfile class (si_3DS_MemoryFile)).
// - Compiled Vertex Array implementation (please note that the 3DS model must
//   be in one object for this feature).
// - Possibility of using GL_LINES, GL_POINTS and GL_TRIANGLES modes, with the
//   standard textures, no texture, or an externally defined texture.
//_____________________________________________________________________
//
// Model_3DS.cpp: implementation of the Model_3DS class.
// This is a simple class for loading and viewing
// 3D Studio model files (.3ds). It supports models
// with multiple objects. It also supports multiple
// textures per object. It does not support the animation
// for 3D Studio models b/c there are simply too many
// ways for an artist to animate a 3D Studio model and
// I didn't want to impose huge limitations on the artists.
// However, I have imposed a limitation on how the models are
// textured:
// 1) Every faces must be assigned a material
// 2) If you want the face to be textured assign the
//    texture to the Diffuse Color map
// 3) The texture must be supported by the GLTexture class
//    which only supports bitmap and targa right now
// 4) The texture must be located in the same directory as
//    the model
//
// Support for non-textured faces is done by reading the color
// from the material's diffuse color.
//
// Some models have problems loading even if you follow all of
// the restrictions I have stated and I don't know why. If you
// can import the 3D Studio file into Milkshape 3D 
// (http://www.swissquake.ch/chumbalum-soft) and then export it
// to a new 3D Studio file. This seems to fix many of the problems
// but there is a limit on the number of faces and vertices Milkshape 3D
// can read.
//
// To use this 3DS loader in your project:
//
// First, the code which needs to be changed is followed by "// <CUSTOM>". So you can use Ctrl+F to find it
// 1) Copy the 6 files (3 .cpp + 3 .h) in your project folder.
// 2) Change the #include statements by yours on top of the 3 .cpp files.
// 3) Change the declarations of the models "Model_3DS m;" by yours on top of this file.
// 4) Change or delete the external textures array by yours in this file (herein named texture[])
// 5) Change the name of all the textures resource ids in si_3DS_Texture.cpp
//    (by default only JPG resource textures are used (LoadJPGResource()) but you can
//    easily change that to fit your needs. There is code for BMP and TGA, so i you want
//    to use it uncomment LoadTGA(), LoadBMP(), LoadTGAResource(), LoadBMPResource()
//    and make the resource ids changes.
// 6) Enjoy :)
//
// Usage:

// 1) Declare your model
//    ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
// Model_3DS m;
//
// 2) Load your model from a .3ds file or from a "BINARY" resource
//    ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
// m.LoadFromFile("model.3ds");					// from a file
// m.LoadFromResource(ID_RESOURCE_OBJECT_3DS);	// from a resource
//
// 3) Compiled Vertex Array (optional)
//    ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
// If your model is static (i.e. vertices, normals and textures coordinates are
// not changing in your code) you can "compile the vertex (and normals, and textures
// coordinates) array" of this model i.e. theses arrays will be sent to the video card
// memory once per frame so then you can display the model several times in the same
// frame without doing multiple transfers of these arrays between your program and
// the video card. It is only (and rather) useful if you want to draw several times
// the model : either you're doing multi-pass, or you want to display the same model
// many times in the same frame (a row of aliens for example :)
// After loading the model, just do:
//
// m.compiled_vertex_array = true;				// Enable Compiled Vertex Array for this model
//
// 4) Draw the model (each game loop)
//    ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
// m.Draw();									// Renders the model normally to the screen
//
// or...
//
// m.Draw(int tex, GLenum mode);				// Renders the model with following options
//
//      tex:      The texture you want to use for the model
//                (-2 => texture(s) from model, -1 => no texture, >=0 => an external texture)
//
//      mode:     The primitive mode you want to use for drawing the model
//                (GL_TRIANGLES, GL_LINES, or GL_POINTS)
//
//      examples: m.Draw(-2, GL_POINTS);    // Draw the model in points mode with its normal textures.
//                m.Draw(-1, GL_LINES);     // Draw the model in line mode with no texture
//                m.Draw( 4, GL_TRIANGLES); // Draw the model in triangles mode (fill mode) with an external texture.
//                                          // You can change the name of the array of textures indices used in this
//                                          // file to correspond to - your - global texture variable in Model_3DS::Draw()
//											// (herein named texture[])
//
// 5) Some properties you might change
//    ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
// // If you want to show the model's normals (default to false)
// m.shownormals = true;
//
// // If the model is not going to be lit (default to true)
// m.lit = false;
//
// // You can disable the rendering of the model (default to true)
// m.visible = false;
// 
// 6) Rotation and movement of your model
//    ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
// // You can move and rotate the model like this:
// m.rot.x = 90.0f;
// m.rot.y = 30.0f;
// m.rot.z = 0.0f;
//
// m.pos.x = 10.0f;
// m.pos.y = 0.0f;
// m.pos.z = 0.0f;
//
// // If you want to move or rotate individual objects
// m.Objects[0].rot.x = 90.0f;
// m.Objects[0].rot.y = 30.0f;
// m.Objects[0].rot.z = 0.0f;
//
// m.Objects[0].pos.x = 10.0f;
// m.Objects[0].pos.y = 0.0f;
// m.Objects[0].pos.z = 0.0f;
//
//////////////////////////////////////////////////////////////////////

// You need to uncomment this if you are using MFC
//#pragma warn( You need to uncomment this if you are using MFC )
//#include "stdafx.h"

#include "si_3DS.h"
#include "si_main.h"								// <CUSTOM> Header file for Error Handling : ERR()
#include "si_Loading.h"								// <CUSTOM> Header file for update progress bar

// Declarations of my 3DS models
Model_3DS m, m1, m2, m3, m5; //m4					// <CUSTOM> Instanciation des objets 3DS du jeu

// Just an openGL error checking macro
GLenum err;
#define CHECK_ERROR												\
	err = glGetError();											\
	if (glGetError())											\
	{															\
		char err_msg[300];										\
		sprintf(err_msg, "Error %d at line %d", err, __LINE__);	\
		MessageBox(0,err_msg,"OpenGL Error",0);					\
	}															\

// Variables used for Compiled Vertex Arrays (CVA)
int		numModels						= 0;
int		numModel_last_draw				= -1;
bool	compiled_vertex_array_underway	= false;

// CVA Extension Definitions
typedef void (__stdcall *PFNGLLOCKARRAYSEXTPROC) (int first, int count);
typedef void (__stdcall *PFNGLUNLOCKARRAYSEXTPROC) (void);

// CVA Extension Function Pointers
PFNGLLOCKARRAYSEXTPROC      glLockArraysEXT   = NULL;
PFNGLUNLOCKARRAYSEXTPROC    glUnlockArraysEXT = NULL;


// The chunk's id numbers
#define MAIN3DS				0x4D4D
 #define MAIN_VERS			0x0002
 #define EDIT3DS			0x3D3D
  #define MESH_VERS			0x3D3E
  #define OBJECT			0x4000
   #define TRIG_MESH		0x4100
    #define VERT_LIST		0x4110
    #define FACE_DESC		0x4120
     #define FACE_MAT		0x4130
    #define TEX_VERTS		0x4140
     #define SMOOTH_GROUP	0x4150
    #define LOCAL_COORDS	0x4160
  #define MATERIAL			0xAFFF
   #define MAT_NAME			0xA000
   #define MAT_AMBIENT		0xA010
   #define MAT_DIFFUSE		0xA020
   #define MAT_SPECULAR		0xA030
   #define SHINY_PERC		0xA040
   #define SHINY_STR_PERC	0xA041
   #define TRANS_PERC		0xA050
   #define TRANS_FOFF_PERC	0xA052
   #define REF_BLUR_PERC	0xA053
   #define RENDER_TYPE		0xA100
   #define SELF_ILLUM		0xA084
   #define MAT_SELF_ILPCT	0xA08A
   #define WIRE_THICKNESS	0xA087
   #define MAT_TEXMAP		0xA200
    #define MAT_MAPNAME		0xA300
  #define ONE_UNIT			0x0100
 #define KEYF3DS			0xB000
  #define FRAMES			0xB008
  #define MESH_INFO			0xB002
   #define HIER_POS			0xB030
   #define HIER_FATHER		0xB010
   #define PIVOT_PT			0xB013
   #define TRACK00			0xB020
   #define TRACK01			0xB021
   #define TRACK02			0xB022
#define	COLOR_RGB			0x0010
#define COLOR_TRU			0x0011
#define COLOR_TRUG			0x0012
#define COLOR_RGBG			0x0013
#define PERC_INT			0x0030
#define PERC_FLOAT			0x0031


//////////////////////////////////////////////////////////////////////
// Constructor/Destructor
//////////////////////////////////////////////////////////////////////

Model_3DS::Model_3DS()
{
	// Initialization

	shownormals = false;					// Don't show the normals by default
	lit			= true;						// The model is lit by default
	visible		= true;						// The model is visible by default
	compiled_vertex_array = false;			// The model doesn't use CVA by default

	pos.x = 0.0f;							// Set up the default position
	pos.y = 0.0f;
	pos.z = 0.0f;
	
	rot.x = 0.0f;							// Set up the default rotation
	rot.y = 0.0f;
	rot.z = 0.0f;

	path = new char[256];					// Set up the path 3ds filename
	sprintf(path, "");

	numModel		= numModels++;			// Set an identifier for this model (for CVA feature)
	numObjects		= 0;					// Initialize the some variables
	numMaterials	= 0;
	totalVerts		= 0;
	totalFaces		= 0;
	scale			= 1.0f;
}

Model_3DS::~Model_3DS()
{
}

//////////////////////////////////////////////////////////////////////
// Loading the 3DS object
//////////////////////////////////////////////////////////////////////

void Model_3DS::LoadFromFile(char *name)
{
	StorageType = 0;
	Load(name, 0);
}

void Model_3DS::LoadFromResource(int resource_Id)
{
	StorageType = 1;
	Load(NULL, resource_Id);
}

void Model_3DS::Load(char *name, int resource_Id)
{
	// Compiled Vertex Array functions
	static bool first = true;
	if (first)
	{
		glLockArraysEXT   =  ( PFNGLLOCKARRAYSEXTPROC ) wglGetProcAddress("glLockArraysEXT");
		glUnlockArraysEXT =  ( PFNGLUNLOCKARRAYSEXTPROC ) wglGetProcAddress("glUnlockArraysEXT");
		first = false;
	}

	// load 3DS from a file
	if (StorageType == 0)
	{
		// le fichier 3ds est-il présent ?
		WIN32_FIND_DATA FindFileData;
		if (FindFirstFile(name, &FindFileData) == INVALID_HANDLE_VALUE)
			ERR("A 3DS file is missing.");

		// strip "'s
		if (strstr(name, "\""))
			name = strtok(name, "\"");

		// Find the path
		if (strstr(name, "/") || strstr(name, "\\"))
		{
			// Holds the name of the model minus the path
			char *temp;

			// Find the name without the path
			if (strstr(name, "/"))
				temp = strrchr(name, '/');
			else
				temp = strrchr(name, '\\');

			// Allocate space for the path
			//path = new char[strlen(name)-strlen(temp)+1];
			path = new char[256]; // modifié par mat (sinon memory leak)

			// Get a pointer to the end of the path and name
			char *src = name + strlen(name) - 1;

			// Back up until a \ or the start
			while (src != path && !((*(src-1)) == '\\' || (*(src-1)) == '/'))
				src--;

			// Copy the path into path
			memcpy (path, name, src-name);
			path[src-name] = 0;
		}

		// Load the file
		file3ds = fopen(name,"rb");
		
		// mem3ds (CxFile object used to load the 3DS model)
		// is initialized with the file pointer
		mem3ds = CxFile(file3ds);
	}

	// load 3DS from a resource
	else
	{
		// The 3DS model must be a "BINARY" resource
		HRSRC hRes = FindResource(NULL, MAKEINTRESOURCE(resource_Id), "BINARY");
		
		if (!hRes)
			ERR("A 3DS object is missing");

		DWORD	dwDataSize	= SizeofResource(NULL,hRes);
		HGLOBAL	hGlob		= LoadResource(NULL,hRes);
		LPVOID	pData		= LockResource(hGlob);

		HGLOBAL hGlobal		= GlobalAlloc(GMEM_MOVEABLE, dwDataSize);
		BYTE* pData2		= (BYTE*) GlobalLock(hGlobal);
		
		memcpy(pData2, pData, dwDataSize);
		
		GlobalUnlock(hGlobal);
		UnlockResource(hGlob);
		FreeResource(hGlob);	

		// mem3ds (CxFile object used to load the 3DS model)
		// is initialized with the memory object
		mem3ds = CxFile(pData2, dwDataSize);
	}

	// The CxFile had been created, we can now load the structure of the model

	// Make sure we are at the beginning
	mem3ds.Seek(0, SEEK_SET);

	// Holds the main chunk header
	ChunkHeader main;

	// Load the Main Chunk's header
	mem3ds.Read(&main.id, sizeof(main.id), 1);
	mem3ds.Read(&main.len, sizeof(main.len), 1);

	// Start Processing
	MainChunkProcessor(main.len, mem3ds.Tell());

	// Don't need the file anymore so close it
	mem3ds.Close();

	// Calculate the vertex normals
	CalculateNormals();

	// Find the total number of faces and vertices
	totalFaces = 0;
	totalVerts = 0;

	for (int i = 0; i < numObjects; i ++)
	{
		totalFaces += Objects[i].numFaces/3;
		totalVerts += Objects[i].numVerts;
	}

	static int model_number = 0;
	model_number++;

	// If the object doesn't have any texcoords generate some
	for (int k = 0; k < numObjects; k++)
	{
		if (Objects[k].numTexCoords == 0)
		{
			// Set the number of texture coords
			Objects[k].numTexCoords = Objects[k].numVerts;

			// Allocate an array to hold the texture coordinates
			Objects[k].TexCoords = new GLfloat[Objects[k].numTexCoords * 2];

			// Make some texture coords
			for (int m = 0; m < Objects[k].numTexCoords; m++)
			{
//				Objects[k].TexCoords[2*m] = Objects[k].Vertexes[3*m];
//				Objects[k].TexCoords[2*m+1] = Objects[k].Vertexes[3*m+1];
				Objects[k].TexCoords[2*m] = Objects[k].Vertexes[m];
				Objects[k].TexCoords[2*m+1] = Objects[k].Vertexes[m+1];
			}
		}
	}

	// Let's build simple colored textures for the materials w/o a texture
	for (int j = 0; j < numMaterials; j++)
	{
		if (Materials[j].textured == false)
		{
			unsigned char r = Materials[j].color.r;
			unsigned char g = Materials[j].color.g;
			unsigned char b = Materials[j].color.b;
			Materials[j].tex.BuildColorTexture(r, g, b);
			Materials[j].textured = true;
		}
	}
}

//////////////////////////////////////////////////////////////////////
//
// Draw the 3DS objet with standard texture and GL_TRIANGLES mode
// (filled)
//
//////////////////////////////////////////////////////////////////////

void Model_3DS::Draw() const
{
	// Call the underneath method
	Draw(-2, GL_TRIANGLES);
}

//////////////////////////////////////////////////////////////////////
//
// Draw the 3DS objet with options : selected texture and primitive mode
//
// tex     ->   -2 = model texture(s)
//              -1 = no texture
//             >=0 = id of an extern texture of the extern array texture[]
//
// mode    ->   GL_TRIANGLES, GL_LINES ou GL_POINTS
//
//////////////////////////////////////////////////////////////////////

void Model_3DS::Draw(int tex, GLenum mode) const
{
	if (!visible)
		return;

	// SETUP FOR MODE GL_LINES
	// -----------------------
	if (game_3d_lines)	 // <CUSTOM> can be removed for other programs
		mode = GL_LINES; // <CUSTOM> can be removed for other programs

	if (mode == GL_LINES)
	{
		// The lines
        glLineWidth(1.0f);
        glDisable(GL_LINE_STIPPLE);

		// Smooth lines (Anti-aliased)
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_LINE_SMOOTH);

		// Prevent some rasterization artifacts at the joints
		// of smooth line segments that share common vertices
		glDisable(GL_DEPTH_TEST);
	}
	
	// WHICH TEXTURE(S) ?
	// ------------------
	if (tex >= 0)									// texture externe
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[tex]); // <CUSTOM> texture[] (external array) can be changed by your external textures array
	}

	else if (tex == -1)								// pas de texture
		glDisable(GL_TEXTURE_2D);

	else if (tex == -2)								// texture(s) par défaut du model
		glEnable(GL_TEXTURE_2D);

	// POSITIONNEMENT ET AFFICHAGE
	// ---------------------------
	glPushMatrix();

		// Move the model
		glTranslatef(pos.x, pos.y, pos.z);

		// Rotate the model
		glRotatef(rot.x, 1.0f, 0.0f, 0.0f);
		glRotatef(rot.y, 0.0f, 1.0f, 0.0f);
		glRotatef(rot.z, 0.0f, 0.0f, 1.0f);

		// Scale the model
		glScalef(scale, scale, scale);

		// LOOP THROUGH THE OBJECTS
		// ------------------------
		for (int i=0; i<numObjects; i++)
		{
			// CVA
			// ---
			// Compiled Vertex Array Extension not available
			if (!glLockArraysEXT && compiled_vertex_array)
				compiled_vertex_array = false;

			// Compiled Vertex Array (STOP)
			if (compiled_vertex_array_underway && numModel != numModel_last_draw)
			{
				glUnlockArraysEXT();
				compiled_vertex_array_underway	= false;
			}
			// Update numModel_last_draw identifier for next frame
			numModel_last_draw = numModel;
			
			// ARRAYS
			// ------
			// Indicates the Textures, Normals and Vertex pointers (and set compiled vertex array if needed)
			if (!compiled_vertex_array || (compiled_vertex_array && !compiled_vertex_array_underway))
			{
				// Vertices Array
				glEnableClientState(GL_VERTEX_ARRAY);
				glVertexPointer(3, GL_FLOAT, 0, Objects[i].Vertexes);

				// Normals Array
				if (lit)
				{
					glEnableClientState(GL_NORMAL_ARRAY);
					glNormalPointer(GL_FLOAT, 0, Objects[i].Normals);
				}
				else
					glDisableClientState(GL_NORMAL_ARRAY);

				// Textures Array
				if (Objects[i].textured)
				{
					glEnableClientState(GL_TEXTURE_COORD_ARRAY);
					glTexCoordPointer(2, GL_FLOAT, 0, Objects[i].TexCoords);
				}
				else
					glDisableClientState(GL_TEXTURE_COORD_ARRAY);

				// If you scale your object, you can normalize its normals here
				// (make length=1) if the light doesn't appear good (too bright
				// or too dark). It requires more calculations.
				//glEnable(GL_NORMALIZE);

				// CVA
				// ---
				// Compiled Vertex Array (START)
				if (compiled_vertex_array && !compiled_vertex_array_underway)
				{
					glLockArraysEXT(0, Objects[i].numVerts);
					compiled_vertex_array_underway	= true;
				}
			}

			// LOOP THROUGH THE FACES (AS SORTED BY MATERIAL) AND DRAW THEM
			// ------------------------------------------------------------
			for (int j=0; j<Objects[i].numMatFaces; j++)
			{
				// Use the material's texture ?
				if (tex == -2)												// si on n'a pas spécifié une texture externe
					Materials[Objects[i].MatFaces[j].MatIndex].tex.Use();	// on utilise la texture par défaut de la face

				glPushMatrix();

					// Move to the object
					glTranslatef(Objects[i].pos.x, Objects[i].pos.y, Objects[i].pos.z);

					// Rotate the object
					glRotatef(Objects[i].rot.z, 0.0f, 0.0f, 1.0f);
					glRotatef(Objects[i].rot.y, 0.0f, 1.0f, 0.0f);
					glRotatef(Objects[i].rot.x, 1.0f, 0.0f, 0.0f);

					// DRAW THE MODEL
					// --------------
					glDrawElements(mode, Objects[i].MatFaces[j].numSubFaces, GL_UNSIGNED_SHORT, Objects[i].MatFaces[j].subFaces);
				
				glPopMatrix();
			}


			// Show the normals?
			if (shownormals)
			{
				// Loop through the vertices and normals and draw the normal
				for (int k = 0; k < Objects[i].numVerts * 3; k += 3)
				{
					// Disable texturing
					glDisable(GL_TEXTURE_2D);
					// Disbale lighting if the model is lit
					if (lit)
						glDisable(GL_LIGHTING);
					// Draw the normals blue
					glColor3f(0.0f, 0.0f, 1.0f);

					// Draw a line between the vertex and the end of the normal
					glBegin(GL_LINES);
						glVertex3f(Objects[i].Vertexes[k], Objects[i].Vertexes[k+1], Objects[i].Vertexes[k+2]);
						glVertex3f(Objects[i].Vertexes[k]+Objects[i].Normals[k], Objects[i].Vertexes[k+1]+Objects[i].Normals[k+1], Objects[i].Vertexes[k+2]+Objects[i].Normals[k+2]);
					glEnd();

					// Reset the color to white
					glColor3f(1.0f, 1.0f, 1.0f);
					// If the model is lit then renable lighting
					if (lit)
						glEnable(GL_LIGHTING);
				}
			}
		}

	glPopMatrix();

	// we exit with MODE TEXTURE ON
	glEnable(GL_TEXTURE_2D);

}

//////////////////////////////////////////////////////////////////////
// Internal functions
//////////////////////////////////////////////////////////////////////

void Model_3DS::CalculateNormals()
{
	// Let's build some normals
	for (int i = 0; i < numObjects; i++)
	{
		for (int g = 0; g < Objects[i].numVerts; g++)
		{
			// Reduce each vert's normal to unit
			float length;
			Vector unit;

			unit.x = Objects[i].Normals[g*3];
			unit.y = Objects[i].Normals[g*3+1];
			unit.z = Objects[i].Normals[g*3+2];

			length = (float)sqrt((unit.x*unit.x) + (unit.y*unit.y) + (unit.z*unit.z));

			if (length == 0.0f)
				length = 1.0f;

			unit.x /= length;
			unit.y /= length;
			unit.z /= length;

			Objects[i].Normals[g*3]   = unit.x;
			Objects[i].Normals[g*3+1] = unit.y;
			Objects[i].Normals[g*3+2] = unit.z;
		}
	}
}

void Model_3DS::MainChunkProcessor(long length, long findex)
{
	ChunkHeader h;

	// move the file pointer to the beginning of the main
	// chunk's data findex + the size of the header
	mem3ds.Seek(findex, SEEK_SET);

	while (mem3ds.Tell() < (findex + length - 6))
	{
		mem3ds.Read(&h.id, sizeof(h.id), 1);
		mem3ds.Read(&h.len, sizeof(h.len), 1);

		switch (h.id)
		{
			// This is the mesh information like vertices, faces, and materials
			case EDIT3DS	:
				EditChunkProcessor(h.len, mem3ds.Tell());
				break;
			// I left this in case anyone gets very ambitious
			case KEYF3DS	:
				//KeyFrameChunkProcessor(h.len, mem3ds.Tell());
				break;
			default			:
				break;
		}

		mem3ds.Seek((h.len - 6), SEEK_CUR);
	}

	// move the file pointer back to where we got it so
	// that the ProcessChunk() which we interrupted will read
	// from the right place
	mem3ds.Seek(findex, SEEK_SET);
}

void Model_3DS::EditChunkProcessor(long length, long findex)
{
	ChunkHeader h;

	// move the file pointer to the beginning of the main
	// chunk's data findex + the size of the header
	mem3ds.Seek(findex, SEEK_SET);

	// First count the number of Objects and Materials
	while (mem3ds.Tell() < (findex + length - 6))
	{
		mem3ds.Read(&h.id, sizeof(h.id), 1);
		mem3ds.Read(&h.len, sizeof(h.len), 1);
		
		loading_display2(true); // <CUSTOM> update the progress bar
		
		switch (h.id)
		{
			case OBJECT	:
				numObjects++;
				break;
			case MATERIAL	:
				numMaterials++;
				break;
			default			:
				break;
		}

		mem3ds.Seek((h.len - 6), SEEK_CUR);
	}

	// Now load the materials
	if (numMaterials > 0)
	{
		Materials = new Material[numMaterials];

		// Material is set to untextured until we find otherwise
		for (int d = 0; d < numMaterials; d++)
			Materials[d].textured = false;

		mem3ds.Seek(findex, SEEK_SET);

		int i = 0;

		while (mem3ds.Tell() < (findex + length - 6))
		{
			mem3ds.Read(&h.id, sizeof(h.id), 1);
			mem3ds.Read(&h.len, sizeof(h.len), 1);

			switch (h.id)
			{
				case MATERIAL	:
					MaterialChunkProcessor(h.len, mem3ds.Tell(), i);
					i++;
					break;
				default			:
					break;
			}

			mem3ds.Seek((h.len - 6), SEEK_CUR);
		}
	}

	// Load the Objects (individual meshes in the whole model)
	if (numObjects > 0)
	{
		Objects = new Object[numObjects];

		// Set the textured variable to false until we find a texture
		for (int k = 0; k < numObjects; k++)
			Objects[k].textured = false;

		// Zero the objects position and rotation
		for (int m = 0; m < numObjects; m++)
		{
			Objects[m].pos.x = 0.0f;
			Objects[m].pos.y = 0.0f;
			Objects[m].pos.z = 0.0f;

			Objects[m].rot.x = 0.0f;
			Objects[m].rot.y = 0.0f;
			Objects[m].rot.z = 0.0f;
		}

		// Zero out the number of texture coords
		for (int n = 0; n < numObjects; n++)
			Objects[n].numTexCoords = 0;

		mem3ds.Seek(findex, SEEK_SET);

		int j = 0;

		while (mem3ds.Tell() < (findex + length - 6))
		{
			mem3ds.Read(&h.id, sizeof(h.id), 1);
			mem3ds.Read(&h.len, sizeof(h.len), 1);

			switch (h.id)
			{
				case OBJECT	:
					ObjectChunkProcessor(h.len, mem3ds.Tell(), j);
					j++;
					break;
				default			:
					break;
			}

			mem3ds.Seek((h.len - 6), SEEK_CUR);
		}
	}

	// move the file pointer back to where we got it so
	// that the ProcessChunk() which we interrupted will read
	// from the right place
	mem3ds.Seek(findex, SEEK_SET);
}

void Model_3DS::MaterialChunkProcessor(long length, long findex, int matindex)
{
	ChunkHeader h;

	// move the file pointer to the beginning of the main
	// chunk's data findex + the size of the header
	mem3ds.Seek(findex, SEEK_SET);

	while (mem3ds.Tell() < (findex + length - 6))
	{
		mem3ds.Read(&h.id, sizeof(h.id), 1);
		mem3ds.Read(&h.len, sizeof(h.len), 1);

		switch (h.id)
		{
			case MAT_NAME	:
				// Loads the material's names
				MaterialNameChunkProcessor(h.len, mem3ds.Tell(), matindex);
				break;
			case MAT_AMBIENT	:
				//ColorChunkProcessor(h.len, mem3ds.Tell());
				break;
			case MAT_DIFFUSE	:
				DiffuseColorChunkProcessor(h.len, mem3ds.Tell(), matindex);
				break;
			//case 0xA050	: // MAT_TRANSPARENCY //mat
			//	{
			//		ChunkHeader h;
			//		// move the file pointer to the beginning of the main
			//		// chunk's data findex + the size of the header
			//		fseek(bin3ds, findex, SEEK_SET);

			//		fread(&h.id,sizeof(h.id),1,bin3ds);
			//		fread(&h.len,sizeof(h.len),1,bin3ds);

			//		BYTE col=0;
			//		// Read the level value
			//		fread(&col,sizeof(BYTE), 1, bin3ds); 

			//		// Convert to a value between 1 and zero
			//		Materials[matindex].color.a = (unsigned char)(1.0f-(col*0.01f))*255.0f;

			//		// return to right file position
			//		fseek(bin3ds, matindex, SEEK_SET);
			//	}
			//	break;
			case MAT_SPECULAR	:
				//ColorChunkProcessor(h.len, mem3ds.Tell());
			case MAT_TEXMAP	:
				// Finds the names of the textures of the material and loads them
				TextureMapChunkProcessor(h.len, mem3ds.Tell(), matindex);
				break;
			default			:
				break;
		}

		mem3ds.Seek((h.len - 6), SEEK_CUR);
	}

	// move the file pointer back to where we got it so
	// that the ProcessChunk() which we interrupted will read
	// from the right place
	mem3ds.Seek(findex, SEEK_SET);
}

void Model_3DS::MaterialNameChunkProcessor(long length, long findex, int matindex)
{
	// move the file pointer to the beginning of the main
	// chunk's data findex + the size of the header
	mem3ds.Seek(findex, SEEK_SET);

	// Read the material's name
	for (int i = 0; i < 80; i++)
	{
		Materials[matindex].name[i] = mem3ds.GetC();
		if (Materials[matindex].name[i] == 0)
		{
			Materials[matindex].name[i] = NULL;
			break;
		}
	}

	// move the file pointer back to where we got it so
	// that the ProcessChunk() which we interrupted will read
	// from the right place
	mem3ds.Seek(findex, SEEK_SET);
}

void Model_3DS::DiffuseColorChunkProcessor(long length, long findex, int matindex)
{
	ChunkHeader h;

	// move the file pointer to the beginning of the main
	// chunk's data findex + the size of the header
	mem3ds.Seek(findex, SEEK_SET);

	while (mem3ds.Tell() < (findex + length - 6))
	{
		mem3ds.Read(&h.id, sizeof(h.id), 1);
		mem3ds.Read(&h.len, sizeof(h.len), 1);

		// Determine the format of the color and load it
		switch (h.id)
		{
			case COLOR_RGB	:
				// A rgb float color chunk
				FloatColorChunkProcessor(h.len, mem3ds.Tell(), matindex);
				break;
			case COLOR_TRU	:
				// A rgb int color chunk
				IntColorChunkProcessor(h.len, mem3ds.Tell(), matindex);
				break;
			case COLOR_RGBG	:
				// A rgb gamma corrected float color chunk
				FloatColorChunkProcessor(h.len, mem3ds.Tell(), matindex);
				break;
			case COLOR_TRUG	:
				// A rgb gamma corrected int color chunk
				IntColorChunkProcessor(h.len, mem3ds.Tell(), matindex);
				break;
			default			:
				break;
		}

		mem3ds.Seek((h.len - 6), SEEK_CUR);
	}

	// move the file pointer back to where we got it so
	// that the ProcessChunk() which we interrupted will read
	// from the right place
	mem3ds.Seek(findex, SEEK_SET);
}

void Model_3DS::FloatColorChunkProcessor(long length, long findex, int matindex)
{
	float r;
	float g;
	float b;

	// move the file pointer to the beginning of the main
	// chunk's data findex + the size of the header
	mem3ds.Seek(findex, SEEK_SET);

	mem3ds.Read(&r, sizeof(r), 1);
	mem3ds.Read(&g, sizeof(g), 1);
	mem3ds.Read(&b, sizeof(b), 1);
	

	Materials[matindex].color.r = (unsigned char)(r*255.0f);
	Materials[matindex].color.g = (unsigned char)(r*255.0f);
	Materials[matindex].color.b = (unsigned char)(r*255.0f);
	Materials[matindex].color.a = 255;

	// move the file pointer back to where we got it so
	// that the ProcessChunk() which we interrupted will read
	// from the right place
	mem3ds.Seek(findex, SEEK_SET);
}

void Model_3DS::IntColorChunkProcessor(long length, long findex, int matindex)
{
	unsigned char r;
	unsigned char g;
	unsigned char b;

	// move the file pointer to the beginning of the main
	// chunk's data findex + the size of the header
	mem3ds.Seek(findex, SEEK_SET);

	mem3ds.Read(&r, sizeof(r), 1);
	mem3ds.Read(&g, sizeof(g), 1);
	mem3ds.Read(&b, sizeof(b), 1);

	Materials[matindex].color.r = r;
	Materials[matindex].color.g = g;
	Materials[matindex].color.b = b;
	Materials[matindex].color.a = 255;

	// move the file pointer back to where we got it so
	// that the ProcessChunk() which we interrupted will read
	// from the right place
	mem3ds.Seek(findex, SEEK_SET);
}

void Model_3DS::TextureMapChunkProcessor(long length, long findex, int matindex)
{
	ChunkHeader h;

	// move the file pointer to the beginning of the main
	// chunk's data findex + the size of the header
	mem3ds.Seek(findex, SEEK_SET);

	while (mem3ds.Tell() < (findex + length - 6))
	{
		mem3ds.Read(&h.id, sizeof(h.id), 1);
		mem3ds.Read(&h.len, sizeof(h.len), 1);

		switch (h.id)
		{
			case MAT_MAPNAME:
				// Read the name of texture in the Diffuse Color map
				MapNameChunkProcessor(h.len, mem3ds.Tell(), matindex);
				break;
			default			:
				break;
		}

		mem3ds.Seek((h.len - 6), SEEK_CUR);
	}

	// move the file pointer back to where we got it so
	// that the ProcessChunk() which we interrupted will read
	// from the right place
	mem3ds.Seek(findex, SEEK_SET);
}

void Model_3DS::MapNameChunkProcessor(long length, long findex, int matindex)
{
	char name[80];

	// move the file pointer to the beginning of the main
	// chunk's data findex + the size of the header
	mem3ds.Seek(findex, SEEK_SET);

	// Read the name of the texture
	for (int i = 0; i < 80; i++)
	{
		name[i] = mem3ds.GetC();
		if (name[i] == 0)
		{
			name[i] = NULL;
			break;
		}
	}

	// Load the name and indicate that the material has a texture
	char fullname[80];
	sprintf(fullname, "%s%s", path, name);
	Materials[matindex].tex.Load(fullname);
	Materials[matindex].textured = true;

	// move the file pointer back to where we got it so
	// that the ProcessChunk() which we interrupted will read
	// from the right place
	mem3ds.Seek(findex, SEEK_SET);
}

void Model_3DS::ObjectChunkProcessor(long length, long findex, int objindex)
{
	ChunkHeader h;

	// move the file pointer to the beginning of the main
	// chunk's data findex + the size of the header
	mem3ds.Seek(findex, SEEK_SET);

	// Load the object's name
	for (int i = 0; i < 80; i++)
	{
		Objects[objindex].name[i] = mem3ds.GetC();
		if (Objects[objindex].name[i] == 0)
		{
			Objects[objindex].name[i] = NULL;
			break;
		}
	}

	while (mem3ds.Tell() < (findex + length - 6))
	{
		mem3ds.Read(&h.id, sizeof(h.id), 1);
		mem3ds.Read(&h.len, sizeof(h.len), 1);

		switch (h.id)
		{
			case TRIG_MESH	:
				// Process the triangles of the object
				TriangularMeshChunkProcessor(h.len, mem3ds.Tell(), objindex);
				break;
			default			:
				break;
		}

		mem3ds.Seek((h.len - 6), SEEK_CUR);
	}

	// move the file pointer back to where we got it so
	// that the ProcessChunk() which we interrupted will read
	// from the right place
	mem3ds.Seek(findex, SEEK_SET);
}

void Model_3DS::TriangularMeshChunkProcessor(long length, long findex, int objindex)
{
	ChunkHeader h;

	// move the file pointer to the beginning of the main
	// chunk's data findex + the size of the header
	mem3ds.Seek(findex, SEEK_SET);

	while (mem3ds.Tell() < (findex + length - 6))
	{
		mem3ds.Read(&h.id, sizeof(h.id), 1);
		mem3ds.Read(&h.len, sizeof(h.len), 1);

		switch (h.id)
		{
			case VERT_LIST	:
				// Load the vertices of the onject
				VertexListChunkProcessor(h.len, mem3ds.Tell(), objindex);
				break;
			case LOCAL_COORDS	:
				//LocalCoordinatesChunkProcessor(h.len, mem3ds.Tell());
				break;
			case TEX_VERTS	:
				// Load the texture coordinates for the vertices
				TexCoordsChunkProcessor(h.len, mem3ds.Tell(), objindex);
				Objects[objindex].textured = true;
				break;
			default			:
				break;
		}

		mem3ds.Seek((h.len - 6), SEEK_CUR);
	}

	// After we have loaded the vertices we can load the faces
	mem3ds.Seek(findex, SEEK_SET);

	while (mem3ds.Tell() < (findex + length - 6))
	{
		mem3ds.Read(&h.id, sizeof(h.id), 1);
		mem3ds.Read(&h.len, sizeof(h.len), 1);

		switch (h.id)
		{
			case FACE_DESC	:
				// Load the faces of the object
				FacesDescriptionChunkProcessor(h.len, mem3ds.Tell(), objindex);
				break;
			default			:
				break;
		}

		mem3ds.Seek((h.len - 6), SEEK_CUR);
	}

	// move the file pointer back to where we got it so
	// that the ProcessChunk() which we interrupted will read
	// from the right place
	mem3ds.Seek(findex, SEEK_SET);
}

void Model_3DS::VertexListChunkProcessor(long length, long findex, int objindex)
{
	unsigned short numVerts;

	// move the file pointer to the beginning of the main
	// chunk's data findex + the size of the header
	mem3ds.Seek(findex, SEEK_SET);

	// Read the number of vertices of the object
	mem3ds.Read(&numVerts,sizeof(numVerts), 1);

	// Allocate arrays for the vertices and normals
	Objects[objindex].Vertexes = new GLfloat[numVerts * 3];
	Objects[objindex].Normals = new GLfloat[numVerts * 3];

	// Assign the number of vertices for future use
	Objects[objindex].numVerts = numVerts;

	// Zero out the normals array
	for (int j = 0; j < numVerts * 3; j++)
		Objects[objindex].Normals[j] = 0.0f;

	// Read the vertices, switching the y and z coordinates and changing the sign of the z coordinate
	for (int i = 0; i < numVerts * 3; i+=3)
	{
		mem3ds.Read(&Objects[objindex].Vertexes[i],sizeof(GLfloat), 1);
		mem3ds.Read(&Objects[objindex].Vertexes[i+2],sizeof(GLfloat), 1);
		mem3ds.Read(&Objects[objindex].Vertexes[i+1],sizeof(GLfloat), 1);

		// Change the sign of the z coordinate
		Objects[objindex].Vertexes[i+2] = -Objects[objindex].Vertexes[i+2];
	}

	// move the file pointer back to where we got it so
	// that the ProcessChunk() which we interrupted will read
	// from the right place
	mem3ds.Seek(findex, SEEK_SET);
}

void Model_3DS::TexCoordsChunkProcessor(long length, long findex, int objindex)
{
	// The number of texture coordinates
	unsigned short numCoords;

	// move the file pointer to the beginning of the main
	// chunk's data findex + the size of the header
	mem3ds.Seek(findex, SEEK_SET);

	// Read the number of coordinates
	mem3ds.Read(&numCoords,sizeof(numCoords), 1);

	// Allocate an array to hold the texture coordinates
	Objects[objindex].TexCoords = new GLfloat[numCoords * 2];

	// Set the number of texture coords
	Objects[objindex].numTexCoords = numCoords;

	// Read teh texture coordinates into the array
	for (int i = 0; i < numCoords * 2; i+=2)
	{
		mem3ds.Read(&Objects[objindex].TexCoords[i],sizeof(GLfloat), 1);
		mem3ds.Read(&Objects[objindex].TexCoords[i+1],sizeof(GLfloat), 1);
	}

	// move the file pointer back to where we got it so
	// that the ProcessChunk() which we interrupted will read
	// from the right place
	mem3ds.Seek(findex, SEEK_SET);
}

void Model_3DS::FacesDescriptionChunkProcessor(long length, long findex, int objindex)
{
	ChunkHeader h;
	unsigned short numFaces;	// The number of faces in the object
	unsigned short vertA;		// The first vertex of the face
	unsigned short vertB;		// The second vertex of the face
	unsigned short vertC;		// The third vertex of the face
	unsigned short flags;		// The winding order flags
	long subs;					// Holds our place in the file
	int numMatFaces = 0;		// The number of different materials

	// move the file pointer to the beginning of the main
	// chunk's data findex + the size of the header
	mem3ds.Seek(findex, SEEK_SET);

	// Read the number of faces
	mem3ds.Read(&numFaces,sizeof(numFaces), 1);

	// Allocate an array to hold the faces
	Objects[objindex].Faces = new GLushort[numFaces * 3];
	// Store the number of faces
	Objects[objindex].numFaces = numFaces * 3;

	// Read the faces into the array
	for (int i = 0; i < numFaces * 3; i+=3)
	{
		// Read the vertices of the face
		mem3ds.Read(&vertA,sizeof(vertA), 1);
		mem3ds.Read(&vertB,sizeof(vertB), 1);
		mem3ds.Read(&vertC,sizeof(vertC), 1);
		mem3ds.Read(&flags,sizeof(flags), 1);

		// Place them in the array
		Objects[objindex].Faces[i]   = vertA;
		Objects[objindex].Faces[i+1] = vertB;
		Objects[objindex].Faces[i+2] = vertC;

		// Calculate the face's normal
		Vector n;
		Vertex v1;
		Vertex v2;
		Vertex v3;

		v1.x = Objects[objindex].Vertexes[vertA*3];
		v1.y = Objects[objindex].Vertexes[vertA*3+1];
		v1.z = Objects[objindex].Vertexes[vertA*3+2];
		v2.x = Objects[objindex].Vertexes[vertB*3];
		v2.y = Objects[objindex].Vertexes[vertB*3+1];
		v2.z = Objects[objindex].Vertexes[vertB*3+2];
		v3.x = Objects[objindex].Vertexes[vertC*3];
		v3.y = Objects[objindex].Vertexes[vertC*3+1];
		v3.z = Objects[objindex].Vertexes[vertC*3+2];

		// calculate the normal
		float u[3], v[3];

		// V2 - V3;
		u[0] = v2.x - v3.x;
		u[1] = v2.y - v3.y;
		u[2] = v2.z - v3.z;

		// V2 - V1;
		v[0] = v2.x - v1.x;
		v[1] = v2.y - v1.y;
		v[2] = v2.z - v1.z;

		n.x = (u[1]*v[2] - u[2]*v[1]);
		n.y = (u[2]*v[0] - u[0]*v[2]);
		n.z = (u[0]*v[1] - u[1]*v[0]);

		// Add this normal to its verts' normals
		Objects[objindex].Normals[vertA*3]   += n.x;
		Objects[objindex].Normals[vertA*3+1] += n.y;
		Objects[objindex].Normals[vertA*3+2] += n.z;
		Objects[objindex].Normals[vertB*3]   += n.x;
		Objects[objindex].Normals[vertB*3+1] += n.y;
		Objects[objindex].Normals[vertB*3+2] += n.z;
		Objects[objindex].Normals[vertC*3]   += n.x;
		Objects[objindex].Normals[vertC*3+1] += n.y;
		Objects[objindex].Normals[vertC*3+2] += n.z;
	}

	// Store our current file position
	subs = mem3ds.Tell();

	// Check to see how many materials the faces are split into
	while (mem3ds.Tell() < (findex + length - 6))
	{
		mem3ds.Read(&h.id, sizeof(h.id), 1);
		mem3ds.Read(&h.len, sizeof(h.len), 1);

		switch (h.id)
		{
			case FACE_MAT	:
				//FacesMaterialsListChunkProcessor(h.len, mem3ds.Tell(), objindex);
				numMatFaces++;
				break;
			default			:
				break;
		}

		mem3ds.Seek((h.len - 6), SEEK_CUR);
	}

	// Split the faces up according to their materials
	if (numMatFaces > 0)
	{
		// Allocate an array to hold the lists of faces divided by material
		Objects[objindex].MatFaces = new MaterialFaces[numMatFaces];
		// Store the number of material faces
		Objects[objindex].numMatFaces = numMatFaces;

		mem3ds.Seek(subs, SEEK_SET);
		int j = 0;

		// Split the faces up
		while (mem3ds.Tell() < (findex + length - 6))
		{
			mem3ds.Read(&h.id, sizeof(h.id), 1);
			mem3ds.Read(&h.len, sizeof(h.len), 1);

			switch (h.id)
			{
				case FACE_MAT	:
					// Process the faces and split them up
					FacesMaterialsListChunkProcessor(h.len, mem3ds.Tell(), objindex, j);
					j++;
					break;
				default			:
					break;
			}

			mem3ds.Seek((h.len - 6), SEEK_CUR);
		}
	}

	// move the file pointer back to where we got it so
	// that the ProcessChunk() which we interrupted will read
	// from the right place
	mem3ds.Seek(findex, SEEK_SET);
}

void Model_3DS::FacesMaterialsListChunkProcessor(long length, long findex, int objindex, int subfacesindex)
{
	int i;

	char name[80];				// The material's name
	unsigned short numEntries;	// The number of faces associated with this material
	unsigned short Face;		// Holds the faces as they are read
	int material;				// An index to the Materials array for this material

	// move the file pointer to the beginning of the main
	// chunk's data findex + the size of the header
	mem3ds.Seek(findex, SEEK_SET);

	// Read the material's name
	for (i = 0; i < 80; i++)
	{
		name[i] = mem3ds.GetC();
		if (name[i] == 0)
		{
			name[i] = NULL;
			break;
		}
	}

	// Find the material's index in the Materials array
	for (material = 0; material < numMaterials; material++)
	{
		if (strcmp(name, Materials[material].name) == 0)
			break;
	}

	// Store this value for later so that we can find the material
	Objects[objindex].MatFaces[subfacesindex].MatIndex = material;

	// Read the number of faces associated with this material
	mem3ds.Read(&numEntries,sizeof(numEntries), 1);

	// Allocate an array to hold the list of faces associated with this material
	Objects[objindex].MatFaces[subfacesindex].subFaces = new GLushort[numEntries * 3];
	// Store this number for later use
	Objects[objindex].MatFaces[subfacesindex].numSubFaces = numEntries * 3;

	// Read the faces into the array
	for (i = 0; i < numEntries * 3; i+=3)
	{
		// read the face
		mem3ds.Read(&Face,sizeof(Face), 1);
		// Add the face's vertices to the list
		Objects[objindex].MatFaces[subfacesindex].subFaces[i] = Objects[objindex].Faces[Face * 3];
		Objects[objindex].MatFaces[subfacesindex].subFaces[i+1] = Objects[objindex].Faces[Face * 3 + 1];
		Objects[objindex].MatFaces[subfacesindex].subFaces[i+2] = Objects[objindex].Faces[Face * 3 + 2];
	}
	
	// move the file pointer back to where we got it so
	// that the ProcessChunk() which we interrupted will read
	// from the right place
	mem3ds.Seek(findex, SEEK_SET);
}
