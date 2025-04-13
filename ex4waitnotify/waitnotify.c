#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define MAX_ITEMS 20

int buffer[BUFFER_SIZE];
int count = 0;
int produced = 0;
int consumed = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t can_produce = PTHREAD_COND_INITIALIZER;
pthread_cond_t can_consume = PTHREAD_COND_INITIALIZER;

void* producer(void* arg) {
    while (produced < MAX_ITEMS) {
        pthread_mutex_lock(&mutex);
        
        while (count == BUFFER_SIZE) {
            printf("Producer waiting (buffer full)\n");
            pthread_cond_wait(&can_produce, &mutex);
        }
        
        int item = ++produced;
        buffer[count++] = item;
        printf("Produced %d (count %d)\n", item, count);
        
        pthread_cond_signal(&can_consume);
        pthread_mutex_unlock(&mutex);
        
        usleep(100000);
    }
    return NULL;
}

void* consumer(void* arg) {
    while (consumed < MAX_ITEMS) {
        pthread_mutex_lock(&mutex);
        
        while (count == 0) {
            printf("Consumer waiting (buffer empty)\n");
            pthread_cond_wait(&can_consume, &mutex);
        }
        
        int item = buffer[--count];
        consumed++;
        printf("Consumed %d (count %d)\n", item, count);
        
        pthread_cond_signal(&can_produce);
        pthread_mutex_unlock(&mutex);
        
        usleep(150000);
    }
    return NULL;
}

int main() {
    pthread_t prod_thread, cons_thread;
    
    pthread_create(&prod_thread, NULL, producer, NULL);
    pthread_create(&cons_thread, NULL, consumer, NULL);
    
    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);
    
    printf("Done: Produced %d, Consumed %d\n", produced, consumed);
    
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&can_produce);
    pthread_cond_destroy(&can_consume);
    
    return 0;
}