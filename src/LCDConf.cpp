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
File        : LCDConf.c
Purpose     : Display controller configuration (single layer)
---------------------------END-OF-HEADER------------------------------
*/

#include "Arduino.h"
#include "st7701.h"
#include "stm32h7xx.h"
#include "SDRAM.h"
#include "dsi.h"

#include "GUI.h"
#include "GUIDRV_Lin.h"
#include "PIDConf.h"

/*********************************************************************
*
*       Layer configuration (to be modified)
*
**********************************************************************
*/
//
// Physical display size
//
#define XSIZE_PHYS _xSize
#define YSIZE_PHYS _ySize

//
// Color conversion
//
#define COLOR_CONVERSION GUICC_M565

//
// Display driver
//
#define DISPLAY_DRIVER_0_0 GUIDRV_LIN_16
#define DISPLAY_DRIVER_0_1 GUIDRV_LIN_OSX_16
#define DISPLAY_DRIVER_0_2 GUIDRV_LIN_OXY_16
#define DISPLAY_DRIVER_0_3 GUIDRV_LIN_OSY_16

#define BYTES_PER_LINE  (2)

#define NUM_LAYERS   1  // Leave it at 1, no support for multi layers in this config for now
#define NUM_BUFFERS  2

/*********************************************************************
*
*       Configuration checking
*
**********************************************************************
*/
#ifndef   XSIZE_PHYS
  #error Physical X size of display is not defined!
#endif
#ifndef   YSIZE_PHYS
  #error Physical Y size of display is not defined!
#endif
#ifndef   COLOR_CONVERSION
  #error Color conversion not defined!
#endif
#ifndef   DISPLAY_DRIVER_0_0
  #error No display driver defined!
#endif

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
typedef struct {
  U32                 address[2];          
  I32                 pending_buffer;   
  I32                 buffer_index;     
  U32                 xSize;            
  U32                 ySize;            
  U32                 BytesPerPixel;
  LCD_API_COLOR_CONV *pColorConvAPI;
} LCD_LayerPropTypedef;

/*********************************************************************
*
*       Redirect bulk conversion to DMA2D routines
*/
#define DEFINE_DMA2D_COLORCONVERSION(PFIX, PIXELFORMAT)                                                        \
static void _Color2IndexBulk_##PFIX##_DMA2D(LCD_COLOR * pColor, void * pIndex, U32 NumItems, U8 SizeOfIndex) { \
  _DMA_Color2IndexBulk(pColor, pIndex, NumItems, SizeOfIndex, PIXELFORMAT);                                    \
}                                                                                                              \
static void _Index2ColorBulk_##PFIX##_DMA2D(void * pIndex, LCD_COLOR * pColor, U32 NumItems, U8 SizeOfIndex) { \
  _DMA_Index2ColorBulk(pIndex, pColor, NumItems, SizeOfIndex, PIXELFORMAT);                                    \
}

#define LTDC_Pixelformat_ARGB8888  ((U32)0x00000000)
#define LTDC_Pixelformat_RGB888    ((U32)0x00000001)
#define LTDC_Pixelformat_RGB565    ((U32)0x00000002)
#define LTDC_Pixelformat_ARGB1555  ((U32)0x00000003)
#define LTDC_Pixelformat_ARGB4444  ((U32)0x00000004)
#define LTDC_Pixelformat_L8        ((U32)0x00000005)
#define LTDC_Pixelformat_AL44      ((U32)0x00000006)
#define LTDC_Pixelformat_AL88      ((U32)0x00000007)
#define LTDC_Pixelformat_L4        ((U32)0x00000008)

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
//
// Array for speeding up nibble conversion for A4 bitmaps
//
static const U8 _aMirror[] = {
  0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 0x90, 0xA0, 0xB0, 0xC0, 0xD0, 0xE0, 0xF0,
  0x01, 0x11, 0x21, 0x31, 0x41, 0x51, 0x61, 0x71, 0x81, 0x91, 0xA1, 0xB1, 0xC1, 0xD1, 0xE1, 0xF1,
  0x02, 0x12, 0x22, 0x32, 0x42, 0x52, 0x62, 0x72, 0x82, 0x92, 0xA2, 0xB2, 0xC2, 0xD2, 0xE2, 0xF2,
  0x03, 0x13, 0x23, 0x33, 0x43, 0x53, 0x63, 0x73, 0x83, 0x93, 0xA3, 0xB3, 0xC3, 0xD3, 0xE3, 0xF3,
  0x04, 0x14, 0x24, 0x34, 0x44, 0x54, 0x64, 0x74, 0x84, 0x94, 0xA4, 0xB4, 0xC4, 0xD4, 0xE4, 0xF4,
  0x05, 0x15, 0x25, 0x35, 0x45, 0x55, 0x65, 0x75, 0x85, 0x95, 0xA5, 0xB5, 0xC5, 0xD5, 0xE5, 0xF5,
  0x06, 0x16, 0x26, 0x36, 0x46, 0x56, 0x66, 0x76, 0x86, 0x96, 0xA6, 0xB6, 0xC6, 0xD6, 0xE6, 0xF6,
  0x07, 0x17, 0x27, 0x37, 0x47, 0x57, 0x67, 0x77, 0x87, 0x97, 0xA7, 0xB7, 0xC7, 0xD7, 0xE7, 0xF7,
  0x08, 0x18, 0x28, 0x38, 0x48, 0x58, 0x68, 0x78, 0x88, 0x98, 0xA8, 0xB8, 0xC8, 0xD8, 0xE8, 0xF8,
  0x09, 0x19, 0x29, 0x39, 0x49, 0x59, 0x69, 0x79, 0x89, 0x99, 0xA9, 0xB9, 0xC9, 0xD9, 0xE9, 0xF9,
  0x0A, 0x1A, 0x2A, 0x3A, 0x4A, 0x5A, 0x6A, 0x7A, 0x8A, 0x9A, 0xAA, 0xBA, 0xCA, 0xDA, 0xEA, 0xFA,
  0x0B, 0x1B, 0x2B, 0x3B, 0x4B, 0x5B, 0x6B, 0x7B, 0x8B, 0x9B, 0xAB, 0xBB, 0xCB, 0xDB, 0xEB, 0xFB,
  0x0C, 0x1C, 0x2C, 0x3C, 0x4C, 0x5C, 0x6C, 0x7C, 0x8C, 0x9C, 0xAC, 0xBC, 0xCC, 0xDC, 0xEC, 0xFC,
  0x0D, 0x1D, 0x2D, 0x3D, 0x4D, 0x5D, 0x6D, 0x7D, 0x8D, 0x9D, 0xAD, 0xBD, 0xCD, 0xDD, 0xED, 0xFD,
  0x0E, 0x1E, 0x2E, 0x3E, 0x4E, 0x5E, 0x6E, 0x7E, 0x8E, 0x9E, 0xAE, 0xBE, 0xCE, 0xDE, 0xEE, 0xFE,
  0x0F, 0x1F, 0x2F, 0x3F, 0x4F, 0x5F, 0x6F, 0x7F, 0x8F, 0x9F, 0xAF, 0xBF, 0xCF, 0xDF, 0xEF, 0xFF,
};

static int _xSize;
static int _ySize;

static DMA2D_HandleTypeDef dma2d;

static   LCD_LayerPropTypedef layer_prop[NUM_LAYERS];

static const LCD_API_COLOR_CONV * _apColorConvAPI[] = {
  COLOR_CONVERSION,
#if NUM_LAYERS == 2
  COLOR_CONVERSION_1,
#endif
};

static U32 _aBuffer[40 * 40];  // Only required for drawing AA4 characters
//
// DMA2D transfer is ready flag
//
static volatile int _WaitForDMA2D;

