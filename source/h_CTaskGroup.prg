/**************************************************************************************************
* Proyecto     : TTaskpanel
* Archivo      : h_CTaskGroup.prg
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

#define RC_LEFT         1
#define RC_TOP          2
#define RC_RIGHT        3
#define RC_BOTTOM       4

#define SW_HIDE                0
#define SW_SHOWNA              8

#define  ANIMATION_EXPAND      1
#define  ANIMATION_COLLAPSE    2

#define GWL_STYLE         -16

#define WM_PRINTCLIENT   792  //   0x0318

#define TRANSPARENT_CONTROLS "TCHECKBOX|TSAY|TGROUP|TRADIO"
//----------------------------------------------------------------------------//

CLASS CTaskPanelGroup FROM TControl


   DATA lSpecial
   DATA lAutoSize
   DATA lLayout               INIT .F.
   DATA lHeaderVisible
   DATA lHidden               INIT .F.
   DATA lCollapsed            INIT .F.
   DATA lNeedUpdate           INIT .F.
   DATA lCollapsable

   DATA fnHeaderHeight        INIT 0

   DATA bAction
   DATA bOnCollapse
   DATA bOnExpand

   //////////////////////////
   // Bitmap de Cabecera y Bitmap de Fondo
   //////////

   DATA hBitmap
   DATA hWaterMark
   DATA lHasAlpha              INIT .F.
   DATA lHasAlphaW             INIT .F.

   //////////////////////////
   // Datas del Motor de Animacion
   //////////

   DATA oTimer
   DATA nAnimationType
   DATA aAnimationHeight
   DATA nStepCounter           INIT 0
   DATA lAnimating             INIT .F.
   DATA nOldHeight             INIT 0

   DATA hAnimationBitmap
   DATA nVirtualHeight         INIT 0

   CLASSDATA lRegistered      AS LOGICAL

   METHOD New()
   METHOD AddControl( oControl )

   METHOD AddInfo( cPrompt, cBitmap, cTooltip )   INLINE ::AddTask( cPrompt, cBitmap, Nil, cTooltip, .T. )

   METHOD AddTask( cPrompt, cBitmap, bAction, cTooltip, lInfo, bWhen )
   METHOD Animate( n )
   METHOD CheckTooltip()            VIRTUAL
   METHOD Collapse()
   METHOD CoorsUpdate()
   METHOD DeleteTask( nItem )
   METHOD Destroy()
   METHOD Display()                 INLINE ::BeginPaint(), ::Paint(), ::EndPaint(), 0
   METHOD DoLayout()
   METHOD EndAnimation()
   METHOD EraseBkGnd( hDC )         INLINE 1
   METHOD Expand()
   METHOD GetDlgCode( nLastKey )
   METHOD GotFocus( hCtlLost )
   METHOD GoPrevCtrl( hCtrl )
   METHOD GoNextCtrl( hCtrl )

   METHOD Hide()                    INLINE ShowWindow( ::hWnd, SW_HIDE ),;
                                           ::lHidden := .T.,;
                                           ::oWnd:DoLayout()


   METHOD HandleEvent( nMsg, nWParam, nLParam )
   METHOD IsOverHeader( nRow, nCol )
   METHOD InsertTask( nPos, cPrompt, cBitmap, bAction, cTooltip, lInfo )

   METHOD Item( n )                 INLINE ::aControls[n]

   METHOD KeyDown( nKey, nFlags )

   METHOD lAnimate()                INLINE ::oWnd:lAnimate

   METHOD lHasActiveCtrl()
   METHOD Move( nTop, nLeft, nWidth, nHeight, lRepaint )

   METHOD Moved()                   INLINE ::lNeedUpdate := .T., nil

   METHOD nBmpHeight                INLINE if( !(::hBitmap == nil), nBmpHeight( ::hBitmap ), 0 )

   METHOD nHeaderHeight()
   METHOD nMarginToPanel()
   METHOD nMarginToTask()

   METHOD oTheme()                  INLINE ::oWnd:foTheme

   METHOD Paint()
   METHOD PaintAnimation()
   METHOD PaintHeader()
   METHOD PrintClient( hDc, uFlags )
   METHOD ReSize( nSizeType, nWidth, nHeight )
   METHOD SetFocus()
   METHOD SetIcon( cIcon )
   METHOD SetWatermark( cWatermark )

   METHOD Show()                    INLINE ShowWindow( ::hWnd, SW_SHOWNA ),;
                                           ::lHidden := .F.,;
                                           ::oWnd:DoLayout()

   METHOD ThemeChanged()
   METHOD Toggle()
   METHOD Update()
   METHOD UpdateBrush()

END CLASS

//----------------------------------------------------------------------------//

METHOD New( cPrompt, cBitmap, lSpecial, oWnd, cWatermark, nHeight, lHeader, lCollapsed, lCollapsable, bOnCollapse, bOnExpand, bOnClick ) CLASS CTaskPanelGroup

   DEFAULT lSpecial     := .F.,;
           lHeader      := .T.,;
           lCollapsed   := .F.,;
           lCollapsable := oWnd:lCollapsable


   ::nId             := ::GetNewId()
   ::nTop            := 0
   ::nLeft           := 0
   ::nBottom         := if( nHeight == nil, 0, nHeight )
   ::nRight          := 0
   ::cCaption        := cPrompt
   ::lSpecial        := lSpecial
   ::lAutoSize       := ( nHeight == nil )
   ::lHeaderVisible  := lHeader
   ::lCollapsable    := lCollapsable
   ::oWnd            := oWnd

   ::bOnCollapse     := bOnCollapse
   ::bOnExpand       := bOnExpand
   ::bAction         := bOnClick

   ::nClrPane        := th_GetTaskGroupBackground( ::oTheme:nHandle, lSpecial, oWnd:nPlainStyle )

   ::nStyle       := nOr( WS_CHILD, WS_VISIBLE, WS_CLIPCHILDREN, WS_CLIPSIBLINGS ,WS_TABSTOP )
   ::lDrag        := .f.
   ::lVisible     := .T.

   ::aControls    := {}

   ::Register( nOr( CS_PARENTDC, CS_VREDRAW, CS_HREDRAW ) )

   ::Create()
   ::oWnd:AddControl( Self )

   IF !( ::oBrush == nil )
      ::oBrush:End()
   ENDIF

   ::oBrush := TBrush():New( Str(::hWnd) )

   ::GetFont()

   IF !(cBitmap == nil)
      ::SetIcon( cBitmap )
   ENDIF

   IF !(cWatermark == nil)
      ::SetWatermark( cWatermark )
   ENDIF

   IF lCollapsed
      ::lCollapsed         := .T.
      ::lVisible           := .F.
      ShowWindow( ::hWnd, SW_HIDE )
   ENDIF

   RETURN Self

//----------------------------------------------------------------------------//

METHOD AddControl( oControl ) CLASS CTaskPanelGroup

   Super:AddControl( oControl )

   IF oControl:Classname $ TRANSPARENT_CONTROLS
      oControl:lTransparent := .T.
      oControl:SetFont( ::oFont )
      oControl:SetColor(, ::nClrPane )
   ENDIF

   /*****
   * Controles Soportados
   * TSay ... OK
   * TCheckbox ... OK
   * TGet ... OK
   * TButton ... OK
   * TCombobox ... OK
   * TMeter
   * TProgrees
   * */

   IF IsWindowVisible( ::hWnd )
      ::DoLayout()
   ENDIF

   RETURN nil

