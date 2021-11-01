#include <malloc.h>

struct Pixel {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
};

typedef struct PixelProcessor {
    struct Pixel *pixels;
    int width;
    int height;
} PixelProcessor;

/**
 * Shift color of Pixel array. The shift value of RGB is
 * rShift, gShift，bShift. Useful for color shift.
 *
 * @param  pP: the reference to this PixelProcessor object where the pixel array is stored
 * @param  rShift: the shift value of color r shift
 * @param  gShift: the shift value of color g shift
 * @param  bShift: the shift value of color b shift
 */
void colorShiftPixels(struct PixelProcessor *pP, int rShift, int gShift, int bShift);

/**
 * Read pixels from a BMP file
 * @param file BMP file to be read
 * @param self reference to this PixelProcessor object to store the data in
 * @param paddingBytes amount of padding to read from the BMP
 */
void readPixelsBMP(FILE *file, struct PixelProcessor *self, int paddingBytes);

/**
 * Write pixels to a BMP file
 * @param file file to be written to
 * @param pP a reference to this PixelProcessor object
 * @param pad the amount of padding to write
 */
void writePixelsBMP(FILE *file, PixelProcessor *pP, int pad);
