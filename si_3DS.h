//////////////////////////////////////////////////////////////////////
//
// 3D Studio Model Class
// by: Matthew Fairfax
//
// Additional code by Mathieu Vidalinc
//
// Model_3DS.h: interface for the Model_3DS class.
//
//////////////////////////////////////////////////////////////////////

#ifndef MODEL_3DS_H
#define MODEL_3DS_H

#include "si_3DS_MemoryFile.h"	// For reading from file or resource
#include "si_3DS_Texture.h"		// For loading textures
#include <stdio.h>

class Model_3DS
{
public:
	// A VERY simple vector struct
	// I could have included a complex class but I wanted the model class to stand alone
	struct Vector {
		float x;
		float y;
		float z;
	};

	// Vertex struct to make code easier to read in places
	struct Vertex {
		float x;
		float y;
		float z;
	};

	// Color struct holds the diffuse color of the material
	struct Color4i {
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;
	};

	// Holds the material info
	// TODO: add color support for non textured polys
	struct Material {
		char name[80];	// The material's name
		GLTexture tex;	// The texture (this is the only outside reference in this class)
		bool textured;	// whether or not it is textured
		Color4i color;
	};

	// Every chunk in the 3ds file starts with this struct
	struct ChunkHeader {
		unsigned short id;	// The chunk's id
		unsigned int   len;	// The length of the chunk (3DS stores this as 32 bits)
	};

	// I sort the mesh by material so that I won't have to switch textures a great deal
	struct MaterialFaces {
		unsigned short *subFaces;	// Index to our vertex array of all the faces that use this material
		int numSubFaces;			// The number of faces
		int MatIndex;				// An index to our materials
	};
	// The 3ds file can be made up of several objects
	struct Object {
		char name[80];				// The object name
		float *Vertexes;			// The array of vertices
		float *Normals;				// The array of the normals for the vertices
		float *TexCoords;			// The array of texture coordinates for the vertices
		unsigned short *Faces;		// The array of face indices
		int numFaces;				// The number of faces
		int numMatFaces;			// The number of differnet material faces
		int numVerts;				// The number of vertices
		int numTexCoords;			// The number of vertices
		bool textured;				// True: the object has textures
		MaterialFaces *MatFaces;	// The faces are divided by materials
		Vector pos;					// The position to move the object to
		Vector rot;					// The angles to rotate the object
	};

	int numModel;			// The number of the 3ds model
	char *path;				// The path of the model
	int numObjects;			// Total number of objects in the model
	int numMaterials;		// Total number of materials in the model
	int totalVerts;			// Total number of vertices in the model
	int totalFaces;			// Total number of faces in the model
	bool shownormals;		// True: show the normals
	Material *Materials;	// The array of materials
	Object *Objects;		// The array of objects in the model
	Vector pos;				// The position to move the model to
	Vector rot;				// The angles to rotate the model
	float scale;			// The size you want the model scaled to
	bool lit;				// True: the model is lit
	bool visible;			// True: the model gets rendered
	FILE *file3ds;			// The 3ds file from user's temporary directory
	CxFile mem3ds;			// The 3ds memory object (either from a file or from a resource)
	mutable bool compiled_vertex_array;		// Enable/disable the compilation of the Vertex Array (mutable : can been modified by the "void draw() const;" function)
	void LoadFromFile(char *name);			// Loads a model from a file
	void LoadFromResource(int resource_Id);	// Loads a model from a resource
	void Draw() const;						// Draws the model
	void Draw(int tex, GLenum mode) const;  // Draws the model with the selected texture (texture>=0) and primitives mode.
							// or with no texture (texture=-1) and with specified mode
							// (triangles, lines, points)
	Model_3DS();			// Constructor
	virtual ~Model_3DS();	// Destructor

private:
	void IntColorChunkProcessor(long length, long findex, int matindex);
	void FloatColorChunkProcessor(long length, long findex, int matindex);
	// Processes the Main Chunk that all the other chunks exist is
	void MainChunkProcessor(long length, long findex);
		// Processes the model's info
		void EditChunkProcessor(long length, long findex);
			
			// Processes the model's materials
			void MaterialChunkProcessor(long length, long findex, int matindex);
				// Processes the names of the materials
				void MaterialNameChunkProcessor(long length, long findex, int matindex);
				// Processes the material's diffuse color
				void DiffuseColorChunkProcessor(long length, long findex, int matindex);
				// Processes the material's texture maps
				void TextureMapChunkProcessor(long length, long findex, int matindex);
					// Processes the names of the textures and load the textures
					void MapNameChunkProcessor(long length, long findex, int matindex);
			
			// Processes the model's geometry
			void ObjectChunkProcessor(long length, long findex, int objindex);
				// Processes the triangles of the model
				void TriangularMeshChunkProcessor(long length, long findex, int objindex);
					// Processes the vertices of the model and loads them
					void VertexListChunkProcessor(long length, long findex, int objindex);
					// Processes the texture cordiantes of the vertices and loads them
					void TexCoordsChunkProcessor(long length, long findex, int objindex);
					// Processes the faces of the model and loads the faces
					void FacesDescriptionChunkProcessor(long length, long findex, int objindex);
						// Processes the materials of the faces and splits them up by material
						void FacesMaterialsListChunkProcessor(long length, long findex, int objindex, int subfacesindex);

	// Calculates the normals of the vertices by averaging
	// the normals of the faces that use that vertex
	void CalculateNormals();

	int StorageType;						// Type of the 3DS Object source (0 => file, 1 => memory)
	void Load(char *name, int resource_Id);	// Loads actually the model (called by LoadFromFile() or LoadFromResource())

};

#endif MODEL_3DS_H


