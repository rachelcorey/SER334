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
#include <pthread.h>
#include <getopt.h>

//TODO: finish me

#include "BmpProcessor.c"
#include "PixelProcessor.c"

////////////////////////////////////////////////////////////////////////////////
//MACRO DEFINITIONS

//problem assumptions
#define MAXIMUM_IMAGE_SIZE 4096
#define NUM_DIVISIONS 4

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



void processImage(char filter) {
    blur_init(pP);

    int divisions = NUM_DIVISIONS;
    int sectWidth = floor(pP->width / divisions);
    int start = sectWidth;
    int offset = -1;
    struct Circle *circles = NULL;

    int s;
    void *res;

    if (filter == 'c') {
        int holes = 0;
        if (pP->width > pP->height) {
            holes = floor(pP->height / 10);
        } else {
            holes = floor(pP->width / 10);
        }
        circles = circles_init(pP, holes);
    }

    struct SectionArgs *args = calloc(divisions, sizeof(*args));
    args->pP = pP;
    if (args == NULL) {
        printf("FATAL ERROR: Failed to create thread arguments.\n");
        printf("Terminating program.....\n");
        exit(1);
    }

    for (int i = 0; i <= divisions - 1; ++i) {
        int n = sectWidth * (1 + i);

        args[i].threadNum = i + 1;
        args[i].start = start * i;
        args[i].sectWidth = n;
        args[i].height = pP->height;
        args[i].offset = offset;

        if (filter == 'c') {
            args[i].circles = circles;
            s = pthread_create(&args[i].threadNum, NULL, &cheeseSection, &args[i]);
        } else if (filter == 'b') {
            s = pthread_create(&args[i].threadNum, NULL, &blurSection, &args[i]);
        }

        if (s != 0) {
            printf("FATAL ERROR: Thread creation failed.\n");
            printf("Terminating program.....\n");
            exit(1);
        }

        if (n % 3 == 1) {
            offset = 0;
        } else if (n % 3 == 2) {
            offset = -2;
        } else {
            offset = -1;
        }
    }

    for (int tnum = 0; tnum < divisions; tnum++) {
        s = pthread_join(args[tnum].threadNum, &res);
        if (s != 0) {
            printf("FATAL ERROR: Thread join failed.\n");
            printf("Terminating program.....\n");
            exit(1);
        }
    }

    free(args->circles);
    free(args);

    struct Pixel *old = pP->pixels;
    pP->pixels = pP->blurred;
    free(old);
}

void processFile(FILE *img, char *outputFile, char filter) {
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

    processImage(filter);

    FILE *output = fopen(outputFile, "wb");

    writeBMPHeader(output, bmpP->bmpHeader);
    writeDIBHeader(output, bmpP->dibHeader);
    writePixelsBMP(output, pP, pad);

    fclose(output);
}

// TODO: revise this
/**
 * Explains the usage of the program, to be called when no arguments are entered when the program is run
 */
void explainUsage() {
    printf("ERROR: Please enter some arguments to use the program.\n");
    printf("====================== USAGE: =====================\n");
    printf("================= For conversion: =================\n");
    printf("./<a.out name> <inputFileName>.<bmp or ppm> -o <outputFileName> -t <bmp or ppm>\n");
    printf("EXAMPLE: ./imageProcessor ../path/to/inputFile.bmp -o ../path/to/outputFile -t ppm\n");
    printf("================ For color shifting: ==============\n");
    printf("After the input file name/location, input - followed by the first letter of the color, followed by the amount of shifting for each color as an"
           " integer from -255 to 255.\n");
    printf("./<a.out name> <inputFileName>.<bmp or ppm> -r <int red> -g <int green> -b <int blue>\n");
    printf("EXAMPLE: ./imageProcessor ../path/to/inputFile.bmp -r 69 g -4 b 20\n");
    printf("OR:\n");
    printf("EXAMPLE: ./imageProcessor ../path/to/inputFile.bmp -r 69 g -4 b 20 -o ../path/to/outputFile\n");
    printf("=================================================\n");
}

/**
 * Helper method for setting a file extension type for a given output file string
 * @param outputFile the string of the output file
 * @param type the type of the file it should be output to
 */
void setFileExt(char *outputFile, char *type) {
    strcat(outputFile, "-2.");
    strcat(outputFile, type);
}

/**
 * Validates if a given file name has a valid extension on the end of it
 * @param isInput if the file is given as an input for the program or not
 * @param fileName the name of the file to check
 * @return 1 if the extension string is valid, 0 if not
 */
int validateFileType(int isInput, char *fileName) {
    int isBMP = 0;
    int fileLength = strlen(fileName);

    if (fileLength > 4) {
        if ((strcmp(&fileName[fileLength - 4], ".bmp") == 0)) {
            if (isInput != 1) {
                setFileExt(fileName, "bmp");
            }
            isBMP = 1;
        } else {
            // file name is present, but lacking extension. warning will show.
            isBMP = 2;
        }
    } else if (fileLength >= 1) {
        // file name is present, but lacking extension. warning will show.
        isBMP = 2;
    } else {
        if (isInput) {
            printf("FATAL ERROR: Input file type invalid!\n");
            printf("Valid file extension is .bmp!\n");
            printf("Terminating program.....\n");
            exit(1);
        }
    }
    return isBMP;
}

int main(int argc, char *argv[]) {

    char inputFile[40];
    strcpy(inputFile, "empty");
    char outputFile[40];
    strcpy(outputFile, "empty");
    bmpP = BmpProcessor_init();
    int opt;
    char filter;

    while ((opt = getopt(argc, argv, ":i:o:c:b:")) != -1) {
        switch (opt) {
            case 'i':
                if (optarg != NULL) {
                    strcpy(inputFile, optarg);
                }
                break;
            case 'o':
                if (optarg != NULL) {
                    if (strlen(optarg) > 0) {
                        strcpy(outputFile, optarg);
                    }
                }
                break;
            case 'c':
                filter = 'c';
                break;
            case 'b':
                filter = 'b';
                break;
            case ':':
                filter = 'n';
                break;
            default:
                break;
        }
    }

    if (strcmp(inputFile, "empty") != 0) {
        if (strcmp(outputFile, "empty") == 1 || strcmp(outputFile, ".bmp") == 0 || validateFileType(0, outputFile) == 0) {
            printf("Output file name or type was invalid or not specified.\n");
            printf("Program will output as %s-2.bmp......\n", inputFile);
            strcat(outputFile, "-2.bmp");
        }

        FILE *img = fopen(inputFile, "rb");

        if (img != NULL && validateFileType(1, inputFile) != 0) {
            if (filter == 'c' || filter == 'b') {
                processFile(img, outputFile, filter);
            } else {
                printf("FATAL ERROR: Filter type was invalid or unspecified.\n");
                printf("Terminating program........\n");
                exit(1);
            }
        } else {
            printf("FATAL ERROR: Input file '%s' was not found in the system or is an invalid type.\n", inputFile);
            printf("Terminating program........\n");
            exit(1);
        }
    } else {
        explainUsage();
        exit(1);
    }

//    FILE *img = fopen("../Module-6/test4.bmp", "rb");
//    processFile(img, "../../../Desktop/b.bmp");

    BmpProcessor_clean(bmpP);
    PixelProcessor_clean(pP);
    return 0;
}