//----------------------------------------------------------------------------//

METHOD AddTask( cPrompt, cBitmap, bAction, cTooltip, lInfo, bWhen, crBackground, crBackgroundHot ) CLASS CTaskPanelGroup

   aAdd( ::aControls, TTaskItem():New( cPrompt, cBitmap, bAction, Self, cTooltip, lInfo, bWhen, crBackground, crBackgroundHot ) )

   IF IsWindowVisible( ::hWnd )
      ::DoLayout()
   ENDIF

   RETURN aTail( ::aControls )

//----------------------------------------------------------------------------//

METHOD Animate() CLASS CTaskPanelGroup

   IF ::nAnimationType == ANIMATION_COLLAPSE

      ++::nStepCounter

      IF ::nStepCounter == 23
         ::lCollapsed := .T.
         ::EndAnimation()
      ELSE
         ::nVirtualHeight := ::aAnimationHeight[ ::nStepCounter ]
      ENDIF

   ELSEIF ::nAnimationType == ANIMATION_EXPAND

      --::nStepCounter

      IF ::nStepCounter == 0
         ::lCollapsed := .F.
         ::EndAnimation()

         ::lVisible := .T.
         ShowWindow( ::hWnd , SW_SHOWNA )
      ELSE
         ::nVirtualHeight := ::aAnimationHeight[ ::nStepCounter ]
      ENDIF

   ENDIF

   ::oWnd:DoLayout()

   IF !::lAnimating
      IF ::nAnimationType == ANIMATION_COLLAPSE
         IF ValType( ::bOnCollapse ) == "B"
            Eval( ::bOnCollapse, Self )
         ENDIF
      ELSEIF ::nAnimationType == ANIMATION_EXPAND
         IF ValType( ::bOnExpand ) == "B"
            Eval( ::bOnExpand, Self )
         ENDIF
      ENDIF
      ::nAnimationType := nil
   ENDIF

   RETURN nil

