#include "parta.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * TODO: Describe what the function does
 */
struct pcb* init_procs(int* bursts, int blen) {
    if (blen <= 0 || bursts == NULL) {
        return NULL;
    }

    struct pcb* procs = malloc(sizeof(struct pcb) * blen);
    if (procs == NULL) {
        return NULL;
    }

    for (int i = 0; i < blen; i++) {
        procs[i].pid = i;
        procs[i].burst_left = bursts[i];
        procs[i].wait = 0;
    }

    return procs;
}

/**
 * TODO: Describe what the function does
 */
void printall(struct pcb* procs, int plen) {
    if (procs == NULL || plen <= 0) {
        return;
    }

    for (int i = 0; i < plen; i++) {
        printf("P%d: burst_left=%d, wait=%d\n",
               procs[i].pid, procs[i].burst_left, procs[i].wait);
    }
}

/**
 * TODO: Describe what the function does
 */
void run_proc(struct pcb* procs, int plen, int current, int amount) {
    if (procs == NULL || plen <= 0) {
        return;
    }
    if (current < 0 || current >= plen) {
        return;
    }
    if (amount <= 0) {
        return;
    }
    if (procs[current].burst_left <= 0) {
        return;
    }

    int actual = amount;
    if (actual > procs[current].burst_left) {
        actual = procs[current].burst_left;
    }

    procs[current].burst_left -= actual;

    for (int i = 0; i < plen; i++) {
        if (i == current) {
            continue;
        }
        if (procs[i].burst_left > 0) {
            procs[i].wait += actual;
        }
    }
}

/**
 * TODO: Describe what the function does
 */
int fcfs_run(struct pcb* procs, int plen) {
    if (procs == NULL || plen <= 0) {
        return 0;
    }

    int current_time = 0;

    for (int i = 0; i < plen; i++) {
        if (procs[i].burst_left <= 0) {
            continue;
        }
        int burst = procs[i].burst_left;
        run_proc(procs, plen, i, burst);
        current_time += burst;
    }

    return current_time;
}

/**
 * TODO: Describe what the function does
 */
int rr_next(int current, struct pcb* procs, int plen) {
    if (procs == NULL || plen <= 0) {
        return -1;
    }

    int all_done = 1;
    for (int i = 0; i < plen; i++) {
        if (procs[i].burst_left > 0) {
            all_done = 0;
            break;
        }
    }
    if (all_done) {
        return -1;
    }

    int start;
    if (current < 0 || current >= plen) {
        start = 0;
    } else {
        start = (current + 1) % plen;
    }

    int i = start;
    while (1) {
        if (procs[i].burst_left > 0) {
            return i;
        }
        i = (i + 1) % plen;
        if (i == start) {
            return -1;
        }
    }
}

/**
 * TODO: Describe what the function does
 */
int rr_run(struct pcb* procs, int plen, int quantum) {
    if (procs == NULL || plen <= 0 || quantum <= 0) {
        return 0;
    }

    int current_time = 0;
    int current = 0;
    int first = 1;

    while (1) {
        if (first) {
            if (current < 0 || current >= plen || procs[current].burst_left <= 0) {
                current = rr_next(-1, procs, plen);
                if (current == -1) {
                    break;
                }
            }
            first = 0;
        }

        if (procs[current].burst_left > 0) {
            int amount = procs[current].burst_left < quantum
                         ? procs[current].burst_left
                         : quantum;
            run_proc(procs, plen, current, amount);
            current_time += amount;
        }

        int next = rr_next(current, procs, plen);
        if (next == -1) {
            break;
        }
        current = next;
    }

    return current_time;
}
