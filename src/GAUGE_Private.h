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
File        : GAUGE.h
Purpose     : GAUGE include
--------------------END-OF-HEADER-------------------------------------
*/

#ifndef GAUGE_PRIVATE_H
#define GAUGE_PRIVATE_H

#include "WM_Intern.h"
#include "GAUGE.h"

#if (GUI_WINSUPPORT)

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define PRE_VOID                   \
  {                                \
    GAUGE_OBJ * pObj;              \
    if (hObj) {                    \
      GUI_LOCK();                  \
      pObj = GAUGE_LOCK_H(hObj); {

#define POST_VOID_INVALIDATE       \
        WM_InvalidateWindow(hObj); \
      } GUI_UNLOCK_H(pObj);        \
      GUI_UNLOCK();                \
    }                              \
  }

#define POST_VOID                  \
      } GUI_UNLOCK_H(pObj);        \
      GUI_UNLOCK();                \
    }                              \
  }

/*********************************************************************
*
*       Object definition
*
**********************************************************************
*/
typedef struct {
  GUI_COLOR aColor[2];
  GUI_COLOR BkColor;
  I16       Align;
  I16       xOff, yOff;
} GAUGE_PROPS;

typedef struct {
  WIDGET      Widget;             // (obvious)
  GAUGE_PROPS Props;              // (obvious)
  int         Radius;             // 
  int         aWidth[2];          // 
  I32         Value;              // 
  I32         Ang0;               // 
  I32         Ang1;               // 
  I32         Angle;              // Current angle within the given range (Ang0 & Ang1)
  I32         MinVRange;          // Minimum of value range
  I32         MaxVRange;          // Maximum of value range
  U8          Flags;
} GAUGE_OBJ;

/*********************************************************************
*
*       Macros for internal use
*
**********************************************************************
*/
#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  #define GAUGE_INIT_ID(p) (p->Widget.DebugId = WIDGET_TYPE_GAUGE)
#else
  #define GAUGE_INIT_ID(p)
#endif

#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  GAUGE_OBJ * GAUGE_LockH(GAUGE_Handle h);
  #define GAUGE_LOCK_H(h) GAUGE_LockH(h)
#else
  #define GAUGE_LOCK_H(h) (GAUGE_OBJ *)WM_LOCK_H(h)
#endif

/*********************************************************************
*
*       Public data (internal defaults)
*
**********************************************************************
*/
extern GAUGE_PROPS GAUGE__DefaultProps;

#endif  // (GUI_SUPPORT_MEMDEV && GUI_WINSUPPORT)
#endif  // GAUGE_PRIVATE_H
