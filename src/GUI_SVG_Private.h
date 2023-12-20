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
File        : GUI_SVG_Private.h
Purpose     : Private header for SVG support
---------------------------END-OF-HEADER------------------------------
*/

#ifndef GUI_SVG_PRIVATE_H
#define GUI_SVG_PRIVATE_H

#include "GUI_Private.h"
#include "GUI_ARRAY.h"

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define SVG_LOCK(h)    if (h) {                \
                         {                     \
                           GUI_SVG_Obj * pObj; \
                           GUI_LOCK();         \
                           pObj = (GUI_SVG_Obj *)GUI_LOCK_H(h)

#define SVG_UNLOCK()       GUI_UNLOCK_H(pObj); \
                           GUI_UNLOCK();       \
                         }                     \
                       }
//
// Iterating over a GUI_ARRAY
//
#define GUI_ARRAY_BEGIN_FOREACH(TYPE, VAR, ARR)                                         \
                               {                                                        \
                                 unsigned i, NumItems;                                  \
                                 TYPE * pUnlock, * VAR;                                 \
                                 if (ARR) {                                             \
                                   NumItems = GUI_ARRAY_GetNumItems(ARR);               \
                                   for (i = 0; i < NumItems; i++) {                     \
                                     pUnlock = VAR = (TYPE *)GUI_ARRAY_GetpItemLocked(ARR, i);
#define GUI_ARRAY_END_FOREACH()                                                         \
                                     GUI_UNLOCK_H(pUnlock);                             \
                                   }                                                    \
                                 }                                                      \
                               }
#define GUI_ARRAY_ADD_ITEM(ARR, VAR)   GUI_ARRAY_AddItem(ARR, &VAR, sizeof(VAR))

/*********************************************************************
*
*       Enum types
*
**********************************************************************
*/
typedef enum {
  GUI_SVG_CAP_BUTT   = 0x1700,
  GUI_SVG_CAP_ROUND  = 0x1701,
  GUI_SVG_CAP_SQUARE = 0x1702,
} GUI_SVG_CAP_STYLE;

typedef enum {
  GUI_SVG_JOIN_MITER = 0x1800,
  GUI_SVG_JOIN_ROUND = 0x1801,
  GUI_SVG_JOIN_BEVEL = 0x1802,
} GUI_SVG_JOIN_STYLE;

typedef enum {
  GUI_SVG_STROKE_PATH = (1 << 0),
  GUI_SVG_FILL_PATH   = (1 << 1),
} GUI_SVG_PAINT_MODE;

typedef enum {
  GUI_SVG_CLOSE_PATH  = ( 0 << 1),
  GUI_SVG_MOVE_TO     = ( 1 << 1),
  GUI_SVG_LINE_TO     = ( 2 << 1),
  GUI_SVG_HLINE_TO    = ( 3 << 1),
  GUI_SVG_VLINE_TO    = ( 4 << 1),
  GUI_SVG_QUAD_TO     = ( 5 << 1),
  GUI_SVG_CUBIC_TO    = ( 6 << 1),
  GUI_SVG_SQUAD_TO    = ( 7 << 1),
  GUI_SVG_SCUBIC_TO   = ( 8 << 1),
  GUI_SVG_SCCWARC_TO  = ( 9 << 1),
  GUI_SVG_SCWARC_TO   = (10 << 1),
  GUI_SVG_LCCWARC_TO  = (11 << 1),
  GUI_SVG_LCWARC_TO   = (12 << 1),
} GUI_SVG_PATH_SEGMENT;

typedef enum {
  GUI_SVG_ABSOLUTE = 0,
  GUI_SVG_RELATIVE = 1,
} GUI_SVG_PATH_ABSREL;

