/**************************************************************************************************
* Project      : TTaskpanel
* File         : c_DibBmp.h
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

#ifndef _DIBBMP_H_
#define _DIBBMP_H_

#define WINVER               0x0501

#include <windows.h>

typedef struct
{
    HBITMAP   hBitmap;            //Bitmap Handle
    WORD      wBitCount;          //Original Bits per pixel
    WORD      wClrUsed;
    PDWORD    ColorTable;         //Color Table Only for 1, 4 , 8 bits Bitmaps
    BOOL      bHasAlpha;          //Is Alpha Channel Present

} DIBBMP, *PDIBBMP;

#ifdef __cplusplus
extern "C" {
#endif

void DibBmp_Init( PDIBBMP db, HBITMAP hBitmap, BOOL bHasAlpha );
void DibBmp_Init2( PDIBBMP db, HBITMAP hBitmap, BOOL bHasAlpha );
BOOL DibBmp_LoadFromResource( PDIBBMP db, HINSTANCE hInstance, const char * szResName );
BOOL DibBmp_LoadFromFile( PDIBBMP db,  const char * szFileName );
BOOL DibBmp_Duplicate( PDIBBMP db, HBITMAP hBitmap );
BOOL DibBmp_CreateFromMem( PDIBBMP db, void * pvMem, ULONG ulSize );
BOOL DibBmp_SetTransparentColor( PDIBBMP db, DWORD dwTransColor );
BOOL DibBmp_SetTransparentColorIndex( PDIBBMP db, BYTE byIndex );
BOOL DibBmp_SetTransparentColorFirstPixel( PDIBBMP db  );
BOOL DibBmp_SetTransparentColor2( PDIBBMP db, DWORD dwTransColor, WORD wType  );
BOOL DibBmp_FilterGrayScale( PDIBBMP db );
BOOL DibBmp_FilterWatermark( PDIBBMP db, DWORD dwColorTone );
BOOL DibBmp_PreMultiplyAlpha( PDIBBMP db );
BOOL DibBmp_Release( PDIBBMP db );
BOOL DibBmp_Release2( PDIBBMP db );


BOOL DibBmp_DrawAt( PDIBBMP db, HDC hdc, int x, int y );
BOOL DibBmp_DrawAtI( PDIBBMP db, HDC hdc, int x, int y );
BOOL DibBmp_DrawAt2( PDIBBMP db, HDC hdc, int x, int y, BOOL bInverse );

BOOL DibBmp_DrawStretch( PDIBBMP db, HDC hdc, int x, int y , int iWidth, int iHeight );

#ifdef __cplusplus
}
#endif


#define _FILTER_GRAYSCALE
#define _FILTER_WATERMARK

#endif
