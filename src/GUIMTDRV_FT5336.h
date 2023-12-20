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
File        : GUIMTDRV_FT5336.h
Purpose     : Multitouch driver for FT5336.
---------------------------END-OF-HEADER------------------------------
*/

#ifndef GUIMTDRV_FT5336_H
#define GUIMTDRV_FT5336_H

#include "GUI_Type.h"

/*********************************************************************
*
*       Defines, fixed
*
**********************************************************************
*/

#define GUIMTDRV_FT5336_TOUCH_MAX  10

/*********************************************************************
*
*       Types
*
**********************************************************************
*/

typedef struct {
  unsigned         NumTouch;
  GUI_MTOUCH_INPUT aTouch[GUIMTDRV_FT5336_TOUCH_MAX];
} GUIMTDRV_FT5336_STATE;

typedef struct {
  //
  int LayerIndex;
  //
  // Initialization
  //
  void (*pf_I2C_Init) (unsigned SlaveAddr);
  //
  // Read- and write access
  //
  void (*pf_I2C_Read)  (unsigned SlaveAddr,       U8 *pData, int Addr, int Len);
  void (*pf_I2C_Write) (unsigned SlaveAddr, const U8 *pData, int Addr, int Len);
  //
  // Optionally translate controller frame of reference to emWin frame of reference.
  //
  void (*pf_Translate) (GUI_MTOUCH_INPUT *pCoord);
  //
  // 7-bit address to be used to address the I2C slave
  //
  unsigned SlaveAddr;
} GUIMTDRV_FT5336_CONFIG;

/*********************************************************************
*
*       Interface
*
**********************************************************************
*/

int  GUIMTDRV_FT5336_Exec          (void);
void GUIMTDRV_FT5336_GetTouchState (GUIMTDRV_FT5336_STATE *pState);
void GUIMTDRV_FT5336_Init          (const GUIMTDRV_FT5336_CONFIG *pConfig);
void GUIMTDRV_FT5336_SetLayerIndex (int LayerIndex);

#endif
