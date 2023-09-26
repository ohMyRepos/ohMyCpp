#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

void randSleep(int range) {
    int rand_ms = rand() % range;
    usleep(rand_ms * range);
}

void randSleep() {
    randSleep(10);
}

const int pMax = 5;

sem_t forks[pMax];

int left(int p) { return p; }
int right(int p) { return (p + 1) % pMax; }

void think(int id) {
    printf("philosopher %d is thinking\n", id);
    randSleep();
}

void eat(int id) {
    printf("philosopher %d is eating\n", id);
    randSleep();
}

void getforks(int id) {
    int first;
    int second;
    if (id == 0) {
        first = right(id);
        second = left(id);
    } else {
        first = left(id);
        second = right(id);
    }
    printf("philosopher %d expect fork %d\n", id, first);
    sem_wait(&forks[first]);
    printf("philosopher %d got fork %d\n", id, first);
    printf("philosopher %d expect fork %d\n", id, second);
    sem_wait(&forks[second]);
    printf("philosopher %d got fork %d\n", id, second);
}

void putforks(int id) {
    int first = left(id);
    int second = right(id);
    sem_post(&forks[first]);
    printf("philosopher %d released fork %d\n", id, first);
    sem_post(&forks[second]);
    printf("philosopher %d released fork %d\n", id, second);
}

typedef struct _philosopher {
    int id;
} philosopher_t;

void *philosopherWorker(void *arg) {
    philosopher_t *p = (philosopher_t *)arg;
    int id = p->id;
    while (1) {
        think(id);
        getforks(id);
        eat(id);
        putforks(id);
    }
}

void philosopherDemo() {
    printf("philosopherDemo:\n");

    srand(time(NULL));

    for (int i = 0; i < pMax; i++) {
        sem_init(&forks[i], 0, 1);
    }

    pthread_t thrs[pMax];
    philosopher_t philosophers[pMax];
    for (int i = 0; i < pMax; i++) {
        philosophers[i] = {
            .id = i,
        };
        pthread_create(&thrs[i], NULL, philosopherWorker, (void *)&philosophers[i]);
    }

    for (int i = 0; i < pMax; i++) {
        pthread_join(thrs[i], NULL);
    }

    for (int i = 0; i < pMax; i++) {
        sem_destroy(&forks[i]);
    }
}