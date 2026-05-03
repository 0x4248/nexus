/**
 * BITMAP
 * © 2025 0x4248
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "bmp_head.h"
#include "bmp_pixel.h"

void printRGB(PIXEL24 *pixels, int width, int height, int row_padded) {
    for (int x = 0; x < width; x++) {
        PIXEL24 *row = (PIXEL24 *)((uint8_t *)pixels + (height - 1 - x) * row_padded);
            for (int y = 0; y < height; y++) {
            printf("%d,%d:%u,%u,%u\n",
                   x, y, row[x].r, row[x].g, row[x].b);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <bitmap_file.bmp>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("Error opening file");
        return 1;
    }

    BM_FILE_HEAD fileHeader;


    fread(&fileHeader, sizeof(BM_FILE_HEAD), 1, fp);
    BM_INFO_V3 infoHeaderV3 = {0};
    BM_INFO_V2 infoHeaderV2 = {0};
    BM_INFO_V1 infoHeader = {0};

    printf("Reading base header\n");
    fread(&infoHeader, sizeof(BM_INFO_V1), 1, fp);

    switch (fileHeader.biSize) {
        case 52:
            printf("Using V2\n");
            fread(&infoHeaderV2, sizeof(BM_INFO_V2), 1, fp);
            break;

        case 56:
            printf("Using V2\n");
            fread(&infoHeaderV2, sizeof(BM_INFO_V2), 1, fp);
            printf("Using V3\n");
            fread(&infoHeaderV3, sizeof(BM_INFO_V3), 1, fp);
            break;

        default:
            printf("Only using V1");
            fclose(fp);
            return 1;
    }

    if (fileHeader.bfType != 0x4D42) { // 'BM' in little endian
        printf("Error: Not a valid BMP file.\n");
        fclose(fp);
        return 1;
    }

    printf("\n=== FILE HEADER ===\n");
    printf("File Size: %u bytes\n", fileHeader.bfSize);
    printf("Data Offset: %u bytes\n", fileHeader.bfOffBits);
    printf("Header Size: %u bytes\n", fileHeader.biSize);

    printf("\n=== BITMAP INFO ===\n");
    printf("Width: %d px\n", infoHeader.biWidth);
    printf("Height: %d px\n", infoHeader.biHeight);
    printf("Bits per Pixel: %u\n", infoHeader.biBitCount);

    int row_bytes = (infoHeader.biWidth * 3 + 3) & ~3;
    int width = infoHeader.biWidth;
    int height = infoHeader.biHeight;
    int row_padded = (width * sizeof(PIXEL24) + 3) & ~3;

    PIXEL24 *pixels = malloc(row_padded * height);

    fseek(fp, fileHeader.bfOffBits, SEEK_SET);

    for (int y = 0; y < height; y++) {
        fread((uint8_t*)pixels + y * row_padded, 1, row_padded, fp);
    }

    printRGB(pixels, width, height, row_padded);

    fclose(fp);
    return 0;
}
