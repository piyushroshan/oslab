#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#define BUFF_SIZE   5		/* total number of slots */
#define NP          3		/* total number of producers */
#define NC          3		/* total number of consumers */
#define NITERS      4		/* number of items produced/consumed */

typedef struct {
    int buf[BUFF_SIZE];   /* shared var */
    int in;
    int out;
    sem_t full;     	  /* keep track of the number of full spots */
    sem_t empty;    	  /* keep track of the number of empty spots */
    sem_t mutex;    	  /* enforce mutual exclusion to shared data */
} sbuf_t;

sbuf_t shared;

void *Producer(void *arg)
{
    int i, item, index;

    index = (int)arg;

    for (i=0; i < NITERS; i++) {

        /* Produce item */
        item = i;	

        /* Prepare to write item to buf */

        /* If there are no empty slots, wait */
        sem_wait(&shared.empty);
        /* If another thread uses the buffer, wait */
        sem_wait(&shared.mutex);
        item=index*10+i;
        shared.buf[shared.in] = item;
        shared.in = (shared.in+1)%BUFF_SIZE;
        printf("[P%d] Producing %d\n", index, item); fflush(stdout);
        /* Release the buffer */

        sem_post(&shared.mutex);
        /* Increment the number of full slots */
        sem_post(&shared.full);

        /* Interleave  producer and consumer execution */
        sleep(1);
        
    }
    return NULL;
}

void *Consumer(void *arg)
{
    int i, item, index;
    index = (int)arg;

    for (i=0; i < NITERS; i++) {

        /* Consume item */ 

        /* Prepare to read item from buf */
        /* If there are no full slots, wait */
        sem_wait(&shared.full);
        /* If another thread uses the buffer, wait */
        sem_wait(&shared.mutex);
        item = shared.buf[shared.out] ;
        shared.out = (shared.out+1)%BUFF_SIZE;
        printf("--> [C%d] Consuming %d\n", index, item); fflush(stdout);
        /* Release the buffer */
        sem_post(&shared.mutex);
        /* Increment the number of empty slots */
        sem_post(&shared.empty);

        /* Interleave  producer and consumer execution */
        sleep(1);
    }
    return NULL;
}

int main()
{
    pthread_t idP[NP], idC[NC];
    int index;

    if(sem_init(&shared.full, 0, 0))
    {
        printf("Error full\n");
    }
    if(sem_init(&shared.empty, 0, BUFF_SIZE))
    {
        printf("Error Empty\n");
    }
    /* Initialize mutex*/
    if(sem_init(&shared.mutex, 0, 1))
    {
        printf("Error mutex\n");
    }
    for (index = 0; index < NP; index++)
    {  
       /* Create a new producer */
       if(pthread_create(&idP[index], NULL, Producer, (void*)index))
       {
        printf("Error threadp %d\n",index);
       }
    }
    for (index = 0; index < NC; index++)
    {  
       /* Create a new producer */
       if(pthread_create(&idC[index], NULL, Consumer, (void*)index))
       {
        printf("Error threadc %d\n",index);
       }
    }
    for (index = 0; index < NP; index++)
    {  
       /* Create a new producer */
       if(pthread_join(idP[index], NULL))
       {
         printf("Error threadp join %d\n",index);
       }
    }

    /* Insert code here to create NC consumers */

    for (index = 0; index < NC; index++)
    {  
       /* Create a new producer */
       if(pthread_join(idC[index], NULL))
       {
         printf("Error threadc join %d\n",index);
       }
    }

    return 0;
}

