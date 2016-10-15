# Microsoft Developer Studio Project File - Name="Space Invaders" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Space Invaders - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Space Invaders.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Space Invaders.mak" CFG="Space Invaders - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Space Invaders - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "Space Invaders - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Space Invaders - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "CONFIG_FMOD_EXTRACT_YES" /D "_MBCS" /FR /YX /EHsc /GZ /c
# ADD CPP /nologo /MTd /W3 /GX /ZI /Od /D "_DEBUG" /D "CONFIG_FMOD_EXTRACT_YES" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D _WIN32_WINDOWS=0x0410 /FR /YX /EHsc /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG" /d "FMOD_EXTRACT_YES"
# ADD RSC /l 0x40c /d "_DEBUG" /d "FMOD_EXTRACT_YES"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib opengl32.lib glu32.lib glaux.lib fmodvc.lib delayimp.lib msvcrtd.lib kernel32.lib user32.lib advapi32.lib ole32.lib wsock32.lib msacm32.lib winmm.lib wininet.lib comctl32.lib shlwapi.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib /out:"Space Invaders OpenGL.exe" /pdbtype:sept /delayload:"fmod.dll" /delay:unload
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 fmodvc.lib msvcrtd.lib delayimp.lib glpng.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib opengl32.lib glu32.lib wsock32.lib msacm32.lib winmm.lib wininet.lib comctl32.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib /out:"Space Invaders OpenGL.exe" /pdbtype:sept /delayload:"fmod.dll" /delay:unload
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Space Invaders - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /Ob1 /Gy /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "CONFIG_FMOD_EXTRACT_YES" /D "_MBCS" /FR /YX /GF /c
# ADD CPP /nologo /MT /W3 /GX /Ob1 /Gy /D "NDEBUG" /D "CONFIG_FMOD_EXTRACT_YES" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D _WIN32_WINDOWS=0x0410 /FR /YX /GF /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG" /d "FMOD_EXTRACT_YES"
# ADD RSC /l 0x40c /d "NDEBUG" /d "FMOD_EXTRACT_YES"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib opengl32.lib glu32.lib glaux.lib fmodvc.lib delayimp.lib msvcrt.lib kernel32.lib user32.lib advapi32.lib ole32.lib wsock32.lib msacm32.lib winmm.lib wininet.lib comctl32.lib shlwapi.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib /pdbtype:sept /delayload:"fmod.dll" /delay:unload
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 fmodvc.lib msvcrt.lib delayimp.lib glpng.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib opengl32.lib glu32.lib wsock32.lib msacm32.lib winmm.lib wininet.lib comctl32.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib /out:".\Release/Space Invaders OpenGL.exe" /pdbtype:sept /delayload:"fmod.dll" /delay:unload
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Space Invaders - Win32 Debug"
# Name "Space Invaders - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "3DS Loader"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\si_3DS.cpp
DEP_CPP_SI_3D=\
	".\gl\glext.h"\
	".\gl\wglext.h"\
	".\si_3DS.h"\
	".\si_3DS_MemoryFile.h"\
	".\si_3DS_Texture.h"\
	".\si_Basecode.h"\
	".\si_Fmod.h"\
	".\si_Fmod_errors.h"\
	".\si_Font.h"\
	".\si_Fps.h"\
	".\si_Functions.h"\
	".\si_Loading.h"\
	".\si_Main.h"\
	".\si_Particles.h"\
	".\si_Resources.h"\
	".\si_World.h"\
	
# End Source File
# Begin Source File

SOURCE=.\si_3DS_MemoryFile.cpp
DEP_CPP_SI_3DS=\
	".\si_3DS_MemoryFile.h"\
	".\si_Loading.h"\
	
# End Source File
# Begin Source File

SOURCE=.\si_3DS_Texture.cpp
DEP_CPP_SI_3DS_=\
	".\gl\glext.h"\
	".\gl\wglext.h"\
	".\si_3DS_Texture.h"\
	".\si_Basecode.h"\
	".\si_Fmod.h"\
	".\si_Fmod_errors.h"\
	".\si_Font.h"\
	".\si_Fps.h"\
	".\si_Functions.h"\
	".\si_Main.h"\
	".\si_Particles.h"\
	".\si_Resources.h"\
	".\si_World.h"\
	
# End Source File
# End Group
# Begin Source File

SOURCE=.\si_Basecode.cpp
DEP_CPP_SI_BA=\
	".\gl\glext.h"\
	".\gl\wglext.h"\
	".\si_Basecode.h"\
	".\si_Fmod.h"\
	".\si_Fmod_errors.h"\
	".\si_Font.h"\
	".\si_Fps.h"\
	".\si_Functions.h"\
	".\si_Main.h"\
	".\si_Particles.h"\
	".\si_Resources.h"\
	".\si_World.h"\
	