//----------------------------------------------------------------------------//

METHOD Collapse() CLASS CTaskPanelGroup

   IF ::nHeight == 0
      RETURN nil
   ENDIF

   IF !::lAnimating .And. !::lCollapsed

      IF ::lAnimate .And. ::oWnd:lCanAnimate

         ::oTimer             := TTimer():New( 10 , {|| ::Animate() }, Self )

         ::nAnimationType     := ANIMATION_COLLAPSE
         ::lAnimating         := .T.
         ::nStepCounter       := 0
         ::aAnimationHeight   := CalcAnimationHeights( 23, ::nHeight )
         ::hAnimationBitmap   := GetBitmapFromWnd( ::hWnd )
         ::nVirtualHeight     := ::nHeight
         ::lVisible           := .F.

         ShowWindow( ::hWnd, SW_HIDE )

         ::oTimer:Activate()

      ELSE

         //////////////////////////
         // Simplemente Ocultamos la ventana y mandamos reposicionar
         // los grupos de tareas necesarios
         //////////

         ::lCollapsed         := .T.
         ::lVisible           := .F.
         ShowWindow( ::hWnd, SW_HIDE )
         ::oWnd:DoLayout()

         IF ValType( ::bOnCollapse ) == "B"
            Eval( ::bOnCollapse, Self )
         ENDIF

      ENDIF

   ENDIF

   RETURN Nil

//----------------------------------------------------------------------------//

METHOD CoorsUpdate() CLASS CTaskPanelGroup

   local aRect := GetCoors( ::hWnd )

   ::nTop    = aRect[ 1 ]
   ::nLeft   = aRect[ 2 ]
   ::nBottom = aRect[ 3 ]
   ::nRight  = aRect[ 4 ]

   RETURN nil

//----------------------------------------------------------------------------//

METHOD DeleteTask( nItem ) CLASS CTaskPanelGroup

   LOCAL o := ::Item( nItem )

   IF o != nil
      o:End()
      RETURN .T.
   ENDIF

   RETURN .F.

//----------------------------------------------------------------------------//

METHOD Destroy() CLASS CTaskPanelGroup

   IF !(::hBitmap == Nil)
      DeleteObject( ::hBitmap )
   ENDIF

   IF !(::hWaterMark == Nil)
      DeleteObject( ::hWaterMark )
   ENDIF

   IF ::hWnd != nil
      Super:Destroy()
   ENDIF

   RETURN nil

//----------------------------------------------------------------------------//

