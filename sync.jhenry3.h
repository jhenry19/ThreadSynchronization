#ifndef THREADSYNCHRONIZATION_SYNC_JHENRY3_H
#define THREADSYNCHRONIZATION_SYNC_JHENRY3_H
#include <pthread.h>
#include <stdbool.h>
#define N 10
#define MAX 80


typedef struct {
    int cumulativeSum;
    int count;
} AvgStruct;

typedef struct {
    int buf[MAX];
    int counter;
    int sum;
    bool readyToRead;
    pthread_mutex_t lock;
} Buffer;

int insertItem(Buffer *buffer, int element);
void *producer(void *param);
void *consumer(void *param);


#endif //THREADSYNCHRONIZATION_SYNC_JHENRY3_H
