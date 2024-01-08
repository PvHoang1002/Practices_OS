#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_READERS 5

int shared_data = 0;
int readers_count = 0;

sem_t mutex; 
sem_t wrt;

void* writer(void* arg) {
    while (1) {
        sem_wait(&wrt); // Wait for writer's turn to write
        printf("Writer is writing...\n");
        shared_data++;
        printf("Writer has finished writing. Shared data: %d\n", shared_data);
        sem_post(&wrt); // Release writer semaphore
        sleep(1);
    }
}

void* reader(void* arg) {
    int reader_id = *(int*)arg;
    while (1) {
        sem_wait(&mutex); // Gain access to readers_count
        readers_count++;
        if (readers_count == 1) {
            sem_wait(&wrt); // If the first reader, block writers
        }
        sem_post(&mutex); // Release mutex for readers_count

        printf("Reader %d is reading. Shared data: %d\n", reader_id, shared_data);
        
        sem_wait(&mutex); // Gain access to readers_count
        readers_count--;
        if (readers_count == 0) {
            sem_post(&wrt); // If the last reader, unblock writers
        }
        sem_post(&mutex); // Release mutex for readers_count

        sleep(2); // Reading time
    }
}

int main() {
    pthread_t writer_thread;
    pthread_t reader_threads[MAX_READERS];
    int reader_ids[MAX_READERS];

    sem_init(&mutex, 0, 1); 
    sem_init(&wrt, 0, 1);   

    // Create writer thread
    if (pthread_create(&writer_thread, NULL, writer, NULL) != 0) {
        perror("Failed to create writer thread");
        return 1;
    }

    // Create reader threads
    for (int i = 0; i < MAX_READERS; i++) {
        reader_ids[i] = i + 1;
        if (pthread_create(&reader_threads[i], NULL, reader, &reader_ids[i]) != 0) {
            perror("Failed to create reader thread");
            return 1;
        }
    }

    // Wait for threads to finish (this will never happen since threads run indefinitely)
    pthread_join(writer_thread, NULL);
    for (int i = 0; i < MAX_READERS; i++) {
        pthread_join(reader_threads[i], NULL);
    }

    sem_destroy(&mutex);
    sem_destroy(&wrt);

    return 0;
}