//
// Prototypes of DMA2D color conversion routines
//
static void _DMA_Index2ColorBulk(void * pIndex, LCD_COLOR * pColor, U32 NumItems, U8 SizeOfIndex, U32 PixelFormat);
static void _DMA_Color2IndexBulk(LCD_COLOR * pColor, void * pIndex, U32 NumItems, U8 SizeOfIndex, U32 PixelFormat);

//
// Color conversion routines using DMA2D
//
DEFINE_DMA2D_COLORCONVERSION(M8888I, LTDC_Pixelformat_ARGB8888)
DEFINE_DMA2D_COLORCONVERSION(M888,   LTDC_Pixelformat_ARGB8888) // Internal pixel format of emWin is 32 bit, because of that ARGB8888
DEFINE_DMA2D_COLORCONVERSION(M565,   LTDC_Pixelformat_RGB565)
DEFINE_DMA2D_COLORCONVERSION(M1555I, LTDC_Pixelformat_ARGB1555)
DEFINE_DMA2D_COLORCONVERSION(M4444I, LTDC_Pixelformat_ARGB4444)

/*********************************************************************
*
*       static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _GetPixelformat
*/
static U32 _GetPixelformat(int LayerIndex) {
  const LCD_API_COLOR_CONV * pColorConvAPI;

  if ((unsigned)LayerIndex >= GUI_COUNTOF(_apColorConvAPI)) {
    return 0;
  }
  pColorConvAPI = _apColorConvAPI[LayerIndex];
  if        (pColorConvAPI == GUICC_M8888I) {
    return LTDC_PIXEL_FORMAT_ARGB8888;
  } else if (pColorConvAPI == GUICC_M888  ) {
    return LTDC_PIXEL_FORMAT_RGB888;
  } else if (pColorConvAPI == GUICC_M565  ) {
    return LTDC_PIXEL_FORMAT_RGB565;
  } else if (pColorConvAPI == GUICC_M1555I) {
    return LTDC_PIXEL_FORMAT_ARGB1555;
  } else if (pColorConvAPI == GUICC_M4444I) {
    return LTDC_PIXEL_FORMAT_ARGB4444;
  } else if (pColorConvAPI == GUICC_8666  ) {
    return LTDC_PIXEL_FORMAT_L8;
  } else if (pColorConvAPI == GUICC_1616I ) {
    return LTDC_PIXEL_FORMAT_AL44;
  } else if (pColorConvAPI == GUICC_88666I) {
    return LTDC_PIXEL_FORMAT_AL88;
  }
  while (1); // Error
}

/*********************************************************************
*
*       _DMA_ExecOperation
*/
static void _DMA_ExecOperation(void) {
  //
  // Invalidate and clean the data cache before executing the DMA2D operation.
  // Otherwise we would have artifacts on the LCD.
  //
  SCB_CleanInvalidateDCache();
  //
  // Set Flag which gets cleared when DMA2D transfer is completed
  //
  _WaitForDMA2D = 1;
  //
  // Execute operation
  //
  DMA2D->CR     |= 1;                               // Control Register (Start operation)
  dma2d.Instance = DMA2D;
  //
  // Wait for DMA2D
  //
  HAL_DMA2D_PollForTransfer(&dma2d, 25);
}

/*********************************************************************
*
*       _DMA_Fill
*/
static void _DMA_Fill(U32 LayerIndex, void * pDst, U32 xSize, U32 ySize, U32 OffLine, U32 ColorIndex) {
  U32 PixelFormat;
  
  PixelFormat    = _GetPixelformat(LayerIndex);
  DMA2D->CR      = 0x00030000UL /*| (1 << 9)*/;
  DMA2D->OCOLR   = ColorIndex;                     
  DMA2D->OMAR    = (U32)pDst;                      
  DMA2D->OOR     = OffLine;
  DMA2D->OPFCCR  = PixelFormat;
  DMA2D->NLR     = (U32)(xSize << 16) | (U16)ySize;  
  _DMA_ExecOperation();
}

/*********************************************************************
*
*       _DMA_DrawAlphaBitmap
*/
/**/
static void _DMA_DrawAlphaBitmap(void * pDst, const void * pSrc, int xSize, int ySize, int OffLineSrc, int OffLineDst, int PixelFormat) {
  DMA2D->CR      = 0x00020000UL /* | (1 << 9) */;         // Control Register (Memory to memory with blending of FG and BG and TCIE)
  DMA2D->FGMAR   = (U32)pSrc;                       // Foreground Memory Address Register (Source address)
  DMA2D->BGMAR   = (U32)pDst;                       // Background Memory Address Register (Destination address)
  DMA2D->OMAR    = (U32)pDst;                       // Output Memory Address Register (Destination address)
  DMA2D->FGOR    = OffLineSrc;                      // Foreground Offset Register (Source line offset)
  DMA2D->BGOR    = OffLineDst;                      // Background Offset Register (Destination line offset)
  DMA2D->OOR     = OffLineDst;                      // Output Offset Register (Destination line offset)
  DMA2D->FGPFCCR = LTDC_PIXEL_FORMAT_ARGB8888;      // Foreground PFC Control Register (Defines the input pixel format)
  DMA2D->BGPFCCR = PixelFormat;                     // Background PFC Control Register (Defines the destination pixel format)
  DMA2D->OPFCCR  = PixelFormat;                     // Output     PFC Control Register (Defines the output pixel format)
  DMA2D->NLR     = (U32)(xSize << 16) | (U16)ySize; // Number of Line Register (Size configuration of area to be transfered)
  _DMA_ExecOperation();
}

/*********************************************************************
*
*       _DMA_DrawBitmap
*/
static void _DMA_DrawBitmap(void * pDst, const void * pSrc, int xSize, int ySize, int OffLineSrc, int OffLineDst, int PixelFormatSrc, int PixelFormatDst) {
  DMA2D->CR      = 0x00010000UL /* | (1 << 9) */;         // Control Register (Memory-to-memory with PFC and TCIE)
  DMA2D->FGMAR   = (U32)pSrc;                       // Foreground Memory Address Register (Source address)
  DMA2D->BGMAR   = (U32)pDst;                       // Background Memory Address Register (Destination address)
  DMA2D->OMAR    = (U32)pDst;                       // Output Memory Address Register (Destination address)
  DMA2D->FGOR    = OffLineSrc;                      // Foreground Offset Register (Source line offset)
  DMA2D->BGOR    = OffLineDst;                      // Background Offset Register (Destination line offset)
  DMA2D->OOR     = OffLineDst;                      // Output Offset Register (Destination line offset)
  DMA2D->FGPFCCR = PixelFormatSrc;                  // Foreground PFC Control Register (Defines the input pixel format)
  DMA2D->OPFCCR  = PixelFormatDst;                  // Output     PFC Control Register (Defines the output pixel format)
  DMA2D->NLR     = (U32)(xSize << 16) | (U16)ySize; // Number of Line Register (Size configuration of area to be transfered)
  _DMA_ExecOperation();
}

/*********************************************************************
*
*       _DMA_Copy
*/
static void _DMA_Copy(int LayerIndex, const void * pSrc, void * pDst, int xSize, int ySize, int OffLineSrc, int OffLineDst) {
  U32 PixelFormat;

  PixelFormat    = _GetPixelformat(LayerIndex);
  DMA2D->CR      = 0x00000000UL /* | (1 << 9) */;         // Control Register (Memory to memory and TCIE)
  DMA2D->FGMAR   = (U32)pSrc;                       // Foreground Memory Address Register (Source address)
  DMA2D->OMAR    = (U32)pDst;                       // Output Memory Address Register (Destination address)
  DMA2D->FGOR    = OffLineSrc;                      // Foreground Offset Register (Source line offset)
  DMA2D->OOR     = OffLineDst;                      // Output Offset Register (Destination line offset)
  DMA2D->FGPFCCR = PixelFormat;                     // Foreground PFC Control Register (Defines the input pixel format)
  DMA2D->NLR     = (U32)(xSize << 16) | (U16)ySize; // Number of Line Register (Size configuration of area to be transfered)
  _DMA_ExecOperation();
}


