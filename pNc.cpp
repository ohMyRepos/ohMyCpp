#include <stdio.h>
#include <pthread.h>
#include <assert.h>

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
pthread_cond_t empty_cond, fill_cond;

typedef struct consumerStruct {
    int loops;
    char *name;
} consumerStruct_t;

void *produceWorker(void *arg) {
    int loops = *((int *)arg);
    for(int i = 1; i <= loops; i++) {
        Pthread_mutex_lock(&mutex);
        while(count == bufMAX) {
            Pthread_cond_wait(&empty_cond, &mutex);
        }
        put(i);
        Pthread_cond_signal(&fill_cond);
        Pthread_mutex_unlock(&mutex);
        printf("produced: %d\n", i);
    }
}

void *consumeWorker(void *arg) {
    consumerStruct_t *cs = (consumerStruct_t *)arg;
    int loops = cs->loops;
    char *name = cs->name;
    for(int i = 1; i <= loops; i++) {
        Pthread_mutex_lock(&mutex);
        while(count == 0) {
            Pthread_cond_wait(&fill_cond, &mutex);
        }
        int tmp = get();
        Pthread_cond_signal(&empty_cond);
        Pthread_mutex_unlock(&mutex);
        printf("%s consumed: %d\n", name, tmp);
    }
}

void pNcDemo() {
    printf("producerConsumerDemo:\n");

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&empty_cond, NULL);
    pthread_cond_init(&fill_cond, NULL);

    pthread_t producer;
    pthread_t consumer1, consumer2;
    int consumeLoops = 100;
    int produceLoops = consumeLoops * 2;
    consumerStruct_t cs1 = {
        .loops = consumeLoops,
        .name = "consumer1",
    };
    consumerStruct_t cs2 = {
        .loops = consumeLoops,
        .name = "consumer2",
    };
    pthread_create(&producer, NULL, produceWorker, (void *)&produceLoops);
    pthread_create(&consumer1, NULL, consumeWorker, (void *)&cs1);
    pthread_create(&consumer2, NULL, consumeWorker, (void *)&cs2);
    pthread_join(producer, NULL);
    pthread_join(consumer1, NULL);
    pthread_join(consumer2, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&empty_cond);
    pthread_cond_destroy(&fill_cond);
}