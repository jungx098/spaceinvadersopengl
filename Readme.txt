
  Space Invaders OpenGL 0.6.1                                    2004-May
  ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
 
  OVERVIEW
  ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
  Space Invaders OpenGL is a clone of Space Invaders (a classic arcade
  game of the early 80´s), developed with the OpenGL API. The aim is to
  make a game which looks like the original one but with pretty 2D / 3D
  improvements.

  Additional sources/libraries used for the game :
            - Nehe Basecode (http://nehe.gamedev.net)
            - FMOD Sound System (http://www.fmod.org)
            - GLTexFont (http://nate.scuzzy.net/programming/gltexfont)

  Musics :  - 1 000 000 miles by ronny / teklords (Game)
            - StrangleHold (C)JT (About)
            - Digitally Imported http://www.di.fm (Streaming)
  
  Note : When selecting streaming a request is sent to www.google.com
  to launch Internet connection and/or firewall setting.

  SYSTEM REQUIREMENTS
  ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
  - Windows 98/ME/2000/XP
  - 3D Video card OpenGL compatible (Nvidia GeForce, Ati Radeon...)

  CONTROLS
  ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
  Up Arrow,Ctrl -  Shoot
  Left Arrow    -  Move to the left
  Right Arrow   -  Move to the right
  Mouse         -  Scene Rotation
  NumPad +      -  Zoom In
  NumPad -      -  Zoom Out
  B             -  Change Background
  M             -  Music On/Off
  R             -  Reshape Window
  A             -  Anti-aliasing on/off (mainly 3D mode)
  <F5>+<F6>     -  Cheating
  Escape        -  Quit
  
  FEEDBACK
  ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
  Feedback is always appreciated - of positive and of negative kind.
  Please send your feedback or problem to the forum :
  https://sourceforge.net/forum/?group_id=74529

             Author:    Mathieu Vidalinc
             E-mail:    <maya75@users.sourceforge.net>
             Forum:     https://sourceforge.net/forum/?group_id=74529
             Homepage:  http://spaceinvadersgl.sourceforge.net

  VERSION HISTORY
  ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
  May   2004 v0.6.1b- Added Anti-aliasing x2 (4 samples/9 taps), previous
                      Anti-aliasing is now 2/5 (Quincunx) instead of 2/2.
                    - Added support for custom stream URL (Shoutcast
                      playlist (*.pls), Shoutcast stream or mp3 stream.
                      Retrieve now the name of artist and song.
                    - Added recording of startup options in registry.
                    - Added support for Wivik on-screen virtual keyboard.
                    - Added PNG support for using alpha component
                      (translucence, antialiasing and shadow effects).
                      Textures updated/added : equalizer, frames, energy.
                    - <V> key for VSync during game, tracking tooltips.
  
  Jan   2004 v0.6.0 - Added Compiled Vertex Array feature for 3D objects
                      drawn several times (like ships). These objects are
                      loaded in the GPU memory for faster rendering.
                      Needs a compatible graphic card (all recent ones)
                    - Fixed bug when going in 3D mode.
                    - Loads now the 3D models from internal memory.
  
  Dec   2003 v0.5.9 - Improved 3D line mode (artifacts eliminated)
                    - Added Anti-aliasing option (mainly 3D filled models)
                    - Fixed finally the changing music bug (extern var)
                    - Improved equalizer, explosions.
                    - Added bonuses in 3D mode (mini ships, weapons)
                    - Decrease the executable size.
                    - Ships in 3d mode have now life points.

  Sep   2003 v0.5.8 - Work on the executable size.
             v0.5.7 - Some small enhancements in the code.
             v0.5.6 - Improved mp3 reading performance by reading files
                      in background without affecting frame-rate (use of
                      the new flag FSOUND_NONBLOCKING)
                    - Added Internet music streaming functionality.
                    - Added spectrum equalizer.
             v0.5.5 - Fixed bug while playing mp3 files (stream closing)
                    - Improved mipmapping handling.
                    - Improved levels, shoots, code.
                    - Added mp3 titles scrolling.
  
  Aug   2003 v0.5.4 - Fixed bug "msvcr70.dll is missing" (linking issue)
                    - Disable vertical synchro now works even if enabled
                      by default in Display Properties.
                    - Improved loading thread without using of thread 
                      priorities anymore.
                    - Added debug console (² key). Needs 1024x768 res.
                    - Added viewport (mini-screen) in debug console.
                    - Added lines mode (3D game).
                    - Added <ctrl> key for shooting.
                    - Added Hiscore update while playing.
  
  Aug   2003 v0.5.3 - New startup screen with new options : VSYNC,
                      billboarding, smooth shading.
                    - Added Error Handling.
                    - Added Vertical Synchronization function.
                    - Added spectrum analysis for beat detection.
                    - Added camera path (Bézier curves)
  
  Jul   2003 v0.5.1 - Added billboarding for particle system (explosion,
                      shoots) i.e. these sprites seem now to be in 3d.
                    - Added Speed Loading option which set the loading
                      thread priority to highest (approx. twice faster)
                    - Added multiline tooltips in startup window.
                    - Added progress bar during loading.
                    - Added fmod version checking routine for replacement.

  June  2003 v0.5.0 - Added multithreading for loading in a distinct
                      thread, which defreeze window and permit to move/
                      close by setting priority to lowest, then back to
                      normal.
                    - Added About-DialogBox (design/readme/music).
  
  May   2003 v0.4.9 - Added 3D game mode. 3DS objects/particle system
                      shoot/lights/ship reactors/etc.
                    - Improved startup screen : enabled/desabled radio
                      buttons, mipmap button. window centered in window
                      mode.
                    - Added animated passage to 3D mode with shaked screen
                      and ship ring.

  March 2003 v0.4.0 - Game was reduced to one lone file.
                    - Integration of files into intern resources :
                      textures, sounds & music. fmod.dll is extracted
                      at run-time in /system32 if not exist (and loaded
                      at run-time not at load-time).
                    - Extraction of font texture in user temporary dir
                      and deletion of the file when user exit the game.
                    - Use of UPX (Ultimate Packer for eXecutable) for
                      decrease the size of the executable.
                    - Added quick Fullscreen mode for running game without
                      having to change the resolution.
                    - Added loading screen.
                    - Added particle system for explosions effects.
                    - Improved inertia regulation independently from
                      system.
                    - Reorganization of functions between several
                      source files making them more comprehensive.
                    - Parameters setting and optimization of debug and
                      release VC++ modes.
                    - Added fmodvc.lib in the source archive so anyone
                      can compile without having to download additional
                      libraries (glTexFont.lib already included).
                    - Improved management of music : read of MP3 files
                      if present in game dir otherwise read and play
                      the XM default file.
                    - Background image is changing with passed levels.
                    - Sound files resampling, decrease of the exe file.
                    - Hiscore now written in window registry.
  
  March 2003 v0.3.1 - Fixed display bug (glDepthMask() func retired)
  
  Feb.  2003 v0.3.0 - The code had been written in full win32 so the game
                      can run without glut. It corrects the problem
                      of latency delay with the keyboard and works more
                      efficiently with windows management.
                    - Added menu on game startup to choose the screen
                      resolution, the color depth and the fullscreen
                      mode.
                    - Record of the hiscore from one game to another.
  
  Jan.  2003 v0.2.0 - Added installation assistant to copy glut32.dll
                      in the windows system folder.
                    - Added Shortcuts in the start menu, possibility to
                      uninstall from the installation folder, the start
                      menu or the config panel using registry records.
  
  Apr.  2002 v0.1.0 - Coding the first version of the game.

  LICENSE
  ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
  The GNU General Public License (GPL)
	
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or (at
  your option) any later version.
 
  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTA-
  BILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public
  License for more details.
 
  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  59 Temple Place, Suite 330, Boston, MA 02111-1307 USA. For complete
  terms of the license please visit http://www.opensource.org/licenses/
  gpl-license.html

  ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
                                                         Mathieu Vidalinc


                                          ***********                    
                                    ***********************              
                                 *****************************           
                               *********************************         
                              ************************************       
                             **************************************      
                            *****************************************    
                           ******************************************    
                          ********************************************   
                         **********************************************  
                        ************************************************ 
                       ************************************************* 
                    *****************************************************
                   ****************************************** ***    ****
                  *************************************  **              
                 **************************************    *             
                 ***************************************                 
                 ***************************************                 
***              **************************************                  
*******          ************************************                    
************     ***********************************                     
*****************************************************                    
******************************************************                   
******************************************************                   
*******************************************************    *****         
******************************************************  ********         
*****************************************************   *********        
*****************************************************  ******   *        
****************************************************                     
****************************************************                     
***************************************************                      
***************************************************                      
**************************************************                       
*************************************************                        
*******************************************                              
******************************************  *                            
********************************************          *                  
************************************** *      **     *                   
***************************************            ***                   
************************************             ****                    
**********************************   *        ******                     
***********************************        *********                     
*****************************************************                    
******************************************************                   
*******************************************************                  
*********************************************************                
**********************************************************             