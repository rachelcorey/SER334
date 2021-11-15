#include <bits/types/FILE.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

struct Process *processColl;
int ticks;
int processCount;

typedef struct Process {
    int tau;
    float alpha;
    int *ticks;
} Process;

void readFile(char *filename) {
    FILE *file = fopen(filename, "r");
    ticks = 0;
    processCount = 0;

    fscanf(file, "%d", &ticks);
    fscanf(file, "%d", &processCount);

    processColl = malloc(sizeof(struct Process) * processCount);
    struct Process *process = NULL;

    for (int i = 0; i < processCount; ++i) {
        int processNum = 0;
        fscanf(file, "%d", &processNum);
        if (i == processNum) {
            process = malloc(sizeof(struct Process));
            process->ticks = malloc(sizeof(int) * ticks);

            fscanf(file, "%d", &process->tau);
            fscanf(file, "%f", &process->alpha);
            for (int j = 0; j < ticks; ++j) {
                fscanf(file, "%d", &process->ticks[j]);
            }
            processColl[i] = *process;
        }
    }

    fclose(file);
}


int main(int argc, char *argv[]) {
    char *datafile;

    datafile = "../Module-9/data2.txt";
    readFile(datafile);

    for (int i = 0; i < processCount; ++i) {
        printf("process #%d: \n", i);
        printf("tau: %d, alpha: %f\n", processColl[i].tau, processColl[i].alpha);
        for (int j = 0; j < ticks; ++j) {
            printf("tick #%d: %d \n", j, processColl[i].ticks[j]);
        }
    }

//    int opt;
//    while ((opt = getopt(argc, argv, ":d:i:")) != -1) {
//        switch (opt) {
//            case 'f':
//                datafile = optarg;
//                break;
//        }
//    }
//
//    int dflen;
//    if (datafile != NULL) {
//        dflen = strlen(datafile);
//        if (dflen >= 5
//            && (strcmp(&datafile[dflen - 4], ".txt") == 0)
//            && (access(datafile, F_OK) != -1)) {
//            printf("Importing data from %s\n\n", datafile);
//            readFile(datafile);
//        } else {
//            printf("Data file has an invalid name or does not exist.\n");
////            print_usage();
//            exit(1);
//        }
//    } else {
//        printf("No data file name provided. This is a required field.\n");
////        print_usage();
//        exit(1);
//    }

    return 0;
}
