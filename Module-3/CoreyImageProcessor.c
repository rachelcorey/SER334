#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <sys/stat.h>
#include "BmpProcessor.c"
#include "PixelProcessor.c"
#include "PpmProcessor.c"


int validateInt(char *optInt) {
    if (isdigit(optInt)) {
        return 1;
    } else {
        printf("\nPlease enter a valid digit for color shifting.");
        return 0;
    }
}

int validateOutputType(char *type) {
    char bmpType[3] = {'B', 'M', 'P'};
    char ppmType[3] = {'P', 'P', 'M'};
    if (strcmp(bmpType, type) == 0 || strcmp(ppmType, type) == 0) {
        return 1;
    } else {
        printf("\nPlease enter a valid output type. Using BMP as default....");
        return 0;
    }
}

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








int main(int argc, char *argv[]) {
    char *imgFile;
    char *ptr;
//    long redShift = 0;
//    long blueShift = 0;
//    long greenShift = 0;
//    char outputType[3] = "";
//
//    int opt;
//    while ((opt = getopt(argc, argv, ":o:r:g:b:t:")) != -1) {
//        switch (opt) {
//            case 'o':
//                imgFile = optarg;
//                break;
//            case 'r':
//                if (validateInt(optarg)) {
//                    redShift = strtol(optarg, &ptr, 10);
//                }
//                break;
//            case 'g':
//                if (validateInt(optarg)) {
//                    greenShift = strtol(optarg, &ptr, 10);
//                }
//                break;
//            case 'b':
//                if (validateInt(optarg)) {
//                    blueShift = strtol(optarg, &ptr, 10);
//                }
//                break;
//            case 't':
//                if (validateOutputType(optarg)) {
//                    strcpy(outputType, optarg);
//                }
//                break;
//            case ':':
//                printf("\noption needs a value");
//                break;
//            case '?':
//                printf("\nunknown option: %c", optopt);
//                break;
//        }
//    }
//
//    int dflen;
//    if (imgFile != NULL) {
//        dflen = strlen(imgFile);
//        if (dflen >= 5
//            && ((strcmp(&imgFile[dflen - 4], ".bmp") == 0) || (strcmp(&imgFile[dflen - 4], ".ppm") == 0))
//            && (access(imgFile, F_OK) != -1)) {
//            printf("\nImporting data from %s\n", imgFile);
////            readFile(imgFile);
//        } else {
//            printf("\nImage file has an invalid name or does not exist.\n");
//            exit(1);
//        }
//    } else {
//        printf("\nNo image file name provided. This is a required field.\n");
//        exit(1);
//    }

// *******
//     call convert PPM stuff here
//    PpmProcessor *ppmP = PpmProcessor_init();
//    const char fName[30] = "../Module-3/nehoymenoy.ppm";
//    FILE *img = fopen(fName, "rb");
//
//    readPPMHeader(img, ppmP->ppmHeader);
//    fseek(img, 4, SEEK_CUR);
//
//    int width = ppmP->ppmHeader->imgWidth;
//    int height =ppmP->ppmHeader->imgHeight;
//
//    PixelProcessor *pP = PixelProcessor_init(width, height);
//    readPixelsPPM(img, pP, width, height);
//    fclose(img);
//    colorShiftPixels(pP, 56, 78, 45);
//    FILE *output = fopen("../Module-3/output.ppm", "wb");
//
//    writePPMHeader(output,ppmP->ppmHeader);
//
//    writePixelsPPM(output,pP);
//    fclose(output);



// *******
//     PPM to BMP stuff here
    PpmProcessor *ppmP = PpmProcessor_init();
    const char fName[30] = "../Module-3/nehoymenoy.ppm";
    FILE *img = fopen(fName, "rb");

    readPPMHeader(img, ppmP->ppmHeader);
    fseek(img, 4, SEEK_CUR);

    int width = ppmP->ppmHeader->imgWidth;
    int height =ppmP->ppmHeader->imgHeight;

    int dest = fileno(img);
    struct stat data;
    fstat(dest, &data);
    int size = data.st_size;
    BmpProcessor *bmP = BmpProcessor_init();
    makeBMPHeader(bmP->bmpHeader, width, height);
    bmP->bmpHeader->size = size;
    makeDIBHeader(bmP->dibHeader, width, height);
    bmP->dibHeader->imgSize = size;

    PixelProcessor *pP = PixelProcessor_init(width, height);
    readPixelsPPM(img, pP, width, height);
    fclose(img);
    FILE *output = fopen("../Module-3/output.bmp", "wb");

    writeBMPHeader(output, bmP->bmpHeader);
    writeDIBHeader(output,bmP->dibHeader);

    int pad = calculatePadding(pP->width);
    writePixelsBMP(output,pP, pad);

//    fclose(output);


// ******
    // call convert BMP stuff here
//    BmpProcessor *bmpP = BmpProcessor_init();
//    const char fName[25] = "../Module-3/ttt.bmp";
//    FILE *img = fopen(fName, "rb");
//
//    char h[45] = "";
//
//    readBMPHeader(img, bmpP->bmpHeader);
//    readDIBHeader(img, bmpP->dibHeader);
//
//    PixelProcessor *pP = PixelProcessor_init(bmpP->dibHeader->imgWidth, bmpP->dibHeader->imgHeight);
//    int pad = calculatePadding(pP->width);
//    readPixelsBMP(img, pP, pad);
//
//    fclose(img);
//
//    FILE *output = fopen("../Module-3/output.bmp", "wb");
//
//    writeBMPHeader(output,bmpP->bmpHeader);
//    writeDIBHeader(output,bmpP->dibHeader);
//    writePixelsBMP(output,pP,pad);
//
//    BmpProcessor_clean(bmpP);
//
////    PpmProcessor_clean(ppmP);
//    PixelProcessor_clean(pP);

// *****
// bmp to ppm here
//    BmpProcessor *bmpP = BmpProcessor_init();
//    const char fName[25] = "../Module-3/ttt.bmp";
//    FILE *img = fopen(fName, "rb");
//
//    char h[45] = "";
//
//    readBMPHeader(img, bmpP->bmpHeader);
//    readDIBHeader(img, bmpP->dibHeader);
//
//    int width = bmpP->dibHeader->imgWidth;
//    int height = bmpP->dibHeader->imgHeight;
//    PixelProcessor *pP = PixelProcessor_init(width, height);
//
//    int pad = calculatePadding(pP->width);
//    readPixelsBMP(img, pP, pad);
//
//    fclose(img);
//
//
//    PpmProcessor *ppmP = PpmProcessor_init();
//    FILE *output = fopen("../Module-3/output.ppm", "wb");
//
//    makePPMHeader(ppmP->ppmHeader, width, height);
//    writePPMHeader(output,ppmP->ppmHeader);
//
//    writePixelsPPM(output,pP);
//    fclose(output);
//
//
//    BmpProcessor_clean(bmpP);
//
////    PpmProcessor_clean(ppmP);
//    PixelProcessor_clean(pP);

//    if (output != NULL) {
//        fclose(output);
//    }
    return 0;
}




