
#include "fivewin.ch"
#include "taskpanel.ch"

PROCEDURE MAIN()

   LOCAL oWnd, oTaskPanel, cText, oMenu

   MENU oMenu
      MENUITEM "&Archivo"
      MENU
         MENUITEM "&Salir"                   ACTION oWnd:End()
      ENDMENU
      oMenu:AddMdi()
      MENUITEM "&Ver"
      MENU
         MENUITEM "&Animar"                  ACTION oTaskPanel:lAnimate := !oTaskPanel:lAnimate,;
                                                    oMenuItem:SetCheck( oTaskPanel:lAnimate )
         SEPARATOR
         MENUITEM "&Temas"
         MENU
            MENUITEM "&Classic"                 ACTION oTaskPanel:SetTheme( "#CLASSIC" ), SetCheckG( oMenuItem )
            MENUITEM "&Forever Blue"            ACTION oTaskPanel:SetTheme( "Foreverblue.dll" ), SetCheckG( oMenuItem )
            MENUITEM "&Default"      CHECKED    ACTION oTaskPanel:SetTheme(), SetCheckG( oMenuItem )
            MENUITEM "&Luna Royale"             ACTION oTaskPanel:SetTheme( "LunaRoyale.dll" ), SetCheckG( oMenuItem )
            MENUITEM "Luna Royale &Black"       ACTION oTaskPanel:SetTheme( "LunaRoyaleB.dll" ), SetCheckG( oMenuItem )
            MENUITEM "&Panther"                 ACTION oTaskPanel:SetTheme( "Panther.dll" ), SetCheckG( oMenuItem )
            MENUITEM "&Razor"                   ACTION oTaskPanel:SetTheme( "Razor.dll" ), SetCheckG( oMenuItem )
            MENUITEM "Windows &Media 10"        ACTION oTaskPanel:SetTheme( "WMedia10.dll" ), SetCheckG( oMenuItem )
            MENUITEM "Windows XP Homestead"     ACTION oTaskPanel:SetTheme( "XPGreen.dll" ), SetCheckG( oMenuItem )
            MENUITEM "Windows XP Metallic"      ACTION oTaskPanel:SetTheme( "XPGray.dll" ), SetCheckG( oMenuItem )
            MENUITEM "Windows XP NormalColor"   ACTION oTaskPanel:SetTheme( "XPBlue.dll" ), SetCheckG( oMenuItem )
         ENDMENU
         SEPARATOR
         MENUITEM  "Anchor Left"             ACTION oTaskPanel:AnchorLeft()
         MENUITEM "Anchor Right"            ACTION oTaskPanel:AnchorRight()
         SEPARATOR
         MENUITEM  "&Collapse All"           ACTION oTaskPanel:CollapseAll()
         MENUITEM  "&Expand All"             ACTION oTaskPanel:ExpandAll()
      ENDMENU
   ENDMENU


   //Definimos la Ventana Principal
   DEFINE WINDOW oWnd                     ;
         FROM 0, 0  TO 700, 550 PIXEL     ;
         TITLE "TTaskPanel 1.0"         ;
         COLOR CLR_WHITE, CLR_GRAY   ;
         MENU oMenu ;
         ICON "#1" MDI

      SET MESSAGE OF oWnd to ""

      DEFINE TASKPANEL oTaskPanel ANCHOR LEFT
         TASKGROUP "Tareas de Video" BITMAP "HEADER1" SPECIAL WATERMARK "WATERMARK"
            TASKITEM "Reproducir todo" BITMAP "ICON1" TOOLTIP "Tooltip de Prueba 1"
            TASKITEM "Obtener Imagenes desde una camara o un escaner" BITMAP "ICON2"
            TASKITEM "Copiar todos los elementos a un CD" BITMAP "ICON3"
         ENDTASKGROUP
         TASKGROUP "Tareas de archivo y carpeta sa"
            TASKITEM "Cambiar nombre a este archivo" BITMAP "ICON4"
            TASKITEM "Mover este archivo" BITMAP "ICON5"
            TASKITEM "Copiar este archivo" BITMAP "ICON6"
            TASKITEM "Publicar este archivo en web" BITMAP "ICON7"
            TASKITEM "Enviar este archivo por correo electronico" BITMAP "ICON8"
            TASKITEM "Eliminar este archivo" BITMAP "ICON9"
         ENDTASKGROUP
         TASKGROUP  "Otros sitios"
            TASKITEM "Mi PC" BITMAP "ICON10"
            TASKITEM "Mis sitios de red" BITMAP "ICON11"
         ENDTASKGROUP
         TASKGROUP  "Detalles" NO COLLAPSABLE
            TASKITEM "<b>TTaskPanel()</b>" + ;
                     "<p>Clase para la creacion de paneles de tareas</p>"+;
                     "<p><b>Autor</b></p>" +;
                     "<p>Andres Reyes Hernandez</p>" INFO
         ENDTASKGROUP
      ENDTASKPANEL

   cText:=memoread("mdimemo.prg")

   ACTIVATE WINDOW oWnd On Init memo( cText )

   RETURN
//----------------------------------------------------------------------------//

function memo(cText)
local oMemo,oWnd,oLbx,oBar

define window oWnd title "MDI Edit" MDICHILD

@ 0,0 get oMemo var cText memo of oWnd update

define buttonbar oBar of oWnd
define button of oBar message "Save" tooltip "Save" // action...
define button of oBar message "Cancel" tooltip "Cancel" // action...
oWnd:setControl(oMemo)   // call this after bar and msg config
activate window oWnd
return nil



//----------------------------------------------------------------------------//

STATIC FUNCTION SetCheckG( oMenuItem )

   AEVAL( oMenuItem:oMenu:aMenuItems, {|o| o:SetCheck( .F.) } )

   oMenuItem:SetCheck( .T. )

   RETURN nil

//----------------------------------------------------------------------------//
