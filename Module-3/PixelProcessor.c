#include <PixelProcessor.h>
#include <malloc.h>

struct PixelProcessor* PixelProcessor_init(int imgWidth, int imgHeight) {
    PixelProcessor* pixelProcessor = malloc(sizeof(PixelProcessor));
    struct Pixel** pixelArray = malloc(((sizeof(struct Pixel)) * imgWidth * 4));
    for (int i = 0; i < imgHeight; ++i) {
        pixelArray[i] = malloc((sizeof(struct Pixel)) * imgHeight);
    }
    pixelProcessor->width = imgWidth;
    pixelProcessor->height = imgHeight;
    pixelProcessor->pixels = pixelArray;
    return pixelProcessor;
}

struct Pixel* Pixel_init(unsigned char blue, unsigned char green, unsigned char red) {
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
 * read Pixels from BMP file based on width and height.
 *
 * @param  file: A pointer to the file being read or written
 */
void readPixelsBMP(FILE* file, struct PixelProcessor *pixelProcessor) {
    for (int i = 0; i < pixelProcessor->height; ++i) {
        for (int j = 0; j < pixelProcessor->width; ++j) {
            unsigned char b, g, r;
            fread(&b, sizeof(char), 1, file);
            fread(&g, sizeof(char), 1, file);
            fread(&r, sizeof(char), 1, file);
            struct Pixel *p = Pixel_init(b, g, r);
            buildPixelArray(pixelProcessor, p, i, j);
        }
    }


//    for (int i = 0; i < pixelProcessor->height; ++i) {
//        for (int j = 0; j < pixelProcessor->width; ++j) {
//            printf("[r:%d b:%d: g:%d] ", pixelProcessor->pixels[i][j].red, pixelProcessor->pixels[i][j].blue, pixelProcessor->pixels[i][j].green);
//        }
//        printf("\n end of row \n");
//        fseek(file, 4, SEEK_CUR);
//    }
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
