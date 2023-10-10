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
----------------------------------------------------------------------
File        : PIDConf.h
Purpose     : Touch screen controller interface
---------------------------END-OF-HEADER------------------------------
*/

#ifndef PIDCONF_H
#define PIDCONF_H

#if defined(__cplusplus)
  extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

void PID_X_Init(void);
void PID_X_SetLayerIndex(int LayerIndex);

#if defined(__cplusplus)
  }
#endif

#endif // PIDCONF_H
