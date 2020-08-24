
#include "fivewin.ch"
#include "taskpanel.ch"

PROCEDURE MAIN()

   LOCAL oWnd, oMenu, oExBar, oPanel1, oPanel2, oPanel3, oPanel4, bClick
   LOCAL o

   MENU oMenu
      MENUITEM "&Archivo"
      MENU
         MENUITEM "&Salir"                   ACTION oWnd:End()
      ENDMENU
      MENUITEM "&Ver"
      MENU
         MENUITEM "&Animar"                  ACTION oExBar:lAnimate := !oExBar:lAnimate,;
                                                    oMenuItem:SetCheck( oExBar:lAnimate )
         SEPARATOR
         MENUITEM "&Temas"
         MENU
            MENUITEM "&Classic"                 ACTION oExBar:SetTheme( "#CLASSIC" ), SetCheckG( oMenuItem )
            MENUITEM "&Forever Blue"            ACTION oExBar:SetTheme( "Foreverblue.dll" ), SetCheckG( oMenuItem )
            MENUITEM "&Default"      CHECKED    ACTION oExBar:SetTheme(), SetCheckG( oMenuItem )
            MENUITEM "&Luna Royale"             ACTION oExBar:SetTheme( "LunaRoyale.dll" ), SetCheckG( oMenuItem )
            MENUITEM "Luna Royale &Black"       ACTION oExBar:SetTheme( "LunaRoyaleB.dll" ), SetCheckG( oMenuItem )
            MENUITEM "&Panther"                 ACTION oExBar:SetTheme( "Panther.dll" ), SetCheckG( oMenuItem )
            MENUITEM "&Razor"                   ACTION oExBar:SetTheme( "Razor.dll" ), SetCheckG( oMenuItem )
            MENUITEM "Windows &Media 10"        ACTION oExBar:SetTheme( "WMedia10.dll" ), SetCheckG( oMenuItem )
            MENUITEM "Windows XP Homestead"     ACTION oExBar:SetTheme( "XPGreen.dll" ), SetCheckG( oMenuItem )
            MENUITEM "Windows XP Metallic"      ACTION oExBar:SetTheme( "XPGray.dll" ), SetCheckG( oMenuItem )
            MENUITEM "Windows XP NormalColor"   ACTION oExBar:SetTheme( "XPBlue.dll" ), SetCheckG( oMenuItem )
         ENDMENU
         SEPARATOR
         MENUITEM  "Anchor Left"             ACTION oExBar:AnchorLeft()
         MENUITEM "Anchor Right"            ACTION oExBar:AnchorRight()
         SEPARATOR
         MENUITEM  "&Collapse All"           ACTION oExBar:CollapseAll()
         MENUITEM  "&Expand All"             ACTION oExBar:ExpandAll()
      ENDMENU
   ENDMENU



   //Definimos la Ventana Principal
   DEFINE WINDOW oWnd                     ;
         FROM 0, 0  TO 700, 250 PIXEL     ;
         TITLE "TTaskPanel 1.0"         ;
         COLOR CLR_WHITE, CLR_GRAY   ;
         MENU oMenu ;
         ICON "#1"



   bClick := {|| msgbeep() }
   oExBar = TExplorerBar():New()

   oPanel1 = oExBar:AddPanel( "One" )
   oPanel1:lSpecial = .T.
   oPanel1:AddLink( "First item", bClick, "..\bitmaps\16x16\additem.bmp" )
   oPanel1:AddLink( "Second item", bClick, "..\bitmaps\16x16\copy.bmp" )

   oPanel2 = oExBar:AddPanel( "Two" )
   oPanel2:AddLink( "First item", bClick, "..\bitmaps\16x16\adddbf.bmp" )
   oPanel2:AddLink( "Second item", bClick, "..\bitmaps\16x16\delete0.bmp" )
   oPanel2:AddLink( "Third item", bClick, "..\bitmaps\16x16\envelope.bmp" )
   oPanel2:AddLink( "Fourth item", bClick, "fourthBMP" )

   oPanel3 = oExBar:AddPanel( "Three" )
   oPanel3:cHtmlText := "<b><font color=#FF00FF>TExplorer<font color=#FF0000>Bar</font>()</font></b><br>" +;
                        "<br size=5>" +;
                        "Clase para la creacion de barras de "+;
                        "exploracion al estilo de Windows XP<br>" +;
                        "<br>" +;
                        "<b>Autor</b><br>" +;
                        "<br size=5>" +;
                        "Andres Reyes Hernandez<br>"+;
                        "visita: http://codigo-base.blogspot.com"

   oPanel4 = oExBar:AddPanel( "Four" )
   oPanel4:AddLink( "First item", bClick, "firstBMP" )
   oPanel4:AddLink( "Second item", bClick, "secondBMP" )

   oExBar:DoLayout()
   oWnd:oClient = oExBar


   ACTIVATE WINDOW oWnd

   RETURN

//----------------------------------------------------------------------------//

STATIC FUNCTION SetCheckG( oMenuItem )

   AEVAL( oMenuItem:oMenu:aMenuItems, {|o| o:SetCheck( .F.) } )

   oMenuItem:SetCheck( .T. )

   RETURN nil

//----------------------------------------------------------------------------//
