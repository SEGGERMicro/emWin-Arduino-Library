#include "DIALOG.h"

#define BUTTON_SIZE 50

/*********************************************************************
*
*       _cbButton
*/
static void _cbButton(WM_MESSAGE * pMsg) {
  static GUI_RECT Rect;

  switch (pMsg->MsgId) {
  case WM_PAINT:
    //
    // Draw button
    //
    if (BUTTON_IsPressed(pMsg->hWin)) {
      GUI_SetColor(GUI_LIGHTRED);
    }
    else {
      GUI_SetColor(GUI_GRAY_C8);
    }
    GUI_FillCircle(BUTTON_SIZE / 2, BUTTON_SIZE / 2, BUTTON_SIZE / 2 - 1);
    GUI_SetColor(GUI_BLACK);
    GUI_SetPenSize(2);
    GUI_AA_DrawCircle(BUTTON_SIZE / 2, BUTTON_SIZE / 2, BUTTON_SIZE / 2 - 1);
    GUI_SetPenSize(5);
    GUI_AA_DrawLine(16, 16, 33, 33);
    GUI_AA_DrawLine(33, 16, 16, 33);
    break;
  default:
    BUTTON_Callback(pMsg);
    break;
  }
}

/*********************************************************************
*
*       _cbWin
*/
static void _cbWin(WM_MESSAGE * pMsg) {
  WM_HWIN hButton;

  switch (pMsg->MsgId) {
  case WM_CREATE:
    //
    // Create a button
    //
    hButton = BUTTON_Create(10, 10, BUTTON_SIZE, BUTTON_SIZE, GUI_ID_BUTTON0, WM_CF_SHOW);
    //
    // Set callback routine
    //
    WM_SetCallback(hButton, _cbButton);
    break;
  case WM_PAINT:
    GUI_SetBkColor(GUI_WHITE);
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
  //
  // Create a window
  //
  WM_CreateWindowAsChild(0, 0, LCD_GetXSize(), LCD_GetYSize(), WM_HBKWIN, WM_CF_SHOW, _cbWin, 0);

}

void loop() {
  // put your main code here, to run repeatedly:
  GUI_Exec();  // Keep emWin alive, handle touch and other stuff
}
