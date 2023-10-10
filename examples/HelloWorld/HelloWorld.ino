#include <DIALOG.h>  // This header allows to use any emWin feature (including GUI.h would be sufficient for a simple hello world)

void setup() {
  GUI_Init();                    // Init emWin
  GUI_MULTIBUF_Begin();
  GUI_SetBkColor(GUI_BLACK);     // Set background color
  GUI_Clear();                   // Clear entire screen with previously set BK color
  GUI_DispString("Hello world"); // Show something
  GUI_MULTIBUF_End();
}

void loop() {
  // put your main code here, to run repeatedly:
  GUI_Exec();  // Keep emWin alive, handle touch and other stuff
}
