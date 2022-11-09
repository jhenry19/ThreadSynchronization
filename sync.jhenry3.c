#include "sync.jhenry3.h"

int insertItem(Buffer *buffer, int element) {
    int rtnval = 1;
    pthread_mutex_lock(&buffer->lock); // Lock acquired because the shared buffer is accessed
    if (buffer->counter < MAX && buffer->readyToRead == false) { // check if over capacity or the last value hasn't been read
        buffer->buf[buffer->counter] = element;
        buffer->readyToRead = true; // changes so consumer knows a new element has been added
        buffer->counter += 1;
        rtnval = 0;
        printf("producer wrote the value %d\n", element);
    }
    pthread_mutex_unlock(&buffer->lock);
    return rtnval;
}

int removeItem(Buffer *buffer, int *element) {
    int rtnval = 1;
    pthread_mutex_lock(&buffer->lock); // Lock is used because shared buffer is accessed
    if (buffer->counter > 0 && buffer->readyToRead == true) {
        if (buffer->buf[buffer->counter - 1] < 0) { // if producer is done, it puts a negative number in the buffer
            rtnval = 2;
            printf("consumer read the value %d\n", buffer->buf[buffer->counter - 1]);
        }
        else { // consuming a value the consumer has produced and adding it to sum
            buffer->sum += buffer->buf[buffer->counter - 1];
            buffer->readyToRead = false;
            rtnval = 0;
            printf("consumer read the value %d\n", buffer->buf[buffer->counter - 1]);
        }
    }
    pthread_mutex_unlock(&buffer->lock);
    return rtnval;
}

/**
 * Sends N values: the integers 0 through N-1. When the producer is done, it sends a negative integer if done producing
 */
void *producer(void *param) {
    Buffer *buffer = (Buffer *) param;
    int sts;
    for (int i = 0; i <= N; ++i) {
        bool tryToInsert = true;
        while(tryToInsert) { // tries to insert until successful
            if (i == N) sts = insertItem(buffer, -1); // inserts -1 on last run
            else sts = insertItem(buffer, i);

            if (sts == 0) { // stops trying to insert if the insert is succesfull
                tryToInsert = false;
            }
        }
    }
    pthread_exit(NULL);
}

void *consumer(void *param) {
    Buffer *buffer = (Buffer *) param;
    int sts;
    bool consuming = true;
    int counter = 0;
    while (consuming) {
        bool tryToRead = true;
        while(tryToRead) {
            sts = removeItem(buffer, &counter);
            if (sts == 0) { // If a normal element is read (element >= 0)
                ++counter;
                tryToRead = false;
            }
            else if (sts == 2) { // If a negative number is read
                pthread_mutex_lock(&buffer->lock); // lock used because shared resource is accessed
                printf("#samples = %d\n", counter);
                printf("sum = %d\n", buffer->sum);
                pthread_mutex_unlock(&buffer->lock);
                tryToRead = false; // Quits this reads while loop
                consuming = false; // Quits main while loop
            }
        }
    }
    pthread_exit(NULL);
}


int main() {
    // Initialize data structures and threads
    Buffer buffer;
    pthread_t producerThread, consumerThread;
    buffer.counter = 0;
    buffer.sum = 0;
    buffer.readyToRead = false;
    pthread_mutex_init(&buffer.lock, NULL);

    // Start threads
    pthread_create(&producerThread, NULL, producer, &buffer);
    pthread_create(&consumerThread, NULL, consumer, &buffer);

    // Join threads
    pthread_join(producerThread, NULL);
    pthread_join(consumerThread, NULL);

    return 0;
}