# End Source File
# Begin Source File

SOURCE=.\si_Font.c
DEP_CPP_SI_FO=\
	".\si_Font.h"\
	
# End Source File
# Begin Source File

SOURCE=.\si_Fps.cpp
DEP_CPP_SI_FP=\
	".\si_Fps.h"\
	
# End Source File
# Begin Source File

SOURCE=.\si_Functions.cpp
DEP_CPP_SI_FU=\
	".\gl\glext.h"\
	".\gl\wglext.h"\
	".\si_Basecode.h"\
	".\si_Fmod.h"\
	".\si_Fmod_errors.h"\
	".\si_Font.h"\
	".\si_Fps.h"\
	".\si_Functions.h"\
	".\si_Loading.h"\
	".\si_Main.h"\
	".\si_Particles.h"\
	".\si_Resources.h"\
	".\si_World.h"\
	
# End Source File
# Begin Source File

SOURCE=.\si_Loading.cpp
DEP_CPP_SI_LO=\
	".\gl\glext.h"\
	".\gl\wglext.h"\
	".\si_3DS.h"\
	".\si_3DS_MemoryFile.h"\
	".\si_3DS_Texture.h"\
	".\si_Basecode.h"\
	".\si_Fmod.h"\
	".\si_Fmod_errors.h"\
	".\si_Font.h"\
	".\si_Fps.h"\
	".\si_Functions.h"\
	".\si_Loading.h"\
	".\si_Main.h"\
	".\si_Particles.h"\
	".\si_Resources.h"\
	".\si_Textures.h"\
	".\si_World.h"\
	
# End Source File
# Begin Source File

SOURCE=.\si_Main.cpp
DEP_CPP_SI_MA=\
	".\gl\glext.h"\
	".\gl\wglext.h"\
	".\si_3DS.h"\
	".\si_3DS_MemoryFile.h"\
	".\si_3DS_Texture.h"\
	".\si_Basecode.h"\
	".\si_Fmod.h"\
	".\si_Fmod_errors.h"\
	".\si_Font.h"\
	".\si_Fps.h"\
	".\si_Functions.h"\
	".\si_Main.h"\
	".\si_Particles.h"\
	".\si_Resources.h"\
	".\si_World.h"\
	
# End Source File
# Begin Source File

SOURCE=.\si_Particles.cpp
DEP_CPP_SI_PA=\
	".\gl\glext.h"\
	".\gl\wglext.h"\
	".\si_Basecode.h"\
	".\si_Fmod.h"\
	".\si_Fmod_errors.h"\
	".\si_Font.h"\
	".\si_Fps.h"\
	".\si_Functions.h"\
	".\si_Main.h"\
	".\si_Particles.h"\
	".\si_Resources.h"\
	".\si_World.h"\
	
# End Source File
# Begin Source File

SOURCE=.\si_Resources.cpp
DEP_CPP_SI_RE=\
	".\gl\glext.h"\
	".\gl\wglext.h"\
	".\si_Basecode.h"\
	".\si_Fmod.h"\
	".\si_Fmod_errors.h"\
	".\si_Font.h"\
	".\si_Fps.h"\
	".\si_Functions.h"\
	".\si_Main.h"\
	".\si_Particles.h"\
	".\si_Resources.h"\
	".\si_World.h"\
	
# End Source File
# Begin Source File

SOURCE=.\si_Textures.cpp
DEP_CPP_SI_TE=\
	".\gl\glext.h"\
	".\gl\wglext.h"\
	".\glpng.h"\
	".\si_Basecode.h"\
	".\si_Fmod.h"\
	".\si_Fmod_errors.h"\
	".\si_Font.h"\
	".\si_Fps.h"\
	".\si_Functions.h"\
	".\si_Main.h"\
	".\si_Particles.h"\
	".\si_Resources.h"\
	".\si_Textures.h"\
	".\si_World.h"\
	
# End Source File
# Begin Source File

SOURCE=.\si_World.cpp
DEP_CPP_SI_WO=\
	".\gl\glext.h"\
	".\gl\wglext.h"\
	".\si_3DS.h"\
	".\si_3DS_MemoryFile.h"\
	".\si_3DS_Texture.h"\
	".\si_Basecode.h"\
	".\si_Fmod.h"\
	".\si_Fmod_errors.h"\
	".\si_Font.h"\
	".\si_Fps.h"\
	".\si_Functions.h"\
	".\si_Main.h"\
	".\si_Particles.h"\
	".\si_Resources.h"\
	".\si_World.h"\
	
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "3DS Loader H"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\si_3DS.h
# End Source File
# Begin Source File

