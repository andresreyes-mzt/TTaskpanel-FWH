/**************************************************************************************************
* Project      : TTaskpanel
* File         : n_TThemeManager.prg
*
* Copyright 2010 Andres Reyes Hernandez - <andresreyes_mzt/at/yahoo.com.mx>
* http://codigo-base.blogspot.com
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this software; see the file LICENCIA. If not, write to
* the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
* Boston, MA 02111-1307 USA (or visit the web site http://www.gnu.org/).
*
* As a special exception, the TTaskpanel Project gives permission for
* additional uses of the text contained in its release of TTaskpanel.
*
* The exception is that, if you link the TTaskpanel libraries with other
* files to produce an executable, this does not by itself cause the
* resulting executable to be covered by the GNU General Public License.
* Your use of that executable is in no way restricted on account of
* linking the TTaskpanel library code into it.
*
* This exception does not however invalidate any other reasons why
* the executable file might be covered by the GNU General Public License.
*
* This exception applies only to the code released by the TTaskpanel
* Project under the name TTaskpanel.  If you copy code from other
* TTaskpanel Project or Free Software Foundation releases into a copy of
* TTaskpanel, as the General Public License permits, the exception does
* not apply to the code that you add in this way.  To avoid misleading
* anyone as to the status of such modified files, you must delete
* this exception notice from them.
*
* If you write modifications of your own for TTaskpanel, it is your choice
* whether to permit this exception to apply to your modifications.
* If you do not wish that, delete this exception notice.
*
***************************************************************************************************/
//----------------------------------------------------------------------------//
#include "Fivewin.ch"
//----------------------------------------------------------------------------//

EXIT PROCEDURE FreeResources()

   LOCAL aThemes := TThemeManager():aThemes

   WHILE Len( aThemes ) > 0
      aThemes[1]:Destroy()
   END

   RETURN

//----------------------------------------------------------------------------//

CLASS TThemeManager

   DATA nHandle
   DATA cShellStyle              //Ruta del Archivo DLL conteniendo
                                 //informacion del Tema

   DATA lCustom                  INIT .F.   //Tema Especificado por el Usuario


   DATA nCount                   INIT 1    //Contador de Instancias de un Tema Especifico

   CLASSDATA aThemes             INIT {}

   METHOD New( cThemeName )
   METHOD Destroy()

   METHOD End()                  INLINE ::Destroy()
   METHOD LoadFromFile( cFile )
   METHOD Reset()
   METHOD Update()

   METHOD TaskPanel()            INLINE    TMTaskPanel():New( ::nHandle )

ENDCLASS

//----------------------------------------------------------------------------//

METHOD New( cThemeDll ) CLASS TThemeManager

   LOCAL nAt, oObj

   IF cThemeDll == nil

      //Siempre por defecto es el tema actual
      cThemeDll := tm_GetShellStylePath()

   ELSE

      //En este caso seria un tema definido por el usuario
      ::lCustom := .T.

   ENDIF

   IF (nAt := aScan( ::aThemes, {|o| o:cShellStyle == cThemeDll } )) > 0

      Self := ::aThemes[ nAt ]
      ::nCount++;

      RETURN Self
   ENDIF

   //Obtenemos el Handle que contiene toda la informacion del tema
   //trataremos de cargar el archivo DLL en caso de fallo siempre
   //un Handle con la informacion del Tema Clasico de Windows

   ::cShellStyle := cThemeDll
   ::nHandle     := tm_OpenThemeData( cThemeDll )
   
   IF Left( cThemeDll, 1) == "#"
      ::LoadFromFile( SubStr( cThemeDll, 2 ) )
   ENDIF

   aAdd( ::aThemes, Self )

   RETURN Self

//----------------------------------------------------------------------------//

METHOD Destroy() CLASS TThemeManager

   LOCAL nAt

   IF --::nCount < 1

      nAt := aScan( ::aThemes, {|o| o == self } )

      IF nAt > 0
         aDel( ::aThemes, nAt )
         aSize( ::aThemes, Len( ::aThemes ) - 1 )
      ENDIF

      IF !(::nHandle == nil)
         tm_CloseThemeData( ::nHandle )
         ::nHandle := nil
      ENDIF
   ENDIF

   RETURN nil

