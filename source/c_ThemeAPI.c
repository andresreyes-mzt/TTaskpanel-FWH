/**************************************************************************************************
* Project      : TTaskpanel
* File         : c_ThemeAPI.c
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

#define DEBUG
#include "stdafx.h"

#ifndef RT_MANIFEST
#define RT_MANIFEST 24
#endif
//----------------------------------------------------------------------------//

typedef struct __themex    THEMEX, *PTHEMEX;
typedef struct __resid     RESID,  *PRESID;
typedef struct __element   ELEMENT, *PELEMENT;
typedef struct __prop      PROP, *PPROP;

struct __themex
{
   char       *cName;
   PINITTHEME  InitTheme;
   PTHEMEX     pNext;
};

struct __resid
{
   char *   cName;
   PELEMENT pItems;
   PRESID   pNext;
};

struct __element
{
   char *      cName;
   PPROP       pItems;
   PELEMENT    pNext;
};

struct __prop
{
   char * cName;
   char * cValue;
   PPROP  pNext;
};

static PRESID ParseUIFILE( PBUFFER b );
static void FreeParseInfo( PRESID pInfo );
static BOOL GetShellStyleValues( PTHEMEINFO pti, PRESID pInfo, HINSTANCE hDll );
static void FreeThemeInfo( PTHEMEINFO pti );
static void LoadInternalTheme( PTHEMEINFO pti  );
static void ThemeConfigure( PTHEMEINFO pti );
static BOOL ConvertTo( const char *szString, WORD wType, void * pValue, ... );

static PTHEMEX s_pThemex = NULL;

//----------------------------------------------------------------------------//

static void FreeThemex( void * cargo )
{
   PTHEMEX pNext = s_pThemex;
   PTHEMEX pThemex;

   while( pNext )
   {
      pThemex = pNext;
      pNext   = pNext->pNext;

      hb_xfree( pThemex );
   }

   HB_SYMBOL_UNUSED( cargo );
}

//----------------------------------------------------------------------------//

BOOL RegisterTHEMEX( char * cThemeName, PINITTHEME p )
{
   PTHEMEX pThemex = hb_xgrab( sizeof(THEMEX) );

   if( s_pThemex == NULL )
      hb_vmAtExit( FreeThemex, NULL );

   if( pThemex )
   {
      pThemex->cName     = cThemeName;
      pThemex->InitTheme = p;
      pThemex->pNext     = s_pThemex;

      s_pThemex = pThemex;

      return TRUE;
   }

   return FALSE;
}

//----------------------------------------------------------------------------//

static BOOL InitThemeInfo( const char *cThemeName, PTHEMEINFO pti )
{
   PTHEMEX pNext = s_pThemex;

   while( pNext )
   {
      if( stricmp( pNext->cName, cThemeName ) == 0 )
         return pNext->InitTheme( pti );

      pNext = pNext->pNext;
   }

   return FALSE;
}

//----------------------------------------------------------------------------//

HB_FUNC( TM_GETSHELLSTYLEPATH )
{
   OSVERSIONINFO osvi;
   HKEY           hKey = NULL;
   DWORD          dwSize;
   char           szString[MAX_PATH];
   char           szMsStylesDllName[MAX_PATH];
   char           szColorName[MAX_PATH];
   char          *p;


   osvi.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
   GetVersionEx ( &osvi );

   //Shellstyle.dll es incluido solo a partir de XP
   if( (osvi.dwMajorVersion < 5) && (osvi.dwMinorVersion < 1) )
   {
      DefaultTheme:

         //Por consiguiente usaremos el Tema Clasico de Windows
         hb_retc( "#CLASSIC" );
         return;
   }

   //Tambien revisamos que la aplicacion cuente con el Manifiesto de Windows
   if( FindResource(GetModuleHandle(NULL),MAKEINTRESOURCE(MANIFEST_RESOURCE_ID),MAKEINTRESOURCE(RT_MANIFEST)) == NULL )
   {
         hb_retc( "#CLASSIC" );
         return;
   }

   if ((ERROR_SUCCESS != RegOpenKey(HKEY_CURRENT_USER,
                                 "Software\\Microsoft\\Windows\\CurrentVersion\\ThemeManager",
                                 &hKey)) || (!hKey))
      goto DefaultTheme;

   dwSize = sizeof( szString );

   //Revisamos si hay un Tema Activo
   if ( ERROR_SUCCESS != RegQueryValueEx( hKey,
                                         "ThemeActive",
                                         NULL,
                                         NULL,
                                         (LPBYTE)szString,
                                         &dwSize)
      )
   {
      RegCloseKey(hKey);
      goto DefaultTheme;
   }

   if( !atoi( szString ) )
   {
      RegCloseKey(hKey);
      goto DefaultTheme;
   }

   //////////////////////////
   // Oks ... sabiendo que el tema esta activo
   // verificamos si es windows seven o vista
   // y mandamos nuestro tema ya que nos es imposible
   // leer el uifile
   //////////

   if( (osvi.dwMajorVersion >= 6) && (osvi.dwMinorVersion >= 0) )
   {
      RegCloseKey(hKey);
      hb_retc( "#XPMETALLIC" );
      return;
   }

   dwSize = sizeof( szString );

   if ( ERROR_SUCCESS != RegQueryValueEx(hKey,
                                         "DllName",
                                         NULL,
                                         NULL,
                                         (LPBYTE)szString,
                                         &dwSize)
       )
   {
      RegCloseKey(hKey);
      goto DefaultTheme;
   }

   ExpandEnvironmentStrings( szString, szMsStylesDllName, MAX_PATH - 1 );

   strcpy( szString, szMsStylesDllName );
   strlwr( szString );

   if( !strstr( szString, ".msstyles" ) )
   {
      RegCloseKey(hKey);
      goto DefaultTheme;
   }

   //Obtenemos el Color del Tema
   dwSize = sizeof( szColorName );

   if ( ERROR_SUCCESS != RegQueryValueEx( hKey,
                                        "ColorName",
                                        NULL,
                                        NULL,
                                        (LPBYTE) szColorName,
                                        &dwSize)
      )
   {
      RegCloseKey(hKey);
      goto DefaultTheme;
   }

   RegCloseKey(hKey);

   //Obtenemos ruta de shellstyle.dll

   strcpy( szString, szMsStylesDllName );
   p = strrchr( szString, '\\' );

   if ( !p )
      goto DefaultTheme;

   *p = 0;
   strcat( szString, "\\shell\\" );
   strcat( szString, szColorName );
   strcat( szString, "\\shellstyle.dll" );

   hb_retc( szString );
}

//----------------------------------------------------------------------------//

HB_FUNC( TM_OPENTHEMEDATA )
{
   PTHEMEINFO pti;
   const char * szShellStyleDllName;

   //En caso de no especificarse nombre del archivo DLL
   //Se cargara el tema Clasico de Windows
   if( ISCHAR(1) )
      szShellStyleDllName = hb_parc(1);
   else
   {
      LoadDefault:

      szShellStyleDllName = "#CLASSIC";
   }

   pti = (PTHEMEINFO) hb_xgrab( sizeof(THEMEINFO) );
   memset( pti, 0, sizeof(THEMEINFO) );
   strcpy( pti->szShellStyleDllName, szShellStyleDllName );

   if( pti->szShellStyleDllName[0] == '#')
   {
      LoadInternalTheme( pti );
   }
   else
   {
      //Intentamos cargar el DLL
      HINSTANCE   hDll;
      HRSRC       hShellInfo;
      HGLOBAL     hShellInfoMem;
      BUFFER      b;
      PRESID      pInfo;


      if ( ( hDll = LoadLibraryEx( pti->szShellStyleDllName,
                                  NULL,
                                  LOAD_LIBRARY_AS_DATAFILE )
            ) == NULL )
      {
         //Cualquier Fallo al Cargar o leer la informacion del DLL
         //cargaremos el Tema por Default
         goto LoadDefault;
      }

      if ( ( hShellInfo = FindResource( hDll, MAKEINTRESOURCE(1), "UIFILE" ) ) == NULL )
      {
         FreeLibrary( hDll );
         goto LoadDefault;
      }

      if( ( hShellInfoMem = LoadResource( hDll, hShellInfo ) ) == NULL )
      {
         FreeLibrary( hDll );
         goto LoadDefault;
      }

      if( ( b.wSize = (WORD) SizeofResource( hDll, hShellInfo ) ) == 0 ||
          ( b.szBuffer = LockResource( hShellInfoMem ) ) == NULL )
      {
         FreeResource( hShellInfoMem );
         FreeLibrary( hDll );
         goto LoadDefault;
      }

      //Analizamos el recurso UIFILE y extraemos los datos necesarios para nuestro tema
      if( ( pInfo = ParseUIFILE( &b )) == NULL )
      {
         FreeResource( hShellInfoMem );
         FreeLibrary( hDll );
         goto LoadDefault;
      }


      if( !GetShellStyleValues( pti, pInfo, hDll ) )
      {
         FreeThemeInfo( pti );
         FreeParseInfo( pInfo );
         FreeResource( hShellInfoMem );
         FreeLibrary( hDll );
         goto LoadDefault;

      }

      ThemeConfigure( pti );
      FreeParseInfo( pInfo );
      FreeResource( hShellInfoMem );
      FreeLibrary( hDll );
   }


   hb_retnl( (long) pti );

   return;

}

//----------------------------------------------------------------------------//

HB_FUNC( TM_CLOSETHEMEDATA )
{
   FreeThemeInfo( (PTHEMEINFO) hb_parnl(1) );
}

//----------------------------------------------------------------------------//

static void FreeThemeInfo( PTHEMEINFO pti )
{
   if( pti )
   {
      int i;

      //////////////////////////
      // Taskpanel
      //////////

      if( pti->TaskPanel.hBmpBackground )
         DeleteObject( pti->TaskPanel.hBmpBackground );

      //////////////////////////
      // Normal Group
      //////////

      if( pti->NormalGroup.Header.hFontNormal )
         DeleteObject( pti->NormalGroup.Header.hFontNormal );

      if( pti->NormalGroup.Header.hFontHot )
         DeleteObject( pti->NormalGroup.Header.hFontHot );

      if( pti->NormalGroup.Header.hBitmap )
         DeleteObject( pti->NormalGroup.Header.hBitmap );

      if( pti->NormalGroup.Header.hBitmapHot )
         DeleteObject( pti->NormalGroup.Header.hBitmapHot );

      if( pti->NormalGroup.Header.hBitmapArrowUp[0] )
         DeleteObject( pti->NormalGroup.Header.hBitmapArrowUp[0] );

      if( pti->NormalGroup.Header.hBitmapArrowUp[1] )
         DeleteObject( pti->NormalGroup.Header.hBitmapArrowUp[1] );

      if( pti->NormalGroup.Header.hBitmapArrowDn[0] )
         DeleteObject( pti->NormalGroup.Header.hBitmapArrowDn[0] );

      if( pti->NormalGroup.Header.hBitmapArrowDn[1] )
         DeleteObject( pti->NormalGroup.Header.hBitmapArrowDn[1] );

      if( pti->NormalGroup.Tasklist.hFontNormal )
         DeleteObject( pti->NormalGroup.Tasklist.hFontNormal );

      if( pti->NormalGroup.Tasklist.hFontHot )
         DeleteObject( pti->NormalGroup.Tasklist.hFontHot );

      for ( i = 0; i < 6; i++ )
      {
          if (pti->NormalGroup.Header.szBitmapName[i])
          {
              hb_xfree(pti->NormalGroup.Header.szBitmapName[i]);
          }
      }

      //////////////////////////
      // Special Group
      //////////

      if( pti->SpecialGroup.Header.hFontNormal )
         DeleteObject( pti->SpecialGroup.Header.hFontNormal );

      if( pti->SpecialGroup.Header.hFontHot )
         DeleteObject( pti->SpecialGroup.Header.hFontHot );

      if( pti->SpecialGroup.Header.hBitmap )
         DeleteObject( pti->SpecialGroup.Header.hBitmap );

      if( pti->SpecialGroup.Header.hBitmapHot )
         DeleteObject( pti->SpecialGroup.Header.hBitmapHot );

      if( pti->SpecialGroup.Header.hBitmapArrowUp[0] )
         DeleteObject( pti->SpecialGroup.Header.hBitmapArrowUp[0] );

      if( pti->SpecialGroup.Header.hBitmapArrowUp[1] )
         DeleteObject( pti->SpecialGroup.Header.hBitmapArrowUp[1] );

      if( pti->SpecialGroup.Header.hBitmapArrowDn[0] )
         DeleteObject( pti->SpecialGroup.Header.hBitmapArrowDn[0] );

      if( pti->SpecialGroup.Header.hBitmapArrowDn[1] )
         DeleteObject( pti->SpecialGroup.Header.hBitmapArrowDn[1] );

      if( pti->SpecialGroup.Tasklist.hFontNormal )
         DeleteObject( pti->SpecialGroup.Tasklist.hFontNormal );

      if( pti->SpecialGroup.Tasklist.hFontHot )
         DeleteObject( pti->SpecialGroup.Tasklist.hFontHot );

      for ( i = 0; i < 6; i++ )
      {
          if (pti->SpecialGroup.Header.szBitmapName[i])
          {
              hb_xfree(pti->SpecialGroup.Header.szBitmapName[i]);
          }
      }
      hb_xfree( pti );
   }
}

//----------------------------------------------------------------------------//
static char * GetBLOCK( PBUFFER b, const char cEnd )
{
   WORD  wPos     = b->wPos;
   char *szBuffer  = NULL;

   while( b->wPos < b->wSize )
   {
      if( b->szBuffer[ b->wPos ] == cEnd )
         break;
      b->wPos++;
   }

   if( b->wPos > wPos )
   {
      WORD wSize = (WORD)((int)b->wPos - (int)wPos + 1) ;

      szBuffer = (char *) hb_xgrab( wSize );

      memcpy( szBuffer, &b->szBuffer[ wPos ], wSize - 1 );

      szBuffer[ wSize -1 ] = 0;
   }

   if( b->szBuffer[ b->wPos ] == cEnd )
      b->wPos++;

   return szBuffer;
}

//----------------------------------------------------------------------------//

static PRESID ParseUIFILE( PBUFFER b )
{
   PRESID   pRoot = NULL;
   PRESID   pResId;
   PELEMENT pElement;
   PPROP    pProp;
   char     *szString, *p, *p2;

   b->wPos = 0;

   while( b->wPos < b->wSize )
   {

      switch( b->szBuffer[ b->wPos ] )
      {
         case '<':
            {
               b->wPos++;

               if( (szString = GetBLOCK( b, '>')) == NULL )
                  break;

               if( szString[0] == '/' || (p = strstr( szString, "resid=" )) == NULL )
               {
                  hb_xfree( szString );
                  break;
               }

               //Comprobacion Basica
               if( !(strnicmp( szString, "style", 5 ) == 0 ) )
               {
                  hb_xfree( szString );
                  return pRoot;
               }

               p += 6;

               pResId = (PRESID) hb_xgrab( sizeof(RESID) );
               pResId->cName  = hb_strdup( p );
               pResId->pNext  = pRoot;
               pResId->pItems = NULL;

               pRoot = pResId;

               hb_xfree( szString );
               break;
            }
         case '{':
            {
               b->wPos++;

               if( pRoot )
               {
                  char * szName;
                  char * szValue;

                  if( (szString = GetBLOCK( b, '}' ) ) == NULL )
                     break;

                  p2 =
                  p = szString;

                  while( *p )
                  {
                     switch( p[0] )
                     {
                        case 13:
                        case 10:
                              p2 = p + 1;
                              break;

                        case ':':
                              *p++ = 0;
                              szName = p2;
                              p2 = p;
                              break;

                        case ';':
                              *p++ = 0;
                              szValue = p2;
                              p2 = p;

                              while( *szValue == ' ' )
                                 szValue++;

                              if( pRoot && pRoot->pItems )
                              {
                                 pProp = (PPROP) hb_xgrab( sizeof(PROP) );
                                 pProp->cName  = hb_strdup( szName );
                                 pProp->cValue = hb_strdup( szValue );
                                 pProp->pNext  = pRoot->pItems->pItems;

                                 pRoot->pItems->pItems = pProp;
                              }

                              break;
                     }

                     p++;
                  }

                  hb_xfree( szString );
               }

               break;
            }
         case 13:
         case 10:
            {
               b->wPos++;
               break;
            }
         default:
            {
               if( isalpha( b->szBuffer[ b->wPos ] ) && pRoot )
               {
                  BOOL bSep = FALSE;

                  if( (szString = GetBLOCK( b, 13 ) ) == NULL )
                     break;

                  p  =
                  p2 = szString;

                  while( *p )
                  {
                     if( isalpha(*p) )
                     {
                        if( bSep )
                         *p2++ = '-';

                        *p2++ = *p;

                        bSep = FALSE;
                     }
                     else
                     {
                        bSep = TRUE;
                     }
                     p++;
                  }
                  *p2 = 0;

                  p = strstr( szString, "id-atom-" );

                  if( p )
                     strcpy( p, (p + 8) );

                  pElement = (PELEMENT) hb_xgrab( sizeof(ELEMENT) );
                  pElement->cName  = hb_strdup( szString );
                  pElement->pNext  = pRoot->pItems;
                  pElement->pItems = NULL;

                  pRoot->pItems = pElement;

                  hb_xfree( szString );
                  break;
               }


               b->wPos++;
               break;
            }

      }
   }

   return pRoot;
}

//----------------------------------------------------------------------------//

static void FreeParseInfo( PRESID pInfo )
{
   PRESID pNext = pInfo;

   while( pNext )
   {
      pInfo = pNext;
      pNext = pNext->pNext;

      if( pInfo->pItems )
      {
         PELEMENT pElement;
         PELEMENT pNextE = pInfo->pItems;

         while( pNextE )
         {
            pElement = pNextE;
            pNextE = pNextE->pNext;

            if( pElement->pItems )
            {
               PPROP pProp;
               PPROP pNextP = pElement->pItems;

               while( pNextP )
               {
                  pProp = pNextP;
                  pNextP = pNextP->pNext;

                  hb_xfree( pProp->cName );
                  hb_xfree( pProp->cValue );
                  hb_xfree( pProp );
               }
            }


            hb_xfree( pElement->cName );
            hb_xfree( pElement );
         }
      }
      hb_xfree( pInfo->cName );
      hb_xfree( pInfo );
   }

}
//----------------------------------------------------------------------------//

static char * GetPropertyValue( const PRESID pInfo, char * szResId, char * szElement, char * szProp )
{
   PRESID resid = pInfo;

   while( resid )
   {
      if( stricmp( resid->cName, szResId ) == 0 )
      {
         PELEMENT element = resid->pItems;

         while( element )
         {
            if( stricmp( element->cName, szElement ) == 0 )
            {
               PPROP prop = element->pItems;

               while( prop)
               {
                  if( stricmp( prop->cName, szProp) == 0 )
                     return prop->cValue;

                  prop = prop->pNext;
               }

               break;
            }

            element = element->pNext;
         }

         break;
      }

      resid = resid->pNext;
   }

   return NULL;
}


//----------------------------------------------------------------------------//

#define GetToken( t )      ( t->szToken[ t->wPos] )

static char * GetNextToken( PTOKEN token )
{
   token->wPos++;

   if( token->wPos >= token->wSize )
      return "";
   else
      return token->szToken[ token->wPos ];
}



//----------------------------------------------------------------------------//

static BOOL __GetColor( PTOKEN token, COLORREF *crValue )
{
   COLORREF crColor = 0;

   //Color Guardado en Formato Hexadecimal
   if( *GetToken( token ) == '#' )
   {
      return GetHTMLColor( GetToken( token ), crValue );
   }
   else if( stricmp( GetToken( token ), "rgb" ) == 0 )
   {
      //Red Color
      crColor |= (((DWORD)atoi( GetNextToken( token ) )) &0xFFL ) << 16;

      //Green Color
      crColor |= (((DWORD)atoi( GetNextToken( token ) )) &0xFFL ) << 8;

      //Blue Color
      crColor |= (((DWORD)atoi( GetNextToken( token ) )) &0xFFL ) << 0;

      if( crValue )
         *crValue = XCHGCOLORREF( (DWORD) crColor );
   }
   else if( stricmp( GetToken( token ), "argb" ) == 0 )
   {
      //Alpha Color
      crColor |= (((DWORD)atoi( GetNextToken( token ) )) &0xFFL ) << 24;

      //Red Color
      crColor |= (((DWORD)atoi( GetNextToken( token ) )) &0xFFL ) << 16;

      //Green Color
      crColor |= (((DWORD)atoi( GetNextToken( token ) )) &0xFFL ) << 8;

      //Blue Color
      crColor |= (((DWORD)atoi( GetNextToken( token ) )) &0xFFL ) << 0;

      if( crValue )
         *crValue = XCHGCOLORREF( (DWORD) crColor );
   }
   else
   {
      if( !GetColorFromName( GetToken( token ), crValue ) )
      {
         if( isdigit( GetToken( token )[0] ) )
         {
            if( crValue )
               *crValue = atoi( GetToken( token ) );
         }
         else
            return FALSE;
      }
   }

   return TRUE;
}

//----------------------------------------------------------------------------//

static BOOL __GetGradient( PTOKEN token, COLORREF *crValue )
{
   if( stricmp( GetToken( token ), "gradient" ) == 0 )
   {
      COLORREF crTmp[3];

      GetNextToken( token );
      __GetColor( token, &crTmp[0] );

      GetNextToken( token );
      __GetColor( token, &crTmp[1] );

      crTmp[2] = atoi( GetNextToken( token ) );


      if( crValue )
      {
         crValue[0] = crTmp[0];
         crValue[1] = crTmp[1];
         crValue[2] = crTmp[2];
      }

      return TRUE;
   }

   return FALSE;
}

//----------------------------------------------------------------------------//

static BOOL __GetRect( PTOKEN token, RECT * prt )
{
   if( stricmp( GetToken( token ), "rect" ) == 0 )
   {
      RECT rt;

      rt.left     = atoi( GetNextToken( token ) );
      rt.top      = atoi( GetNextToken( token ) );
      rt.right    = atoi( GetNextToken( token ) );
      rt.bottom   = atoi( GetNextToken( token ) );

      if( prt )
         *prt = rt;

      return TRUE;
   }
   return FALSE;
}

//----------------------------------------------------------------------------//

static BOOL __GetFontStyle( PTOKEN token, LOGFONT * lf )
{
   if( lf )
   {
      if( stricmp( GetToken( token ), "underline" ) == 0 )
         lf->lfUnderline = TRUE;
      else if( stricmp( GetToken( token ), "italic" ) == 0 || stricmp( GetToken( token ), "oblique" ) == 0)
         lf->lfItalic = TRUE;
      else if( stricmp( GetToken( token ), "underline" ) == 0 )
         lf->lfWeight = FW_BOLD;
      else
         return FALSE;

      return TRUE;
   }
   return FALSE;
}

//----------------------------------------------------------------------------//

static BOOL __GetAlign( PTOKEN token, UINT * uAlign )
{
   return GetAlignFromName( GetToken( token ), uAlign );
}

//----------------------------------------------------------------------------//

static BOOL __GetBitmap( PTOKEN token, HBITMAP * pBitmap, HINSTANCE hDll )
{
   if( stricmp( GetToken( token ), "rcbmp" ) == 0 )
   {
      int       iResId;
      //int       iStretching;
      COLORREF  crTransColor;
      char     *szToken;
      BOOL      bColorIndex = FALSE;
      DIBBMP db;


      //Id del Recurso
      iResId = atoi( GetNextToken( token ) );

      //Compression
      //iStretching = atoi( GetNextToken( token ) );
      GetNextToken( token );

      //Color Transparente
      //En Bitmaps de menores a 24 bits Indica un Indice
      szToken = GetNextToken( token );

      if( isdigit( szToken[0]) )
        bColorIndex = TRUE;

      __GetColor( token, &crTransColor );

      DibBmp_LoadFromResource( &db, hDll, MAKEINTRESOURCE( iResId ) );

      if( db.wBitCount == 32 && bColorIndex )
         DibBmp_PreMultiplyAlpha( &db );
      else if( db.wBitCount == 24 )
      {
         if( bColorIndex )
            DibBmp_SetTransparentColor( &db, 0x00FFFFFF );
         else
            DibBmp_SetTransparentColor( &db, crTransColor );

      }
      else if( db.wBitCount < 24 )
      {
         if( bColorIndex )
         {
            if( crTransColor > 0 )
               DibBmp_SetTransparentColorIndex( &db, (BYTE) crTransColor );
         }
         else
            DibBmp_SetTransparentColor( &db, crTransColor );
      }

      DibBmp_Release2( &db );

      if( db.hBitmap )
      {
         if(pBitmap)
            *pBitmap = db.hBitmap;

         return TRUE;
      }
   }

   return FALSE;
}

//----------------------------------------------------------------------------//

int Pt2FontHeight ( int iPt )
{
   HDC hdc     = GetDC( 0 );
   int iHeight = -MulDiv( iPt, GetDeviceCaps(hdc, LOGPIXELSY), 72);

   ReleaseDC( 0, hdc );

   return iHeight;
}

//----------------------------------------------------------------------------//

int FontHeight2Pt( int iHeight )
{
   HDC hdc     = GetDC( 0 );
   int iPt  = MulDiv( abs( iHeight ), 72, GetDeviceCaps(hdc, LOGPIXELSY));

   ReleaseDC( 0, hdc );

   return iPt;
}

//----------------------------------------------------------------------------//

static BOOL __GetInt( PTOKEN token, int * iValue, HINSTANCE hDll )
{
   int iTmp;

   if( stricmp( GetToken( token ), "rcint" ) == 0 )
   {
      char szBuffer[28] = {0};
      int iResId = atoi( GetNextToken( token ) );

      LoadString( hDll, iResId, szBuffer, 28 );

      iTmp = atoi( szBuffer );

      if( strcmp( GetNextToken(token), "pt" ) == 0 )
         iTmp = Pt2FontHeight( iTmp );
   }
   else
   {
      iTmp = atoi( GetToken( token ) );

      if( !(strstr( GetToken(token), "pt" ) == NULL) )
         iTmp = Pt2FontHeight( iTmp );
   }

   if( iValue )
      *iValue = iTmp;

   return TRUE;
}

//----------------------------------------------------------------------------//

static BOOL __GetString( PTOKEN token, LPSTR szValue, WORD wSize, HINSTANCE hDll )
{
   if( stricmp( GetToken( token ), "rcstr" ) == 0 )
   {
      int iResId = atoi( GetNextToken( token ) );

      if( szValue )
         LoadString( hDll, iResId, szValue, wSize );
   }
   else
   {
      if( szValue )
         strncpy( szValue, GetToken( token ), wSize );
   }

   return TRUE;
}

//----------------------------------------------------------------------------//

#define CT_COLOR        1
#define CT_GRADIENT     2
#define CT_RECT         3
#define CT_BITMAP       4
#define CT_INT          5
#define CT_STRING       6
#define CT_FONTSTYLE    7
#define CT_ALIGN        8


static BOOL ConvertTo( const char *szString, WORD wType, void * pValue, ... )
{
   BOOL  lRet = 0;
   TOKEN token;
   WORD  n = 0;
   char *p;

   if( !szString )
      return FALSE;

   token.szBuffer   = hb_strdup( szString );
   token.wSize      = MAX_TOKEN;
   token.wPos       = 0;

   p                 = token.szBuffer;


   while( token.szBuffer[n] )
   {
      switch( token.szBuffer[n] )
      {
         case ' ':
         case '(':
         case ')':
         case ',':
            {
               token.szBuffer[n] = 0;

               //Skip Space
               while( token.szBuffer[ n + 1 ] == ' ' )
                  n++;

               if( token.wPos < token.wSize )
               {
                  if( !(p[0] == 0) )
                  {
                     token.szToken[ token.wPos ] = p;
                     token.wPos++;
                  }
               }

               p = &token.szBuffer[ n + 1 ];
            }
      }
      n++;
   }

   if( token.wPos < token.wSize )
   {
      if( !(p[0] == 0) )
      {
         token.szToken[ token.wPos ] = p;
         token.wPos++;
      }
   }

   token.wSize = token.wPos;
   token.wPos  = 0;


   switch( wType )
   {
      case CT_COLOR        :  lRet = __GetColor( &token, pValue ); break;
      case CT_GRADIENT     :  lRet = __GetGradient( &token, pValue ); break;
      case CT_RECT         :  lRet = __GetRect( &token, pValue ); break;
      case CT_FONTSTYLE    :  lRet = __GetFontStyle( &token, pValue ); break;
      case CT_ALIGN        :  lRet = __GetAlign( &token, pValue ); break;
      case CT_INT          :
      case CT_BITMAP       :
         {
            va_list   va;
            HINSTANCE hDll;

            //Obtenemos el handle del DLL
            va_start( va, pValue );
            hDll = va_arg( va, HINSTANCE );
            va_end( va );

            if( wType == CT_BITMAP )
               lRet = __GetBitmap( &token, pValue, hDll );
            else
               lRet = __GetInt( &token, pValue, hDll );

            break;
         }
      case CT_STRING       :
         {
            va_list   va;
            HINSTANCE hDll;
            WORD      wSize;

            //Obtenemos el handle del DLL
            va_start( va, pValue );
            wSize = va_arg( va, WORD );
            hDll  = va_arg( va, HINSTANCE );
            va_end( va );

            lRet = __GetString( &token, pValue, wSize, hDll );

            break;
         }

   }

   hb_xfree( token.szBuffer );
   return lRet;
}

//----------------------------------------------------------------------------//

static BOOL GetShellStyleValues( PTHEMEINFO pti, PRESID pInfo, HINSTANCE hDll )
{
   char * szValue;
   RECT rt;
   //COLORREF cColor;

   //-------------------------------------------------
   // 1.) task link information (normal)
   //-----

   //////////
   // Normal Color

   szValue = GetPropertyValue( pInfo,
                               "sectiontaskss",
                               "button",
                               "foreground" );

   if( !ConvertTo( szValue , CT_COLOR, &pti->NormalGroup.Tasklist.Link.crNormal ) )
      return FALSE;

   szValue = GetPropertyValue( pInfo,
                               "sectiontaskss",
                               "element-title",
                               "foreground" );

   ConvertTo( szValue , CT_COLOR, &pti->NormalGroup.Tasklist.Link.crNormal );

   //////////
   // Hot Color

   szValue = GetPropertyValue( pInfo,
                               "sectiontaskss",
                               "element-title-mousefocused",
                               "foreground" );

   if( !ConvertTo( szValue , CT_COLOR, &pti->NormalGroup.Tasklist.Link.crHot ) )
      return FALSE;

   //////////
   // Padding

   szValue = GetPropertyValue( pInfo,
                               "sectiontaskss",
                               "element-title",
                               "padding" );

   if( !ConvertTo( szValue, CT_RECT, &pti->NormalGroup.Tasklist.Link.rcPadding ) )
   {
      pti->NormalGroup.Tasklist.Link.rcPadding.left   = 6;
      pti->NormalGroup.Tasklist.Link.rcPadding.top    = 0;
      pti->NormalGroup.Tasklist.Link.rcPadding.right  = 4;
      pti->NormalGroup.Tasklist.Link.rcPadding.bottom = 0;
   }

   //////////
   // Margin

   szValue = GetPropertyValue( pInfo,
                               "sectiontaskss",
                               "actiontask",
                               "margin" );

   if( ConvertTo( szValue, CT_RECT, &rt ) )
   {
      pti->NormalGroup.Tasklist.iMargin = rt.left;

      if( rt.top > pti->NormalGroup.Tasklist.iMargin ) pti->NormalGroup.Tasklist.iMargin  = rt.top;
      if( rt.right > pti->NormalGroup.Tasklist.iMargin ) pti->NormalGroup.Tasklist.iMargin  = rt.right;
      if( rt.bottom > pti->NormalGroup.Tasklist.iMargin ) pti->NormalGroup.Tasklist.iMargin  = rt.bottom;
   }
   else
   {
      szValue = GetPropertyValue( pInfo,
                                  "sectiontaskss",
                                  "destinationtask",
                                  "margin" );

      if( ConvertTo( szValue, CT_RECT, &rt ) )
      {
         pti->NormalGroup.Tasklist.iMargin = rt.left;

         if( rt.top > pti->NormalGroup.Tasklist.iMargin ) pti->NormalGroup.Tasklist.iMargin  = rt.top;
         if( rt.right > pti->NormalGroup.Tasklist.iMargin ) pti->NormalGroup.Tasklist.iMargin  = rt.right;
         if( rt.bottom > pti->NormalGroup.Tasklist.iMargin ) pti->NormalGroup.Tasklist.iMargin  = rt.bottom;
      }
      else
         pti->NormalGroup.Tasklist.iMargin = 4;
   }

   //////////
   // Font Style

   szValue = GetPropertyValue( pInfo,
                               "sectiontaskss",
                               "button-mousefocused",
                               "fontstyle" );


   ConvertTo( szValue, CT_FONTSTYLE, &pti->NormalGroup.Tasklist.lfHot );

   //-------------------------------------------------
   // 2.) task link information (special)
   //-----

   //////////
   // Normal Color

   szValue = GetPropertyValue( pInfo,
                               "mainsectiontaskss",
                               "button",
                               "foreground" );

   if( !ConvertTo( szValue , CT_COLOR, &pti->SpecialGroup.Tasklist.Link.crNormal ) )
      return FALSE;

   szValue = GetPropertyValue( pInfo,
                               "mainsectiontaskss",
                               "element-title",
                               "foreground" );

   ConvertTo( szValue , CT_COLOR, &pti->SpecialGroup.Tasklist.Link.crNormal );

   //////////
   // Hot Color

   szValue = GetPropertyValue( pInfo,
                               "mainsectiontaskss",
                               "element-title-mousefocused",
                               "foreground" );

   if( !ConvertTo( szValue , CT_COLOR, &pti->SpecialGroup.Tasklist.Link.crHot ) )
      return FALSE;

   //////////
   // Padding

   szValue = GetPropertyValue( pInfo,
                               "mainsectiontaskss",
                               "element-title",
                               "padding" );

   if( !ConvertTo( szValue, CT_RECT, &pti->SpecialGroup.Tasklist.Link.rcPadding ) )
   {
      pti->SpecialGroup.Tasklist.Link.rcPadding.left   = 6;
      pti->SpecialGroup.Tasklist.Link.rcPadding.top    = 0;
      pti->SpecialGroup.Tasklist.Link.rcPadding.right  = 4;
      pti->SpecialGroup.Tasklist.Link.rcPadding.bottom = 0;
   }

   //////////
   // Margin

   szValue = GetPropertyValue( pInfo,
                               "mainsectiontaskss",
                               "actiontask",
                               "margin" );

   if( ConvertTo( szValue, CT_RECT, &rt ) )
   {
      pti->SpecialGroup.Tasklist.iMargin = rt.left;

      if( rt.top > pti->SpecialGroup.Tasklist.iMargin ) pti->SpecialGroup.Tasklist.iMargin  = rt.top;
      if( rt.right > pti->SpecialGroup.Tasklist.iMargin ) pti->SpecialGroup.Tasklist.iMargin  = rt.right;
      if( rt.bottom > pti->SpecialGroup.Tasklist.iMargin ) pti->SpecialGroup.Tasklist.iMargin  = rt.bottom;
   }
   else
   {
      szValue = GetPropertyValue( pInfo,
                                  "mainsectiontaskss",
                                  "destinationtask",
                                  "margin" );

      if( ConvertTo( szValue, CT_RECT, &rt ) )
      {
         pti->SpecialGroup.Tasklist.iMargin = rt.left;

         if( rt.top > pti->SpecialGroup.Tasklist.iMargin ) pti->SpecialGroup.Tasklist.iMargin  = rt.top;
         if( rt.right > pti->SpecialGroup.Tasklist.iMargin ) pti->SpecialGroup.Tasklist.iMargin  = rt.right;
         if( rt.bottom > pti->SpecialGroup.Tasklist.iMargin ) pti->SpecialGroup.Tasklist.iMargin  = rt.bottom;
      }
      else
         pti->SpecialGroup.Tasklist.iMargin = 4;
   }

   //////////
   // Font Style

   szValue = GetPropertyValue( pInfo,
                               "mainsectiontaskss",
                               "button-mousefocused",
                               "fontstyle" );


   ConvertTo( szValue, CT_FONTSTYLE, &pti->SpecialGroup.Tasklist.lfHot );

   //-------------------------------------------------
   // 3.) Explorer bar information
   //-----

   szValue = GetPropertyValue( pInfo,
                               "taskpane",
                               "element-sectionlist",
                               "background" );

   if( !szValue ) return FALSE;

   if( strnicmp( szValue, "rcbmp", 5 ) == 0 )
   {
      ConvertTo( szValue, CT_BITMAP, &pti->TaskPanel.hBmpBackground, hDll );

      pti->TaskPanel.crBackground1 =
      pti->TaskPanel.crBackground2 =
      pti->TaskPanel.iValue        = 0;
   }
   else
   if( strnicmp( szValue, "gradient", 8 ) == 0 )
   {
      COLORREF crTmp[3];

      if( !ConvertTo( szValue, CT_GRADIENT, crTmp ) )
         return FALSE;

      pti->TaskPanel.crBackground1 =  crTmp [0];
      pti->TaskPanel.crBackground2 =  crTmp [1];
      pti->TaskPanel.iValue        =  (int) crTmp[2];
   }
   else
   {
      if( !ConvertTo( szValue, CT_COLOR, &pti->TaskPanel.crBackground1 ) )
         return FALSE;

      pti->TaskPanel.crBackground2 =
      pti->TaskPanel.iValue        = 0;
   }

   szValue = GetPropertyValue( pInfo,
                               "taskpane",
                               "element-sectionlist",
                               "padding" );

   if( !ConvertTo( szValue, CT_RECT, &pti->TaskPanel.rcPadding ) )
   {
      pti->TaskPanel.rcPadding.left =
      pti->TaskPanel.rcPadding.top =
      pti->TaskPanel.rcPadding.right =
      pti->TaskPanel.rcPadding.bottom = 12;
   }

   szValue = GetPropertyValue( pInfo,
                              "taskpane",
                               "element-sectionlist",
                               "borderthickness" );

   ConvertTo( szValue, CT_RECT, &pti->TaskPanel.rcBorderThickness );

   szValue = GetPropertyValue( pInfo,
                               "taskpane",
                               "element-sectionlist",
                               "bordercolor" );

   ConvertTo( szValue, CT_COLOR, &pti->TaskPanel.crBorder );

   //-------------------------------------------------
   // 4.) normal header information
   //------

   szValue = GetPropertyValue( pInfo,
                               "sectionss",
                               "expando",
                               "fontface" );

   if( !ConvertTo( szValue, CT_STRING, &pti->NormalGroup.Header.lf.lfFaceName, LF_FACESIZE, hDll ) )
      strcpy( pti->NormalGroup.Header.lf.lfFaceName, "Tahoma" );

   strcpy( pti->NormalGroup.Tasklist.lfNormal.lfFaceName, pti->NormalGroup.Header.lf.lfFaceName );
   strcpy( pti->NormalGroup.Tasklist.lfHot.lfFaceName, pti->NormalGroup.Header.lf.lfFaceName );

   szValue = GetPropertyValue( pInfo,
                               "sectionss",
                               "expando",
                               "fontsize" );

   ConvertTo( szValue, CT_INT, &pti->NormalGroup.Header.lf.lfHeight, hDll );

   if( pti->NormalGroup.Header.lf.lfHeight  == 0 )
      pti->NormalGroup.Header.lf.lfHeight = Pt2FontHeight( 8 );

   pti->NormalGroup.Tasklist.lfNormal.lfHeight = pti->NormalGroup.Header.lf.lfHeight;
   pti->NormalGroup.Tasklist.lfHot.lfHeight    = pti->NormalGroup.Header.lf.lfHeight;

   szValue = GetPropertyValue( pInfo,
                               "sectionss",
                               "expando",
                               "margin" );

   if( ConvertTo( szValue, CT_RECT, &rt ) )
   {
      pti->NormalGroup.Header.iMargin = rt.left;

      if( rt.top > pti->NormalGroup.Header.iMargin ) pti->NormalGroup.Header.iMargin  = rt.top;
      if( rt.right > pti->NormalGroup.Header.iMargin ) pti->NormalGroup.Header.iMargin  = rt.right;
      if( rt.bottom > pti->NormalGroup.Header.iMargin ) pti->NormalGroup.Header.iMargin  = rt.bottom;
   }
   else
      pti->NormalGroup.Header.iMargin = 15;

   szValue = GetPropertyValue( pInfo,
                               "sectionss",
                               "button",
                               "foreground" );


   if( !ConvertTo( szValue, CT_COLOR, &pti->NormalGroup.Header.crNormal ) )
      return FALSE;


   szValue = GetPropertyValue( pInfo,
                               "sectionss",
                               "button",
                               "fontweight" );

   if( !ConvertTo( szValue, CT_INT, &pti->NormalGroup.Header.lf.lfWeight, hDll ) )
      pti->NormalGroup.Header.lf.lfWeight = FW_BOLD;


   szValue = GetPropertyValue( pInfo,
                               "sectionss",
                               "button",
                               "padding" );

   ConvertTo( szValue, CT_RECT, &pti->NormalGroup.Header.rcPadding );


   szValue = GetPropertyValue( pInfo,
                               "sectionss",
                               "button",
                               "borderthickness" );

   ConvertTo( szValue, CT_RECT, &pti->NormalGroup.Header.rcBorderThickness );


   szValue = GetPropertyValue( pInfo,
                               "sectionss",
                               "button",
                               "background" );

   ConvertTo( szValue, CT_BITMAP, &pti->NormalGroup.Header.hBitmap, hDll );
   ConvertTo( szValue, CT_BITMAP, &pti->NormalGroup.Header.hBitmapHot, hDll );

   

   szValue = GetPropertyValue( pInfo,
                               "sectionss",
                               "element-title",
                               "contentalign" );

   ConvertTo( szValue, CT_ALIGN, &pti->NormalGroup.Header.uAlign );

   szValue = GetPropertyValue( pInfo,
                               "sectionss",
                               "element-title-mousefocused",
                               "foreground" );

   if( !ConvertTo( szValue, CT_COLOR, &pti->NormalGroup.Header.crHot ) )
      pti->NormalGroup.Header.crHot = pti->NormalGroup.Header.crNormal;


   szValue = GetPropertyValue( pInfo,
                               "sectionss",
                               "element-arrow",
                               "content" );

   ConvertTo( szValue, CT_BITMAP, &pti->NormalGroup.Header.hBitmapArrowDn[0], hDll );


   szValue = GetPropertyValue( pInfo,
                               "sectionss",
                               "element-arrow-selected",
                               "content" );

   ConvertTo( szValue, CT_BITMAP, &pti->NormalGroup.Header.hBitmapArrowUp[0], hDll );


   szValue = GetPropertyValue( pInfo,
                               "sectionss",
                               "element-arrow-mousefocused",
                               "content" );

   ConvertTo( szValue, CT_BITMAP, &pti->NormalGroup.Header.hBitmapArrowDn[1], hDll );


   szValue = GetPropertyValue( pInfo,
                               "sectionss",
                               "element-arrow-selected-mousefocused",
                               "content" );

   ConvertTo( szValue, CT_BITMAP, &pti->NormalGroup.Header.hBitmapArrowUp[1], hDll );


   szValue = GetPropertyValue( pInfo,
                               "sectionss",
                               "tasklist-tasklist",
                               "background" );

   if( !ConvertTo( szValue, CT_COLOR, &pti->NormalGroup.Tasklist.crBackground1 ) )
      return FALSE;

   pti->NormalGroup.Tasklist.crBackground2 = pti->NormalGroup.Tasklist.crBackground1;

   szValue = GetPropertyValue( pInfo,
                               "sectionss",
                               "tasklist-tasklist",
                               "borderthickness" );

   if( !ConvertTo( szValue, CT_RECT, &pti->NormalGroup.Tasklist.rcBorderThickness ) )
   {
      pti->NormalGroup.Tasklist.rcBorderThickness.left   = 1;
      pti->NormalGroup.Tasklist.rcBorderThickness.right  = 1;
      pti->NormalGroup.Tasklist.rcBorderThickness.top    = 1;
      pti->NormalGroup.Tasklist.rcBorderThickness.bottom = 1;
   }


   szValue = GetPropertyValue( pInfo,
                               "sectionss",
                               "tasklist-tasklist",
                               "padding" );

   if( !ConvertTo( szValue, CT_RECT, &pti->NormalGroup.Tasklist.rcPadding ) )
   {
      pti->NormalGroup.Tasklist.rcPadding.left   =
      pti->NormalGroup.Tasklist.rcPadding.right  = 12;
      pti->NormalGroup.Tasklist.rcPadding.top    =
      pti->NormalGroup.Tasklist.rcPadding.bottom = 10;
   }

   szValue = GetPropertyValue( pInfo,
                               "sectionss",
                               "tasklist-tasklist",
                               "bordercolor" );

   if( !ConvertTo( szValue, CT_COLOR, &pti->NormalGroup.Tasklist.crBorder ) )
      pti->NormalGroup.Tasklist.crBorder = 0xFFFFFF;

   szValue = GetPropertyValue( pInfo,
                               "NameSpaceItemInfoList",
                               "NameSpaceItemInfo",
                               "background" );

   ConvertTo( szValue, CT_COLOR, &pti->NormalGroup.Tasklist.Info.crBackground1 );

   pti->NormalGroup.Tasklist.Info.crBackground2 = pti->NormalGroup.Tasklist.Info.crBackground1;

   szValue = GetPropertyValue( pInfo,
                               "NameSpaceItemInfoList",
                               "NameSpaceItemInfo",
                               "foreground" );

   ConvertTo( szValue, CT_COLOR, &pti->NormalGroup.Tasklist.Info.crNormal );

   //-------------------------------------------------
   // 5.) special header information
   //-----

   szValue = GetPropertyValue( pInfo,
                               "mainsectionss",
                               "expando",
                               "fontface" );

   if( !ConvertTo( szValue, CT_STRING, &pti->SpecialGroup.Header.lf.lfFaceName, LF_FACESIZE, hDll ) )
      strcpy( pti->SpecialGroup.Header.lf.lfFaceName, "Tahoma" );

   strcpy( pti->SpecialGroup.Tasklist.lfNormal.lfFaceName, pti->SpecialGroup.Header.lf.lfFaceName );
   strcpy( pti->SpecialGroup.Tasklist.lfHot.lfFaceName, pti->SpecialGroup.Header.lf.lfFaceName );

   szValue = GetPropertyValue( pInfo,
                               "mainsectionss",
                               "expando",
                               "fontsize" );

   ConvertTo( szValue, CT_INT, &pti->SpecialGroup.Header.lf.lfHeight, hDll );

   if( pti->SpecialGroup.Header.lf.lfHeight  == 0 )
      pti->SpecialGroup.Header.lf.lfHeight = Pt2FontHeight( 8 );

   pti->SpecialGroup.Tasklist.lfNormal.lfHeight = pti->SpecialGroup.Header.lf.lfHeight;
   pti->SpecialGroup.Tasklist.lfHot.lfHeight = pti->SpecialGroup.Header.lf.lfHeight;

   /*
   szValue = GetPropertyValue( pInfo,
                               "mainsectionss",
                               "expando",
                               "margin" );

   if( ConvertTo( szValue, CT_RECT, &rt ) )
   {
      pti->SpecialGroup.Header.iMargin = rt.left;

      if( rt.top > pti->SpecialGroup.Header.iMargin ) pti->SpecialGroup.Header.iMargin  = rt.top;
      if( rt.right > pti->SpecialGroup.Header.iMargin ) pti->SpecialGroup.Header.iMargin  = rt.right;
      if( rt.bottom > pti->SpecialGroup.Header.iMargin ) pti->SpecialGroup.Header.iMargin  = rt.bottom;
   }
   else
      pti->SpecialGroup.Header.iMargin = 15;
   */
   pti->SpecialGroup.Header.iMargin = pti->NormalGroup.Header.iMargin;

   szValue = GetPropertyValue( pInfo,
                               "mainsectionss",
                               "button-header",
                               "foreground" );


   if( !ConvertTo( szValue, CT_COLOR, &pti->SpecialGroup.Header.crNormal ) )
      return FALSE;


   szValue = GetPropertyValue( pInfo,
                               "mainsectionss",
                               "button-header",
                               "fontweight" );

   if( !ConvertTo( szValue, CT_INT, &pti->SpecialGroup.Header.lf.lfWeight, hDll ) )
      pti->SpecialGroup.Header.lf.lfWeight = FW_BOLD;

   szValue = GetPropertyValue( pInfo,
                               "mainsectionss",
                               "button-header",
                               "padding" );

   ConvertTo( szValue, CT_RECT, &pti->SpecialGroup.Header.rcPadding );


   szValue = GetPropertyValue( pInfo,
                               "mainsectionss",
                               "button-header",
                               "borderthickness" );

   ConvertTo( szValue, CT_RECT, &pti->SpecialGroup.Header.rcBorderThickness );


   szValue = GetPropertyValue( pInfo,
                               "mainsectionss",
                               "button-header",
                               "background" );

   ConvertTo( szValue, CT_BITMAP, &pti->SpecialGroup.Header.hBitmap, hDll );
   ConvertTo( szValue, CT_BITMAP, &pti->SpecialGroup.Header.hBitmapHot, hDll );   

   szValue = GetPropertyValue( pInfo,
                               "sectionss",
                               "element-title",
                               "contentalign" );

   ConvertTo( szValue, CT_ALIGN, &pti->SpecialGroup.Header.uAlign );

   szValue = GetPropertyValue( pInfo,
                               "mainsectionss",
                               "element-title-mousefocused",
                               "foreground" );

   if( !ConvertTo( szValue, CT_COLOR, &pti->SpecialGroup.Header.crHot ) )
      pti->SpecialGroup.Header.crHot = pti->SpecialGroup.Header.crNormal;


   szValue = GetPropertyValue( pInfo,
                               "mainsectionss",
                               "element-arrow",
                               "content" );

   ConvertTo( szValue, CT_BITMAP, &pti->SpecialGroup.Header.hBitmapArrowDn[0], hDll );


   szValue = GetPropertyValue( pInfo,
                               "mainsectionss",
                               "element-arrow-selected",
                               "content" );

   ConvertTo( szValue, CT_BITMAP, &pti->SpecialGroup.Header.hBitmapArrowUp[0], hDll );


   szValue = GetPropertyValue( pInfo,
                               "mainsectionss",
                               "element-arrow-mousefocused",
                               "content" );

   ConvertTo( szValue, CT_BITMAP, &pti->SpecialGroup.Header.hBitmapArrowDn[1], hDll );


   szValue = GetPropertyValue( pInfo,
                               "mainsectionss",
                               "element-arrow-selected-mousefocused",
                               "content" );

   ConvertTo( szValue, CT_BITMAP, &pti->SpecialGroup.Header.hBitmapArrowUp[1], hDll );


   szValue = GetPropertyValue( pInfo,
                               "mainsectionss",
                               "element-watermark",
                               "background" );

   if( !ConvertTo( szValue, CT_COLOR, &pti->SpecialGroup.Tasklist.crBackground1 ) )
      return FALSE;

   pti->SpecialGroup.Tasklist.crBackground2 = pti->SpecialGroup.Tasklist.crBackground1;

   szValue = GetPropertyValue( pInfo,
                               "mainsectionss",
                               "tasklist-tasklist",
                               "borderthickness" );

   if( !ConvertTo( szValue, CT_RECT, &pti->SpecialGroup.Tasklist.rcBorderThickness ) )
   {
      pti->SpecialGroup.Tasklist.rcBorderThickness.left   = 1;
      pti->SpecialGroup.Tasklist.rcBorderThickness.right  = 1;
      pti->SpecialGroup.Tasklist.rcBorderThickness.top    = 1;
      pti->SpecialGroup.Tasklist.rcBorderThickness.bottom = 1;
   }


   szValue = GetPropertyValue( pInfo,
                               "mainsectionss",
                               "tasklist-tasklist",
                               "padding" );

   if( !ConvertTo( szValue, CT_RECT, &pti->SpecialGroup.Tasklist.rcPadding ) )
   {
      pti->SpecialGroup.Tasklist.rcPadding.left   =
      pti->SpecialGroup.Tasklist.rcPadding.right  = 12;
      pti->SpecialGroup.Tasklist.rcPadding.top    =
      pti->SpecialGroup.Tasklist.rcPadding.bottom = 10;
   }

   szValue = GetPropertyValue( pInfo,
                               "mainsectionss",
                               "tasklist-tasklist",
                               "bordercolor" );

   if( !ConvertTo( szValue, CT_COLOR, &pti->SpecialGroup.Tasklist.crBorder ) )
      pti->SpecialGroup.Tasklist.crBorder = 0xFFFFFF;

   szValue = GetPropertyValue( pInfo,
                               "NameSpaceItemInfoList",
                               "NameSpaceItemInfo",
                               "background" );

   ConvertTo( szValue, CT_COLOR, &pti->SpecialGroup.Tasklist.Info.crBackground1 );

   pti->SpecialGroup.Tasklist.Info.crBackground2 = pti->SpecialGroup.Tasklist.Info.crBackground1;

   szValue = GetPropertyValue( pInfo,
                               "NameSpaceItemInfoList",
                               "NameSpaceItemInfo",
                               "foreground" );

   ConvertTo( szValue, CT_COLOR, &pti->SpecialGroup.Tasklist.Info.crNormal );

   return TRUE;
}

