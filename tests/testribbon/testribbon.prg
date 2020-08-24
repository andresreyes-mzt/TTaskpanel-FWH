
#include "fivewin.ch"
#include "taskpanel.ch"
#include "ribbon.ch"


PROCEDURE MAIN()

   LOCAL oWnd, oTaskPanel
   LOCAL oRBar, oGr, oBtn1, oBtn2, oBtn3, oPanel

   //Definimos la Ventana Principal
   DEFINE WINDOW oWnd                     ;
         FROM 1, 1  TO 600, 600 PIXEL     ;
         TITLE "TTaskPanel 1.0"         ;
         COLOR CLR_WHITE, CLR_GRAY   ;
         ICON "#1"

   DEFINE RIBBONBAR oRBar WINDOW oWnd PROMPT "Configuración", "Ficheros", "Informes", "Ayudas" HEIGHT 133 TOPMARGIN 25

   oRBar:nLeftMargin = 75
   oRBar:CalcPos()

   ADD GROUP oGr RIBBON oRBar TO OPTION 1 PROMPT "Test" width 200

   @ 2,5 ADD BUTTON oBtn1 PROMPT "Opcion #1" GROUP oGr ACTION ( SetOption1( oPanel ) );
   ROUND SIZE 60,65

   @ 2,70 ADD BUTTON oBtn1 PROMPT "Opcion #2" GROUP oGr ACTION ( SetOption2( oPanel ) ) ;
   ROUND SIZE 60,65

   @ 2,135 ADD BUTTON oBtn3 PROMPT "Ocultar" GROUP oGr ACTION ( oPanel:Hide() ) ;
   ROUND SIZE 60,65

   oPanel := TPanel():New(,,,, oWnd)
   oPanel:Hide()

   BrowseTest( oPanel )

   oWnd:oClient := oPanel
   oWnd:Resize()

   ACTIVATE WINDOW oWnd

   RETURN

FUNCTION BrowseTest( oPanel )

   LOCAL oBrw, oTaskPanel, aArray

      DEFINE TASKPANEL oTaskPanel OF oPanel ANCHOR LEFT
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

      aArray := {}
      aAdd( aArray , { "Juan Domingo Lopez Aramburo", "Rio Panuco #454645", "6699843023"} )
      aAdd( aArray , { "Mario Rosales Montevideo", "Rio Presidio #546564", "6699879788"} )
      aAdd( aArray , { "Felipe de Jesus Ramirez Lopez", "Rio de la Loma #18795", "6695644556"} )
      aAdd( aArray , { "Renesio Recto", "Rio Florido #56546", "6699565464"} )

      oBrw := TXBrowse():New( oPanel )

      oBrw:SetArray( aArray )

      oBrw:aCols[1]:cHeader := "Nombre"
      oBrw:aCols[2]:cHeader := "Direccion"
      oBrw:aCols[3]:cHeader := "Telefono"

      oBrw:CreateFromCode()

      oPanel:oClient := oBrw
      oPanel:Resize()

   RETURN Nil

FUNCTION SetOption1( oPanel )
   LOCAL oTaskPanel := oPanel:oLeft

   oTaskPanel:SuspendLayout()
   oTaskPanel:Item(1):Show()
   oTaskPanel:Item(2):Hide()
   oTaskPanel:Item(3):Item(1):Show()
   oTaskPanel:Item(3):Item(2):Hide()
   oTaskPanel:EndLayout()

   oPanel:Show()

   RETURN Nil

FUNCTION SetOption2( oPanel )
   LOCAL oTaskPanel := oPanel:oLeft

   oTaskPanel:SuspendLayout()
   oTaskPanel:Item(1):Hide()
   oTaskPanel:Item(2):Show()
   oTaskPanel:Item(3):Item(1):Hide()
   oTaskPanel:Item(3):Item(2):Show()
   oTaskPanel:EndLayout()


   oPanel:Show()
   RETURN Nil

//----------------------------------------------------------------------------//
