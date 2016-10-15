
  Space Invaders OpenGL 0.6.1                                    mai 2004
  ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
 
  PRÉSENTATION
  ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
  Space Invaders OpenGL est un clone du jeu Space Invaders (un classique
  de la fin des années 70, développé avec l'API OpenGL. Le but est de
  faire un jeu ressemblant à l'original mais avec des améliorations
  visuelles en 2D et 3D.

  Sources et bibliothèques additionnelles :
             - Nehe Basecode (http://nehe.gamedev.net)
             - FMOD Sound System (http://www.fmod.org)
             - GLTexFont (http://nate.scuzzy.net/programming/gltexfont)

  Musiques : - 1 000 000 miles by ronny / teklords (Jeu)
             - StrangleHold (C)JT (A Propos)
             - Digitally Imported http://www.di.fm (Streaming)
  
  Note : Lors de la sélection de l'option streaming une requête est
  envoyée à google.com pour lancer la connexion Internet et/ou le
  paramétrage de pare-feu.

  RECOMMANDATIONS LOGICIEL/MATÉRIEL
  ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
  - Windows 98/ME/2000/XP
  - Carte vidéo compatible OpenGL (Nvidia GeForce, Ati Radeon...)

  CONTRÔLES
  ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
  Flèche Haut, Ctrl -  Tir
  Flèche Gauche     -  Aller à gauche
  Flèche Droite     -  Aller à droite
  Souris            -  Rotation de la Scène
  NumPad +          -  Zoom avant
  NumPad -          -  Zoom arrière
  B                 -  Changer le fond du jeu
  M                 -  Musique On/Off
  R                 -  Replace la caméra
  A                 -  Anti-crénelage on/off (principalement pour la 3D)
  <F5>+<F6>         -  Tricher
  Echap             -  Quitter
  
  CONTACT
  ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
  Pour laisser un message concernant un problème ou autre vous pouvez
  utiliser le forum à l'adresse ci-dessous :
  https://sf.net/forum/?group_id=74529

             Auteur:    Mathieu Vidalinc
             E-mail:    <maya75@users.sourceforge.net>
             Forum:     https://sourceforge.net/forum/?group_id=74529
             Page Web:  http://spaceinvadersgl.sourceforge.net

  HISTORIQUE DES VERSIONS
  ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
  May   2004 v0.6.1 - Added support for custom stream URL (Shoutcast
                      playlist (*.pls), Shoutcast stream or mp3 stream.
                      Retrieve now the name of artist and song.
                    - Added recording of startup options in registry.
                    - Added support for Wivik on-screen virtual keyboard.
                    - Added PNG support for using alpha component
                      (translucence, antialiasing and shadow).
                      Textures updated/added : equalizer, frames, energy.
  
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

  LICENCE
  ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
  La GNU General Public License (GPL)
	
  Ce programme est libre, vous pouvez le redistribuer et/ou le modifier
  selon les termes de la Licence Publique Générale GNU publiée par la
  Free Software Foundation (version 2 ou bien toute autre version
  ultérieure choisie par vous).

  Ce programme est distribué car potentiellement utile, mais SANS AUCUNE
  GARANTIE, ni explicite ni implicite, y compris les garanties de
  commercialisation ou d'adaptation dans un but spécifique. Reportez-vous
  à la Licence Publique Générale GNU pour plus de détails.

  Vous devez avoir reçu une copie de la Licence Publique Générale GNU en
  même temps que ce programme ; si ce n'est pas le cas, écrivez à la Free
  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111
  -1307, États-Unis. Pour connaître les termes complets de la licence
  veuillez visiter  http://www.opensource.org/licenses/gpl-license.html

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