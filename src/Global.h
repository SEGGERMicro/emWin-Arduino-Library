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

File    : Global.h
Purpose : Global types
          In case your application already has a Global.h, you should
          merge the files. In order to use Segger code, the types
          U8, U16, U32, I8, I16, I32 need to be defined in Global.h;
          additional definitions do not hurt.
Revision: $Rev: 32989 $

-------------------------- END-OF-HEADER -----------------------------
*/

#ifndef GLOBAL_H            // Guard against multiple inclusion
#define GLOBAL_H

#define U8    unsigned char
#define I8    signed char
#define U16   unsigned short
#define I16   signed short
#if defined(__x86_64__) || defined(__aarch64__)
#define U32   unsigned int
#define I32   int
#else
#define U32   unsigned long
#define I32   signed long
#endif

//
// CC_NO_LONG_SUPPORT can be defined to compile test
// without long support for compilers that do not
// support C99 and its long type.
//
#ifdef CC_NO_LONG_SUPPORT
  #define PTR_ADDR  U32
#else  // Supports long type.
  #if defined(_MSC_VER)
    //
    // Microsoft VC6 and newer.
    // Older versions exist (_MSC_VER <= 1200) but are
    // not even officially listed by Microsoft and might
    // not work with these defines.
    //
    #define U64   unsigned __int64
    #define U128  unsigned __int128
    #define I64   __int64
    #define I128  __int128
    #if (_MSC_VER <= 1200)
      #define U64_C(x) x##UI64
    #else
      #define U64_C(x) x##ULL
    #endif
  #else
    //
    // C99 compliant compiler.
    //
    #define U64   unsigned long long
    #define I64   signed long long
    #define U64_C(x) x##ULL
  #endif
  //
  // Types for pointer<->address conversions.
  //
  #if (defined(_WIN64) || defined(__LP64__))  // 64-bit symbols used by Visual Studio and GCC, maybe others as well.
    #define PTR_ADDR  U64
  #else
    #define PTR_ADDR  U32
  #endif
#endif  // Supports long type.

#endif                      // Avoid multiple inclusion

/*************************** End of file ****************************/