SOURCE=.\si_3DS_MemoryFile.h
# End Source File
# Begin Source File

SOURCE=.\si_3DS_Texture.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\si_Basecode.h
# End Source File
# Begin Source File

SOURCE=.\si_Font.h
# End Source File
# Begin Source File

SOURCE=.\si_Fps.h
# End Source File
# Begin Source File

SOURCE=.\si_Functions.h
# End Source File
# Begin Source File

SOURCE=.\si_Loading.h
# End Source File
# Begin Source File

SOURCE=.\si_Main.h
# End Source File
# Begin Source File

SOURCE=.\si_Particles.h
# End Source File
# Begin Source File

SOURCE=.\si_Resources.h
# End Source File
# Begin Source File

SOURCE=.\si_Textures.h
# End Source File
# Begin Source File

SOURCE=.\si_World.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe;wav;mp3"
# Begin Source File

SOURCE=.\sounds\1_music_jt_strng.xm
# End Source File
# Begin Source File

SOURCE=.\sounds\2_music_1mmsan.oxm
# End Source File
# Begin Source File

SOURCE=images\3dgmove.cur
# End Source File
# Begin Source File

SOURCE=sounds\applause.mp3
# End Source File
# Begin Source File

SOURCE=images\barre.bmp
# End Source File
# Begin Source File

SOURCE=images\barre3d.bmp
# End Source File
# Begin Source File

SOURCE=.\sounds\bonus.mp3
# End Source File
# Begin Source File

SOURCE=.\images\bonus1.png
# End Source File
# Begin Source File

SOURCE=images\cadre_centre.bmp
# End Source File
# Begin Source File

SOURCE=images\cadre_centre.jpg
# End Source File
# Begin Source File

SOURCE=.\images\cadre_centre_long.jpg
# End Source File
# Begin Source File

SOURCE=images\cadre_contour.bmp
# End Source File
# Begin Source File

SOURCE=images\cadre_contour.jpg
# End Source File
# Begin Source File

SOURCE=.\images\cadre_energy.png
# End Source File
# Begin Source File

SOURCE=.\images\cadre_energy_contour.png
# End Source File
# Begin Source File

SOURCE=.\images\cadre_life.gif
# End Source File
# Begin Source File

SOURCE=.\images\cadre_life.jpg
# End Source File
# Begin Source File

SOURCE=images\chiffre0.bmp
# End Source File
# Begin Source File

SOURCE=images\chiffre1.bmp
# End Source File
# Begin Source File

SOURCE=images\chiffre2.bmp
# End Source File
# Begin Source File

SOURCE=images\chiffre3.bmp
# End Source File
# Begin Source File

SOURCE=images\chiffre4.bmp
# End Source File
# Begin Source File

SOURCE=images\chiffre5.bmp
# End Source File
# Begin Source File

SOURCE=images\chiffre6.bmp
# End Source File
# Begin Source File

SOURCE=images\chiffre7.bmp
# End Source File
# Begin Source File

SOURCE=images\chiffre8.bmp
# End Source File
# Begin Source File

SOURCE=images\chiffre9.bmp
# End Source File
# Begin Source File

SOURCE=.\sounds\cross.mp3
# End Source File
# Begin Source File

SOURCE=sounds\descent.mp3
# End Source File
# Begin Source File

SOURCE=images\equalizer.gif
# End Source File
# Begin Source File

SOURCE=.\images\equalizer.png
# End Source File
# Begin Source File

SOURCE=images\equalizer_barre.gif
# End Source File
# Begin Source File

SOURCE=sounds\explosion.mp3
# End Source File
# Begin Source File

SOURCE=models\F02_512.jpg
# End Source File
# Begin Source File

SOURCE=.\models\fighter_laser.3ds
# End Source File
# Begin Source File

SOURCE=.\images\flag_en.bmp
# End Source File
# Begin Source File

SOURCE=.\images\flag_es.bmp
# End Source File
# Begin Source File

SOURCE=.\images\flag_fr.bmp
# End Source File
# Begin Source File

SOURCE=.\fmod.dll
# End Source File
# Begin Source File

SOURCE=images\fond_earth_moon.jpg
# End Source File
# Begin Source File

SOURCE=images\fond_espace.gif
# End Source File
# Begin Source File

SOURCE=images\fond_galaxy1.jpg
# End Source File
# Begin Source File

SOURCE=images\fond_planete.jpg
# End Source File
# Begin Source File

SOURCE=images\fond_skybox.bmp
# End Source File
# Begin Source File

SOURCE=images\fond_skybox.jpg
# End Source File
# Begin Source File

SOURCE=images\fond_skybox2.bmp
# End Source File
# Begin Source File

SOURCE=.\images\font.tga
# End Source File
# Begin Source File

