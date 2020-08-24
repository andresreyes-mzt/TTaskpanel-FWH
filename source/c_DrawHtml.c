/**************************************************************************************************
* Project      : TTaskpanel
* File         : c_DrawHTML.c
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

#define GTOKEN_NONE              0x000
#define GTOKEN_TEXT              0x001  // 00000000001
#define GTOKEN_BOLD              0x002  // 00000000010
#define GTOKEN_ITALIC            0x004  // 00000000100
#define GTOKEN_UNDERLINE         0x008  // 00000001000
#define GTOKEN_FONT              0x010  // 00000010000
#define GTOKEN_P                 0x100  // 00100000000
#define GTOKEN_BR                0x200  // 01000000000
#define GTOKEN_ENDTAG            0x400  // 10000000000

#define FSTYLE_BOLD              0x001  // 0001
#define FSTYLE_ITALIC            0x002  // 0010
#define FSTYLE_UNDERLINE         0x004  // 0100

typedef struct
{
   char * szStr;
   char   szChar;
} REPLACELST, * PREPLACELST;

static REPLACELST Entities[] =
{
   { "&qout;" , '"' },
   { "&amp;"  , '&' },
   { "&lt;"   , '<' },
   { "&gt;"   , '>' },
   { "&nbsp;" , ' ' },
   { NULL     ,  0  }
};

typedef struct
{
   char * szTag;
   int    iToken;
   BOOL   bBlock;
} TOKENS, PTOKENS;

static TOKENS tokens[] =
{
   { "<b>"       , GTOKEN_BOLD                       , FALSE },
   { "</b>"      , GTOKEN_BOLD | GTOKEN_ENDTAG       , FALSE },
   { "<br "      , GTOKEN_BR                         , TRUE  },
   { "<br>"      , GTOKEN_BR                         , TRUE  },
   { "</br>"     , GTOKEN_BR | GTOKEN_ENDTAG         , TRUE  },
   { "<em>"      , GTOKEN_ITALIC                     , FALSE },
   { "</em>"     , GTOKEN_ITALIC | GTOKEN_ENDTAG     , FALSE },
   { "<font "    , GTOKEN_FONT                       , FALSE },
   { "</font>"   , GTOKEN_FONT | GTOKEN_ENDTAG       , FALSE },
   { "<i>"       , GTOKEN_ITALIC                     , FALSE },
   { "</i>"      , GTOKEN_ITALIC | GTOKEN_ENDTAG     , FALSE },
   { "<p>"       , GTOKEN_P                          , TRUE  },
   { "</p>"      , GTOKEN_P | GTOKEN_ENDTAG          , TRUE  },
   { "<strong>"  , GTOKEN_BOLD                       , FALSE },
   { "</strong>" , GTOKEN_BOLD | GTOKEN_ENDTAG       , FALSE },
   { "<u>"       , GTOKEN_UNDERLINE                  , FALSE },
   { "</u>"      , GTOKEN_UNDERLINE | GTOKEN_ENDTAG  , FALSE },
   { NULL        , GTOKEN_NONE                       , FALSE }

};

//----------------------------------------------------------------------------//

static int GetToken( LPSTR *lpString, int * iSize, int * iTokenLength, BOOL * bWhiteSpace )
{
   LPSTR lpStart = *lpString;
   LPSTR lpEnd;
   int     iLength;
   int     iToken = GTOKEN_NONE;
   BOOL    bEntryWhiteSpace;

   bEntryWhiteSpace = *bWhiteSpace;
   *bWhiteSpace = bEntryWhiteSpace || isspace( *lpStart );

   //Nos Brincamos los espacios en blanco
   while( *iSize > 0 && isspace( *lpStart ) )
   {
       lpStart++;
       *iSize -= 1;
   }

   if( *iSize <= 0 )
       return iToken;

   lpEnd   = lpStart;
   iLength = 0;

   if( lpEnd[0] == '<' )
   {
       BOOL bSkipWhiteSpace = FALSE;
       int  i = 0;
       int  iLen;

       while( tokens[i].szTag != NULL )
       {
           iLen = strlen( tokens[i].szTag );

           if( strnicmp( lpEnd, tokens[i].szTag, iLen ) == 0 )
           {
               iLength += iLen;
               lpEnd   += iLen;


               if( tokens[i].szTag[ iLen - 1 ] == ' ' ) // Accept Parameters
               {
                   while( iLength < *iSize && lpEnd[0] != '>' )
                   {
                       iLength ++;
                       lpEnd++;
                   }

                   if( lpEnd[0] == '>' )
                   {
                     iLength++;
                     lpEnd++;
                   }
               }

               iToken          = tokens[i].iToken;
               bSkipWhiteSpace = tokens[i].bBlock;
               break;
           }
           i++;
       }

       if( iToken != GTOKEN_NONE && (bSkipWhiteSpace || bEntryWhiteSpace) )
       {
           //Nos Brincamos los espacios en blanco
           while( iLength < *iSize && isspace( *lpEnd ) )
           {
               lpEnd   ++;
               iLength ++;
           }

       }

       if( iToken == GTOKEN_NONE )
       {
         lpEnd++;
         iLength++;
       }
   }

   //Si no es un tag entonces es Texto
   if( iToken == GTOKEN_NONE )
   {
       iToken = GTOKEN_TEXT;

       while( iLength < *iSize && !isspace( *lpEnd ) && lpEnd[0] != '<' )
       {
           lpEnd++;
           iLength++;
       }
   }

   *iTokenLength = iLength;
   *iSize        -= iLength;
   *lpString     = lpStart;

   return iToken;
}

//----------------------------------------------------------------------------//

static LPSTR ReplaceEntities( LPSTR lpString, int *iSize )
{
   LPSTR lpStart  = lpString;
   LPTSTR lpBuffer = (LPTSTR) hb_xgrab( *iSize + 1 );
   LPTSTR p        = lpBuffer;
   int    iLength  = 0;
   int    n        = *iSize;
   BOOL   bCopy;

   if( !lpBuffer || !lpString )
      return NULL;

   while( n-- )
   {
      bCopy = TRUE;

      if( lpStart[0] == '&' )
      {
         int  i      = 0;

         while( Entities[i].szStr != NULL )
         {
            int iLen = strlen( Entities[i].szStr );

            if( strnicmp( lpStart, Entities[i].szStr, iLen ) == 0 )
            {
               lpStart += iLen;
               n       -= (iLen - 1);
               *p++     = Entities[i].szChar;
               bCopy    = FALSE;
               break;
            }
            i++;
         }
      }

      if( bCopy )
         *p++ = *lpStart++;

      iLength++;
   }

   *p     = 0;
   *iSize = iLength;

   return lpBuffer;
}

typedef struct __colorx COLORX, *PCOLORX;

struct __colorx
{
   COLORREF crColor;
   PCOLORX  pNext;
};

static PCOLORX s_pColor = NULL;

static void PushColor( HDC hdc, COLORREF crColor )
{
   PCOLORX pColor = hb_xgrab( sizeof(COLORX) );

   pColor->crColor = GetTextColor( hdc );
   pColor->pNext   = s_pColor;

   s_pColor = pColor;

   SetTextColor( hdc, crColor );
}

static void PopColor( HDC hdc )
{
   PCOLORX pColor;

   if( s_pColor )
   {
      pColor   = s_pColor;
      s_pColor = s_pColor->pNext;

      SetTextColor( hdc, pColor->crColor );

      hb_xfree( pColor );
   }
}

static void FreeColors( void )
{
   PCOLORX pNext = s_pColor;

   while( pNext )
   {
      s_pColor = pNext;
      pNext    = pNext->pNext;

      hb_xfree( s_pColor );
   }
}

//----------------------------------------------------------------------------//

static LPSTR GetProperty( LPSTR lpString, int iLength , LPSTR lpProperty )
{
   LPSTR lpEnd  = lpString;
   int   i      = iLength;
   int   iLen   = strlen( lpProperty );

   while( --i )
   {
      if( strnicmp( lpEnd, lpProperty, iLen ) == 0 )
      {
         LPSTR lpStart;
         int   iCount;

         lpEnd  += iLen;

         //Eliminamos espacios previos
         while( isspace(*lpEnd) && i )
         {
            lpEnd++;
            i--;
         }

         lpStart = lpEnd;

         while( !isspace(*lpEnd) && lpEnd[0] != '>' && i )
         {
            lpEnd++;
            i--;
         }

         iCount = lpEnd - lpStart;

         if( iCount > 0 )
         {
            lpEnd = hb_xgrab( iCount + 1 );

            strncpy( lpEnd, lpStart, iCount );
            lpEnd[iCount] = 0;

            return lpEnd;
         }
         else
            return NULL;
      }

      lpEnd++;
   }

   return NULL;
}
//----------------------------------------------------------------------------//

int DrawHTML( HDC hdc, LPCTSTR lpString, int nCount, LPRECT lpRect, UINT uFormat )
{
   int          iSaveDC;
   HFONT        hOldFont;
   SIZE         size;

   int          iTop;
   int          iLeft;
   int          iHeight;
   int          iMaxHeight;
   int          iMaxWidth;
   int          iMinWidth;
   int          iLineHeight;
   HFONT        hFontBase;
   HFONT        hPrevFontBase = NULL;
   LOGFONT      lf;
   int          iFontStyle = 0;
   int          iCurrentFontStyle = 0;

   LPSTR      lpStart;
   int          iToken;
   int          iTokenLength;
   BOOL         bWhiteSpace = FALSE;
   BOOL         bWSBeforeTag = FALSE;

   RECT         rc;
   int          iPos;
   int          iWidth;
   int          iWidthOfSpace;



   if( hdc == NULL || lpString == NULL || lpRect == NULL )
       return 0;

   if( nCount < 0 )
       nCount = strlen( lpString );


   iTop         = lpRect->top;
   iLeft        = lpRect->left;
   iMaxWidth    = lpRect->right - lpRect->left;
   iMaxHeight   = lpRect->bottom - lpRect->top;

   iSaveDC = SaveDC( hdc );

   //Obtenemos la fuente por defecto del DC
   hOldFont  = (HFONT) SelectObject( hdc, GetStockObject(SYSTEM_FONT) );
   GetObject( hOldFont, sizeof(LOGFONT), &lf );
   hFontBase = CreateFontIndirect( &lf );
   SelectObject( hdc, hFontBase );

   //Obtenemos el Alto de linea
   GetTextExtentPoint32(hdc, "Áy", 2, &size);
   iLineHeight = size.cy;

   GetTextExtentPoint32(hdc, " ", 1, &size);
   iWidthOfSpace = size.cx;

   uFormat &= ~(DT_CENTER | DT_RIGHT | DT_TABSTOP);
   uFormat |= (DT_LEFT | DT_NOPREFIX);

   lpStart   = (LPSTR)lpString;
   iPos      = 0;
   iHeight   = 0;
   iWidth    = iMaxWidth;
   iMinWidth = 0;

   while(1)
   {
      iToken = GetToken( &lpStart, &nCount, &iTokenLength, &bWhiteSpace );

      if( iToken == GTOKEN_NONE )
         break;

      switch( iToken &~ GTOKEN_ENDTAG )
      {
         case GTOKEN_BOLD:
            iFontStyle = (iToken & GTOKEN_ENDTAG) ? iFontStyle &~ FSTYLE_BOLD : iFontStyle | FSTYLE_BOLD;
            break;

         case GTOKEN_ITALIC:
            iFontStyle = (iToken & GTOKEN_ENDTAG) ? iFontStyle &~ FSTYLE_ITALIC : iFontStyle | FSTYLE_ITALIC;
            break;

         case GTOKEN_UNDERLINE:
            iFontStyle = (iToken & GTOKEN_ENDTAG) ? iFontStyle &~ FSTYLE_UNDERLINE : iFontStyle | FSTYLE_UNDERLINE;
            break;

         case GTOKEN_FONT:
            {
               if( iToken & GTOKEN_ENDTAG )
                  PopColor( hdc );
               else
               {
                  LPSTR lpValue = GetProperty( lpStart + 6, iTokenLength - 6 , "color=" );

                  if( lpValue )
                  {
                     COLORREF crColor = 0;

                     if( !GetHTMLColor( lpValue, &crColor ) )
                        GetColorFromName( lpValue, &crColor );

                     PushColor( hdc, crColor );
                     hb_xfree( lpValue );
                  }
               }
               break;
            }
         case GTOKEN_BR:
            if( (iToken & GTOKEN_ENDTAG) == 0 && (uFormat & DT_SINGLELINE ) == 0 )
            {
               int iLineHeight0 = iLineHeight;

               if( lpStart[3] == ' ' )
               {
                  LPSTR lpValue = GetProperty( lpStart + 4, iTokenLength - 4 , "size=" );

                  if( lpValue )
                  {
                     iLineHeight0 = atoi( lpValue );
                     hb_xfree( lpValue );
                  }
               }


               iHeight += iLineHeight0;
               iPos    = 0;
            }
            break;

          case GTOKEN_P:
            if( (iToken & GTOKEN_ENDTAG) == 0 && (uFormat & DT_SINGLELINE ) == 0 )
            {
               if( lpStart != lpString )
               {
                  iHeight += 3 * (iLineHeight / 2);
                  iPos    = 0;
               }
            }
            break;

          case GTOKEN_TEXT:
            {
               LPSTR lpBuffer;
               int     iBufferLength = iTokenLength;

               if( iCurrentFontStyle != iFontStyle )
               {
                  HFONT hNewFont;

                  lf.lfWeight    = ( iFontStyle & FSTYLE_BOLD ) ? FW_BOLD : FW_NORMAL;
                  lf.lfItalic    = (BYTE) (( iFontStyle & FSTYLE_ITALIC ) ? 1 : 0);
                  lf.lfUnderline = (BYTE) (( iFontStyle & FSTYLE_UNDERLINE ) ? 1 : 0);

                  hNewFont  = CreateFontIndirect( &lf );
                  SelectObject( hdc, hNewFont );

                  if( hPrevFontBase )
                     DeleteObject( hPrevFontBase );

                  hPrevFontBase   = hFontBase;
                  hFontBase       = hNewFont;

                  GetTextExtentPoint32(hdc, " ", 1, &size);
                  iWidthOfSpace = size.cx;

                  iCurrentFontStyle = iFontStyle;
               }

               lpBuffer = ReplaceEntities( lpStart, &iBufferLength );

               //Checamos el ancho
               GetTextExtentPoint32(hdc, lpBuffer, iBufferLength , &size);

               if( bWhiteSpace )
                  iPos += iWidthOfSpace;

               if( (iPos + size.cx) > iWidth && bWhiteSpace )
               {
                  if( (uFormat & DT_WORDBREAK) != 0 )
                  {
                     iHeight += iLineHeight;
                     iPos     = 0;
                  }
               }

               if( (iHeight + iLineHeight) <= iMaxHeight && (uFormat & DT_CALCRECT) == 0)
               {
                  //Output Text
                  SetRect( &rc, iLeft + iPos, iTop + iHeight,
                              iLeft + iMaxWidth, iTop + iHeight + iLineHeight );

                  DrawText( hdc, lpBuffer, iBufferLength, &rc, uFormat );

                  if( bWhiteSpace && iPos >= iWidthOfSpace )
                  {

                     if( hPrevFontBase && bWSBeforeTag )
                        SelectObject( hdc, hPrevFontBase );

                     SetRect( &rc, iLeft + iPos - iWidthOfSpace, iTop + iHeight,
                                   iLeft + iMaxWidth, iTop + iHeight + iLineHeight );


                     DrawText( hdc, " ", 1, &rc, uFormat );

                     if( hPrevFontBase && bWSBeforeTag )
                     SelectObject( hdc, hFontBase );

                  }

               }

               iPos += size.cx;

               if( iPos > iMinWidth )
                  iMinWidth = iPos;

               bWhiteSpace = FALSE;
               bWSBeforeTag = FALSE;

               hb_xfree( (void *)lpBuffer );
            }
            break;
      }


      if( bWhiteSpace )
         bWSBeforeTag = TRUE;

      lpStart += iTokenLength;
   }

   SelectObject( hdc, hOldFont );

   if( hPrevFontBase )
      DeleteObject( hPrevFontBase );

   DeleteObject( hFontBase );
   RestoreDC( hdc, iSaveDC );

   FreeColors();

   if( (uFormat & DT_CALCRECT) != 0 )
   {
      lpRect->right  = lpRect->left + iMinWidth;
      lpRect->bottom = lpRect->top + iHeight + iLineHeight;
   }

   return iHeight;
}

//----------------------------------------------------------------------------//
// EOF
//-----