#include <BmpProcessor.h>
#include <malloc.h>
#include <string.h>

BMP_Header* BMPHeader_init() {
    BMP_Header *bmpHeader = malloc(sizeof(BMP_Header));
    strcpy(bmpHeader->signature, "BM");
    bmpHeader->reserved1 = 0;
    bmpHeader->reserved2 = 0;

    return bmpHeader;
}

DIB_Header* DIBHeader_init() {
    DIB_Header *dibHeader = malloc(sizeof(DIB_Header));
    dibHeader->planes = 1;
    dibHeader->compression = 0;
    dibHeader->xPixelsPerMeter = 3780;
    dibHeader->yPixelsPerMeter = 3780;
    dibHeader->colorsInTable = 0;
    dibHeader->importantColors = 0;

//    dibHeader->size = size;
//    dibHeader->imgWidth = imgWidth;
//    dibHeader->imgHeight = imgHeight;
//    dibHeader->bitsPerPixel = bitsPerPixel;
//    dibHeader->imgSize = imgSize;
    return dibHeader;
}

BmpProcessor* BmpProcessor_init() {
    BmpProcessor *bmpP = malloc(sizeof (BmpProcessor));
    bmpP->bmpHeader = BMPHeader_init();
    bmpP->dibHeader = DIBHeader_init();
    return bmpP;
}

void BmpProcessor_clean(BmpProcessor *self) {
    free(self->bmpHeader);
    free(self->dibHeader);
}

/**
 * read BMP header of a file. Useful for converting files from PPM to BMP.
 *
 * @param  file: A pointer to the file being read or written
 * @param  header: Pointer to the destination BMP header
 */
void readBMPHeader(FILE* file, struct BMP_Header* header) {
    char temp[10];

    fread(&temp, sizeof(char) * 2, 1, file);
    fread(&header->size, sizeof (int), 1, file);
    fread(&temp, sizeof(short), 2, file);
    fread(&header->offset_pixel_array, sizeof(int), 1, file);
}

/**
 * write BMP header of a file. Useful for converting files from PPM to BMP.
 *
 * @param  file: A pointer to the file being read or written
 * @param  header: The header made by makeBMPHeader function
 */
void writeBMPHeader(FILE* file, struct BMP_Header* header) {

}

/**
 * read DIB header from a file. Useful for converting files from PPM to BMP.
 *
 * @param  file: A pointer to the file being read or written
 * @param  header: Pointer to the destination DIB header
 */
void readDIBHeader(FILE* file, struct DIB_Header* header) {

}

/**
 * write DIB header of a file. Useful for converting files from PPM to BMP.
 *
 * @param  file: A pointer to the file being read or written
 * @param  header: The header made by makeDIBHeader function
 */
void writeDIBHeader(FILE* file, struct DIB_Header* header) {

}

/**
 * make BMP header based on width and height.
 * The purpose of this is to create a new BMPHeader struct using the information
 * from a PPMHeader when converting from PPM to BMP.
 *
 * @param  header: Pointer to the destination DIB header
 * @param  width: Width of the image that this header is for
 * @param  height: Height of the image that this header is for
 */
void makeBMPHeader(struct BMP_Header* header, int width, int height) {

}


/**
* Makes new DIB header based on width and height. Useful for converting files from PPM to BMP.
*
* @param  header: Pointer to the destination DIB header
* @param  width: Width of the image that this header is for
* @param  height: Height of the image that this header is for
*/
void makeDIBHeader(struct DIB_Header* header, int width, int height) {

}


/**
 * read Pixels from BMP file based on width and height.
 *
 * @param  file: A pointer to the file being read or written
 * @param  pArr: Pixel array of the image that this header is for
 * @param  width: Width of the image that this header is for
 * @param  height: Height of the image that this header is for
 */
void readPixelsBMP(FILE* file, struct Pixel** pArr, int width, int height) {

}


/**
 * write Pixels from BMP file based on width and height.
 *
 * @param  file: A pointer to the file being read or written
 * @param  pArr: Pixel array of the image that this header is for
 * @param  width: Width of the image that this header is for
 * @param  height: Height of the image that this header is for
 */
void writePixelsBMP(FILE* file, struct Pixel** pArr, int width, int height) {

}
