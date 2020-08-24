/**************************************************************************************************
* Project      : TTaskpanel
* File         : c_ThemeAux.c
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

HMODULE GetResources( void );

typedef struct
{
  char        *szColor;
  ULONG       uColor;
} COLDEF, *PCOLDEF;

#define NUM_DEFCOLORS                 175

static COLDEF sColors[NUM_DEFCOLORS] =
{
{ "AliceBlue"             , 0xF0F8FF },
{ "AntiqueWhite"          , 0xFAEBD7 },
{ "Aqua"                  , 0x00FFFF },
{ "Aquamarine"            , 0x7FFFD4 },
{ "Azure"                 , 0xF0FFFF },
{ "Beige"                 , 0xF5F5DC },
{ "Bisque"                , 0xFFE4C4 },
{ "Black"                 , 0x000000 },
{ "BlanchedAlmond"        , 0xFFEBCD },
{ "Blue"                  , 0x0000FF },
{ "BlueViolet"            , 0x8A2BE2 },
{ "Brown"                 , 0xA52A2A },
{ "BurlyWood"             , 0xDEB887 },
{ "CadetBlue"             , 0x5F9EA0 },
{ "Chartreuse"            , 0x7FFF00 },
{ "Chocolate"             , 0xD2691E },
{ "Coral"                 , 0xFF7F50 },
{ "CornflowerBlue"        , 0x6495ED },
{ "Cornsilk"              , 0xFFF8DC },
{ "Crimson"               , 0xDC143C },
{ "Cyan"                  , 0x00FFFF },
{ "DarkBlue"              , 0x00008B },
{ "DarkCyan"              , 0x008B8B },
{ "DarkGoldenrod"         , 0xB8860B },
{ "DarkGray"              , 0xA9A9A9 },
{ "DarkGreen"             , 0x006400 },
{ "DarkGrey"              , 0xA9A9A9 },
{ "DarkKhaki"             , 0xBDB76B },
{ "DarkMagenta"           , 0x8B008B },
{ "DarkOliveGreen"        , 0x556B2F },
{ "DarkOrange"            , 0xFF8C00 },
{ "DarkOrchid"            , 0x9932CC },
{ "DarkRed"               , 0x8B0000 },
{ "DarkSalmon"            , 0xE9967A },
{ "DarkSeaGreen"          , 0x8FBC8F },
{ "DarkSlateBlue"         , 0x483D8B },
{ "DarkSlateGray"         , 0x2F4F4F },
{ "DarkTurquoise"         , 0x00CED1 },
{ "DarkViolet"            , 0x9400D3 },
{ "DeepPink"              , 0xFF1493 },
{ "DeepSkyBlue"           , 0x00BFFF },
{ "DimGray"               , 0x696969 },
{ "DkBlue"                , 0x00008B },
{ "DkCyan"                , 0x008B8B },
{ "DkGoldenrod"           , 0xB8860B },
{ "DkGray"                , 0xA9A9A9 },
{ "DkGreen"               , 0x006400 },
{ "DkGrey"                , 0xA9A9A9 },
{ "DkKhaki"               , 0xBDB76B },
{ "DkMagenta"             , 0x8B008B },
{ "DkOliveGreen"          , 0x556B2F },
{ "DkOrange"              , 0xFF8C00 },
{ "DkOrchid"              , 0x9932CC },
{ "DkRed"                 , 0x8B0000 },
{ "DkSalmon"              , 0xE9967A },
{ "DkSeaGreen"            , 0x8FBC8F },
{ "DkSlateBlue"           , 0x483D8B },
{ "DkSlateGray"           , 0x2F4F4F },
{ "DkTurquoise"           , 0x00CED1 },
{ "DkViolet"              , 0x9400D3 },
{ "DodgerBlue"            , 0x1E90FF },
{ "FireBrick"             , 0xB22222 },
{ "FloralWhite"           , 0xFFFAF0 },
{ "ForestGreen"           , 0x228B22 },
{ "Fuchsia"               , 0xFF00FF },
{ "Gainsboro"             , 0xDCDCDC },
{ "GhostWhite"            , 0xF8F8FF },
{ "Gold"                  , 0xFFD700 },
{ "Goldenrod"             , 0xDAA520 },
{ "Gray"                  , 0x808080 },
{ "Green"                 , 0x008000 },
{ "GreenYellow"           , 0xADFF2F },
{ "Grey"                  , 0x808080 },
{ "Honeydew"              , 0xF0FFF0 },
{ "HotPink"               , 0xFF69B4 },
{ "IndianRed"             , 0xCD5C5C },
{ "Indigo"                , 0x4B0082 },
{ "Ivory"                 , 0xFFFFF0 },
{ "Khaki"                 , 0xF0E68C },
{ "Lavender"              , 0xE6E6FA },
{ "LavenderBlush"         , 0xFFF0F5 },
{ "LawnGreen"             , 0x7CFC00 },
{ "LemonChiffon"          , 0xFFFACD },
{ "LightBlue"             , 0xADD8E6 },
{ "LightCoral"            , 0xF08080 },
{ "LightCyan"             , 0xE0FFFF },
{ "LightGoldenrodYellow"  , 0xFAFAD2 },
{ "LightGray"             , 0xD3D3D3 },
{ "LightGreen"            , 0x90EE90 },
{ "LightGrey"             , 0xD3D3D3 },
{ "LightPink"             , 0xFFB6C1 },
{ "LightSalmon"           , 0xFFA07A },
{ "LightSeaGreen"         , 0x20B2AA },
{ "LightSkyBlue"          , 0x87CEFA },
{ "LightSlateGray"        , 0x778899 },
{ "LightSteelBlue"        , 0xB0C4DE },
{ "LightYellow"           , 0xFFFFE0 },
{ "Lime"                  , 0x00FF00 },
{ "LimeGreen"             , 0x32CD32 },
{ "Linen"                 , 0xFAF0E6 },
{ "LtBlue"                , 0xADD8E6 },
{ "LtCoral"               , 0xF08080 },
{ "LtCyan"                , 0xE0FFFF },
{ "LtGoldenrodYellow"     , 0xFAFAD2 },
{ "LtGray"                , 0xD3D3D3 },
{ "LtGreen"               , 0x90EE90 },
{ "LtGrey"                , 0xD3D3D3 },
{ "LtPink"                , 0xFFB6C1 },
{ "LtSalmon"              , 0xFFA07A },
{ "LtSeaGreen"            , 0x20B2AA },
{ "LtSkyBlue"             , 0x87CEFA },
{ "LtSlateGray"           , 0x778899 },
{ "LtSteelBlue"           , 0xB0C4DE },
{ "LtYellow"              , 0xFFFFE0 },
{ "Magenta"               , 0xFF00FF },
{ "Maroon"                , 0x800000 },
{ "MediumAquamarine"      , 0x66CDAA },
{ "MediumBlue"            , 0x0000CD },
{ "MediumOrchid"          , 0xBA55D3 },
{ "MediumPurple"          , 0x9370DB },
{ "MediumSeaGreen"        , 0x3CB371 },
{ "MediumSlateBlue"       , 0x7B68EE },
{ "MediumSpringGreen"     , 0x00FA9A },
{ "MediumTurquoise"       , 0x48D1CC },
{ "MediumVioletRed"       , 0xC71585 },
{ "MidnightBlue"          , 0x191970 },
{ "MintCream"             , 0xF5FFFA },
{ "MistyRose"             , 0xFFE4E1 },
{ "Moccasin"              , 0xFFE4B5 },
{ "NavajoWhite"           , 0xFFDEAD },
{ "Navy"                  , 0x000080 },
{ "OldLace"               , 0xFDF5E6 },
{ "Olive"                 , 0x808000 },
{ "OliveDrab"             , 0x6B8E23 },
{ "Orange"                , 0xFFA500 },
{ "OrangeRed"             , 0xFF4500 },
{ "Orchid"                , 0xDA70D6 },
{ "PaleGoldenrod"         , 0xEEE8AA },
{ "PaleGreen"             , 0x98FB98 },
{ "PaleTurquoise"         , 0xAFEEEE },
{ "PaleVioletRed"         , 0xDB7093 },
{ "PapayaWhip"            , 0xFFEFD5 },
{ "PeachPuff"             , 0xFFDAB9 },
{ "Peru"                  , 0xCD853F },
{ "Pink"                  , 0xFFC0CB },
{ "Plum"                  , 0xDDA0DD },
{ "PowderBlue"            , 0xB0E0E6 },
{ "Purple"                , 0x800080 },
{ "Red"                   , 0xFF0000 },
{ "RosyBrown"             , 0xBC8F8F },
{ "RoyalBlue"             , 0x4169E1 },
{ "SaddleBrown"           , 0x8B4513 },
{ "Salmon"                , 0xFA8072 },
{ "SandyBrown"            , 0xF4A460 },
{ "SeaGreen"              , 0x2E8B57 },
{ "Seashell"              , 0xFFF5EE },
{ "Sienna"                , 0xA0522D },
{ "Silver"                , 0xC0C0C0 },
{ "SkyBlue"               , 0x87CEEB },
{ "SlateBlue"             , 0x6A5ACD },
{ "SlateGray"             , 0x708090 },
{ "Snow"                  , 0xFFFAFA },
{ "SpringGreen"           , 0x00FF7F },
{ "SteelBlue"             , 0x4682B4 },
{ "Tan"                   , 0xD2B48C },
{ "Teal"                  , 0x008080 },
{ "Thistle"               , 0xD8BFD8 },
{ "Tomato"                , 0xFF6347 },
{ "Turquoise"             , 0x40E0D0 },
{ "Violet"                , 0xEE82EE },
{ "Wheat"                 , 0xF5DEB3 },
{ "White"                 , 0xFFFFFF },
{ "WhiteSmoke"            , 0xF5F5F5 },
{ "Yellow"                , 0xFFFF00 },
{ "YellowGreen"           , 0x9ACD32 }
};

typedef struct
{
  char        *szAlign;
  UINT        uAlign;
} ALIGNDEF, *PALIGNDEF;

#define NUM_DEFALIGNS               29

static ALIGNDEF     sAlignments[NUM_DEFALIGNS] =
{
  { "bottom",       DT_LEFT },
  { "bottomcenter", DT_CENTER },
  { "bottomleft",   DT_LEFT },
  { "bottomright",  DT_RIGHT },
  { "center",       DT_CENTER },
  { "centerbottom", DT_CENTER },
  { "centermiddle", DT_CENTER },
  { "centertop",    DT_CENTER },
  { "focusrect",    DT_LEFT },
  { "left",         DT_LEFT },
  { "leftbottom",   DT_LEFT },
  { "leftmiddle",   DT_LEFT },
  { "lefttop",      DT_LEFT },
  { "middle",       DT_LEFT },
  { "middlecenter", DT_CENTER },
  { "middleleft",   DT_LEFT },
  { "middleright",  DT_RIGHT },
  { "right",        DT_RIGHT },
  { "rightbottom",  DT_RIGHT },
  { "rightmiddle",  DT_RIGHT },
  { "righttop",     DT_RIGHT },
  { "top",          DT_LEFT },
  { "topcenter",    DT_CENTER },
  { "topleft",      DT_LEFT },
  { "topright",     DT_RIGHT },
  { "wrap",         DT_LEFT },
  { "wrapcenter",   DT_CENTER },
  { "wrapleft",     DT_LEFT },
  { "wrapright",    DT_RIGHT }
};

//----------------------------------------------------------------------------//

int htoi( const char *cHex )
{
   ULONG ulNum = 0;
   char  c;
   int   iDigit;

   while ( *cHex && (*cHex == ' ') ) cHex++;

   while ( *cHex )
   {

      c = *cHex;

      ulNum <<= 4;

      if ( c >= '0' && c <= '9' )
      {
         iDigit = c - '0';
      }
      else if ( c >= 'A' && c <= 'F' )
      {
         iDigit = c - 'A' + 10;
      }
      else if ( c >= 'a' && c <= 'f' )
      {
         iDigit = c - 'a' + 10;
      }
      else
      {
         ulNum >>= 4;
         break;
      }
      ulNum += iDigit;
      cHex++;
   }

   return ulNum;
}

//----------------------------------------------------------------------------//

BOOL GetHTMLColor( const char * szColor, COLORREF * crColor )
{
   if( szColor )
   {
      if( szColor[0] == '#' )
      {
         if( crColor )
            *crColor = htoi( &szColor[1] );
         return TRUE;
      }
   }

   return FALSE;
}

//----------------------------------------------------------------------------//

BOOL GetColorFromName( const char * szColor, COLORREF * crColor )
{
   if( szColor )
   {
      int iLeft  = 0;
      int iRight = NUM_DEFCOLORS-1;
      int iMiddle, iCmp;

      while (iLeft <= iRight)
      {
         iMiddle = (iLeft+iRight) >>1 ;

         if ( !( iCmp = stricmp(szColor, sColors[ iMiddle ].szColor ) ))
         {
            if( crColor )
               *crColor = XCHGCOLORREF( (DWORD) sColors[ iMiddle ].uColor );
            return TRUE;
         }

         if ( iCmp < 0 )
           iRight = iMiddle - 1;
         else
           iLeft  = iMiddle + 1;
      }

   }

   return FALSE;
}


//----------------------------------------------------------------------------//

BOOL GetAlignFromName( const char * szAlign, UINT * uAlign )
{
   if( uAlign )
   {
      int iLeft  = 0;
      int iRight = NUM_DEFALIGNS-1;
      int iMiddle, iCmp;

      while (iLeft <= iRight)
      {
         iMiddle = (iLeft+iRight) >>1 ;

         if ( !( iCmp = stricmp(szAlign, sAlignments[ iMiddle ].szAlign ) ))
         {
            if( uAlign )
               *uAlign = sAlignments[ iMiddle ].uAlign ;
            return TRUE;
         }

         if ( iCmp < 0 )
           iRight = iMiddle - 1;
         else
           iLeft  = iMiddle + 1;
      }

   }

   return FALSE;
}

//----------------------------------------------------------------------------//

COLORREF DarkenColor(long lScale, COLORREF lColor)
{
  long red   = MulDiv(GetRValue(lColor),(255-lScale),255);
  long green = MulDiv(GetGValue(lColor),(255-lScale),255);
  long blue  = MulDiv(GetBValue(lColor),(255-lScale),255);

  return( RGB(red, green, blue));
}

//----------------------------------------------------------------------------//

long Object_GetNL( PHB_ITEM pSelf, char *varname )
{
   PHB_DYNS pDyns = hb_dynsymFindName( varname );

   if( pDyns )
   {
      hb_vmPushSymbol( hb_dynsymSymbol( pDyns ) );
      hb_vmPush( pSelf );
      hb_vmSend( 0 );
   }
   return hb_parnl( -1 );
}

//----------------------------------------------------------------------------//

BOOL Object_GetL( PHB_ITEM pSelf, char *varname )
{
   PHB_DYNS pDyns = hb_dynsymFindName( varname );

   if( pDyns )
   {
      hb_vmPushSymbol( hb_dynsymSymbol( pDyns ) );
      hb_vmPush( pSelf );
      hb_vmSend( 0 );
   }
   return hb_parl( -1 );
}

//----------------------------------------------------------------------------//

HBITMAP CreateMemDIBitmapT( void * lpBits, ULONG ulSize, DWORD dwTransColor )
{
  DIBBMP db;

  DibBmp_CreateFromMem( &db, lpBits, ulSize );

  if( db.hBitmap )
  {
    if( db.bHasAlpha )
      DibBmp_PreMultiplyAlpha( &db );
    else
      DibBmp_SetTransparentColor( &db, dwTransColor );
  }
  DibBmp_Release2( &db );

   return db.hBitmap;
}

//----------------------------------------------------------------------------//

HBITMAP LoadBitmapEx( HINSTANCE hInstance, const char * szName, BOOL bTransparent )
{
  DIBBMP db;

  if( !DibBmp_LoadFromResource( &db, hInstance, szName ) )
    DibBmp_LoadFromFile( &db, szName );

  if( db.hBitmap && bTransparent && !db.bHasAlpha )
    DibBmp_SetTransparentColor( &db, RGB(255,0,255) );

  DibBmp_Release2( &db );

  return db.hBitmap;
}
//----------------------------------------------------------------------------//

HB_FUNC( LOADBITMAPEX )
{
  HINSTANCE    hInstance;
  const char * szName;
  BOOL         bTransparent;
  DIBBMP db;

  if( ISCHAR(1) )
  {
    hInstance    = GetResources();
    szName       = hb_parc(1);
    bTransparent = hb_parl(2);
  }
  else if( ISCHAR(2) || ISNUM(2) )
  {
    hInstance    = (HINSTANCE) hb_parnl(1);
    szName       = ISCHAR(2) ? hb_parc(2) : MAKEINTRESOURCE( hb_parni(2) );
    bTransparent = hb_parl(3);
  }
  else
  {
    hb_retnl( 0 );
    return;
  }

  if( !DibBmp_LoadFromResource( &db, hInstance, szName ) )
    DibBmp_LoadFromFile( &db, szName );

  if( db.hBitmap && bTransparent && !db.bHasAlpha )
  {
    DibBmp_SetTransparentColorFirstPixel( &db );
  }

  DibBmp_Release2( &db );

  hb_reta(2);
  hb_storvnl( (long) db.hBitmap   , -1, 1 );
  hb_storvl(         db.bHasAlpha , -1, 2 );
}


HB_FUNC( SETSCROLLRANGEX )
{
   SetScrollRange( ( HWND ) hb_parnl( 1 ),
                   hb_parni( 2 ),
                   hb_parni( 3 ),
                   hb_parni( 4 ),
                   hb_parl( 5 ) );
}

//----------------------------------------------------------------------------//
