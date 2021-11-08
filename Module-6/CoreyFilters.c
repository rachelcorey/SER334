/**
*
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
#include "BmpProcessor.c"
#include "PixelProcessor.c"

////////////////////////////////////////////////////////////////////////////////
//MACRO DEFINITIONS

//problem assumptions
#define MAXIMUM_IMAGE_SIZE 4096
#define THREAD_COUNT 4

////////////////////////////////////////////////////////////////////////////////
//DATA STRUCTURES

BmpProcessor *bmpP;
PixelProcessor *pP;
SectionArgs *args;

////////////////////////////////////////////////////////////////////////////////
//MAIN PROGRAM CODE

/**
 * Calculate the padding needed to read/write a BMP file
 * @param imgWidth width of the image
 * @return the amount of padding needed
 */
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

/**
 * Starts a thread and renders a section of the image based on the chosen filter
 * @param i the thread iteration
 * @param n the section width
 * @param filter the chosen image filter
 * @param divisions the number of divisions
 * @param sectWidth the section width
 * @param start the starting position to render at
 * @param offset the amount of offset to apply
 * @param circles the array of circles to generate holes
 */
void
handleSection(int i, int n, char filter, int divisions, int sectWidth, int start, int offset, struct Circle *circles) {
    int s;

    args[i].pP = pP;
    args[i].threadNum = i + 1;
    args[i].start = start * i;
    args[i].sectWidth = n;
    args[i].height = pP->height;
    args[i].offset = offset;
    args[i].circles = circles;

    if (i == divisions - 1) {
        args[i].last = 1;
    } else {
        args[i].last = 0;
    }

    if (filter == 'c') {
        s = pthread_create(&args[i].threadNum, NULL, &cheeseSection, &args[i]);
    } else if (filter == 'b') {
        s = pthread_create(&args[i].threadNum, NULL, &blurSection, &args[i]);
    }

    if (s != 0) {
        printf("FATAL ERROR: Thread creation failed.\n");
        printf("Terminating program.....\n");
        exit(1);
    }
}

/**
 * A function to loop through the chosen number of threads and call the handleSection function for the specified
 * parameters.
 * @param filter chosen image filter
 */
void processImage(char filter) {
    newPixels_init(pP);

    int divisions = THREAD_COUNT;
    int sectWidth = ceil(pP->width / divisions);
    int start = sectWidth;
    int offset = -1;
    int holes = 0;

    int s;
    void *res;

    if (pP->width > pP->height) {
        holes = floor(pP->height / 10);
    } else {
        holes = floor(pP->width / 10);
    }

    struct Circle *circles = circles_init(pP, holes);

    args = calloc(divisions + 1, sizeof(*args));
    if (args == NULL) {
        printf("FATAL ERROR: Failed to create thread arguments.\n");
        printf("Terminating program.....\n");
        exit(1);
    }

    for (int i = 0; i <= divisions - 1; ++i) {
        int n = sectWidth * (1 + i);
        if (i == divisions - 1 && pP->width - n >= 1) {
            int diff = pP->width - n;
            handleSection(i, n + diff, filter, divisions, sectWidth, start, offset, circles);
        } else {
            handleSection(i, n, filter, divisions, sectWidth, start, offset, circles);
        }
        if (n % 3 == 1) {
            offset = 0;
        } else if (n % 3 == 2) {
            offset = -2;
        } else {
            offset = -1;
        }

    }

    for (int i = 0; i < divisions; ++i) {
        s = pthread_join(args[i].threadNum, &res);
        if (s != 0) {
            printf("FATAL ERROR: Thread join failed.\n");
            printf("Terminating program.....\n");
            exit(1);
        }
    }

    free(args->circles);
    free(args);
    struct Pixel *old = pP->pixels;
    pP->pixels = pP->filterNew;
    free(old);
}

/**
 * Perform file processing operations
 * @param img the input image file
 * @param outputFile the output image file
 * @param filter the chosen filter algorithm to run, c or b
 */
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

    if (width > MAXIMUM_IMAGE_SIZE || height > MAXIMUM_IMAGE_SIZE || width < THREAD_COUNT || height < THREAD_COUNT) {
        printf("FATAL ERROR: Image dimensions were either too big or too small.\n");
        printf("Terminating program........\n");
        exit(1);
    }

    processImage(filter);

    FILE *output = fopen(outputFile, "wb");

    writeBMPHeader(output, bmpP->bmpHeader);
    writeDIBHeader(output, bmpP->dibHeader);
    writePixelsBMP(output, pP, pad);

    fclose(output);
}

/**
 * Explains the usage of the program, to be called when no arguments are entered when the program is run
 */
void explainUsage() {
    printf("ERROR: Please enter some arguments to use the program.\n");
    printf("====================== USAGE: =====================\n");
    printf("./<a.out name> -i <input file name.bmp> -o <output file name.bmp> -f <filter type c or b>\n");
    printf("EXAMPLE: ./filter -i test1.bmp -o output.bmp -f b\n");
    printf("OR:\n");
    printf("EXAMPLE: ./filter -i test1.bmp -o output.bmp -f c\n");
    printf("=================================================\n");
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
            isBMP = 1;
        } else {
            isBMP = 0;
        }
    } else if (fileLength >= 1) {
        isBMP = 0;
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


/**
 * Helper method for setting the .bmp extension for a given output file string
 * @param outputFile the string of the output file
 */
void setFileExt(char *outputFile) {
    strcat(outputFile, ".bmp");
}

int main(int argc, char *argv[]) {

    char inputFile[40];
    strcpy(inputFile, "empty");
    char outputFile[40];
    strcpy(outputFile, "empty");
    bmpP = BmpProcessor_init();
    int opt;
    char filter = 'n';

    while ((opt = getopt(argc, argv, ":i:o:f:")) != -1) {
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
            case 'f':
                if (optarg != NULL) {
                    if (strcmp(optarg, "c") == 0) {
                        filter = 'c';
                    } else if (strcmp(optarg, "b") == 0) {
                        filter = 'b';
                    }
                }
                break;
            default:
                break;
        }
    }

    if (strcmp(inputFile, "empty") != 0) {
        if (strcmp(outputFile, "empty") == 1 || strcmp(outputFile, ".bmp") == 0 ||
            validateFileType(0, outputFile) == 0) {
            printf("Output file name or type was invalid or not specified.\n");
            int len = strlen(inputFile);
            char copy[len + 7];
            strncpy(copy, inputFile, len - 4);
            copy[strlen(copy) + 1] = '\0';
            printf("Program will output as %s-2.bmp......\n", copy);
            strncat(copy, "-2", 3);
            setFileExt(copy);
            strncpy(outputFile, copy, strlen(copy) + 1);
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

    BmpProcessor_clean(bmpP);
    PixelProcessor_clean(pP);
    return 0;
}