//----------------------------------------------------------------------------//

METHOD LoadFromFile( cFile )

   IF !( "." $ cFile )
       cFile += ".ini"
   ENDIF
   
   IF !( "\" $ cFile )
       IF File( ".\"+cFile )
	      cFile := ".\" + cFile 
	   ELSEIF File( "theme\" + cFile )
	      cFile := "theme\" + cFile
	   ELSE
	      RETURN Self
       ENDIF
   ELSEIF !File( cFile ) 
       RETURN Self
   ENDIF

   LoadIniSection( ::TaskPanel, cFile, "TaskPanel", ;
         { ;
            "Background",;
            "BorderColor",;
            "BorderThickness",;
            "Padding" ;
          } )

   LoadIniSection( ::TaskPanel:SpecialGroup:Header, cFile, "TASKPANEL-SPECIAL-GROUP-HEADER",;
         { ;
		      "Align",;
            "Background",;
            "BorderColor",;
            "BorderThickness",;
            "BorderStyle",;
            "ButtonCollapse",;
            "ButtonCollapseHi",;
            "ButtonExpand",;
            "ButtonExpandHi",;
            "Font",;
            "FontStyle",;
            "Foreground",;
            "MarginToPanel",;
            "Padding";
         } )

   LoadIniSection( ::TaskPanel:NormalGroup:Header, cFile, "TASKPANEL-NORMAL-GROUP-HEADER",;
         { ;
		      "Align",;
            "Background",;
            "BorderColor",;
            "BorderThickness",;
            "BorderStyle",;
            "ButtonCollapse",;
            "ButtonCollapseHi",;
            "ButtonExpand",;
            "ButtonExpandHi",;
            "Font",;
            "FontStyle",;
            "Foreground",;
            "MarginToPanel",;
            "Padding";
         } )

   LoadIniSection( ::TaskPanel:SpecialGroup:TaskList, cFile, "TASKPANEL-SPECIAL-GROUP-TASKLIST",;
         { ;
            "Background",;
            "BorderColor",;
            "BorderThickness",;
            "MarginToTask",;
            "Padding";
         } )

   LoadIniSection( ::TaskPanel:NormalGroup:TaskList, cFile, "TASKPANEL-NORMAL-GROUP-TASKLIST",;
         { ;
            "Background",;
            "BorderColor",;
            "BorderThickness",;
            "MarginToTask",;
            "Padding";
         } )

   LoadIniSection( ::TaskPanel:SpecialGroup:TaskList:Task, cFile, "TASKPANEL-SPECIAL-GROUP-TASK",;
         { ;
            "Background",;
            "BorderColor",;
            "BorderThickness",;
            "Font",;
            "FontStyle",;
            "Foreground",;
            "ImagePadding",;
            "InfoBackground",;
            "InfoForeground",;
            "Padding";
         } )

   LoadIniSection( ::TaskPanel:NormalGroup:TaskList:Task, cFile, "TASKPANEL-NORMAL-GROUP-TASK",;
         { ;
            "Background",;
            "BorderColor",;
            "BorderThickness",;
            "Font",;
            "FontStyle",;
            "Foreground",;
            "ImagePadding",;
            "InfoBackground",;
            "InfoForeground",;
            "Padding";
         } )

	RETURN Self

//----------------------------------------------------------------------------//

METHOD Reset() CLASS TThemeManager

   IF !(::nHandle == nil)
      tm_CloseThemeData( ::nHandle )
   ENDIF
   
   ::nHandle     := tm_OpenThemeData( ::cShellStyle )
   
   IF Left( ::cShellStyle, 1) == "#"
      ::LoadFromFile( SubStr( ::cShellStyle, 2 ) )
   ENDIF

   RETURN Self
//----------------------------------------------------------------------------//

METHOD Update() CLASS TThemeManager

   LOCAL cShellStyle

   IF !::lCustom

      cShellStyle := tm_GetShellStylePath()

      IF !(cShellStyle == ::cShellStyle)

         //Cerramos la informacion de nuestro tema
         tm_CloseThemeData( ::nHandle )

         //y cargamos la informacion del tema actual
         ::cShellStyle  := cShellStyle
         ::nHandle      := tm_OpenThemeData( cShellStyle )
      ENDIF

   ENDIF

   RETURN nil

//----------------------------------------------------------------------------//

CLASS TMTaskPanel STATIC

   DATA nHandle

   METHOD New( nHandle )            INLINE ::nHandle := nHandle, Self
   METHOD Background                SETGET
   METHOD BorderColor               SETGET
   METHOD BorderThickness           SETGET
   METHOD Padding                   SETGET

   METHOD SpecialGroup()         INLINE    TMTaskGroup():New( ::nHandle, .T. )
   METHOD NormalGroup()          INLINE    TMTaskGroup():New( ::nHandle, .F. )
   METHOD Header( lSpecial )     INLINE    TMTaskHeader():New( ::nHandle, lSpecial )
   METHOD TaskList( lSpecial )   INLINE    TMTaskList():New( ::nHandle, lSpecial )

ENDCLASS

//----------------------------------------------------------------------------//

CLASS TMTaskGroup STATIC

   DATA nHandle
   DATA lSpecial

   METHOD New( nHandle, lSpecial )  INLINE  ::nHandle := nHandle,;
                                            ::lSpecial := lSpecial,;
                                            Self

   METHOD Header()                  INLINE    TMTaskHeader():New( ::nHandle, ::lSpecial )
   METHOD TaskList()                INLINE    TMTaskList():New( ::nHandle, ::lSpecial )

ENDCLASS

//----------------------------------------------------------------------------//

CLASS TMTaskHeader STATIC

   DATA nHandle
   DATA lSpecial

   METHOD New( nHandle, lSpecial )  INLINE  ::nHandle := nHandle,;
                                            ::lSpecial := lSpecial,;
                                            Self

   METHOD Align                     SETGET
   METHOD Background                SETGET
   METHOD BackgroundHot             SETGET
   METHOD BorderColor               SETGET
   METHOD BorderThickness           SETGET
   METHOD BorderStyle               SETGET
   METHOD ButtonCollapse            SETGET
   METHOD ButtonCOllapseHi          SETGET
   METHOD ButtonExpand              SETGET
   METHOD ButtonExpandHi            SETGET
   METHOD Font                      SETGET
   METHOD FontStyle                 SETGET
   METHOD Foreground                SETGET
   METHOD ForegroundHot             SETGET
   METHOD IsBackgroundBitmap
   METHOD IsBackgroundHotBitmap
   METHOD MarginToPanel             SETGET
   METHOD Padding                   SETGET

ENDCLASS

//----------------------------------------------------------------------------//

CLASS TMTaskList STATIC

   DATA nHandle
   DATA lSpecial

   METHOD New( nHandle, lSpecial )  INLINE  ::nHandle := nHandle,;
                                            ::lSpecial := lSpecial,;
                                            Self

   METHOD Background                SETGET
   METHOD BorderColor               SETGET
   METHOD BorderThickness           SETGET
   METHOD MarginToTask              SETGET
   METHOD Padding                   SETGET

   METHOD Task                      INLINE  TMTaskItem():New( ::nHandle, ::lSpecial )

ENDCLASS

//----------------------------------------------------------------------------//

CLASS TMTaskItem STATIC

   DATA nHandle
   DATA lSpecial

   METHOD New( nHandle, lSpecial )  INLINE  ::nHandle := nHandle,;
                                            ::lSpecial := lSpecial,;
                                            Self

   METHOD Background                SETGET
   METHOD BackgroundHot             SETGET
   METHOD BorderColor               SETGET
   METHOD BorderThickness           SETGET
   METHOD Font                      SETGET
   METHOD FontStyle                 SETGET
   METHOD Foreground                SETGET
   METHOD ForegroundHot             SETGET
   METHOD ImagePadding              SETGET
   METHOD InfoBackground            SETGET
   METHOD InfoForeground            SETGET
   METHOD Padding                   SETGET

ENDCLASS

//----------------------------------------------------------------------------//

STATIC FUNCTION LoadIniSection( oPart, cIniFile, cSection, aEntries )
   LOCAL cEntry, cValue

   FOR EACH cEntry IN aEntries

      cValue := GetPvProfString( cSection, cEntry,"NULL", cIniFile )
      cEntry := UPPER( cEntry )

      IF !(cValue == "NULL")
         DO CASE
            CASE cEntry == "ALIGN"              ; oPart:Align           := Convert2Align( cValue )
            CASE cEntry == "BACKGROUND"         ; oPart:Background      := Convert2Background( cValue )
            CASE cEntry == "BORDERCOLOR"        ; oPart:BorderColor     := Convert2Color( cValue ) // Check
            CASE cEntry == "BORDERSTYLE"        ; oPart:BorderStyle     := Val( cValue )
            CASE cEntry == "BORDERTHICKNESS"    ; oPart:BorderThickness := Convert2Rect( cValue )
            CASE cEntry == "BUTTONCOLLAPSE"     ; oPart:ButtonCollapse  := Convert2Button( cValue, cIniFile  )
            CASE cEntry == "BUTTONCOLLAPSEHI"   ; oPart:ButtonCollapseHi:= Convert2Button( cValue, cIniFile  )
            CASE cEntry == "BUTTONEXPAND"       ; oPart:ButtonExpand    := Convert2Button( cValue, cIniFile  )
            CASE cEntry == "BUTTONEXPANDHI"     ; oPart:ButtonExpandHi  := Convert2Button( cValue, cIniFile  )
            CASE cEntry == "FONT"               ; oPart:Font            := Convert2Font( cValue )
            CASE cEntry == "FONTSTYLE"          ; oPart:FontStyle       := Convert2FontStyle( cValue )
            CASE cEntry == "FOREGROUND"         ; oPart:Foreground      := Convert2Foreground( cValue ) // Check
            CASE cEntry == "IMAGEPADDING"       ; oPart:ImagePadding    := Convert2Rect( cValue )
            CASE cEntry == "INFOBACKGROUND"     ; oPart:InfoBackground  := Convert2Color( cValue )
            CASE cEntry == "INFOFOREGROUND"     ; oPart:InfoForeground  := Convert2Color( cValue ) // Check
            CASE cEntry == "MARGINTOPANE"       ; oPart:MargintoPane    := Val( cValue )
            CASE cEntry == "MARGINTOTASK"       ; oPart:MargintoTask    := Val( cValue )
            CASE cEntry == "PADDING"            ; oPart:Padding         := Convert2Rect( cValue )
         END
      ENDIF

   NEXT

   RETURN NIL


//----------------------------------------------------------------------------//

STATIC FUNCTION Convert2Array( cStr, nSize )

   LOCAL aArray := {}
   LOCAL nLen   := Len( cStr )
   LOCAL cItem  := ""
   LOCAL cChar
   LOCAL n      := 1
   LOCAL nOpen  := 0

   WHILE n <= nLen
      cChar = SubStr( cStr, n , 1 )

      IF cChar == "," .And. nOpen == 0
         aAdd( aArray, cItem )
         cItem := ""
      ELSE

         IF cChar == "("
            nOpen++
         ELSEIF cChar == ")"
            nOpen--
         ENDIF

         cItem += cChar
      ENDIF
      n++
   END

   aAdd( aArray, cItem )

   IF !(nSize == nil)
      aSize( aArray, nSize )

      FOR n := 1 TO LEN( aArray )

         IF aArray[ n ] == NIL
            aArray[ n ] := ""
         ENDIF

      NEXT
   ENDIF

   RETURN aArray


//----------------------------------------------------------------------------//

STATIC FUNCTION Convert2Rect( cStr )

   LOCAL nTmp, n
   LOCAL aRect := {0,0,0,0}

   cStr := Upper( Alltrim(cStr) )

   IF Left( cStr, 4 ) == "RECT"
      cStr := LTrim( SubStr( cStr, 5 ) )

      IF Left( cStr , 1 ) == "(" .And. Right( cStr, 1 ) == ")"

         aRect := Convert2Array( SubStr( cStr, 2, Len( cStr) - 2 ), 4 )

         FOR n := 1 TO 4
             aRect[ n  ] := Val( aRect[ n ] )
         NEXT

      ENDIF
   ELSE
      nTmp  := Val( cStr )
      aRect := { nTmp, nTmp, nTmp, nTmp }
   ENDIF

   RETURN aRect

//----------------------------------------------------------------------------//

STATIC FUNCTION Convert2Background( cStr )

   LOCAL aColor := 0

   cStr := Upper( Alltrim(cStr) )

   IF Left( cStr, 8 ) == "GRADIENT"

      cStr := LTrim( SubStr( cStr, 9 ) )

      IF Left( cStr , 1 ) == "(" .And. Right( cStr, 1 ) == ")"

         aColor := Convert2Array( SubStr( cStr, 2, Len( cStr) - 2 ), 3 )

         aColor[ 1  ] := Convert2Color( aColor[ 1 ] )
         aColor[ 2  ] := Convert2Color( aColor[ 2 ] )
         aColor[ 3  ] := Val( aColor [3] )

      ENDIF

   ELSE
      aColor := Convert2Color( cStr )
   ENDIF

   RETURN aColor

//----------------------------------------------------------------------------//

STATIC FUNCTION Convert2Foreground( cStr )

   LOCAL aColor := Convert2Array( Upper(cStr) )

   IF Len( aColor ) == 2
      aColor[ 1 ] := Convert2Color( aColor[1] )
      aColor[ 2 ] := Convert2Color( aColor[2] )
   ELSE
      aColor := Convert2Color( aColor[1] )
   ENDIF

   RETURN aColor


//----------------------------------------------------------------------------//

STATIC FUNCTION Convert2Color( cStr )

   LOCAL aColor := 0

   cStr := Upper( Alltrim(cStr) )

   IF Left( cStr, 3 ) == "RGB"

      cStr := LTrim( SubStr( cStr, 4 ) )

      IF Left( cStr , 1 ) == "(" .And. Right( cStr, 1 ) == ")"

         aColor := Convert2Array( SubStr( cStr, 2, Len( cStr) - 2 ), 3 )
         aColor := RGB( Val(aColor[1]), Val(aColor[2]), Val(aColor[3]) )

      ENDIF

   ELSE
      aColor := Val( cStr )
   ENDIF

   RETURN aColor

//----------------------------------------------------------------------------//

STATIC FUNCTION Convert2Button( cStr, cFile )
    LOCAL cPath
    hb_FNameSplit( cFile, @cPath )
    RETURN cPath + Alltrim(cStr)


//----------------------------------------------------------------------------//

STATIC FUNCTION Convert2Font( cStr )
   LOCAL n := Convert2Array( cStr, 2 )
   n[1] := Alltrim( n[1] )
   n[2] := Val( n[2] )
   RETURN n

//----------------------------------------------------------------------------//

STATIC FUNCTION Convert2FontStyle( cStr )
   LOCAL n := UPPER( cStr )

   n := StrTran( n , "BOLD", "B" )
   n := StrTran( n , "ITALIC", "I" )
   n := StrTran( n , "UNDERLINE", "U" )
   n := StrTran( n , "STRIKEOUT", "S" )
   n := StrTran( n , " ", "" )

   n := Convert2Array( n, 2 )
   n[1] := n[1]
   n[2] := n[2]

   RETURN n
   
//----------------------------------------------------------------------------//

STATIC FUNCTION Convert2Align( cStr )   

   LOCAL nAlign := 0
   
   cStr := Alltrim( Upper( cStr) )
   
   IF IsAlpha( cStr )
      IF cStr == "CENTER"
	     nAlign := 1
	  ELSEIF cStr == "RIGHT"
	     nAlign := 2
	  ENDIF	  
   ELSE
      nAlign := Val( cStr )
   ENDIF
   
   RETURN nAlign
   
//----------------------------------------------------------------------------//
// EOF:
//------