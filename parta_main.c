#include "parta.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("ERROR: Missing arguments\n");
        return 1;
    }

    char* alg = argv[1];

    if (strcmp(alg, "fcfs") == 0) {
        int plen = argc - 2;
        if (plen <= 0) {
            printf("ERROR: Missing arguments\n");
            return 1;
        }

        int* bursts = malloc(sizeof(int) * plen);
        if (bursts == NULL) {
            return 1;
        }

        for (int i = 0; i < plen; i++) {
            bursts[i] = atoi(argv[2 + i]);
        }

        printf("Using FCFS\n\n");

        for (int i = 0; i < plen; i++) {
            printf("Accepted P%d: Burst %d\n", i, bursts[i]);
        }

        struct pcb* procs = init_procs(bursts, plen);
        if (procs == NULL) {
            free(bursts);
            return 1;
        }

        fcfs_run(procs, plen);

        int total_wait = 0;
        for (int i = 0; i < plen; i++) {
            total_wait += procs[i].wait;
        }
        double avg = (double) total_wait / (double) plen;
        printf("Average wait time: %.2f\n", avg);

        free(procs);
        free(bursts);
        return 0;

    } else if (strcmp(alg, "rr") == 0) {
        if (argc < 4) {
            printf("ERROR: Missing arguments\n");
            return 1;
        }

        int quantum = atoi(argv[2]);
        int plen = argc - 3;
        if (plen <= 0) {
            printf("ERROR: Missing arguments\n");
            return 1;
        }

        int* bursts = malloc(sizeof(int) * plen);
        if (bursts == NULL) {
            return 1;
        }

        for (int i = 0; i < plen; i++) {
            bursts[i] = atoi(argv[3 + i]);
        }

        printf("Using RR(%d).\n\n", quantum);

        for (int i = 0; i < plen; i++) {
            printf("Accepted P%d: Burst %d\n", i, bursts[i]);
        }

        struct pcb* procs = init_procs(bursts, plen);
        if (procs == NULL) {
            free(bursts);
            return 1;
        }

        rr_run(procs, plen, quantum);

        int total_wait = 0;
        for (int i = 0; i < plen; i++) {
            total_wait += procs[i].wait;
        }
        double avg = (double) total_wait / (double) plen;
        printf("Average wait time: %.2f\n", avg);

        free(procs);
        free(bursts);
        return 0;

    } else {
        printf("ERROR: Missing arguments\n");
        return 1;
    }
}
