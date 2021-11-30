/**
 * Helper functions to load a reference string.
 *
 * @author Acuna
 * @version 0.9
 */

#ifndef FILELOADER_H
#define FILELOADER_H

#include <stdio.h>
#include <stdlib.h>

//structs
struct test_scenario {
    int refstr_len;
    int refstr[512];
    int page_count;
    int frame_count;
};


/**
 * Loads a test_scenario strut from a textfile.
 *
 * @param filename The name of the file to load.
 * @return A struct containing the loaded file.
 */
struct test_scenario* load_test_data(char* filename);

#endif