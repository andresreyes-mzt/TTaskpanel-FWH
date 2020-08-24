/**************************************************************************************************
* Proyecto     : TTaskpanel
* Archivo      : TaskPanel.ch
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

#ifndef _TASKPANEL_CH_
#define _TASKPANEL_CH_

//////////////////////////
// Creating Taskpanel
//////////

#xtranslate __align__( LEFT )   => 0
#xtranslate __align__( RIGHT )  => 1
#xtranslate __align__( CENTER ) => 2

#xcommand DEFINE TASKPANEL [ <oObj> ] ;
                    [ OF <oWnd> ] ;
                    [ <x:DLL,THEME,NAME> <cTheme> ] ;
                    [ AT <nTop>, <nLeft> ] ;
                    [ SIZE <nWidth>, <nHeight> ] ;
                    [ ANCHOR [ <left:LEFT> ] [ <right:RIGHT> ] ];
                    [ HEADER <cBitmap> [ ALIGN <align:LEFT,RIGHT,CENTER> ] ];
                    [ <animate:ANIMATE> ];
                    [ <collapsable:NO COLLAPSABLE> ] ;
                    [ <bubble:BUBBLE> ] ;
                    [ <plain:PLAIN> ] ;
         =>;
         [ <oObj> := ] TaskPanelBegin( [ <cTheme> ],;
                                        <nTop>,;
                                        <nLeft>,;
                                        <nWidth>,;
                                        <nHeight>,;
                                         if( <.left.>, 1, if( <.right.>, 2, 0 ) ),;
                                         <oWnd>,;
                                         [ <cBitmap> ],;
                                         [ __align__( <align> ) ],;
                                         <.animate.>,;
                                         !<.collapsable.>,;
                                         if( <.bubble.>, 1, if( <.plain.>, 2, 0 ) ),;
                                         <oObj> )

#xcommand REDEFINE TASKPANEL [ <oObj> ] ;
                    [ ID <nId> ];
                    [ OF <oWnd> ] ;
                    [ <x:DLL,THEME,NAME> <cTheme> ] ;
                    [ HEADER <cBitmap> [ ALIGN <align:LEFT,RIGHT,CENTER> ] ];
                    [ <animate:ANIMATE> ];
                    [ <collapsable:NO COLLAPSABLE> ] ;
                    [ <bubble:BUBBLE> ] ;
                    [ <plain:PLAIN> ] ;
         =>;
         [ <oObj> := ] TTaskPanel():ReDefine( <nId>,;
                                        [ <cTheme> ],;
                                         <oWnd>,;
                                         [ <cBitmap> ],;
                                         [ __align__( <align> ) ],;
                                         <.animate.>,;
                                         !<.collapsable.>,;
                                         if( <.bubble.>, 1, if( <.plain.>, 2, 0 ) );
                                         )


#xcommand TASKGROUP [ <oObj> PROMPT ] [ <cPrompt> ] ;
                  [ BITMAP <cBitmap> ];
                  [ WATERMARK <cWatermark> ];
                  [ HEIGHT <nHeight> ];
                  [ <special:SPECIAL> ];
                  [ <header:NO HEADER> ];
                  [ <collapsed:COLLAPSED> ];
                  [ <collapsable:COLLAPSABLE> ];
                  [ <nocollapsable:NO COLLAPSABLE> ] ;
                  [ ONCLICK <onclick,...> ];
                  [ ONCOLLAPSE <oncollapse,...> ];
                  [ ONEXPAND <onexpand,...> ];
         =>;
         [ <oObj> := ] TaskGroupBegin( <cPrompt>,;
                                       <cBitmap>,;
                                       <.special.>,;
                                       <cWatermark>,;
                                       <nHeight>,;
                                       !<.header.>,;
                                       <.collapsed.>,;
                                       [ <.collapsable.> ];
                                       [ !<.nocollapsable.> ],;
                                       [ {|oTaskGroup| <oncollapse> } ],;
                                       [ {|oTaskGroup| <onexpand> } ],;
                                       [ {|oTaskGroup| <onclick> } ] )

#xcommand TASKITEM [ <oObj> PROMPT ] [ <cPrompt> ] ;
                  [ BITMAP <cBitmap> ];
                  [ BACKGROUND <background>, <backgroundhot>] ;
                  [ <info:INFO> ] ;
                  [ <xa:ONCLICK,ACTION> <bAction,...> ];
                  [ TOOLTIP <cTooltip> ];
                  [ WHEN <when> ] ;
         =>;
         [ <oObj> := ] TaskGroupAddItem( <cPrompt>,;
                                         <cBitmap>,;
                                         [ {|oTaskItem, nRow, nCol| <bAction> } ],;
                                         [ <cTooltip> ], <.info.> ;
                                         , [ <{when}> ], [ <background> ], [ <backgroundhot> ] )

#xcommand ENDTASKGROUP => TaskGroupEnd()

#xcommand ENDTASKPANEL => TaskPanelEnd()

//////////////////////////
// Theme
//////////

#xcommand LOAD THEME <cName> [, <cNamen> ];
          =>;
             LoadTheme_<cName>();
          [; LoadTheme_<cNamen>() ]

#xcommand DEFINE THEME [ <oObj> ][ NAME <cName> ];
          => ;
          WITH OBJECT [ <oObj> := ] TThemeManager():New( <cName> )

#xcommand ELEMENT TASKPANEL ;
                  [ BACKGROUND <background> ];
                  [ BORDERCOLOR <bordercolor> ];
                  [ BORDERTHICKNESS <borderthickness> ];
                  [ PADDING <padding> ];
          => ;
          WITH OBJECT :TaskPanel ;;
               [ :Background        := <background>      ;] ;
               [ :BorderColor       := <bordercolor>     ;] ;
               [ :BorderThickness   := <borderthickness> ;] ;
               [ :Padding           := <padding>          ]


#xcommand ELEMENT NORMAL GROUP;
          =>;
          WITH OBJECT :NormalGroup

#xcommand ELEMENT SPECIAL GROUP;
          =>;
          WITH OBJECT :SpecialGroup

#xcommand ELEMENT HEADER;
                  [ BACKGROUND <background> ];
                  [ BORDERCOLOR <bordercolor> ];
                  [ BORDERSTYLE <borderstyle> ];
                  [ BORDERTHICKNESS <borderthickness> ];
                  [ BUTTON COLLAPSE <buttoncollapse> ];
                  [ BUTTON EXPAND <buttonexpand> ];
                  [ FONT <font> ];
                  [ FONTSTYLE <fontstyle> ];
                  [ FOREGROUND <foreground> ];
                  [ MARGIN TO PANEL <margintopanel> ];
                  [ PADDING <padding> ];
          =>;
          WITH OBJECT :Header ;;
               [ :Background        := <background>      ;] ;
               [ :BorderColor       := <bordercolor>     ;] ;
               [ :BorderStyle       := <borderstyle>     ;] ;
               [ :BorderThickness   := <borderthickness> ;] ;
               [ :ButtonCollapse    := <buttoncollapse>  ;] ;
               [ :ButtonExpand      := <buttonexpand>    ;] ;
               [ :Font              := <font>            ;] ;
               [ :FontStyle         := <fontstyle>       ;] ;
               [ :Foreground        := <foreground>      ;] ;
               [ :MargintoPanel     := <margintopanel>   ;] ;
               [ :Padding           := <padding>          ]

#xcommand ELEMENT TASKLIST;
                  [ BACKGROUND <background> ];
                  [ BORDERCOLOR <bordercolor> ];
                  [ BORDERTHICKNESS <borderthickness> ];
                  [ MARGIN TO TASK <margintotask> ];
                  [ PADDING <padding> ];
          =>;
          WITH OBJECT :TaskList ;;
               [ :Background        := <background>      ;] ;
               [ :BorderColor       := <bordercolor>     ;] ;
               [ :BorderThickness   := <borderthickness> ;] ;
               [ :MargintoTask      := <margintotask>    ;] ;
               [ :Padding           := <padding>          ]

#xcommand ELEMENT TASK;
                  [ BACKGROUND <background> ];
                  [ BORDERCOLOR <bordercolor> ];
                  [ BORDERTHICKNESS <borderthickness> ];
                  [ FONT <font> ];
                  [ FONTSTYLE <fontstyle> ];
                  [ FOREGROUND <foreground> ];
                  [ IMAGEPADDING <imagepadding>] ;
                  [ INFOBACKGROUND <ibackground> ];
                  [ INFOFOREGROUND <iforeground> ];
                  [ PADDING <padding> ];
          =>;
          WITH OBJECT :Task ;;
               [ :Background        := <background>      ;] ;
               [ :BorderColor       := <bordercolor>     ;] ;
               [ :BorderThickness   := <borderthickness> ;] ;
               [ :Font              := <font>            ;] ;
               [ :FontStyle         := <fontstyle>       ;] ;
               [ :Foreground        := <foreground>      ;] ;
               [ :ImagePadding      := <imagepadding>    ;] ;
               [ :InfoBackground    := <ibackground>     ;] ;
               [ :InfoForeground    := <iforeground>     ;] ;
               [ :Padding           := <padding>          ]

#xcommand ENDELEMENT => END

#xcommand ENDTHEME => END

#endif