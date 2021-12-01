#include <DataLoader.h>
#include <bits/types/FILE.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define MAX_ENTRIES 512

struct test_scenario* load_test_data(char* filename) {

    FILE *file = fopen(filename, "r");

    int pages = 0;
    int frames = 0;
    int entries = 0;


    fscanf(file, "%d", &pages);
    fscanf(file, "%d", &frames);
    fscanf(file, "%d", &entries);
    struct test_scenario* test = malloc(sizeof(struct test_scenario));
    test->page_count = pages;
    test->frame_count = frames;
    test->refstr_len = entries;

    for (int i = 0; i < entries; ++i) {
        fscanf(file, "%d", &test->refstr[i]);
    }


    fclose(file);

    return test;
}