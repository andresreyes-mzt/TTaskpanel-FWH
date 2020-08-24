/**************************************************************************************************
* Project      : TTaskpanel
* File         : TaskPanel.prg
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

static oTaskPanel
static oTaskGroup
static oWndDefault

//----------------------------------------------------------------------------//

FUNCTION TaskPanelBegin( cTheme, nTop, nLeft, nWidth, nHeight, nAnchor, oWnd, cBitmap, nHeaderAlign, lAnimate, lCollapsable, nPlainStyle, oObj )

   IF !(oObj == nil) .And. !(oObj:oWnd == nil)
      IF oObj:lRedefine
         oObj:SuspendLayout()
         RETURN oTaskPanel := oObj
      ENDIF
   ENDIF

   oTaskPanel := TTaskPanel():New( cTheme, nTop, nLeft, nWidth, nHeight, nAnchor, oWnd, cBitmap, nHeaderAlign, lAnimate, lCollapsable, nPlainStyle )
   oTaskPanel:SuspendLayout()
   RETURN  oTaskPanel

//----------------------------------------------------------------------------//

FUNCTION TaskPanelEnd()
   oTaskPanel:EndLayout()
   RETURN oTaskPanel := nil

//----------------------------------------------------------------------------//

FUNCTION TaskGroupBegin( cPrompt, cBitmap, lSpecial, cWatermark, nHeight, lHeader, lCollapsed, lCollapsable, bOnCollapse, bOnExpand, bOnClick )
   oWndDefault := GetWndDefault()
   oTaskGroup := TTaskPanelGroup():New( cPrompt, cBitmap, lSpecial, oTaskPanel, cWatermark, nHeight, lHeader, lCollapsed, lCollapsable, bOnCollapse, bOnExpand, bOnClick )
   SetWndDefault( oTaskGroup )
   RETURN  oTaskGroup

//----------------------------------------------------------------------------//

FUNCTION TaskGroupEnd()
   SetWndDefault( oWndDefault )
   RETURN oTaskGroup := nil

//----------------------------------------------------------------------------//

FUNCTION TaskGroupAddItem( cPrompt, cBitmap, bAction, cTooltip, lInfo, bWhen, crBackground, crBackgroundHot )
   RETURN oTaskGroup:AddTask( cPrompt, cBitmap, bAction, cTooltip, lInfo, bWhen, crBackground, crBackgroundHot )

//----------------------------------------------------------------------------//
// EOF
//-----