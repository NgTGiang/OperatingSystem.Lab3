#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "seqlock.h"  /* TODO implement this header file */

#define NUM_READERS 5
#define NUM_WRITERS 2

pthread_seqlock_t seqlock;

void *reader(void *arg) {
    for (int i = 0; i < 10; i++) {
        pthread_seqlock_rdlock(&seqlock);
        printf("Reader %ld reading...\n", (long)arg);
        pthread_seqlock_rdunlock(&seqlock);
        usleep(100);
    }
    return NULL;
}

void *writer(void *arg) {
    for (int i = 0; i < 5; i++) {
        pthread_seqlock_wrlock(&seqlock);
        printf("Writer %ld writing...\n", (long)arg);
        pthread_seqlock_wrunlock(&seqlock);
        usleep(200);
    }
    return NULL;
}

int main() {
    pthread_t r_threads[NUM_READERS], w_threads[NUM_WRITERS];

    pthread_seqlock_init(&seqlock);

    for (long i = 0; i < NUM_READERS; i++)
        pthread_create(&r_threads[i], NULL, reader, (void *)i);
    for (long i = 0; i < NUM_WRITERS; i++)
        pthread_create(&w_threads[i], NULL, writer, (void *)i);

    for (int i = 0; i < NUM_READERS; i++)
        pthread_join(r_threads[i], NULL);
    for (int i = 0; i < NUM_WRITERS; i++)
        pthread_join(w_threads[i], NULL);

    return 0;
}
