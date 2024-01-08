#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <windows.h>

#define BUFFER_SIZE 5
#define MAX_ITEMS 10

int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;
int produced_count = 0;
int consumed_count = 0;

sem_t mutex;   // Semaphore to ensure synchronization between processes
sem_t full;    // Semaphore to count the number of items produced and not yet consumed in the buffer
sem_t empty;   // Semaphore to count the remaining empty slots in the buffer

// Variables to track the number of empty slots and items in the buffer
int empty_slots = BUFFER_SIZE;
int items_in_buffer = 0;

void* producer(void* arg) {
   int item = 1;

   while (produced_count < MAX_ITEMS) {
      if (empty_slots == 0) {
         printf("Producer waiting: Buffer is full\n");
      }
      sem_wait(&empty);   // Wait if there are no empty slots
      sem_wait(&mutex);   // Ensure no other process changes the buffer simultaneously

      buffer[in] = item;
      printf("Produced: %d\n", item);
      item++;
      in = (in + 1) % BUFFER_SIZE;

      produced_count++;
      items_in_buffer++;
      empty_slots--;

      sem_post(&mutex);   // Release the lock to allow other threads to execute
      sem_post(&full);    // Increase the count of items produced and not yet consumed
   }

   pthread_exit(NULL);
}

void* consumer(void* arg) {
   Sleep(1000);
   while (consumed_count < MAX_ITEMS) {
      if (items_in_buffer == 0) {
         printf("Consumer waiting: Buffer is empty\n");
      }
      sem_wait(&full);    // Wait if there are no items to consume
      sem_wait(&mutex);   // Ensure no other process changes the buffer simultaneously

      int item = buffer[out];
      printf("Consumed: %d\n", item);
      out = (out + 1) % BUFFER_SIZE;

      consumed_count++;
      items_in_buffer--;
      empty_slots++;

      sem_post(&mutex);   // Release the lock to allow other threads to execute
      sem_post(&empty);   // Increase the count of remaining empty slots in the buffer
   }

   pthread_exit(NULL);
}

int main() {
   pthread_t producerThread, consumerThread;

   sem_init(&mutex, 0, 1);
   sem_init(&full, 0, 0);
   sem_init(&empty, 0, BUFFER_SIZE);

   pthread_create(&producerThread, NULL, producer, NULL);
   pthread_create(&consumerThread, NULL, consumer, NULL);

   pthread_join(producerThread, NULL);
   pthread_join(consumerThread, NULL);

   sem_destroy(&mutex);
   sem_destroy(&full);
   sem_destroy(&empty);

   return 0;
}
