#include <PpmProcessor.h>
#include <malloc.h>
#include <string.h>

/**
 * Initialize a PPM Header Object
 */
PPM_Header *PPMHeader_init() {
    PPM_Header *ppmHeader = malloc(sizeof(PPM_Header));
    strcpy(ppmHeader->id, "P6");
    ppmHeader->maxColorValue = 255;
    return ppmHeader;
}

/**
 * Initialize a PPM Processor Object
 * @return a reference to the initialized PPM Processor Object
 */
PpmProcessor *PpmProcessor_init() {
    PpmProcessor *ppmP = malloc(sizeof(BmpProcessor));
    ppmP->ppmHeader = PPMHeader_init();
    return ppmP;
}

/**
 * Clean a PPM Processor Object from the system
 * @param self a reference to the object to be freed
 */
void PpmProcessor_clean(PpmProcessor *self) {
    free(self->ppmHeader);
    free(self);
}

/**
 * Read a number from the specified file
 * @param file the number to be read from
 * @return the int converted from a read string
 */
int readNumber(FILE *file) {
    int dim = 0;
    char *ptr;
    char imgDim[4] = "";
    char space[1] = " ";
    char num[2] = "";
    fread(&num, sizeof(char), 1, file);
    int p = 0;
    while (strcmp(space, num) != 0 && p < 3) {
        strcpy(&imgDim[p], &num[0]);
        ++p;
        fread(&num, sizeof(char), 1, file);
    }
    dim = strtol(imgDim, &ptr, 10);
    return dim;
}

/**
 * read PPM header of a file. Useful for converting files from BMP to PPM.
 *
 * @param  file: A pointer to the file being read or written
 * @param  header: Pointer to the destination PPM header
 */
void readPPMHeader(FILE *file, struct PPM_Header *header) {
    char temp[10] = "";
    fread(&temp, sizeof(char), 3, file);
    header->imgWidth = readNumber(file);
    header->imgHeight = readNumber(file);
}

/**
 * Read a given row of pixels from a PPM file that represent a pixel
 * @param file PPM file to be read
 * @param red red value
 * @param green green value
 * @param blue blue value
 */
void readToPixelArray(FILE *file, unsigned char *red, unsigned char *green, unsigned char *blue) {
    unsigned char chars[4];
    fread(chars, 3, 1, file);

    *red = chars[0];
    *green = chars[1];
    *blue = chars[2];
}

/**
 * Helper function to calculate the number of digits that a number has
 * @param n number to calculate
 * @return the number of digits
 */
int numPlaces(int n) {
    int r = 1;
    if (n < 0) n = (n == INT_MIN) ? INT_MAX : -n;
    while (n > 9) {
        n /= 10;
        r++;
    }
    return r;
}

/**
 * Converts an Unsigned Long type to a char array
 * @param u the long to be converted
 * @param arr the associated char array of the values to be written to
 */
void unsignedLongToCharArray(unsigned long u, char *arr) {
    char color[4];
    int chars = 0;
    int e = (int) u;
    if (e / 100 >= 1) {
        int h = e / 100;
        int t = e / 10 % 10;
        int o = e % 10;
        char cch = h + '0';
        color[0] = cch;
        cch = t + '0';
        color[1] = cch;
        cch = o + '0';
        color[2] = cch;
        chars = 3;
    } else if (e / 10 >= 1) {
        int t = e / 10;
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
}

/**
 * Write an integer to the given file
 * @param output the file to write to
 * @param num the number to write
 */
void writeIntToFile(FILE *output, int num) {
    int numP = numPlaces((int) num);
    char arr[4];
    unsignedLongToCharArray(num, arr);

    for (int i = 0; i < numP; ++i) {
        fwrite(&arr[i], sizeof(char), 1, output);
    }
}

/**
 * write PPM header of a file. Useful for converting files from BMP to PPM.
 *
 * @param  file: A pointer to the file being read or written
 * @param  header: The header made by makePPMHeader function
 */
void writePPMHeader(FILE *file, struct PPM_Header *header) {
    fwrite(header->id, sizeof(char) * 2, 1, file);
    fwrite("\n", sizeof(char), 1, file);
    writeIntToFile(file, header->imgWidth);
    fwrite("\n", sizeof(char), 1, file);
    writeIntToFile(file, header->imgHeight);
    fwrite("\n", sizeof(char), 1, file);
    writeIntToFile(file, header->maxColorValue);
    fwrite("\n", sizeof(char), 1, file);
}


/**
 * make PPM header based on width and height. Useful for converting files from BMP to PPM.
 *
 * @param  header: Pointer to the destination PPM header
 * @param  width: Width of the image that this header is for
 * @param  height: Height of the image that this header is for
 */
void makePPMHeader(struct PPM_Header *header, int width, int height) {
    header->imgWidth = width;
    header->imgHeight = height;
}

/**
 * read Pixels from PPM file based on width and height.
 *
 * @param  file: A pointer to the file being read or written
 * @param  pP: A reference to the PixelProcessor object that holds the pixel data
 * @param  width: Width of the image that this header is for
 * @param  height: Height of the image that this header is for
 */
void readPixelsPPM(FILE *file, struct PixelProcessor *pP, int width, int height) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            readToPixelArray(file, &pP->pixels[i * height + j].red, &pP->pixels[i * height + j].green,
                             &pP->pixels[i * height + j].blue);
        }
    }
}

/**
 * write Pixels from PPM file based on width and height.
 *
 * @param  file: A pointer to the file being read or written
 * @param  pP: A reference to the PixelProcessor object that holds the pixel data
 */
void writePixelsPPM(FILE *file, struct PixelProcessor *pP) {
    for (int i = 0; i < pP->height; ++i) {
        for (int j = 0; j < pP->width; ++j) {
            fwrite(&pP->pixels[i * pP->height + j].red, 1, 1, file);
            fwrite(&pP->pixels[i * pP->height + j].green, 1, 1, file);
            fwrite(&pP->pixels[i * pP->height + j].blue, 1, 1, file);
        }
    }
}
