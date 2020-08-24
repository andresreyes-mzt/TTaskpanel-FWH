
#include "fivewin.ch"
#include "taskpanel.ch"
#define SWP_NOSIZE          0x0001
#define SWP_NOMOVE          0x0002


PROCEDURE MAIN()

   LOCAL oDlg, oTaskPanel


   DEFINE DIALOG oDlg RESOURCE "Test"

      REDEFINE TASKPANEL oTaskPanel ID 100 ANIMATE

   ACTIVATE DIALOG oDlg ON INIT oDlg_Init( oTaskPanel )



   RETURN

//----------------------------------------------------------------------------//

FUNCTION oDlg_Init( oTaskPanel )
   LOCAL oWnd := oTaskPanel:oWnd
   LOCAL oBrw, aArray
   LOCAL cNombre := Space(256)
   LOCAL cParte  := Space(256)
   LOCAL cBuscar := ""
   LOCAL cTipo   := ""
   LOCAL aCheck  := { .T., .F., .T., .F., .F. }
   LOCAL nRadio  := 1


      DEFINE TASKPANEL oTaskPanel
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

      oWnd:oLeft   := oTaskPanel
      oWnd:oClient := oBrw
	  oWnd:Resize()	  
	  
	  BringWindowToTop( oBrw:hWnd )
	  //SetWindowPos( oBrw:hWnd, 0,,,,,SWP_NOMOVE|SWP_NOSIZE)
	  

   RETURN nil


//----------------------------------------------------------------------------//