/*********************************************************************
*
*       _DMA_DrawBitmapL8
*/
static void _DMA_DrawBitmapL8(void * pSrc, void * pDst,  U32 OffSrc, U32 OffDst, U32 PixelFormatDst, U32 xSize, U32 ySize) {
  //
  // Set up mode
  //
  DMA2D->CR      = 0x00010000UL /* | (1 << 9) */;         // Control Register (Memory to memory with pixel format conversion and TCIE)
  //
  // Set up pointers
  //
  DMA2D->FGMAR   = (U32)pSrc;                       // Foreground Memory Address Register (Source address)
  DMA2D->OMAR    = (U32)pDst;                       // Output Memory Address Register (Destination address)
  //
  // Set up offsets
  //
  DMA2D->FGOR    = OffSrc;                          // Foreground Offset Register (Source line offset)
  DMA2D->OOR     = OffDst;                          // Output Offset Register (Destination line offset)
  //
  // Set up pixel format
  //
  DMA2D->FGPFCCR = LTDC_PIXEL_FORMAT_L8;             // Foreground PFC Control Register (Defines the input pixel format)
  DMA2D->OPFCCR  = PixelFormatDst;                  // Output PFC Control Register (Defines the output pixel format)
  //
  // Set up size
  //
  DMA2D->NLR     = (U32)(xSize << 16) | ySize;      // Number of Line Register (Size configuration of area to be transfered)
  //
  // Execute operation
  //
  _DMA_ExecOperation();
}

/*********************************************************************
*
*       _DMA_DrawBitmapA4
*/
static int _DMA_DrawBitmapA4(void * pSrc, void * pDst,  U32 OffSrc, U32 OffDst, U32 PixelFormatDst, U32 xSize, U32 ySize) {
  U8 * pRD;
  U8 * pWR;
  U32 NumBytes, Color, Index;

  //
  // Check size of conversion buffer
  //
  NumBytes = (((xSize + 1) & ~1) * ySize) >> 1;
  if ((NumBytes > sizeof(_aBuffer)) || (NumBytes == 0)) {
    return 1;
  }
  //
  // Conversion (swapping nibbles)
  //
  pWR = (U8 *)_aBuffer;
  pRD = (U8 *)pSrc;
  do {
    *pWR++ = _aMirror[*pRD++];
  } while (--NumBytes);
  //
  // Get current drawing color (ABGR)
  //
  Index = LCD_GetColorIndex();
  Color = LCD_Index2Color(Index);
  //
  // Set up operation mode
  //
  DMA2D->CR = 0x00020000UL /* | (1 << 9) */;
  //
  // Set up source
  //
#if (GUI_USE_ARGB == 0)
  DMA2D->FGCOLR  = ((Color & 0xFF) << 16)  // Red
                 |  (Color & 0xFF00)       // Green
                 | ((Color >> 16) & 0xFF); // Blue
#else
  DMA2D->FGCOLR  = Color;
#endif
  DMA2D->FGMAR   = (U32)_aBuffer;
  DMA2D->FGOR    = 0;
  DMA2D->FGPFCCR = 0xA;                    // A4 bitmap
  DMA2D->NLR     = (U32)((xSize + OffSrc) << 16) | ySize;
  DMA2D->BGMAR   = (U32)pDst;
  DMA2D->BGOR    = OffDst - OffSrc;
  DMA2D->BGPFCCR = PixelFormatDst;
  DMA2D->OMAR    = DMA2D->BGMAR;
  DMA2D->OOR     = DMA2D->BGOR;
  DMA2D->OPFCCR  = DMA2D->BGPFCCR;
  //
  // Execute operation
  //
  _DMA_ExecOperation();
  return 0;
}

/*********************************************************************
*
*       _GetBufferSize
*/
static U32 _GetBufferSize(U32 LayerIndex) {
  return (layer_prop[LayerIndex].xSize * layer_prop[LayerIndex].ySize * layer_prop[LayerIndex].BytesPerPixel);
}

/*********************************************************************
*
*       _LCD_CopyBuffer
*/
static void _LCD_CopyBuffer(int LayerIndex, int IndexSrc, int IndexDst) {
  U32 BufferSize, AddrSrc, AddrDst;
  
  BufferSize = _GetBufferSize(LayerIndex);
  AddrSrc    = layer_prop[LayerIndex].address[IndexSrc];
  AddrDst    = layer_prop[LayerIndex].address[IndexDst];
  _DMA_Copy(LayerIndex, (void *)AddrSrc, (void *)AddrDst, layer_prop[LayerIndex].xSize, layer_prop[LayerIndex].ySize, 0, 0);
  layer_prop[LayerIndex].buffer_index = IndexDst;
}

/*********************************************************************
*
*       _LCD_CopyRect
*/
static void _LCD_CopyRect(int LayerIndex, int x0, int y0, int x1, int y1, int xSize, int ySize) {
  U32 BufferSize, AddrSrc, AddrDst;
  
  BufferSize = _GetBufferSize(LayerIndex);
  AddrSrc = layer_prop[LayerIndex].address[layer_prop[LayerIndex].pending_buffer] + (y0 * layer_prop[LayerIndex].xSize + x0) * layer_prop[LayerIndex].BytesPerPixel;
  AddrDst = layer_prop[LayerIndex].address[layer_prop[LayerIndex].pending_buffer] + (y1 * layer_prop[LayerIndex].xSize + x1) * layer_prop[LayerIndex].BytesPerPixel;
  _DMA_Copy(LayerIndex, (void *)AddrSrc, (void *)AddrDst, xSize, ySize, layer_prop[LayerIndex].xSize - xSize, 0);
}

/*********************************************************************
*
*       _LCD_FillRect
*/
static void _LCD_FillRect(int LayerIndex, int x0, int y0, int x1, int y1, U32 PixelIndex) {
  U32 BufferSize, AddrDst;
  int xSize, ySize;
  
  if (GUI_GetDrawMode() == GUI_DM_XOR) {		
    LCD_SetDevFunc(LayerIndex, LCD_DEVFUNC_FILLRECT, NULL);
    LCD_FillRect(x0, y0, x1, y1);
    LCD_SetDevFunc(LayerIndex, LCD_DEVFUNC_FILLRECT, (void(*)(void))_LCD_FillRect);
  } else {
    xSize = x1 - x0 + 1;
    ySize = y1 - y0 + 1;
    BufferSize = _GetBufferSize(LayerIndex);
    AddrDst = layer_prop[LayerIndex].address[layer_prop[LayerIndex].buffer_index] + (y0 * layer_prop[LayerIndex].xSize + x0) * layer_prop[LayerIndex].BytesPerPixel;
    _DMA_Fill(LayerIndex, (void *)AddrDst, xSize, ySize, layer_prop[LayerIndex].xSize - xSize, PixelIndex);
  }
}

/*********************************************************************
*
*       _LCD_DrawBitmapM565
*/
static void _LCD_DrawBitmapM565(int LayerIndex, int x, int y, const void * p, int xSize, int ySize, int BytesPerLine) {
  U32 BufferSize, AddrDst;
  int OffLineSrc, OffLineDst;
  U32 PixelFormatDst;

  PixelFormatDst = _GetPixelformat(LayerIndex);
  BufferSize = _GetBufferSize(LayerIndex);
  AddrDst    = layer_prop[LayerIndex].address[layer_prop[LayerIndex].buffer_index] + (y * layer_prop[LayerIndex].xSize + x) * layer_prop[LayerIndex].BytesPerPixel;
  OffLineSrc = (BytesPerLine / 2) - xSize;
  OffLineDst = layer_prop[LayerIndex].xSize - xSize;
  _DMA_DrawBitmap((void *)AddrDst, p, xSize, ySize, OffLineSrc, OffLineDst, LTDC_PIXEL_FORMAT_RGB565, PixelFormatDst);
}

