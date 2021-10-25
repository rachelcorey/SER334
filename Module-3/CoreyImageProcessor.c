/**
* This program reads and parses an image file of the user's choice of bmp or ppm type, and can convert the file in to
* bmp or ppm type, or shift the pixel color values by a specified amount.
*
* Completion time: 9000000 hours
*
* @author Rachel Corey, Whoever Wrote The Base Code On Canvas
* @version 1.0, completed on 10/25/2021
*
*/

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

BmpProcessor *bmpP;
PpmProcessor *ppmP;
PixelProcessor *pP;
int outputIsBMP;
int inputIsBMP;
int typeSet;

/**
 * Helper method for verifying if a character is an integer
 * @param inty
 * @return 1 if character is an integer, 0 if not
 */
int validateInt(char inty) {
    if (isdigit(inty)) {
        return 1;
    } else {
        return 0;
    }
}

/**
 * Helper method for setting a file extension type for a given output file string
 * @param outputFile the string of the output file
 * @param type the type of the file it should be output to
 */
void setFileExt(char *outputFile, char *type) {
    strcat(outputFile, ".");
    strcat(outputFile, type);
    typeSet = 1;
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
        } else if ((strcmp(&fileName[fileLength - 4], ".ppm") == 0)) {
            if (isInput != 1) {
                setFileExt(fileName, "ppm");
            }
            isBMP = 0;
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
            printf("Valid file extensions are .bmp or .ppm!\n");
            printf("Terminating program.....\n");
            exit(1);
        }
    }
    return isBMP;
}

/**
 * Calculates the padding for a BMP type image
 * @param imgWidth the width of the image
 * @return the amount of padding needed to write to the file
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
 * Validates if the given string for a color shift option in the command line is valid or not
 * @param numOption the option given by the user
 * @return 1 if it's a valid integer, 0 if not
 */
int validateColorShift(char *numOption) {
    int shift = 0;
    char *ptr;
    char first;
    char second;
    first = numOption[0];
    second = numOption[1];

    if (first == '-') {
        if (validateInt(second)) {
            shift = strtol(numOption, &ptr, 10);
        }
    } else if (validateInt(*numOption)) {
        shift = strtol(numOption, &ptr, 10);
    } else {
        printf("Error: Please enter a valid integer for color shifting.\n");
    }
    return shift;
}

/**
 * Processes the image input from the user
 * @param img the image to be processed
 * @param inputIsBMP if the input image is of BMP type or not
 * @param outputIsBMP if the output image is of BMP type or not
 * @param outputFile the file string to output to
 * @param red the amount to shift the red pixels of the image
 * @param green the amount to shift the green pixels of the image
 * @param blue the amount to shift the blue pixels of the image
 */
void processImage(FILE *img, int inputIsBMP, int outputIsBMP, char *outputFile, int red, int green, int blue) {
    int pad = 0;
    int width = 0;
    int height = 0;

    if (inputIsBMP) {
        readBMPHeader(img, bmpP->bmpHeader);
        readDIBHeader(img, bmpP->dibHeader);
        width = bmpP->dibHeader->imgWidth;
        height = bmpP->dibHeader->imgHeight;
        pP = PixelProcessor_init(width, height);
        pad = calculatePadding(pP->width);
        readPixelsBMP(img, pP, pad);
        makePPMHeader(ppmP->ppmHeader, width, height);
    } else {
        readPPMHeader(img, ppmP->ppmHeader);
        fseek(img, 4, SEEK_CUR);
        width = ppmP->ppmHeader->imgWidth;
        height = ppmP->ppmHeader->imgHeight;
        pP = PixelProcessor_init(width, height);
        readPixelsPPM(img, pP, width, height);
        int dest = fileno(img);
        struct stat data;
        fstat(dest, &data);
        int size = data.st_size;
        makeBMPHeader(bmpP->bmpHeader);
        bmpP->bmpHeader->size = size;
        makeDIBHeader(bmpP->dibHeader, width, height);
        bmpP->dibHeader->imgSize = size;

    }

    fclose(img);
    colorShiftPixels(pP, red, green, blue);
    FILE *output = fopen(outputFile, "wb");

    if (outputIsBMP) {
        writeBMPHeader(output, bmpP->bmpHeader);
        writeDIBHeader(output, bmpP->dibHeader);
        writePixelsBMP(output, pP, pad);
    } else {
        writePPMHeader(output, ppmP->ppmHeader);
        writePixelsPPM(output, pP);
    }

    fclose(output);
}

