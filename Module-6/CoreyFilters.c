/**
* (basic description of the program or class)
*
* Completion time: (estimation of hours spent on this program)
*
* @author (your name), (anyone else, e.g., Acuna, whose code you used)
* @version (a version number or a date)
*/

////////////////////////////////////////////////////////////////////////////////
//INCLUDES
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

//TODO: finish me

#include "BmpProcessor.c"
#include "PixelProcessor.c"

////////////////////////////////////////////////////////////////////////////////
//MACRO DEFINITIONS

//problem assumptions
#define MAXIMUM_IMAGE_SIZE 4096
#define DIV_CONSTANT 4

////////////////////////////////////////////////////////////////////////////////
//DATA STRUCTURES

BmpProcessor *bmpP;
PixelProcessor *pP;

////////////////////////////////////////////////////////////////////////////////
//MAIN PROGRAM CODE

int calculatePadding(int imgWidth) {
    int pad = 0;
    int rowBytes = imgWidth * sizeof(struct Pixel);
    int rowBytes2 = rowBytes;

    do {
        int rem = rowBytes % 4;
        if (rem != 0) {
            rowBytes += 1;
        }
        pad = rowBytes - rowBytes2;
    } while ((rowBytes % 4) != 0);
    return pad;
}


void makeYellow() {
    colorShiftPixels(pP, 50, 50, -100);
}
void turnPixBlue(int x, int y) {
    pP->blurred[x * pP->height + y].red = 0;
    pP->blurred[x * pP->height + y].green = 0;
    pP->blurred[x * pP->height + y].blue = 255;
}

void turnPixRed(int x, int y) {
    pP->blurred[x * pP->height + y].red = 255;
    pP->blurred[x * pP->height + y].green = 0;
    pP->blurred[x * pP->height + y].blue = 0;
}


struct Pixel* getAvgPixel_2(PixelProcessor *pP2, int sectWidth, int x, int y) {
    struct Pixel* avgPix = calloc(1, sizeof(struct Pixel));
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

void blur3x3_2(PixelProcessor *pP, int sectWidth, int x, int y, int n, int m) {
    int curX = x;
    int curY = y;

    for (int i = curX; i < curX + n; ++i) {
        for (int j = curY; j < curY + m; ++j) {
            struct Pixel* avgPix = getAvgPixel_2(pP, sectWidth, i, j);
            int num = i * pP->height + j;
            pP->blurred[num].red = clampActual(pP->pixels[num].red, avgPix->red);
            pP->blurred[num].green = clampActual(pP->pixels[num].green, avgPix->green);
            pP->blurred[num].blue = clampActual(pP->pixels[num].blue, avgPix->blue);
            free(avgPix);

        }
    }
}


void renderSection(int start, int sectWidth, int height, int offset) {
    int extraH = sectWidth % 3;
    int extraV = height % 3;
    srand(time(NULL));
    int centerX = rand() % pP->width;
    int centerY = rand() % pP->height;
    int holes = 0;
    int c = 0;

    if (pP->width > pP->height) {
        holes = floor(pP->height/10);
    } else {
        holes = floor(pP->width/10);
    }

    int radius = rand() % holes;
//    int circle = pow((centerX-100), 2) + pow((centerY-100), 2) - pow(radius, 2);

    while (c < holes) {
        for (int i = 1; i <= height + extraV; ++i) {
            for (int j = start; j < sectWidth; ++j) {
                int num = i * pP->height + j;

                if (((i - centerX) * (i - centerX) + (j - centerY) * (j - centerY)) <= radius * radius) {
                    if ((j > sectWidth - extraH + offset || i >= height - extraV + 1)) {
//                        turnPixBlue(i, j);
                    } else {
//                        turnPixRed(i, j);
                    }

                    pP->blurred[num].red = 0;
                    pP->blurred[num].green = 0;
                    pP->blurred[num].blue = 0;
                } else {
                    pP->blurred[num].red = clamp(pP->pixels[num].red, 50);
                    pP->blurred[num].green = clamp(pP->pixels[num].green, 50);
                    pP->blurred[num].blue = clamp(pP->pixels[num].blue, -100);
                }
            }
        }
        ++c;
        if (c % 2 == 0 || c % 3 == 0) {
            radius = holes;
        } else {
            radius = rand() % holes + (rand() % (int)floor(holes));
        }
        centerX = rand() % pP->width;
        centerY = rand() % pP->height;
    }


}

void blurSection(int start, int sectWidth, int height, int offset) {
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
                    blur3x3_2(pP, sectWidth, i, j, 2, 1);
                } else if (tH <= 1 && tV == 0) {
                    ++bC;
                    if (bC >= 3) {
                        blur3x3_2(pP, sectWidth, i, j-3, 1, 5);
                        bC = 0;
                    }
                } else if (tV <= 1 && tH > 1) {
                    blur3x3(pP, sectWidth, i+1, j);
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
}

void blurImage() {
    blur_init(pP);

    int divisions = 4;
    int sectWidth = floor(pP->width/divisions);
    int start = sectWidth;
    int offset = -1;


    for (int i = 0; i <= divisions - 1; ++i) {
        int n = sectWidth * (1+i);
//        blurSection(start * i, n, pP->height, offset);
        renderSection(start * i, n, pP->height, offset);
        if (n % 3 == 1) {
            offset = 0;
        } else if (n % 3 == 2) {
            offset = -2;
        } else {
            offset = -1;
        }
    }

    struct Pixel *old = pP->pixels;
    pP->pixels = pP->blurred;
    free(old);

}

void processImage(FILE *img, char *outputFile) {
    int pad = 0;
    int width = 0;
    int height = 0;

    readBMPHeader(img, bmpP->bmpHeader);
    readDIBHeader(img, bmpP->dibHeader);
    width = bmpP->dibHeader->imgWidth;
    height = bmpP->dibHeader->imgHeight;
    pP = PixelProcessor_init(width, height);
    pad = calculatePadding(pP->width);
    readPixelsBMP(img, pP, pad);
    fclose(img);

    blurImage();
    FILE *output = fopen(outputFile, "wb");

    writeBMPHeader(output, bmpP->bmpHeader);
    writeDIBHeader(output, bmpP->dibHeader);
    writePixelsBMP(output, pP, pad);

    fclose(output);
}



int main(int argc, char* argv[]) {

    bmpP = BmpProcessor_init();

    FILE *img = fopen("../Module-6/test4.bmp", "rb");
    processImage(img, "../../../Desktop/b.bmp");

    BmpProcessor_clean(bmpP);
    PixelProcessor_clean(pP);
    return 0;
}