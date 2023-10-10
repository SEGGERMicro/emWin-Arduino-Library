#include "DIALOG.h"

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

/*********************************************************************
*
*       _cbWin
*
* Function description
*   Callback routine for the window created in MainTask. All messages
*   sent to the sample window are handled in this routine.
*
* Parameters
*   pMsg   : Pointer to a message structure sent to the window.
*/
static void _cbWin(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
  case WM_CREATE:
    //
    // We land here right after the window has been created.
    //
    break;
  case WM_PAINT:
    //
    // This case is called everytime the window has to be redrawn
    //
    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();
    GUI_SetColor(GUI_BLACK);
    GUI_DispStringAt("Sample window", 10, 10);
    break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       _cbBk
*/
static void _cbBk(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
  case WM_PAINT:
    GUI_SetBkColor(GUI_BLACK);
    GUI_Clear();
    break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

void setup() {
  GUI_Init();
  WM_MULTIBUF_Enable(1);
  WM_SetCallback(WM_HBKWIN, _cbBk);
  //
  // Create a window as a child of the background window
  //
  WM_CreateWindowAsChild(30, 30, 160, 60, WM_HBKWIN, WM_CF_SHOW, _cbWin, 0);
}

void loop() {
  // put your main code here, to run repeatedly:
  GUI_Exec();  // Keep emWin alive, handle touch and other stuff
}
