// jdh CS201 Fall 2022
// example of thread-safe bounded buffer

#include <stdio.h>
#include <pthread.h>

#define MAX 8

typedef struct {
  int buf[MAX];
  int in;
  int out;
  int counter;
  pthread_mutex_t lock;
} TSBuffer;

int insertItem(TSBuffer *buffer, int element) {
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

int removeItem(TSBuffer *buffer, int *element) {
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

void *runnerOne(void *param) {
  TSBuffer *buffer = (TSBuffer *) param;
  int i, sts;

  for (i=0; i<50; ++i) {
    sts = insertItem(buffer, i);
    if (sts == 0)
      printf("runner #1 inserted %d\n", i);
    else
      printf("runner #1 failed trying to insert %d\n", i);
  }

  pthread_exit(NULL);
}

void *runnerTwo(void *param) {
  TSBuffer *buffer = (TSBuffer *) param;
  int i, element, sts;

  for (i=0; i<50; ++i) {
    sts = removeItem(buffer, &element);
    if (sts == 0)
      printf("runner #2 removed %d\n", element);
    else
      printf("runner #2 failed trying to remove\n");
  }

  pthread_exit(NULL);
}

int main() {
  TSBuffer buffer;
  pthread_t tid1, tid2;

  buffer.in = 0;
  buffer.out = 0;
  buffer.counter = 0;
  pthread_mutex_init(&buffer.lock, NULL);

  pthread_create(&tid1, NULL, runnerOne, &buffer);  
  pthread_create(&tid2, NULL, runnerTwo, &buffer);  

  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);

  return 0;
}
