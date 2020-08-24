
#include "fivewin.ch"
#include "taskpanel.ch"

PROCEDURE MAIN()

   LOCAL oDlg, oTaskPanel


   DEFINE DIALOG oDlg RESOURCE "Test"

      //REDEFINE TASKPANEL oTaskPanel ID 100 ANIMATE

   ACTIVATE DIALOG oDlg ON INIT oDlg_Init( oTaskPanel )



   RETURN

//----------------------------------------------------------------------------//

FUNCTION oDlg_Init( oTaskPanel )

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

   RETURN nil


//----------------------------------------------------------------------------//
