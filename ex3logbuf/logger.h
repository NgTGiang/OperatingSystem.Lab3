#ifndef LOGGER_H
#define LOGGER_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define MAXLOGLENGTH 10
#define MAXBUFFERSLOT 5

extern char **logbuf;
extern int log_count;
extern pthread_mutex_t log_mutex;
extern pthread_cond_t log_not_full, log_not_empty;

int wrlog(char *new_log);
int flushlog();
void sigalrm_handler(int sig);
void init_logger();

#endif // LOGGER_H