METHOD DoLayout( lUpdate ) CLASS CTaskPanelGroup

   LOCAL aPadding
   LOCAL nTop, nHeight
   LOCAL oObj
   LOCAL nMargin := 0, nBottom

   DEFAULT lUpdate := .F.

   //////////////////////////
   // Si ya estamos procesando la disposicion del TaskPanelGroup
   // no necesitamos hacerla de nuevo
   //////////

   IF ::lLayout .OR. !(::oWnd:nLayoutCount == 0)
      RETURN Nil
   ENDIF

   ::lLayout     := .T.
   ::lNeedUpdate := .T.

   ::aEvalWhen()

   IF !(::bAction == nil)
      ::nHeight := 0

      IF ::lVisible
         ShowWindow( ::hWnd, SW_HIDE )
         ::lVisible := .F.
      ENDIF

      ::lLayout := .F.
      RETURN nil
   ENDIF


   //////////////////////////
   // Inicializamos algunas variables
   //////////

   aPadding    := ::oTheme:TaskPanel:Tasklist( ::lSpecial ):Padding
   nTop        := aPadding[ RC_TOP ]

   FOR EACH oObj IN ::aControls

      IF oObj:IsKindOf( "TTASKITEM" )
         IF !oObj:lHidden
            oObj:Move( nTop,;
                       aPadding[ RC_LEFT ],;
                       ::nWidth - aPadding[ RC_LEFT ] - aPadding[ RC_RIGHT ],;
                       ,.F. )

            IF lUpdate
               oObj:DoLayout()
            ENDIF

            nTop += oObj:nHeight + (nMargin := ::nMarginToTask)
						
			IF oObj:nSeparatorStyle > 0
			   nTop += oObj:nSeparatorHeight
			ENDIF 
         ENDIF
      ELSE

         nBottom := GetCoors( oObj:hWnd )[ 3 ]

         IF nTop < nBottom
            nTop := nBottom + (nMargin := 3 )
         ENDIF

         //Si contamos con controles que no seas TTaskItem
         //Estonces desactivamos la Animacion
         ::oWnd:lCanAnimate := .F.
      ENDIF

    NEXT

   nHeight := nTop + aPadding[ RC_BOTTOM ] - nMargin



   IF !::lAutosize

      //////////////////////////
      // Para los que no son autoajustables manejaremos como minimo
      // el relleno superior e inferior
      //////////

      IF ::nHeight < (aPadding[ RC_TOP ] + aPadding[ RC_BOTTOM ])
         nHeight := (aPadding[ RC_TOP ] + aPadding[ RC_BOTTOM ])
      ELSE
         nHeight := ::nHeight
      ENDIF
   ENDIF

   ::Move( ,,, nHeight, .F. )

   ::lLayout := .F.

   ::UpdateBrush()

   IF ::lAnimating
      DeleteObject( ::hAnimationBitmap )
      ::aAnimationHeight   := CalcAnimationHeights( 23, ::nHeight )
      ::hAnimationBitmap   := GetBitmapFromWnd( ::hWnd )
   ENDIF

   IF IsWindowVisible( ::hWnd )
      ::oWnd:DoLayout()
   ELSE
      ::oWnd:lNeedLayout := .T.
   ENDIF

   RETURN nHeight

//----------------------------------------------------------------------------//

METHOD EndAnimation() CLASS CTaskPanelGroup

   IF !(::oTimer == nil)
      ::oTimer:End()
      ::oTimer          := nil
      ::nVirtualHeight  := 0
      ::lAnimating      := .F.

      DeleteObject( ::hAnimationBitmap )
      ::hAnimationBitmap := Nil

   ENDIF

   RETURN Nil

//----------------------------------------------------------------------------//