/*********************************************************************
*
*       _LCD_DrawBitmapAlpha
*/
static void _LCD_DrawBitmapAlpha(int LayerIndex, int x, int y, void const * p, int xSize, int ySize, int BytesPerLine) {
  U32 BufferSize, AddrDst;
  int OffLineSrc, OffLineDst;
  U32 PixelFormat;

  PixelFormat = _GetPixelformat(LayerIndex);
  BufferSize = _GetBufferSize(LayerIndex);
  AddrDst = layer_prop[LayerIndex].address[layer_prop[LayerIndex].buffer_index] + (y * layer_prop[LayerIndex].xSize + x) * layer_prop[LayerIndex].BytesPerPixel;
  OffLineSrc = (BytesPerLine / 4) - xSize;
  OffLineDst = layer_prop[LayerIndex].xSize - xSize;
  _DMA_DrawAlphaBitmap((void *)AddrDst, p, xSize, ySize, OffLineSrc, OffLineDst, PixelFormat);
}

/*********************************************************************
*
*       _LCD_DrawBitmap32bpp
*/
static void _LCD_DrawBitmap32bpp(int LayerIndex, int x, int y, U16 const * p, int xSize, int ySize, int BytesPerLine) {
  U32 BufferSize, AddrDst;
  int OffLineSrc, OffLineDst;

  BufferSize = _GetBufferSize(LayerIndex);
  AddrDst = layer_prop[LayerIndex].address[layer_prop[LayerIndex].buffer_index] + (y * layer_prop[LayerIndex].xSize + x) * layer_prop[LayerIndex].BytesPerPixel;
  OffLineSrc = (BytesPerLine / 4) - xSize;
  OffLineDst = layer_prop[LayerIndex].xSize - xSize;
  _DMA_Copy(LayerIndex, (void *)p, (void *)AddrDst, xSize, ySize, OffLineSrc, OffLineDst);
}

/*********************************************************************
*
*       _LCD_DrawBitmap16bpp
*/
static void _LCD_DrawBitmap16bpp(int LayerIndex, int x, int y, U16 const * p, int xSize, int ySize, int BytesPerLine) {
  U32 BufferSize, AddrDst;
  int OffLineSrc, OffLineDst;

  BufferSize = _GetBufferSize(LayerIndex);
  AddrDst = layer_prop[LayerIndex].address[layer_prop[LayerIndex].buffer_index] + (y * layer_prop[LayerIndex].xSize + x) * layer_prop[LayerIndex].BytesPerPixel;
  OffLineSrc = (BytesPerLine / 2) - xSize;
  OffLineDst = layer_prop[LayerIndex].xSize - xSize;
  _DMA_Copy(LayerIndex, (void *)p, (void *)AddrDst, xSize, ySize, OffLineSrc, OffLineDst);
}

/*********************************************************************
*
*       _LCD_DrawBitmap8bpp
*/
static void _LCD_DrawBitmap8bpp(int LayerIndex, int x, int y, U8 const * p, int xSize, int ySize, int BytesPerLine) {
  U32 BufferSize, AddrDst;
  int OffLineSrc, OffLineDst;
  U32 PixelFormat;

  PixelFormat = _GetPixelformat(LayerIndex);
  BufferSize = _GetBufferSize(LayerIndex);
  AddrDst = layer_prop[LayerIndex].address[layer_prop[LayerIndex].buffer_index] + (y * layer_prop[LayerIndex].xSize + x) * layer_prop[LayerIndex].BytesPerPixel;
  OffLineSrc = BytesPerLine - xSize;
  OffLineDst = layer_prop[LayerIndex].xSize - xSize;
  _DMA_DrawBitmapL8((void *)p, (void *)AddrDst, OffLineSrc, OffLineDst, PixelFormat, xSize, ySize);
}

/*********************************************************************
*
*       _LCD_DrawBitmap4bpp
*/
static int _LCD_DrawBitmap4bpp(int LayerIndex, int x, int y, U8 const * p, int xSize, int ySize, int BytesPerLine) {
  U32 BufferSize, AddrDst;
  int OffLineSrc, OffLineDst;
  U32 PixelFormat;
  const GUI_RECT * pRect;

  pRect = GUI_GetClipRect();
  if (x < pRect->x0) {
    return 1;
  }
  if ((x + xSize) >= pRect->x1) {
    return 1;
  }
  if (y < pRect->y0) {
    return 1;
  }
  if ((y + ySize) >= pRect->y1) {
    return 1;
  }
  PixelFormat = _GetPixelformat(LayerIndex);
  //
  // Check if destination has direct color mode
  //
  if (PixelFormat > LTDC_PIXEL_FORMAT_ARGB4444) {
    return 1;
  }
  BufferSize = _GetBufferSize(LayerIndex);
  AddrDst = layer_prop[LayerIndex].address[layer_prop[LayerIndex].buffer_index] + (y * layer_prop[LayerIndex].xSize + x) * layer_prop[LayerIndex].BytesPerPixel;
  OffLineSrc = (BytesPerLine * 2) - xSize;
  OffLineDst = layer_prop[LayerIndex].xSize - xSize;
  return _DMA_DrawBitmapA4((void *)p, (void *)AddrDst, OffLineSrc, OffLineDst, PixelFormat, xSize, ySize);;
}

/*********************************************************************
*
*       _LCD_DrawMemdevAlpha
*/
static void _LCD_DrawMemdevAlpha(void * pDst, const void * pSrc, int xSize, int ySize, int BytesPerLineDst, int BytesPerLineSrc) {
  int OffLineSrc, OffLineDst;

  OffLineSrc = (BytesPerLineSrc / 4) - xSize;
  OffLineDst = (BytesPerLineDst / 4) - xSize;
  _DMA_DrawAlphaBitmap(pDst, pSrc, xSize, ySize, OffLineSrc, OffLineDst, LTDC_PIXEL_FORMAT_ARGB8888);
}

/*********************************************************************
*
*       _LCD_DrawMemdevM565
*
* Purpose:
*   Copy data with conversion
*/
static void _LCD_DrawMemdevM565(void * pDst, const void * pSrc, int xSize, int ySize, int BytesPerLineDst, int BytesPerLineSrc) {
  int OffLineSrc, OffLineDst;

  OffLineSrc = (BytesPerLineSrc / 2) - xSize;
  OffLineDst = (BytesPerLineDst / 4) - xSize;
  _DMA_DrawBitmap(pDst, pSrc, xSize, ySize, OffLineSrc, OffLineDst, LTDC_PIXEL_FORMAT_RGB565, LTDC_PIXEL_FORMAT_ARGB8888);
}

#if GUI_MEMDEV_SUPPORT_CUSTOMDRAW

/*********************************************************************
*
*       _DMA_CopyRGB565
*/
static void _DMA_CopyRGB565(const void * pSrc, void * pDst, int xSize, int ySize, int OffLineSrc, int OffLineDst) {
  DMA2D->CR      = 0x00000000UL /* | (1 << 9) */;         // Control Register (Memory to memory and TCIE)
  DMA2D->FGMAR   = (U32)pSrc;                       // Foreground Memory Address Register (Source address)
  DMA2D->OMAR    = (U32)pDst;                       // Output Memory Address Register (Destination address)
  DMA2D->FGOR    = OffLineSrc;                      // Foreground Offset Register (Source line offset)
  DMA2D->OOR     = OffLineDst;                      // Output Offset Register (Destination line offset)
  DMA2D->FGPFCCR = LTDC_PIXEL_FORMAT_RGB565;        // Foreground PFC Control Register (Defines the input pixel format)
  DMA2D->NLR     = (U32)(xSize << 16) | (U16)ySize; // Number of Line Register (Size configuration of area to be transfered)
  _DMA_ExecOperation();
}

