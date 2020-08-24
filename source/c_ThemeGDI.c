/**************************************************************************************************
* Project      : TTaskpanel
* File         : c_ThemeGDI.c
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
#include "stdafx.h"
//----------------------------------------------------------------------------//

static void DegradeLineX( HDC hDC, int * x, COLORREF crStart, COLORREF crEnd , BOOL bVertical )
{
   int nSegments = 100;
   COLORREF cr;
   int nR      = GetRValue(crStart);
   int nG      = GetGValue(crStart);
   int nB      = GetBValue(crStart);
   int neB     = GetBValue(crEnd);
   int neG     = GetGValue(crEnd);
   int neR     = GetRValue(crEnd);
   int nDiffR  = (neR - nR);
   int nDiffG  = (neG - nG);
   int nDiffB  = (neB - nB);
   int ndR ;
   int ndG ;
   int ndB ;
   int iStep, iNext, iLast;
   int i;
   HPEN  hPen, hOldPen;

   if( nSegments > ( x[1] - x[0] ) )
      nSegments = ( x[1] - x[0] );

   ndR = 256 * (nDiffR) / (max(nSegments,1));
   ndG = 256 * (nDiffG) / (max(nSegments,1));
   ndB = 256 * (nDiffB) / (max(nSegments,1));

   iStep = ( x[1] - x[0] ) / max( nSegments, 1 );
   iLast = x[0];

   nR *= 256;
   nG *= 256;
   nB *= 256;

   hPen    = CreatePen( BS_SOLID, 1, crStart );
   hOldPen = SelectPen( hDC, hPen );

   for (i = 0; i < nSegments; i++, nR += ndR, nG += ndG, nB += ndB)
   {
      if( i == (nSegments - 1) )
         iNext = x[1];
      else
         iNext = iLast + iStep;

      cr = RGB(nR / 256, nG / 256, nB / 256);

      {
         hPen = CreatePen( BS_SOLID , 1, cr );
         DeletePen( SelectPen( hDC, hPen ) );

         if( bVertical )
         {
             MoveToEx( hDC, x[2], iLast, NULL ) ;
             LineTo( hDC, x[2],iNext );
         }
         else
         {
             MoveToEx( hDC, iLast, x[2], NULL ) ;
             LineTo( hDC, iNext, x[2] );
         }
      }
      iLast = iNext;
   }

   SelectPen( hDC, hOldPen );
   DeletePen( hPen );

}

//----------------------------------------------------------------------------//

static void DegradeFillX( HDC hDC, RECT* rct, COLORREF crStart, COLORREF crEnd, BOOL bVertical )
{
   HDC     hDC2 = CreateCompatibleDC( hDC );
   HBITMAP hBmp, hOldBmp;
   HBRUSH  hBrush;
   POINT   pt;
   int     x[3];

   if( bVertical )
   {
      hBmp    = CreateCompatibleBitmap( hDC, 1, rct->bottom - rct->top + 1 );
      hOldBmp = SelectBitmap( hDC2, hBmp );

      x[0] = 0;
      x[1] = rct->bottom - rct->top + 1;
      x[2] = 0;

      DegradeLineX( hDC2, x, crStart, crEnd, TRUE );
   }
   else
   {
      hBmp    = CreateCompatibleBitmap( hDC, rct->right - rct->left + 1, 1 );
      hOldBmp = SelectBitmap( hDC2, hBmp );

      x[0] = 0;
      x[1] = rct->right - rct->left + 1;
      x[2] = 0;

      DegradeLineX( hDC2, x, crStart, crEnd, FALSE );
   }

   SelectBitmap( hDC2, hOldBmp );
   DeleteDC( hDC2 );

   hBrush = CreatePatternBrush( hBmp );
   SetBrushOrgEx( hDC,rct->left, rct->top, &pt );
   FillRect( hDC, rct, hBrush );
   SetBrushOrgEx( hDC, pt.x, pt.y, NULL );

   DeleteBitmap( hBmp );
   DeleteBrush( hBrush );

}

//----------------------------------------------------------------------------//

HB_FUNC( ALPHABITMAPDRAW )
{
   HDC               hdc = (HDC) hb_parnl(1);
   HBITMAP           hBitmap = (HBITMAP) hb_parnl(6);
   BLENDFUNCTION     bf;
   BITMAP            bm;
   HBITMAP           hOldBmp;
   HDC               hdc2;

   bf.BlendOp             = AC_SRC_OVER;
   bf.BlendFlags          = 0;
   bf.SourceConstantAlpha = (BYTE) hb_parni(9);
   bf.AlphaFormat         = AC_SRC_ALPHA;

   hdc2    = CreateCompatibleDC( hdc );
   hOldBmp = SelectObject( hdc2, hBitmap );

   GetObject( hBitmap, sizeof(BITMAP), &bm);

   AlphaBlend( hdc,
               hb_parni(2),
               hb_parni(3),
               hb_parni(4),
               hb_parni(5),
               hdc2,
               hb_parni(7),
               hb_parni(8),
               bm.bmWidth - hb_parni(7),
               bm.bmHeight - hb_parni(8),
               bf );

   SelectObject( hdc2, hOldBmp );
   DeleteDC( hdc2 );

}

//----------------------------------------------------------------------------//

HB_FUNC( GETBITMAPFROMWND )
{
   HWND        hWnd = (HWND) hb_parnl(1);
   HDC         hdc  = GetDC( hWnd );
   HDC         hdc2;
   HBITMAP     hBitmap, hOldBmp;
   RECT        rt;
   BITMAPINFO  bmInfo;
   PDWORD      pdwBits;

   GetClientRect( hWnd, &rt );

   bmInfo.bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
   bmInfo.bmiHeader.biWidth         = rt.right - rt.left;
   bmInfo.bmiHeader.biHeight        = rt.bottom - rt.top;
   bmInfo.bmiHeader.biPlanes        = 1;
   bmInfo.bmiHeader.biBitCount      = 32;
   bmInfo.bmiHeader.biCompression   = BI_RGB;

   hBitmap = CreateDIBSection( hdc, &bmInfo, DIB_RGB_COLORS, (void*)&pdwBits, NULL, 0 );

   if( hBitmap )
   {
      int i, j;
      PDWORD p;

      hdc2    = CreateCompatibleDC( hdc );
      hOldBmp = SelectObject( hdc2, hBitmap );

      SendMessage( hWnd, WM_PRINT, (UINT) hdc2, (PRF_CLIENT | PRF_CHILDREN | PRF_OWNED) );

      SelectObject( hdc2, hOldBmp );
      DeleteDC( hdc2 );

      p = pdwBits;

      for ( i = 0 ; i < bmInfo.bmiHeader.biHeight ; i++ )
      {
         for ( j = 0 ; j < bmInfo.bmiHeader.biWidth ; j++ )
         {
            *p = *p | 0xFF000000;
            p++;
         }
      }

      hb_retnl( (long) hBitmap );

   }
   else
      hb_retnl(0);

   ReleaseDC( hWnd, hdc );
}
//----------------------------------------------------------------------------//

//////////////////////////
// th_TaskItemDoLayout( nHandle, hWnd, szText, hBitmap, lSpecial )
//////////

HB_FUNC( TH_TASKITEMDOLAYOUT )
{
   PTHEMEINFO pti  = (PTHEMEINFO) hb_parnl(1);
   HWND       hWnd = (HWND) hb_parnl(2);

   if( pti && hWnd )
   {
      LPSTR    lpString     = (LPSTR) hb_parc(3);
      int      iCount       = hb_parclen(3);
      HBITMAP  hBitmap      = (HBITMAP) hb_parnl(4);
      BOOL     bHTML        = hb_parl(6);
      int      nPlainStyle  = hb_parni(7);
      HDC      hdc          = GetDC( hWnd );
      PTPGROUP g            = ( hb_parl(5) ? &pti->SpecialGroup : &pti->NormalGroup );
      RECT     rt, rta;
	   RECT     rt1, rt2;
      int      iDeductW;
      int      iDeductH;
      BITMAP   bm;
      HFONT    hOldFont;
      BOOL     bTransparent = FALSE;

      if( bHTML )
         bTransparent = (g->Tasklist.Info.crBackground1 == 0 && g->Tasklist.Info.crBackground2 == 0);
      
      if( bTransparent )
      {
            bTransparent = (g->Tasklist.Link.crBackground1 == 0 && g->Tasklist.Link.crBackground2 == 0 &&
                            g->Tasklist.Link.crBackgroundHot1 == 0 && g->Tasklist.Link.crBackgroundHot2 == 0);
      }

      iDeductW  = g->Tasklist.Link.rcPadding.left + g->Tasklist.Link.rcPadding.right;
      iDeductW += g->Tasklist.Link.rcBorderThickness.left + g->Tasklist.Link.rcBorderThickness.right;

      iDeductH  = g->Tasklist.Link.rcPadding.top + g->Tasklist.Link.rcPadding.bottom;
      iDeductH += g->Tasklist.Link.rcBorderThickness.top + g->Tasklist.Link.rcBorderThickness.bottom;

      GetWindowRect( hWnd, &rt );

      SetRect( &rta, 0,
                     0,
                     rt.right - rt.left - iDeductW,
                     5 );
      if( hBitmap )
      {
         GetObject( hBitmap, sizeof(BITMAP), &bm);
         rta.left += bm.bmWidth;
         rta.left += g->Tasklist.Link.rcImagePadding.left + g->Tasklist.Link.rcImagePadding.right;

      }
	  
	  hb_xmemcpy( &rt1, &rta, sizeof(RECT) );
	  hb_xmemcpy( &rt2, &rta, sizeof(RECT) );

      hOldFont = SelectObject( hdc, g->Tasklist.hFontNormal );

      if( bHTML )
         DrawHTML( hdc, lpString, iCount, &rt1, (DT_LEFT | DT_WORDBREAK | DT_CALCRECT| DT_NOPREFIX));
      else
         DrawText( hdc, lpString, iCount, &rt1, (DT_LEFT | DT_WORDBREAK | DT_CALCRECT | DT_NOPREFIX));

      SelectObject( hdc, g->Tasklist.hFontHot );

      if( bHTML )
         DrawHTML( hdc, lpString, iCount, &rt2, (DT_LEFT | DT_WORDBREAK | DT_CALCRECT| DT_NOPREFIX));
      else
         DrawText( hdc, lpString, iCount, &rt2, (DT_LEFT | DT_WORDBREAK | DT_CALCRECT | DT_NOPREFIX));      

      SelectObject( hdc, hOldFont );
	  
	  if( (rt1.right > rt2.right && rt1.bottom == rt2.bottom) || rt1.bottom > rt2.bottom )
		 hb_xmemcpy( &rta, &rt1, sizeof(RECT) );
	  else
	     hb_xmemcpy( &rta, &rt2, sizeof(RECT) );
	  

      if( hBitmap )
      {
         bm.bmHeight += g->Tasklist.Link.rcImagePadding.top + g->Tasklist.Link.rcImagePadding.bottom;

         if( bm.bmHeight > (rta.bottom - rta.top + 1) )
            rta.bottom = rta.top + bm.bmHeight;
      }

      if( bTransparent || nPlainStyle != 0 )
         rta.right  += iDeductW;
      else
         rta.right    = rt.right - rt.left;

      rta.bottom += iDeductH;

      SetWindowPos( hWnd, 0, 0, 0, rta.right, rta.bottom, SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW );

      ReleaseDC( hWnd, hdc );
   }


}

//////////////////////////
// th_TPGroupHeaderHeight( tpi, hdc, szCaption, hBitmap, bSpecial, bCollasable, iWidth )
//////////

HB_FUNC( TH_TPGROUPHEADERHEIGHT )
{
   PTHEMEINFO pti = (PTHEMEINFO) hb_parnl(1);
   HDC        hdc = (HDC) hb_parnl(2);

   if( pti && hdc )
   {
      RECT     rt;
      PTPGROUP g              = ( hb_parl(5) ? &pti->SpecialGroup : &pti->NormalGroup );
      BITMAP   bm;
      HBITMAP  hBitmap        = (HBITMAP) hb_parnl(4);
      int      iHeaderHeight;

      SetRect( &rt, 0, 0, hb_parni(6), 0 );

      //////////////////////////
      // Button
      //////////

      if( g->Header.hBitmapArrowUp[0] )
      {
         GetObject( g->Header.hBitmapArrowUp[0], sizeof(BITMAP), &bm);

         iHeaderHeight = bm.bmHeight + g->Header.rcBorderThickness.top + g->Header.rcBorderThickness.bottom + g->Header.rcPadding.top + g->Header.rcPadding.bottom;
         rt.right     -= bm.bmWidth;
      }
      else
         iHeaderHeight = 23;

      //////////////////////////
      // Bitmap
      //////////

      if( hBitmap )
      {
         GetObject( hBitmap, sizeof(BITMAP), &bm);
         rt.right -= bm.bmWidth;
      }

      //////////////////////////
      // Caption
      //////////

      if( ISCHAR(3) )
      {

         HFONT  hOldFont;
         LPSTR  lpString  = (LPSTR) hb_parc(3);
         int    iLength   = hb_parclen(3);
         int    iHeight;

         //rcPaddingHeader
         rt.left  += g->Header.rcBorderThickness.left + g->Header.rcPadding.left + 1;
         rt.right  -= g->Header.rcBorderThickness.right + g->Header.rcPadding.right + 1;

         hOldFont    = SelectObject( hdc, g->Header.hFontNormal );

         iHeight  = DrawText( hdc, lpString, iLength, &rt, DT_LEFT | DT_NOPREFIX | DT_WORDBREAK | DT_CALCRECT );
         iHeight += g->Header.rcBorderThickness.top + g->Header.rcPadding.top + 1;
         iHeight += g->Header.rcBorderThickness.bottom + g->Header.rcPadding.bottom + 1;
         iHeight += 4;

         SelectObject( hdc, hOldFont );

         if( iHeight > iHeaderHeight )
            iHeaderHeight = iHeight;
      }

      hb_retni( iHeaderHeight );
   }
}

//----------------------------------------------------------------------------//

static void _i_DrawBorder( HDC hdc, RECT * r, RECT *b, COLORREF c, int iStyle )
{
   RECT rt;
   int Add2Left, Add2Top, Add2Right;

   //////////////////////////
   // Border
   //////////
   HBRUSH hBrush = CreateSolidBrush( c );

   Add2Left  =
   Add2Top   =
   Add2Right = 0;

   if( iStyle == 1 )
   {
      RECT rt2;

      if( b->left )
         Add2Left  = b->left + 1;

      if( b->top )
         Add2Top   = b->top + 1;

      if( b->right )
         Add2Right = b->right + 1;


      if( b->left && b->top )
      {
         rt2.left   = r->left +  1 ;
         rt2.top    = r->top + 1;
         rt2.right  = rt2.left + b->left;
         rt2.bottom = rt2.top + b->top;
         FillRect( hdc, &rt2, hBrush );
      }

      if( b->right && b->top )

      {
         rt2.right  = r->right - 1;
         rt2.top    = r->top +  1;
         rt2.left   = rt2.right - b->right;
         rt2.bottom = rt2.top + b->top;
         FillRect( hdc, &rt2, hBrush );
      }

   }

   //////////////////////////
   // Left Side
   //////////

   if( b->left )
   {
      rt        = *r;
      rt.right  = rt.left + b->left;
      rt.top    += Add2Top;
      FillRect( hdc, &rt, hBrush );
   }

   //////////////////////////
   // Top Side
   //////////

   if( b->top )
   {
      rt         = *r;
      rt.bottom  = rt.top + b->top;
      rt.left    += Add2Left;
      rt.right   -= Add2Right;
      FillRect( hdc, &rt, hBrush );
   }

   //////////////////////////
   // Right Side
   //////////

   if( b->right )
   {
      rt         = *r;
      rt.left    = rt.right - b->right;
      rt.top    += Add2Top;
      FillRect( hdc, &rt, hBrush );
   }

   //////////////////////////
   // Bottom Side
   //////////

   if( b->bottom )
   {
      rt         = *r;
      rt.top     = rt.bottom - b->bottom ;
      FillRect( hdc, &rt, hBrush );
   }

   DeleteObject( hBrush );

}

//////////////////////////
// th_DrawTaskPanel( tpi, hdc, left, top, right, bottom )
//////////

HB_FUNC( TH_DRAWTASKPANEL )
{
   PTHEMEINFO pti = (PTHEMEINFO) hb_parnl(1);
   HDC        hdc = (HDC) hb_parnl(2);

   if( pti && hdc )
   {
      RECT   rt, rtClient;
      RECT   rcBorderThickness;
      HBRUSH hBrush;
      int    nPlainStyle = hb_parni(7);
      int    nAnchor     = hb_parni(9);

      SetRect( &rt, hb_parnl(3),
                    hb_parnl(4),
                    hb_parnl(5),
                    hb_parnl(6) );

      rtClient = rt;

      if ( nPlainStyle == 2 )
      {
            hBrush = CreateSolidBrush( DarkenColor( 10, pti->NormalGroup.Tasklist.crBackground1 ) );
            FillRect( hdc, &rt, hBrush );
            DeleteObject( hBrush );
      }
      else
      {
         memcpy( &rcBorderThickness, &pti->TaskPanel.rcBorderThickness, sizeof(RECT) );

         if( nAnchor == 2 )
         {
            rcBorderThickness.left  = pti->TaskPanel.rcBorderThickness.right;
            rcBorderThickness.right = pti->TaskPanel.rcBorderThickness.left;
         }

         rtClient.left     += rcBorderThickness.left ;
         rtClient.top      += rcBorderThickness.top;
         rtClient.right    -= rcBorderThickness.right;
         rtClient.bottom   -= rcBorderThickness.bottom;

         if( pti->TaskPanel.iValue == 0  &&
             pti->TaskPanel.crBackground1 == 0 &&
             pti->TaskPanel.crBackground2 == 0 &&
             pti->TaskPanel.hBmpBackground )
         {
            DIBBMP db;

            DibBmp_Init( &db, pti->TaskPanel.hBmpBackground, FALSE );

            DibBmp_DrawStretch( &db,
                                hdc,
                                rtClient.left,
                                rtClient.top,
                                rtClient.right - rtClient.left + 1,
                                rtClient.bottom - rtClient.top + 1 );

         }
         //Panel de un solo Color
         else if( pti->TaskPanel.crBackground1 == pti->TaskPanel.crBackground2 )
         {
            hBrush = CreateSolidBrush( pti->TaskPanel.crBackground1 );
            FillRect( hdc, &rtClient, hBrush );
            DeleteObject( hBrush );
         }
         else
            DegradeFillX( hdc, &rtClient, pti->TaskPanel.crBackground1, pti->TaskPanel.crBackground2, ( pti->TaskPanel.iValue == 1 ? 1 : 0 ) );

         //////////////////////////
         // Border
         //////////

         _i_DrawBorder( hdc, &rt, &rcBorderThickness, pti->TaskPanel.crBorder, 0 );


         if( nPlainStyle == 1 )
         {
            HBRUSH hOldBrush;
            HPEN   hOldPen, hPen;
            int    nDiff = hb_parnl(4);

            rtClient.left     = hb_parvnl(8, 1);
            rtClient.top      = hb_parvnl(8, 2) + nDiff;
            rtClient.right    = hb_parvnl(8, 3);
            rtClient.bottom   = hb_parvnl(8, 4) + nDiff;


            hBrush = CreateSolidBrush( DarkenColor( 10, pti->NormalGroup.Tasklist.crBackground1 ) );
            hPen   = CreatePen( PS_SOLID, 1, pti->NormalGroup.Tasklist.crBorder );

            hOldBrush = SelectObject( hdc, hBrush );
            hOldPen   = SelectObject( hdc, hPen );

            RoundRect( hdc, rtClient.left, rtClient.top, rtClient.right, rtClient.bottom, 18, 18);

            SelectObject( hdc, hOldPen );
            SelectObject( hdc, hOldBrush );
            DeleteObject( hOldPen );
            DeleteObject( hOldBrush );
         }
      }
   }

}

//////////////////////////
// th_DrawTaskPanelGroup( tpi, hdc, lSpecial, left, top, right bottom , hWatermark, bHasAlpha, bHeaderVisible )
//////////

HB_FUNC( TH_DRAWTASKPANELGROUP )
{
   PTHEMEINFO pti = (PTHEMEINFO) hb_parnl(1);
   HDC        hdc = (HDC) hb_parnl(2);

   if( pti && hdc )
   {
      RECT     rt, rtClient;
      HBRUSH   hBrush;
      HBITMAP  hBitmap        = (HBITMAP) hb_parnl(8);
      BOOL     bHasAlpha      = hb_parl(9);
      BOOL     bHeaderVisible = hb_parl(10);
      int      nPlainStyle    = hb_parni(11);
      PTPGROUP g              = ( hb_parl(3) && nPlainStyle == 0 ? &pti->SpecialGroup : &pti->NormalGroup );


      SetRect( &rt, hb_parnl(4),
                    hb_parnl(5),
                    hb_parnl(6),
                    hb_parnl(7) );

      rtClient = rt;

      if( nPlainStyle == 0 )
      {
         RECT rtb;

         rtClient.left     += g->Tasklist.rcBorderThickness.left ;
         rtClient.top      += g->Tasklist.rcBorderThickness.top;
         rtClient.right    -= g->Tasklist.rcBorderThickness.right;
         rtClient.bottom   -= g->Tasklist.rcBorderThickness.bottom;

         //////////////////////////
         // Background
         //////////

         if( g->Tasklist.crBackground1 == g->Tasklist.crBackground2 )
         {
            hBrush = CreateSolidBrush( g->Tasklist.crBackground1 );
            FillRect( hdc, &rtClient, hBrush );
            DeleteObject( hBrush );
         }
         else
            DegradeFillX( hdc, &rtClient, g->Tasklist.crBackground1, g->Tasklist.crBackground2, ( g->Tasklist.iValue == 1 ? 1 : 0 ) );

         //////////////////////////
         // Border
         //////////

         rtb = g->Tasklist.rcBorderThickness;

         if( !bHeaderVisible && rtb.top == 0 )
            rtb.top = 1;

         _i_DrawBorder( hdc, &rt, &rtb, g->Tasklist.crBorder, 0 );

      }
      else
      {
         //////////////////////////
         // Background
         //////////

         hBrush = CreateSolidBrush( DarkenColor( 10 , g->Tasklist.crBackground1 ) );
         FillRect( hdc, &rtClient, hBrush );
         DeleteObject( hBrush );
      }

      //////////////////////////
      // WaterMark
      //////////

      if( hBitmap )
      {
         DIBBMP db;
         COLORREF crColor = g->Tasklist.crBackground1;

         if( g->Tasklist.crBackground1 != g->Tasklist.crBackground2 )
             crColor = g->Tasklist.crBackground2;

         DibBmp_Init2( &db, hBitmap, bHasAlpha );
         DibBmp_FilterWatermark( &db, crColor );
         DibBmp_DrawAtI( &db, hdc, rtClient.right, rtClient.bottom );
         DibBmp_Release( &db );
      }
   }
}

//////////////////////////
// th_DrawTaskPanelGroupSep( tpi, hdc, lSpecial, nSepStyle, width, top, nPlainStyle )
//////////

HB_FUNC( TH_DRAWTASKPANELGROUPSEP )
{
   PTHEMEINFO pti = (PTHEMEINFO) hb_parnl(1);
   HDC        hdc = (HDC) hb_parnl(2);

   if( pti && hdc )
   { 
      HPEN     hPen, hPen2,  hOldPen;
	  HBRUSH   hBrush, hOldBrush;
	  COLORREF nColor;
      int      nSepStyle      = hb_parnl(4); 
	  int      left           = 1;
      int      right          = hb_parnl(5) - 1;
      int      top            = hb_parnl(6);      
      int      nPlainStyle    = hb_parni(7);
      PTPGROUP g              = ( hb_parl(3) && nPlainStyle == 0 ? &pti->SpecialGroup : &pti->NormalGroup );
	  
	  if( nPlainStyle == 2 )
		nColor = DarkenColor( 70, pti->NormalGroup.Tasklist.crBackground1 );
	  else
	    nColor = g->Tasklist.crBorder;
		
      hPen    = CreatePen( PS_SOLID, 1, nColor );
	  hOldPen = SelectObject( hdc, hPen );
		
	  
	  if( nSepStyle == 3 || nSepStyle == 4 )
	  {
	     left += 9;
		 right -= 9;
	  }
	  else if( nSepStyle == 5 || nSepStyle == 6 )
	  {
	     left += (right * 0.80) ;		 
	  }
	  
	  MoveToEx( hdc, left, top, NULL );
	  LineTo( hdc, right, top );		
	  
	  hPen2   = CreatePen( PS_SOLID, 1, RGB(210,210,210)); 
	  SelectObject( hdc, hPen2 );

	  if( nSepStyle == 2 || nSepStyle == 4 || nSepStyle == 6)
	  {		
        MoveToEx( hdc, left, top+1, NULL );
        LineTo( hdc, right, top+1 );
	  }

      if( nSepStyle == 5 || nSepStyle == 6 )
      {
	      POINT pt[4];
		  int   w[1];
		  
		  hBrush = CreateSolidBrush( nColor );
		  SelectObject( hdc, hPen );
		  hOldBrush = SelectObject( hdc, hBrush );
		  
		  w[0] = 3 ;
		  
		  pt[0].x = right - 1 ;
		  pt[0].y = top - 2;
		  pt[1].x = right - 3;
		  pt[1].y = top;
		  pt[2].x = right -1 ;
		  pt[2].y = top + 2 ;
		  
		  PolyPolygon( hdc, pt, w, 1 );
		  SelectObject( hdc, hOldBrush );
		  DeleteObject( hBrush);
      }	  
	  
	  SelectObject( hdc, hOldPen );
	  DeleteObject( hPen );
	  DeleteObject( hPen2 );

   }
}



//////////////////////////
// th_DrawTaskPanelGroupHeader( pti, hdc, left, top, right, bottom, szName, hBitmap, hHasAlpha, bSpecial, bCollapsed, bFocused, bMouseFocused )
//////////

HB_FUNC( TH_DRAWTASKPANELGROUPHEADER )
{
   PTHEMEINFO pti = (PTHEMEINFO) hb_parnl(1);
   HDC        hdc = (HDC) hb_parnl(2);

   if( pti && hdc )
   {
      RECT     rt;
      DIBBMP   db;
      HDC      hdc2;
      BITMAP   bm;
      HBITMAP  hBitmap        = (HBITMAP) hb_parnl(8);
      BOOL     bHasAlpha      = hb_parl(9);
      BOOL     bCollapsed     = hb_parl(11);
      BOOL     bCollapsable   = hb_parl(12);
      BOOL     bFocused       = hb_parl(13);
      BOOL     bMouseFocused  = hb_parl(14);
      int      nPlainStyle    = hb_parni(15);
      PTPGROUP g              = ( hb_parl(10) && nPlainStyle == 0 ? &pti->SpecialGroup : &pti->NormalGroup );
      int      iHeaderHeight;

      SetRect( &rt, hb_parnl(3),
                    hb_parnl(4),
                    hb_parnl(5),
                    hb_parnl(6) );


      //////////////////////////
      // Background
      //////////

      hdc2    = CreateCompatibleDC( hdc );

      if( nPlainStyle == 0 )
      {
         if( g->Header.hBitmap && !bMouseFocused )
         {
            DibBmp_Init( &db, g->Header.hBitmap, TRUE );

            DibBmp_DrawStretch( &db,
                                hdc,
                                rt.left,
                                rt.top,
                                rt.right - rt.left,
                                rt.bottom - rt.top );
         }
         else if( g->Header.hBitmapHot && bMouseFocused )
         {
            DibBmp_Init( &db, g->Header.hBitmapHot, TRUE );

            DibBmp_DrawStretch( &db,
                                hdc,
                                rt.left,
                                rt.top,
                                rt.right - rt.left,
                                rt.bottom - rt.top );
         }
         else
         {
            COLORREF crBackground1;
            COLORREF crBackground2;
            int      iValue;

            if (!bMouseFocused)
            {
               crBackground1 = g->Header.crBackground1;
               crBackground2 = g->Header.crBackground2;
               iValue        = g->Header.iValue;
            }
            else
            {
               crBackground1 = g->Header.crBackgroundHot1;
               crBackground2 = g->Header.crBackgroundHot2;
               iValue        = g->Header.iValueHot;
            }
            
            if( crBackground1 != 0 && crBackground2 != 0 ) // Not Transparent
            {
               HBRUSH hBrush;
               RECT rtClient = rt;
   
               rtClient.left     += g->Header.rcBorderThickness.left ;
               rtClient.top      += g->Header.rcBorderThickness.top;
               rtClient.right    -= g->Header.rcBorderThickness.right;
               rtClient.bottom   -= g->Header.rcBorderThickness.bottom;
   
               if( crBackground1 == crBackground2 )
               {
                  //Fondo
                  hBrush = CreateSolidBrush( crBackground1 );
                  FillRect( hdc, &rtClient, hBrush );
                  DeleteObject( hBrush );
               }
               else
                  DegradeFillX( hdc, &rtClient, crBackground1, crBackground2, ( iValue == 1 ? 1 : 0 ) );
   
               _i_DrawBorder( hdc, &rt, &g->Header.rcBorderThickness, g->Header.crBorder, g->Header.iBorderStyle );
            }
         }
      }
      else if( nPlainStyle == 2 )
      {
         HPEN hOldPen, hPen;

         hPen = CreatePen( PS_SOLID, 1, DarkenColor( 70, pti->NormalGroup.Tasklist.crBackground1 ) );
         hOldPen = SelectObject( hdc, hPen );

         MoveToEx( hdc, rt.left, rt.bottom -1, NULL );
         LineTo( hdc, rt.right, rt.bottom -1);

         SelectObject( hdc, hOldPen );
         DeleteObject( hPen );
      }

      //////////////////////////
      // Focus Rect
      //////////

      if( bFocused )
      {
         RECT rta = rt;

         rta.left   += 2;
         rta.top    += 2;
         rta.right  -= 2;
         rta.bottom -= 1;

         DrawFocusRect( hdc, &rta );
      }

      //////////////////////////
      // Button
      //////////

      iHeaderHeight = (rt.bottom - rt.top) - 
                      (g->Header.rcBorderThickness.bottom + g->Header.rcBorderThickness.top + g->Header.rcPadding.bottom + g->Header.rcPadding.top );

                        //(rt.bottom - g->Header.rcBorderThickness.bottom - g->Header.rcPadding.bottom) - 
                     //(rt.top + g->Header.rcBorderThickness.top + g->Header.rcPadding.top);

      if( bCollapsable )
      {
         HBITMAP hButton;

         if( bCollapsed )
            hButton = g->Header.hBitmapArrowDn[ bMouseFocused ? 1 : 0 ];
         else
            hButton = g->Header.hBitmapArrowUp[ bMouseFocused ? 1 : 0 ];

         if( hButton )
         {
            POINT pt;

            GetObject( hButton, sizeof(BITMAP), &bm);

            pt.x = rt.right - bm.bmWidth - g->Header.rcBorderThickness.right - g->Header.rcPadding.right;
            pt.y = rt.top + g->Header.rcBorderThickness.top + g->Header.rcPadding.top;

            if( iHeaderHeight >= bm.bmHeight )
            {               
               if( iHeaderHeight >  bm.bmHeight )
                  pt.y += (iHeaderHeight - bm.bmHeight)/3;
             
               DibBmp_Init( &db, hButton, TRUE );
               DibBmp_DrawAt( &db, hdc, pt.x, pt.y );

            }

            rt.right -= bm.bmWidth;
         }
      }

      DeleteDC( hdc2 );

      //////////////////////////
      // Bitmap
      //////////

      if( hBitmap )
      {
         POINT pt;

         GetObject( hBitmap, sizeof(BITMAP), &bm);

         pt.y = rt.bottom - (bm.bmHeight + g->Header.rcPadding.bottom + g->Header.rcBorderThickness.bottom) + 1;
         pt.x = rt.left;

         if( iHeaderHeight >  bm.bmHeight )
            pt.y = rt.top + (iHeaderHeight - bm.bmHeight)/2;

         DibBmp_Init( &db, hBitmap, bHasAlpha );
         DibBmp_DrawAt( &db, hdc, pt.x, pt.y );

         rt.left += bm.bmWidth - g->Header.rcPadding.left;
      }

      //////////////////////////
      // Caption
      //////////

      if( ISCHAR(7) )
      {
         int iOldBkMode;
         HFONT hOldFont;
         COLORREF crOldColor;
         COLORREF crColor = (bMouseFocused ? g->Header.crHot : g->Header.crNormal);
         LPSTR  lpString  = (LPSTR) hb_parc(7);
         int    iLength   = hb_parclen(7);
         SIZE   size;
         UINT   uFormat = g->Header.uAlign | DT_NOPREFIX;

         if( nPlainStyle > 0 )
            crColor = 0;

         //rcPaddingHeader
         rt.top   += g->Header.rcBorderThickness.top + g->Header.rcPadding.top + 1;
         rt.bottom -= g->Header.rcBorderThickness.bottom + g->Header.rcPadding.bottom + 1;
         rt.left  += g->Header.rcBorderThickness.left + g->Header.rcPadding.left + 1;
         rt.right  -= g->Header.rcBorderThickness.right + g->Header.rcPadding.right + 1;

         hOldFont    = SelectObject( hdc, (bMouseFocused ? g->Header.hFontHot : g->Header.hFontNormal) );
         iOldBkMode  = SetBkMode( hdc, TRANSPARENT );
         crOldColor  = SetTextColor( hdc, crColor );

         GetTextExtentPoint32(hdc, lpString, iLength , &size);

         if( size.cx > (rt.right - rt.left) )
            uFormat |= DT_WORDBREAK;
         else
            uFormat |= (DT_VCENTER | DT_SINGLELINE);

         DrawText( hdc, lpString, iLength, &rt, g->Header.uAlign | uFormat );

         SetTextColor( hdc, crOldColor );
         SetBkMode( hdc, iOldBkMode );
         SelectObject( hdc, hOldFont );
      }
   }
}

//////////////////////////
// th_DrawTaskItemB( pti, hdc, left, top, right, bottom, bSpecial, bFocused, bMousefocused, bHtml, nPlainStyle )
//////////

HB_FUNC( TH_DRAWTASKITEMB )
{
   PTHEMEINFO pti = (PTHEMEINFO) hb_parnl(1);
   HDC        hdc = (HDC) hb_parnl(2);

   if( pti && hdc )
   {
      RECT     rt, rtClient;
      HBRUSH   hBrush;
      BOOL     bFocused       = hb_parl(8);
      BOOL     bMouseFocused  = hb_parl(9);
      BOOL     bHTML          = hb_parl(10);
      int      nPlainStyle    = hb_parni(11);
      PTPGROUP g              = ( hb_parl(7) && nPlainStyle == 0 ? &pti->SpecialGroup : &pti->NormalGroup );

      SetRect( &rt, hb_parnl(3),
                    hb_parnl(4),
                    hb_parnl(5),
                    hb_parnl(6) );


      //////////////////////////
      // Background
      //////////

      rtClient = rt;
      rtClient.left     += g->Tasklist.Link.rcBorderThickness.left ;
      rtClient.top      += g->Tasklist.Link.rcBorderThickness.top;
      rtClient.right    -= g->Tasklist.Link.rcBorderThickness.right;
      rtClient.bottom   -= g->Tasklist.Link.rcBorderThickness.bottom;


      if( ISNIL(12) && ISNIL(13) )
      {
         if(  nPlainStyle != 0 )
         {
            //Transparent
         }
         else if( bHTML && (g->Tasklist.Info.crBackground1 != 0 || g->Tasklist.Info.crBackground2 != 0 ) )
         {
            if( g->Tasklist.Info.crBackground1 == g->Tasklist.Info.crBackground2 )
            {
               hBrush = CreateSolidBrush( g->Tasklist.Info.crBackground1 );
               FillRect( hdc, &rtClient, hBrush );
               DeleteObject( hBrush );
            }
            else
               DegradeFillX( hdc, &rtClient, g->Tasklist.Info.crBackground1, g->Tasklist.Info.crBackground2, ( g->Tasklist.Info.iValue == 1 ? 1 : 0 ) );
         }
         else
         {
            COLORREF crBackground1;
            COLORREF crBackground2;
            int      iValue;

            if( !bMouseFocused )
            {
               crBackground1 = g->Tasklist.Link.crBackground1;
               crBackground2 = g->Tasklist.Link.crBackground2;
               iValue        = g->Tasklist.Link.iValue;
            }
            else
            {
               crBackground1 = g->Tasklist.Link.crBackgroundHot1;
               crBackground2 = g->Tasklist.Link.crBackgroundHot2;
               iValue        = g->Tasklist.Link.iValueHot;
            }

            if( crBackground1 == 0 && crBackground2 == 0 )
            {
               //Transparent
            }
            else
            {
               //Solid Color
               if( crBackground1 == crBackground2  )
               {
                  hBrush = CreateSolidBrush( crBackground1 );
                  FillRect( hdc, &rtClient, hBrush );
                  DeleteObject( hBrush );
               }
               else 
                  DegradeFillX( hdc, &rtClient, crBackground1, crBackground2, ( iValue == 1 ? 1 : 0 ) );
   
               _i_DrawBorder( hdc, &rt, &g->Tasklist.Link.rcBorderThickness, g->Tasklist.Link.crBorder, 0 );
            }
         }
      }
      else
      {
         if( bMouseFocused && !bHTML )
         {
            if( ISNUM(13) )
            {
               hBrush = CreateSolidBrush( hb_parnl(13) );
               FillRect( hdc, &rtClient, hBrush );
               DeleteObject( hBrush );
            }
            else if( ISARRAY(13) )
               DegradeFillX( hdc, &rtClient, hb_parvnl(13, 1), hb_parvnl(13, 2), ( hb_parvnl(13, 3) == 1 ? 1 : 0 ) );
         }
         else
         {
            if( ISNUM(12) )
            {
               hBrush = CreateSolidBrush( hb_parnl(12) );
               FillRect( hdc, &rtClient, hBrush );
               DeleteObject( hBrush );
            }
            else if( ISARRAY(12) )
               DegradeFillX( hdc, &rtClient, hb_parvnl(12, 1), hb_parvnl(12, 2), ( hb_parvnl(12, 3) == 1 ? 1 : 0 ) );
         }

         _i_DrawBorder( hdc, &rt, &g->Tasklist.Link.rcBorderThickness, g->Tasklist.Link.crBorder, 0 );
      }
      //////////////////////////
      // Focus Rect
      //////////
      if( bFocused )
         DrawFocusRect( hdc, &rt );

   }
}


//////////////////////////
// th_DrawTaskItem( pti, hdc, left, top, right, bottom, szName, hBitmap, hHasAlpha, bSpecial, bMousefocused, bEnabled )
//////////

HB_FUNC( TH_DRAWTASKITEM )
{
   PTHEMEINFO pti = (PTHEMEINFO) hb_parnl(1);
   HDC        hdc = (HDC) hb_parnl(2);

   if( pti && hdc )
   {
      RECT     rt;
      //HBRUSH   hBrush;
      HBITMAP  hBitmap        = (HBITMAP) hb_parnl(8);
      BOOL     bHasAlpha      = hb_parl(9);
      BOOL     bMouseFocused  = hb_parl(11);
      BOOL     bEnabled       = hb_parl(12);
      BOOL     bHTML          = hb_parl(13);
      int      nPlainStyle    = hb_parni(14);
      PTPGROUP g              = ( hb_parl(10) && nPlainStyle == 0 ? &pti->SpecialGroup : &pti->NormalGroup );

      SetRect( &rt, hb_parnl(3),
                    hb_parnl(4),
                    hb_parnl(5),
                    hb_parnl(6) );


      //////////////////////////
      // Bitmap
      //////////

      if( hBitmap )
      {
         DIBBMP db;
         BITMAP bm;
         int x = g->Tasklist.Link.rcImagePadding.left + g->Tasklist.Link.rcBorderThickness.left;
         int y = g->Tasklist.Link.rcImagePadding.top + g->Tasklist.Link.rcBorderThickness.top; 

         GetObject( hBitmap, sizeof(BITMAP), &bm);

         rt.left += bm.bmWidth + g->Tasklist.Link.rcImagePadding.left + g->Tasklist.Link.rcImagePadding.right;

         if( bEnabled )
         {
            DibBmp_Init( &db, hBitmap, bHasAlpha );
            DibBmp_DrawAt( &db, hdc, x, y );
         }
         else
         {
            DibBmp_Init2( &db, hBitmap, bHasAlpha );
            DibBmp_FilterGrayScale( &db );
            DibBmp_DrawAt( &db, hdc, x, y );
            DibBmp_Release( &db );
         }         
      }

      //////////////////////////
      // Caption
      //////////
      {
         int      iOldBkMode;
         HFONT    hFont;
         HFONT    hOldFont;
         COLORREF crOldColor;
         COLORREF crLink;

         hFont  = bMouseFocused ? g->Tasklist.hFontHot  : g->Tasklist.hFontNormal;

         if( bEnabled )
            crLink = bMouseFocused ? g->Tasklist.Link.crHot : g->Tasklist.Link.crNormal;
         else
            crLink  = GetSysColor( COLOR_GRAYTEXT );

         iOldBkMode = SetBkMode( hdc, TRANSPARENT );
         hOldFont   = SelectObject( hdc, hFont );

         if( bHTML)
            crOldColor = SetTextColor( hdc , g->Tasklist.Info.crNormal );
         else
            crOldColor = SetTextColor( hdc , crLink );

         rt.left   += g->Tasklist.Link.rcPadding.left + g->Tasklist.Link.rcBorderThickness.left;
         rt.right  -= g->Tasklist.Link.rcPadding.right + g->Tasklist.Link.rcBorderThickness.right;
         rt.top    += g->Tasklist.Link.rcPadding.top + g->Tasklist.Link.rcBorderThickness.top;
         rt.bottom -= g->Tasklist.Link.rcPadding.bottom + + g->Tasklist.Link.rcBorderThickness.bottom;

         if( bHTML )
            DrawHTML( hdc, hb_parc(7), hb_parclen(7), &rt, (DT_LEFT | DT_WORDBREAK | DT_NOPREFIX) );
         else
         {
            int iHeight  = rt.bottom - rt.top;
            int iHeight2 = DrawText( hdc, hb_parc(7), hb_parclen(7), &rt, (DT_LEFT | DT_WORDBREAK | DT_NOPREFIX | DT_CALCRECT) );

            if( iHeight2 < iHeight )
            {
               int iDiff = (iHeight - iHeight2) / 2;

               rt.top    += iDiff;
               rt.bottom += iDiff;
            }

            DrawText( hdc, hb_parc(7), hb_parclen(7), &rt, (DT_LEFT | DT_WORDBREAK | DT_NOPREFIX) );
         }

         SelectObject( hdc, hOldFont );
         SetTextColor( hdc, crOldColor );
         SetBkMode( hdc, iOldBkMode );
      }

   }
}

//----------------------------------------------------------------------------//

HB_FUNC( TH_GETTASKGROUPBACKGROUND )
{
   PTHEMEINFO pti = (PTHEMEINFO) hb_parnl(1);

   if( pti  )
   {
      int      nPlainStyle    = hb_parni(3);
      PTPGROUP g              = ( hb_parl(2) && nPlainStyle == 0 ? &pti->SpecialGroup : &pti->NormalGroup );

      if( nPlainStyle == 0 )
         hb_retnl( (long) g->Tasklist.crBackground1 );
      else
         hb_retnl( (long) DarkenColor( 10 , g->Tasklist.crBackground1 ) );
   }
}

//----------------------------------------------------------------------------//

HB_FUNC( CALCANIMATIONHEIGHTS )
{
   int     iWidth   = hb_parni(1);
   int     iHeight   = hb_parni(2);
   HBITMAP hBitmap   = CreateBitmap( iWidth, iHeight, 1, 1, NULL );
   HDC     hdc       = CreateCompatibleDC( NULL );
   HBITMAP hOldBmp;
   RECT    rt;
   POINT   pt[4];
   int     i, j;

   hOldBmp = SelectObject( hdc, hBitmap );

   rt.top    = 0;
   rt.left   = 0;
   rt.right  = iWidth;
   rt.bottom = iHeight;

   FillRect( hdc, &rt, GetStockObject( WHITE_BRUSH ) );

   pt[0].x = 0;
   pt[0].y = iHeight - 1;
   pt[1].x = iWidth / 4 * 3;
   pt[1].y = iHeight / 4 * 3;
   pt[2].x = iWidth / 4;
   pt[2].y = iHeight / 4;
   pt[3].x = iWidth - 1;
   pt[3].y = 0;

   PolyBezier( hdc, pt, 4 );

   hb_reta( iWidth );
   //Extract Heights
   for( i = 0; i < iWidth; i++ )
   {
      for( j = iHeight - 1; j > 0; j-- )
      {
         if( GetPixel(hdc, i, j ) == 0 )
            break;
      }
      hb_storvni( j, -1, i+1 );
   }

   SelectObject( hdc, hOldBmp );
   DeleteDC( hdc );
   DeleteObject( hBitmap );
}

//----------------------------------------------------------------------------//

HB_FUNC( PAINTWINDOW )
{
   HWND hWnd = (HWND) hb_parnl(1);

   InvalidateRect( hWnd, NULL, 1 );

   if( hb_parl(2) )
      SetWindowPos( hWnd, NULL, NULL, NULL, NULL, NULL,  SWP_FRAMECHANGED|SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER );
   else
      UpdateWindow( hWnd );
}
//----------------------------------------------------------------------------//

HB_FUNC( WNDADJLEFT2 ) // ( hWnd, hTop, hBottom )
{
   HWND hControl = (HWND) hb_parnl( 1 );
   HWND hWnd;
   RECT rct, rctCtrl;
   WORD wTopHeight = 0, wBottomHeight = 0;

   if( (hWnd = ( HWND ) hb_parnl( 2 )) != NULL )
   {
      GetWindowRect( hWnd, &rct );
      wTopHeight = (WORD) (rct.bottom - rct.top);
   }

   if( (hWnd = ( HWND ) hb_parnl( 3 )) != NULL )
   {
      GetWindowRect( hWnd, &rct );
      wBottomHeight = (WORD) (rct.bottom - rct.top);
   }

   GetClientRect( GetParent( hControl ), &rct );
   GetWindowRect( hControl, &rctCtrl );

   MoveWindow( hControl, 0, wTopHeight,
               rctCtrl.right - rctCtrl.left,
               rct.bottom - rct.top
               - wTopHeight - wBottomHeight,
               TRUE );
}
//----------------------------------------------------------------------------//

HB_FUNC( WNDADJRIGHT2 ) // ( hWnd, hTop, hBottom )
{
   HWND hControl = ( HWND ) hb_parnl( 1 );
   HWND hWnd;
   RECT rct, rctCtrl;
   WORD wTopHeight = 0, wBottomHeight = 0;

   if( (hWnd = ( HWND ) hb_parnl( 2 )) != NULL )
   {
      GetWindowRect( hWnd, &rct );
      wTopHeight = (WORD) (rct.bottom - rct.top);
   }

   if( (hWnd = ( HWND ) hb_parnl( 3 )) != NULL )
   {
      GetWindowRect( hWnd, &rct );
      wBottomHeight = (WORD) (rct.bottom - rct.top);
   }

   GetClientRect( GetParent( hControl ), &rct );
   GetWindowRect( hControl, &rctCtrl );

   MoveWindow( hControl, rct.right - ( rctCtrl.right - rctCtrl.left ),
               wTopHeight,
               rctCtrl.right - rctCtrl.left,
               rct.bottom - rct.top
               - wTopHeight - wBottomHeight,
               TRUE );
}

//----------------------------------------------------------------------------//

HB_FUNC( CREATECOMPATIBLEBITMAP ) // hDC, nWidth, nHeight
{
   hb_retnl( ( LONG ) CreateCompatibleBitmap( ( HDC ) hb_parnl( 1 ), hb_parnl( 2 ), hb_parnl( 3 ) ) );
}

//----------------------------------------------------------------------------//