SOURCE=images\game_about_button_mute.bmp
# End Source File
# Begin Source File

SOURCE=images\game_about_button_play.bmp
# End Source File
# Begin Source File

SOURCE=images\game_loading.bmp
# End Source File
# Begin Source File

SOURCE=images\game_startup.bmp
# End Source File
# Begin Source File

SOURCE=images\game_startup_internet.bmp
# End Source File
# Begin Source File

SOURCE=images\game_startup_internet_over.bmp
# End Source File
# Begin Source File

SOURCE=images\hand.cur
# End Source File
# Begin Source File

SOURCE=images\hand_closed.cur
# End Source File
# Begin Source File

SOURCE=images\hiscore.bmp
# End Source File
# Begin Source File

SOURCE=images\icon_app.ico
# End Source File
# Begin Source File

SOURCE=.\images\icon_music.ico
# End Source File
# Begin Source File

SOURCE=.\sounds\key.mp3
# End Source File
# Begin Source File

SOURCE=images\lives.bmp
# End Source File
# Begin Source File

SOURCE=.\models\missile.3ds
# End Source File
# Begin Source File

SOURCE=images\particle.bmp
# End Source File
# Begin Source File

SOURCE=images\particle2.bmp
# End Source File
# Begin Source File

SOURCE=images\particle2.jpg
# End Source File
# Begin Source File

SOURCE=images\particle3.gif
# End Source File
# Begin Source File

SOURCE=.\images\particle3.png
# End Source File
# Begin Source File

SOURCE=.\images\particle3_3d.gif
# End Source File
# Begin Source File

SOURCE=images\particule4.jpg
# End Source File
# Begin Source File

SOURCE=images\pixel_vert.bmp
# End Source File
# Begin Source File

SOURCE=images\pixel_vert3d.bmp
# End Source File
# Begin Source File

SOURCE=.\models\player.3ds
# End Source File
# Begin Source File

SOURCE=.\images\player.tga
# End Source File
# Begin Source File

SOURCE=.\images\player3d.tga
# End Source File
# Begin Source File

SOURCE=.\images\png1.bin
# End Source File
# Begin Source File

SOURCE=images\powerup.jpg
# End Source File
# Begin Source File

SOURCE=resource.h
# End Source File
# Begin Source File

SOURCE=.\resource.rc
# End Source File
# Begin Source File

SOURCE=sounds\respawn.mp3
# End Source File
# Begin Source File

SOURCE=models\S02_5121.jpg
# End Source File
# Begin Source File

SOURCE=images\score.bmp
# End Source File
# Begin Source File

SOURCE=sounds\shield_charge.mp3
# End Source File
# Begin Source File

SOURCE=.\models\ship.3ds
# End Source File
# Begin Source File

SOURCE=images\ship1.bmp
# End Source File
# Begin Source File

SOURCE=images\ship1_b.bmp
# End Source File
# Begin Source File

SOURCE=images\ship2.bmp
# End Source File
# Begin Source File

SOURCE=images\ship2_b.bmp
# End Source File
# Begin Source File

SOURCE=images\ship3.bmp
# End Source File
# Begin Source File

SOURCE=images\ship3_b.bmp
# End Source File
# Begin Source File

SOURCE=sounds\shoot.mp3
# End Source File
# Begin Source File

SOURCE=sounds\shoot_3d.mp3
# End Source File
# Begin Source File

SOURCE=.\images\sphere.jpg
# End Source File
# Begin Source File

SOURCE=models\sphere_o.jpg
# End Source File
# Begin Source File

SOURCE=.\models\sphere_ogive.3ds
# End Source File
# Begin Source File

SOURCE=.\images\super_ship.tga
# End Source File
# Begin Source File

SOURCE=.\models\supership.3ds
# End Source File
# Begin Source File

SOURCE=images\text_bonus.gif
# End Source File
# Begin Source File

SOURCE=images\text_weapon.gif
# End Source File
# Begin Source File

SOURCE=images\tir.bmp
# End Source File
# Begin Source File

SOURCE=images\tir_3d_base.jpg
# End Source File
# Begin Source File

SOURCE=images\tir_3d_base_dark_backgrd.jpg
# End Source File
# Begin Source File

SOURCE=images\tir_3d_base_double.jpg
# End Source File
# Begin Source File

SOURCE=images\tir_ship.bmp
# End Source File
# Begin Source File

SOURCE=sounds\weapon.mp3
# End Source File
# End Group
# Begin Group "Other Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=Readme.txt
# End Source File
# Begin Source File

SOURCE=.\Readme_es.txt
# End Source File
# Begin Source File

SOURCE=.\Readme_fr.txt
# End Source File
# End Group
# End Target
# End Project
