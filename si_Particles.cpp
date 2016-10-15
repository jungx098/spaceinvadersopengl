// si_Particles.cpp - space invaders opengl

#include "si_Main.h"

//////////////////////////////////////////////////////////////////////////////////////
//
// PARTICLES => crée une texture pour le tir 3D
//
//
/*
BOOL init_particle()
{
	// MAKES A TEXTURE FOR PARTICLES

	int texWidth = 256;
	int texHeight = 256;
	GLubyte *texPixels, *p;
	int texSize;
	int i, j;

	texSize = texWidth*texHeight*4*sizeof(GLubyte);
	texPixels = (GLubyte *) malloc(texSize);
	if (texPixels == NULL)
		return FALSE;

	p = texPixels;
	for (i=0; i<texHeight; ++i)
	{
		for (j=0; j<texWidth; ++j)
		{
			GLuint dist = _hypot(float(i - (texHeight / 2)),float(j - (texWidth / 2)));

			float color = 255-(dist*1.8);
			if (color < 0) color = 0;
			p[0] = color;
			p[1] = color;
			p[2] = color;
			p[3] = color;
			p+=4;
		}
	}
	glGenTextures (1, &texture[91]);
	glBindTexture (GL_TEXTURE_2D, texture[91]);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_NEAREST); // changed needed to MIN_FILTER
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texWidth, texHeight, GL_RGBA, GL_UNSIGNED_BYTE, texPixels);

	free(texPixels);
	return TRUE;
}
*/
//////////////////////////////////////////////////////////////////////////////////////
//
// PARTICLES => Explosion d'un ship (initialisation des particules)
//
//

void exploded(int i)
{
	exploded(i, 0, 0, 0);
}

void exploded(int i, double x, double y, double z)
{    

	// INITIALISATION DES PARTICLES
	float V, Angle;

	for (int loop=0;loop<MAX_PARTICLES;loop++)							// Initializes All The Textures
	{
		explosion[i].particle[loop].life=1.0f;							// Give All The Particles Full Life
		explosion[i].particle[loop].fade=float(rand()%100)/10000.0f+0.01f;// Random Fade Speed

		V = float(rand()%4+2) *1.4;										// Speed of the particle 
		Angle = float(rand()%360);										// Angle of the particle

		if (game_3d && i != 50 && i != 51)								// si c'est pas un laser
			V *= 1.3;
		explosion[i].particle[loop].counteri	= 0;

		if (i <= 49)
		{
			explosion[i].particle[loop].x = ship[i].x;						// Set X position
			explosion[i].particle[loop].y = ship[i].y;						// Set Y position
			explosion[i].particle[loop].z = ship[i].z;						// Set Z position
			ship[i].alive			= false;
			ship[i].explosion		= true;
			ship[i].explosion_life	= float((rand()%100)+500);
		}
		else
		{
			explosion[i].particle[loop].x = x;								// Set X position
			explosion[i].particle[loop].y = y;								// Set Y position
			explosion[i].particle[loop].z = z;								// Set Z position
			if (i == 50)		// laser gauche
			{
				player[0].laser_left = false;
				player[0].laser_left_explosion = true;
				player[0].laser_left_explosion_life = float((rand()%100)+500);
			}
			else if (i == 51)	// laser droite
			{
				player[0].laser_right = false;
				player[0].laser_right_explosion = true;
				player[0].laser_right_explosion_life = float((rand()%100)+500);
			}
			else if (i == 52)	// joueur
			{
				player[0].alive = false;
				player[0].explosion = true;
				player[0].explosion_life = float((rand()%100)+500);
			}
			else if (i == 53)	// supership
			{
				supership[0].alive = false;
				supership[0].explosion = true;
				supership[0].explosion_life = float((rand()%100)+500);
			}
		}

		explosion[i].particle[loop].xi = sin(Angle) * V;				// Set X velocity
		explosion[i].particle[loop].yi = cos(Angle) * V;				// Set Y velocity
		//explosion[i].particle[loop].zi = float(((rand()%10)-5)/10) * V;// Set Z velocity
	}

}

