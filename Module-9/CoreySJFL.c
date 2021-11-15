/**
* This program reads and parses a data file of processes and their tau/alpha values, then
* sorts them accordingly.
*
* Completion time: 3 hours
*
* @author Rachel Corey
* @version 1.0, completed on 11/15/2021
*
*/


#include <bits/types/FILE.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct Process *processColl;
int ticks;
int processCount;
int currentTick;

typedef struct Process {
    int name;
    int tau;
    float alpha;
    int *ticks;
    int *estimations;
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
            process->name = i;
            process->ticks = malloc(sizeof(int) * ticks);
            process->estimations = malloc(sizeof(int) * ticks);

            fscanf(file, "%d", &process->tau);
            fscanf(file, "%f", &process->alpha);
            for (int j = 0; j < ticks; ++j) {
                fscanf(file, "%d", &process->ticks[j]);
                process->estimations[j] = -1;
            }
            processColl[i] = *process;
            free(process);
        }
    }

    fclose(file);
}


int reset(const void *a, const void *b) {
    Process *proc1 = (Process *) a;
    Process *proc2 = (Process *) b;

    return (proc1->name - proc2->name);
}

int compare(const void *a, const void *b) {
    Process *proc1 = (Process *) a;
    Process *proc2 = (Process *) b;

    return (proc1->ticks[currentTick] - proc2->ticks[currentTick]);
}


int compare2(const void *a, const void *b) {
    Process *proc1 = (Process *) a;
    Process *proc2 = (Process *) b;

    return (proc1->estimations[currentTick] - proc2->estimations[currentTick]);
}

void SJF() {
    int time = 0;
    int turnAroundTime = 0;
    int waitingTime = 0;
    printf("==Shortest-Job-First==\n");
    for (int i = 0; i < ticks; ++i) {
        currentTick = i;
        qsort(processColl, processCount, sizeof(Process), compare);
        printf("Simulating %dth tick of processes @ time %d: \n", i, time);
        for (int j = 0; j < processCount; ++j) {
            printf("    Process %d took %d.\n", processColl[j].name, processColl[j].ticks[i]);
            time += processColl[j].ticks[i];
            if (j > 0) {
                waitingTime += processColl[j - 1].ticks[i];
            }
        }
    }
    turnAroundTime = time + waitingTime;
    printf("Turnaround time: %d\n", turnAroundTime);
    printf("Waiting time: %d\n", waitingTime);
    currentTick = 0;
    qsort(processColl, processCount, sizeof(Process), compare);
}

void SJFL() {
    int time = 0;
    int turnAroundTime = 0;
    int waitingTime = 0;
    int prevTau = 0;
    int tickTime = 0;
    int estimation = 0;
    int estimationError = 0;
    printf("\n");
    printf("==Shortest-Job-First Live==\n");
    for (int i = 0; i < ticks; ++i) {
        currentTick = i;
        for (int j = 0; j < processCount; ++j) {
            float alpha = processColl[j].alpha;
            if (i > 0) {
                prevTau = processColl[j].estimations[i - 1];
                tickTime = processColl[j].ticks[i - 1];
                estimation = (((alpha) * (tickTime)) + ((1 - alpha) * (prevTau)));
                processColl[j].estimations[i] = estimation;
            } else {
                processColl[j].estimations[i] = processColl[j].tau;
                prevTau = processColl[j].tau;
            }
        }
        qsort(processColl, processCount, sizeof(Process), compare2);
        printf("Simulating %dth tick of processes @ time %d: \n", i, time);
        for (int j = 0; j < processCount; ++j) {
            printf("    Process %d was estimated for %d and took %d.\n", processColl[j].name,
                   processColl[j].estimations[i], processColl[j].ticks[i]);
            time += processColl[j].ticks[i];
            estimationError += abs((processColl[j].estimations[i] - processColl[j].ticks[i]));
            if (j > 0) {
                waitingTime += processColl[j - 1].ticks[i];
            }
        }
    }
    turnAroundTime = time + waitingTime;
    printf("Turnaround time: %d\n", turnAroundTime);
    printf("Waiting time: %d\n", waitingTime);
    printf("Estimation Error: %d\n", estimationError);
}

void destroyProcessColl() {
    for (int i = processCount - 1; i >= 0; --i) {
        free(processColl[i].ticks);
        free(processColl[i].estimations);
    }
    free(processColl);
}

void print_usage() {
    printf("====================== USAGE: =====================\n");
    printf("./<a.out name> -f <inputFileName.txt>\n");
    printf("EXAMPLE: ./sjfl -f ../path/to/inputFile.txt\n");
}

int main(int argc, char *argv[]) {
    char datafile[40];
    strcpy(datafile, "empty");

    int opt;
    while ((opt = getopt(argc, argv, ":f:")) != -1) {
        switch (opt) {
            case 'f':
                if (optarg != NULL) {
                    strcpy(datafile, optarg);
                }
                break;
        }
    }

    int dflen = strlen(datafile);
    if (dflen >= 5
        && (strcmp(&datafile[dflen - 4], ".txt") == 0)
        && (access(datafile, F_OK) != -1)
        && (strcmp(datafile, "empty") != 0)) {
            printf("Importing data from %s.....\n\n", datafile);
            readFile(datafile);
    } else {
        printf("FATAL ERROR: Data file has an invalid name or does not exist.\n");
        print_usage();
        exit(1);
    }


    SJF();
    qsort(processColl, processCount, sizeof(struct Process), reset);
    SJFL();
    destroyProcessColl();

    return 0;
}