/*********************************************************************
*
*       _LCD_DrawMemdev16bpp
*/
static void _LCD_DrawMemdev16bpp(void * pDst, const void * pSrc, int xSize, int ySize, int BytesPerLineDst, int BytesPerLineSrc) {
  int OffLineSrc, OffLineDst;

  OffLineSrc = (BytesPerLineSrc / 2) - xSize;
  OffLineDst = (BytesPerLineDst / 2) - xSize;
  _DMA_CopyRGB565(pSrc, pDst, xSize, ySize, OffLineSrc, OffLineDst);
}
#endif

/*********************************************************************
*
*       _DMA_AlphaBlendingBulk
*/
static void _DMA_AlphaBlendingBulk(LCD_COLOR * pColorFG, LCD_COLOR * pColorBG, LCD_COLOR * pColorDst, U32 NumItems) {
  //
  // Set up mode
  //
  DMA2D->CR      = 0x00020000UL | (1 << 9);         // Control Register (Memory to memory with blending of FG and BG and TCIE)
  //
  // Set up pointers
  //
  DMA2D->FGMAR   = (U32)pColorFG;                   // Foreground Memory Address Register
  DMA2D->BGMAR   = (U32)pColorBG;                   // Background Memory Address Register
  DMA2D->OMAR    = (U32)pColorDst;                  // Output Memory Address Register (Destination address)
  //
  // Set up offsets
  //
  DMA2D->FGOR    = 0;                               // Foreground Offset Register
  DMA2D->BGOR    = 0;                               // Background Offset Register
  DMA2D->OOR     = 0;                               // Output Offset Register
  //
  // Set up pixel format
  //
  DMA2D->FGPFCCR = LTDC_Pixelformat_ARGB8888;       // Foreground PFC Control Register (Defines the FG pixel format)
  DMA2D->BGPFCCR = LTDC_Pixelformat_ARGB8888;       // Background PFC Control Register (Defines the BG pixel format)
  DMA2D->OPFCCR  = LTDC_Pixelformat_ARGB8888;       // Output     PFC Control Register (Defines the output pixel format)
  //
  // Set up size
  //
  DMA2D->NLR     = (U32)(NumItems << 16) | 1;       // Number of Line Register (Size configuration of area to be transfered)
  //
  // Execute operation
  //
  _DMA_ExecOperation();
}

#if 0  // This function mixes a single pixel. Mixing a single pixel is faster in software

/*********************************************************************
*
*       _DMA_MixColors
*
* Purpose:
*   Function for mixing up 2 colors with the given intensity.
*   If the background color is completely transparent the
*   foreground color should be used unchanged.
*/
static LCD_COLOR _DMA_MixColors(LCD_COLOR Color, LCD_COLOR BkColor, U8 Intens) {
  volatile U32 ColorDst;

#if (GUI_USE_ARGB == 0)
  Color   ^= 0xFF000000;
  BkColor ^= 0xFF000000;
#endif
  //
  // Set up mode
  //
  DMA2D->CR      = 0x00020000UL /* | (1 << 9) */;         // Control Register (Memory to memory with blending of FG and BG and TCIE)
  //
  // Set up pointers
  //
  DMA2D->FGMAR   = (U32)&Color;                   // Foreground Memory Address Register
  DMA2D->BGMAR   = (U32)&BkColor;                   // Background Memory Address Register
  DMA2D->OMAR    = (U32)&ColorDst;                  // Output Memory Address Register (Destination address)
  //
  // Set up pixel format
  //
  DMA2D->FGPFCCR = LTDC_PIXEL_FORMAT_ARGB8888
                 | (1UL << 16)
                 | ((U32)Intens << 24);
  DMA2D->BGPFCCR = LTDC_PIXEL_FORMAT_ARGB8888
                 | (0UL << 16)
                 | ((U32)(255 - Intens) << 24);
  DMA2D->OPFCCR  = LTDC_PIXEL_FORMAT_ARGB8888;
  //
  // Set up size
  //
  DMA2D->NLR     = (U32)(1 << 16) | 1;              // Number of Line Register (Size configuration of area to be transfered)
  //
  // Execute operation
  //
  _DMA_ExecOperation();
#if (GUI_USE_ARGB == 0)
  ColorDst ^= 0xFF000000;
#endif
  return ColorDst;
}

#endif

/*********************************************************************
*
*       _DMA_MixColorsBulk
*/
static void _DMA_MixColorsBulk(LCD_COLOR * pColorFG, LCD_COLOR * pColorBG, LCD_COLOR * pColorDst, U8 Intens, U32 NumItems) {
  //
  // Set up mode
  //
  DMA2D->CR      = 0x00020000UL /*| (1 << 9)*/;         // Control Register (Memory to memory with blending of FG and BG and TCIE)
  //
  // Set up pointers
  //
  DMA2D->FGMAR   = (U32)pColorFG;                   // Foreground Memory Address Register
  DMA2D->BGMAR   = (U32)pColorBG;                   // Background Memory Address Register
  DMA2D->OMAR    = (U32)pColorDst;                  // Output Memory Address Register (Destination address)
  //
  // Set up pixel format
  //
  DMA2D->FGPFCCR = LTDC_Pixelformat_ARGB8888
                 | (2UL << 16)
                 | ((U32)Intens << 24);
  DMA2D->BGPFCCR = LTDC_Pixelformat_ARGB8888
                 | (0UL << 16)
                 | ((U32)(255 - Intens) << 24);
  DMA2D->OPFCCR  = LTDC_Pixelformat_ARGB8888;
  //
  // Set up size
  //
  DMA2D->NLR     = (U32)(NumItems << 16) | 1;              // Number of Line Register (Size configuration of area to be transfered)
  //
  // Execute operation
  //
  _DMA_ExecOperation();
}

/*********************************************************************
*
*       _DMA_ConvertColor
*/
static void _DMA_ConvertColor(void * pSrc, void * pDst,  U32 PixelFormatSrc, U32 PixelFormatDst, U32 NumItems) {
  //
  // Set up mode
  //
  DMA2D->CR      = 0x00010000UL /*| (1 << 9)*/;         // Control Register (Memory to memory with pixel format conversion and TCIE)
  //
  // Set up pointers
  //
  DMA2D->FGMAR   = (U32)pSrc;                       // Foreground Memory Address Register (Source address)
  DMA2D->OMAR    = (U32)pDst;                       // Output Memory Address Register (Destination address)
  //
  // Set up offsets
  //
  DMA2D->FGOR    = 0;                               // Foreground Offset Register (Source line offset)
  DMA2D->OOR     = 0;                               // Output Offset Register (Destination line offset)
  //
  // Set up pixel format
  //
  DMA2D->FGPFCCR = PixelFormatSrc;                  // Foreground PFC Control Register (Defines the input pixel format)
  DMA2D->OPFCCR  = PixelFormatDst;                  // Output PFC Control Register (Defines the output pixel format)
  //
  // Set up size
  //
  DMA2D->NLR     = (U32)(NumItems << 16) | 1;       // Number of Line Register (Size configuration of area to be transfered)
  //
  // Execute operation
  //
  _DMA_ExecOperation();
}

/*********************************************************************
*
*       _DMA_LoadLUT
*/
static void _DMA_LoadLUT(LCD_COLOR * pColor, U32 NumItems) {
  DMA2D->FGCMAR  = (U32)pColor;                     // Foreground CLUT Memory Address Register
  //
  // Foreground PFC Control Register
  //
  DMA2D->FGPFCCR  = LTDC_Pixelformat_RGB888         // Pixel format
                  | ((NumItems - 1) & 0xFF) << 8;   // Number of items to load
  DMA2D->FGPFCCR |= (1 << 5);                       // Start loading
  //
  // Waiting not required here...
  //
}

