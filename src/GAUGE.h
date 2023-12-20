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
Purpose     : GAUGE public header file (API)
--------------------END-OF-HEADER-------------------------------------
*/

#ifndef GAUGE_H
#define GAUGE_H

#include "WM.h"
#include "DIALOG_Type.h"      /* Req. for Create indirect data structure */
#include "WIDGET.h"

#if GUI_WINSUPPORT

#if defined(__cplusplus)
  extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*       GAUGE curved flags
*
*  Description
*    With these flags the drawing of the widget's arc lines can be set to have
*    round edges. The flags can be used upon creation of the GAUGE widget.
*/
#define GAUGE_CURVED_VALUE  (1 << 0)  // The arc that is drawn for the GAUGE's value will have a
                                      // curved edge on the beginning and end of the line.
#define GAUGE_CURVED_END    (1 << 1)  // The background arc will be drawn with a curved edge on the
                                      // beginning and end of the line.
#define GAUGE_DIRECTION_CCW (1 << 2)  // Widget works counterclockwise

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
typedef WM_HMEM GAUGE_Handle;

/*********************************************************************
*
*       Create function(s)

  Note: the parameters to a create function may vary.
         Some widgets may have multiple create functions
*/
GAUGE_Handle GAUGE_CreateUser    (int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id, int NumExtraBytes);
GAUGE_Handle GAUGE_CreateIndirect(const GUI_WIDGET_CREATE_INFO * pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK * cb);

/*********************************************************************
*
*       The callback ...
*
* Do not call it directly ! It is only to be used from within an
* overwritten callback.
*/
void GAUGE_Callback(WM_MESSAGE *pMsg);

/*********************************************************************
*
*       Member functions
*
**********************************************************************
*/
void GAUGE_EnableCCW      (GAUGE_Handle hObj, int OnOff);
I32  GAUGE_GetValue       (GAUGE_Handle hObj);
void GAUGE_GetRange       (GAUGE_Handle hObj, I32 * pAng0, I32 * pAng1);
void GAUGE_SetAlign       (GAUGE_Handle hObj, int Align);
void GAUGE_SetBkColor     (GAUGE_Handle hObj, GUI_COLOR BkColor);
void GAUGE_SetColor       (GAUGE_Handle hObj, unsigned Index, GUI_COLOR Color);
void GAUGE_SetWidth       (GAUGE_Handle hObj, unsigned Index, int Width);
void GAUGE_SetOffset      (GAUGE_Handle hObj, int xOff, int yOff);
void GAUGE_SetRadius      (GAUGE_Handle hObj, int Radius);
void GAUGE_SetRange       (GAUGE_Handle hObj, I32 Ang0, I32 Ang1);
void GAUGE_SetRoundedEnd  (GAUGE_Handle hObj, int OnOff);
void GAUGE_SetRoundedValue(GAUGE_Handle hObj, int OnOff);
void GAUGE_SetValueRange  (GAUGE_Handle hObj, I32 Min, I32 Max);
void GAUGE_SetValue       (GAUGE_Handle hObj, I32 Value);

#if defined(__cplusplus)
  }
#endif

#endif  // GUI_WINSUPPORT
#endif  // GAUGE_H

/*************************** End of file ****************************/

