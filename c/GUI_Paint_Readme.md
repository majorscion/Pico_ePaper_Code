# GUI Paint Library Documentation

## Overview

GUI_Paint is a graphics library designed for embedded systems, specifically for driving e-Paper (e-ink) displays. It provides functions for drawing basic shapes, displaying text (both English and Chinese), and managing image buffers with rotation and mirroring capabilities.

**Library Version:** V3.2 (2020-07-10)  
**Author:** Waveshare electronics  
**Target Platform:** Raspberry Pi Pico / Embedded systems using C

---

## Data Types

| Type | Definition | Description |
|------|------------|-------------|
| `UBYTE` | `uint8_t` | 8-bit unsigned integer (byte) |
| `UWORD` | `uint16_t` | 16-bit unsigned integer (word) |
| `UDOUBLE` | `uint32_t` | 32-bit unsigned integer |

---

## Color Definitions

| Constant | Value | Description |
|----------|-------|-------------|
| `WHITE` | `0xFF` | White/Background color |
| `BLACK` | `0x00` | Black/Foreground color |
| `RED` | `BLACK` | Red (same as black for monochrome displays) |

### 4-Level Gray Scale (for grayscale e-Paper)
| Constant | Value | Description |
|----------|-------|-------------|
| `GRAY1` | `0x03` | Blackest (darkest) |
| `GRAY2` | `0x02` | Dark gray |
| `GRAY3` | `0x01` | Light gray |
| `GRAY4` | `0x00` | White (lightest) |

---

## Image Attributes Structure (`PAINT`)

```c
typedef struct {
    UBYTE  *Image;          // Pointer to the image buffer/cache
    UWORD  Width;           // Display width (after rotation)
    UWORD  Height;          // Display height (after rotation)
    UWORD  WidthMemory;     // Memory width (actual buffer width)
    UWORD  HeightMemory;    // Memory height (actual buffer height)
    UWORD  Color;           // Inversion flag
    UWORD  Rotate;          // Rotation angle (0, 90, 180, 270)
    UWORD  Mirror;          // Mirror mode
    UWORD  WidthByte;       // Width in bytes (depends on scale)
    UWORD  HeightByte;      // Height in bytes
    UWORD  Scale;           // Color depth scale (2, 4, or 7)
} PAINT;
```

---

## Rotation Modes

| Constant | Value | Description |
|----------|-------|-------------|
| `ROTATE_0` | 0 | No rotation (portrait) |
| `ROTATE_90` | 90 | Rotated 90° clockwise |
| `ROTATE_180` | 180 | Rotated 180° |
| `ROTATE_270` | 270 | Rotated 270° clockwise |

---

## Mirror Modes

| Constant | Value | Description |
|----------|-------|-------------|
| `MIRROR_NONE` | `0x00` | No mirroring |
| `MIRROR_HORIZONTAL` | `0x01` | Horizontal flip |
| `MIRROR_VERTICAL` | `0x02` | Vertical flip |
| `MIRROR_ORIGIN` | `0x03` | Flip across origin (both axes) |

---

## Dot Pixel Sizes

| Constant | Size | Description |
|----------|------|-------------|
| `DOT_PIXEL_1X1` | 1×1 | Single pixel |
| `DOT_PIXEL_2X2` | 2×2 | 2×2 square |
| `DOT_PIXEL_3X3` | 3×3 | 3×3 square |
| `DOT_PIXEL_4X4` | 4×4 | 4×4 square |
| `DOT_PIXEL_5X5` | 5×5 | 5×5 square |
| `DOT_PIXEL_6X6` | 6×6 | 6×6 square |
| `DOT_PIXEL_7X7` | 7×7 | 7×7 square |
| `DOT_PIXEL_8X8` | 8×8 | 8×8 square |

---

## Dot Fill Styles

