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
                if (tV == 1) {
                    turnPixBlue(i, j);
                }
                else if (tH <= 1 && tV == 0) {
                    printf("%d,%d\n", i,j);
                    ++bC;
                    if (bC >= 3) {
                        turnPixBlue(i, j);
                        bC = 0;
                    }
                }
                tH = 0;
            } else if (c) {
                turnPixRed(i,j);
                tH = 0;
            } else if (c2) {
                turnPixRed(i,j);
                tH = -2;
            }
            ++tH;
        }
        if (tV == 2) {
            tV = -1;
        }
        ++tV;
        tH = 0;
        if (i == height + extraV) {
            turnPixBlue(i - 1, sectWidth - 1);
        }
    }


}

void blurImage() {
//    int divisions = 4;
//    int sectWidth = floor(pP->width/divisions);
//    blur_init(pP);
//    pP->sections = malloc(sizeof(struct Section) * divisions);
//
//    struct Section *sec = malloc(sizeof(struct Section));
//    sec->width = sectWidth;
//    pP->sections[0] = *sec;
//    blurSection(pP, *sec, pP->height);

//    int i = 2;
//    int j = 1;
//    int num = i * pP->height + j;
//    turnPixRed(i, j);
//    printf("first pixel num: %d\n", num);
//
//    i = 5;
//    num = i * pP->height + j;
//    printf("second pixel num: %d\n", num);
//    turnPixRed(i, j);
//
//    i = 8;
//    j = 4;
//    num = i * pP->height + j;
//    printf("third pixel num: %d\n", num);
//    turnPixRed(i, j);
//
//    i = 11;
//    num = i * pP->height + j;
//    turnPixRed(i, j);
//
//    i = 14;
//    num = i * pP->height + j;
//    turnPixRed(i, j);


//    printf("%d %d\n", extraH, extraV);

//    for (int i = 0; i <= pP->height; ++i) {
//        for (int j = 0; j < pP->width; ++j) {
//            if (j == pP->width - extraH || i == pP->height - extraV + 1) {
//                turnPixBlue(i, j);
//            } else {
//                struct Pixel *avgPix = getAvgPixel(pP, i, j);
//                int num = i * pP->height + j;
//                pP->blurred[num].red = clampActual(pP->pixels[num].red, avgPix->red);
//                pP->blurred[num].green = clampActual(pP->pixels[num].green, avgPix->green);
//                pP->blurred[num].blue = clampActual(pP->pixels[num].blue, avgPix->blue);
//                free(avgPix);
//            }
//        }
//    }


    blur_init(pP);

    int divisions = 4;
    int sectWidth = floor(pP->width/divisions);
    int start = sectWidth;
    int offset = -1;

    for (int i = 0; i <= 3; ++i) {
        int n = sectWidth * (1+i);
        blurSection(start * i, n, pP->height, offset);
        if (n % 3 == 1) {
            offset = 0;
        } else if (n % 3 == 2) {
            offset = -2;
        } else {
            offset = -1;
        }
    }



//    for (int i = 0; i < pP->height; ++i) {
//        for (int j = 0; j < pP->width; ++j) {
//            blur3x3(pP,i,j);
//        }
//    }


//    for (int i = 0; i < pP->height; ++i) {
//        for (int j = 0; j < pP->width; ++j) {
//            struct Pixel* avgPix = getAvgPixel(pP, i, j);
//            int num = i * pP->height + j;
//            pP->blurred[num].red = clampActual(pP->pixels[num].red, avgPix->red);
//            pP->blurred[num].green = clampActual(pP->pixels[num].green, avgPix->green);
//            pP->blurred[num].blue = clampActual(pP->pixels[num].blue, avgPix->blue);
//            free(avgPix);
//
//        }
//    }

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

void makeYellow() {
    colorShiftPixels(pP, 50, 50, -100);
}

int main(int argc, char* argv[]) {

    bmpP = BmpProcessor_init();

    FILE *img = fopen("../Module-6/test4.bmp", "rb");
    processImage(img, "../../../Desktop/b.bmp");

    BmpProcessor_clean(bmpP);
    PixelProcessor_clean(pP);
    return 0;
}