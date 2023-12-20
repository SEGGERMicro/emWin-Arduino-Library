/*********************************************************************
*                SEGGER Microcontroller GmbH                         *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2023  SEGGER Microcontroller GmbH                *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V6.36 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The  software has  been licensed  to STMicroelectronics International
N.V. a Dutch company with a Swiss branch and its headquarters in Plan-
les-Ouates, Geneva, 39 Chemin du Champ des Filles, Switzerland for the
purposes of creating libraries for ARM Cortex-M-based 32-bit microcon_
troller products commercialized by Licensee only, sublicensed and dis_
tributed under the terms and conditions of the End User License Agree_
ment supplied by STMicroelectronics International N.V.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
Licensing information
Licensor:                 SEGGER Software GmbH
Licensed to:              STMicroelectronics International NV, 39, Chemin du Champ-des Filles, 1228 Plan Les Ouates, Geneva, SWITZERLAND
Licensed SEGGER software: emWin
License number:           GUI-00429
License model:            Buyout SRC [Buyout Source Code License, signed November 29th 2012]
Licensed product:         -
Licensed platform:        STMs ARM Cortex-M based 32 BIT CPUs
Licensed number of seats: -
----------------------------------------------------------------------
Support and Update Agreement (SUA)
SUA period:               2012-12-07 - 2023-12-31
Contact to extend SUA:    sales@segger.com
----------------------------------------------------------------------
File        : SWIPELIST.h
Purpose     : SWIPELIST include
--------------------END-OF-HEADER-------------------------------------
*/

#ifndef SWIPELIST_PRIVATE_H
#define SWIPELIST_PRIVATE_H

#include "WM_Intern.h"
#include "SWIPELIST.h"
#include "GUI_ARRAY.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*       Object definition
*
**********************************************************************
*/
typedef struct {
  WM_HWIN hWin;
  int     xPos;
  int     yPos;
} SWIPELIST_WIN;

typedef struct {
  GUI_ARRAY      WinArray;
  GUI_ARRAY      TextArray;
  int            TextSize;
  WM_HMEM        hDrawObj;
  GUI_COLOR      SepColor;
  int            SepSize;
  int            Pos;
  int            Size;
  int            BitmapAlign;
  int            TextAlign;
  U8             Flags;
  U32            UserData;
} SWIPELIST_ITEM;

typedef struct {
  const GUI_FONT * pSepFont;
  const GUI_FONT * pHeaderFont;
  const GUI_FONT * pTextFont;
  GUI_COLOR        aTextColor[5];
  GUI_COLOR        aBkColor[4];
  int              BitmapSpace;
  int              aBorderSize[4];
  int              Period;
  U8               Flags;
  int              Threshold;
  unsigned         Overlap;
} SWIPELIST_PROPS;

typedef struct {
  WIDGET                  Widget;
  SWIPELIST_PROPS         Props;
  WIDGET_DRAW_ITEM_FUNC * pfDrawItem;
  GUI_ARRAY               ItemArray;
  int                     Pos;
  int                     Size;
  int                     FirstVisible;
  int                     LastVisible;
  int                     Sel;
  int                     ReleasedItem;
  WM_HMEM                 hContext;       // Motion context.
} SWIPELIST_OBJ;

/*********************************************************************
*
*       Macros for internal use
*
**********************************************************************
*/
#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  #define SWIPELIST_INIT_ID(p) p->Widget.DebugId = WIDGET_TYPE_SWIPELIST
#else
  #define SWIPELIST_INIT_ID(p)
#endif

#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  SWIPELIST_OBJ * SWIPELIST_LockH(SWIPELIST_Handle h);
  #define SWIPELIST_LOCK_H(h)   SWIPELIST_LockH(h)
#else
  #define SWIPELIST_LOCK_H(h)   (SWIPELIST_OBJ *)WM_LOCK_H(h)
#endif

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
//
// WM_MOTION_OVERLAP... flags are stored in the upper two bits of Props.Flags
//
#define OVERLAP_FLAG_SHIFT     2
#define OVERLAP_FLAG_MASK      ((WM_MOTION_OVERLAP_TOP | WM_MOTION_OVERLAP_BOTTOM) << OVERLAP_FLAG_SHIFT)

/*********************************************************************
*
*       Module internal data
*
**********************************************************************
*/
extern SWIPELIST_PROPS SWIPELIST__DefaultProps;
extern GUI_COLOR       SWIPELIST__DefaultSepColor;
extern int             SWIPELIST__DefaultSepSize;
extern int             SWIPELIST__DefaultTextAlign;

/*********************************************************************
*
*       Private functions
*
**********************************************************************
*/
void SWIPELIST__SetDrawObj   (SWIPELIST_Handle hObj, int Index, int Align, GUI_DRAW_HANDLE hDrawObj);
void SWIPELIST__CalcItemPos  (GUI_ARRAY ItemArray, int StartIndex, int EndIndex, int AddSize);
int  SWIPELIST__CalcItemSize (SWIPELIST_Handle hObj, int ItemIndex);

#endif   /* if GUI_WINSUPPORT */
#endif   /* SWIPELIST_PRIVATE_H */
