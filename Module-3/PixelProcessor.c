#include <PixelProcessor.h>
#include <malloc.h>

struct PixelProcessor* PixelProcessor_init(int imgWidth, int imgHeight) {
    PixelProcessor* pixelProcessor = malloc(sizeof(PixelProcessor));
    struct Pixel** pixelArray = malloc((sizeof(struct Pixel) * imgWidth * 4) * (sizeof(struct Pixel) * imgHeight));
    pixelProcessor->width = imgWidth;
    pixelProcessor->height = imgHeight;
    pixelProcessor->pixels = pixelArray;
    return pixelProcessor;
}

struct Pixel* Pixel_init(unsigned char red, unsigned char green, unsigned char blue) {
    struct Pixel* pixel = malloc(sizeof (struct Pixel));
    pixel->red = red;
    pixel->green = green;
    pixel->blue = blue;
    return pixel;
}

void buildPixelArray(PixelProcessor *self, struct Pixel* pix, int x, int y) {
    self->pixels[x][y] = *pix;
}

/**
 * Shift color of Pixel array. The dimension of the array is width * height. The shift value of RGB is
 * rShift, gShift，bShift. Useful for color shift.
 *
 * @param  pArr: Pixel array of the image that this header is for
 * @param  width: Width of the image that this header is for
 * @param  height: Height of the image that this header is for
 * @param  rShift: the shift value of color r shift
 * @param  gShift: the shift value of color g shift
 * @param  bShift: the shift value of color b shift
 */
void colorShiftPixels(struct Pixel** pArr, int width, int height, int rShift, int gShift, int bShift) {

}
