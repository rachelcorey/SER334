#include <BmpProcessor.h>
#include <malloc.h>
#include <string.h>

/**
 * Initializes a BMP Header Object
 **/
BMP_Header *BMPHeader_init() {
    BMP_Header *bmpHeader = malloc(sizeof(BMP_Header));
    strcpy(bmpHeader->signature, "BM");
    bmpHeader->reserved1 = 0;
    bmpHeader->reserved2 = 0;
    return bmpHeader;
}

/**
 * Initializes a DIB Header Object
 **/
DIB_Header *DIBHeader_init() {
    DIB_Header *dibHeader = malloc(sizeof(DIB_Header));
    dibHeader->size = 40;
    dibHeader->planes = 1;
    dibHeader->compression = 0;
    dibHeader->xPixelsPerMeter = 3780;
    dibHeader->yPixelsPerMeter = 3780;
    dibHeader->colorsInTable = 0;
    dibHeader->importantColors = 0;
    return dibHeader;
}


/**
 * Initializes a BMP Processor Object
 **/
BmpProcessor *BmpProcessor_init() {
    BmpProcessor *bmpP = malloc(sizeof(BmpProcessor));
    bmpP->bmpHeader = BMPHeader_init();
    bmpP->dibHeader = DIBHeader_init();
    return bmpP;
}

/**
 * Cleans the BMP Processor Object
 * @param self a reference to the BMP Processor Object to be freed
 */
void BmpProcessor_clean(BmpProcessor *self) {
    free(self->bmpHeader);
    free(self->dibHeader);
    free(self);
}

/**
 * read BMP header of a file. Useful for converting files from PPM to BMP.
 *
 * @param  file: A pointer to the file being read or written
 * @param  header: Pointer to the destination BMP header
 */
void readBMPHeader(FILE *file, struct BMP_Header *header) {
    char temp[10] = "";
    fread(&temp, sizeof(char) * 2, 1, file);
    fread(&header->size, sizeof(int), 1, file);
    fread(&temp, sizeof(short), 2, file);
    fread(&header->offset_pixel_array, sizeof(int), 1, file);
}

/**
 * write BMP header of a file. Useful for converting files from PPM to BMP.
 *
 * @param  file: A pointer to the file being read or written
 * @param  header: The header made by makeBMPHeader function
 */
void writeBMPHeader(FILE *file, struct BMP_Header *header) {
    fwrite("BM", sizeof(char) * 2, 1, file);
    fwrite(&header->size, sizeof(int), 1, file);
    fwrite(&header->reserved1, sizeof(short), 1, file);
    fwrite(&header->reserved2, sizeof(short), 1, file);
    fwrite(&header->offset_pixel_array, sizeof(int), 1, file);
}

/**
 * read DIB header from a file. Useful for converting files from PPM to BMP.
 *
 * @param  file: A pointer to the file being read or written
 */
void readDIBHeader(FILE *file, struct DIB_Header *header) {
    fseek(file, 4, SEEK_CUR);
    fread(&header->imgWidth, sizeof(signed int), 1, file);
    fread(&header->imgHeight, sizeof(signed int), 1, file);
    fseek(file, 2, SEEK_CUR);
    fread(&header->bitsPerPixel, sizeof(unsigned short), 1, file);
    fseek(file, 4, SEEK_CUR);
    fread(&header->imgSize, sizeof(int), 1, file);
    fseek(file, 16, SEEK_CUR);
}

/**
 * write DIB header of a file. Useful for converting files from PPM to BMP.
 *
 * @param  file: A pointer to the file being read or written
 * @param  header: The header made by makeDIBHeader function
 */
void writeDIBHeader(FILE *file, struct DIB_Header *header) {
    fwrite(&header->size, sizeof(int), 1, file);
    fwrite(&header->imgWidth, sizeof(int), 1, file);
    fwrite(&header->imgHeight, sizeof(int), 1, file);
    fwrite(&header->planes, sizeof(short), 1, file);
    fwrite(&header->bitsPerPixel, sizeof(short), 1, file);
    fwrite(&header->compression, sizeof(int), 1, file);
    fwrite(&header->imgSize, sizeof(int), 1, file);
    fwrite(&header->xPixelsPerMeter, sizeof(int), 1, file);
    fwrite(&header->yPixelsPerMeter, sizeof(int), 1, file);
    fwrite(&header->colorsInTable, sizeof(int), 1, file);
    fwrite(&header->importantColors, sizeof(int), 1, file);
}

/**
 * make BMP header based on width and height.
 * The purpose of this is to create a new BMPHeader struct using the information
 * from a PPMHeader when converting from PPM to BMP.
 *
 * @param  header: Pointer to the destination DIB header
 */
void makeBMPHeader(struct BMP_Header *header) {
    header->offset_pixel_array = 54;
}


/**
* Makes new DIB header based on width and height. Useful for converting files from PPM to BMP.
*
* @param  header: Pointer to the destination DIB header
* @param  width: Width of the image that this header is for
* @param  height: Height of the image that this header is for
*/
void makeDIBHeader(struct DIB_Header *header, int width, int height) {
    header->bitsPerPixel = 24;
    header->imgWidth = width;
    header->imgHeight = height;
}

