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
File        : TEXT.h
Purpose     : TEXT include
--------------------END-OF-HEADER-------------------------------------
*/

#ifndef TEXT_H
#define TEXT_H

#include "WM.h"
#include "DIALOG_Type.h"      /* Req. for Create indirect data structure */
#include "WIDGET.h"

#if GUI_WINSUPPORT

#if defined(__cplusplus)
  extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/************************************************************
*
*       #defines
*
*************************************************************
*/

/************************************************************
*
*       Create / Status flags
*/
#define TEXT_CF_LEFT    GUI_TA_LEFT
#define TEXT_CF_RIGHT   GUI_TA_RIGHT
#define TEXT_CF_HCENTER GUI_TA_HCENTER

#define TEXT_CF_VCENTER GUI_TA_VCENTER
#define TEXT_CF_TOP     GUI_TA_TOP
#define TEXT_CF_BOTTOM  GUI_TA_BOTTOM


/*********************************************************************
*
*       Public Types
*
**********************************************************************

*/
typedef WM_HMEM TEXT_Handle;

/*********************************************************************
*
*       Create functions
*
**********************************************************************
*/
TEXT_Handle TEXT_Create        (int x0, int y0, int xSize, int ySize, int Id, int Flags, const char * s, int Align);
TEXT_Handle TEXT_CreateAsChild (int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int Id, int Flags, const char * s, int Align);
TEXT_Handle TEXT_CreateEx      (int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id, const char * pText);
TEXT_Handle TEXT_CreateUser    (int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id, const char * pText, int NumExtraBytes);
TEXT_Handle TEXT_CreateIndirect(const GUI_WIDGET_CREATE_INFO * pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK * cb);

/*********************************************************************
*
*       The callback ...
*
* Do not call it directly ! It is only to be used from within an
* overwritten callback.
*/
void TEXT_Callback(WM_MESSAGE * pMsg);

/*********************************************************************
*
*       Member functions
*
**********************************************************************
*/

/* Methods changing properties */

GUI_COLOR            TEXT_GetBkColor   (TEXT_Handle hObj); 
const GUI_FONT *     TEXT_GetFont      (TEXT_Handle hObj);
int                  TEXT_GetNumLines  (TEXT_Handle hObj);
#if GUI_SUPPORT_ROTATION
const GUI_ROTATION * TEXT_GetRotation  (TEXT_Handle hObj);
#endif
int                  TEXT_GetText      (TEXT_Handle hObj, char * pDest, U32 BufferSize);
int                  TEXT_GetTextAlign (TEXT_Handle hObj);
GUI_COLOR            TEXT_GetTextColor (TEXT_Handle hObj);
GUI_COLOR            TEXT_GetFrameColor(TEXT_Handle hObj);
void                 TEXT_GetTextOffset(TEXT_Handle hObj, int * pxPos, int * pyPos);
int                  TEXT_GetUserData  (TEXT_Handle hObj, void * pDest, int NumBytes);
GUI_WRAPMODE         TEXT_GetWrapMode  (TEXT_Handle hObj);
void                 TEXT_SetBkColor   (TEXT_Handle hObj, GUI_COLOR Color);
void                 TEXT_SetFont      (TEXT_Handle hObj, const GUI_FONT * pFont);
int                  TEXT_SetDec       (TEXT_Handle hObj, I32 v, U8 Len, U8 Shift, U8 Signed, U8 Space);
int                  TEXT_SetHex       (TEXT_Handle hObj, I32 v, U8 Len);
#if GUI_SUPPORT_ROTATION
void                 TEXT_SetRotation  (TEXT_Handle hObj, const GUI_ROTATION * pLCD_Api);
#endif
int                  TEXT_SetText      (TEXT_Handle hObj, const char * s);
void                 TEXT_SetTextAlign (TEXT_Handle hObj, int Align);
void                 TEXT_SetTextColor (TEXT_Handle hObj, GUI_COLOR Color);
void                 TEXT_SetFrameColor(TEXT_Handle hObj, GUI_COLOR Color);
void                 TEXT_SetTextOffset(TEXT_Handle hObj, int xPos, int yPos);
int                  TEXT_SetUserData  (TEXT_Handle hObj, const void * pSrc, int NumBytes);
void                 TEXT_SetWrapMode  (TEXT_Handle hObj, GUI_WRAPMODE WrapMode);

int                  TEXT__SetDec      (TEXT_Handle hObj, I32 v, U8 Len, U8 Shift, U8 Signed, U8 Space, I32 Min, I32 Max);

/*********************************************************************
*
*       Managing default values
*
**********************************************************************
*/

const GUI_FONT *     TEXT_GetDefaultFont      (void);
GUI_COLOR            TEXT_GetDefaultFrameColor(void);
#if GUI_SUPPORT_ROTATION
const GUI_ROTATION * TEXT_GetDefaultRotation  (void);
#endif
GUI_COLOR            TEXT_GetDefaultTextColor (void);
GUI_WRAPMODE         TEXT_GetDefaultWrapMode  (void);
void                 TEXT_SetDefaultFrameColor(GUI_COLOR Color);
void                 TEXT_SetDefaultFont      (const GUI_FONT * pFont);
#if GUI_SUPPORT_ROTATION
const GUI_ROTATION * TEXT_SetDefaultRotation  (const GUI_ROTATION * pLCD_Api);
#endif
void                 TEXT_SetDefaultTextColor (GUI_COLOR Color);
GUI_WRAPMODE         TEXT_SetDefaultWrapMode  (GUI_WRAPMODE WrapMode);

#if defined(__cplusplus)
  }
#endif

#endif  // GUI_WINSUPPORT
#endif  // TEXT_H

/*************************** End of file ****************************/
