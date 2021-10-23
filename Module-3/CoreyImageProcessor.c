#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
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

int numPlaces (int n) {
    int r = 1;
    if (n < 0) n = (n == INT_MIN) ? INT_MAX: -n;
    while (n > 9) {
        n /= 10;
        r++;
    }
    return r;
}

const char* unsignedLongToCharArray(unsigned long u, char* arr) {
    char color[4];
    int chars = 0;
    int e = (int) u;
    if (e / 100 >= 1) {
        int h = e/100;
        int t = e/10 % 10;
        int o = e % 10;
        char cch = h + '0';
        color[0] = cch;
        cch = t + '0';
        color[1] = cch;
        cch = o + '0';
        color[2] = cch;
        chars = 3;
    } else if (e / 10 >= 1) {
        int t = e/10;
        int o = e % 10;
        char cch = t + '0';
        color[0] = cch;
        cch = o + '0';
        color[1] = cch;
        chars = 2;
    } else {
        int o = e;
        char cch = o + '0';
        color[0] = cch;
        chars = 1;
    }


    char cResized[chars + 1];
    for (int i = 0; i < chars; ++i) {
        cResized[i] = color[i];
    }
    cResized[chars] = '\0';
    strcpy(arr, cResized);
    return cResized;
}

void writeIntToFile(FILE* output, int num) {
    int numP = numPlaces((int) num);
    char arr[numP];
    unsignedLongToCharArray(num, arr);

    for (int i = 0; i < numP; ++i) {
        fwrite(&arr[i], sizeof(char), 1, output);
    }
}
static int p6_rgb(FILE *in, int maxval, unsigned char *red, unsigned char *green, unsigned char *blue)
{
    const float    max = (float)maxval;
    unsigned char  buf[6];


    if (maxval >= 1 && maxval < 256) {
        if (fread(buf, 3, 1, in) != 1)
        return -1; /* Error! */

        if (red)
            *red = buf[0];
        if (green)
            *green = buf[1];
        if (blue)
            *blue = buf[2];
        return 0;
    } else
        return -2; /* Invalid maxval */
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
    PpmProcessor *ppmP = PpmProcessor_init();
    const char fName[30] = "../Module-3/nehoymenoy.ppm";
    FILE *img = fopen(fName, "rb");

    readPPMHeader(img, ppmP->ppmHeader);



    char temp[10] = "";
    fseek(img, 4, SEEK_CUR);

//    fread(&ppmP->ppmHeader->imgWidth, 1, 1, img);
//    fread(&ppmP->ppmHeader->imgHeight, 1, 1, img);

//    fread(&temp, sizeof(char), 5, img);
    int width = ppmP->ppmHeader->imgWidth;
    int height =ppmP->ppmHeader->imgHeight;
    printf("%d, %d\n", width, height);


    PixelProcessor *pP = PixelProcessor_init(width, height);





    int c = 0;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
//            int ch;
//            int r = 0;
//            int g = 0;
//            int b = 0;
//            ch = fgetc(img);
//
//            if (c == 3) {
//                c = 0;
//            }
//            if (c == 0) {
//                r = ch;
//            } else if (c == 1) {
//                g = ch;
//            } else if (c == 2) {
//                b = ch;
//            }
//            ++c;
        if (p6_rgb(img, 255, &pP->pixels[i * height + j].red, &pP->pixels[i * height + j].green, &pP->pixels[i * height + j].blue) == 0) {
        }

//            pP->pixels[i * height + j].red = r;
//            pP->pixels[i * height + j].green = g;
//            pP->pixels[i * height + j].blue = b;

        }
//        if (paddingBytes > 0) {
//            fseek(file, paddingBytes, SEEK_CUR);
//        }
    }










    fclose(img);
    colorShiftPixels(pP, 56, 78, 45);
    FILE *output = fopen("../Module-3/output.ppm", "wb");


    fwrite(&ppmP->ppmHeader->id, sizeof(char) * 2, 1, output);
    fwrite("\n", sizeof(char), 1, output);
    writeIntToFile(output, ppmP->ppmHeader->imgWidth);
    fwrite("\n", sizeof(char), 1, output);
    writeIntToFile(output, ppmP->ppmHeader->imgHeight);
    fwrite("\n", sizeof(char), 1, output);
    writeIntToFile(output, ppmP->ppmHeader->maxColorValue);
    fwrite("\n", sizeof(char), 1, output);


    for (int i = 0; i < pP->height; ++i) {
        for (int j = 0; j < pP->width; ++j) {

            fwrite(&pP->pixels[i*pP->height+j].red, 1, 1, output);

            fwrite(&pP->pixels[i*pP->height+j].green, 1, 1, output);

            fwrite(&pP->pixels[i*pP->height+j].blue, 1, 1, output);

        }
//        if (pad > 0) {
//            fwrite(&p, sizeof(char) * pad, 1, file);
//        }
    }








//    fclose(output);





    BmpProcessor *bmpP = BmpProcessor_init();
//    const char fName[25] = "../Module-3/wb.bmp";
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

//    printf("%d\n", pP->width * pP->height * 4);



//    writeBMPHeader(output,bmpP->bmpHeader);
//    writeDIBHeader(output,bmpP->dibHeader);
//    writePixelsBMP(output,pP,pad);

    BmpProcessor_clean(bmpP);

    PpmProcessor_clean(ppmP);
    PixelProcessor_clean(pP);

//    fclose(output);
    return 0;
}




