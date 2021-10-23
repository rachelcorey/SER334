#include <PpmProcessor.h>
#include <malloc.h>
#include <string.h>

PPM_Header* PPMHeader_init() {
    PPM_Header *ppmHeader = malloc(sizeof(PPM_Header));
    strcpy(ppmHeader->id, "P6");
    ppmHeader->maxColorValue = 255;
    return ppmHeader;
}

PpmProcessor* PpmProcessor_init() {
    PpmProcessor *ppmP = malloc(sizeof (BmpProcessor));
    ppmP->ppmHeader = PPMHeader_init();
    return ppmP;
}


void PpmProcessor_clean(PpmProcessor *self) {
    free(self->ppmHeader);
    free(self);
}

int readNumber(FILE* file) {
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
void readPPMHeader(FILE* file, struct PPM_Header* header) {
    char temp[10] = "";
    fread(&temp, sizeof(char), 3, file);
    header->imgWidth = readNumber(file);
    header->imgHeight = readNumber(file);
}


void readToPixelArray(FILE *file, unsigned char *red, unsigned char *green, unsigned char *blue)
{
    unsigned char chars[4];
    fread(chars, 3, 1, file);

    *red = chars[0];
    *green = chars[1];
    *blue = chars[2];
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
void unsignedLongToCharArray(unsigned long u, char* arr) {
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
//    return cResized;
}

void writeIntToFile(FILE* output, int num) {
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
void writePPMHeader(FILE* file, struct PPM_Header* header) {
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
void makePPMHeader(struct PPM_Header* header, int width, int height) {
    header->imgWidth = width;
    header->imgHeight = height;
}

/**
 * read Pixels from PPM file based on width and height.
 *
 * @param  file: A pointer to the file being read or written
 * @param  pArr: Pixel array of the image that this header is for
 * @param  width: Width of the image that this header is for
 * @param  height: Height of the image that this header is for
 */
void readPixelsPPM(FILE* file, struct PixelProcessor *pP, int width, int height) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            readToPixelArray(file, &pP->pixels[i * height + j].red, &pP->pixels[i * height + j].green,
                             &pP->pixels[i * height + j].blue);
        }
//        if (paddingBytes > 0) {
//            fseek(file, paddingBytes, SEEK_CUR);
//        }
    }
}

/**
 * write Pixels from PPM file based on width and height.
 *
 * @param  file: A pointer to the file being read or written
 * @param  pArr: Pixel array of the image that this header is for
 * @param  width: Width of the image that this header is for
 * @param  height: Height of the image that this header is for
 */
void writePixelsPPM(FILE* file, struct PixelProcessor *pP) {

//    for (int i = pP->height - 1 ; i > 0; --i) {
//        for (int j = pP->width - 1; j > 0; --j) {
//
//            fwrite(&pP->pixels[j*pP->height+i].red, 1, 1, file);
//
//            fwrite(&pP->pixels[j*pP->height+i].green, 1, 1, file);
//
//            fwrite(&pP->pixels[j*pP->height+i].blue, 1, 1, file);
//
//        }
//        if (pad > 0) {
//            fwrite(&p, sizeof(char) * pad, 1, file);
//        }
//    }
    for (int i = 0; i < pP->height; ++i) {
        for (int j = 0; j < pP->width; ++j) {

            fwrite(&pP->pixels[i*pP->height+j].red, 1, 1, file);

            fwrite(&pP->pixels[i*pP->height+j].green, 1, 1, file);

            fwrite(&pP->pixels[i*pP->height+j].blue, 1, 1, file);

        }
//        if (pad > 0) {
//            fwrite(&p, sizeof(char) * pad, 1, file);
//        }
    }
}