METHOD Expand() CLASS CTaskPanelGroup

   IF ::nHeight == 0
      RETURN nil
   ENDIF

   IF !::lAnimating .And. ::lCollapsed

      IF ::lAnimate .And. ::oWnd:lCanAnimate

         ::DoLayout()

         ::oTimer             := TTimer():New( 10 , {|| ::Animate() }, Self )

         ::lAnimating         := .T.
         ::nAnimationType     := ANIMATION_EXPAND
         ::nStepCounter       := 24
         ::aAnimationHeight   := CalcAnimationHeights( 23, ::nHeight )
         ::hAnimationBitmap   := GetBitmapFromWnd( ::hWnd )
         ::nVirtualHeight     := 0
         ::lVisible           := .F.

         ::oTimer:Activate()

      ELSE

         //////////////////////////
         // Simplemente Mostramos la ventana y mandamos reposicionar
         // los grupos de tareas necesarios
         //////////

         ::lCollapsed := .F.
         ::lVisible  := .T.
         ShowWindow( ::hWnd , SW_SHOWNA )
         ::oWnd:DoLayout()

         IF ValType( ::bOnExpand ) == "B"
            Eval( ::bOnExpand, Self )
         ENDIF

      ENDIF
   ENDIF

   RETURN Nil

//----------------------------------------------------------------------------//

METHOD GetDlgCode( nLastKey ) CLASS CTaskPanelGroup

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

METHOD GotFocus( hCtlLost ) CLASS CTaskPanelGroup

   Super:GotFocus( hCtlLost )

   IF ::aControls != nil .and. Len( ::aControls ) > 0
      IF ::aControls[ 1 ] != nil
         ::hCtlFocus = NextDlgTab( ::hWnd )
         ::oWnd:SetCtlFocus( ::hCtlFocus )
      ENDIF
   ENDIF

   RETURN nil

//----------------------------------------------------------------------------//

METHOD GoNextCtrl( hCtrl ) CLASS CTaskPanelGroup

   LOCAL hCtlNext := NextDlgTab( ::hWnd, hCtrl )
   LOCAL nAt, nAtCtrl

   //Bug GoNextCtrl is called 2 times consecutively
   IF  !(GetFocus() == hCtrl)
      RETURN Nil
   ENDIF

   nAtCtrl := AScan( ::aControls, { | o | o:hWnd == hCtrl } )
   nAt     := AScan( ::aControls, { | o | o:hWnd == hCtlNext } )

   IF nAt != 0
      IF nAt > nAtCtrl .And. ::aControls[ nAt ]:hWnd != hCtrl
         ::oWnd:SetCtlFocus( ::aControls[ nAt ]:hWnd )
      ELSE
         IF GetFocus() != ::oWnd:hWnd
            ::oWnd:GoNextExpando()
         ENDIF
      ENDIF
      RETURN nil
   ENDIF

   ::hCtlFocus = hCtlNext

   RETURN nil

//----------------------------------------------------------------------------//

METHOD GoPrevCtrl( hCtrl ) CLASS CTaskPanelGroup

   LOCAL hCtlNext := NextDlgTab( ::hWnd, hCtrl, .T. )
   LOCAL nAt, nAtCtrl

   //Bug GoPrevCtrl is called 2 times consecutively
   IF  !(GetFocus() == hCtrl)
      RETURN Nil
   ENDIF

   nAtCtrl := AScan( ::aControls, { | o | o:hWnd == hCtrl } )
   nAt     := AScan( ::aControls, { | o | o:hWnd == hCtlNext } )

   IF nAt != 0
      IF nAt < nAtCtrl .And. ::aControls[ nAt ]:hWnd  != hCtrl
         ::oWnd:SetCtlFocus( ::aControls[ nAt ]:hWnd )
      ELSE
         IF GetFocus() != ::oWnd:hWnd
            ::oWnd:GoCurrentExpando()
         ENDIF
      ENDIF
      RETURN nil
   ENDIF

   ::hCtlFocus = hCtlNext

   RETURN nil

//----------------------------------------------------------------------------//

METHOD HandleEvent( nMsg, nWParam, nLParam ) CLASS CTaskPanelGroup

   IF nMsg == WM_PRINTCLIENT
      RETURN ::PrintClient( nWParam, nLParam )
   ENDIF

   RETURN Super:HandleEvent( nMsg, nWParam, nLParam )

//----------------------------------------------------------------------------//