| Constant | Value | Description |
|----------|-------|-------------|
| `DOT_FILL_AROUND` | 1 | Surrounding fill (symmetrical) |
| `DOT_FILL_RIGHTUP` | - | Right-up corner fill |

---

## Line Styles

| Constant | Value | Description |
|----------|-------|-------------|
| `LINE_STYLE_SOLID` | 0 | Solid line |
| `LINE_STYLE_DOTTED` | 1 | Dotted line (pattern: 2 solid, 1 gap) |

---

## Draw Fill Modes

| Constant | Value | Description |
|----------|-------|-------------|
| `DRAW_FILL_EMPTY` | 0 | Outline only (hollow) |
| `DRAW_FILL_FULL` | 1 | Filled shape |

---

# Core Functions

## 1. Image Creation and Selection

### `Paint_NewImage()`

**Purpose:** Initialize a new image with specified properties.

**Parameters:**
- `image`: Pointer to the image buffer/cache
- `Width`: Image width in pixels
- `Height`: Image height in pixels
- `Rotate`: Rotation angle (0, 90, 180, 270)
- `Color`: Initial color/inversion setting

**How it works:**
- Sets up the global `Paint` structure with image properties
- Calculates memory dimensions based on width and rotation
- Default scale is set to 2 (1-bit color depth)
- Adjusts width/height based on rotation (swaps if 90° or 270°)

**Memory calculation:**
```
WidthByte = (Width % 8 == 0) ? (Width / 8) : (Width / 8 + 1)
```

---

### `Paint_SelectImage()`

**Purpose:** Select an existing image buffer for drawing operations.

**Parameters:**
- `image`: Pointer to the image buffer

**How it works:**
- Changes the active image buffer without modifying other properties
- Useful for double-buffering or switching between images

---

## 2. Image Transformation

### `Paint_SetRotate()`

**Purpose:** Set the rotation angle for subsequent drawing operations.

**Parameters:**
- `Rotate`: Rotation angle (0, 90, 180, 270)

**How it works:**
- Updates the global rotation setting
- Coordinates are transformed before pixel manipulation:
  - **0°**: No transformation
  - **90°**: (x, y) → (width - y - 1, x)
  - **180°**: (x, y) → (width - x - 1, height - y - 1)
  - **270°**: (x, y) → (y, height - x - 1)

---

### `Paint_SetMirroring()`

**Purpose:** Set the mirroring mode for subsequent drawing operations.

**Parameters:**
- `mirror`: Mirror mode (MIRROR_NONE, MIRROR_HORIZONTAL, MIRROR_VERTICAL, MIRROR_ORIGIN)

**How it works:**
- Applies coordinate transformation after rotation
- **HORIZONTAL**: x → width - x - 1
- **VERTICAL**: y → height - y - 1
- **ORIGIN**: Both x and y are mirrored

---

### `Paint_SetScale()`

**Purpose:** Set the color depth scale for the display.

**Parameters:**
- `scale`: Scale factor (2, 4, or 7)

**How it works:**

#### Scale 2 (1-bit per pixel - Monochrome)
```
WidthByte = (Width % 8 == 0) ? (Width / 8) : (Width / 8 + 1)
Each byte holds 8 pixels (1 bit per pixel)
```

#### Scale 4 (2-bit per pixel - 4-level grayscale)
```
WidthByte = (Width % 4 == 0) ? (Width / 4) : (Width / 4 + 1)
Each byte holds 4 pixels (2 bits per pixel)
Color range: 0-3 (GRAY1 to GRAY4)
```

#### Scale 7 (4-bit per pixel - 16-level grayscale for 5.65" e-Paper)
```
WidthByte = (Width % 2 == 0) ? (Width / 2) : (Width / 2 + 1)
Each byte holds 2 pixels (4 bits per pixel)
Color range: 0-15
```

---

## 3. Pixel Operations

### `Paint_SetPixel()`

**Purpose:** Draw a single pixel at the specified coordinates.

