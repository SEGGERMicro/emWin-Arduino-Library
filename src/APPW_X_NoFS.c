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
File        : APPW_X_emFile.c
Purpose     : emFile file system access
---------------------------END-OF-HEADER------------------------------
*/

#if !defined(WIN32) && !defined(APP_WIZARD)

#include "AppWizard.h"

#include <string.h>

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

/*********************************************************************
*
*       _FileAccess
*/
//static APPW_X_FILEACCESS _FileAccess = {
//  NULL,
//  NULL,
//  NULL,
//  NULL,
//  NULL,
//};

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       APPW_X_FS_Init
*/
void APPW_X_FS_Init(void) {
  //APPW_SetFileAccess(&_FileAccess);
}

#endif

/*************************** End of file ****************************/
