#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include "BmpProcessor.c"


int validateInt(char* optInt) {
    if (isdigit(optInt)) {
        return 1;
    } else {
        printf("\nPlease enter a valid digit for color shifting.");
        return 0;
    }
}

int validateOutputType(char* type) {
    char bmpType[3] = {'B', 'M', 'P'};
    char ppmType[3] = {'P', 'P', 'M'};
    if (strcmp(bmpType, type) == 0 || strcmp(ppmType, type) == 0) {
        return 1;
    } else {
        printf("\nPlease enter a valid output type. Using BMP as default....");
        return 0;
    }
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

    // call convert stuff here
    BmpProcessor* bmpP = BmpProcessor_init();
    const char fName[25] = "../Module-3/wb.bmp";
    FILE *img = fopen(fName, "rb");


    readBMPHeader(img, bmpP->bmpHeader);

    printf("\nsignature: %s\n", bmpP->bmpHeader->signature);
    printf("size: %d\n", bmpP->bmpHeader->size);
    printf("reserved 1 and 2: %d %d\n", bmpP->bmpHeader->reserved1, bmpP->bmpHeader->reserved2);
    printf("offset pixel array: %d\n", bmpP->bmpHeader->offset_pixel_array);

    fclose(img);

    return 0;
}
