#include <PixelProcessor.h>
#include <malloc.h>

struct PixelProcessor* PixelProcessor_init(int imgWidth, int imgHeight) {
    PixelProcessor* pixelProcessor = malloc(sizeof(PixelProcessor));
    pixelProcessor->pixels = calloc(imgHeight * imgWidth, sizeof(struct Pixel));
    pixelProcessor->width = imgWidth;
    pixelProcessor->height = imgHeight;
    return pixelProcessor;
}

void PixelProcessor_clean(PixelProcessor *self) {
    free(self->pixels);
    free(self);
}

/**
 * read Pixels from BMP file based on width and height.
 *
 * @param  file: A pointer to the file being read or written
 */
void readPixelsBMP(FILE* file, struct PixelProcessor *self, int paddingBytes) {
    for (int i = 0; i < self->height; ++i) {
        for (int j = 0; j < self->width; ++j) {
            unsigned char b, g, r;
            fread(&b, sizeof(char), 1, file);
            fread(&g, sizeof(char), 1, file);
            fread(&r, sizeof(char), 1, file);
            self->pixels[i * self->height + j].red = r;
            self->pixels[i * self->height + j].green = g;
            self->pixels[i * self->height + j].blue = b;
        }
        if (paddingBytes > 0) {
            fseek(file, paddingBytes, SEEK_CUR);
        }
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
    void colorShiftPixels(struct Pixel **pArr, int width, int height, int rShift, int gShift, int bShift) {

    }
}
