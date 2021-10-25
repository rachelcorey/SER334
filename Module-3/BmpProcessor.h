#include <bits/types/FILE.h>

typedef struct BMP_Header {
    char signature[2];
    int size;
    short reserved1;
    short reserved2;
    int offset_pixel_array;
} BMP_Header;

typedef struct DIB_Header {
    int size;
    signed int imgWidth;
    signed int imgHeight;
    int planes;
    unsigned short bitsPerPixel;
    int compression;
    int imgSize;
    signed int xPixelsPerMeter;
    signed int yPixelsPerMeter;
    int colorsInTable;
    int importantColors;
} DIB_Header;

typedef struct BmpProcessor {
    struct BMP_Header *bmpHeader;
    struct DIB_Header *dibHeader;
} BmpProcessor;

/**
 * read BMP header of a file. Useful for converting files from PPM to BMP.
 *
 * @param  file: A pointer to the file being read or written
 * @param  header: Pointer to the destination BMP header
 */
void readBMPHeader(FILE *file, struct BMP_Header *header);

/**
 * write BMP header of a file. Useful for converting files from PPM to BMP.
 *
 * @param  file: A pointer to the file being read or written
 * @param  header: The header made by makeBMPHeader function
 */
void writeBMPHeader(FILE *file, struct BMP_Header *header);

/**
 * read DIB header from a file. Useful for converting files from PPM to BMP.
 *
 * @param  file: A pointer to the file being read or written
 * @param  header: Pointer to the destination DIB header
 */
void readDIBHeader(FILE *file, struct DIB_Header *header);

/**
 * write DIB header of a file. Useful for converting files from PPM to BMP.
 *
 * @param  pP: the reference to this PixelProcessor object where the pixel array is stored
 * @param  rShift: the shift value of color r shift
 * @param  gShift: the shift value of color g shift
 * @param  bShift: the shift value of color b shift
*/
void writeDIBHeader(FILE *file, struct DIB_Header *header);

/**
 * make BMP header based on width and height.
 * The purpose of this is to create a new BMPHeader struct using the information
 * from a PPMHeader when converting from PPM to BMP.
 *
 * @param  header: Pointer to the destination DIB header
 */
void makeBMPHeader(struct BMP_Header *header);

/**
* Makes new DIB header based on width and height. Useful for converting files from PPM to BMP.
*
* @param  header: Pointer to the destination DIB header
* @param  width: Width of the image that this header is for
* @param  height: Height of the image that this header is for
*/
void makeDIBHeader(struct DIB_Header *header, int width, int height);


