#include <printf.h>
#include "sync.jhenry3.h"


int insertItem(Buffer *buffer, int element) {
    int rtnval = 1;
    pthread_mutex_lock(&buffer->lock);
    if (buffer->counter < MAX) {
        buffer->buf[buffer->in] = element;
        buffer->in = (buffer->in + 1) % MAX;
        buffer->counter = buffer->counter + 1;
        rtnval = 0;
    }
    pthread_mutex_unlock(&buffer->lock);
    return rtnval;
}

int removeItem(Buffer *buffer, int *element) {
    int rtnval = 1;
    pthread_mutex_lock(&buffer->lock);
    if (buffer->counter > 0) {
        *element = buffer->buf[buffer->out];
        buffer->out = (buffer->out + 1) % MAX;
        buffer->counter = buffer->counter - 1;
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
        if (sts == 0)
            printf("producer wrote the value %d\n", i);
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
        if (sts == 0)
            printf("consumer read the value %d\n", i);
        else
            printf("consumer FAILED to read value %d\n", i);
    }
    pthread_exit(NULL);
}


int main() {
    // Initialize data structures and threads
    Buffer buffer;
    pthread_t producer, consumer;

    buffer.in = 0;
    buffer.out = 0;
    buffer.counter = 0;
    buffer.sum = 0;
    pthread_mutex_init(&buffer.lock, NULL);

    pthread_create(&producer, NULL, (void *(*)(void *)) producer, &buffer);
    pthread_create(&consumer, NULL, (void *(*)(void *)) consumer, &buffer);

    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);
}