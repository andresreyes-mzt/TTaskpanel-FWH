/**************************************************************************************************
* Project      : TTaskpanel
* File         : c_TThemeManager.c
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

HMODULE GetResources( void );

//----------------------------------------------------------------------------//

static void SetFontStyle( LOGFONT * lf, const char * szStyle )
{
   lf->lfWeight      = FW_NORMAL;
   lf->lfUnderline   = FALSE;
   lf->lfItalic      = FALSE;
   lf->lfStrikeOut   = FALSE;

   if( szStyle )
   {
      char *p = (char *) szStyle;

      while( *p )
      {
         switch( *p )
         {
            case 'b':
            case 'B':
               lf->lfWeight = FW_BOLD;
               break;
            case 'u':
            case 'U':
               lf->lfUnderline = TRUE;
               break;
            case 'i':
            case 'I':
            case 'o':
            case 'O':
               lf->lfItalic = TRUE;
               break;
            case 's':
            case 'S':
               lf->lfStrikeOut = TRUE;
               break;
         }
         p++;
      }
   }

}

//----------------------------------------------------------------------------//

char * GetFontStyle( LOGFONT * lf )
{
   static char szStyle[5];
   char *p = szStyle;

   if( lf->lfWeight == FW_BOLD )
      *p++ = 'b';

   if( lf->lfUnderline )
      *p++ = 'u';

   if( lf->lfItalic )
      *p++ = 'i';

   if( lf->lfStrikeOut )
      *p++ = 's';

   *p = 0;

   return szStyle;
}

//----------------------------------------------------------------------------//

static PTPGROUP _i_GetGroup( PTHEMEINFO pti )
{
   if( Object_GetL( hb_stackSelfItem(), "LSPECIAL" ) )
      return &pti->SpecialGroup;
   else
      return &pti->NormalGroup;
}

//----------------------------------------------------------------------------//

static void _i_GetSetBackGround( COLORREF *c1, COLORREF *c2, int * i, HBITMAP *b, char ** name )
{

   if( ISCHAR(1) && b )
   {
      if(*b)
         DeleteObject( *b );

      if( *name)
          hb_xfree(*name);
      
      *name = hb_strdup( hb_parc(1) );
      *b    = LoadBitmapEx( GetResources(), *name, TRUE );

      *c1 =
      *c2 =
      *i  = 0;
   }
   if( ISARRAY(1) )
   {
      *c1 = (COLORREF) hb_parvnl( 1, 1 );
      *c2 = (COLORREF) hb_parvnl( 1, 2 );
      *i  =            hb_parvni( 1, 3 );

      if(b)
         if(*b)
         {
            DeleteObject( *b );
            *b = 0;
         }

      if(name)
          if( *name )
          {
              hb_xfree(*name);
              *name = NULL;
          }
      
      

   }
   else if ( ISNUM(1) )
   {
      *c1 =
      *c2 = (COLORREF) hb_parnl( 1 );
      *i  = 0;

      if(b)
         if(*b)
         {
            DeleteObject( *b );
            *b = 0;
         }

      if(name)
          if( *name )
          {
              hb_xfree(*name);
              *name = NULL;
          }
   }

   //////////////////////////
   //  Return Array { cBackground1, cBackground2, iValue }
   //////////

   if(b)
       if(*b)
       {
           if( *name)
               hb_retc( *name );
           else
               hb_retc("");
           return;
       }

   hb_reta(3);
   hb_storvnl( (long)*c1, -1, 1 );
   hb_storvnl( (long)*c2, -1, 2 );
   hb_storvni(       *i , -1, 3 );

}

//----------------------------------------------------------------------------//

static void _i_GetSetRECT( RECT * r )
{
   if( ISARRAY(1) )
   {
      r->left   = hb_parvni( 1, 1 );
      r->top    = hb_parvni( 1, 2 );
      r->right  = hb_parvni( 1, 3 );
      r->bottom = hb_parvni( 1, 4 );
   }
   else if ( ISNUM(1) )
   {
      r->left   =
      r->top    =
      r->right  =
      r->bottom = hb_parni( 1 );
   }

   hb_reta(4);
   hb_storvni( r->left   , -1, 1 );
   hb_storvni( r->top    , -1, 2 );
   hb_storvni( r->right  , -1, 3 );
   hb_storvni( r->bottom , -1, 4 );
}

//----------------------------------------------------------------------------//

static void _i_GetSetLong( long * l )
{
   if( ISNUM(1) )
      *l   = hb_parnl( 1 );

   hb_retnl( *l );

}

//----------------------------------------------------------------------------//

static void _i_GetSetFont( HFONT * f1, HFONT* f2, LOGFONT * lf1, LOGFONT * lf2 )
{
   if( ISARRAY(1) )
   {
      const char * szFaceName = hb_parvc( 1, 1 );
      int          iHeight    = hb_parvni( 1, 2 );

      if( iHeight == 0 )
         iHeight = 8;

      iHeight = Pt2FontHeight( iHeight );

      DeleteObject( *f1 );
      DeleteObject( *f2 );

      strcpy( lf1->lfFaceName   , szFaceName );
      strcpy( lf2->lfFaceName, szFaceName );

      lf1->lfHeight    = iHeight;
      lf2->lfHeight = iHeight;

      *f1 = CreateFontIndirect( lf1 );
      *f2 = CreateFontIndirect( lf2 );
   }

   hb_reta(2);
   hb_storvc ( lf1->lfFaceName ,  -1, 1 );
   hb_storvni( FontHeight2Pt(lf1->lfHeight)   ,  -1, 2 );

}

//----------------------------------------------------------------------------//

static void _i_GetSetFontStyle( HFONT * f1, HFONT* f2, LOGFONT * lf1, LOGFONT * lf2 )
{
   if( ISCHAR(1) )
   {
      const char * szStyle = hb_parc(1);

      DeleteObject( *f1 );
      DeleteObject( *f2 );

      SetFontStyle( lf1, szStyle );
      SetFontStyle( lf2, szStyle );

      *f1 = CreateFontIndirect( lf1 );
      *f2 = CreateFontIndirect( lf2 );
   }
   else if ( ISARRAY(1) )
   {
      const char * szStyle = hb_parvc(1,1);
      const char * szStyle2 = hb_parvc(1,2);

      DeleteObject( *f1 );
      DeleteObject( *f2 );

      if( hb_parinfa( 1, 1 ) == HB_IT_STRING ) // Character
         SetFontStyle( lf1, szStyle );

      if( hb_parinfa( 1, 2 ) == HB_IT_STRING ) // Character
         SetFontStyle( lf2, szStyle2 );

      *f1 = CreateFontIndirect( lf1 );
      *f2 = CreateFontIndirect( lf2 );
   }

   hb_reta(2);
   hb_storvc ( GetFontStyle( lf1 ) ,  -1, 1 );
   hb_storvc ( GetFontStyle( lf2 ) ,  -1, 2 );

}

//----------------------------------------------------------------------------//

static void _i_GetSetForeground( COLORREF * c1, COLORREF *c2 )
{
   if( ISARRAY(1) )
   {
      *c1 = (COLORREF) hb_parvnl( 1, 1 );

      if( c2 )
         *c2 = (COLORREF) hb_parvnl( 1, 2 );
   }
   else if ( ISNUM(1) )
   {

      *c1 = (COLORREF) hb_parnl( 1 );

      if( c2 )
         *c2 = *c1;
   }

   //////////////////////////
   //  Return Array { crNormal, crHot }
   //////////

   if( c2 )
   {
      hb_reta(2);
      hb_storvnl( (long)*c1, -1, 1 );
      hb_storvnl( (long)*c2   , -1, 2 );
   }
   else
      hb_retnl( *c1 );
}

//----------------------------------------------------------------------------//

static void _i_GetSetButton( char ** name, HBITMAP *b )
{
    if( ISCHAR(1) )
    {
        if( *name ) // Free String Name
            hb_xfree(*name);
        if( *b )
            DeleteObject( *b );
        
        *name   = hb_strdup( hb_parc(1) );
        *b      = LoadBitmapEx( GetResources(), *name, TRUE );
        
    }

    /////////////////////////////
    ///     By Default Return Bitmap Name
    ////////////

    if( *name )
        hb_retc(*name);
    else
        hb_retc("");
}

//----------------------------------------------------------------------------//
#define GetThemeInfo()    ((PTHEMEINFO) Object_GetNL( hb_stackSelfItem(), "NHANDLE" ))
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
//   TASKPANEL
//----------------------------------------------------------------------------//

//////////////////////////
//   oObj:BackGround :=  { cBackground1, cBackground2, iValue } || cBackGround 1
//////////

HB_FUNC( TMTASKPANEL_BACKGROUND )
{
   PTHEMEINFO pti = GetThemeInfo();

   if( pti )
      _i_GetSetBackGround( &pti->TaskPanel.crBackground1, &pti->TaskPanel.crBackground2, &pti->TaskPanel.iValue, NULL, NULL );

}

//////////////////////////
// oObj:Padding :=  { left, top, right, bottom } | nPadding
//////////

HB_FUNC( TMTASKPANEL_PADDING )
{
   PTHEMEINFO pti = GetThemeInfo();

   if( pti )
      _i_GetSetRECT( &pti->TaskPanel.rcPadding );

}

//////////////////////////
// oObj:BorderThickness := { left, top, right, bottom } || nBorderThickness
//////////

HB_FUNC( TMTASKPANEL_BORDERTHICKNESS )
{
   PTHEMEINFO pti = GetThemeInfo();

   if( pti )
      _i_GetSetRECT( &pti->TaskPanel.rcBorderThickness );

}


//////////////////////////
// oObj:BorderColor :=  crBorder
//////////

HB_FUNC( TMTASKPANEL_BORDERCOLOR )
{
   PTHEMEINFO pti = GetThemeInfo();

   if( pti )
      _i_GetSetLong( (long*)&pti->TaskPanel.crBorder );

}

//----------------------------------------------------------------------------//
//   TASKHEADER
//----------------------------------------------------------------------------//

//////////////////////////
// oObj:Align =                  0 = LEFT   1 = CENTER   2 = RIGHT
//////////

HB_FUNC( TMTASKHEADER_ALIGN )
{
   PTHEMEINFO pti = GetThemeInfo();

   if( pti )
      _i_GetSetLong( (long*)&( _i_GetGroup( pti ) )->Header.uAlign );

}

//////////////////////////
// oObj:Background := { cBackground1, cBackground2, iValue } || cBackGround
//////////

HB_FUNC( TMTASKHEADER_BACKGROUND )
{
   PTHEMEINFO pti = GetThemeInfo();

   if( pti )
   {
      PTPGROUP g = _i_GetGroup( pti );

      _i_GetSetBackGround( &g->Header.crBackground1, &g->Header.crBackground2, &g->Header.iValue, &g->Header.hBitmap, &g->Header.szBitmapName[0] );
   }
}

//////////////////////////
// oObj:BackgroundHot := { cBackground1, cBackground2, iValue } || cBackGround
//////////

HB_FUNC( TMTASKHEADER_BACKGROUNDHOT )
{
   PTHEMEINFO pti = GetThemeInfo();

   if( pti )
   {
      PTPGROUP g = _i_GetGroup( pti );

      _i_GetSetBackGround( &g->Header.crBackgroundHot1, &g->Header.crBackgroundHot2, &g->Header.iValueHot, &g->Header.hBitmapHot, &g->Header.szBitmapName[1] );
   }
}

//////////////////////////
// oObj:BorderColor := cBorderColor
//////////

HB_FUNC( TMTASKHEADER_BORDERCOLOR )
{
   PTHEMEINFO pti = GetThemeInfo();

   if( pti )
      _i_GetSetLong( (long*)&( _i_GetGroup( pti ) )->Header.crBorder );
}

//////////////////////////
// oObj:BorderStyle
//////////

HB_FUNC( TMTASKHEADER_BORDERSTYLE )
{
   PTHEMEINFO pti = GetThemeInfo();

   if( pti )
      _i_GetSetLong( (long*)&( _i_GetGroup( pti ) )->Header.iBorderStyle );

}

//////////////////////////
// oObj:Font := { FaceName, Size }
//////////

HB_FUNC( TMTASKHEADER_FONT )
{
   PTHEMEINFO pti = GetThemeInfo();

   if( pti )
   {
      PTPGROUP g = _i_GetGroup( pti );

      _i_GetSetFont( &g->Header.hFontNormal, &g->Header.hFontHot, &g->Header.lf, &g->Header.lfHot );
   }
}

//////////////////////////
// oObj:FontStyle := "buis" BOLD | UNDERLINE | ITALIC | STRIKEOUT
//////////

HB_FUNC( TMTASKHEADER_FONTSTYLE )
{
   PTHEMEINFO pti = GetThemeInfo();

   if( pti )
   {
      PTPGROUP g = _i_GetGroup( pti );

      _i_GetSetFontStyle( &g->Header.hFontNormal, &g->Header.hFontHot, &g->Header.lf, &g->Header.lfHot );
   }

}

//////////////////////////
// oObj:Foreground :=  cNormal
//////////

HB_FUNC( TMTASKHEADER_FOREGROUND )
{
   PTHEMEINFO pti = GetThemeInfo();

   if( pti )
      _i_GetSetLong( (long*)&( _i_GetGroup( pti ) )->Header.crNormal );
     
}

//////////////////////////
// oObj:ForegroundHot :=  cHot
//////////

HB_FUNC( TMTASKHEADER_FOREGROUNDHOT )
{
   PTHEMEINFO pti = GetThemeInfo();

   if( pti )
      _i_GetSetLong( (long*)&( _i_GetGroup( pti ) )->Header.crHot );  
}

//////////////////////////
// oObj:ButtonCollapse :=  cNormalArrow
//////////

HB_FUNC( TMTASKHEADER_BUTTONCOLLAPSE )
{
   PTHEMEINFO pti = GetThemeInfo();

   if( pti )
   {
      PTPGROUP g = _i_GetGroup( pti );

      _i_GetSetButton( &g->Header.szBitmapName[ 2 ], &g->Header.hBitmapArrowUp[ 0 ] );
   }
}

//////////////////////////
// oObj:ButtonCollapseHi :=  cHotArrow
//////////

HB_FUNC( TMTASKHEADER_BUTTONCOLLAPSEHI )
{
   PTHEMEINFO pti = GetThemeInfo();

   if( pti )
   {
      PTPGROUP g = _i_GetGroup( pti );

      _i_GetSetButton( &g->Header.szBitmapName[ 3 ], &g->Header.hBitmapArrowUp[ 1 ] );
   }
}


//////////////////////////
// oObj:ButtonExpand :=  cNormalArrow
//////////

HB_FUNC( TMTASKHEADER_BUTTONEXPAND )
{
   PTHEMEINFO pti = GetThemeInfo();

   if( pti )
   {
      PTPGROUP g = _i_GetGroup( pti );

      _i_GetSetButton( &g->Header.szBitmapName[ 4 ], &g->Header.hBitmapArrowDn[ 0 ] );
   }
}

//////////////////////////
// oObj:ButtonExpandHi := cHotArrow
//////////

HB_FUNC( TMTASKHEADER_BUTTONEXPANDHI )
{
   PTHEMEINFO pti = GetThemeInfo();

   if( pti )
   {
      PTPGROUP g = _i_GetGroup( pti );

      _i_GetSetButton( &g->Header.szBitmapName[ 5 ], &g->Header.hBitmapArrowDn[ 1 ] );
   }
}

//////////////////////////
// oObj:IsBackgroundBitmap
//////////

HB_FUNC( TMTASKHEADER_ISBACKGROUNDBITMAP )
{
   PTHEMEINFO pti = GetThemeInfo();

   if( pti )
   {
      PTPGROUP g = _i_GetGroup( pti );
	  
	  hb_retl( g->Header.hBitmap != NULL );
   }
}

//////////////////////////
// oObj:IsBackgroundBitmap
//////////

HB_FUNC( TMTASKHEADER_ISBACKGROUNDHOTBITMAP )
{
   PTHEMEINFO pti = GetThemeInfo();

   if( pti )
   {
      PTPGROUP g = _i_GetGroup( pti );
	  
	  hb_retl( g->Header.hBitmapHot != NULL );
   }
}

//////////////////////////
// oObj:Padding :=  { left, top, right, bottom } | nPadding
//////////

HB_FUNC( TMTASKHEADER_PADDING )
{
   PTHEMEINFO pti = GetThemeInfo();

   if( pti )
      _i_GetSetRECT( &( _i_GetGroup( pti ) )->Header.rcPadding );
}

//////////////////////////
// oObj:BorderThickness :=  { left, top, right, bottom } | nPadding
//////////

HB_FUNC( TMTASKHEADER_BORDERTHICKNESS )
{
   PTHEMEINFO pti = GetThemeInfo();

   if( pti )
      _i_GetSetRECT( &( _i_GetGroup( pti ) )->Header.rcBorderThickness );

}

//////////////////////////
// oObj:MarginToPanel
//////////

HB_FUNC( TMTASKHEADER_MARGINTOPANEL )
{
   PTHEMEINFO pti = GetThemeInfo();

   if( pti )
      _i_GetSetLong( (long*)&( _i_GetGroup( pti ) )->Header.iMargin );

}


//----------------------------------------------------------------------------//
//  TASKLIST
//----------------------------------------------------------------------------//

//////////////////////////
// oObj:Background := { cBackground1, cBackground2, iValue } || cBackGround
//////////

HB_FUNC( TMTASKLIST_BACKGROUND )
{
   PTHEMEINFO pti = GetThemeInfo();

   if( pti )
   {
      PTPGROUP g = _i_GetGroup( pti );

      _i_GetSetBackGround( &g->Tasklist.crBackground1, &g->Tasklist.crBackground2, &g->Tasklist.iValue, NULL, NULL );
   }

}

//////////////////////////
// oObj:BorderColor := cBorderColor
//////////

HB_FUNC( TMTASKLIST_BORDERCOLOR )
{
   PTHEMEINFO pti = GetThemeInfo();

   if( pti )
      _i_GetSetLong( (long*)&( _i_GetGroup( pti ) )->Tasklist.crBorder );

}

//////////////////////////
// oObj:Padding :=  { left, top, right, bottom } | nPadding
//////////

HB_FUNC( TMTASKLIST_PADDING )
{
   PTHEMEINFO pti = GetThemeInfo();

   if( pti )
      _i_GetSetRECT( &( _i_GetGroup( pti ) )->Tasklist.rcPadding );

}

//////////////////////////
// oObj:BorderThickness :=  { left, top, right, bottom } | nPadding
//////////

HB_FUNC( TMTASKLIST_BORDERTHICKNESS )
{
   PTHEMEINFO pti = GetThemeInfo();

   if( pti )
      _i_GetSetRECT( &( _i_GetGroup( pti ) )->Tasklist.rcBorderThickness );

}

//////////////////////////
// oObj:MarginToTask
//////////

HB_FUNC( TMTASKLIST_MARGINTOTASK )
{
   PTHEMEINFO pti = GetThemeInfo();

   if( pti )
      _i_GetSetLong( (long*)&( _i_GetGroup( pti ) )->Tasklist.iMargin );

}

//----------------------------------------------------------------------------//
// TASKITEM
//----------------------------------------------------------------------------//

//////////////////////////
// oObj:Background := { cBackground1, cBackground2, iValue } || cBackGround
//////////

HB_FUNC( TMTASKITEM_BACKGROUND )
{
   PTHEMEINFO pti = GetThemeInfo();

   if( pti )
   {
      PTPGROUP g = _i_GetGroup( pti );

      _i_GetSetBackGround( &g->Tasklist.Link.crBackground1, &g->Tasklist.Link.crBackground2, &g->Tasklist.Link.iValue, NULL, NULL );
   }

}

//////////////////////////
// oObj:BackgroundHot := { cBackground1, cBackground2, iValue } || cBackGround
//////////

HB_FUNC( TMTASKITEM_BACKGROUNDHOT )
{
   PTHEMEINFO pti = GetThemeInfo();

   if( pti )
   {
      PTPGROUP g = _i_GetGroup( pti );

      _i_GetSetBackGround( &g->Tasklist.Link.crBackgroundHot1, &g->Tasklist.Link.crBackgroundHot2, &g->Tasklist.Link.iValueHot, NULL, NULL );
   }

}

//////////////////////////
// oObj:BorderColor := cBorderColor
//////////

HB_FUNC( TMTASKITEM_BORDERCOLOR )
{
   PTHEMEINFO pti = GetThemeInfo();

   if( pti )
      _i_GetSetLong( (long*)&( _i_GetGroup( pti ) )->Tasklist.Link.crBorder );

}

//////////////////////////
// oObj:BorderThickness :=  { left, top, right, bottom } | nPadding
//////////

HB_FUNC( TMTASKITEM_BORDERTHICKNESS )
{
   PTHEMEINFO pti = GetThemeInfo();

   if( pti )
      _i_GetSetRECT( &( _i_GetGroup( pti ) )->Tasklist.Link.rcBorderThickness );

}

//////////////////////////
// oObj:Foreground :=  cNormal
//////////

HB_FUNC( TMTASKITEM_FOREGROUND )
{
   PTHEMEINFO pti = GetThemeInfo();

   if( pti )
      _i_GetSetLong( (long*)&( _i_GetGroup( pti ) )->Tasklist.Link.crNormal );
     
}

//////////////////////////
// oObj:ForegroundHot :=  cNormal
//////////

HB_FUNC( TMTASKITEM_FOREGROUNDHOT )
{
   PTHEMEINFO pti = GetThemeInfo();

   if( pti )
      _i_GetSetLong( (long*)&( _i_GetGroup( pti ) )->Tasklist.Link.crHot );
     
}

//////////////////////////
// oObj:InfoBackground :=  { cNormal, cHot, nDirec } | cColor
//////////

HB_FUNC( TMTASKITEM_INFOBACKGROUND )
{
   PTHEMEINFO pti = GetThemeInfo();

   if( pti )
   {
      PTPGROUP g = _i_GetGroup( pti );

      _i_GetSetBackGround( &g->Tasklist.Info.crBackground1, &g->Tasklist.Info.crBackground2, &g->Tasklist.Info.iValue, NULL, NULL );
   }
}

//////////////////////////
// oObj:InfoForeground :=  { cNormal, cHot }
//////////

HB_FUNC( TMTASKITEM_INFOFOREGROUND )
{
   PTHEMEINFO pti = GetThemeInfo();

   if( pti )
   {
      PTPGROUP g = _i_GetGroup( pti );

      _i_GetSetForeground( &g->Tasklist.Info.crNormal, NULL );
   }
}

//////////////////////////
// oObj:ImagePadding :=  { left, top, right, bottom } | nPadding
//////////

HB_FUNC( TMTASKITEM_IMAGEPADDING )
{
   PTHEMEINFO pti = GetThemeInfo();

   if( pti )
      _i_GetSetRECT( &( _i_GetGroup( pti ) )->Tasklist.Link.rcImagePadding );

}

//////////////////////////
// oObj:Padding :=  { left, top, right, bottom } | nPadding
//////////

HB_FUNC( TMTASKITEM_PADDING )
{
   PTHEMEINFO pti = GetThemeInfo();

   if( pti )
      _i_GetSetRECT( &( _i_GetGroup( pti ) )->Tasklist.Link.rcPadding );

}

//////////////////////////
// oObj:Font := { FaceName, Size }
//////////

HB_FUNC( TMTASKITEM_FONT )
{
   PTHEMEINFO pti = GetThemeInfo();

   if( pti )
   {
      PTPGROUP g = _i_GetGroup( pti );

      _i_GetSetFont( &g->Tasklist.hFontNormal, &g->Tasklist.hFontHot, &g->Tasklist.lfNormal, &g->Tasklist.lfHot );
   }

}

//////////////////////////
// oObj:FontStyle := "buis" BOLD | UNDERLINE | ITALIC | STRIKEOUT
//////////

HB_FUNC( TMTASKITEM_FONTSTYLE )
{
   PTHEMEINFO pti = GetThemeInfo();

   if( pti )
   {
      PTPGROUP g = _i_GetGroup( pti );

      _i_GetSetFontStyle( &g->Tasklist.hFontNormal, &g->Tasklist.hFontHot, &g->Tasklist.lfNormal, &g->Tasklist.lfHot );
   }
}
