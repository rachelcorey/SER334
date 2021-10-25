#include <PixelProcessor.h>
#include <malloc.h>

/**
 * Initializes a PixelProcessor Object
 * @param imgWidth the width of the image to process
 * @param imgHeight the height of the image to process
 */
PixelProcessor *PixelProcessor_init(int imgWidth, int imgHeight) {
    PixelProcessor *pixelProcessor = malloc(sizeof(PixelProcessor));
    pixelProcessor->pixels = calloc(imgHeight * imgWidth * 4, sizeof(struct Pixel));
    pixelProcessor->width = imgWidth;
    pixelProcessor->height = imgHeight;
    return pixelProcessor;
}

/**
 * Cleans a PixelProcessor Object from the system
 * @param self object to be freed
 */
void PixelProcessor_clean(PixelProcessor *self) {
    free(self->pixels);
    free(self);
}

/**
 * Read pixels from a BMP file
 * @param file BMP file to be read
 * @param self reference to this PixelProcessor object to store the data in
 * @param paddingBytes amount of padding to read from the BMP
 */
void readPixelsBMP(FILE *file, struct PixelProcessor *self, int paddingBytes) {

    for (int i = self->width - 1; i > 0; --i) {
        for (int j = 0; j < self->height; ++j) {
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
}

/**
 * Clamps values entered for color shifting that are above 255 and below 0
 * @param color initial color value
 * @param mod the requested modification
 * @return the clamped modification of the color
 */
unsigned char clamp(unsigned char color, int mod) {
    if (color + mod < 0) {
        color = 0;
    } else if (color + mod > 255) {
        color = 255;
    } else {
        color = color + mod;
    }
    return color;
}

/**
 * Shift color of Pixel array. The shift value of RGB is
 * rShift, gShift，bShift. Useful for color shift.
 *
 * @param  pP: the reference to this PixelProcessor object where the pixel array is stored
 * @param  rShift: the shift value of color r shift
 * @param  gShift: the shift value of color g shift
 * @param  bShift: the shift value of color b shift
 */
void colorShiftPixels(struct PixelProcessor *pP, int rShift, int gShift, int bShift) {
    for (int i = 0; i < pP->height; ++i) {
        for (int j = 0; j < pP->width; ++j) {
            pP->pixels[i * pP->height + j].red = clamp(pP->pixels[i * pP->height + j].red, rShift);
            pP->pixels[i * pP->height + j].green = clamp(pP->pixels[i * pP->height + j].green, gShift);
            pP->pixels[i * pP->height + j].blue = clamp(pP->pixels[i * pP->height + j].red, bShift);
        }
    }
}

/**
 * Write pixels to a BMP file
 * @param file file to be written to
 * @param pP a reference to this PixelProcessor object
 * @param pad the amount of padding to write
 */
void writePixelsBMP(FILE *file, PixelProcessor *pP, int pad) {
    char *p[pad];
    for (int i = 0; i < pad; ++i) {
        p[i] = "0";
    }
    for (int i = pP->width; i > 0; --i) {
        for (int j = 0; j <= pP->height; ++j) {
            fwrite(&pP->pixels[i * pP->height + j].blue, sizeof(unsigned char), 1, file);
            fwrite(&pP->pixels[i * pP->height + j].green, sizeof(unsigned char), 1, file);
            fwrite(&pP->pixels[i * pP->height + j].red, sizeof(unsigned char), 1, file);
        }
        fseek(file, -3, SEEK_CUR);
        if (pad > 0) {
            fwrite(&p, sizeof(char) * pad, 1, file);
        }
    }
}

