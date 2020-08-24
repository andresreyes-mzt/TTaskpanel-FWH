/**************************************************************************************************
* Proyecto     : TTaskpanel
* Archivo      : h_CTaskPanel.prg
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

#include "fivewin.ch"

#define WM_MOUSELEAVE            675
#define WM_MOUSEWHEEL            522 //0x020A
#define WM_THEMECHANGED          794 //0x031A
#define WM_NEXTDLGCTL                   0x0028
#define TME_LEAVE             2

#define GWL_STYLE         - 16

#define SM_CXVSCROLL          2

#define RC_LEFT         1
#define RC_TOP          2
#define RC_RIGHT        3
#define RC_BOTTOM       4

//----------------------------------------------------------------------------//

CLASS CTaskPanel FROM TControl

   DATA nAnchor                  INIT 0
   DATA nFocused                 INIT 0
   DATA nHeaderAlign             INIT 0  // LEFT
   DATA nVirtualTop              INIT 0
   DATA nVirtualHeight           INIT 0
   DATA nRealHeight              INIT 0
   DATA nPlainStyle              INIT 0
   DATA nLayoutCount             INIT 0
   DATA lNeedLayout              INIT .T.
   DATA lLayout                  INIT .F.
   DATA lScrollVisible           INIT .F.
   DATA lShowFocusRect           INIT .T.

   DATA lAnimate                 INIT .F.
   DATA lCollapsable             INIT .T.
   DATA lCanAnimate              INIT .T.
   DATA lRedefine                INIT .F.


   DATA foTheme                  //Este es el responsable de Obtener la Informacion
                                 //sobre los temas del shellstyle.dll, y contiene
                                 //toda la informacion necesaria sobre colore, fuentes
                                 //espaciado, etc.

   DATA oMouseOver
   DATA oItemFocused

   DATA hBitmap
   DATA nBitmapHeight            INIT 0
   DATA lHasAlpha

   DATA aClientRect



   CLASSDATA lRegistered   AS LOGICAL

   METHOD New()
   METHOD AddGroup()

   METHOD AdjLeft()
   METHOD AdjRight()
   METHOD AnchorLeft()
   METHOD AnchorRight()
   METHOD CoorsUpdate()
   METHOD CollapseAll()       INLINE aEval( ::aControls, {|o|o:Collapse } )


   METHOD Destroy()

   METHOD Display()           INLINE ::BeginPaint(), ::Paint(), ::EndPaint(), 0

   METHOD DoLayout()

   METHOD EraseBkGnd( hDC )   INLINE 1

   METHOD EndLayout()         INLINE if(::nLayoutCount > 0, ::nLayoutCount--,), ::DoLayout(.T.)

   METHOD ExpandAll()         INLINE aEval( ::aControls, {|o|o:Expand() } )

   METHOD GetDlgCode( nLastKey )
   METHOD GetDialogParent()
   METHOD GetNextExpando( lPrev )

   METHOD GoCurrentExpando()

   METHOD GoNextExpando()
   METHOD GetParentOf( hWnd )
   METHOD GoPrevExpando()
   METHOD GotFocus( hCtlLost )
   
   METHOD Group( n )                         INLINE ::aControls[ n ]
   
   METHOD HandleEvent( nMsg, nWParam, nLParam )

   METHOD Item( n )                         INLINE ::aControls[ n ]

   METHOD Initiate( hDlg )
   METHOD KeyDown( nKey, nFlags )
   METHOD KeyChar( nKey, nFlags )
   METHOD lAnimating()
   METHOD LButtonUp()
   METHOD LostFocus( hWndGetFocus )
   METHOD MouseLeave()
   METHOD MouseMove( nRow, nCol, bKeyFlags )
   METHOD MouseWheel( nKey, nDelta, nXPos, nYPos )
   METHOD oTheme( oTheme )    SETGET
   METHOD Paint()
   METHOD Redefine( nId )
   METHOD ReSize( nSizeType, nWidth, nHeight )
   METHOD SetHeader( cIcon )
   METHOD SetCtlFocus( hCtlFocus )

   METHOD SetTheme( cTheme );
                              INLINE ::oTheme := TThemeManager():New( cTheme )

   METHOD SBSetPos( nPos )
   METHOD SBSkip( nSkip )

   METHOD SuspendLayout()     INLINE ::nLayoutCount++

   METHOD ThemeChanged( lUpdate )
   METHOD Update()

END CLASS

//----------------------------------------------------------------------------//

METHOD New( cTheme, nTop, nLeft, nWidth, nHeight, nAnchor, oWnd, cBitmap, nHeaderAlign, lAnimate, lCollapsable, nPlainStyle ) CLASS CTaskpanel

   DEFAULT oWnd         := GetWndDefault(),;
           lAnimate     := .F.,;
           lCollapsable := .T.,;
           nPlainStyle  := 0,;
           nTop         := 0,;
           nLeft        := 0,;
           nWidth       := 240,;
           nHeight      := 400,;
           nAnchor      := 0,;
           nHeaderAlign := 0


   ::nId          := ::GetNewId()
   ::nTop         := nTop
   ::nLeft        := nLeft
   ::nBottom      := (nTop + nHeight)
   ::nRight       := (nLeft + nWidth)
   ::oWnd         := oWnd
   ::lAnimate     := lAnimate
   ::lCollapsable := lCollapsable
   ::nPlainStyle  := nPlainStyle
   ::nAnchor      := nAnchor
   ::nHeaderAlign := nHeaderAlign
   ::aControls    := {}

   ::foTheme      := TThemeManager():New( cTheme )

   ::nStyle       := nOr( WS_CHILD, WS_VISIBLE, WS_CLIPSIBLINGS, WS_CLIPCHILDREN, WS_TABSTOP )
   ::lDrag        := .f.

   ::Register()

   IF Empty( ::oWnd:hWnd )
      //Error
      RETURN Nil
   ENDIF

   ::Create()
   ::oWnd:AddControl( Self )

   IF !(cBitmap == nil)
      ::SetHeader( cBitmap )
   ENDIF

   //////////////////////////
   // Creamos la Barra de Desplazamiento a Utilizar
   //////////

   DEFINE SCROLLBAR ::oVScroll VERTICAL OF Self

   ::oVScroll:bGoUp   := {|| ::SBSkip( - 10 ) }
   ::oVScroll:bGoDown := {|| ::SBSkip( 10 ) }
   ::oVScroll:bPageUp   := {|| ::SBSkip( - ::oVScroll:nPgStep ) }
   ::oVScroll:bPageDown := {|| ::SBSkip( ::oVScroll:nPgStep ) }

   ::oVScroll:bPos    := {|nPos| ::SBSetPos( nPos ) }
   ::oVScroll:bTrack  := {|nPos| ::SBSetPos( nPos ) }

   IF nAnchor == 1
      ::oWnd:oLeft   := Self
      ::oWnd:Resize()
   ELSEIF nAnchor == 2
      ::oWnd:oRight   := Self
      ::oWnd:Resize()
   ENDIF

   RETURN Self

//----------------------------------------------------------------------------//

METHOD Initiate( hDlg  ) CLASS CTaskpanel

   Super:Initiate( hDlg )

   ::nStyle       := nOr( WS_CHILD, WS_VISIBLE, WS_CLIPSIBLINGS, WS_CLIPCHILDREN, WS_TABSTOP )
   SetWindowLong( ::hWnd, GWL_STYLE, ::nStyle )

   RETURN Self

//----------------------------------------------------------------------------//

METHOD Redefine( nId, cTheme, oWnd, cBitmap, nHeaderAlign, lAnimate, lCollapsable, nPlainStyle ) CLASS CTaskpanel

   DEFAULT oWnd         := GetWndDefault(),;
           nHeaderAlign := 0,;
           lAnimate     := .F.,;
           lCollapsable := .T.,;
           nPlainStyle  := 0

   ::nId          := nId
   ::oWnd         := oWnd
   ::nHeaderAlign := nHeaderAlign
   ::lAnimate     := lAnimate
   ::lCollapsable := lCollapsable
   ::nPlainStyle  := nPlainStyle
   ::foTheme      := TThemeManager():New( cTheme )
   ::lDrag        := .f.
   ::lRedefine    := .T.

   ::Register()

   ::oWnd:DefControl( Self )

   IF !(cBitmap == nil)
      ::SetHeader( cBitmap )
   ENDIF

   //////////////////////////
   // Creamos la Barra de Desplazamiento a Utilizar
   //////////

   DEFINE SCROLLBAR ::oVScroll VERTICAL OF Self

   ::oVScroll:bGoUp   := {|| ::SBSkip( - 10 ) }
   ::oVScroll:bGoDown := {|| ::SBSkip( 10 ) }
   ::oVScroll:bPageUp   := {|| ::SBSkip( - ::oVScroll:nPgStep ) }
   ::oVScroll:bPageDown := {|| ::SBSkip( ::oVScroll:nPgStep ) }

   ::oVScroll:bPos    := {|nPos| ::SBSetPos( nPos ) }
   ::oVScroll:bTrack  := {|nPos| ::SBSetPos( nPos ) }

   RETURN Self

//----------------------------------------------------------------------------//

METHOD AddGroup( cPrompt, cBitmap, lSpecial, cWatermark, nHeight, lHeader, lCollapsed, lCollapsable, bOnCollapse, bOnExpand ) CLASS CTaskpanel
   RETURN TTaskPanelGroup():New( cPrompt, cBitmap, lSpecial, Self, cWatermark, nHeight, lHeader, lCollapsed, lCollapsable, bOnCollapse, bOnExpand )

//----------------------------------------------------------------------------//

METHOD AdjLeft() CLASS CTaskpanel

   LOCAL hTop, hBottom

   IF ::oWnd:oTop != nil
      hTop = ::oWnd:oTop:hWnd
   ENDIF

   IF ::oWnd:oBar != nil
      hTop = ::oWnd:oBar:hWnd
   ENDIF

   IF ::oWnd:oBottom != nil
      hBottom = ::oWnd:oBottom:hWnd
   ENDIF

   IF ::oWnd:oMsgBar != nil
      hBottom = ::oWnd:oMsgBar:hWnd
   ENDIF

   //////////////////////////
   // Fivewin Bug ??
   //////////
   // si hTop == nil  ::nTop es siempre -2

   WndAdjLeft2( ::hWnd, hTop, hBottom )

   IF ::lDrag .and. GetFocus() == ::hWnd
      ::ShowDots()
   ENDIF

   RETURN nil

//----------------------------------------------------------------------------//

METHOD AdjRight() CLASS CTaskpanel

   LOCAL hTop, hBottom

   IF ::oWnd:oTop != nil
      hTop = ::oWnd:oTop:hWnd
   ENDIF

   IF ::oWnd:oBar != nil
      hTop = ::oWnd:oBar:hWnd
   ENDIF

   IF ::oWnd:oBottom != nil
      hBottom = ::oWnd:oBottom:hWnd
   ENDIF

   IF ::oWnd:oMsgBar != nil
      hBottom = ::oWnd:oMsgBar:hWnd
   ENDIF

   WndAdjRight2( ::hWnd, hTop, hBottom )

   IF ::lDrag .and. GetFocus() == ::hWnd
      ::ShowDots()
   ENDIF

   RETURN nil

//----------------------------------------------------------------------------//

METHOD AnchorLeft() CLASS CTaskpanel

   IF ::nAnchor != 1
      ::nAnchor := 1

      IF ::oWnd:oRight == Self
         ::oWnd:oRight := nil
      ENDIF

      ::oWnd:oLeft := self
      ::oWnd:Resize()
      ::Refresh()
   ENDIF

   RETURN nil

//----------------------------------------------------------------------------//

METHOD AnchorRight() CLASS CTaskpanel

   IF ::nAnchor != 2
      ::nAnchor := 2

      IF ::oWnd:oLeft == Self
         ::oWnd:oLeft := nil
      ENDIF

      ::oWnd:oRight := self
      ::oWnd:Resize()
      ::Refresh()
   ENDIF

   RETURN nil

//----------------------------------------------------------------------------//

METHOD CoorsUpdate() CLASS CTaskpanel

   local aRect := GetCoors( ::hWnd )

   ::nTop    = aRect[ 1 ]
   ::nLeft   = aRect[ 2 ]
   ::nBottom = aRect[ 3 ]
   ::nRight  = aRect[ 4 ]

return nil

//----------------------------------------------------------------------------//

METHOD Destroy() CLASS CTaskpanel

   IF !(::hBitmap == nil)
      DeleteObject( ::hBitmap )
   ENDIF

   IF !(::hBitmap == nil)
      ::foTheme:End()
      ::foTheme := nil
   ENDIF

   IF ::hWnd != nil
      Super:Destroy()
   ENDIF

   RETURN nil

//----------------------------------------------------------------------------//

METHOD DoLayout( lUpdate ) CLASS CTaskpanel

   LOCAL oObj
   LOCAL nGroupRow, nGroupCol, nGroupWidth
   LOCAL nMargin := 0
   LOCAL aPadding
   LOCAL nHeight, oBrush

   //////////////////////////
   // Si ya estamos procesando la disposicion del TaskPanel
   // no necesitamos hacerla de nuevo
   //////////

   IF !(::nLayoutCount == 0) .Or. ::lLayout
      RETURN Nil
   ENDIF

   //////////////////////////
   // Inicializamos Variables
   //////////

   ::lNeedLayout := .F.
   ::lLayout     := .T.

   aPadding    := ::foTheme:TaskPanel:Padding
   nGroupWidth := ::nWidth  - aPadding[ RC_LEFT] - ;  // El Tamaño del Panel menos
                              aPadding[ RC_RIGHT ] - ;  // los margenes derecho e izquierdo
                              if( ::nPlainStyle == 1, 8, 0 )

   IF ::nRealHeight == 0
      ::nRealHeight := ::nHeight
   ENDIF

   //Si el Alto Virtual es mayor del real
   //tenemos que ajustar el ancho de los Grupos

   IF ::nRealHeight > ::nHeight

      //////////////////////////
      // Disminuimos el ancho de la barra de desplazamiento
      // al ancho de nuestro grupos
      //////////

      nGroupWidth -=  GetSysMetrics( SM_CXVSCROLL )

      //////////////////////////
      // Aqui mostramos (en caso de que este oculta) y fijamos
      // los valores para nuestra barra de dezplamiento
      //////////

      IF ::oWnd:IsKindOf( "TDIALOG" )
         oBrush := ::oWnd:oBrush
         ::oWnd:oBrush := TBrush():New( "NULL" )
      ENDIF

      //----------------------------------------------------------
	  // Modificado por incompatibilidad a versiones recientes FWH
	  //----------------------------------------------------------
      //::oVScroll:SetRange( 0, ::nVirtualHeight - 1) // original
      SetScrollRangeX( ::hWnd, 1, 0, ::nVirtualHeight - 1)

      IF ::oWnd:IsKindOf( "TDIALOG" )
         ::oWnd:oBrush:End()
         ::oWnd:oBrush := oBrush
      ENDIF

      IF  (::nVirtualHeight - ::nVirtualTop) < ::nHeight
         ::nVirtualTop := ::nVirtualHeight - ::nHeight
      ENDIF

      //----------------------------------------------------------
	  // Modificado para trabajar con versiones anteriores de FWH
	  //----------------------------------------------------------
      //::oVScroll:SetPage( ::nHeight, .F. ) // Original
	  SetScrollInfo( ::hWnd, 1, ::nHeight, .F. )

      ::oVScroll:SetPos( ::nVirtualTop )

      ::lScrollVisible := .T.
   ELSE

      //////////////////////////
      // Como no necesitamos barra de desplazamiento
      // la ocultamos (si esta visible)
      //////////

      ::nVirtualTop := 0
      ::nVirtualHeight := ::nHeight

      IF ::oWnd:IsKindOf( "TDIALOG" )
         oBrush := ::oWnd:oBrush
         ::oWnd:oBrush := TBrush():New( "NULL" )
      ENDIF

      //::oVScroll:SetRange( 0, 0 )
      SetScrollRangeX( ::hWnd, 1, 0, 0 )

      IF ::oWnd:IsKindOf( "TDIALOG" )
         ::oWnd:oBrush:End()
         ::oWnd:oBrush := oBrush
      ENDIF

      ::lScrollVisible := .F.
   ENDIF

   //////////////////////////
   // Asignamos algunas variables que necesitamos
   // para posicionar nuestro grupos de tareas
   //////////

   nGroupRow   := aPadding[ RC_TOP ] - ::nVirtualTop + ::nBitmapHeight +;
               if( ::nPlainStyle == 1, 4, 0 )
   nGroupCol   := aPadding[ RC_LEFT ] + if( ::nPlainStyle == 1, 4, 0 )

   //////////////////////////
   // Posicionamos los grupos en el renglon y columna
   // que les corresponde, tomando en cuenta si estan colapsados,
   // Tambien fijamos el ancho del grupo de tareas
   //////////

   FOR EACH oObj IN ::aControls

      IF !oObj:lHidden

         nGroupRow += if( oObj:lHeaderVisible, Max( oObj:nHeaderHeight( nGroupWidth ), oObj:nBmpHeight ), 0 )

         oObj:Move( nGroupRow, nGroupCol, nGroupWidth,,.F. )

         IF lUpdate == .T.
            oObj:DoLayout( .T. )
         ENDIF

         nGroupRow   += If( oObj:lVisible, oObj:nHeight, 0 ) +;
                        If( oObj:lAnimating, oObj:nVirtualHeight, 0 )

         IF !(!oObj:lHeaderVisible .And. oObj:lCollapsed) .OR. oObj:lAnimating
            nGroupRow   += if( ::nPlainStyle == 1, ( nMargin := 2) ,(nMargin := oObj:nMarginToPanel))
         ENDIF

      ENDIF

   NEXT

   //////////////////////////
   // Calculamos la altura real que necesita para ser
   // visto en su totalidad nuestro panel de tareas
   //////////

   ::nRealHeight := ;
   nHeight := nGroupRow -              ; // Renglon del siguiente grupo
              nMargin +                ; // Margen entre grupos
              aPadding[ RC_BOTTOM ] +  ; // Margen inferior
              ::nVirtualTop +          ; // Primer Renglon Visible
              if( ::nPlainStyle == 1, 4, 0 )


   IF nHeight > ::nVirtualHeight
      ::nVirtualHeight := nHeight
      IF !::lScrollVisible
         ::lLayout := .F.
         ::DoLayout( lUpdate )
         RETURN Nil
      ENDIF
   ELSEIF nHeight < ::nVirtualHeight
      IF nHeight <= ::nHeight
         ::nVirtualHeight := ::nHeight
         IF ::lScrollVisible
            ::lLayout := .F.
            ::DoLayout( lUpdate )
            RETURN Nil
         ENDIF
      ELSE
         ::nVirtualHeight  := nHeight
         ::lLayout := .F.
         ::DoLayout( lUpdate )
         RETURN Nil
      ENDIF
   ENDIF

   ::aClientRect := { ;
                        aPadding[ RC_LEFT ],;
                        aPadding[ RC_TOP ] + ::nBitmapHeight,;
                        aPadding[ RC_LEFT ] + nGroupWidth + if( ::nPlainStyle == 1, 8, 0 ),;
                        nHeight - nMargin + if( ::nPlainStyle == 1, 8, 0 ) }


   //////////////////////////
   // Hacemos un refresco del Panel de Tareas Completo
   ::Update()

   ::lLayout := .F.

   RETURN Nil

//----------------------------------------------------------------------------//

METHOD GetDlgCode( nLastKey ) CLASS CTaskpanel

   IF .not. ::oWnd:lValidating
      IF nLastKey == VK_UP .or. nLastKey == VK_DOWN ;
         .or. nLastKey == VK_RETURN .or. nLastKey == VK_TAB
         ::oWnd:nLastKey = nLastKey
      ELSE
         ::oWnd:nLastKey = 0
      ENDIF
   ENDIF

   RETURN If( IsWindowEnabled( ::hWnd ), DLGC_WANTALLKEYS, 0 )

//----------------------------------------------------------------------------//

METHOD GetDialogParent()

   LOCAL oParent := Self:oWnd

   WHILE !(oParent == nil)
      IF oParent:IsKindOf( "TDIALOG" )
         RETURN oParent
      ENDIF
      oParent := oParent:oWnd
   END

   RETURN nil

//----------------------------------------------------------------------------//

METHOD GetNextExpando( lPrev )

   LOCAL nIndex, oObj

   IF ::oItemFocused == nil
      ::oItemFocused := ::GetParentOf( GetFocus() )
   ENDIF

   nIndex := aScan( ::aControls, {|o| o == ::oItemFocused } )
   nIndex := If( lPrev == .T., --nIndex, ++nIndex )

   IF nIndex <= 0
      oObj := ::aControls[1]
   ELSEIF nIndex > Len( ::aControls )
      oObj := ::aControls[ Len( ::aControls ) ]
   ELSE
      oObj := ::aControls[ nIndex ]
   ENDIF

   RETURN oObj

//----------------------------------------------------------------------------//

METHOD GetParentOf( hWnd ) CLASS CTaskpanel

   LOCAL oControl := oWndFromhWnd( hWnd )

   WHILE !(oControl == nil)
      IF oControl:IsKindOf( "TTASKPANELGROUP" )
         RETURN oControl
      ENDIF
      oControl := oControl:oWnd
   END

   RETURN Nil

//----------------------------------------------------------------------------//

METHOD GoCurrentExpando() CLASS CTaskpanel

   IF ::oItemFocused == nil
      ::oItemFocused := ::GetParentOf( GetFocus() )
   ENDIF

   IF !(::oItemFocused == nil )
      ::oItemFocused:SetFocus()
   ENDIF

   RETURN nil

//----------------------------------------------------------------------------//

METHOD GoNextExpando() CLASS CTaskpanel
   LOCAL oObj

   IF ::oItemFocused != nil .And. IsWindowVisible( ::oItemFocused:hWnd ) .And. ;
       ::oItemFocused:lHasActiveCtrl .And. ::lFocused

      ::SetCtlFocus( ::oItemFocused:hWnd )
      ::Refresh()

   ELSE

      oObj := ::GetNextExpando()

      IF !( oObj == ::oItemFocused )
         oObj:SetFocus()
      ELSE
         ::oWnd:GoNextCtrl( ::hWnd )
      ENDIF
   ENDIF

   RETURN nil

//----------------------------------------------------------------------------//

METHOD GoPrevExpando() CLASS CTaskpanel

   LOCAL oObj := ::GetNextExpando( .T. )

   IF !( oObj == ::oItemFocused )
      IF IsWindowVisible( oObj:hWnd ) .And. oObj:lHasActiveCtrl
          oObj:SetFocus()
          ::SetCtlFocus( ::oItemFocused:LastActiveCtrl():hWnd )
      ELSE
         oObj:SetFocus()
      ENDIF
   ELSE
      ::oWnd:GoPrevCtrl( ::hWnd )
   ENDIF

   RETURN nil

//----------------------------------------------------------------------------//

METHOD GotFocus( hCtlLost ) CLASS CTaskpanel

   Super:GotFocus( hCtlLost )

   IF ::oItemFocused == nil .And. Len( ::aControls ) > 0
      ::aControls[ 1 ]:SetFocus()
   ENDIF

   ::Refresh()

   RETURN Nil

//----------------------------------------------------------------------------//

METHOD HandleEvent( nMsg, nWParam, nLParam ) CLASS CTaskpanel

   DO CASE
      CASE nMsg == WM_MOUSELEAVE     ; return ::MouseLeave( nHiWord( nLParam ), nLoWord( nLParam ), nWParam )
      CASE nMsg == WM_THEMECHANGED   ; return ::ThemeChanged()
   END

   RETURN Super:HandleEvent( nMsg, nWParam, nLParam )

//----------------------------------------------------------------------------//

METHOD KeyChar( nKey, nFlags ) CLASS CTaskpanel


   if nKey == VK_ESCAPE
      ::oWnd:KeyChar( nKey, nFlags )
      return 0
   endif

   do case
      case nKey == VK_TAB .and. GetKeyState( VK_SHIFT )
           return 0    // We don't want API default behavior
      case nKey == VK_TAB
           return 0    // We don't want API default behavior

   endcase


return Super:KeyChar( nKey, nFlags )

//----------------------------------------------------------------------------//

METHOD KeyDown( nKey, nFlags ) CLASS CTaskpanel

   IF (nKey == VK_TAB .and. GetKeyState( VK_SHIFT )) .OR.;
       nKey == VK_UP
      ::lShowFocusRect := .T.
      ::GoPrevExpando()
      RETURN 0
   ENDIF

   IF nKey == VK_TAB .OR. nKey == VK_DOWN
      ::lShowFocusRect := .T.
      ::GoNextExpando()
      RETURN 0
   ENDIF

   IF nKey == VK_SPACE .OR. nKey == VK_RETURN
      ::lShowFocusRect := .T.
      IF !(::oItemFocused:bAction == nil)
         Eval( ::oItemFocused:bAction, ::oItemFocused )
      ELSEIF ::oItemFocused:lCollapsable
         ::oItemFocused:Toggle()
      ENDIF
      RETURN 0

   ENDIF

   RETURN Super:KeyDown( nKey, nFlags )

//----------------------------------------------------------------------------//

METHOD lAnimating() CLASS CTaskpanel
   RETURN aScan( ::aControls, {|o| o:lAnimating } ) > 0

//----------------------------------------------------------------------------//

METHOD LButtonUp( nRow, nCol, nKeyFlags ) CLASS CTaskpanel

   LOCAL oMouseOver := ::oMouseOver

   IF !(oMouseOver == nil)

      IF oMouseOver:lCollapsable .And. oMouseOver:bAction == nil
         oMouseOver:Toggle()
      ENDIF

      ::oItemFocused   := oMouseOver
      ::lShowFocusRect := .F.

      IF !(::hWnd == GetFocus())
         ::SetCtlFocus( ::hWnd )
      ENDIF

      IF ::lShowFocusRect
         ::Refresh()
      ENDIF

      IF !(oMouseOver:bAction == nil)
         Eval( oMouseOver:bAction, oMouseOver )
      ENDIF

   ENDIF

   RETURN Super:LButtonUp( nRow, nCol, nKeyFlags )

//----------------------------------------------------------------------------//

METHOD LostFocus( hWndGetFocus ) CLASS CTaskpanel

   IF  !IsChild( ::hWnd, hWndGetFocus )
      ::oItemFocused := Nil
   ENDIF

   Super:LostFocus( hWndGetFocus )
   ::Refresh()

   RETURN nil

//----------------------------------------------------------------------------//

METHOD MouseLeave() CLASS CTaskpanel

   LOCAL oObj

   IF !(::oMouseOver == nil)

      oObj := ::oMouseOver
      ::oMouseOver := nil

      ::Refresh()

   ENDIF

   RETURN Nil

//----------------------------------------------------------------------------//

METHOD MouseMove( nRow, nCol, bKeyFlags ) CLASS CTaskpanel

   LOCAL oObj

   TrackMouseEvent( ::hWnd, TME_LEAVE )

   FOR EACH oObj IN ::aControls
      IF oObj:IsOverHeader( nRow, nCol )

         IF !(::oMouseOver == oObj)
            ::oMouseOver := oObj
            ::Refresh()
         ENDIF

         IF ::oMouseOver:lCollapsable
            CursorHand()
         ELSE
            CursorArrow()
         ENDIF

         RETURN 0
      ENDIF
   NEXT

   IF !(::oMouseOver == nil)
      ::oMouseOver   := nil
      ::Refresh()
   ENDIF

   RETURN Super:MouseMove( nRow, nCol, bKeyFlags )


//----------------------------------------------------------------------------//

METHOD MouseWheel( nKey, nDelta, nXPos, nYPos ) CLASS CTaskpanel

   IF !::lLayout
      nDelta := nDelta / 5
      ::SBSkip( if( nDelta > 0, -nDelta, Abs( nDelta) ) )
   ENDIF

   RETURN Nil

//----------------------------------------------------------------------------//

METHOD oTheme( oTheme ) CLASS CTaskpanel

   IF !(oTheme == nil)
      IF oTheme:IsKindOf( "TTHEMEMANAGER" )
         ::foTheme:End()
         ::foTheme := oTheme
         ::ThemeChanged( .F. )
      ENDIF
   ENDIF

   RETURN ::foTheme

//----------------------------------------------------------------------------//

METHOD Paint() CLASS CTaskpanel

   LOCAL nCol, nWidth
   LOCAL aDisp

   IF ::lNeedLayout
      ::DoLayout()
      ::Refresh()
      RETURN Nil
   ENDIF

   aDisp := ::DispBegin()

   //////////////////////////
   // Painting Background
   //////////


   th_DrawTaskPanel( ::foTheme:nHandle,;                  //Theme Handle
                     ::hDc,;                             //hdc
                     0,;                                 //left
                     -::nVirtualTop,;                    //top
                     WndWidth( ::hWnd ),;                //right
                     ::nVirtualHeight - ::nVirtualTop,;  //bottom
                     ::nPlainStyle,;
                     ::aClientRect,;
                     ::nAnchor )

   //////////////////////////
   // Painting Header Bitmap
   //////////

   IF !(::hBitmap == nil)

      nWidth := nBmpWidth( ::hBitmap )

      DO CASE
         CASE ::nHeaderAlign == 0 ; nCol := ::aClientRect[ RC_LEFT ]
         CASE ::nHeaderAlign == 1 ; nCol := ::aClientRect[ RC_RIGHT ] - nWidth
         CASE ::nHeaderAlign == 2 ; nCol := (::aClientRect[ RC_RIGHT ] - ::aClientRect[ RC_LEFT ] - nWidth) / 2
      ENDCASE

      AlphaBitmapDraw( ::hDc,;
                        nCol ,;
                        ::aClientRect[ RC_TOP ] - ::nVirtualTop + 5 - ::nBitmapHeight,;
                        nWidth,;
                        nBmpHeight( ::hBitmap ),;
                        ::hBitmap,;
                        0, 0,;
                        255 )

   ENDIF

   //////////////////////////
   // Painting Headers
   //////////

   aEval( ::aControls, {|o| o:PaintHeader(),;
                         if( o:lAnimating, o:PaintAnimation( ::hDC ), ) } )

   IF ValType( ::bPainted ) == "B"
      Eval( ::bPainted, ::hDC, ::cPS, Self )
   ENDIF

   ::DispEnd( aDisp )

   RETURN Nil

//----------------------------------------------------------------------------//

METHOD ReSize( nSizeType, nWidth, nHeight ) CLASS CTaskpanel

   IF !::lLayout .And. !(::oVScroll == nil) .And. ::nLayoutCount == 0
      ::DoLayout()
   ENDIF

   RETURN Super:ReSize( nSizeType, nWidth, nHeight )

//----------------------------------------------------------------------------//

METHOD SetCtlFocus( hCtlFocus ) CLASS CTaskpanel

   LOCAL oDlg := ::GetDialogParent()

   IF !(oDlg == nil)
      oDlg:PostMsg( WM_NEXTDLGCTL, hCtlFocus, 1 )
   ELSE
      SetFocus( hCtlFocus )
   ENDIF

   RETURN nil

//----------------------------------------------------------------------------//

METHOD SBSetPos( nPos ) CLASS CTaskpanel

   ::nVirtualTop := nPos
   ::DoLayout()

   RETURN Nil

//----------------------------------------------------------------------------//

METHOD SBSkip( nSkip ) CLASS CTaskpanel

   LOCAL nHeight := ( ::nVirtualHeight - ::nHeight )

   IF (::nVirtualTop == 0 .And. nSkip < 0) .Or. ;
      (::nVirtualTop == nHeight .And. nSkip > 0)
      RETURN Nil
   ENDIF

   ::nVirtualTop += nSkip

   IF ::nVirtualTop < 0
      ::nVirtualTop := 0
   ELSEIF ::nVirtualTop > nHeight
      ::nVirtualTop := nHeight
   ENDIF

   ::DoLayout()

   RETURN Nil

//----------------------------------------------------------------------------//

METHOD SetHeader ( cIcon ) CLASS CTaskpanel

   LOCAL aBmpInfo
   ::lHasAlpha := .F.

   IF !(::hBitmap == Nil)
      DeleteObject( ::hBitmap )
      ::hBitmap       := Nil
      ::lHasAlpha     := .F.
      ::nBitmapHeight := 0
   ENDIF

   IF !(cIcon == nil)
      aBmpInfo    := LoadBitmapEx( GetResources(), cIcon, .T. )
      ::hBitmap   := aBmpInfo[1]
      ::lHasAlpha := aBmpInfo[2]

      IF ::hBitmap == 0
         ::hBitmap := nil
      ELSE
         ::nBitmapHeight := nBmpHeight( ::hBitmap ) + 10
      ENDIF

   ENDIF

   IF IsWindowVisible( ::hWnd )
      ::DoLayout()
   ENDIF

   RETURN Nil

//----------------------------------------------------------------------------//

METHOD ThemeChanged( lUpdate )

   DEFAULT lUpdate := .T.

   IF lUpdate
      ::foTheme:Update()
   ENDIF

   aEval( ::aControls, {|o| o:ThemeChanged() } )

   ::DoLayout( .T. )

   RETURN nil

//----------------------------------------------------------------------------//

METHOD Update() CLASS CTaskpanel

   LOCAL o
   LOCAL lAnimating := ::lAnimating

   IF IsWindowVisible( ::hWnd )

      PaintWindow( ::hWnd )

      FOR EACH o IN ::aControls
         IF lAnimating .Or. o:lNeedUpdate
            o:Update()
         ENDIF
      NEXT

   ENDIF

   RETURN Nil

//----------------------------------------------------------------------------//

PROCEDURE DebugProc( n )
   DEFAULT n := 1
   OutputDebug( "Called from: ", ProcFile( n + 1 ) + "->" + ProcName(n+1) + '(' + LTrim( Str( Procline( n + 1 ) ) ) + ')' )
   RETURN

//----------------------------------------------------------------------------//

PROCEDURE OutputDebug( ... )

   LOCAL xParam

   FOR EACH xParam IN hb_aParams()
      #ifdef __XHARBOUR__
         OutputDebugString( cStr( xParam ) + " " )
      #else
         OutputDebugString( hb_cStr( xParam ) + " " )
      #endif
   NEXT

   OutputDebugString( CRLF )

   RETURN

//----------------------------------------------------------------------------//
// EOF:
//-------//