//////////////////////////////////////////////////////////////////////////////////////
//
// PARTICLES => Explosion d'un ship ou autre élément (lasers joueur) (déplacements et affichage des particules)
//
//
void display_explosions(bool viewport2)
{
	float V, Angle;
	glBindTexture (GL_TEXTURE_2D, texture[41]); //29 (créée) ou 91 (générée)

	float modelview[16];
	int k,l;

	for (int i=0;i<=53;i++)												// pour chaque ship...
	{
		if ((ship[i].explosion && i <= 49) || (player[0].laser_left_explosion && i == 50 && game_3d) || (player[0].laser_right_explosion && i == 51 && game_3d) || (player[0].explosion && i == 52) || (supership[0].explosion && i == 53))
		{
			int particles_alive = 0;
			for (int loop=0; loop<MAX_PARTICLES; loop++)				// Loop Through All The Particles
			{
				if (explosion[i].particle[loop].life > 0.0f)
					particles_alive++;

				float x = explosion[i].particle[loop].x;					// Grab Our Particle X Position
				float y = explosion[i].particle[loop].y;					// Grab Our Particle Y Position
				//float z = explosion[i].particle[loop].z;				// Grab Our Particle Z Position
				float z = 0.05;//ship[i].z;										// Grab Our Particle Z Position

				// Draw The Particle Using Our RGB Values, Fade The Particle Based On It's Life
				glColor4f(1.0f,1.0f,1.0f,explosion[i].particle[loop].life);

				float size = cos(explosion[i].particle[loop].life / 2) / 1.6; //1.8

				if (game_3d)
				{
					if (i == 53)					// si supership
						size *= 2.0;
					else if (i != 50 && i != 51)	// si pas un réacteur
						size *= 1.2;

					// BILLBOARDING (les particules sont perpendiculaires à la caméra)
					// --------------------------------------------------------------
					if (game_option_billboard && !viewport2)
					{
						glPushMatrix();
						glTranslatef ( x, y, 0 );

						// get the current modelview matrix
						glGetFloatv(GL_MODELVIEW_MATRIX , modelview);

						// undo all rotations (beware all scaling is lost as well)
						for( k=0; k<3; k++ ) 
							for( l=0; l<3; l++ )
							{
								if ( k==l )
									modelview[k*4+l] = 1.0;
								else
									modelview[k*4+l] = 0.0;
							}

						// set the modelview with no rotations and scaling
						glLoadMatrixf(modelview);

						glBegin(GL_TRIANGLE_STRIP);							// Build Quad From A Triangle Strip
						glTexCoord2f(1,1); glVertex3f(+ size,+ size,z);		// Top Right
						glTexCoord2f(0,1); glVertex3f(- size,+ size,z);		// Bottom Right
						glTexCoord2f(1,0); glVertex3f(+ size,- size,z);		// Top Left
						glTexCoord2f(0,0); glVertex3f(- size,- size,z);		// Bottom Left
						glEnd();											// Done Building Triangle Strip

						glPopMatrix();
					}
					else // pas de billboarding (explosion ship)
					{
						glBegin(GL_TRIANGLE_STRIP);							// Build Quad From A Triangle Strip
						glTexCoord2f(1,1); glVertex3f(x + size,y + size,z);	// Top Right
						glTexCoord2f(0,1); glVertex3f(x - size,y + size,z);	// Bottom Right
						glTexCoord2f(1,0); glVertex3f(x + size,y - size,z);	// Top Left
						glTexCoord2f(0,0); glVertex3f(x - size,y - size,z);	// Bottom Left
						glEnd();											// Done Building Triangle Strip
					}
				}
				else // game 2D
				{
					glBegin(GL_TRIANGLE_STRIP);								// Build Quad From A Triangle Strip
					glTexCoord2f(1,1); glVertex3f(x + size,y + size,z);		// Top Right
					glTexCoord2f(0,1); glVertex3f(x - size,y + size,z);		// Bottom Right
					glTexCoord2f(1,0); glVertex3f(x + size,y - size,z);		// Top Left
					glTexCoord2f(0,0); glVertex3f(x - size,y - size,z);		// Bottom Left
					glEnd();												// Done Building Triangle Strip
				}
	
				if (viewport2) // For the Mini-screen, we don't recalculate the movements
					continue;

				// Move the particles
				explosion[i].particle[loop].x += (explosion[i].particle[loop].xi /250) * 0.075 * ms10;// Move On The X Axis By X Speed
				explosion[i].particle[loop].y += (explosion[i].particle[loop].yi /250) * 0.075 * ms10;// Move On The Y Axis By Y Speed
				//explosion[i].particle[loop].z+=explosion[i].particle[loop].zi/250;				// Move On The Z Axis By Z Speed

				// Slow down the particles
				explosion[i].particle[loop].counteri += (3 * ms10) / 40 ;
				if ( explosion[i].particle[loop].counteri > 0.99)
				{
					if (game_3d)
					{
						explosion[i].particle[loop].xi*=(.90);//.94
						explosion[i].particle[loop].yi*=(.90);
						//explosion[i].particle[loop].zi*=.99;
					}
					else
					{
						explosion[i].particle[loop].xi*=(.88);//.90
						explosion[i].particle[loop].yi*=(.88);
						//explosion[i].particle[loop].zi*=.99;
					}
					explosion[i].particle[loop].counteri = 0;
				}

				// Particle life
				explosion[i].particle[loop].life -= explosion[i].particle[loop].fade * 0.075 * ms10;// Reduce Particles Life By 'Fade'

				// Creation of new particle, if particle is burned out
				if ((explosion[i].particle[loop].life < 0.0f)
					&& (
					       (i <= 49 && ship[i].explosion_life > 0)
						|| (i == 50 && player[0].laser_left_explosion_life > 0)
						|| (i == 51 && player[0].laser_right_explosion_life > 0)
						|| (i == 52 && player[0].explosion_life > 0)
						|| (i == 53 && supership[0].explosion_life > 0)	))
				{
					explosion[i].particle[loop].life	= 1.0f;				// Give It New Life
					explosion[i].particle[loop].fade	= float(rand()%100)/10000 + 0.015f;// Random Fade Value

					if (i <= 49)											// It's a ship
					{
						explosion[i].particle[loop].x		= ship[i].x;	// Center On X Axis
						explosion[i].particle[loop].y		= ship[i].y;	// Center On Y Axis
						explosion[i].particle[loop].z		= ship[i].z;	// Center On Z Axis
					}
					else
					{
						explosion[i].particle[loop].x		= x;			// Center On X Axis
						explosion[i].particle[loop].y		= y;			// Center On Y Axis
						explosion[i].particle[loop].z		= z;			// Center On Z Axis
					}

					V = (float((rand()%3)+2));								// Velocity of the particles
					if (game_3d)
						V *= 1.5;
					Angle = float(rand()%360);								// Angle of the particles

					explosion[i].particle[loop].xi = sin(Angle) * V;		// x acceleration
					explosion[i].particle[loop].yi = cos(Angle) * V;		// y acceleration
					//explosion[i].particle[loop].zi = ((rand()%10)-5)/5;
				} 
				
				// la durée de vie de l'explosion diminue
				if (i <= 49)
					ship[i].explosion_life-= ((3 * ms10) /40);
				else if (i == 50)
					player[0].laser_left_explosion_life-= ((3 * ms10) /40);
				else if (i == 51)
					player[0].laser_right_explosion_life-= ((3 * ms10) /40);
				else if (i == 52)
					player[0].explosion_life-= ((3 * ms10) /40);
				else if (i == 53)
					supership[0].explosion_life-= ((3 * ms10) /40);
			}

			// s'il n'y a plus de particules on remet l'explosion à "false"
			if (particles_alive == 0)
				if (i <= 49)
					ship[i].explosion = false;
				else if (i == 50)
					player[0].laser_left_explosion = false;
				else if (i == 51)
					player[0].laser_right_explosion = false;
				else if (i == 52)
					player[0].explosion = false;
				else if (i == 53)
					supership[0].explosion = false;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////
//
// PARTICLE SYSTEM 2 (TIRS 3D)
//
//
/*
float	slowdown=2.0f;				// Slow Down Particles

void init_tir3d(int i)
{
	for (int loop=0;loop<MAX_PARTICLES2;loop++)							// Initials All The Textures
	{
		tir[i].particle2[loop].active=true;								// Make All The Particles Active
		tir[i].particle2[loop].first_one=true;
		//		tir[i].particle2[loop].life=1.0f;								// Give All The Particles Full Life
		//		tir[i].particle2[loop].fade=float(rand()%100)/1000.0f+0.003f;	// Random Fade Speed
		tir[i].particle2[loop].life=float(rand()%10)/100+.2;			// (max 0.5) Give All The Particles Full Life
		tir[i].particle2[loop].fade=float(rand()%50)/1000.0f+0.001f;	//%50 (max 0.11) Random Fade Speed //%100)/1000...+0.01f;
		tir[i].particle2[loop].x=tir[i].x;								// Center On X Axis
		tir[i].particle2[loop].y=tir[i].y + 0.5f + float((rand()%3))/10.0f;//+0.5	// Y Axis
		tir[i].particle2[loop].z=tir[i].z;								// Center On Z Axis
		tir[i].particle2[loop].r=255;									// Select Red Rainbow Color
		tir[i].particle2[loop].g=255;									// Select Red Rainbow Color
		tir[i].particle2[loop].b=200;									// Select Red Rainbow Color
		tir[i].particle2[loop].xi=float((rand()%30)-15.0f);// 60...16	// Random Speed On X Axis
		tir[i].particle2[loop].yi=float((rand()%60)-30.0f);				// Random Speed On Y Axis
		tir[i].particle2[loop].zi=float((rand()%60)-30.0f);				// Random Speed On Z Axis
		tir[i].particle2[loop].xg=0.0f;//float(rand()%10)/10;//0.0f;	// Set Horizontal Pull To Zero
		tir[i].particle2[loop].yg=-0.8f;//0.8							// Set Vertical Pull Downward
		tir[i].particle2[loop].zg=0.0f;									// Set Pull On Z Axis To Zero

	}
}


void display_shoot3d(int i, bool viewport2)
{
	glBindTexture (GL_TEXTURE_2D, texture[91]); // 29 => particle.bmp, 91 => texture générée en load-time
	int total_active = 0;

	float modelview[16];
	int k,l;

	for (int loop=0;loop<MAX_PARTICLES2;loop++)							// Loop Through All The Particles
	{
		if (tir[i].particle2[loop].active)								// If The Particle Is Active
		{
			total_active++;

			float x=tir[i].particle2[loop].x;							// Grab Our Particle X Position
			float y=tir[i].particle2[loop].y;							// Grab Our Particle Y Position
			float z=tir[i].particle2[loop].z;							// Particle Z Pos + Zoom

			// Draw The Particle Using Our RGB Values, Fade The Particle Based On It's Life
			glColor4f(1.0, 1.0, 1.0, tir[i].particle2[loop].life);

			// BILLBOARDING (les particules sont perpendiculaires à la caméra)
			// --------------------------------------------------------------
			if (game_option_billboard && !viewport2)
			{
				glPushMatrix();
				glTranslatef ( x, y, 0 );

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

				glBegin(GL_TRIANGLE_STRIP);									// Build Quad From A Triangle Strip
				glTexCoord2d(1,1); glVertex3f(+0.15f,+0.15f,z);				// Top Right
				glTexCoord2d(0,1); glVertex3f(-0.15f,+0.15f,z);				// Top Left
				glTexCoord2d(1,0); glVertex3f(+0.15f,-0.15f,z);				// Bottom Right
				glTexCoord2d(0,0); glVertex3f(-0.15f,-0.15f,z);				// Bottom Left
				glEnd();													// Done Building Triangle Strip

				glPopMatrix();
			}
			else // pas de billboarding (shoot 3D)
			{
				glBegin(GL_TRIANGLE_STRIP);									// Build Quad From A Triangle Strip
				glTexCoord2d(1,1); glVertex3f(x + 0.15f,y + 0.15f,z);		// Top Right
				glTexCoord2d(0,1); glVertex3f(x - 0.15f,y + 0.15f,z);		// Top Left
				glTexCoord2d(1,0); glVertex3f(x + 0.15f,y - 0.15f,z);		// Bottom Right
				glTexCoord2d(0,0); glVertex3f(x - 0.15f,y - 0.15f,z);		// Bottom Left
				glEnd();													// Done Building Triangle Strip
			}

			if (viewport2)
				continue;													// if mini-screen we don't calcule a second time the particles movements

				tir[i].particle2[loop].x+=tir[i].particle2[loop].xi/(slowdown*1000)* 0.075 * ms10;// Move On The X Axis By X Speed
				tir[i].particle2[loop].y+=tir[i].particle2[loop].yi/(slowdown*1000)* 0.075 * ms10;// Move On The Y Axis By Y Speed
				tir[i].particle2[loop].z+=tir[i].particle2[loop].zi/(slowdown*1000)* 0.075 * ms10;// Move On The Z Axis By Z Speed

				tir[i].particle2[loop].xi+=tir[i].particle2[loop].xg;		// Take Pull On X Axis Into Account
				tir[i].particle2[loop].yi+=tir[i].particle2[loop].yg;		// Take Pull On Y Axis Into Account
				tir[i].particle2[loop].zi+=tir[i].particle2[loop].zg;		// Take Pull On Z Axis Into Account
				tir[i].particle2[loop].life-=tir[i].particle2[loop].fade* 0.075 * ms10;	// Reduce Particles Life By 'Fade'

				if (tir[i].particle2[loop].life<0.0f && tir[i].active)		// If Particle Is Burned Out
				{
					tir[i].particle2[loop].first_one = false;
					tir[i].particle2[loop].life=1.0f;						// Give It New Life
					tir[i].particle2[loop].fade=float(rand()%100)/1000.0f+0.01f;// Random Fade Value
					tir[i].particle2[loop].x=tir[i].x;						// Center On X Axis
					tir[i].particle2[loop].y=tir[i].y + float((rand()%6)-1.0f)/10.0f;// Y Axis
					tir[i].particle2[loop].z=tir[i].z;						// Center On Z Axis

					tir[i].particle2[loop].xi=float((rand()%30)-15.0f);		// X Axis Speed And Direction
					tir[i].particle2[loop].yi=float((rand()%60)-30.0f);		// Y Axis Speed And Direction
					tir[i].particle2[loop].zi=float((rand()%60)-30.0f);		// Z Axis Speed And Direction

					tir[i].particle2[loop].r=255;							// Select Red From Color Table
					tir[i].particle2[loop].g=255;							// Select Green From Color Table
					tir[i].particle2[loop].b=255;							// Select Blue From Color Table
				}

				else if (tir[i].particle2[loop].life<0.0f && !tir[i].active)	// If Particle Is Burned Out
					tir[i].particle2[loop].active = false;
		}
	}

	if (total_active == 0)
		tir[i].tail = false;
}
*/