#if (GUI_USE_ARGB == 0)

/*********************************************************************
*
*       _InvertAlpha_SwapRB_CPY
*
* Purpose:
*   Color format of DMA2D is different to emWin color format. This routine
*   swaps red and blue and inverts alpha that it is compatible to emWin
*   and vice versa. Result is located in destination buffer.
*
* Performance:
*   10.8 MPixel/s
*/
static void _InvertAlpha_SwapRB_CPY(LCD_COLOR * pColorSrc, LCD_COLOR * pColorDst, U32 NumItems) {
  U32 Color;
  while (NumItems >= 4) {
    Color = *(pColorSrc + 0);
    Color = ((Color << 24) >>  8)  // Red
          |  (Color & 0xFF00)      // Green
          | ((Color <<  8) >> 24)  // Blue
          | (~Color & 0xFF000000); // Alpha
    *(pColorDst + 0) = Color;
    Color = *(pColorSrc + 1);
    Color = ((Color << 24) >>  8)  // Red
          |  (Color & 0xFF00)      // Green
          | ((Color <<  8) >> 24)  // Blue
          | (~Color & 0xFF000000); // Alpha
    *(pColorDst + 1) = Color;
    Color = *(pColorSrc + 2);
    Color = ((Color << 24) >>  8)  // Red
          |  (Color & 0xFF00)      // Green
          | ((Color <<  8) >> 24)  // Blue
          | (~Color & 0xFF000000); // Alpha
    *(pColorDst + 2) = Color;
    Color = *(pColorSrc + 3);
    Color = ((Color << 24) >>  8)  // Red
          |  (Color & 0xFF00)      // Green
          | ((Color <<  8) >> 24)  // Blue
          | (~Color & 0xFF000000); // Alpha
    *(pColorDst + 3) = Color;
    pColorSrc += 4;
    pColorDst += 4;
    NumItems -= 4;
  };
  while (NumItems--) {
    Color = *pColorSrc++;
    Color = ((Color << 24) >>  8)  // Red
          |  (Color & 0xFF00)      // Green
          | ((Color <<  8) >> 24)  // Blue
          | (~Color & 0xFF000000); // Alpha
    *pColorDst++ = Color;
  };
}

/*********************************************************************
*
*       _InvertAlpha_SwapRB_MOD
*
* Purpose:
*   Color format of DMA2D is different to emWin color format. This routine
*   swaps red and blue and inverts alpha that it is compatible to emWin
*   and vice versa. Conversion is done in given buffer.
*
* Performance:
*   10.9 MPixel/s
*/
static void _InvertAlpha_SwapRB_MOD(LCD_COLOR * pColor, U32 NumItems) {
  U32 Color;
  while (NumItems >= 4) {
    Color = *(pColor + 0);
    Color = ((Color << 24) >>  8)  // Red
          |  (Color & 0xFF00)      // Green
          | ((Color <<  8) >> 24)  // Blue
          | (~Color & 0xFF000000); // Alpha
    *(pColor + 0) = Color;
    Color = *(pColor + 1);
    Color = ((Color << 24) >>  8)  // Red
          |  (Color & 0xFF00)      // Green
          | ((Color <<  8) >> 24)  // Blue
          | (~Color & 0xFF000000); // Alpha
    *(pColor + 1) = Color;
    Color = *(pColor + 2);
    Color = ((Color << 24) >>  8)  // Red
          |  (Color & 0xFF00)      // Green
          | ((Color <<  8) >> 24)  // Blue
          | (~Color & 0xFF000000); // Alpha
    *(pColor + 2) = Color;
    Color = *(pColor + 3);
    Color = ((Color << 24) >>  8)  // Red
          |  (Color & 0xFF00)      // Green
          | ((Color <<  8) >> 24)  // Blue
          | (~Color & 0xFF000000); // Alpha
    *(pColor + 3) = Color;
    pColor += 4;
    NumItems -= 4;
  }
  while (NumItems--) {
    Color = *pColor;
    Color = ((Color << 24) >>  8)  // Red
          |  (Color & 0xFF00)      // Green
          | ((Color <<  8) >> 24)  // Blue
          | (~Color & 0xFF000000); // Alpha
    *pColor++ = Color;
  };
}

/*********************************************************************
*
*       _InvertAlpha_CPY
*
* Purpose:
*   Color format of DMA2D is different to emWin color format. This routine
*   inverts alpha that it is compatible to emWin and vice versa.
*   Changes are done in the destination memory location.
*
* Performance:
*   17.5 MPixel/s
*/
static void _InvertAlpha_CPY(LCD_COLOR * pColorSrc, LCD_COLOR * pColorDst, U32 NumItems) {
  U32 Color;

  while (NumItems >= 4) {
    Color = *(pColorSrc + 0);
    Color ^= 0xFF000000;      // Invert alpha
    *(pColorDst + 0) = Color;
    Color = *(pColorSrc + 1);
    Color ^= 0xFF000000;      // Invert alpha
    *(pColorDst + 1) = Color;
    Color = *(pColorSrc + 2);
    Color ^= 0xFF000000;      // Invert alpha
    *(pColorDst + 2) = Color;
    Color = *(pColorSrc + 3);
    Color ^= 0xFF000000;      // Invert alpha
    *(pColorDst + 3) = Color;
    pColorSrc += 4;
    pColorDst += 4;
    NumItems -= 4;
  }
  while (NumItems--) {
    Color = *pColorSrc++;
    *pColorDst++ = Color ^ 0xFF000000; // Invert alpha
  };
}

/*********************************************************************
*
*       _InvertAlpha_MOD
*
* Purpose:
*   Color format of DMA2D is different to emWin color format. This routine
*   inverts alpha that it is compatible to emWin and vice versa.
*   Changes are done in the given buffer.
*
* Performance:
*   18.0 MPixel/s
*/
static void _InvertAlpha_MOD(LCD_COLOR * pColor, U32 NumItems) {
  U32 Color;

  while (NumItems >= 4) {
    Color = *(pColor + 0);
    Color ^= 0xFF000000; // Invert alpha
    *(pColor + 0) = Color;
    Color = *(pColor + 1);
    Color ^= 0xFF000000; // Invert alpha
    *(pColor + 1) = Color;
    Color = *(pColor + 2);
    Color ^= 0xFF000000; // Invert alpha
    *(pColor + 2) = Color;
    Color = *(pColor + 3);
    Color ^= 0xFF000000; // Invert alpha
    *(pColor + 3) = Color;
    pColor += 4;
    NumItems -= 4;
  }
  while (NumItems--) {
    Color = *pColor;
    Color ^= 0xFF000000; // Invert alpha
    *pColor++ = Color;
  };
}

#endif

/*********************************************************************
*
*       _DMA_AlphaBlending
*/
static void _DMA_AlphaBlending(LCD_COLOR * pColorFG, LCD_COLOR * pColorBG, LCD_COLOR * pColorDst, U32 NumItems) {
#if (GUI_USE_ARGB)
  //
  // Use DMA2D for mixing
  //
  _DMA_AlphaBlendingBulk(pColorFG, pColorBG, pColorDst, NumItems);
#else
  //
  // Invert alpha values
  //
  _InvertAlpha_CPY(pColorFG, _pBuffer_FG, NumItems);
  _InvertAlpha_CPY(pColorBG, _pBuffer_BG, NumItems);
  //
  // Use DMA2D for mixing
  //
  _DMA_AlphaBlendingBulk(_pBuffer_FG, _pBuffer_BG, pColorDst, NumItems);
  //
  // Invert alpha values
  //
  _InvertAlpha_MOD(pColorDst, NumItems);
#endif
}

