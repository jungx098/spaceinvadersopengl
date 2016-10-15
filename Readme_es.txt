
  Space Invaders OpenGL 0.6.1                                   Mayo 2004
  ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
 
  Traducción (inglés a español)
  ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
  El presente documento, y juego, ha sigo traducido del inglés al español
  por
         Autor:      Antonio Caballero Martínez
         E-mail:     <war3d@ono.com>
         Página Web: La Panocha Digital - The Digital CornCob
                     (http://www.panochadigital.f2o.org/)
                     GnuWin, Open Your Windows (http://gnuwin.epfl.ch/)

  ACERCA DE
  ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
  Space Invaders OpenGL es un clon del Space Invaders (un juego arcade
  clásico de comienzo de los 80´s), desarrollando con el API del OpenGL.
  El propósito es realizar un juego similar en aspecto al original pero
  con mejoras y efectos 2D / 3D.

  Librerías/fuentes adicionales usadas para el juego:
            - Nehe Basecode (http://nehe.gamedev.net)
            - FMOD Sound System (http://www.fmod.org)
            - GLTexFont (http://nate.scuzzy.net/programming/gltexfont)

  Sonidos:  - 1 000 000 miles by ronny / teklords (Game)
            - StrangleHold (C)JT (About)
            - Digitally Imported http://www.di.fm (Streaming)
  
  Nota: cuando se selecciona una emisora on-line se envía una petición
  a www.google.com para lanzar la conexión a Internet y/o configuración
  de cortafuegos.

  REQUIREMENTOS DEL SISTEMA
  ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
  - Windows 98/ME/2000/XP
  - Tarjeta gráfica compatible con OpenGL (Nvidia GeForce, Ati Radeon...)

  TECLAS
  ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
  Flecha Arriba,Ctrl -  Disparo
  Left Arrow         -  Desplazar a la izquierda
  Right Arrow        -  Desplazar a la derecha
  Mouse              -  Rotación de la escena
  NumPad +           -  Zoom In
  NumPad -           -  Zoom Out
  B                  -  Cambiar fondo
  M                  -  Sonido Si/No
  R                  -  Redimensionar ventana
  A                  -  Anti-aliasing Si/No (principalmente en modo 3D)
  <F5>+<F6>          -  Trucos
  Escape             -  Salir
  
  FORO
  ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
  Si tiene un problema, deje un mensaje en el foro (en inglés):
  https://sourceforge.net/forum/?group_id=74529

             Autor:       Mathieu Vidalinc (english/french)
             E-mail:      <maya75@users.sourceforge.net>
             Forum:       https://sourceforge.net/forum/?group_id=74529
             Página Web:  http://spaceinvadersgl.sourceforge.net

  VERSION HISTORY
  ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
  Mayo  2004 v0.6.1 - Se añade soporte para personalizar la URL de la
                      emisora (Lista reproducción (*.pls), emisora o
					  archivos mp3.
                      Aparece ahora el nombre del artista y la canción.
                    - Se añaden opciones de comienzo en el registro.
                    - Se añade soporte para los teclados virtuales Wivik.
                    - Se añade soporte para los gráficos PNG usando
                      componentes alfa de textura (translúcido,
                      antialiasing y sombras). Texturas actualizadas/
                      añadidas : ecualizador, frames, energía.
  
  Enero 2004 v0.6.0 - Se añade un vector (Compiled Vertex Array) especial
                      para los objetos 3D dibujados muchas veces (como
                      las naves). Estos objetos se cargan en la memoria
                      GPU para un renderizado más rápido.
                      Necesita una tarjeta gráfica compatible (todas las
                      últimas).
                    - Corregido un bug al entrar en el modo 3D.
                    - Ahora se cargan los modelos 3D desde la memoria
                      interna.
  
  Diciembre 2003 v0.5.9 - Se mejora el modo 3D lineal (se eliminan los
                      artefactos)
                    - Se añade la opción de Anti-aliasing (principalmente
                      modelos de llenado 3D)
                    - Se corrige finalmente el bug del cambio de música
                      (variable externa)
                    - Se improvisa un ecualizador, y explosiones.
                    - Se añaden bonus en el modo 3D (mini naves,
                      armamentos)
                    - Se decrementa el tamaño del ejecutable.
                    - Las naves en el modo 3D tienen ahora "puntos de vida".

  Septiembre 2003 v0.5.8 - Se reduce el tamaño del ejecutable.
             v0.5.7 - Algunas pequeñas mejoras en el código.
             v0.5.6 - Se mejora la lectura de archivos musicales mp3
                      leyendo los archivos en segundo plano sin que afecte
                      al frame-rate (uso de la nueva bandera FSOUND_
                      NONBLOCKING)
                    - Se añade la funcionalidad del ancho de banda de
                      Internet musical.
                    - Se añade el ecualizador de espectros.
             v0.5.5 - Corregido un bug mientras se reproducían archivos
                      mp3 ("cierre" de la lista)
                    - Se mejora manualmente el mapeo (mipmapping).
                    - Se mejoran los niveles, disparos, y el código fuente.
                    - Se añade scroll a los títulos de los mp3.
  
  Agosto 2003 v0.5.4 - Se corrige un bug en el que el archivo "msvcr70.dll"
                      falta (fase de linkeo)
                    - Se deshabilita la sincronización vertical, ahora
                      funciona incluso si está activado
                      por defecto en las propiedades de pantalla.
                    - Se mejora la carga de los procesos sin hacer uso de
                      las propiedades de thread.
                    - Se añade una consola de Debug (² key). Necesita una
                      resolución de 1024x768.
                    - Se añade un "viewport" (mini-ventana) en la consola
                      de Debug.
                    - Se añade el modo lineal (juego 3D).
                    - Se añade la tecla <ctrl> para disparar.
                    - Se añade la actualización del record "Hiscore"
                      mientras se juega.
  
  Agosto 2003 v0.5.3 - Nueva pantalla de inicio con nuevas opciones: VSYNC,
                      billboarding, sombras suaves.
                    - Se añade Error de Handle.
                    - Se añade la función de sincronización Vertical.
                    - Se añade un análisis espectral para la detección de
                      choques.
                    - Se añade la ruta de la cámara (curvas de Bézier)
  
  Julio 2003 v0.5.1 - Se añade billboarding para el sistema de partículas
                      (explosiones, disparos) es decir: estos sprites
                      parecen ahora ser en 3D.
                    - Se añade una opción de velocidad de carga que hace
                      que la prioridad de carga de los thread sean mayores
                      (aproximadamente el doble de rápido)
                    - Se añaden utilidades multilíneas en la ventana de
                      inicio.
                    - Se añade la barra de progreso durante la carga.
                    - Se añade la rutina de chequeo de versión de los fmod
                      por reemplazo.

  Junio 2003 v0.5.0 - Se añade multiproceso para la carga en distintos
                      procesos (thread), los cuales refrescan la ventana
                      y permiten moverla/ cerrarla cambiando la prioridad
                      al mínimo, y entonces volver a normal.
                    - Se añade la ventana "Acerca de ..." (diseño/léame/
                      música).
  
  Mayo  2003 v0.4.9 - Se añade el modo 3D al juego. 3DS sistema de objetos/
                      particulas disparos/luces/reactores de las naves/etc.
                    - Se mejora la pantalla de inicio: habilitada/
                      deshabilitada botones radio, botón mipmap. ventana
                      centrada en el modo ventana.
                    - Se añade un pasaje animado al modo 3D con una
                      venatana movida y un anillo a la nave.

  Marzo 2003 v0.4.0 - Se reducen los archivos del juego a un único
                      ejecutable.
                    - Se integran los ficheros en los recursos internos:
                      texturas, sonidos y música. fmod.dll es colocada
                      en el directorio /system32 como una librería si no
                      existe (y cargada en tiempo de ejecución, no en
                      tiempo de carga).
                    - Se extrae la textura de las fuentes en el directorio
                      temporal de los usuarios y se borra el archivo
                      cuando el usuario sale del juego.
                    - Se utiliza UPX (Ultimate Packer for eXecutable,
                      compresor para ejecutables) para reducir el tamaño
                      del ejecutable.
                    - Se añade un modo de pantalla completa rápido para
                      ejecutar el juego sin tener que cambiar la resolución.
                    - Se añade la ventana de carga.
                    - Se añade el sistema de partículas para los efectos
                      de explosiones.
                    - Se implementa la regulación de la inercia
                      independiente del sistema.
                    - Reorganización de las funciones entre bastantes
                      archivos de código fuente
                      haciéndolos más comprensibles y clarificadores.
                    - Configuración de parámetros y optimización del debug
                      y los modos de realización VC++.
                    - Se añade fmodvc.lib en el archivo fuente por lo que
                      cualquiera puede compilarlo sin tener que bajarse
                      librerías adicionales (también se incluye
                      glTexFont.lib).
                    - Se mejora el gestor de la música: la lectura de los
                      archivos MP3
                      si están presentes en el directorio del juego y en
                      cualquier otro caso se lee y reproduce el fichero
                      por defectoe XM.
                    - La imagen de fondo cambia confirme se avanzan en los
                      diferentes niveles.
                    - Se mezclan los archivos de sonido (resampling),
                      decremento del tamaño del ejecutable.
                    - Ahora el record (Hiscore) se almacena en el
                      registro de window.
  
  Marzo 2003 v0.3.1 - Corregido un bug de pantalla (glDepthMask() se
                      requiere la función glDepthMask())
  
  Febrero 2003 v0.3.0 - El código se ha escrito completamente en win32 por
                      lo que el juego se puede ejecutar sin glut. Se
                      corrige el problema del retraso en la latencia con
                      el teclado y se mejora la eficiencia con windows.
                    - Se añade un menú al juego en el inicio para escoger
                      la resolución de pantalla, la profundidad del color
                      y el modo a pantalla completa.
                    - Se almacena el record de puntuación de un juego para
                      el siguiente.
  
  Enero 2003 v0.2.0 - Se añade un asistente en la instalación para copiar
                      glut32.dll en la carpeta de sistema de windows.
                    - Se añade un acceso directo en el menú inicio,
                      posibilitando la desinstalación desde la carpeta de
                      instalación, el menú inicio o el panel de control
                      usando grabaciones en el registro.
  
  Abril 2002 v0.1.0 - La primera versión del juego ve la luz.

  LICENCIA
  ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
  La Licencia Pública General GNU (GPL)
	
  Este programa es software gratuito; puede redistribuirlo y/o modificarlo
  bajo los términos de la Licencia Pública General GNU (GPL) publicada por
  la Free Software Foundation; desde la versión 2 de la Licencia, o (en
  su opción) a una versión posterior.
 
  Este programa se distribuye con el propósito de ser util, pero
  SIN NINGUNA GARANTÍA; sin incluso la implicación de MERCANTIBILIDAD o
  PROPÓSITOS COMERCIALES PERSONALES. Vea la Licencia Pública General GNU
  (GPL) para más detalles.
 
  Debe haber recibido una copia de la Licencia Pública General GNU (GPL)
  con el programa; si no, escriba a Free Software Foundation, Inc.,
  59 Temple Place, Suite 330, Boston, MA 02111-1307 USA. Para completar
  los términos de la licencia, visite por favor
  http://es.gnu.org/Licencias/fdles.html

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