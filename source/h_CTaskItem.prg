/**************************************************************************************************
* Proyecto     : TTaskpanel
* Archivo      : h_CTaskItem.prg
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

#define WM_MOUSELEAVE       675
#define TME_LEAVE             2

#define SW_HIDE                0
#define SW_SHOWNA              8

#define WM_ENABLE             10   //   0x000A
#define WM_PRINTCLIENT        792  //   0x0318

//----------------------------------------------------------------------------//

CLASS CTaskItem FROM TControl

   DATA lIsInfo               INIT .F.
   DATA lHidden               INIT .F.
   DATA lLayout               INIT .F.
   DATA lMouseOver            INIT .F.   
   DATA bAction

   DATA lHasAlpha             INIT .F.
   DATA hBitmap

   DATA crBackground
   DATA crBackgroundHot
   
   DATA nSeparatorStyle       INIT 0
   DATA nSeparatorHeight      INIT 12

   CLASSDATA lRegistered      AS LOGICAL

   METHOD New()
   METHOD CoorsUpdate()

   METHOD Display()                    INLINE ::BeginPaint(), ::Paint(), ::EndPaint(), 0

   METHOD DoLayout()
   METHOD Destroy()

   METHOD End()                        INLINE Super:End(), If( IsWindowVisible( ::oWnd:hWnd ), ::oWnd:DoLayout(), )

   METHOD EraseBkGnd( hDC )            INLINE 1

   METHOD GetDlgCode( nLastKey )
   METHOD GotFocus( hCtlLost )

   METHOD Hide()                    INLINE ShowWindow( ::hWnd, SW_HIDE ),;
                                           ::lHidden := .T.,;
                                           ::oWnd:DoLayout()


   METHOD HandleEvent( nMsg, nWParam, nLParam )
   METHOD KeyDown( nKey, nFlags )
   METHOD KeyChar( nKey, nFlags )

   METHOD LButtonUp( nRow, nCol, nKeyFlags )

   METHOD LostFocus( hWndGetFocus )    INLINE Super:LostFocus( hWndGetFocus ), ::Refresh()

   METHOD MouseLeave()
   METHOD MouseMove( nRow, nCol, bKeyFlags )

   METHOD oTaskPanel                   INLINE ::oWnd:oWnd

   METHOD oTheme                       INLINE ::oWnd:oWnd:foTheme

   METHOD Paint()
   METHOD PrintClient( hDc, uFlags )
   METHOD ReSize( nSizeType, nWidth, nHeight )
   METHOD SetIcon( cIcon )

   METHOD SetText( cText ) INLINE ;
                           ::cCaption := cText,;
                           SetWindowText( ::hWnd, cText ),;
                           ::DoLayout(),;
                           cText

   METHOD Show()                    INLINE ShowWindow( ::hWnd, SW_SHOWNA ),;
                                           ::lHidden := .F.,;
                                           ::oWnd:DoLayout()



END CLASS

//----------------------------------------------------------------------------//

METHOD New( cPrompt, cBitmap, bAction, oWnd, cTooltip, lInfo, bWhen, crBackground, crBackgroundHot )

   DEFAULT lInfo := .F.

   ::nId       := ::GetNewId()
   ::nTop      := 0
   ::nLeft     := 0
   ::nBottom   := 0
   ::nRight    := 0
   ::oWnd      := oWnd
   ::cCaption  := cPrompt
   ::bAction   := bAction
   ::cTooltip  := cTooltip
   ::lIsInfo   := lInfo
   ::bWhen     := bWhen

   ::crBackground    := crBackground
   ::crBackgroundHot := crBackgroundHot

   ::nStyle       := nOr( WS_CHILD, WS_VISIBLE, WS_CLIPCHILDREN, if( lInfo, 0, WS_TABSTOP ) )
   ::lDrag        := .f.

   ::Register( nOr( CS_PARENTDC, CS_VREDRAW, CS_HREDRAW ) )

   ::Create()

   IF !(cBitmap == nil)
      ::SetIcon( cBitmap )
   ENDIF

   RETURN Self

//----------------------------------------------------------------------------//

METHOD CoorsUpdate()

   local aRect := GetCoors( ::hWnd )

   ::nTop    = aRect[ 1 ]
   ::nLeft   = aRect[ 2 ]
   ::nBottom = aRect[ 3 ]
   ::nRight  = aRect[ 4 ]

   RETURN nil

//----------------------------------------------------------------------------//

METHOD Destroy()

   IF !(::hBitmap == Nil)
      DeleteObject( ::hBitmap )
   ENDIF

   IF ::hWnd != nil
      Super:Destroy()
   ENDIF

   RETURN nil

//----------------------------------------------------------------------------//

METHOD DoLayout()

   IF ::lLayout .Or. !(::oWnd:oWnd:nLayoutCount == 0)
      RETURN nil
   ENDIF

   ::lLayout := .T.

   th_TaskItemDoLayout( ::oTheme:nHandle,;
                        ::hWnd,;
                        ::GetText(),;
                        ::hBitmap,;
                        ::oWnd:lSpecial,;
                        ::lIsInfo,;
                        ::oWnd:oWnd:nPlainStyle )

   ::CoorsUpdate()

   ::lLayout := .F.

   IF IsWindowVisible( ::hWnd )
      ::oWnd:DoLayout()
   ENDIF

   RETURN Nil

//----------------------------------------------------------------------------//

METHOD GetDlgCode( nLastKey )

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

METHOD GotFocus( hCtlLost )

   Super:GotFocus( hCtlLost )

   IF (::oWnd:nTop + ::nBottom + 5 ) > ::oWnd:oWnd:nHeight
      ::oWnd:oWnd:SBSkip( (::oWnd:nTop + ::nBottom + 5) - ::oWnd:oWnd:nHeight )
      RETURN nil
   ELSEIF (::oWnd:nTop + ::nTop - 5 ) < 0
      ::oWnd:oWnd:SBSkip( (::oWnd:nTop + ::nTop - 5 ) )
   ENDIF

   ::Refresh()

   RETURN nil

//----------------------------------------------------------------------------//

METHOD HandleEvent( nMsg, nWParam, nLParam )

   DO CASE
      CASE nMsg == WM_MOUSELEAVE  ;  RETURN ::MouseLeave( nHiWord( nLParam ), nLoWord( nLParam ), nWParam )
      CASE nMsg == WM_PRINTCLIENT ;  RETURN ::PrintClient( nWParam, nLParam )
      CASE nMsg == WM_ENABLE      ;  ::Refresh()
   ENDCASE

   RETURN Super:HandleEvent( nMsg, nWParam, nLParam )

//----------------------------------------------------------------------------//

METHOD KeyChar( nKey, nFlags )

   IF nKey == VK_ESCAPE
      ::oWnd:KeyChar( nKey, nFlags )
      RETURN 0
   ENDIF

   IF nKey == VK_TAB .OR. nKey == VK_UP .OR. nKey == VK_DOWN .OR. ;
      nKey == VK_SPACE .OR. nKey == VK_RETURN

      RETURN 0

   ENDIF

   RETURN Super:KeyChar( nKey, nFlags )

//----------------------------------------------------------------------------//

METHOD KeyDown( nKey, nFlags )

   IF nKey == VK_ESCAPE
      ::oWnd:KeyDown( nKey, nFlags )
      RETURN 0
   ENDIF

   DO CASE
      CASE nKey == VK_TAB .and. GetKeyState( VK_SHIFT )
         ::oWnd:oWnd:GoCurrentExpando()
         RETURN 0
      CASE nKey == VK_TAB
         ::oWnd:oWnd:GoNextExpando()
         RETURN 0
      CASE nKey == VK_UP
         ::oWnd:GoPrevCtrl( ::hWnd )
         RETURN 0
      CASE nKey == VK_DOWN
         ::oWnd:GoNextCtrl( ::hWnd )
         RETURN 0
      CASE nKey == VK_SPACE .OR. nKey == VK_RETURN

         IF !(::bAction == nil)
            Eval( ::bAction, Self )
         ENDIF

         RETURN 0

   END CASE

   RETURN Super:KeyDown( nKey, nFlags )

//----------------------------------------------------------------------------//

METHOD LButtonUp( nRow, nCol, nKeyFlags )

   IF !(::bAction == nil)
      Eval( ::bAction, Self, nRow, nCol, nKeyFlags )
      RETURN nil
   ENDIF

   RETURN Super:LButtonUp( nRow, nCol, nKeyFlags )

//----------------------------------------------------------------------------//

METHOD MouseLeave()

   IF ::lMouseOver
      ::lMouseOver := .F.
      ::Refresh()
   ENDIF

   RETURN Nil

//----------------------------------------------------------------------------//

METHOD MouseMove( nRow, nCol, nKeyFlags )

   IF !::lMouseOver .And. !::lIsInfo
      ::lMouseOver := .T.
      ::Refresh()
   ENDIF

   IF ::lIsInfo
      CursorArrow()
   ELSE
      TrackMouseEvent( ::hWnd, TME_LEAVE )
      CursorHand()
   ENDIF

   ::SetMsg( ::cMsg )

   ::CheckToolTip()

   IF ::OnMouseMove != nil
      IF ValType( ::OnMouseMove ) == "B"
         Eval( ::OnMouseMove, Self, nRow, nCol, nKeyFlags )
      ENDIF
      IF ValType( ::OnMouseMove ) == "C"
         OSend( Self, ::OnMouseMove, Self, nRow, nCol, nKeyFlags )
      ENDIF
   ENDIF

   IF ::bMMoved != nil
      return Eval( ::bMMoved, nRow, nCol, nKeyFlags )
   ENDIF

   RETURN 0

//----------------------------------------------------------------------------//

METHOD Paint()

   LOCAL aDisp := ::DispBegin()

   SetBrushOrgEx( ::hDc, -::nLeft, -::nTop )
   FillRect( ::hDc, {0, 0, ::nHeight, ::nWidth } ,::oWnd:oBrush:hBrush )

   th_DrawTaskItemB( ::oTheme:nHandle,;
                     ::hDc,;
                     0, 0, ::nWidth, ::nHeight,;
                     ::oWnd:lSpecial,;
                     ::lFocused,;
                     ::lMouseOver,;
                     ::lIsInfo,;
                     ::oWnd:oWnd:nPlainStyle, ;
                     ::crBackground, ;
                     ::crBackgroundHot )

   th_DrawTaskItem( ::oTheme:nHandle,;
                     ::hDc,;
                     0, 0, ::nWidth, ::nHeight,;
                     ::GetText(),;
                     ::hBitmap,;
                     ::lHasAlpha,;
                     ::oWnd:lSpecial,;
                     ::lMouseOver,;
                     ::lActive,;
                     ::lIsInfo,;
                     ::oWnd:oWnd:nPlainStyle )

   IF ValType( ::bPainted ) == "B"
      Eval( ::bPainted, ::hDC, ::cPS, Self )
   ENDIF

   ::DispEnd( aDisp )

   RETURN Nil

//----------------------------------------------------------------------------//

METHOD PrintClient( hDc, uFlags )

   LOCAL hOldDc := ::hDc

   ::hDc := hDc
   ::Paint()
   ::hDc := hOldDc

   RETURN nil

//----------------------------------------------------------------------------//

METHOD SetIcon ( cIcon )

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

METHOD ReSize( nSizeType, nWidth, nHeight )

   IF !::lLayout
      ::DoLayout()
   ENDIF

   RETURN Super:ReSize( nSizeType, nWidth, nHeight )

//----------------------------------------------------------------------------//
