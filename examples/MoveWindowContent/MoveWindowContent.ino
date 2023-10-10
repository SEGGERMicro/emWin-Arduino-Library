#include "DIALOG.h"

/*********************************************************************
*
*       _aColor: Color for each page
*/
static const GUI_COLOR _aColor[] = {
  GUI_BLUE,
  GUI_YELLOW,
  GUI_RED,
  GUI_GREEN
};

/*********************************************************************
*
*       _cbWin
*/
static void _cbWin(WM_MESSAGE * pMsg) {
  GUI_RECT         Rect;
  WM_MOTION_INFO * pInfo;
  int              i;
  int              xDist;
  int              xPos;

  switch (pMsg->MsgId) {
  case WM_MOTION:
    pInfo = (WM_MOTION_INFO *)pMsg->Data.p;
    switch (pInfo->Cmd) {
    case WM_MOTION_INIT:
      WM_GetClientRectEx(pMsg->hWin, &Rect);
      //
      // Snap window at each quarter of the entire window
      // Therefore we get several 'pages', each as big as the screen
      //
      pInfo->SnapX = (Rect.x1 + 1) / 4;
      break;
    }
    break;
  case WM_PAINT:
    GUI_Clear();
    WM_GetClientRect(&Rect);
    xDist = (Rect.x1 + 1) / 4;
    for (i = 0; i < 4; i++) {
      xPos = i * xDist;
      //
      // Draw page
      //
      GUI_SetColor(_aColor[i]);
      GUI_FillRect(xPos, 0, xPos + xDist - 1, Rect.y1);
      //
      GUI_SetColor(GUI_BLACK);
      GUI_SetPenSize(2);
      GUI_DrawRect(xPos, 0, xPos + xDist - 1, Rect.y1);
      GUI_SetFont(GUI_FONT_32B_ASCII);
      GUI_SetTextMode(GUI_TM_TRANS);
      GUI_DispStringAt("Page #", xPos + 40, Rect.y1 / 2 - 16);
      GUI_DispDecMin(i);
    }
    break;
  default:
    WM_DefaultProc(pMsg);
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
  int xSize;
  int ySize;

  //
  // Start GUI
  //
  GUI_Init();
  WM_MULTIBUF_Enable(1);
  WM_SetCallback(WM_HBKWIN, _cbBk);
  //
  // Enable motion support
  //
  WM_MOTION_Enable(1);
  //
  // Enable multi-buffering to avoid flickering.
  //
  WM_MULTIBUF_Enable(1);
  //
  // Set size of Window Manager
  // This is necessary since this sample does not include an actual parent window
  // By default the background window has a size of 16383 x 16383
  //
  xSize = LCD_GetXSize();
  ySize = LCD_GetYSize();
  WM_SetSize(WM_HBKWIN, xSize, ySize);
  //
  // Create a window
  // The flag WM_CF_MOTION_X tells the window that it can be moved via the X-axis.
  //
  WM_CreateWindowAsChild(0, 0, LCD_GetXSize() * 4, LCD_GetYSize(), WM_HBKWIN, WM_CF_SHOW | WM_CF_MOTION_X, _cbWin, 0);

}

void loop() {
  // put your main code here, to run repeatedly:
  GUI_Exec();  // Keep emWin alive, handle touch and other stuff
}
