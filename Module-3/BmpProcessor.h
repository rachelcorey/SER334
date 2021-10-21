#include <bits/types/FILE.h>

typedef struct BMP_Header {
    char signature[2];
    int size;
    short reserved1;
    short reserved2;
    int offset_pixel_array;
} BMP_Header;

typedef struct DIB_Header{
    int size;
    int imgWidth;
    int imgHeight;
    int planes;
    int bitsPerPixel;
    int compression;
    int imgSize;
    int xPixelsPerMeter;
    int yPixelsPerMeter;
    int colorsInTable;
    int importantColors;
} DIB_Header;

typedef struct BmpProcessor {
    struct BMP_Header *bmpHeader;
    struct DIB_Header *dibHeader;
} BmpProcessor;
//
///**
// * read BMP header of a file. Useful for converting files from PPM to BMP.
// *
// * @param  file: A pointer to the file being read or written
// * @param  header: Pointer to the destination BMP header
// */
//void readBMPHeader(FILE* file, struct BMP_Header* header);
//
///**
// * write BMP header of a file. Useful for converting files from PPM to BMP.
// *
// * @param  file: A pointer to the file being read or written
// * @param  header: The header made by makeBMPHeader function
// */
//void writeBMPHeader(FILE* file, struct BMP_Header* header);
//
///**
// * read DIB header from a file. Useful for converting files from PPM to BMP.
// *
// * @param  file: A pointer to the file being read or written
// * @param  header: Pointer to the destination DIB header
// */
//void readDIBHeader(FILE* file, struct DIB_Header* header);
//
///**
// * write DIB header of a file. Useful for converting files from PPM to BMP.
// *
// * @param  file: A pointer to the file being read or written
// * @param  header: The header made by makeDIBHeader function
// */
//void writeDIBHeader(FILE* file, struct DIB_Header* header);
//
///**
// * make BMP header based on width and height.
// * The purpose of this is to create a new BMPHeader struct using the information
// * from a PPMHeader when converting from PPM to BMP.
// *
// * @param  header: Pointer to the destination DIB header
// * @param  width: Width of the image that this header is for
// * @param  height: Height of the image that this header is for
// */
//void makeBMPHeader(struct BMP_Header* header, int width, int height);
//
//
// /**
// * Makes new DIB header based on width and height. Useful for converting files from PPM to BMP.
// *
// * @param  header: Pointer to the destination DIB header
// * @param  width: Width of the image that this header is for
// * @param  height: Height of the image that this header is for
// */
//void makeDIBHeader(struct DIB_Header* header, int width, int height);


/**
 * read Pixels from BMP file based on width and height.
 *
 * @param  file: A pointer to the file being read or written
 * @param  pArr: Pixel array of the image that this header is for
 * @param  width: Width of the image that this header is for
 * @param  height: Height of the image that this header is for
 */
//void readPixelsBMP(FILE* file, struct Pixel** pArr, int width, int height);


/**
 * write Pixels from BMP file based on width and height.
 *
 * @param  file: A pointer to the file being read or written
 * @param  pArr: Pixel array of the image that this header is for
 * @param  width: Width of the image that this header is for
 * @param  height: Height of the image that this header is for
 */
//void writePixelsBMP(FILE* file, struct Pixel** pArr, int width, int height);