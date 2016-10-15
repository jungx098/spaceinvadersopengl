// si_Fps.h - space invaders opengl

#ifndef _FPS_H
#define _FPS_H

//////////////////////////////////////////////////////////////////////
//
// Frames Per Second Stuff
// by: TerraX AKA Dave (Mostly)
//
// Calculates FPS
//
//////////////////////////////////////////////////////////////////////

#pragma comment (lib,"winmm.lib")	// Lib needed for WIndows multimedia stuff (timergettime etc)

extern double fps;			// Current FramesPerSecond
extern double fpsavr;		// Average FramesPerSecond
extern double ms;			// number of milliseconds past between each frame
extern double ms10;			// Average ms of the past 10 frames (used for more fluidity in movement)
extern double msTotal;		// Total number of milliseconds past since start of app
extern int	ClockMS,		// Time past since app started with milliseconds
			ClockS,			// seconds
			ClockM,			// minutes
			ClockHR;		// hours
extern double intermede_ms;	// Valeur moyenne constante utilisée pendant l'intermède (sinon ça ralentit et accélère)
extern bool	intermede;

void Fps_Init();			// Call this on startup of application
void Fps_Reinitialize();	// Reinitialize the records of the ten frames by the last ms past since last frame
void Fps_Update();			// Call this each program loop to update vars
double TimerGetTime();

/////////////////////////////////

double GetSysTimeSeconds(void);

#endif