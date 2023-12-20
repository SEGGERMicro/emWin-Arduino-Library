# emWin-Arduino-Library
This package includes the embedded graphics library emWin.

This packages was set up for the Arduino GIGA R1 in combination with the Arduino GIGA Display Shield. Since the Arduino GIGA R1 is based on an STM32 chip the free of charge STemWin library is used.

The packages can also be used with emWins UI creation tool, the AppWizard.

For further information on emWin please visit www.segger.com/products/user-interface/emwin
For more information on other SEGGER products please visit www.segger.com

The emWin manual can be found here:
https://www.segger.com/doc/UM03001_emWin.html
https://www.segger.com/downloads/emwin/UM03001

The AppWizard and the AppWizard manual can be found under these links:
https://www.segger.com/downloads/emwin/
https://www.segger.com/doc/UM03003_AppWizard.html

Please make sure to use the correct AppWizard version. This package includes the emWin library V6.36 which matches AppWizard version 1.42_6.36.

# Getting started
To get started with emWin simply include "DIALOG.h" which makes any widget as well as the Window Manager and core functionality of emWin available.

After that call GUI_Init() in setup() and add GUI_Exec() to the loop() function.

This might look like below:

```
#include <DIALOG.h>

void setup() {
  GUI_Init();
}

void loop() {
  // put your main code here, to run repeatedly:
  GUI_Exec();  // Keep emWin alive, handle touch and other stuff
}
```

Please note that it is mandatory to start and end multi buffering before the first and last drawing operation. This is only required if you are not using the Window Manager and/or Widgets. If you are using the Window manager a call of WM_MULTIBUF_Enable(1) is sufficient.

Here are two examples:

Without the Window Manager:

```
#include <DIALOG.h>

void setup() {
  GUI_Init();
  GUI_MULTIBUF_Begin();
  GUI_SetBkColor(GUI_BLACK);
  GUI_Clear();
  GUI_DispString("Hello world");
  GUI_MULTIBUF_End();
}

void loop() {
  GUI_Exec();
}
```

And one example with the Window Manager:

```
#include "DIALOG.h"

static void _cbWin(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
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
  GUI_Init();
  WM_MULTIBUF_Enable(1);
  WM_CreateWindowAsChild(30, 30, 160, 60, WM_HBKWIN, WM_CF_SHOW, _cbWin, 0);
}

void loop() {
  GUI_Exec();
}
```

You might also refer to the examples which come along with this library package.

# Running emWin examples

It is possible to port most emWin examples from our website to your Arduino GIGA R1 project. Please note, that some examples are simply not made for the screen dimensions of the Arduino GIGA Display Shield and it might not work as expected.

To get hands on emWin examples you might either refer to our examples website:
https://www.segger.com/products/user-interface/emwin/technology/samples/

Or our Wiki:
https://wiki.segger.com/emWin_Examples

Most emWin examples are independent of the display dimensions and can be ported into your Arduino project.

emWin examples typically start in a function named MainTask(). In most cases emWin and the application will be initialized in MainTask() and will end in an endless loop which keeps emWin "alive".

To port the example simply copy the content of MainTask() into the setup() function (except the while(1)-loop) and copy the content of the while(1)-loop into the loop() function. You might want to replace GUI_Delay() by GUI_Exec() which is basically the same but without performing a delay.

Of course you have to copy the rest of the example content into your .ino file, too.

To get an idea on how this can be done, you can compare the "CreateWindow" example from this package with the WM_CreateWindows example from our Wiki:
https://wiki.segger.com/WM_create_windows_(Sample)

