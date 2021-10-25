typedef struct PPM_Header {
    char id[2];
    int imgWidth;
    int imgHeight;
    int maxColorValue;
} PPM_Header;

typedef struct PpmProcessor {
    struct PPM_Header *ppmHeader;
} PpmProcessor;

/**
 * read PPM header of a file. Useful for converting files from BMP to PPM.
 *
 * @param  file: A pointer to the file being read or written
 * @param  header: Pointer to the destination PPM header
 */
void readPPMHeader(FILE *file, struct PPM_Header *header);

/**
 * write PPM header of a file. Useful for converting files from BMP to PPM.
 *
 * @param  file: A pointer to the file being read or written
 * @param  header: The header made by makePPMHeader function

 */
void writePPMHeader(FILE *file, struct PPM_Header *header);

/**
 * make PPM header based on width and height. Useful for converting files from BMP to PPM.
 *
 * @param  header: Pointer to the destination PPM header
 * @param  width: Width of the image that this header is for
 * @param  height: Height of the image that this header is for
 */
void makePPMHeader(struct PPM_Header *header, int width, int height);

/**
 * read Pixels from PPM file based on width and height.
 *
 * @param  file: A pointer to the file being read or written
 * @param  pP: A reference to the PixelProcessor object that holds the pixel data
 * @param  width: Width of the image that this header is for
 * @param  height: Height of the image that this header is for
 */
void readPixelsPPM(FILE *file, struct PixelProcessor *pP, int width, int height);

/**
 * write Pixels from PPM file based on width and height.
 *
 * @param  file: A pointer to the file being read or written
 * @param  pP: A reference to the PixelProcessor object that holds the pixel data
 */
void writePixelsPPM(FILE *file, struct PixelProcessor *pP);