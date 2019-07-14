#include <stdint.h>

typedef uint8_t  BYTE1;
typedef uint16_t BYTE2;

typedef struct{
    BYTE1 SOI[2];
    BYTE2 APP0;
    BYTE2 Length;
    BYTE1 Identifier[5];
    BYTE2 Version;
    BYTE1 Units;
    BYTE2 Xdensity;
    BYTE2 Ydensity;
    BYTE1 XThumbnail;
    BYTE1 YThumbnail;
} JPEGHEADER;

typedef struct{
    BYTE1 data[2];
} DATA;