**Parameters:**
- `Xpoint`: X coordinate
- `Ypoint`: Y coordinate
- `Color`: Color value (depends on scale)

**How it works:**
1. Checks if coordinates are within display boundaries
2. Applies rotation transformation to coordinates
3. Applies mirroring transformation
4. Calculates memory address based on scale:
   - **Scale 2**: `Addr = X / 8 + Y * WidthByte`
     - Sets/clears bit at position `X % 8` in the byte
   - **Scale 4**: `Addr = X / 4 + Y * WidthByte`
     - Uses 2 bits at position `(X % 4) * 2`
   - **Scale 7**: `Addr = X / 2 + Y * WidthByte`
     - Uses 4 bits at position `(X % 2) * 4`

**Color values by scale:**
- Scale 2: BLACK (0x00), WHITE (0xFF)
- Scale 4: GRAY1-GRAY4 (0x00-0x03)
- Scale 7: 0x00-0x0F (16 levels)

---

### `Paint_Clear()`

**Purpose:** Clear the entire image buffer with a specified color.

**Parameters:**
- `Color`: Fill color

**How it works:**
- Iterates through all bytes in the image buffer
- Sets each byte to the specified color value
- Special handling for scale 7: `(Color << 4) | Color` (duplicates color in both nibbles)

---

### `Paint_ClearWindows()`

**Purpose:** Clear a rectangular region of the image.

**Parameters:**
- `Xstart`, `Ystart`: Top-left corner coordinates
- `Xend`, `Yend`: Bottom-right corner coordinates (exclusive)
- `Color`: Fill color

**How it works:**
- Iterates through all pixels in the rectangle
- Calls `Paint_SetPixel()` for each position

---

## 4. Drawing Functions

### `Paint_DrawPoint()`

**Purpose:** Draw a point with customizable size and style.

**Parameters:**
- `Xpoint`, `Ypoint`: Center coordinates
- `Color`: Point color
- `Dot_Pixel`: Size (1×1 to 8×8)
- `Dot_Style`: Fill style (DOT_FILL_AROUND or DOT_FILL_RIGHTUP)

**How it works:**
- **DOT_FILL_AROUND**: Draws a square of size `(2*Dot_Pixel - 1) × (2*Dot_Pixel - 1)` centered on the point
- **DOT_FILL_RIGHTUP**: Draws a `Dot_Pixel × Dot_Pixel` square starting from the point

---

### `Paint_DrawLine()`

**Purpose:** Draw a line between two points using Bresenham's algorithm.

**Parameters:**
- `Xstart`, `Ystart`: Starting coordinates
- `Xend`, `Yend`: Ending coordinates
- `Color`: Line color
- `Line_width`: Line thickness (dot pixel size)
- `Line_Style`: Solid or dotted

**How it works:**
- Implements Bresenham's line algorithm with error accumulation
- For dotted lines: every 3rd pixel is replaced with background color
- Uses `Paint_DrawPoint()` to render each point with specified thickness

---

### `Paint_DrawRectangle()`

**Purpose:** Draw a rectangle or filled rectangle.

**Parameters:**
- `Xstart`, `Ystart`: Top-left corner coordinates
- `Xend`, `Yend`: Bottom-right corner coordinates
- `Color`: Border color
- `Line_width`: Border thickness
- `Draw_Fill`: Whether to fill the interior

**How it works:**
- **Filled**: Draws horizontal lines from left to right for each row
- **Outline**: Draws 4 border lines using `Paint_DrawLine()`

---

### `Paint_DrawCircle()`

**Purpose:** Draw a circle or filled circle using the midpoint circle algorithm.

**Parameters:**
- `X_Center`, `Y_Center`: Center coordinates
- `Radius`: Circle radius
- `Color`: Circle color
- `Line_width`: Border thickness (for hollow circles)
- `Draw_Fill`: Whether to fill the interior

