/**************************************************************************************************
* Project      : TTaskpanel
* File         : c_DibBmp.c
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

#define MIN3(a,b,c) ( (a)<=(b) ? (a)<=(c)?(a):(c) : (b)<=(c)?(b):(c) )
#define MAX3(a,b,c) ( (a)>=(b) ? (a)>=(c)?(a):(c) : (b)>=(c)?(b):(c) )
#define MakeRGB(r,g,b) (((r)<<16) | ((g)<<8) | (b))
#define XCHGCOLORREF(_x)              ((((_x)&0xFF00FF00L)|(((_x)&0x00FF0000L)>>16)|(((_x)&0x000000FFL)<<16)))

//----------------------------------------------------------------------------//

void DibBmp_Init( PDIBBMP db, HBITMAP hBitmap, BOOL bHasAlpha )
{
    memset( db, 0, sizeof(DIBBMP) );
    db->hBitmap   = hBitmap;
    db->bHasAlpha = bHasAlpha;
}

//----------------------------------------------------------------------------//

void DibBmp_Init2( PDIBBMP db, HBITMAP hBitmap, BOOL bHasAlpha )
{
    memset( db, 0, sizeof(DIBBMP) );
    DibBmp_Duplicate( db, hBitmap );
    db->bHasAlpha = bHasAlpha;
}

//----------------------------------------------------------------------------//

BOOL DibBmp_LoadFromResource( PDIBBMP db, HINSTANCE hInstance, const char * szResName )
{
    BOOL  bReturn  = FALSE;
    BOOL  bCheck   = TRUE;
    HRSRC hRes;

    //Inicializamos la estructura
    memset( db, 0, sizeof( DIBBMP ) );

    hRes = FindResource( hInstance, szResName, RT_BITMAP );

    if( !hRes )
    {
        hRes   = FindResource( hInstance, szResName, "BITMAP32" );
        bCheck = FALSE;
    }

    if( hRes )
    {
        ULONG ulSize    = SizeofResource( hInstance, hRes );
        HGLOBAL hGlobal = LoadResource( hInstance, hRes );

        if( hGlobal )
        {
            if( !bCheck )
            {
                BITMAPFILEHEADER *bf = (BITMAPFILEHEADER*) hGlobal;

                if( bf->bfType == 0x4d42 ) // "BM"
                    bReturn = DibBmp_CreateFromMem( db, &bf[1], ulSize - sizeof(BITMAPFILEHEADER) );
            }
            else
                bReturn = DibBmp_CreateFromMem( db, hGlobal, ulSize );
        }
    }

    return bReturn;
}
//----------------------------------------------------------------------------//

BOOL DibBmp_LoadFromFile( PDIBBMP db,  const char * szFileName )
{
    BOOL bReturn = FALSE;
    int iHandle  = hb_fsOpen( szFileName , FO_READ );

    //Inicializamos la estructura
    memset( db, 0, sizeof( DIBBMP ) );

    if( iHandle )
    {
        ULONG   ulSize = (ULONG) hb_fsSeekLarge( iHandle, 0, FS_END ) ;

        if( ulSize > sizeof(BITMAPFILEHEADER) )
        {
            HGLOBAL hGlobal;

            ulSize -= sizeof(BITMAPFILEHEADER);
            hGlobal = GlobalAlloc( GMEM_FIXED, ulSize  );

            if( hGlobal )
            {
                BITMAPFILEHEADER bf;

                hb_fsSeek( iHandle, 0, FS_SET );

                if( hb_fsReadLarge( iHandle, &bf, sizeof(BITMAPFILEHEADER) ) == sizeof(BITMAPFILEHEADER) )
                {
                    if( bf.bfType == 0x4d42 ) // "BM"
                    {
                        if( hb_fsReadLarge( iHandle, hGlobal, ulSize ) == ulSize )
                            bReturn = DibBmp_CreateFromMem( db, hGlobal, ulSize );
                    }
                }
            }
            GlobalFree( hGlobal );
        }
        hb_fsClose( iHandle );
    }

    return bReturn;
}

//----------------------------------------------------------------------------//

BOOL DibBmp_Duplicate( PDIBBMP db, HBITMAP hBitmap )
{
    BOOL lRet = FALSE;

    if( hBitmap )
    {
        BITMAPINFO  bmInfo;
        BITMAP      bm;
        HDC         hdc;
        LPVOID      pvBits;

        GetObject( hBitmap, sizeof(BITMAP), &bm );

        bmInfo.bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
        bmInfo.bmiHeader.biWidth         = bm.bmWidth;
        bmInfo.bmiHeader.biHeight        = bm.bmHeight;
        bmInfo.bmiHeader.biPlanes        = 1;
        bmInfo.bmiHeader.biBitCount      = 32;
        bmInfo.bmiHeader.biCompression   = BI_RGB;

        hdc = GetDC( NULL );

        db->hBitmap = CreateDIBSection( hdc, &bmInfo, DIB_RGB_COLORS, &pvBits, NULL, 0 );

        if( db->hBitmap )
        {
            HDC     hdcsrc    = CreateCompatibleDC( hdc );
            HDC     hdcdst    = CreateCompatibleDC( hdc );
            HBITMAP hOldBmp1  = SelectObject( hdcdst, db->hBitmap );
            HBITMAP hOldBmp2  = SelectObject( hdcsrc, hBitmap );

            BitBlt(hdcdst, 0, 0, bm.bmWidth, bm.bmHeight, hdcsrc, 0, 0, SRCCOPY);

            SelectObject( hdcdst, hOldBmp1 );
            SelectObject( hdcsrc, hOldBmp2 );
            DeleteDC( hdcdst );
            DeleteDC( hdcsrc );

            db->wBitCount = bm.bmBitsPixel;
            lRet = TRUE;
        }

        ReleaseDC( NULL, hdc );
    }

    return lRet;
}

//----------------------------------------------------------------------------//

BOOL DibBmp_CreateFromMem( PDIBBMP db, void * pvMem, ULONG ulSize )
{
    BITMAPINFOHEADER  bi;
    BOOL              bSizeOk;
    PDWORD            pdwColorTable = NULL;
    ULONG             lWidthBytes;
    char *            szBytes;

    if( !db || !pvMem || ulSize == 0 )
        return FALSE;

    //Inicializamos la estructura
    memset( db, 0, sizeof( DIBBMP ) );

    memcpy( &bi, pvMem, sizeof(BITMAPINFOHEADER) );

    //Checamos Tamaño y compresion
    if( bi.biSize == sizeof(BITMAPINFOHEADER) && bi.biCompression == BI_RGB )
    {

        ULONG ulBufferSize;

        //Revisamos que el tamaño de nuestro buffer sea correcto
        //Obtenemos los Bytes por Renglon

        switch( bi.biBitCount )
        {
            case 1:
                lWidthBytes = bi.biWidth >> 3 ;
                break;
            case 4:
                lWidthBytes = bi.biWidth >> 1;
                break;
            case 8:
                lWidthBytes = bi.biWidth;
                break;
            case 16:
                lWidthBytes = bi.biWidth << 1;
                break;
            case 24:
                lWidthBytes = (bi.biWidth << 1) + bi.biWidth;
                break;
            case 32:
                lWidthBytes = bi.biWidth << 2;
                break;
            default:
                return FALSE;
        }

        bSizeOk       = TRUE;
        lWidthBytes   = (lWidthBytes + 3) &~ 3;
        ulBufferSize  = (lWidthBytes * bi.biHeight) + sizeof(BITMAPINFOHEADER);

        if( bi.biBitCount < 16 )
        {
            //Checamos tambien la tabla de colores

            if( bi.biClrUsed == 0 )
            {
                int iDiff = ulSize - ulBufferSize;

                if( iDiff > 0 )
                {
                    if( !(iDiff & 0x03) )
                    {
                        bi.biClrUsed = iDiff >> 2;

                        if( bi.biClrUsed > 256 )
                            bi.biClrUsed = 0;
                    }
                }
            }


            if( bi.biClrUsed > 0 )
            {
                ulBufferSize += bi.biClrUsed << 2;

                pdwColorTable   = (PDWORD) (((char *) pvMem) + sizeof(BITMAPINFOHEADER));
                szBytes         = (char *) &pdwColorTable[ bi.biClrUsed ];

                db->ColorTable = (PDWORD) GlobalAlloc( GMEM_FIXED, bi.biClrUsed << 2 );

                if( db->ColorTable )
                {
                    db->wClrUsed = (WORD) bi.biClrUsed;
                    memcpy( db->ColorTable, pdwColorTable, bi.biClrUsed << 2 );
                }
                else
                    bSizeOk = FALSE;
            }
            else
                bSizeOk = FALSE;

        }
        else
            szBytes = ((char *) pvMem) + sizeof(BITMAPINFOHEADER);


        //bSizeOk = ulBufferSize <= ulSize;
    }
    else
        return FALSE;


    //Empezamos la Conversion
    if( bSizeOk )
    {
        ULONG  ulImageSize = (bi.biWidth * bi.biHeight) << 2;
        char * szImage     = (char *) GlobalAlloc( GMEM_FIXED,  ulImageSize );

        if( szImage )
        {
            int  i, j;
            WORD BlueMask    = 0x001F;
            WORD GreenMask   = 0x03E0;
            WORD RedMask     = 0x7C00;
            int  BlueShift   = 0;
            int  GreenShift  = 0;
            int  RedShift    = 0;

            if( bi.biBitCount == 16 )
            {
                WORD wTmp;

                wTmp = BlueMask;
                while( wTmp > 31 )
                { wTmp = (WORD)(wTmp >> 1); BlueShift++; }

                wTmp = GreenMask;
                while( wTmp > 31 )
                { wTmp = (WORD)(wTmp >> 1); GreenShift++; }

                wTmp = RedMask;
                while( wTmp > 31 )
                { wTmp = (WORD)(wTmp >> 1); RedShift++; }

            }

            memset( szImage, 0, ulImageSize );

            for( i = 0; i < bi.biHeight; i++ )
            {
                BYTE    *pSrc = (BYTE*)szBytes + (lWidthBytes * i);
                DWORD32 *pDst = (DWORD32*) (szImage + ((bi.biWidth << 2 ) * i));

                for( j = 0; j < bi.biWidth; j++ )
                {

                    switch( bi.biBitCount )
                    {
                        case 1:
                            {
                                BYTE  byIndex = pSrc[ j >> 3 ];
                                BYTE  shift   = (BYTE)(7 - (j & 0x07));
                                BYTE  mask    = (BYTE)(1 << shift);

                                byIndex = (BYTE)((byIndex & mask) >> shift);

                                memcpy( &pDst[j], &pdwColorTable[ byIndex ], 4 );
                                break;

                            }
                        case 4:
                            {
                                BYTE  byIndex = pSrc[ j >> 1 ];

                                if( j & 0x01 )
                                    byIndex = (BYTE) (byIndex & 0x0F);
                                else
                                    byIndex = (BYTE) ((byIndex & 0xF0) >> 4);

                                memcpy( &pDst[j], &pdwColorTable[ byIndex ], 4 );
                                break;
                            }
                        case 8:
                            memcpy( &pDst[j], &pdwColorTable[ pSrc[j] ], 4 );
                            break;
                        case 16:
                            {
                                BYTE color[3];
                                WORD wColor = *((WORD * )&pSrc[ j << 1 ]);

                                color[0] = (BYTE) (((wColor & BlueMask) >> BlueShift ) << 3);
                                color[1] = (BYTE) (((wColor & GreenMask) >> GreenShift) << 3);
                                color[2] = (BYTE) (((wColor & RedMask) >> RedShift) << 3) ;

                                memcpy( &pDst[j], color, 3 );
                                break;
                            }
                        case 24:
                            memcpy( &pDst[j], &pSrc[ (j << 1) + j], 3 );
                            break;
                        case 32:
                            memcpy( &pDst[j], &pSrc[ j << 2 ], 4 );

                            if( pDst[j] & 0xFF000000 )
                                db->bHasAlpha = TRUE;
                            break;
                    }
                }
            }

            //Creamos Nuestro Bitmap
            {
                BITMAPINFO  bmInfo;
                HDC         hdc;
                HBITMAP     hBitmap;
                LPVOID      pvBits;

                bmInfo.bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
                bmInfo.bmiHeader.biWidth         = bi.biWidth;
                bmInfo.bmiHeader.biHeight        = bi.biHeight;
                bmInfo.bmiHeader.biPlanes        = 1;
                bmInfo.bmiHeader.biBitCount      = 32;
                bmInfo.bmiHeader.biCompression   = BI_RGB;

                hdc= GetDC( NULL );

                hBitmap = CreateDIBSection( hdc, &bmInfo, DIB_RGB_COLORS, &pvBits, NULL, 0 );

                if( hBitmap )
                {
                    memcpy( pvBits, szImage, (bi.biWidth * bi.biHeight) << 2 );

                    db->hBitmap   = hBitmap;
                    db->wBitCount = bi.biBitCount;
                }
                else
                    DibBmp_Release( db );

                ReleaseDC( NULL, hdc );

            }
        }

        GlobalFree( (HGLOBAL) szImage );
    }


    return (BOOL) db->hBitmap;
}

//----------------------------------------------------------------------------//

#define STC_INDEX           1
#define STC_FIRSTPIXEL      2

BOOL DibBmp_SetTransparentColor( PDIBBMP db, DWORD dwTransColor )
{
    return DibBmp_SetTransparentColor2( db, dwTransColor, 0 );
}

//----------------------------------------------------------------------------//

BOOL DibBmp_SetTransparentColorIndex( PDIBBMP db, BYTE byIndex )
{
    return DibBmp_SetTransparentColor2( db, (DWORD) byIndex, STC_INDEX );
}

//----------------------------------------------------------------------------//

BOOL DibBmp_SetTransparentColorFirstPixel( PDIBBMP db  )
{
    return DibBmp_SetTransparentColor2( db, 0, STC_FIRSTPIXEL );
}

//----------------------------------------------------------------------------//

BOOL DibBmp_SetTransparentColor2( PDIBBMP db, DWORD dwTransColor, WORD wType  )
{
   DIBSECTION  ds;

    if( !db )
        return FALSE;

    if ( !( sizeof(DIBSECTION) == GetObject( db->hBitmap, sizeof(DIBSECTION),(LPVOID)&ds)))
        return FALSE;

    if ( ds.dsBmih.biBitCount == 32 )
    {
        DWORD    *dwBits;
        int     i, j;
        DWORD   dwColor;

        dwBits = (DWORD*) ds.dsBm.bmBits;


        if( wType == STC_FIRSTPIXEL )
            dwTransColor = (*(dwBits + ( (ds.dsBm.bmHeight-1) * ds.dsBm.bmWidth ) )) & 0x00FFFFFF;
        else if ( wType == STC_INDEX )
        {
            if( db->ColorTable && (WORD)dwTransColor < db->wClrUsed )
                dwTransColor = db->ColorTable[ dwTransColor ];
            else
                return FALSE;
        }
        else
            dwTransColor = XCHGCOLORREF( dwTransColor );

        for ( i = 0 ; i < ds.dsBm.bmHeight ; i++ )
        {
            for ( j = 0 ; j < ds.dsBm.bmWidth ; j++ )
            {
                dwColor = (*(dwBits +  j ));

                if( (dwColor & 0x00FFFFFF) == dwTransColor )
                    dwColor = 0;
                else if( !db->bHasAlpha )
                    dwColor = dwColor | 0xFF000000;

                *(dwBits +  j ) = dwColor;
            }

            dwBits += ds.dsBm.bmWidth;
        }

        db->bHasAlpha = TRUE;

        return TRUE;
    }

    return FALSE;
}

//----------------------------------------------------------------------------//

BOOL DibBmp_PreMultiplyAlpha( PDIBBMP db )
{

   DIBSECTION  ds;

    if( !db )
        return FALSE;

    if ( !( sizeof(DIBSECTION) == GetObject( db->hBitmap, sizeof(DIBSECTION),(LPVOID)&ds)))
        return FALSE;

    if ( ds.dsBmih.biBitCount == 32 && db->bHasAlpha )
    {
        DWORD    *dwBits;
        int     i, j;
        DWORD   dwColor;
        BYTE    byAlpha, byRed, byGreen, byBlue;

        dwBits = (DWORD*) ds.dsBm.bmBits;

        for ( i = 0 ; i < ds.dsBm.bmHeight ; i++ )
        {
            for ( j = 0 ; j < ds.dsBm.bmWidth ; j++ )
            {
                dwColor = (*(dwBits +  j ));

                byAlpha = (BYTE)(dwColor >> 24);
                byRed   = (BYTE)(dwColor >> 16);
                byGreen = (BYTE)(dwColor >> 8);
                byBlue  = (BYTE)(dwColor >> 0);

                byRed   = (BYTE) (((DWORD)byRed)   * ((DWORD)byAlpha) / 255);
                byGreen = (BYTE) (((DWORD)byGreen) * ((DWORD)byAlpha) / 255);
                byBlue  = (BYTE) (((DWORD)byBlue)  * ((DWORD)byAlpha) / 255);

                dwColor = (((DWORD)byAlpha) << 24)|
                          (((DWORD)byRed)   << 16)|
                          (((DWORD)byGreen) << 8) |
                          (((DWORD)byBlue)  << 0);

                *(dwBits +  j ) = dwColor;
            }

            dwBits += ds.dsBm.bmWidth;
        }

        return TRUE;
    }

    return FALSE;
}

//----------------------------------------------------------------------------//

#ifdef _FILTER_WATERMARK

void RGBtoHLS(	const COLORREF rgb, double *H, double *L, double *S )
{
	double delta;
	double r = (double)((rgb>>16)&0xFF)/255;
	double g = (double)((rgb>> 8)&0xFF)/255;
	double b = (double)((rgb    )&0xFF)/255;
	double cmax = MAX3(r,g,b);
	double cmin = MIN3(r,g,b);

	*L=(cmax+cmin)/2.0;

	if(cmax==cmin)
        *S = *H = 0;
	else
	{
		if(*L < 0.5)
            *S = (cmax-cmin)/(cmax+cmin);
		else
            *S = (cmax-cmin)/(2.0-cmax-cmin);
		delta = cmax - cmin;

		if (r==cmax)
            *H = (g-b)/delta;
		else
			if(g==cmax)
                *H = 2.0 +(b-r)/delta;
	  		else
                *H = 4.0+(r-g)/delta;

	  	*H /= 6.0;

        if (*H < 0.0)
            *H += 1;
	}
}

static double HueToRGB(const double m1, const double m2, double h )
{
	if (h<0)
        h+=1.0;
	if (h>1)
        h-=1.0;
	if (6.0*h < 1  )
        return (m1+(m2-m1)*h*6.0);
	if (2.0*h < 1  )
        return m2;
	if (3.0*h < 2.0)
        return (m1+(m2-m1)*((2.0/3.0)-h)*6.0);

	return m1;
}

COLORREF HLStoRGB(const double H, const double L, const double S ) {
	double r,g,b;
	double m1, m2;

	if (S==0)
        r=g=b=L;
	else
	{
		if (L <=0.5)
            m2 = L*(1.0+S);
		else
            m2 = L+S-L*S;

		m1 = 2.0*L-m2;
		r  = HueToRGB(m1,m2,H+1.0/3.0);
		g  = HueToRGB(m1,m2,H);
		b  = HueToRGB(m1,m2,H-1.0/3.0);
	}

  return MakeRGB((BYTE)(r*255),(BYTE)(g*255),(BYTE)(b*255));
}

BOOL DibBmp_FilterWatermark( PDIBBMP db, DWORD dwColorTone )
{
   DIBSECTION  ds;

    if( !db )
        return FALSE;

    if ( !( sizeof(DIBSECTION) == GetObject( db->hBitmap, sizeof(DIBSECTION),(LPVOID)&ds)))
        return FALSE;

    if ( ds.dsBmih.biBitCount == 32 )
    {
        DWORD    *dwBits;
        int      i, j;
        DWORD    dwColor;
        DOUBLE   h0, l0, s0;
        BYTE     byAlpha;
        BYTE     byRed, byRed2;
        BYTE     byGreen, byGreen2;
        BYTE     byBlue, byBlue2;
        int      iColor   = 0;
        int      iCounter = 0;
        int      iLevel;

        dwBits = (DWORD*) ds.dsBm.bmBits;

        for ( i = 0 ; i < ds.dsBm.bmHeight ; i++ )
        {
            for ( j = 0 ; j < ds.dsBm.bmWidth ; j++ )
            {

                dwColor = (*(dwBits +  j ));

                byAlpha = (BYTE)(dwColor >> 24);
                byRed   = (BYTE)(dwColor >> 16);
                byGreen = (BYTE)(dwColor >> 8);
                byBlue  = (BYTE)(dwColor >> 0);

                if( !(db->bHasAlpha && byAlpha == 0) )
                {
                    if( byRed > 125 || byGreen > 125 || byBlue > 125 )
                        iColor++;
                    else
                        iColor--;

                    iCounter++;
                }
            }

            dwBits += ds.dsBm.bmWidth;
        }

        if( iColor > 0 )
            iLevel = (BYTE) (((double)iColor / (double)iCounter) * 100  );
        else
            iLevel = 10;

        dwColorTone = XCHGCOLORREF( dwColorTone );
        RGBtoHLS( dwColorTone, &h0, &l0, &s0 );

        byRed2    = (BYTE)(dwColorTone >> 16);
        byGreen2  = (BYTE)(dwColorTone >> 8);
        byBlue2   = (BYTE)(dwColorTone >> 0);

        dwBits = (DWORD*) ds.dsBm.bmBits;

        for ( i = 0 ; i < ds.dsBm.bmHeight ; i++ )
        {
            for ( j = 0 ; j < ds.dsBm.bmWidth ; j++ )
            {
                DOUBLE h, l, s;

                dwColor = (*(dwBits +  j ));
                byAlpha = (BYTE)(dwColor >> 24);

                if( dwColorTone )
                {
                  RGBtoHLS( dwColor, &h, &l, &s );
                  dwColor = HLStoRGB( h0, l , s0 );
                }

                byRed    = (BYTE)(dwColor >> 16);
                byGreen  = (BYTE)(dwColor >> 8);
                byBlue   = (BYTE)(dwColor >> 0);

                byRed     = (BYTE) (((byRed2 * (255 - iLevel)) + (byRed * iLevel)) >> 8);
                byGreen   = (BYTE) (((byGreen2 * (255 - iLevel)) + (byGreen * iLevel)) >> 8);
                byBlue    = (BYTE) (((byBlue2 * (255 - iLevel)) + (byBlue * iLevel)) >> 8);

                if( db->bHasAlpha )
                {
                    byRed   = (BYTE) (((DWORD)byRed)   * ((DWORD)byAlpha) / 255);
                    byGreen = (BYTE) (((DWORD)byGreen) * ((DWORD)byAlpha) / 255);
                    byBlue  = (BYTE) (((DWORD)byBlue)  * ((DWORD)byAlpha) / 255);
                }

                dwColor = (((DWORD)byAlpha) << 24)|
                          (((DWORD)byRed)   << 16)|
                          (((DWORD)byGreen) << 8) |
                          (((DWORD)byBlue)  << 0);

                *(dwBits +  j ) = dwColor;

            }

            dwBits += ds.dsBm.bmWidth;
        }

        return TRUE;
    }

    return FALSE;
}

#endif

//----------------------------------------------------------------------------//

#ifdef _FILTER_GRAYSCALE

BOOL DibBmp_FilterGrayScale( PDIBBMP db )
{
    DIBSECTION  ds;

    if( !db )
        return FALSE;

    if ( !( sizeof(DIBSECTION) == GetObject( db->hBitmap, sizeof(DIBSECTION),(LPVOID)&ds)))
        return FALSE;

    if ( ds.dsBmih.biBitCount == 32 )
    {
        DWORD    *dwBits;
        int     i, j;
        DWORD   dwColor;
        BYTE    byAlpha, byRed, byGreen, byBlue;

        dwBits = (DWORD*) ds.dsBm.bmBits;

        for ( i = 0 ; i < ds.dsBm.bmHeight ; i++ )
        {
            for ( j = 0 ; j < ds.dsBm.bmWidth ; j++ )
            {
                BYTE byGray;

                dwColor = (*(dwBits +  j ));

                byAlpha = (BYTE)(dwColor >> 24);
                byRed   = (BYTE)(dwColor >> 16);
                byGreen = (BYTE)(dwColor >> 8);
                byBlue  = (BYTE)(dwColor >> 0);

                byGray   = (BYTE) ((BYTE) (byRed   * 0.30) +
                                   (BYTE) (byGreen * 0.59) +
                                   (BYTE) (byBlue  * 0.11)) ;
                byRed    = byGray;
                byGreen  = byGray;
                byBlue   = byGray;

                dwColor = (((DWORD)byAlpha) << 24)|
                          (((DWORD)byRed)   << 16)|
                          (((DWORD)byGreen) << 8) |
                          (((DWORD)byBlue)  << 0);

                *(dwBits +  j ) = dwColor;
            }

            dwBits += ds.dsBm.bmWidth;
        }

        return TRUE;
    }

    return FALSE;
}

#endif

//----------------------------------------------------------------------------//

BOOL DibBmp_DrawAt( PDIBBMP db, HDC hdc, int x, int y )
{
    return DibBmp_DrawAt2( db, hdc, x, y, FALSE );
}

//----------------------------------------------------------------------------//

BOOL DibBmp_DrawAtI( PDIBBMP db, HDC hdc, int x, int y )
{
    return DibBmp_DrawAt2( db, hdc, x, y, TRUE );
}

//----------------------------------------------------------------------------//

BOOL DibBmp_DrawAt2( PDIBBMP db, HDC hdc, int x, int y, BOOL bInverse )
{
    DIBSECTION  ds;
    HDC         hdcMem;
    HBITMAP     hOldBmp;
    if( !db )
        return FALSE;

    if( !db->hBitmap )
        return FALSE;

    if ( !( sizeof(DIBSECTION) == GetObject( db->hBitmap, sizeof(DIBSECTION),(LPVOID)&ds)))
        return FALSE;

    hdcMem = CreateCompatibleDC(hdc);
    hOldBmp = (HBITMAP) SelectObject(hdcMem, db->hBitmap );

    if( bInverse )
    {
        x -= ds.dsBm.bmWidth;
        y -= ds.dsBm.bmHeight;
    }

    if( db->bHasAlpha )
    {
        BLENDFUNCTION     bf;

        bf.BlendOp             = AC_SRC_OVER;
        bf.BlendFlags          = 0;
        bf.SourceConstantAlpha = 255; //
        bf.AlphaFormat         = AC_SRC_ALPHA;

        AlphaBlend( hdc,
                      x,
                      y,
                      ds.dsBm.bmWidth,
                      ds.dsBm.bmHeight,
                      hdcMem,
                      0, 0,
                      ds.dsBm.bmWidth,
                      ds.dsBm.bmHeight,
                      bf );

    }
    else
        BitBlt(hdc, x, y, ds.dsBm.bmWidth, ds.dsBm.bmHeight, hdcMem, 0, 0, SRCCOPY);

    SelectObject(hdcMem, hOldBmp);
    DeleteDC( hdcMem );

    return TRUE;
}

//----------------------------------------------------------------------------//

BOOL DibBmp_DrawStretch( PDIBBMP db, HDC hdc, int x, int y , int iWidth, int iHeight )
{
    DIBSECTION  ds;
    HDC         hdcMem;
    HBITMAP     hOldBmp;

    if( !db )
        return FALSE;

    if( !db->hBitmap )
        return FALSE;

    if ( !( sizeof(DIBSECTION) == GetObject( db->hBitmap, sizeof(DIBSECTION),(LPVOID)&ds)))
        return FALSE;

    hdcMem = CreateCompatibleDC(hdc);
    hOldBmp = (HBITMAP) SelectObject(hdcMem, db->hBitmap );

    if( db->bHasAlpha )
    {
        BLENDFUNCTION     bf;

        bf.BlendOp             = AC_SRC_OVER;
        bf.BlendFlags          = 0;
        bf.SourceConstantAlpha = 255; //
        bf.AlphaFormat         = AC_SRC_ALPHA;

        AlphaBlend( hdc,
                      x,
                      y,
                      iWidth,
                      iHeight,
                      hdcMem,
                      0, 0,
                      ds.dsBm.bmWidth,
                      ds.dsBm.bmHeight,
                      bf );

    }
    else
    {
        int iPrevMode = SetStretchBltMode( hdc, COLORONCOLOR );
        StretchBlt(hdc, x, y, iWidth, iHeight, hdcMem, 0, 0, ds.dsBm.bmWidth, ds.dsBm.bmHeight, SRCCOPY);
        SetStretchBltMode( hdc, iPrevMode );
    }

    SelectObject(hdcMem, hOldBmp);
    DeleteDC( hdcMem );

    return TRUE;
}

//----------------------------------------------------------------------------//

BOOL DibBmp_Release( PDIBBMP db )
{
    if( db->ColorTable  )
        GlobalFree( (HGLOBAL) db->ColorTable );

    if( db->hBitmap )
        DeleteObject( db->hBitmap );

    return TRUE;
}

//----------------------------------------------------------------------------//

BOOL DibBmp_Release2( PDIBBMP db )
{
    if( db->ColorTable  )
        GlobalFree( (HGLOBAL) db->ColorTable );

    return TRUE;
}

//----------------------------------------------------------------------------//