/**
 * Validates the output type from the -t command option
 * @param type the user entered
 * @return 1 if the type is valid, 0 otherwise
 */
int validateOutputType(char *type) {
    char bmpType[4] = {'b', 'm', 'p', '\0'};
    char ppmType[4] = {'p', 'p', 'm', '\0'};
    if (strcmp(bmpType, type) == 0 || strcmp(ppmType, type) == 0) {
        if (strcmp(ppmType, type) == 0) {
            strcpy(type, ppmType);
            typeSet = 1;
            outputIsBMP = 0;
        } else {
            strcpy(type, bmpType);
            typeSet = 1;
            outputIsBMP = 1;
        }
        return 1;
    } else {
        printf("'%s' is not a supported output file type. Using BMP as default....", type);
        return 0;
    }
}

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


int main(int argc, char *argv[]) {
    char inputFile[40];
    strcpy(inputFile, "empty");
    char outputFile[40];
    char type[4] = "";
    ppmP = PpmProcessor_init();
    bmpP = BmpProcessor_init();
    long redShift = 0;
    long blueShift = 0;
    long greenShift = 0;
    inputIsBMP = 0;
    outputIsBMP = 0;
    typeSet = 0;
    int opt;

    while ((opt = getopt(argc, argv, ":f:o:r:g:b:t:")) != -1) {
        switch (opt) {
            case 'f':
                if (optarg != NULL) {
                    strcpy(inputFile, optarg);
                }
                break;
            case 'o':
                if (optarg != NULL) {
                    if (strlen(optarg) > 0) {
                        strcpy(outputFile, optarg);
                        outputIsBMP = validateFileType(0, outputFile);
                    } else {
                        outputIsBMP = 2;
                    }
                } else {
                    outputIsBMP = 3;
                }
                break;
            case 'r':
                redShift = validateColorShift(optarg);
                break;
            case 'g':
                greenShift = validateColorShift(optarg);
                break;
            case 'b':
                blueShift = validateColorShift(optarg);
                break;
            case ':':
                outputIsBMP = 3;
                break;
            case 't':
                if (optarg != NULL) {
                    strcpy(type, optarg);
                    if (validateOutputType(type)) {
                        setFileExt(outputFile, type);
                    } else {
                        outputIsBMP = 3;
                    }
                } else {
                    outputIsBMP = 3;
                }
                break;
            default:
                break;
        }
    }

    if (strlen(outputFile) < 1) {
        outputIsBMP = 3;
    }

    if (strcmp(inputFile, "empty") != 0) {
        char op[] = "output";
        if (outputIsBMP == 3 || strcmp(outputFile, ".bmp") == 0 || strcmp(outputFile, ".ppm") == 0) {
            printf("File output name or type was unspecified.\n");
            if (typeSet) {
                printf("Outputting as output.%s...\n", type);
                strcpy(outputFile, op);
                setFileExt(outputFile, type);
            } else {
                printf("Outputting as output.bmp....\n");
                strcpy(outputFile, op);
                setFileExt(outputFile, "bmp");
            }
        }
        if (outputIsBMP == 2) {
            if (typeSet) {
                setFileExt(outputFile, type);
            } else {
                printf("Output file type was invalid or not specified.\n");
                printf("Program will output as .bmp......\n");
                strcat(outputFile, ".bmp");
                outputIsBMP = 1;
            }
        }
        inputIsBMP = validateFileType(1, inputFile);

        FILE *img = fopen(inputFile, "rb");
        if (img != NULL) {
            processImage(img, inputIsBMP, outputIsBMP, outputFile, redShift, greenShift, blueShift);
        } else {
            printf("FATAL ERROR: File '%s' not found in the system.\n", inputFile);
            printf("Terminating program........\n");
            exit(1);
        }
    } else {
        explainUsage();
        exit(1);
    }

    BmpProcessor_clean(bmpP);
    PpmProcessor_clean(ppmP);
    PixelProcessor_clean(pP);
    return 0;
}