**How it works:**
- Uses 8-point symmetry to calculate circle points
- **Filled**: Draws vertical line segments between symmetric points
- **Hollow**: Draws individual points at symmetric positions
- Algorithm uses error term `Esp` to determine next pixel position

---

## 5. Text Display Functions

### `Paint_DrawChar()`

**Purpose:** Draw a single ASCII character.

**Parameters:**
- `Xpoint`, `Ypoint`: Top-left coordinates
- `Acsii_Char`: Character to draw (ASCII 32-126)
- `Font`: Font structure pointer
- `Color_Foreground`: Text color
- `Color_Background`: Background color

**How it works:**
1. Calculates character offset in font table:
   ```
   Char_Offset = (Acsii_Char - ' ') * Font->Height * (Font->Width / 8 + 1)
   ```
2. Iterates through font bitmap data
3. For each bit in the font pattern:
   - If bit is set: draw foreground color pixel
   - If bit is clear: draw background color pixel (unless it matches)

**Font format:** Each row of pixels is stored as bytes, MSB first

---

### `Paint_DrawString_EN()`

**Purpose:** Draw an English string.

**Parameters:**
- `Xstart`, `Ystart`: Starting coordinates
- `pString`: String to display
- `Font`: Font structure pointer
- `Color_Foreground`: Text color
- `Color_Background`: Background color

**How it works:**
- Calls `Paint_DrawChar()` for each character
- Wraps to next line when X reaches display edge
- Resets to top-left when Y reaches display edge

---

### `Paint_DrawString_CN()`

**Purpose:** Draw Chinese (GB2312) and English mixed strings.

**Parameters:**
- `Xstart`, `Ystart`: Starting coordinates
- `pString`: String to display
- `font`: Chinese font structure pointer
- `Color_Foreground`: Text color
- `Color_Background`: Background color

**How it works:**
1. Checks if character is ASCII (≤ 0x7F) or Chinese (> 0x7F)
2. **ASCII characters:** Searches in font table by first byte, displays with `ASCII_Width` spacing
3. **Chinese characters:** Searches in font table by 2-byte sequence, displays with `Width` spacing
4. Each character uses the same bitmap rendering as `Paint_DrawChar()`

**Font structure for Chinese:**
```c
typedef struct {
    char index[2];              // 2-byte GB2312 code
    char matrix[height*width/8+2];  // Bitmap data
} CH_CN;
```

---

### `Paint_DrawNum()`

**Purpose:** Display an integer number.

**Parameters:**
- `Xpoint`, `Ypoint`: Starting coordinates
- `Nummber`: Integer to display (int32_t)
- `Font`: Font structure pointer
- `Color_Foreground`: Text color
- `Color_Background`: Background color

**How it works:**
1. Converts number to string by repeatedly extracting digits:
   ```
   digit = number % 10 + '0'
   number /= 10
   ```
2. Reverses the digit array
3. Calls `Paint_DrawString_EN()` to display

---

### `Paint_DrawTime()`

**Purpose:** Display time in HH:MM:SS format.

**Parameters:**
- `Xstart`, `Ystart`: Starting coordinates
- `pTime`: Time structure pointer
- `Font`: Font structure pointer
- `Color_Foreground`: Text color
- `Color_Background`: Background color

**How it works:**
- Displays time as: `HH:MM:SS`
- Calculates digit positions based on font width
- Uses fixed spacing for consistent alignment:
  - Hours tens and units
  - Colon separator (offset by 1/4 + 1/2 font width)
  - Minutes tens and units
  - Second colon
  - Seconds tens and units

---

### `Paint_DrawBitMap()`

**Purpose:** Display a pre-converted bitmap image.

**Parameters:**
- `image_buffer`: Pointer to bitmap data array

**How it works:**
- Copies entire buffer directly to image cache
- Assumes bitmap matches current display dimensions
- No color conversion or scaling performed

---

# Usage Example

