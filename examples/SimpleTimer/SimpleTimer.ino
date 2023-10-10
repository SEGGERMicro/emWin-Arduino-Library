#include "DIALOG.h"

#define ID_TIMER 0
#define PERIOD   500

/*********************************************************************
*
*       _cbWin
*/
static void _cbWin(WM_MESSAGE * pMsg) {
  int              Id;
  char             acString[64];
  static WM_HTIMER hTimer; // Timer handle has to be static
  static int       Counter;

  switch (pMsg->MsgId) {
  case WM_CREATE:
    //
    // Create timer handle
    //
    //   Mode (unused, should be 0) ----------------------+
    //   Duration ---------------------------------+      |
    //   User-defined ID -----------------+        |      |
    //   Window to be informed -+         |        |      |
    //                          |         |        |      |
    hTimer = WM_CreateTimer(pMsg->hWin, ID_TIMER, PERIOD, 0);
    break;
  case WM_PAINT:
    //
    // This case is called everytime the window has to be redrawn
    //
    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();
    GUI_SetColor(GUI_BLACK);
    sprintf(acString, "The timer has expired %d times.", Counter);
    GUI_DispStringAt(acString, 10, 10);
    //
    if (hTimer == 0) {
      GUI_DispStringAt("The timer has been deleted.", 10, 30);
    }
    break;
  case WM_TIMER:                      // When a timer expires, a WM_TIMER message is sent to the window
    Id = WM_GetTimerId(pMsg->Data.v); // The handle of the timer that expired is sent via the data value of the message.
    switch (Id) {
    case ID_TIMER:
      Counter++;
      if (Counter < 10) {
        //
        // Restart the timer
        //
        WM_RestartTimer(hTimer, PERIOD);
      }
      else {
        //
        // Delete the timer
        //
        WM_DeleteTimer(hTimer);
        hTimer = 0;
      }
      WM_InvalidateWindow(pMsg->hWin);
      break;
    }
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
  //
  // Start GUI
  //
  GUI_Init();
  WM_MULTIBUF_Enable(1);
  WM_SetCallback(WM_HBKWIN, _cbBk);
  //
  // Create a window
  //
  WM_CreateWindowAsChild(0, 0, LCD_GetXSize(), LCD_GetYSize(), WM_HBKWIN, WM_CF_SHOW, _cbWin, 0);

}

void loop() {
  // put your main code here, to run repeatedly:
  GUI_Exec();  // Keep emWin alive, handle touch and other stuff
}
