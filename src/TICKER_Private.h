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
File        : TICKER.h
Purpose     : TICKER include
--------------------END-OF-HEADER-------------------------------------
*/

#ifndef TICKER_PRIVATE_H
#define TICKER_PRIVATE_H

#include "WM_Intern.h"
#include "TICKER.h"
#include "GUI_ARRAY.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*       Object definition
*
**********************************************************************
*/
typedef struct {
  const GUI_FONT * pFont;
  GUI_COLOR        TextColor;
  GUI_COLOR        BkColor;
  GUI_COLOR        FrameColor;
  U16              TickerDist;
  int              AnimPeriod;
  U16              AnimDist;
} TICKER_PROPS;

typedef struct {
  WIDGET           Widget;
  TICKER_PROPS     Props;
  I16              Align;
  I16              xOffTicker;
  I16              yOffTicker;
  U16              StringDist;
  U16              NumStrings;
  U8               Consecutive;
  GUI_ARRAY        ItemArray;
  void           (*pfPaint)    (TICKER_Handle hObj);
  void           (*pfStartAnim)(TICKER_Handle hObj, int Restart);
  int              Dist2NextItem;
  U16              CurrentItem;
  U16              LastItem;
  GUI_ANIM_HANDLE  hAnim;
  GUI_HMEM         hAnimData;
  U8               Mode;
  U8               Dir;
  U8               Auto;
  WM_HTIMER        hTimer;
  U8               WrapMode;
  U16              ClickedItem;
} TICKER_OBJ;

typedef struct {
  U32       Offset;
  U32       Length;
  char      acText[1];  // This needs to be always the last member as we copy a string to address: &acText[0]
} TICKER_ITEM;

typedef struct {
  int       NewPos;
  int       Dist;
} TICKER_ANIM_DATA;

/*********************************************************************
*
*       Macros for internal use
*
**********************************************************************
*/
#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  #define TICKER_INIT_ID(p) p->Widget.DebugId = WIDGET_TYPE_TICKER
#else
  #define TICKER_INIT_ID(p)
#endif

#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  TICKER_OBJ * TICKER_LockH(TICKER_Handle h);
  #define TICKER_LOCK_H(h)   TICKER_LockH(h)
#else
  #define TICKER_LOCK_H(h)   (TICKER_OBJ *)WM_LOCK_H(h)
#endif

#define TICKER_AUTO            (1 << 0)
#define TICKER_SHOW_PREVITEM   (1 << 7)

/*********************************************************************
*
*       Module internal data
*
**********************************************************************
*/
extern TICKER_PROPS TICKER__DefaultProps;

#endif   /* if GUI_WINSUPPORT */
#endif   /* TICKER_PRIVATE_H */

/*************************** End of file ****************************/
