#include <printf.h>
#include "sync.jhenry3.h"


int insertItem(Buffer *buffer, int element) {
    int rtnval = 1;
    pthread_mutex_lock(&buffer->lock);
    if (buffer->counter < MAX) {
        buffer->buf[buffer->counter] = element;
        buffer->flag = -1;
        buffer->counter += 1;
        rtnval = 0;
    }
    pthread_mutex_unlock(&buffer->lock);
    return rtnval;
}

int removeItem(Buffer *buffer, int *element) {
    int rtnval = 1;
    pthread_mutex_lock(&buffer->lock);
    if (buffer->counter > 0) {
        buffer->sum += buffer->buf[buffer->counter - 1];
        buffer->flag = 0;
        rtnval = 0;
    }
    pthread_mutex_unlock(&buffer->lock);
    return rtnval;
}

/**
 * Sends N values: the integers 0 through N-1. When the producer is done, it should send a negative
 * integer: that will tell the consumer to print a summary (the number of values sent, and the sum of all of the
 * values). For this program, “sending a value” means making it available for the consumer to read.
 */
void *producer(void *param) {
    Buffer *buffer = (Buffer *) param;
    int sts;
    for (int i = 0; i < N; ++i) {
        sts = insertItem(buffer, i);
        if (sts == 0) {
            printf("producer wrote the value %d\n", i);
        }
        else
            printf("producer FAILED to write %d\n", i);
    }
    pthread_exit(NULL);
}

void *consumer(void *param) {
    Buffer *buffer = (Buffer *) param;
    int sts;
    for (int i = 0; i < N; ++i) {
        sts = removeItem(buffer, i);
        if (sts == 0) {
            printf("consumer read the value %d\n", i);
        }
        else
            printf("consumer FAILED to read value %d\n", i);
    }
    pthread_exit(NULL);
}


int main() {
    // Initialize data structures and threads
    Buffer buffer;
    pthread_t producerThread, consumerThread;

    buffer.counter = 0;
    buffer.sum = 0;
    buffer.flag = 0;
    pthread_mutex_init(&buffer.lock, NULL);

    pthread_create(&producerThread, NULL, producer, &buffer);
    pthread_create(&consumerThread, NULL, consumer, &buffer);

    pthread_join(producerThread, NULL);
    pthread_join(consumerThread, NULL);

    printf("sum = %d", buffer.sum);

    return 0;
}