/*********************************************************************
*
*       _DMA_Index2ColorBulk
*
* Purpose:
*   This routine is used by the emWin color conversion routines to use DMA2D for
*   color conversion. It converts the given index values to 32 bit colors.
*   Because emWin uses ABGR internally and 0x00 and 0xFF for opaque and fully
*   transparent the color array needs to be converted after DMA2D has been used.
*/
static void _DMA_Index2ColorBulk(void * pIndex, LCD_COLOR * pColor, U32 NumItems, U8 SizeOfIndex, U32 PixelFormat) {
  GUI_USE_PARA(SizeOfIndex);

#if (GUI_USE_ARGB)
  //
  // Use DMA2D for the conversion
  //
  _DMA_ConvertColor(pIndex, pColor, PixelFormat, LTDC_Pixelformat_ARGB8888, NumItems);
#else
  //
  // Use DMA2D for the conversion
  //
  _DMA_ConvertColor(pIndex, pColor, PixelFormat, LTDC_Pixelformat_ARGB8888, NumItems);
  //
  // Convert colors from ARGB to ABGR and invert alpha values
  //
  _InvertAlpha_SwapRB_MOD(pColor, NumItems);
#endif
}

/*********************************************************************
*
*       _DMA_Color2IndexBulk
*
* Purpose:
*   This routine is used by the emWin color conversion routines to use DMA2D for
*   color conversion. It converts the given 32 bit color array to index values.
*   Because emWin uses ABGR internally and 0x00 and 0xFF for opaque and fully
*   transparent the given color array needs to be converted before DMA2D can be used.
*/
static void _DMA_Color2IndexBulk(LCD_COLOR * pColor, void * pIndex, U32 NumItems, U8 SizeOfIndex, U32 PixelFormat) {
  GUI_USE_PARA(SizeOfIndex);

#if (GUI_USE_ARGB)
  //
  // Use DMA2D for the conversion
  //
  _DMA_ConvertColor(pColor, pIndex, LTDC_Pixelformat_ARGB8888, PixelFormat, NumItems);
#else
  //
  // Convert colors from ABGR to ARGB and invert alpha values
  //
  _InvertAlpha_SwapRB_CPY(pColor, _pBuffer_DMA2D, NumItems);
  //
  // Use DMA2D for the conversion
  //
  _DMA_ConvertColor(_pBuffer_DMA2D, pIndex, LTDC_Pixelformat_ARGB8888, PixelFormat, NumItems);
#endif
}

/*********************************************************************
*
*       _LCD_MixColorsBulk
*/
static void _LCD_MixColorsBulk(U32 * pFG, U32 * pBG, U32 * pDst, unsigned OffFG, unsigned OffBG, unsigned OffDest, unsigned xSize, unsigned ySize, U8 Intens) {
#if (GUI_USE_ARGB)
  unsigned int y;

  GUI_USE_PARA(OffFG);
  GUI_USE_PARA(OffDest);
  for (y = 0; y < ySize; y++) {
    //
    // Use DMA2D for mixing up
    //
    _DMA_MixColorsBulk(pFG, pBG, pDst, Intens, xSize);
    pFG  += xSize + OffFG;
    pBG  += xSize + OffBG;
    pDst += xSize + OffDest;
  }
#else
  unsigned int y;

  GUI_USE_PARA(OffFG);
  GUI_USE_PARA(OffDest);
  for (y = 0; y < ySize; y++) {
    //
    // Invert alpha values
    //
    _InvertAlpha_CPY(pFG, _pBuffer_FG, xSize);
    _InvertAlpha_CPY(pBG, _pBuffer_BG, xSize);
    //
    // Use DMA2D for mixing up
    //
    _DMA_MixColorsBulk(_pBuffer_FG, _pBuffer_BG, pDst, Intens, xSize);
    //
    // Invert alpha values
    //
    _InvertAlpha_MOD(pDst, xSize);
    pFG  += xSize + OffFG;
    pBG  += xSize + OffBG;
    pDst += xSize + OffDest;
  }
#endif
}

/*********************************************************************
*
*       _LCD_GetpPalConvTable
*
* Purpose:
*   The emWin function LCD_GetpPalConvTable() normally translates the given colors into
*   index values for the display controller. In case of index based bitmaps without
*   transparent pixels we load the palette only to the DMA2D LUT registers to be
*   translated (converted) during the process of drawing via DMA2D.
*/
static LCD_PIXELINDEX * _LCD_GetpPalConvTable(const LCD_LOGPALETTE GUI_UNI_PTR * pLogPal, const GUI_BITMAP GUI_UNI_PTR * pBitmap, int LayerIndex) {
#if (GUI_USE_ARGB)
  void (* pFunc)(void);
  int DoDefault = 0;

  //
  // Check if we have a non transparent device independent bitmap
  //
  if (pBitmap->BitsPerPixel == 8) {
    pFunc = LCD_GetDevFunc(LayerIndex, LCD_DEVFUNC_DRAWBMP_8BPP);
    if (pFunc) {
      if (pBitmap->pPal) {
        if (pBitmap->pPal->HasTrans) {
          DoDefault = 1;
        }
      } else {
        DoDefault = 1;
      }
    } else {
      DoDefault = 1;
    }
  } else {
    DoDefault = 1;
  }
  //
  // Default palette management for other cases
  //
  if (DoDefault) {
    //
    // Return a pointer to the index values to be used by the controller
    //
    return LCD_GetpPalConvTable(pLogPal);
  }
  //
  // Load LUT using DMA2D
  //
  _DMA_LoadLUT((U32 *)pLogPal->pPalEntries, pLogPal->NumEntries);
  //
  // Return something not NULL
  //
  return (LCD_PIXELINDEX *)pLogPal->pPalEntries;//_pBuffer_DMA2D;/**/
#else
  void (* pFunc)(void);
  int DoDefault = 0;

  //
  // Check if we have a non transparent device independent bitmap
  //
  if (pBitmap->BitsPerPixel == 8) {
    pFunc = LCD_GetDevFunc(LayerIndex, LCD_DEVFUNC_DRAWBMP_8BPP);
    if (pFunc) {
      if (pBitmap->pPal) {
        if (pBitmap->pPal->HasTrans) {
          DoDefault = 1;
        }
      } else {
        DoDefault = 1;
      }
    } else {
      DoDefault = 1;
    }
  } else {
    DoDefault = 1;
  }
  //
  // Default palette management for other cases
  //
  if (DoDefault) {
    //
    // Return a pointer to the index values to be used by the controller
    //
    return LCD_GetpPalConvTable(pLogPal);
  }
  //
  // Convert palette colors from ARGB to ABGR
  //
  _InvertAlpha_SwapRB_CPY((U32 *)pLogPal->pPalEntries, _pBuffer_DMA2D, pLogPal->NumEntries);
  //
  // Load LUT using DMA2D
  //
  _DMA_LoadLUT(_pBuffer_DMA2D, pLogPal->NumEntries);
  //
  // Return something not NULL
  //
  return _pBuffer_DMA2D;
#endif
}

