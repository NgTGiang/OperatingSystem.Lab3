#ifndef PTHREAD_H
#include <pthread.h>
#endif

typedef struct pthread_seqlock { /* TODO: implement the structure */
   volatile unsigned int sequence;
   pthread_mutex_t write_mutex;
} pthread_seqlock_t;

// static inline void pthread_seqlock_init(pthread_seqlock_t *rw)
// {
//    /* TODO: ... */
// }

// static inline void pthread_seqlock_wrlock(pthread_seqlock_t *rw)
// {
//    /* TODO: ... */
// }

// static inline void pthread_seqlock_wrunlock(pthread_seqlock_t *rw)
// {
//    /* TODO: ... */
// }

// static inline unsigned pthread_seqlock_rdlock(pthread_seqlock_t *rw)
// {
//    /* TODO: ... */

//     return 0;
// }


// static inline unsigned pthread_seqlock_rdunlock(pthread_seqlock_t *rw)
// {
//    /* TODO: ... */

//     return 0;
// }

int pthread_seqlock_init(pthread_seqlock_t *seqlock){
   // if(!seqlock) return -1;
   seqlock->sequence = 0;
   return pthread_mutex_init(&seqlock->write_mutex, NULL);
}

int pthread_seqlock_wrlock(pthread_seqlock_t *rw)
{
   /* TODO: ... */
   // if (!rw) return -1;
    
   int ret = pthread_mutex_lock(&rw->write_mutex);
   if (ret != 0) return ret;
    
   __sync_add_and_fetch(&rw->sequence, 1);
   __asm__ __volatile__("" ::: "memory");
    
    return 0;
}

int pthread_seqlock_wrunlock(pthread_seqlock_t *rw)
{
   /* TODO: ... */
   // if (!rw) return -1;
   __asm__ __volatile__("" ::: "memory");
   __sync_add_and_fetch(&rw->sequence, 1);

   return pthread_mutex_unlock(&rw->write_mutex);
}

int pthread_seqlock_rdlock(pthread_seqlock_t *rw)
{
   /* TODO: ... */
   // if(!rw) return -1;
   unsigned int seq;
   do{
      seq = rw->sequence;
      if(seq & 1){
         #ifdef __x86_64__
         __asm__ __volatile__("pause");
         #endif
         continue;
      }

      __asm__ __volatile__("" ::: "memory");
      if (rw->sequence != seq) {
         continue;
     }
     break;

   } while (1);
   
    return 0;
}


int pthread_seqlock_rdunlock(pthread_seqlock_t *rw)
{
   /* TODO: ... */
   // if (!rw) return -1;
   __asm__ __volatile__("" ::: "memory");
   return 0;
}
