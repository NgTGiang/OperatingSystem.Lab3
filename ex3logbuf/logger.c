#include "logger.h"

char **logbuf;
int log_count = 0;
pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t log_not_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t log_not_empty = PTHREAD_COND_INITIALIZER;

int wrlog(char *new_log) {
    pthread_mutex_lock(&log_mutex);
    
    // Wait if buffer is full
    while (log_count >= MAXBUFFERSLOT) {
        pthread_cond_wait(&log_not_full, &log_mutex);
    }
    
    // Allocate new buffer if needed
    if (log_count == 0) {
        logbuf = malloc(MAXBUFFERSLOT * sizeof(char*));
    }
    
    // Copy the log message
    logbuf[log_count] = malloc(MAXLOGLENGTH * sizeof(char));
    strncpy(logbuf[log_count], new_log, MAXLOGLENGTH);
    logbuf[log_count][MAXLOGLENGTH-1] = '\0'; // Ensure null-termination
    log_count++;
    
    // Signal that buffer is not empty
    pthread_cond_signal(&log_not_empty);
    pthread_mutex_unlock(&log_mutex);
    
    return 0;
}

int flushlog() {
    pthread_mutex_lock(&log_mutex);
    
    // Wait if buffer is empty
    while (log_count == 0) {
        pthread_cond_wait(&log_not_empty, &log_mutex);
    }
    
    // Print all log messages
    printf("--- FLUSHING LOG ---\n");
    for (int i = 0; i < log_count; i++) {
        printf("[LOG] %s\n", logbuf[i]);
        free(logbuf[i]);
    }
    
    // Reset buffer
    free(logbuf);
    logbuf = NULL;
    log_count = 0;
    
    // Signal that buffer is not full
    pthread_cond_signal(&log_not_full);
    pthread_mutex_unlock(&log_mutex);
    
    return 0;
}

// Signal handler for periodic flushing
void sigalrm_handler(int sig) {
    flushlog();
    alarm(1); // Reset timer
}

// Initialize logger
void init_logger() {
    signal(SIGALRM, sigalrm_handler);
    alarm(1); // Set 1-second timer
}