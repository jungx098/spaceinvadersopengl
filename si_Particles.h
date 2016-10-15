// si_Particles.h - space invaders opengl

#ifndef _PARTICLES_H
#define _PARTICLES_H

//////////////////////////////////////////////////////////////////////////////////////
//
// PARTICLE SYSTEM (EXPLOSIONS)
//
//

#define MAX_PARTICLES 15				// # of particles
typedef struct							// Create A Structure For Particle
{
	float	life;						// Particle Life
	float	fade;						// Fade Speed
	float	x;							// X Position
	float	y;							// Y Position
	float	z;							// Z Position
	float	xi;							// X Direction acceleration
	float	yi;							// Y Direction acceleration
	float	zi;							// Z Direction acceleration
	float	counteri;					// Counter to know when to reduce acceleration
}
particles;								// Particles Structure

typedef struct
{
	particles	particle[MAX_PARTICLES];
}
explosions;
extern explosions explosion[60];

//////////////////////////////////////////////////////////////////////////////////////
//
// PARTICLE SYSTEM => Fonctions
//
//

//BOOL init_particle();
void exploded(int i);
void exploded(int i, double x, double y, double z);
void display_explosions(bool viewport2);

//////////////////////////////////////////////////////////////////////////////////////
//
// PARTICLE SYSTEM 2 (TIRS 3D)
//
//
/*
#define	MAX_PARTICLES2	60			// Number Of Particles To Create

typedef struct						// Create A Structure For Particle
{
	bool	active;					// Active (Yes/No)
	bool	first_one;				// The first one doesnt react the same as others
	float	life;					// Particle Life
	float	fade;					// Fade Speed
	float	r;						// Red Value
	float	g;						// Green Value
	float	b;						// Blue Value
	float	x;						// X Position
	float	y;						// Y Position
	float	z;						// Z Position
	float	xi;						// X Direction
	float	yi;						// Y Direction
	float	zi;						// Z Direction
	float	xg;						// X Gravity
	float	yg;						// Y Gravity
	float	zg;						// Z Gravity
}
particles2;							// Particles Structure

void init_tir3d(int i);
void display_shoot3d(int i, bool viewport2);
*/
#endif