```c
#include "GUI_Paint.h"
#include "DEV_Config.h"

// Allocate image buffer (example for 128x64 display, scale 2)
UBYTE image[128 * 64 / 8];

int main() {
    // Initialize image
    Paint_NewImage(image, 128, 64, ROTATE_0, WHITE);
    Paint_SetScale(2);  // 1-bit color
    
    // Clear display
    Paint_Clear(WHITE);
    
    // Draw a line
    Paint_DrawLine(0, 0, 127, 63, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    
    // Draw a rectangle
    Paint_DrawRectangle(10, 10, 50, 50, BLACK, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
    
    // Draw a filled circle
    Paint_DrawCircle(96, 32, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    
    // Display text
    Paint_DrawString_EN(5, 55, "Hello", &Font12, BLACK, WHITE);
    
    // Display number
    Paint_DrawNum(80, 55, 12345, &Font12, BLACK, WHITE);
    
    // Send image to display (implementation-specific)
    DEV_Display(image);
    
    return 0;
}
```

---

# Memory Layout

## Scale 2 (Monochrome)
- **1 byte = 8 pixels** (horizontal)
- Row-major order
- MSB (bit 7) = leftmost pixel, LSB (bit 0) = rightmost pixel

Example for 128×64 display:
```
Buffer size: 128 × 64 / 8 = 1024 bytes
Row 0: bytes 0-15 (128 pixels)
Row 1: bytes 16-31 (128 pixels)
...
```

## Scale 4 (4-level grayscale)
- **1 byte = 4 pixels** (horizontal)
- Each pixel uses 2 bits
- MSB pair = leftmost pixel

Example for 128×64 display:
```
Buffer size: 128 × 64 / 4 = 2048 bytes
Row 0: bytes 0-31 (128 pixels)
...
```

## Scale 7 (16-level grayscale)
- **1 byte = 2 pixels** (horizontal)
- Each pixel uses 4 bits
- Upper nibble = left pixel, lower nibble = right pixel

Example for 128×64 display:
```
Buffer size: 128 × 64 / 2 = 4096 bytes
Row 0: bytes 0-63 (128 pixels)
...
```

---

# Coordinate System

The library uses a **Cartesian coordinate system** with the origin (0,0) at the **top-left corner** of the display.

```
    X →
Y 0 ┌─────────────┐
↓   │             │
    │             │
    │             │
    └─────────────┘
   (width-1,height-1)
```

Coordinates are transformed in this order:
1. **Rotation** (applied first)
2. **Mirroring** (applied second)

---

# Font Information

## English Fonts (sFONT)
| Font | Width | Height |
|------|-------|--------|
| Font8 | 5 | 8 |
| Font12 | 7 | 12 |
| Font16 | 9 | 16 |
| Font20 | 11 | 20 |
| Font24 | 13 | 24 |

## Chinese Fonts (cFONT)
| Font | Width | Height | ASCII_Width |
|------|-------|--------|-------------|
| Font12CN | 24 | 24 | 12 |
| Font24CN | 48 | 48 | 24 |

---

# Notes

1. **Boundary checking:** All drawing functions check if coordinates are within display bounds
2. **Coordinate transformation:** Rotation and mirroring are applied automatically in `Paint_SetPixel()`
3. **Memory efficiency:** Scale 2 uses the least memory (1 bit per pixel)
4. **Performance:** Direct buffer manipulation is used for optimal speed on embedded systems
5. **Color inversion:** The `Color` field in PAINT structure controls display inversion

---

# Version History

- **V3.2 (2020-07-10):** Added scale 7 support for 5.65" e-Paper
- **V3.1 (2019-10-10):** Added gray level support
- **V3.0 (2019-04-18):** Updated function signatures for consistency
- **V2.0 (2018-11-15):** Added rotation, mirroring, Chinese character support
- **V1.0 (2018-07-17):** Initial release
