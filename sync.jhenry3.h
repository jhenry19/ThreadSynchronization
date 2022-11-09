#ifndef THREADSYNCHRONIZATION_SYNC_JHENRY3_H
#define THREADSYNCHRONIZATION_SYNC_JHENRY3_H
#include <pthread.h>
#include <stdbool.h>
#include <printf.h>
#define N 10
#define MAX 80

/**
 * Struct for shared resource between threads. A boolean is used to flag whose turn it is to produce or consume from
 * the buffer
 */
typedef struct {
    int buf[MAX];
    int counter;
    int sum;
    bool readyToRead;
    pthread_mutex_t lock;
} Buffer;

/**
 * Inserts an element into the shared buffer
 * @return 0 if successful, 1 if did not complete insertion
 */
int insertItem(Buffer *buffer, int element);

/**
 * Removes an element from the shared buffer
 * @return 0 if successful, 1 if no new element was in buffer, 2 if value is -1 meaning producers is done
 */
int removeItem(Buffer *buffer, int *element);

/**
 * The main execution of the producer, which manages when to run insertItem
 */
void *producer(void *param);

/**
 * The main execution of the consumer, which interprets the results of removeItem
 */
void *consumer(void *param);


#endif //THREADSYNCHRONIZATION_SYNC_JHENRY3_H