typedef enum {
  GUI_SVG_CLOSE          = GUI_SVG_CLOSE_PATH,
  GUI_SVG_MOVE_TO_ABS    = GUI_SVG_MOVE_TO    | GUI_SVG_ABSOLUTE,
  GUI_SVG_MOVE_TO_REL    = GUI_SVG_MOVE_TO    | GUI_SVG_RELATIVE,
  GUI_SVG_LINE_TO_ABS    = GUI_SVG_LINE_TO    | GUI_SVG_ABSOLUTE,
  GUI_SVG_LINE_TO_REL    = GUI_SVG_LINE_TO    | GUI_SVG_RELATIVE,
  GUI_SVG_HLINE_TO_ABS   = GUI_SVG_HLINE_TO   | GUI_SVG_ABSOLUTE,
  GUI_SVG_HLINE_TO_REL   = GUI_SVG_HLINE_TO   | GUI_SVG_RELATIVE,
  GUI_SVG_VLINE_TO_ABS   = GUI_SVG_VLINE_TO   | GUI_SVG_ABSOLUTE,
  GUI_SVG_VLINE_TO_REL   = GUI_SVG_VLINE_TO   | GUI_SVG_RELATIVE,
  GUI_SVG_QUAD_TO_ABS    = GUI_SVG_QUAD_TO    | GUI_SVG_ABSOLUTE,
  GUI_SVG_QUAD_TO_REL    = GUI_SVG_QUAD_TO    | GUI_SVG_RELATIVE,
  GUI_SVG_CUBIC_TO_ABS   = GUI_SVG_CUBIC_TO   | GUI_SVG_ABSOLUTE,
  GUI_SVG_CUBIC_TO_REL   = GUI_SVG_CUBIC_TO   | GUI_SVG_RELATIVE,
  GUI_SVG_SQUAD_TO_ABS   = GUI_SVG_SQUAD_TO   | GUI_SVG_ABSOLUTE,
  GUI_SVG_SQUAD_TO_REL   = GUI_SVG_SQUAD_TO   | GUI_SVG_RELATIVE,
  GUI_SVG_SCUBIC_TO_ABS  = GUI_SVG_SCUBIC_TO  | GUI_SVG_ABSOLUTE,
  GUI_SVG_SCUBIC_TO_REL  = GUI_SVG_SCUBIC_TO  | GUI_SVG_RELATIVE,
  GUI_SVG_SCCWARC_TO_ABS = GUI_SVG_SCCWARC_TO | GUI_SVG_ABSOLUTE,
  GUI_SVG_SCCWARC_TO_REL = GUI_SVG_SCCWARC_TO | GUI_SVG_RELATIVE,
  GUI_SVG_SCWARC_TO_ABS  = GUI_SVG_SCWARC_TO  | GUI_SVG_ABSOLUTE,
  GUI_SVG_SCWARC_TO_REL  = GUI_SVG_SCWARC_TO  | GUI_SVG_RELATIVE,
  GUI_SVG_LCCWARC_TO_ABS = GUI_SVG_LCCWARC_TO | GUI_SVG_ABSOLUTE,
  GUI_SVG_LCCWARC_TO_REL = GUI_SVG_LCCWARC_TO | GUI_SVG_RELATIVE,
  GUI_SVG_LCWARC_TO_ABS  = GUI_SVG_LCWARC_TO  | GUI_SVG_ABSOLUTE,
  GUI_SVG_LCWARC_TO_REL  = GUI_SVG_LCWARC_TO  | GUI_SVG_RELATIVE,
} GUI_SVG_PATH_COMMAND;

typedef enum {
  GUI_SVG_FILL_EVEN_ODD = 0x1900,
  GUI_SVG_FILL_NON_ZERO = 0x1901,
} GUI_SVG_FILL_RULE;

typedef enum {
  GUI_SVG_GRADIENT_OBJ_BOUND_BOX,
  GUI_SVG_GRADIENT_USER_SPACE_ON_USE,
} GUI_SVG_GRADIENT_UNITS;

typedef enum {
  GUI_SVG_GRADIENT_PAD     = 0x1C00,
  GUI_SVG_GRADIENT_REPEAT  = 0x1C01,
  GUI_SVG_GRADIENT_REFLECT = 0x1C02,
} GUI_SVG_GRADIENT_SPREAD;

