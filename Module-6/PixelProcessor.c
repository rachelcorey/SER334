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

    pixelProcessor->padding = 0;
    return pixelProcessor;
}

void blur_init(PixelProcessor *pP) {
    pP->blurred = calloc(pP->height * pP->width * 4, sizeof(struct Pixel));
    for (int i = 1; i <= pP->height; ++i) {
        for (int j = 0; j < pP->width; ++j) {
            int num = i * pP->height + j;
            pP->blurred[num].w = 0;
        }
    }
}


/**
 *
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


unsigned char clampActual(unsigned char color, int actual) {
    if (actual > 255) {
        color = 255;
    } else if (actual < 0) {
        color = 0;
    } else {
        color = actual;
    }
    return color;
}

struct Pixel* getAvgPixel(PixelProcessor *pP, int sectWidth, int x, int y) {
    struct Pixel* avgPix = calloc(1, sizeof(struct Pixel));
    int rTotal = 0;
    int gTotal = 0;
    int bTotal = 0;
    int pixelsCounted = 0;
    int xAmt = 2;
    int yAmt = 2;

    for (int i = x; i < x + yAmt; ++i) {
        for (int j = y; j < y + xAmt; ++j) {
                rTotal += pP->pixels[i * pP->height + j].red;
                gTotal += pP->pixels[i * pP->height + j].green;
                bTotal += pP->pixels[i * pP->height + j].blue;
                ++pixelsCounted;
            if (i == x + yAmt && j == y + xAmt && pixelsCounted < 9) {
                rTotal = 0;
                gTotal = 0;
                bTotal = 0;
            }
        }
    }


    avgPix->red = rTotal/pixelsCounted;
    avgPix->green = gTotal/pixelsCounted;
    avgPix->blue = bTotal/pixelsCounted;

    return avgPix;
}


void blur3x3(PixelProcessor *pP, int sectWidth, int x, int y) {
    int curX = x - 1;
    int curY = y - 1;

    for (int i = curX; i < curX + 3; ++i) {
        for (int j = curY; j < curY + 3; ++j) {
            struct Pixel* avgPix = getAvgPixel(pP, sectWidth, i, j);
            int num = i * pP->height + j;
            pP->blurred[num].red = clampActual(pP->pixels[num].red, avgPix->red);
            pP->blurred[num].green = clampActual(pP->pixels[num].green, avgPix->green);
            pP->blurred[num].blue = clampActual(pP->pixels[num].blue, avgPix->blue);

            free(avgPix);
        }
    }

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

    for (int i = self->width; i > 0; --i) {
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


struct Pixel *getAvgPixel_edge(PixelProcessor *pP, int sectWidth, int x, int y) {
    struct Pixel *avgPix = calloc(1, sizeof(struct Pixel));
    avgPix->red = 0;
    avgPix->green = 0;
    avgPix->blue = 0;
    int rTotal = 1;
    int gTotal = 1;
    int bTotal = 1;
    int pixelsCounted = 0;
    int xAmt = 2;
    int yAmt = 2;

    for (int i = x; i < x + yAmt; ++i) {
        for (int j = y; j < y + xAmt; ++j) {
            if (j == sectWidth - 1 || i == pP->height) {
                rTotal += pP->pixels[i * pP->height + j].red;
                gTotal += pP->pixels[i * pP->height + j].green;
                bTotal += pP->pixels[i * pP->height + j].blue;
                ++pixelsCounted;
            }
        }
    }

    if (pixelsCounted > 0) {
        avgPix->red = rTotal / pixelsCounted;
        avgPix->green = gTotal / pixelsCounted;
        avgPix->blue = bTotal / pixelsCounted;
    }

    return avgPix;
}

void blur3x3_edge(PixelProcessor *pP, int sectWidth, int x, int y, int n, int m) {
    int curX = x;
    int curY = y;

    for (int i = curX; i < curX + n; ++i) {
        for (int j = curY; j < curY + m; ++j) {
            struct Pixel *avgPix = getAvgPixel_edge(pP, sectWidth, i, j);
            int num = i * pP->height + j;
            pP->blurred[num].red = clampActual(pP->pixels[num].red, avgPix->red);
            pP->blurred[num].green = clampActual(pP->pixels[num].green, avgPix->green);
            pP->blurred[num].blue = clampActual(pP->pixels[num].blue, avgPix->blue);
            free(avgPix);

        }
    }
}

struct Circle *circles_init(PixelProcessor *pP, int num) {
    struct Circle *circles = calloc(num, sizeof(struct Circle));
    srand(time(NULL));
    int centerX = rand() % pP->width;
    int centerY = rand() % pP->height;


    int radius = rand() % num;

    for (int i = 0; i < num; ++i) {
        circles[i].cX = centerX;
        circles[i].cY = centerY;
        circles[i].radius = radius;
        if (i % 2 == 0) {
            radius = num;
        } else {
            radius = rand() % num + (rand() % (int) floor(num));
        }
        centerX = rand() % pP->width;
        centerY = rand() % pP->height;
    }

    return circles;
}

static void *cheeseSection(void *arguments) {
    struct SectionArgs *argz = arguments;
    PixelProcessor *pP = argz->pP;
    pthread_t threadNum = argz->threadNum;
    int height = argz->height;
    int start = argz->start;
    int sectWidth = argz->sectWidth;
    struct Circle *circles = argz->circles;

    int extraV = height % 3;

    int c = 0;
    int holes = 0;

    if (pP->width > pP->height) {
        holes = floor(pP->height / 10);
    } else {
        holes = floor(pP->width / 10);
    }

    while (c < holes) {
        for (int i = 1; i <= height + extraV; ++i) {
            for (int j = start; j < sectWidth; ++j) {
                int num = i * pP->height + j;
                if (((i - circles[c].cX) * (i - circles[c].cX) + (j - circles[c].cY) * (j - circles[c].cY)) <=
                    circles[c].radius * circles[c].radius) {
                    pP->blurred[num].red = 0;
                    pP->blurred[num].green = 0;
                    pP->blurred[num].blue = 0;
                    pP->blurred[num].w = 1;
                } else {
                    if (pP->blurred[num].w == 0) {
                        pP->blurred[num].red = clamp(pP->pixels[num].red, 50);
                        pP->blurred[num].green = clamp(pP->pixels[num].green, 50);
                        pP->blurred[num].blue = clamp(pP->pixels[num].blue, -100);
                        pP->blurred[num].w = 1;
                    }
                }
            }
        }
        ++c;
    }
    pthread_exit(&threadNum);
}

static void *blurSection(void *arguments) {
    struct SectionArgs *argz = arguments;
    PixelProcessor *pP = argz->pP;
    pthread_t threadNum = argz->threadNum;
    int height = argz->height;
    int start = argz->start;
    int sectWidth = argz->sectWidth;
    int offset = argz->offset;
    int extraH = sectWidth % 3;
    int extraV = height % 3;

    int tH = 0;
    int tV = 0;
    int bC = 1;

    for (int i = 1; i <= height + extraV; ++i) {
        for (int j = start; j < sectWidth; ++j) {
            int c = (tH == 4 && tV == 1);
            int c2 = (tH == 1 && tV == 1);
            if ((j > sectWidth - extraH + offset || i >= height - extraV + 1)) {
                if (j == pP->width - 1) {
                    blur3x3_edge(pP, sectWidth, i, j, 2, 1);
                } else if (tH <= 1 && tV == 0) {
                    ++bC;
                    if (bC >= 3) {
                        blur3x3_edge(pP, sectWidth, i, j - 3, 1, 5);
                        bC = 0;
                    }
                } else if (tV <= 1 && tH > 1) {
                    blur3x3(pP, sectWidth, i + 1, j);
                }
                tH = 0;
            } else if (c) {
                blur3x3(pP, sectWidth, i, j);
                tH = 0;
            } else if (c2) {
                blur3x3(pP, sectWidth, i, j);
                tH = -2;
            }
            ++tH;
        }
        if (tV == 2) {
            tV = -1;
        }
        ++tV;
        tH = 0;
    }
    pthread_exit(&threadNum);
}

