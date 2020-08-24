
#include "fivewin.ch"
#include "taskpanel.ch"

PROCEDURE MAIN()

   LOCAL oWnd, oTaskPanel, oMenu
   LOCAL cNombre := Space(256)
   LOCAL cParte  := Space(256)
   LOCAL cBuscar := ""
   LOCAL cTipo   := ""
   LOCAL aCheck  := { .T., .F., .T., .F., .F. }
   LOCAL nRadio  := 1

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
         FROM 0, 0  TO 700, 250 PIXEL     ;
         TITLE "TTaskPanel 1.0"         ;
         COLOR CLR_WHITE, CLR_GRAY   ;
         MENU oMenu ;
         ICON "#1"

      DEFINE TASKPANEL oTaskPanel ANCHOR LEFT BUBBLE
         TASKGROUP "Buscar según los siguientes criterios" ;
                  NO COLLAPSABLE
            @ 10, 10 SAY "Todo o parte del nombre del archivo:" SIZE 179,13 PIXEL
            @ 25, 10 GET cNombre SIZE 179, 20 PIXEL
            @ 53, 10 SAY "Una palabra o frase en el archivo:" SIZE 179, 13 PIXEL
            @ 68, 10 GET cParte SIZE 179, 20 PIXEL
            @ 96, 10 SAY "Buscar en:" SIZE 179, 13 PIXEL
            @ 111, 10 COMBOBOX cBuscar ITEMS {"Mi PC", "Mis Sitios de Red", "Mis Documentos" } SIZE 179, 20 PIXEL
         ENDTASKGROUP
         TASKGROUP "¿Cuando fue modificado?" COLLAPSED
            @ 10, 10 RADIO nRadio ITEMS "No lo recuerdo","La semana pasada", "El mes pasado", "El año pasado", "Especificar fechas" SIZE 179, 17  PIXEL
         ENDTASKGROUP
         TASKGROUP "¿Que tamaño Tiene?" COLLAPSED
         ENDTASKGROUP
         TASKGROUP "¿Mas opciones avanzadas?"
            @ 10, 10 SAY "Todo o parte del nombre del archivo:" SIZE 179,13 PIXEL
            @ 25, 10 COMBOBOX cTipo ITEMS {"(Todos los archivos y carpetas)", "Aplicacion", "Aplicacion MS-DOS" } SIZE 179, 20 PIXEL
            @ 50, 10 CHECKBOX aCheck[1] PROMPT "Buscar en carpetas de sistema" SIZE 162, 26 PIXEL
            @ 80, 10 CHECKBOX aCheck[2] PROMPT "Buscar en archivos y carpetas ocultas" SIZE 162, 26 PIXEL
            @ 110, 10 CHECKBOX aCheck[3] PROMPT "Buscar en subcarpetas" SIZE 162, 13 PIXEL
            @ 127, 10 CHECKBOX aCheck[4] PROMPT "Distinguir mayusculas de minusculas" SIZE 162, 26 PIXEL
            @ 157, 10 CHECKBOX aCheck[5] PROMPT "Buscar en cinta de copia de seguridad" SIZE 162, 26 PIXEL
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
