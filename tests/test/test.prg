
#include "fivewin.ch"
#include "taskpanel.ch"

PROCEDURE MAIN()

   LOCAL oWnd, oTaskPanel, oMenu
   LOCAL o

   MENU oMenu
      MENUITEM "&Archivo"
      MENU
         MENUITEM "&Salir"                   ACTION oWnd:End()
      ENDMENU
      MENUITEM "&Ver"
      MENU
         MENUITEM "&Animar"                  ACTION oTaskPanel:lAnimate := !oTaskPanel:lAnimate,;
                                                    oMenuItem:SetCheck( oTaskPanel:lAnimate )
         SEPARATOR
         MENUITEM "&Temas"
         MENU
            MENUITEM "&Classic"                 ACTION oTaskPanel:SetTheme( "#CLASSIC" ), SetCheckG( oMenuItem )
			MENUITEM "C&ustom"                  ACTION oTaskPanel:SetTheme( "#CUSTOM" ), SetCheckG( oMenuItem )
            MENUITEM "&Black && White"          ACTION oTaskPanel:SetTheme( "#Black&White" ), SetCheckG( oMenuItem )
			MENUITEM "&Pink"                    ACTION oTaskPanel:SetTheme( "#Pink" ), SetCheckG( oMenuItem )
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
         FROM 0, 0  TO 700, 250 PIXEL     ;
         TITLE "TTaskPanel 1.0"         ;
         COLOR CLR_WHITE, CLR_GRAY   ;
         MENU oMenu ;
         ICON "#1"

      DEFINE TASKPANEL oTaskPanel THEME "#BLACK&WHITE" ANCHOR LEFT
         TASKGROUP "Tareas de Video" BITMAP "HEADER1" SPECIAL WATERMARK "WATERMARK"
            TASKITEM "Reproducir todo" BITMAP "ICON1" TOOLTIP "Tooltip de Prueba 1"
            TASKITEM "Obtener Imagenes desde una camara o un escaner" BITMAP "ICON2"
            TASKITEM "Copiar todos los elementos a un CD" BITMAP "ICON3"
         ENDTASKGROUP
         TASKGROUP "Tareas de archivo y carpeta"
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
     

   ACTIVATE WINDOW oWnd

   RETURN

//----------------------------------------------------------------------------//

STATIC FUNCTION SetCheckG( oMenuItem )

   AEVAL( oMenuItem:oMenu:aMenuItems, {|o| o:SetCheck( .F.) } )

   oMenuItem:SetCheck( .T. )

   RETURN nil

//----------------------------------------------------------------------------//