METHOD InsertTask( nPos, cPrompt, cBitmap, bAction, cTooltip, lInfo, bWhen ) CLASS CTaskPanelGroup

   LOCAL oTask  := TTaskItem():New( cPrompt, cBitmap, bAction, Self, cTooltip, lInfo, bWhen )
   LOCAL i      := 0

   IF nPos <= Len( ::aControls )

      aAdd( ::aControls, nil )
      aIns( ::aControls, nPos )
      ::aControls[ nPos ] := oTask

      SetWindowPos( oTask:hWnd,;
                    if( nPos == 1, 0, ::aControls[ nPos - 1 ]:hWnd ),;
                    0, 0, 0, 0, 3 )
   ELSE
      aAdd( ::aControls, oTask )
   ENDIF

   IF IsWindowVisible( ::hWnd )
      ::DoLayout()
   ENDIF

   RETURN oTask


//----------------------------------------------------------------------------//

METHOD IsOverHeader( nRow, nCol ) CLASS CTaskPanelGroup
   RETURN (nRow >= (::nTop - ::nHeaderHeight) .And. nRow <= (::nTop - 1)) .And. ;
          (nCol >= ::nLeft .And. nCol <= ::nRight) .And. ::lHeaderVisible .And. !::lHidden

//----------------------------------------------------------------------------//

METHOD KeyDown( nKey, nFlags ) CLASS CTaskPanelGroup

   IF nKey == VK_ESCAPE
      ::oWnd:KeyChar( nKey, nFlags )
      RETURN 0
   ENDIF

   DO CASE
      CASE nKey == VK_TAB .and. GetKeyState( VK_SHIFT )
         ::oWnd:lShowFocusRect := .T.
         ::oWnd:GoCurrentExpando()
         RETURN 0
      CASE nKey == VK_TAB
         ::oWnd:lShowFocusRect := .T.
         ::oWnd:GoNextExpando()
         RETURN 0
      CASE nKey == VK_UP .OR. nKey == VK_DOWN .OR. ;
		   nKey == VK_SPACE .OR. nKey == VK_RETURN
         RETURN 0
   END CASE

   RETURN Super:KeyDown( nKey, nFlags )

//----------------------------------------------------------------------------//

METHOD lHasActiveCtrl() CLASS CTaskPanelGroup

   LOCAL oControl

   FOR EACH oControl IN ::aControls
      IF oControl:lActive
         IF lAnd( GetWindowLong( oControl:hWnd, GWL_STYLE ), WS_TABSTOP )
            RETURN .T.
         ENDIF
      ENDIF
   NEXT

   RETURN .F.

//----------------------------------------------------------------------------//

METHOD Move( nTop, nLeft, nWidth, nHeight, lRepaint ) CLASS CTaskPanelGroup

   DEFAULT nTop     := ::nTop,;
           nLeft    := ::nLeft,;
           nWidth   := ::nWidth,;
           nHeight  := ::nHeight,;
           lRepaint := .T.

   MoveWindow( ::hWnd, nTop, nLeft, nWidth, nHeight, lRepaint )

   ::CoorsUpdate()

   RETURN Nil

//----------------------------------------------------------------------------//

METHOD Paint() CLASS CTaskPanelGroup

   LOCAL aDisp := aDisp := ::DispBegin()

   SetBrushOrgEx( ::hDc, 0, 0 )
   FillRect( ::hDc, {0, 0, ::nHeight, ::nWidth } ,::oBrush:hBrush )

   IF ValType( ::bPainted ) == "B"
      Eval( ::bPainted, ::hDC, ::cPS, Self )
   ENDIF

   ::DispEnd( aDisp )

   RETURN Nil

//----------------------------------------------------------------------------//

METHOD PaintAnimation( hDC ) CLASS CTaskPanelGroup

   AlphaBitmapDraw( hdc,;
              ::nLeft,;
              ::nTop,;
              ::nWidth,;
              ::nVirtualHeight,;
              ::hAnimationBitmap,;
              0, ::nHeight - ::nVirtualHeight,;
              (23 - ::nStepcounter) * (255/23) )

   RETURN Nil

//----------------------------------------------------------------------------//

