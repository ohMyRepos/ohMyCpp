#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <semaphore.h>

void Pthread_mutex_lock(pthread_mutex_t *m) {
    int rc = pthread_mutex_lock(m);
    assert(rc == 0);
}

void Pthread_mutex_unlock(pthread_mutex_t *m) {
    int rc = pthread_mutex_unlock(m);
    assert(rc == 0);
}

void Pthread_cond_wait(pthread_cond_t *c, pthread_mutex_t *m) {
    int rc = pthread_cond_wait(c, m);
    assert(rc == 0);
}

void Pthread_cond_signal(pthread_cond_t *c) {
    int rc = pthread_cond_signal(c);
    assert(rc == 0);
}

const int bufMAX = 10;
int buffer[bufMAX];
int fill = 0;
int use = 0;
int count = 0;

void put(int v) {
    buffer[fill] = v;
    fill = (fill + 1) % bufMAX;
    count++;
}

int get() {
    int tmp = buffer[use];
    use = (use + 1) % bufMAX;
    count--;
    return tmp;
}

pthread_mutex_t mutex;
sem_t empty_cond, fill_cond;

typedef struct producerStruct {
    int loops;
    char *name;
} producerStruct_t;

typedef struct consumerStruct {
    int loops;
    char *name;
} consumerStruct_t;

void *produceWorker(void *arg) {
    producerStruct_t *ps = (producerStruct_t *)arg;
    int loops = ps->loops;
    char *name = ps->name;
    for(int i = 1; i <= loops; i++) {
        Pthread_mutex_lock(&mutex);
        sem_wait(&empty_cond);
        put(i);
        sem_post(&fill_cond);
        Pthread_mutex_unlock(&mutex);
        printf("%s produced: %d\n", name, i);
    }
}

void *consumeWorker(void *arg) {
    consumerStruct_t *cs = (consumerStruct_t *)arg;
    int loops = cs->loops;
    char *name = cs->name;
    for(int i = 1; i <= loops; i++) {
        Pthread_mutex_lock(&mutex);
        sem_wait(&fill_cond);
        int tmp = get();
        sem_post(&empty_cond);
        Pthread_mutex_unlock(&mutex);
        printf("%s consumed: %d\n", name, tmp);
    }
}

void pNcDemo() {
    printf("producerConsumerDemo:\n");

    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty_cond, 0, bufMAX);
    sem_init(&fill_cond, 0, bufMAX);

    pthread_t producer1, producer2;
    pthread_t consumer1, consumer2;
    int consumeLoops = 100;
    int produceLoops = consumeLoops * 1;
    producerStruct_t ps1 = {
        .loops = produceLoops,
        .name = "producer1",
    };
    producerStruct_t ps2 = {
        .loops = produceLoops,
        .name = "producer2",
    };
    consumerStruct_t cs1 = {
        .loops = consumeLoops,
        .name = "consumer1",
    };
    consumerStruct_t cs2 = {
        .loops = consumeLoops,
        .name = "consumer2",
    };
    pthread_create(&producer1, NULL, produceWorker, (void *)&ps1);
    pthread_create(&producer2, NULL, produceWorker, (void *)&ps2);
    pthread_create(&consumer1, NULL, consumeWorker, (void *)&cs1);
    pthread_create(&consumer2, NULL, consumeWorker, (void *)&cs2);
    pthread_join(producer1, NULL);
    pthread_join(producer2, NULL);
    pthread_join(consumer1, NULL);
    pthread_join(consumer2, NULL);

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty_cond);
    sem_destroy(&fill_cond);
}