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
File        : PIDConf.c
Purpose     : Touch screen controller configuration
---------------------------END-OF-HEADER------------------------------
*/
#include "Arduino_GigaDisplayTouch.h"

#include "WM.h"
#include "PIDConf.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static int _IsInitialized;
static int _TouchX;
static int _TouchY;
static int _TouchPressed;
static int _TimeLastPress;
static int _LayerIndex;

Arduino_GigaDisplayTouch touch;

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       PID_X_Exec
*/
static void PID_X_Exec(void) {
  static int IsTouched;
  static GUI_PID_STATE StatePID;
  int td;

  StatePID.Layer = _LayerIndex;
  td = GUI_GetTime() - _TimeLastPress;
  if (_TouchPressed && (td < 25)) {
    IsTouched = 1;
    StatePID.Pressed = 1;
    StatePID.x = (int)(_TouchX);
    StatePID.y = (int)(_TouchY);
    GUI_PID_StoreState(&StatePID);
  } else {
    if (IsTouched == 1) {
      IsTouched = 0;
      StatePID.Pressed = 0;
      GUI_PID_StoreState(&StatePID);
      _TouchPressed = false;
    }
  }
}

/*********************************************************************
*
*       _HandleTouch
*/
static void _HandleTouch(uint8_t contacts, GDTpoint_t* points) {
  _TouchPressed = true;
  _TimeLastPress = GUI_GetTime();
  for (uint8_t i = 0; i < contacts; i++) {
    if (i == 0) {
      _TouchX = points[i].x;
      _TouchY = points[i].y;
    }
  }
}

/*********************************************************************
*
*       _TouchInit
*/
static void _TouchInit() {
  touch.begin();
  touch.onDetect(_HandleTouch);
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       PID_X_SetLayerIndex
*/
void PID_X_SetLayerIndex(int LayerIndex) {
  _LayerIndex = LayerIndex;
}

/*********************************************************************
*
*       PID_X_Init
*/
void PID_X_Init(void) {
  if (_IsInitialized == 0) {
    _TouchInit();
    WM_SetpfPollPID((WM_tfPollPID *)PID_X_Exec);
    _IsInitialized = 1;
  }
}

/*************************** End of file ****************************/