METHOD PaintHeader() CLASS CTaskPanelGroup

   IF ::lHeaderVisible .And. !::lHidden
      ::oWnd:GetDc()

      th_DrawTaskPanelGroupHeader(  ::oTheme:nHandle,;
                                    ::oWnd:hDc,;
                                    ::nLeft,;
                                    ::nTop - ::nHeaderHeight,;
                                    ::nRight,;
                                    ::nTop,;
                                    ::GetText(),;
                                    ::hBitmap,;
                                    ::lHasAlpha,;
                                    ::lSpecial,;
                                    ::lCollapsed,;
                                    ::lCollapsable .And. ::bAction == nil,;
                                    ::oWnd:oItemFocused == Self .And. GetFocus() == ::oWnd:hWnd .And. ::oWnd:lShowFocusRect,;
                                    ::oWnd:oMouseOver == Self,;
                                    ::oWnd:nPlainStyle )

      ::oWnd:ReleaseDc()
   ENDIF

   RETURN Nil

//----------------------------------------------------------------------------//

METHOD PrintClient( hDc, uFlags ) CLASS CTaskPanelGroup

   LOCAL hOldDc := ::hDc

   ::hDc := hDc
   ::Paint()
   ::hDc := hOldDc

   RETURN nil

//----------------------------------------------------------------------------//

METHOD nHeaderHeight( nWidth ) CLASS CTaskPanelGroup

   IF !(nWidth == nil)
      ::oWnd:GetDc()

      ::fnHeaderHeight := th_TPGroupHeaderHeight( ;
                                             ::oTheme:nHandle,;
                                             ::oWnd:hDc,;
                                             ::GetText(),;
                                             ::hBitmap,;
                                             ::lSpecial,;
                                             nWidth )

      ::oWnd:ReleaseDc()
   ENDIF

   RETURN ::fnHeaderHeight

//----------------------------------------------------------------------------//

METHOD nMarginToPanel() CLASS CTaskPanelGroup
   RETURN ::oTheme:TaskPanel:Header( ::lSpecial ):MarginToPanel

//----------------------------------------------------------------------------//

METHOD nMarginToTask() CLASS CTaskPanelGroup
   RETURN ::oTheme:TaskPanel:Tasklist( ::lSpecial ):MarginToTask

//----------------------------------------------------------------------------//

METHOD ReSize( nSizeType, nWidth, nHeight ) CLASS CTaskPanelGroup

   IF !::lLayout .And. ::oWnd:nLayoutCount == 0
      ::DoLayout()
   ENDIF

   RETURN Super:ReSize( nSizeType, nWidth, nHeight )

//----------------------------------------------------------------------------//

METHOD SetFocus() CLASS CTaskPanelGroup

   ::oWnd:oItemFocused := Self
   ::oWnd:SetCtlFocus( ::oWnd:hWnd )

   IF ::nTop > ::oWnd:nHeight
      ::oWnd:SbSkip( ::nTop - ::oWnd:nHeight )
   ELSEIF (::nTop - ::nHeaderHeight) < 0
      ::oWnd:SbSkip( ::nTop - ::nHeaderHeight )
   ELSE
      ::oWnd:Refresh()
   ENDIF

   RETURN Nil

//----------------------------------------------------------------------------//

METHOD SetIcon ( cIcon ) CLASS CTaskPanelGroup

   LOCAL aBmpInfo
   ::lHasAlpha := .F.

   IF !(::hBitmap == Nil)
      DeleteObject( ::hBitmap )
      ::hBitmap := Nil
   ENDIF

   IF !(cIcon == nil)
      aBmpInfo    := LoadBitmapEx( GetResources(), cIcon, .T. )
      ::hBitmap   := aBmpInfo[1]
      ::lHasAlpha := aBmpInfo[2]

      IF ::hBitmap == 0
         ::hBitmap := nil
      ENDIF
   ENDIF

   IF IsWindowVisible( ::hWnd )
      ::DoLayout()
   ENDIF

   RETURN Nil

//----------------------------------------------------------------------------//