//----------------------------------------------------------------------------//

extern char wtc_NormalArrowUp[];
extern char wtc_NormalArrowUpHot[];
extern char wtc_NormalArrowDn[];
extern char wtc_NormalArrowDnHot[];
extern char wtc_SpecialArrowUp[];
extern char wtc_SpecialArrowUpHot[];
extern char wtc_SpecialArrowDn[];
extern char wtc_SpecialArrowDnHot[];

static void LoadInternalTheme( PTHEMEINFO pti )
{
   LOGFONT lf;

   memset( &lf, 0, sizeof(LOGFONT) );

   if( !InitThemeInfo( pti->szShellStyleDllName, pti) )
   {
      //////////////////////////
      // GUI Font
      //////////
      if( !GetObject((HFONT)GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf) )
      {
         strcpy( lf.lfFaceName, "Arial" );
         lf.lfHeight = -13;
      }

      if (strcmp( lf.lfFaceName, "MS Shell Dlg" )== 0)
      {
          strcpy( lf.lfFaceName, "Microsoft Sans Serif");
      }

      //////////////////////////
      // TaskPanel
      //////////

      pti->TaskPanel.crBackground1     = GetSysColor(COLOR_WINDOW);
      pti->TaskPanel.crBackground2     = GetSysColor(COLOR_WINDOW);
      pti->TaskPanel.iValue            = 0;

      pti->TaskPanel.rcPadding.left    =
      pti->TaskPanel.rcPadding.top     =
      pti->TaskPanel.rcPadding.right   =
      pti->TaskPanel.rcPadding.bottom  = 12;

      //////////////////////////
      // NormalGroup - TaskList
      //////////

      memcpy( &pti->NormalGroup.Tasklist.lfNormal, &lf, sizeof(LOGFONT) );
      memcpy( &pti->NormalGroup.Tasklist.lfHot, &lf , sizeof(LOGFONT) );

      pti->NormalGroup.Tasklist.lfHot.lfUnderline = TRUE;

      pti->NormalGroup.Tasklist.crBackground1 =
      pti->NormalGroup.Tasklist.crBackground2 = GetSysColor(COLOR_WINDOW);
      pti->NormalGroup.Tasklist.crBorder      = GetSysColor(COLOR_3DFACE);

      pti->NormalGroup.Tasklist.rcBorderThickness.left    =
      pti->NormalGroup.Tasklist.rcBorderThickness.right   =
      pti->NormalGroup.Tasklist.rcBorderThickness.bottom  = 1;
      pti->NormalGroup.Tasklist.rcBorderThickness.top     = 0;

      pti->NormalGroup.Tasklist.rcPadding.left   =
      pti->NormalGroup.Tasklist.rcPadding.right  = 12;
      pti->NormalGroup.Tasklist.rcPadding.top    =
      pti->NormalGroup.Tasklist.rcPadding.bottom = 10;

      pti->NormalGroup.Tasklist.iMargin          = 4;

      pti->NormalGroup.Tasklist.Link.crNormal  = GetSysColor( COLOR_BTNTEXT );
      pti->NormalGroup.Tasklist.Link.crHot     = GetSysColor( COLOR_HOTLIGHT );

      pti->NormalGroup.Tasklist.Link.rcPadding.left   = 6;
      pti->NormalGroup.Tasklist.Link.rcPadding.top    = 0;
      pti->NormalGroup.Tasklist.Link.rcPadding.right  = 4;
      pti->NormalGroup.Tasklist.Link.rcPadding.bottom = 0;


      //////////////////////////
      // NormalGroup - Header
      //////////

      memcpy( &pti->NormalGroup.Header.lf, &lf, sizeof(LOGFONT ) );

      pti->NormalGroup.Header.lf.lfWeight      = FW_BOLD;
      pti->NormalGroup.Header.iMargin          = 15;
      pti->NormalGroup.Header.crBorder         =
      pti->NormalGroup.Header.crBackground1    =
      pti->NormalGroup.Header.crBackground2    = 
      pti->NormalGroup.Header.crBackgroundHot1 = 
      pti->NormalGroup.Header.crBackgroundHot2 = GetSysColor(COLOR_3DFACE);
      pti->NormalGroup.Header.crNormal         = GetSysColor(COLOR_BTNTEXT);
      pti->NormalGroup.Header.crHot            = GetSysColor(COLOR_BTNTEXT);

      pti->NormalGroup.Header.rcPadding.left   = 10;
      pti->NormalGroup.Header.rcPadding.top    =  0;
      pti->NormalGroup.Header.rcPadding.right  =  1;
      pti->NormalGroup.Header.rcPadding.bottom =  0;

      pti->NormalGroup.Header.rcBorderThickness.left   =
      pti->NormalGroup.Header.rcBorderThickness.top    =
      pti->NormalGroup.Header.rcBorderThickness.right  = 2;
      pti->NormalGroup.Header.rcBorderThickness.bottom = 0;

      pti->NormalGroup.Header.hBitmap = 0; //No Header Bitmap
      pti->NormalGroup.Header.hBitmapArrowUp[ 0 ] = CreateMemDIBitmapT( wtc_NormalArrowUp,     356, 0xFF00FF );
      pti->NormalGroup.Header.hBitmapArrowUp[ 1 ] = CreateMemDIBitmapT( wtc_NormalArrowUpHot,  356, 0xFF00FF );
      pti->NormalGroup.Header.hBitmapArrowDn[ 0 ] = CreateMemDIBitmapT( wtc_NormalArrowDn,     356, 0xFF00FF );
      pti->NormalGroup.Header.hBitmapArrowDn[ 1 ] = CreateMemDIBitmapT( wtc_NormalArrowDnHot,  356, 0xFF00FF );

      //////////////////////////
      // SpecialGroup <-- NormalGroup
      //////////

      memcpy( &pti->SpecialGroup, &pti->NormalGroup, sizeof(TPGROUP) );

      //////////////////////////
      // SpecialGroup - Header
      //////////

      pti->SpecialGroup.Header.crBorder         =
      pti->SpecialGroup.Header.crBackground1    =
      pti->SpecialGroup.Header.crBackground2    =
      pti->SpecialGroup.Header.crBackgroundHot1 =
      pti->SpecialGroup.Header.crBackgroundHot2 = GetSysColor(COLOR_HIGHLIGHT);
      pti->SpecialGroup.Header.crNormal         = GetSysColor(COLOR_HIGHLIGHTTEXT);
      pti->SpecialGroup.Header.crHot            = GetSysColor(COLOR_HIGHLIGHTTEXT);

      pti->SpecialGroup.Header.hBitmapArrowUp[ 0 ] = CreateMemDIBitmapT( wtc_SpecialArrowUp,     356, 0xFF00FF );
      pti->SpecialGroup.Header.hBitmapArrowUp[ 1 ] = CreateMemDIBitmapT( wtc_SpecialArrowUpHot,  356, 0xFF00FF );
      pti->SpecialGroup.Header.hBitmapArrowDn[ 0 ] = CreateMemDIBitmapT( wtc_SpecialArrowDn,     356, 0xFF00FF );
      pti->SpecialGroup.Header.hBitmapArrowDn[ 1 ] = CreateMemDIBitmapT( wtc_SpecialArrowDnHot,  356, 0xFF00FF );

   }

   ThemeConfigure( pti );
}

