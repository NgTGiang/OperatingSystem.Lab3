#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define MAX_LOG_LENGTH 10
#define MAX_BUFFER_SLOT 6
#define MAX_LOOPS 30

char logbuf[MAX_BUFFER_SLOT][MAX_LOG_LENGTH];

int count;
int should_exit = 0;

pthread_mutex_t buffer_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t buffer_not_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t buffer_not_empty = PTHREAD_COND_INITIALIZER;
void flushlog();

struct _args
{
   unsigned int interval;
};

void *wrlog(void *data)
{
   char str[MAX_LOG_LENGTH];
   int id = *(int*) data;

   pthread_mutex_lock(&buffer_mutex);
   while (count >= MAX_BUFFER_SLOT) {
      pthread_cond_wait(&buffer_not_full, &buffer_mutex);
   }

   // usleep(20);
   sprintf(str, "%d", id);
   strcpy(logbuf[count], str);
   count = (count > MAX_BUFFER_SLOT)? count :(count + 1); /* Only increase count to size MAX_BUFFER_SLOT*/
   // printf("wrlog(): %d \n", id);

   if (count == MAX_BUFFER_SLOT) {
      pthread_cond_signal(&buffer_not_empty);
   }
   pthread_mutex_unlock(&buffer_mutex);
   usleep(1000);
   return 0;
}

void flushlog()
{
   int i;
   char nullval[MAX_LOG_LENGTH];

   // printf("flushlog()\n");
   sprintf(nullval, "%d", -1);

   pthread_mutex_lock(&buffer_mutex);
   while (count < MAX_BUFFER_SLOT && !should_exit) {
      pthread_cond_wait(&buffer_not_empty, &buffer_mutex);
   }

   for (i = 0; i < count; i++)
   {
      printf("Slot  %i: %s\n", i, logbuf[i]);
      strcpy(logbuf[i], nullval);
   }

   fflush(stdout);

   /*Reset buffer */
   count = 0;

   pthread_cond_broadcast(&buffer_not_full);
   pthread_mutex_unlock(&buffer_mutex);
   return;

}

void *timer_start(void *args)
{
   while (!should_exit)
   {
      flushlog();
      /*Waiting until the next timeout */
      usleep(((struct _args *) args)->interval);
      pthread_cond_signal(&buffer_not_empty);
   }
}

int main()
{
   int i;
   count = 0;
   pthread_t tid[MAX_LOOPS];
   pthread_t lgrid;
   int id[MAX_LOOPS];

   struct _args args;
   args.interval = 500e3;
   /*500 msec ~ 500 * 1000 usec */

   pthread_mutex_init(&buffer_mutex, NULL);
   pthread_cond_init(&buffer_not_full, NULL);
   pthread_cond_init(&buffer_not_empty, NULL);

   /*Setup periodically invoke flushlog() */
   pthread_create(&lgrid, NULL, &timer_start, (void*) &args);

   /*Asynchronous invoke task writelog */
   for (i = 0; i < MAX_LOOPS; i++)
   {
      id[i] = i;
      pthread_create(&tid[i], NULL, wrlog, (void*) &id[i]);
   }

   for (i = 0; i < MAX_LOOPS; i++)
      pthread_join(tid[i], NULL);

   pthread_mutex_destroy(&buffer_mutex);
   pthread_cond_destroy(&buffer_not_full);
   pthread_cond_destroy(&buffer_not_empty);

   sleep(5);

   return 0;
}
