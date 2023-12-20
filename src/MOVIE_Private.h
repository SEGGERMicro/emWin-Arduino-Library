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
File        : MOVIE_Private.h
Purpose     : MOVIE private header file
--------------------END-OF-HEADER-------------------------------------
*/

#ifndef MOVIE_PRIVATE_H
#define MOVIE_PRIVATE_H

#include "WM_Intern.h"
#include "MOVIE.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define MOVIE_FLAG_DOLOOP         (1 << 0)
#define MOVIE_FLAG_TIMER          (1 << 1)
#define MOVIE_FLAG_PROGBARPRESSED (1 << 2)

/*********************************************************************
*
*       Object definition
*
**********************************************************************
*/
typedef struct {
  GUI_ANIM_HANDLE hAnim;
  int             Start;
  int             End;
  I16             Pos;
} MOVIE_ANIM_DATA;

typedef struct {
  GUI_COLOR aColor[5];
  int       aPeriod[3];
  int       ySizePanel;
  int       ySizeBar;
  int       Space;
} MOVIE_PROPS;

typedef struct {
  WIDGET           Widget;
  MOVIE_PROPS      Props;
  GUI_MOVIE_INFO   Info;
  GUI_MOVIE_HANDLE hMovie;
  WM_HMEM          ahDrawObj[4];
  int              yOffPanel;
  WM_HTIMER        hTimerPanel;
  MOVIE_ANIM_DATA  AnimShift;
  GUI_MOVIE_FUNC * pfNotify;
  U8               Flags;
  U8               BitmapIndex;
  U8               PanelVisible;
} MOVIE_OBJ;

/*********************************************************************
*
*       Macros for internal use
*
**********************************************************************
*/
#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  #define MOVIE_INIT_ID(p) (p->Widget.DebugId = WIDGET_TYPE_MOVIE)
#else
  #define MOVIE_INIT_ID(p)
#endif

#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  MOVIE_OBJ * MOVIE_LockH(MOVIE_Handle h);
  #define MOVIE_LOCK_H(h)   MOVIE_LockH(h)
#else
  #define MOVIE_LOCK_H(h)   (MOVIE_OBJ *)WM_LOCK_H(h)
#endif

#endif   /* GUI_WINSUPPORT */
#endif   /* MOVIE_PRIVATE_H */

/*************************** End of file ****************************/
