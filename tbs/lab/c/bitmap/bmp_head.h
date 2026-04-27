/**
 * BITMAP
 * © 2025 0x4248
 */

#include <stdint.h>

#pragma pack(push, 1)

typedef struct {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
    /* Yeah I know its part of the file info and
     * not the header, its just easier putting it
     * into this struct.
     */
    uint32_t biSize;
} BM_FILE_HEAD;

/* V1 FILE INFO: 40 BYTES */
typedef struct {
    int32_t  biWidth;
    int32_t  biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t  biXPelsPerMeter;
    int32_t  biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} BM_INFO_V1;

typedef struct {
    /* V2 Data */
    uint32_t biRedMask;
    uint32_t biGreenMask;
    uint32_t biBlueMask;
} BM_INFO_V2;

typedef struct {
    /* V3 Data */
    uint32_t biAlphaMask;

} BM_INFO_V3;


#pragma pack(pop)
