#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "seqlock.h"  /* TODO implement this header file */

// pthread_seqlock_t lock;

// int shared_data = 0;

// void *reader_thread(void *arg) {
//     int value;
//     unsigned seq;
    
//     while (1) {
//         pthread_seqlock_rdlock(&lock);
//         value = shared_data;
//         pthread_seqlock_rdunlock(&lock);
        
//         printf("Reader: %d\n", value);
//         sleep(1);
//     }
//     return NULL;
// }

// void *writer_thread(void *arg) {
//     int i = 0;
//     while (1) {
//         pthread_seqlock_wrlock(&lock);
//         shared_data = i++;
//         pthread_seqlock_wrunlock(&lock);
        
//         printf("Writer: %d\n", shared_data);
//         sleep(2);
//     }
//     return NULL;
// }

// int main()
// {
   // int val = 0;

   // pthread_seqlock_init(&lock);

   // pthread_seqlock_wrlock(&lock);
   // val++;
   // pthread_seqlock_wrunlock(&lock);


   // if(pthread_seqlock_rdlock(&lock) == 1){
   //    printf("val = %d\n", val); 
   //    pthread_seqlock_rdunlock(&lock);
   // }

   // pthread_t reader, writer;
   //  int val = 0;
   //  pthread_seqlock_init(&lock);
    
   //  pthread_create(&reader, NULL, reader_thread, NULL);
   //  pthread_create(&writer, NULL, writer_thread, NULL);
    
   //  pthread_join(reader, NULL);
   //  pthread_join(writer, NULL);

   //  if(pthread_seqlock_rdlock(&lock) == 1){
   //    printf("val = %d\n", val); 
   //    pthread_seqlock_rdunlock(&lock);
   // }
    
   // //  pthread_seqlock_destroy(&lock);
   //  return 0;
// }



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

   //  pthread_seqlock_destroy(&seqlock);
    return 0;
}
