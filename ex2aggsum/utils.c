#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

struct _appconf appconf;

int tonum(const char *nptr, int *num) {
    char *endptr;
    long val;

    errno = 0;
    val = strtol(nptr, &endptr, 10);

    if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN)) ||
        (errno != 0 && val == 0)) {
        return -1;
    }

    if (endptr == nptr || *endptr != '\0') {
        return -1;
    }

    if (val < 0 || val > INT_MAX) {
        return -1;
    }

    *num = (int)val;
    return 0;
}

int processopts(int argc, char **argv, struct _appconf *conf) {
    /* Set defaults */
    conf->seednum = SEEDNO;

    /* Parse arrsz */
    if (tonum(argv[1], &conf->arrsz)) {
        fprintf(stderr, "Invalid array size\n");
        return -1;
    }

    /* Parse tnum */
    if (tonum(argv[2], &conf->tnum)) {
        fprintf(stderr, "Invalid thread count\n");
        return -1;
    }

    /* Parse optional seednum */
    if (argc == 4) {
        if (tonum(argv[3], &conf->seednum)) {
            fprintf(stderr, "Invalid seed number\n");
            return -1;
        }
    }

    return 0;
}

int validate_and_split_argarray(int arraysize, int num_thread, struct _range* thread_idx_range) {
    if (arraysize < num_thread * THRSL_MIN) {
        return -1;
    }

    int base_chunk = arraysize / num_thread;
    int remainder = arraysize % num_thread;
    int current_start = 0;

    for (int i = 0; i < num_thread; i++) {
        thread_idx_range[i].start = current_start;
        thread_idx_range[i].end = current_start + base_chunk - 1;

        /* Distribute remainder */
        if (i < remainder) {
            thread_idx_range[i].end++;
        }

        current_start = thread_idx_range[i].end + 1;
    }

    return 0;
}

int generate_array_data(int* buf, int arraysize, int seednum) {
    if (buf == NULL || arraysize <= 0) {
        return -1;
    }

    srand(seednum);
    for (int i = 0; i < arraysize; i++) {
        buf[i] = LWBND_DATA_VAL + rand() % (UPBND_DATA_VAL - LWBND_DATA_VAL + 1);
    }

    return 0;
}

void help(int xcode) {
    fprintf(stdout, "%s, version %s\n", PACKAGE, VERSION);
    fprintf(stdout, "usage: %s %s %s [%s]\n", PACKAGE, ARG1, ARG2, ARG3);
    fprintf(stdout, "Generate randomly integer array size <%s> and calculate sum parallelly\n", ARG1);
    fprintf(stdout, "using <%s> threads. The optional <%s> value use to control\n", ARG2, ARG3);
    fprintf(stdout, "the randomization of the generated array.\n");
    fprintf(stdout, "Arguments:\n");
    fprintf(stdout, "%s\tspecifies the size of array.\n", ARG1);
    fprintf(stdout, "%s\tnumber of parallel threads.\n", ARG2);
    fprintf(stdout, "%s\tinitialize the state of the randomized generator.\n", ARG3);
    exit(xcode);
}