METHOD SetWaterMark ( cWaterMark ) CLASS CTaskPanelGroup

   LOCAL aBmpInfo
   ::lHasAlphaW := .F.

   IF !(::hWaterMark == Nil)
      DeleteObject( ::hWaterMark )
   ENDIF

   IF !(cWaterMark == nil)
      aBmpInfo     := LoadBitmapEx( GetResources(), cWaterMark, .T. )
      ::hWaterMark := aBmpInfo[1]
      ::lHasAlphaW := aBmpInfo[2]

      IF ::hWaterMark == 0
         ::hWaterMark := nil
      ENDIF
   ENDIF

   IF IsWindowVisible( ::hWnd )
      ::DoLayout()
   ENDIF

   RETURN Nil

//----------------------------------------------------------------------------//

METHOD ThemeChanged() CLASS CTaskPanelGroup

   LOCAL o

   ::nClrPane  := th_GetTaskGroupBackground( ::oTheme:nHandle, ::lSpecial, ::oWnd:nPlainStyle )

   FOR EACH o IN ::aControls

      IF o:Classname $ TRANSPARENT_CONTROLS
         o:SetColor(, ::nClrPane )
      ENDIF

   NEXT

   RETURN Nil

//----------------------------------------------------------------------------//

METHOD Toggle() CLASS CTaskPanelGroup

   IF ::lAnimating
      ::nAnimationType := if( ::nAnimationType == ANIMATION_EXPAND, ANIMATION_COLLAPSE, ANIMATION_EXPAND )
      RETURN Nil
   ENDIF

   IF !::lCollapsed
      ::Collapse()
   ELSE
      ::Expand()
   ENDIF

   RETURN Nil

//----------------------------------------------------------------------------//

METHOD Update() CLASS CTaskPanelGroup

   LOCAL hBitmap

   IF IsWindowVisible( ::hWnd )
      PaintWindow( ::hWnd )
      aEval( ::aControls, {|o| PaintWindow( o:hWnd, !(o:IsKindOf("TTASKITEM")) ) } )
   ENDIF

   ::lNeedUpdate := .F.

   RETURN Nil

//----------------------------------------------------------------------------//

METHOD UpdateBrush() CLASS CTaskPanelGroup

   LOCAL hDc, hOldBmp, oObj

   DeleteObject( ::oBrush:hBitmap )
   DeleteObject( ::oBrush:hBrush )

   ::GetDc()

   hDC               := CreateCompatibleDc( ::hDc )
   ::oBrush:hBitmap  := CreateCompatibleBitmap( ::hDc, ::nWidth, ::nHeight )
   hOldBmp           := SelectObject( hDC, ::oBrush:hBitmap )

   th_DrawTaskPanelGroup(  ::oTheme:nHandle,;     //Theme Handle
                           hDC,;                     //hdc
                           ::lSpecial,;                //Normal or Special Group
                           0,;                         //left
                           0,;                         //top
                           WndWidth( ::hWnd ),;        //right
                           WndHeight( ::hWnd ),;       //bottom
                           ::hWaterMark,;
                           ::lHasAlphaW,;
                           ::lHeaderVisible,;
                           ::oWnd:nPlainStyle ;
                           )
						   
   FOR EACH oObj IN ::aControls
       IF oObj:IsKindOf( "TTASKITEM" )
	      IF !oObj:lHidden .And. oObj:nSeparatorStyle > 0
		     th_DrawTaskPanelGroupSep( ::oTheme:nHandle,;
			                           hDC,;
									   ::lSpecial,;
									   oObj:nSeparatorStyle,;
							           WndWidth( ::hWnd ),;		   
									   oObj:nBottom + ( oObj:nSeparatorHeight / 2 ) ,;
									   ::oWnd:nPlainStyle ;
									   )
		  ENDIF
	   ENDIF	
   NEXT

   SelectObject( hDC, hOldBmp )
   DeleteDC( hDC )

   ::oBrush:hBrush  := CreatePatternBrush( ::oBrush:hBitmap )

   ::ReleaseDc()

   RETURN Nil

//----------------------------------------------------------------------------//
