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
File        : MULTIEDIT_Private.h
Purpose     : MULTIEDIT include
--------------------END-OF-HEADER-------------------------------------
*/

#ifndef MULTIEDIT_PRIVATE_H
#define MULTIEDIT_PRIVATE_H

#include "GUI_Private.h"
#include "WM_Intern.h"

#if GUI_WINSUPPORT

#include <stddef.h>

#include "MULTIEDIT.h"

#if defined(__cplusplus)
  extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*       Object definition
*
**********************************************************************
*/

#define NUM_DISP_MODES 2

/*********************************************************************
*
*       Invalid flags
*
*  Used for partial invalidation. Stored in pObj->InvalidFlags.
*/
#define INVALID_NUMCHARS (1 << 0)
#define INVALID_NUMLINES (1 << 1)
#define INVALID_TEXTSIZE (1 << 2)
#define INVALID_CURSORXY (1 << 3)
#define INVALID_LINEPOSB (1 << 4)

//
// MULTIEDIT properties
//
typedef struct {
  GUI_COLOR        aBkColor    [NUM_DISP_MODES];
  GUI_COLOR        aColor      [NUM_DISP_MODES];
  GUI_COLOR        aCursorColor[2];
  U16              Align;
  const GUI_FONT * pFont;
  U8               HBorder;
} MULTIEDIT_PROPS;

//
// MULTIEDIT object
//
typedef struct {
  WIDGET           Widget;
  MULTIEDIT_PROPS  Props;
  WIDGET_COPY      Copy;
  WM_HMEM          hText;
  U32              MaxNumChars;         // Maximum number of characters including the prompt
  U32              NumChars;            // Number of characters (text and prompt) in object
  U32              NumCharsPrompt;      // Number of prompt characters
  U32              NumLines;            // Number of text lines needed to show all data
  U16              TextSizeX;           // Size in X of text depending of wrapping mode
  U32              BufferSize;
  U32              CursorLine;          // Number of current cursor line
  U32              CursorPosChar;       // Character offset number of cursor
  U32              CursorPosByte;       // Byte offset number of cursor
  I16              CursorPosX;          // Cursor position in X
  I16              CursorPosY;          // Cursor position in Y
  U32              CacheLinePosByte;
  U32              CacheLineNumber;
  U32              CacheFirstVisibleLine;
  U32              CacheFirstVisibleByte;
  WM_SCROLL_STATE  ScrollStateV;
  WM_SCROLL_STATE  ScrollStateH;
  U16              Flags;
  WM_HTIMER        hTimer;
  GUI_WRAPMODE     WrapMode;
  int              MotionPosY;
  int              MotionPosOld;
  WM_HMEM          hContext;             // Motion context.
  U8               CursorVis;            // Indicates whether cursor is visible or not
  U8               InvertCursor;
  U8               InvalidFlags;         // Flags to save validation status
  U8               EditMode;
  U8               MotionActive;
  U8               Radius;               // Currently only used by AppWizard
  U8               Frame;
  U8               Up;
#if GUI_SUPPORT_MEMDEV
  GUI_MEMDEV_Handle ahMemCursor[2];
#endif
} MULTIEDIT_OBJ;

/*********************************************************************
*
*       Private (module internal) data
*
**********************************************************************
*/
extern MULTIEDIT_PROPS MULTIEDIT_DefaultProps;

#if defined(__cplusplus)
  }
#endif

#endif  // GUI_WINSUPPORT
#endif  // MULTIEDIT_PRIVATE_H

/*************************** End of file ****************************/
