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

void turnPixRed(int x, int y) {
    pP->pixels[x * pP->height + y].red = 255;
    pP->pixels[x * pP->height + y].green = 0;
    pP->pixels[x * pP->height + y].blue = 0;
}

void blurImage() {
    int divisions = 4;
    int sectWidth = floor(pP->width/divisions);
    blur_init(pP);
    pP->sections = malloc(sizeof(struct Section) * divisions);

    struct Section *sec = malloc(sizeof(struct Section));
    sec->width = sectWidth;
    pP->sections[0] = *sec;
    blurSection(pP, *sec, pP->height);
    struct Pixel *old = pP->pixels;
    pP->pixels = pP->blurred;
    free(old);

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

//    for (int i = 2; i < pP->height - 1; i+=3) {
//        for (int j = 1; j < pP->width/2 - 1; j+=3) {
//            turnPixRed(i,j);
//        }
//    }

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