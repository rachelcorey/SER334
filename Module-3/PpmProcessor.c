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

int readDim(FILE* file) {
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
    header->imgWidth = readDim(file);
    header->imgHeight = readDim(file);
    fread(&temp, sizeof(int), 1, file);
    fread(&temp, sizeof(char), 2, file);
}