typedef enum {
  GUI_SVG_LINEAR_GRADIENT,
  GUI_SVG_RADIAL_GRADIENT,
} GUI_SVG_GRADIENT_TYPE;

typedef enum {
  GUI_SVG_GRADIENT_REF_STOP_ARRAY = (1 << 0), // If true than haStops array is referenced, not owned.
} GUI_SVG_GRADIENT_FLAGS;

/*********************************************************************
*
*       Structure types
*
**********************************************************************
*/
//
// Abstract buffer type
//
typedef struct {
  void                     * pData;
  int	                       Width, Height;
  int	                       Stride;
  const LCD_API_COLOR_CONV * pColorConv;
} GUI_SVG_RENDER_TARGET;

typedef struct {
  GUI_HMEM hId;
  U32      StopColor;
  float    StopOpacity;
  float    Offset;
} GUI_SVG_GRADIENT_STOP;

typedef struct {
  GUI_SVG_GRADIENT_TYPE   Type;
  GUI_HMEM                hLink;
  GUI_HMEM                hId;
  GUI_SVG_GRADIENT_UNITS  Unit;
  GUI_SVG_GRADIENT_SPREAD Spread;
  GUI_MATRIX              Mat;
  GUI_ARRAY               haStops; // Array of GUI_SVG_GRADIENT_STOP
  U8                      Flags;   // Combination of GUI_SVG_GRADIENT_FLAGS
} GUI_SVG_GRADIENT;

typedef struct {
  GUI_SVG_GRADIENT Base;
  float            x1, y1, x2, y2;
} GUI_SVG_GRADIENT_LINEAR;

typedef struct {
  GUI_SVG_GRADIENT Base;
  float cx, cy; // Center point
  float r;      // Radius
  float fx, fy; // Focal point
} GUI_SVG_GRADIENT_RADIAL;

#define GUI_SVG_MAX_NUM_DASH_ITEMS 10
typedef struct {
  GUI_COLOR          ColorFill;
  GUI_COLOR          ColorStroke;
  float              aDashArray[GUI_SVG_MAX_NUM_DASH_ITEMS];
  int                NumDash;
  float              StrokeWidth;
  float              MiterLimit;
  GUI_SVG_CAP_STYLE  LineCap;
  GUI_SVG_JOIN_STYLE LineJoin;
  GUI_SVG_FILL_RULE  FillRule;
  GUI_SVG_GRADIENT * pGradientFill;
  GUI_SVG_GRADIENT * pGradientStroke;
  float              FillOpacity;
  float              StrokeOpacity;
  float              Opacity;
  float              StrokeDashoffset;
} GUI_SVG_STYLE;
//
// Context for handling paths.
//
typedef struct {
  GUI_POINTF InitialPos;           // Initial position of a sub-path.
  GUI_POINTF PenPos;
  GUI_POINTF LastQuadEndPos;
  GUI_POINTF LastQuadControlPos;
  GUI_POINTF LastCubicEndPos;
  GUI_POINTF LastCubicControlPos;
  U32        Flags;                // Flags to determine if above coords are valid. If set it is invalid. LSB is first struct member, and so on...
} GUI_SVG_PATH_CONTEXT;

//
// Indices for aMatrix[6]
//
#define ID_MATRIX_SVG              0    // Current affine transformation matrix based of all transformation read from the SVG file.
#define ID_MATRIX_TRANSLATE        1    // Translation set by GUI_SVG_Translate().
#define ID_MATRIX_ROTATE           2    // Rotation set by GUI_SVG_Rotate().
#define ID_MATRIX_SCALE            3    // Scale set by GUI_SVG_Scale().
#define ID_MATRIX_ROTATION_VECTOR  4    // Rotation vector set by GUI_SVG_SetRotationPoint().
#define ID_MATRIX_CUSTOM           5    // Custom transformation matrix set by GUI_SVG_Transform().
//
// Shorter aliases for matrices
//
#define MAT_SVG(pSVG)    (&pSVG->aMatrix[ID_MATRIX_SVG])
#define MAT_TR(pSVG)     (&pSVG->aMatrix[ID_MATRIX_TRANSLATE])
#define MAT_RO(pSVG)     (&pSVG->aMatrix[ID_MATRIX_ROTATE])
#define MAT_SC(pSVG)     (&pSVG->aMatrix[ID_MATRIX_SCALE])
#define MAT_CUST(pSVG)   (&pSVG->aMatrix[ID_MATRIX_CUSTOM])
#define VEC_RO(pSVG)     (&pSVG->aMatrix[ID_MATRIX_ROTATION_VECTOR])

