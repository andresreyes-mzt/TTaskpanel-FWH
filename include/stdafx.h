/**************************************************************************************************
* Project      : TTaskpanel
* File         : stdafx.h
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

#ifndef __STDAFX_H__
#define __STADFX_H__

#define WINVER               0x0501
#define _WIN32_IE            0x0600
#define _WIN32_WINNT         0x0501

#define HB_OS_WIN_32_USED

#include <windows.h>
#include <windowsx.h>

#include <hbapi.h>
#include <hbapiitm.h>
#include <hbapifs.h>
#include <hbvm.h>
#include <hbstack.h>
#include <commctrl.h>

#include "DibBmp.h"


#define MANIFEST_RESOURCE_ID            1

#define XCHGCOLORREF(_x)              ((((_x)&0xFF00FF00L)|(((_x)&0x00FF0000L)>>16)|(((_x)&0x000000FFL)<<16)))

//////////////////////////
// TASKLIST & HEADER INFORMATIO
//////////

typedef struct
{
   struct
   {
      COLORREF    crBackground1;
      COLORREF    crBackground2;
      int         iValue;

      COLORREF    crBackgroundHot1;
      COLORREF    crBackgroundHot2;
      int         iValueHot;

      COLORREF    crBorder;
      COLORREF    crNormal;               //Normal Color
      COLORREF    crHot;                  //Hot Color (mousefocused)

      LOGFONT     lf;
      LOGFONT     lfHot;
      HFONT       hFontNormal;            //Font handle
      HFONT       hFontHot;               //Font handle
      UINT        uAlign;                 //TextAlignment

      int         iMargin;                // margin (panel to bar)
      int         iBorderStyle;

      RECT        rcPadding;
      RECT        rcBorderThickness;

      HBITMAP     hBitmap;
      HBITMAP     hBitmapHot;

      HBITMAP     hBitmapArrowUp[2];
      HBITMAP     hBitmapArrowDn[2];
	  
	   char       *szBitmapName[6];

   } Header;

   struct
   {
      COLORREF    crBackground1;
      COLORREF    crBackground2;
      int         iValue;
      COLORREF    crBorder;

      LOGFONT     lfNormal;
      LOGFONT     lfHot;
      HFONT       hFontNormal;            //Normal Font handle
      HFONT       hFontHot;               //Hot Font handle (mousefocussed)

      int         iMargin;

      RECT        rcBorderThickness;
      RECT        rcPadding;

      struct
      {
         COLORREF crBackground1;
         COLORREF crBackground2;
         int      iValue;

         COLORREF crBackgroundHot1;
         COLORREF crBackgroundHot2;
         int      iValueHot;

         COLORREF crBorder;
         COLORREF crNormal;
         COLORREF crHot;

         RECT        rcBorderThickness;
         RECT        rcPadding;
         RECT        rcImagePadding;
      } Link;

      struct
      {
         COLORREF crBackground1;
         COLORREF crBackground2;
         int      iValue;
         COLORREF crNormal;
      } Info;

   } Tasklist;

} TPGROUP, *PTPGROUP;

//////////////////////////
// TASKPANEL INFORMATION
//////////

typedef struct
{
   char     szShellStyleDllName[MAX_PATH];

   struct
   {
      COLORREF    crBackground1;          //Gradient Background Color 1
      COLORREF    crBackground2;          //Gradient Background Color 1
      int         iValue;                 //Gradient Direction

      HBITMAP     hBmpBackground;         //Background Bitmap
      RECT        rcPadding;              //Inner Padding
      RECT        rcBorderThickness;      //Border Thickness
      COLORREF    crBorder;               //Border Color

   } TaskPanel;

   TPGROUP  NormalGroup;
   TPGROUP  SpecialGroup;

} THEMEINFO, *PTHEMEINFO;

//////////////////////////
// Theme Support
//////////

typedef BOOL (* PINITTHEME)( PTHEMEINFO pti);

BOOL RegisterTHEMEX( char * cThemeName, PINITTHEME p );

//////////////////////////
// BUFFER
//////////

typedef struct
{
   char     *szBuffer;
   WORD     wSize;
   WORD     wPos;

} BUFFER, *PBUFFER;

//////////////////////////
// TOKEN
//////////

#define MAX_TOKEN                      20

typedef struct
{
   char    *szBuffer;
   char    *szToken[MAX_TOKEN];
   WORD     wSize;
   WORD     wPos;

} TOKEN, *PTOKEN;

//////////////////////////
// Auxiliar Functions
//////////

BOOL GetHTMLColor( const char * szColor, COLORREF * crColor );
BOOL GetColorFromName( const char * szColor, COLORREF * crColor );
BOOL GetAlignFromName( const char * szAlign, UINT * uAlign );
//////////////////////////
// DEBUG
//////////

#ifdef DEBUG

   #define OUTDBG       OutputDebug

   #define DBG( x )     OutputDebug( x )

   #define DBGS( x ) \
   { \
      OutputDebugString( #x ); \
      OutputDebug( " --> %s\n", x );\
   }

   #define DBGN( x ) \
   { \
      OutputDebugString( #x ); \
      OutputDebug( " --> %d\n", x );\
   }

   #define DBGX( x ) \
   { \
      OutputDebugString( #x ); \
      OutputDebug( " --> %x\n", x );\
   }

   #define DBGWM() \
   { \
      OutputDebug( "WM --> %x, %x, %x \n", msg, wParam, lParam ); \
   }

#else
   #define OUTDBG
   #define DBG( x )
   #define DBGS( x )  x
   #define DBGN( x )  x
   #define DBGX( x )  x
#endif

void  OutputDebug(const char *format, ...);

//////////////////////////
// Object Api
//////////

long Object_GetNL( PHB_ITEM pSelf, char *varname );
BOOL Object_GetL( PHB_ITEM pSelf, char *varname );

//////////////////////////
// Theme Aux
//////////

COLORREF DarkenColor(long lScale, COLORREF lColor);
int      Pt2FontHeight ( int iPt );
HBITMAP  CreateMemDIBitmapT( void * lpBits, ULONG ulSize, DWORD dwTransColor );
HBITMAP  LoadBitmapEx( HINSTANCE hInstance, const char * szName, BOOL bTransparent );



//////////////////////////
// DrawHTML
//////////

int DrawHTML( HDC hdc, LPCTSTR lpString, int nCount, LPRECT lpRect, UINT uFormat );

//////////////////////////
// xharbour/harbour support
//////////

#ifdef __XHARBOUR__

#define hb_parvc        hb_parc
#define hb_parvni       hb_parni
#define hb_parvnl       hb_parnl

#define hb_storvc       hb_storc
#define hb_storvni      hb_storni
#define hb_storvnl      hb_stornl
#define hb_storvl       hb_storl

#endif

#endif
