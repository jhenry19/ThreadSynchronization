#ifndef THREADSYNCHRONIZATION_SYNC_JHENRY3_H
#define THREADSYNCHRONIZATION_SYNC_JHENRY3_H
#include <pthread.h>
#define N 8
#define MAX 8

typedef struct {
    int cumulativeSum;
    int count;
} AvgStruct;

typedef struct {
    int buf[MAX];
    int in;
    int out;
    int counter;
    int sum;
    pthread_mutex_t lock;
} Buffer;

int insertItem(Buffer *buffer, int element);
void *producer(void *param);
void *consumer(void *param);


#endif //THREADSYNCHRONIZATION_SYNC_JHENRY3_H