/*********************************************************************
*
*       _ConfigDriver
*
* Purpose:
*   Called from LCD_ROTATE_SetSelEx() after a new driver has been selected in order to
*   configure the new driver.
*/
static void _ConfigDriver(GUI_DEVICE * pDevice, int Index, int LayerIndex) {
  int NoRotationUsed;
  U32 PixelFormat;
  GUI_USE_PARA(pDevice);
  GUI_USE_PARA(Index);
  GUI_USE_PARA(LayerIndex);

  //
  // Display driver configuration
  //
  if (LCD_GetSwapXY()) {
    LCD_SetSizeEx (0, YSIZE_PHYS, XSIZE_PHYS);
    LCD_SetVSizeEx(0, YSIZE_PHYS, XSIZE_PHYS);
  } else {
    LCD_SetSizeEx (0, XSIZE_PHYS, YSIZE_PHYS);
    LCD_SetVSizeEx(0, XSIZE_PHYS, YSIZE_PHYS);
  }

  layer_prop[0].xSize = XSIZE_PHYS;
  layer_prop[0].ySize = YSIZE_PHYS;
  
  //layer_prop[0].address[0] = (U32)0x60000000;
  //layer_prop[0].address[1] = (U32)dsi_getNextFrameBuffer();  // Get next FB address
  layer_prop[0].address[0] = (U32)dsi_getActiveFrameBuffer();
  layer_prop[0].address[1] = (U32)dsi_getCurrentFrameBuffer();  // Get next FB address
  //
  // Set VRAM address
  //
  LCD_SetBufferPtrEx(0, (void **)layer_prop[0].address); 
  layer_prop[0].pColorConvAPI  = (LCD_API_COLOR_CONV *)_apColorConvAPI[0];    
  layer_prop[0].pending_buffer = -1;    
  layer_prop[0].BytesPerPixel  = LCD_GetBitsPerPixelEx(0) >> 3;
  //
  // Setting up custom functions
  //
  NoRotationUsed = 1;
  LCD_SetDevFunc(0, LCD_DEVFUNC_COPYBUFFER,    (void(*)(void))_LCD_CopyBuffer);
  if (Index == 0) { 
    LCD_SetDevFunc(0, LCD_DEVFUNC_COPYRECT,      (void(*)(void))_LCD_CopyRect);
//    LCD_SetDevFunc(0, LCD_DEVFUNC_FILLRECT,      (void(*)(void))_LCD_FillRect);
    LCD_SetDevFunc(0, LCD_DEVFUNC_DRAWBMP_8BPP,  (void(*)(void))_LCD_DrawBitmap8bpp);
    LCD_SetDevFunc(0, LCD_DEVFUNC_DRAWBMP_16BPP, (void(*)(void))_LCD_DrawBitmap16bpp);  
    LCD_SetDevFunc(0, LCD_DEVFUNC_DRAWBMP_32BPP, (void(*)(void))_LCD_DrawBitmap32bpp); 
  } else {
    NoRotationUsed = 0;
  }
  GUICC_M1555I_SetCustColorConv(_Color2IndexBulk_M1555I_DMA2D, _Index2ColorBulk_M1555I_DMA2D); // Set up custom bulk color conversion using DMA2D for ARGB1555
  GUICC_M4444I_SetCustColorConv(_Color2IndexBulk_M4444I_DMA2D, _Index2ColorBulk_M4444I_DMA2D); // Set up custom bulk color conversion using DMA2D for ARGB4444
  GUICC_M888_SetCustColorConv  (_Color2IndexBulk_M888_DMA2D,   _Index2ColorBulk_M888_DMA2D);   // Set up custom bulk color conversion using DMA2D for RGB888
  GUICC_M8888I_SetCustColorConv(_Color2IndexBulk_M8888I_DMA2D, _Index2ColorBulk_M8888I_DMA2D); // Set up custom bulk color conversion using DMA2D for ARGB8888
  //
  // Set up custom alpha blending function using DMA2D
  //
  GUI_SetFuncAlphaBlending(_DMA_AlphaBlending);
  //
  // Set up custom function for translating a bitmap palette into index values.
  // Required to load a bitmap palette into DMA2D CLUT in case of a 8bpp indexed bitmap
  //
  GUI_SetFuncGetpPalConvTable(_LCD_GetpPalConvTable);
  //
  // Set up custom function for mixing up arrays of colors using DMA2D
  //
  GUI_SetFuncMixColorsBulk(_LCD_MixColorsBulk);
  if (NoRotationUsed) {
    //
    // Set up custom function for drawing AA4 characters
    //
    GUI_AA_SetpfDrawCharAA4(_LCD_DrawBitmap4bpp);
    //
    // Set up custom function for drawing 16bpp memory devices
    //
#if GUI_MEMDEV_SUPPORT_CUSTOMDRAW
    GUI_MEMDEV_SetDrawMemdev16bppFunc(_LCD_DrawMemdev16bpp);
#endif
    //
    // Set up custom functions for drawing 32bpp bitmaps and 32bpp memory devices
    //
    GUI_SetFuncDrawAlpha(_LCD_DrawMemdevAlpha, _LCD_DrawBitmapAlpha);
    //
    // Set up custom function for drawing M565 bitmaps
    //
    GUI_SetFuncDrawM565(_LCD_DrawMemdevM565, _LCD_DrawBitmapM565);
  } else {
    GUI_AA_SetpfDrawCharAA4(NULL);
#if GUI_MEMDEV_SUPPORT_CUSTOMDRAW
    GUI_MEMDEV_SetDrawMemdev16bppFunc(NULL);
#endif
    GUI_SetFuncDrawAlpha(NULL, NULL);
    GUI_SetFuncDrawM565(NULL, NULL);
  }
}

/*********************************************************************
*
*       LCD_X_Config
*
* Purpose:
*   Called during the initialization process in order to set up the
*   display driver configuration.
*   
*/
static void _ClearCacheHook(U32 LayerMask) {
  GUI_USE_PARA(LayerMask);
  SCB_CleanInvalidateDCache();
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       LCD_X_Config
*
* Purpose:
*   Called during the initialization process in order to set up the
*   display driver configuration.
*   
*/
void LCD_X_Config(void) {
  int i;
  GUI_DEVICE * pDevice;

  st7701_init(EDID_MODE_480x800_60Hz);
  
  SDRAM.begin();
  _xSize = dsi_getDisplayXSize();
  _ySize = dsi_getDisplayYSize();
#if (NUM_BUFFERS > 1)
  for (i = 0; i < NUM_LAYERS; i++) { 
	  GUI_MULTIBUF_ConfigEx(i, NUM_BUFFERS);
  }
#endif
  //
  // Set display driver and color conversion for 1st layer
  //
  pDevice = GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER_0_0, COLOR_CONVERSION, 0, 0);

  //
  // Configure the initial driver
  //
  _ConfigDriver(pDevice, 0, 0);
  //
  // If screen rotation is not required the following section could be switched off to spare ROM
  //
#if 1
  //
  // Create additional drivers and add them to the display rotation module
  //
  LCD_ROTATE_AddDriverExOrientation(DISPLAY_DRIVER_0_1, 0, GUI_ROTATION_CW);
  LCD_ROTATE_AddDriverExOrientation(DISPLAY_DRIVER_0_2, 0, GUI_ROTATION_180);
  LCD_ROTATE_AddDriverExOrientation(DISPLAY_DRIVER_0_3, 0, GUI_ROTATION_CCW);
  //
  // Set callback function to be used for layer 0
  //
  LCD_ROTATE_SetCallback(_ConfigDriver, 0);
#endif
  GUI_DCACHE_SetClearCacheHook(_ClearCacheHook);
}

/*********************************************************************
*
*       LCD_X_DisplayDriver
*
* Purpose:
*   This function is called by the display driver for several purposes.
*   To support the according task the routine needs to be adapted to
*   the display controller. Please note that the commands marked with
*   'optional' are not cogently required and should only be adapted if 
*   the display controller supports these features.
*
* Parameter:
*   LayerIndex - Index of layer to be configured
*   Cmd        - Please refer to the details in the switch statement below
*   pData      - Pointer to a LCD_X_DATA structure
*
* Return Value:
*   < -1 - Error
*     -1 - Command not handled
*      0 - Ok
*/
int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData) {
  int r;

  switch (Cmd) {
  //
  // Required
  //
  case LCD_X_INITCONTROLLER: {
    PID_X_Init();
    return 0;
  }
  case LCD_X_SHOWBUFFER: {
    LCD_X_SHOWBUFFER_INFO * p;

    p = (LCD_X_SHOWBUFFER_INFO *)pData;
    dsi_drawCurrentFrameBuffer();
    GUI_MULTIBUF_Confirm(p->Index);
    break;
  }
  default:
    r = -1;
  }
  return r;
}

/*************************** End of file ****************************/