//----------------------------------------------------------------------------//


//----------------------------------------------------------------------------//

static void ThemeConfigure( PTHEMEINFO pti )
{

   //-----------------------------------
   // Fuente Encabezado Normal
   //------

   memcpy( &pti->NormalGroup.Header.lfHot, &pti->NormalGroup.Header.lf, sizeof(LOGFONT) );

   pti->NormalGroup.Header.hFontNormal   = CreateFontIndirect( &pti->NormalGroup.Header.lf );
   pti->NormalGroup.Header.hFontHot      = CreateFontIndirect( &pti->NormalGroup.Header.lfHot );

   //-----------------------------------
   // Fuente Panel de Tareas Normal
   //-----

   pti->NormalGroup.Tasklist.hFontNormal        = CreateFontIndirect( &pti->NormalGroup.Tasklist.lfNormal );
   pti->NormalGroup.Tasklist.hFontHot           = CreateFontIndirect( &pti->NormalGroup.Tasklist.lfHot );

   //-----------------------------------
   // Fuente Encabezado Special
   //------

   memcpy( &pti->SpecialGroup.Header.lfHot, &pti->SpecialGroup.Header.lf, sizeof(LOGFONT) );

   pti->SpecialGroup.Header.hFontNormal   = CreateFontIndirect( &pti->SpecialGroup.Header.lf );
   pti->SpecialGroup.Header.hFontHot      = CreateFontIndirect( &pti->SpecialGroup.Header.lfHot );

   //-----------------------------------
   // Fuente Panel de Tareas Special
   //-----

   pti->SpecialGroup.Tasklist.hFontNormal        = CreateFontIndirect( &pti->SpecialGroup.Tasklist.lfNormal );
   pti->SpecialGroup.Tasklist.hFontHot           = CreateFontIndirect( &pti->SpecialGroup.Tasklist.lfHot );

}

//----------------------------------------------------------------------------//
// EOF
//------//