typedef struct {
  GUI_HMEM             hFile;       // File content read from external memory. May be 0.
  const char         * pFile;       // Unlocked pointer of hFile if hFile!=0, otherwise user pointer to const data.
  U32                  Off;         // Offset in file.
  U32                  FileSize;    // Bytesize of file.
  U32                  OffDraw;     // File offset we go to when we draw the SVG.
  GUI_MATRIX           aMatrix[6];  // All kinds of matrices, see above defines.
  GUI_SVG_STYLE        Style;       // Top-most style attributes (<svg> level)
  GUI_SVG_INFO         Info;        // Public info struct, data is used internally by object, when user retrieves info this will be copied.
  GUI_ARRAY            haClasses;   // Array of CSS_CLASSes, optional.
  GUI_ARRAY            haGradients; // Array of GUI_SVG_GRADIENTs, optional.
  GUI_SVG_PATH_CONTEXT PathCtx;     // Path context, contains positions like pen position, required for normalizing paths.
  void               * pContext;    // Draw context managed by SVG driver.
} GUI_SVG_Obj;

typedef GUI_HMEM GUI_SVG_PATH_Handle;

typedef struct {
  //
  // Path API
  //
  GUI_SVG_PATH_Handle (* pfCreatePath)     (void);
  void                (* pfAppendPath)     (GUI_SVG_Obj * pObj, GUI_SVG_PATH_Handle hPath, int NumSegments, const U8 * pSegments, const float * pCoordinates);
  void                (* pfDestroyPath)    (GUI_SVG_Obj * pObj, GUI_SVG_PATH_Handle hPath);
  void                (* pfDrawPath)       (GUI_SVG_Obj * pObj, GUI_SVG_PATH_Handle hPath, GUI_SVG_STYLE * pStyle);
  void                (* pfGetScaleAndBias)(GUI_SVG_PATH_Handle hPath, float * pScale, float * pBias);
  //
  // Initializing global driver context
  //
  U8   (* pfInitDriver)  (void);
  void (* pfDeinitDriver)(void);
  //
  // Initializing draw context
  //
  U8   (* pfBeginDraw)(GUI_SVG_Obj * pObj);
  void (* pfEndDraw)  (GUI_SVG_Obj * pObj);
  //
  // Driver hooks
  //
  GUI_SVG_HOOKS * (* pfGetHooks)(void);
  void            (* pfSetHooks)(void);
} GUI_SVG_DRIVER_API;

/*********************************************************************
*
*       Private code
*
**********************************************************************
*/
void GUI_SVG__GetRenderTarget(GUI_SVG_RENDER_TARGET * pRT);

/*********************************************************************
*
*       Externals
*
**********************************************************************
*/
//
// Selected 2D vector API
//
extern const void * GUI_SVG__pAPI;
//
// Pointer to API wrappers
//
extern const GUI_SVG_DRIVER_API * GUI_SVG__pDriverAPI;
//
// API wrappers for each vector API
//
extern const GUI_SVG_DRIVER_API GUI_SVG__OpenVG_API;
extern const GUI_SVG_DRIVER_API GUI_SVG__VGLite_API;

#define GUI_SVG_DRIVER_OPENVG  &GUI_SVG__OpenVG_API
#define GUI_SVG_DRIVER_VGLITE  &GUI_SVG__VGLite_API

#if defined(__cplusplus)
}
#endif

#endif /* GUI_SVG_PRIVATE_H */

/*************************** End of file ****************************/
