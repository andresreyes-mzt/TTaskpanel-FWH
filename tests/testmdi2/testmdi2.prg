
#include "fivewin.ch"
#include "taskpanel.ch"
#include "xbrowse.ch"

PROCEDURE MAIN()

   LOCAL oWnd, oMenu

   MENU oMenu
      MENUITEM "&Archivo"
      MENU
         MENUITEM "&Salir"                   ACTION oWnd:End()
      ENDMENU
      oMenu:AddMdi()
   ENDMENU


   //Definimos la Ventana Principal
   DEFINE WINDOW oWnd                     ;
         FROM 0, 0  TO 720, 900 PIXEL     ;
         TITLE "TTaskPanel 1.0"         ;
         COLOR CLR_WHITE, CLR_GRAY   ;
         MENU oMenu ;
         ICON "#1" MDI



   ACTIVATE WINDOW oWnd On Init BrowseTest()

   RETURN
//----------------------------------------------------------------------------//

FUNCTION BrowseTest()

   LOCAL oWnd, oBrw, oTaskPanel, aArray, nFor
   LOCAL cNombre := Space(256)
   LOCAL cParte  := Space(256)
   LOCAL cBuscar := ""
   LOCAL cTipo   := ""
   LOCAL aCheck  := { .T., .F., .T., .F., .F. }
   LOCAL nRadio  := 1




   DEFINE WINDOW oWnd TITLE "MDI Test Taskpanel & Browse" MDICHILD FROM 10, 10 TO 650, 800 PIXEL

      oWnd:Hide()

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


            aArray := {}
      aAdd( aArray , { "Juan Domingo Lopez Aramburo", "Rio Panuco #454645", "6699843023"} )
      aAdd( aArray , { "Mario Rosales Montevideo", "Rio Presidio #546564", "6699879788"} )
      aAdd( aArray , { "Felipe de Jesus Ramirez Lopez", "Rio de la Loma #18795", "6695644556"} )
      aAdd( aArray , { "Renesio Recto", "Rio Florido #56546", "6699565464"} )

      oBrw := TXBrowse():New( oWnd )

      oBrw:SetArray( aArray )

      oBrw:aCols[1]:cHeader := "Nombre"
      oBrw:aCols[2]:cHeader := "Direccion"
      oBrw:aCols[3]:cHeader := "Telefono"

      oBrw:CreateFromCode()

      oWnd:setControl(oBrw)


   ACTIVATE WINDOW oWnd

   RETURN Nil


//----------------------------------------------------------------------------//
