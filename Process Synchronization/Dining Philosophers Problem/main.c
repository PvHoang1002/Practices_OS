#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <windows.h>

#define N 5 // Number of philosophers
#define MAX_MEALS 1 // Number of meals each philosopher will eat

sem_t forks[N]; // Semaphores representing the forks
sem_t meal_counter; // Semaphore to count the number of meals
int meals_eaten[N] = {0}; // Array to track the number of meals eaten by each philosopher
int completed_philosophers = 0; // Counter to track the number of philosophers who have completed
int deadlock_detected[N] = {0}; // Flag to track whether deadlock is detected

void *philosopher(void *arg) {
    int id = *(int *)arg;
    int left_fork = id;
    int right_fork = (id + 1) % N;

    while (meals_eaten[id] < MAX_MEALS) {
        printf("Philosopher %d is thinking\n", id);

        printf("Philosopher %d is trying to pick up fork %d\n", id, left_fork);
        sem_wait(&forks[left_fork]);

        printf("Philosopher %d is trying to pick up fork %d\n", id, right_fork);
        if (sem_trywait(&forks[right_fork]) != 0) {
            // Deadlock detected
            deadlock_detected[id] = 1;
            printf("Deadlock detected! Philosopher %d is waiting for fork %d\n", id, right_fork);
            // break; // Exit the loop to prevent further deadlock checks
            while (sem_trywait(&forks[left_fork]) != 0) {
                printf("Philosopher %d is still thinking\n", id);
                Sleep(1000);
            }
        }

        printf("Philosopher %d is eating\n", id);
        meals_eaten[id]++;

        printf("Philosopher %d is putting down fork %d\n", id, left_fork);
        sem_post(&forks[left_fork]);

        printf("Philosopher %d is putting down fork %d\n", id, right_fork);
        sem_post(&forks[right_fork]);

        // Increase the completed philosophers count
        sem_wait(&meal_counter);
        completed_philosophers++;
        sem_post(&meal_counter);
    }

    if (!deadlock_detected[id]) {
        printf("Philosopher %d completed all his meal(s)\n", id);
    } else {
        printf("Philosopher %d has left the table due to deadlock\n", id);
    }
}

int main() {
    pthread_t philosophers[N];
    int philosopher_ids[N];

    for (int i = 0; i < N; i++) {
        sem_init(&forks[i], 0, 1);
    }

    sem_init(&meal_counter, 0, 1);

    for (int i = 0; i < N; i++) {
        philosopher_ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &philosopher_ids[i]);
    }

    // Wait for threads to finish
    for (int i = 0; i < N; i++) {
        pthread_join(philosophers[i], NULL);
    }

    for(int i = 0; i < N; i++) {
        if(deadlock_detected[i]) {
            printf("The dinner ended due to deadlock!\n");
            break;
        } 
        if (i == N - 1 && !deadlock_detected[i]) {
            printf("All the philosophers have completed their meals!\n");
        }
    }

    return 0;
}






