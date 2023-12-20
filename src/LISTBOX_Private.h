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
File        : LISTBOX_Private.h
Purpose     : Private LISTBOX include
--------------------END-OF-HEADER-------------------------------------
*/

#ifndef LISTBOX_PRIVATE_H
#define LISTBOX_PRIVATE_H

#include "GUI_Private.h"
#include "WM_Intern.h"
#include "LISTBOX.h"
#include "GUI_ARRAY.h"
#include "WIDGET.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define LISTBOX_ITEM_SELECTED         (1 << 0)
#define LISTBOX_ITEM_DISABLED         (1 << 1)
//
// Private flags
//
#define LISTBOX_MOTION_STARTED        (1 << 0)   // Set while motion is running.
#define LISTBOX_MOTION_PID_PRESSED    (1 << 1)   // Set if PID is pressed during motion.
#define LISTBOX_TIMER_SNAPPING        (1 << 2)   // Set at the start of a manual snapping operation (_SnapToNearestItem). Cleared when it is done.

/*********************************************************************
*
*       Object definition
*
**********************************************************************
*/
typedef struct {
  U16  xSize, ySize;
  I32  ItemPosY;
  U8   Status;
  char acText[1];
} LISTBOX_ITEM;

typedef struct {
  const GUI_FONT * pFont;
  U16              ScrollStepH;
  GUI_COLOR        aBackColor[4];
  GUI_COLOR        aTextColor[4];
  GUI_COLOR        aScrollbarColor[3];
  I16              Align;
} LISTBOX_PROPS;

typedef struct {
  WIDGET                  Widget;
  GUI_ARRAY               ItemArray;
  WIDGET_DRAW_ITEM_FUNC * pfDrawItem;
  WM_SCROLL_STATE         ScrollStateV;
  WM_SCROLL_STATE         ScrollStateH;
  LISTBOX_PROPS           Props;
  WIDGET_COPY             Copy;
  WM_HWIN                 hOwner;
  int                     MotionPosY;
  int                     TotalRowHeight;            // Cached value
  int                     yOffset;                   // Cached value
  WM_HMEM                 hContext;                  // Motion context
  GUI_TIMER_HANDLE        hTimer;                    // Timer for motion snapping
  I16                     Sel;                       // current selection
  U16                     ItemSpacing;
  U16                     ContentSizeX;
  U16                     FixedScrollPos;
  U8                      Flags;
  U8                      ScrollbarWidth;
  U8                      FlagsIntern;               // Internal flags, see above
} LISTBOX_Obj;

/*********************************************************************
*
*       Macros for internal use
*
**********************************************************************
*/
#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  #define LISTBOX_INIT_ID(p) p->Widget.DebugId = WIDGET_TYPE_LISTBOX
#else
  #define LISTBOX_INIT_ID(p)
#endif

#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  LISTBOX_Obj * LISTBOX_LockH(LISTBOX_Handle h);
  #define LISTBOX_LOCK_H(h)   LISTBOX_LockH(h)
#else
  #define LISTBOX_LOCK_H(h)   (LISTBOX_Obj *)WM_LOCK_H(h)
#endif

#define LISTBOX_H2P(h)       ((LISTBOX_Obj *)WM_H2P(h))

/*********************************************************************
*
*       Private (module internal) data
*
**********************************************************************
*/

extern LISTBOX_PROPS LISTBOX_DefaultProps;

/*********************************************************************
*
*       Private (module internal) functions
*
**********************************************************************
*/
int          LISTBOX__GetItemPosY           (LISTBOX_Handle hObj, unsigned Index);
int          LISTBOX__GetItemSizeY          (LISTBOX_Handle hObj, unsigned Index);
unsigned     LISTBOX__GetNumItems           (const LISTBOX_Obj * pObj);
const char * LISTBOX__GetpStringLocked      (LISTBOX_Handle hObj, int Index, LISTBOX_ITEM ** ppItem);
void         LISTBOX__InvalidateInsideArea  (LISTBOX_Handle hObj);
void         LISTBOX__InvalidateItem        (LISTBOX_Handle hObj, int Sel);
void         LISTBOX__InvalidateItemAndBelow(LISTBOX_Handle hObj, int Sel);
void         LISTBOX__InvalidateItemSize    (const LISTBOX_Obj * pObj, unsigned Index);
void         LISTBOX__SetScrollbarColor     (LISTBOX_Handle hObj, const LISTBOX_Obj * pObj);
void         LISTBOX__SetScrollbarWidth     (LISTBOX_Handle hObj, const LISTBOX_Obj * pObj);
void         LISTBOX__AddSize               (LISTBOX_Obj * pObj, int Index);

#endif /* GUI_WINSUPPORT */

#endif /* LISTBOX_PRIVATE_H */

/*************************** End of file ****************************/
