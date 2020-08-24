
#include "fivewin.ch"
#include "taskpanel.ch"

PROCEDURE MAIN()

   LOCAL oWnd, oTaskPanel, oMenu

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
            MENUITEM "&Custom"       CHECKED    ACTION oTaskPanel:SetTheme( "#CUSTOM" ), SetCheckG( oMenuItem )
            MENUITEM "&Forever Blue"            ACTION oTaskPanel:SetTheme( "Foreverblue.dll" ), SetCheckG( oMenuItem )
            MENUITEM "&Default"                 ACTION oTaskPanel:SetTheme(), SetCheckG( oMenuItem )
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

   DEFINE THEME NAME "#CUSTOM"
      ELEMENT TASKPANEL ;
               BACKGROUND RGB( 255, 125, 0 );
               PADDING 2;
               BORDERTHICKNESS 0

         ELEMENT SPECIAL GROUP
            ELEMENT HEADER ;
                    BACKGROUND RGB( 51, 51, 51 );
                    FOREGROUND RGB( 255, 255, 255 );
                    BORDERTHICKNESS 0;
                    BUTTON COLLAPSE {};
                    BUTTON EXPAND {};
                    MARGIN TO PANEL 10
            ENDELEMENT
            ELEMENT TASKLIST;
                    BACKGROUND RGB( 255, 152, 0 ) ;
                    BORDERTHICKNESS 0;
                    PADDING 0;
                    MARGIN TO TASK 2
               ELEMENT TASK ;
                       BACKGROUND { RGB( 255, 204, 104 ), RGB(255,248,223), -1 } ;
                       FONTSTYLE { "", "" };
                       FOREGROUND RGB( 0, 0, 0 );
                       PADDING { 6, 2, 4, 2 }
               ENDELEMENT
            ENDELEMENT
         ENDELEMENT

         ELEMENT NORMAL GROUP
            ELEMENT HEADER ;
                    BACKGROUND RGB( 51, 51, 51 );
                    FOREGROUND RGB( 255, 255, 255 );
                    BORDERTHICKNESS 0;
                    BUTTON COLLAPSE {};
                    BUTTON EXPAND {};
                    MARGIN TO PANEL 10
            ENDELEMENT
            ELEMENT TASKLIST;
                    BACKGROUND RGB( 255, 152, 0 ) ;
                    BORDERTHICKNESS 0;
                    PADDING 0;
                    MARGIN TO TASK 2
               ELEMENT TASK ;
                       BACKGROUND { RGB( 255, 204, 104 ), RGB(255,248,223), -1 } ;
                       FONTSTYLE { "", "" };
                       FOREGROUND RGB( 0, 0, 0 );
                       PADDING { 6, 2, 4, 2 }
               ENDELEMENT
            ENDELEMENT
         ENDELEMENT

      ENDELEMENT
   ENDTHEME

   //Definimos la Ventana Principal
   DEFINE WINDOW oWnd                     ;
         FROM 0, 0  TO 600, 250 PIXEL     ;
         TITLE "TTaskBar 1.0"         ;
         COLOR CLR_WHITE, CLR_GRAY   ;
         MENU oMenu ;
         ICON "#1"

      DEFINE TASKPANEL oTaskPanel ANCHOR LEFT;
                           THEME "#CUSTOM";
                           HEADER "LOGO"

         TASKGROUP "Tareas de Video"  SPECIAL
            TASKITEM "Reproducir todo"
            TASKITEM "Obtener Imagenes desde una camara o un escaner"
            TASKITEM "Copiar todos los elementos a un CD"
         ENDTASKGROUP
         TASKGROUP "Tareas de archivo y carpeta"
            TASKITEM "Cambiar nombre a este archivo"
            TASKITEM "Mover este archivo"
            TASKITEM "Copiar este archivo"
            TASKITEM "Publicar este archivo en web"
            TASKITEM "Enviar este archivo por correo electronico"
            TASKITEM "Eliminar este archivo"
         ENDTASKGROUP
         TASKGROUP  "Otros sitios"
            TASKITEM "Mi PC"
            TASKITEM "Mis sitios de red"
         ENDTASKGROUP
         TASKGROUP  "Detalles" NO COLLAPSABLE
            TASKITEM "<b>TTaskPanel()</b>" + ;
                     "<p>Clase para la creacion de paneles de tareas</p>"+;
                     "<p><b>Autor</b></p>" +;
                     "<p>Andres Reyes Hernandez</p>" INFO
         ENDTASKGROUP
      ENDTASKPANEL

      oTaskPanel:Item(1):Item(1):Hide()

   ACTIVATE WINDOW oWnd

   RETURN

//----------------------------------------------------------------------------//

STATIC FUNCTION SetCheckG( oMenuItem )

   AEVAL( oMenuItem:oMenu:aMenuItems, {|o| o:SetCheck( .F.) } )

   oMenuItem:SetCheck( .T. )

   RETURN nil

//----------------------------------------------------------------------------//
