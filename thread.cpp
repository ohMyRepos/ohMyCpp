#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *printWorker(void *arg){
    int stack1, stack2;
    char c = *((char *)arg);
    printf("arg: %c, stack1: %p, stack2: %p\n", c, &stack1, &stack2);

    int *ret = (int *) malloc(sizeof(int));
    *ret = 999;
    return (void *)ret;
}

void threadDemo() {
    printf("threadDemo:\n");

    pthread_t t1, t2;
    pthread_create(&t1, NULL, printWorker, (void *)"A");
    pthread_create(&t2, NULL, printWorker, (void *)"B");
    int *ret;
    pthread_join(t1, (void **)&ret);
    printf("t1 ret: %d\n", *ret);
    pthread_join(t2, (void **)&ret);
    printf("t2 ret: %d\n", *ret);

    char startC = 'C';
    char endC = 'G';
    char c[26];
    pthread_t t[26];
    for (char i = startC; i <= endC; i++) {
        char offset = i - startC;
        c[i-startC] = i;
        pthread_create(&t[i-startC], NULL, printWorker, &c[offset]);
    }
    for (char i = startC; i <= endC; i++)
    {
        pthread_join(t[i-startC], NULL);
    }
}

typedef struct countStruct {
    pthread_mutex_t *lock;
    volatile int cnt;
} countStruct;

void *countWorkerNoLock(void *arg) {
    for (int i = 0; i < 1e7; i++) {
        (*(int *)arg)++;
    }
}

void *countWorkerUseLock(void *arg) {
    countStruct *cs = (countStruct *)arg;
    pthread_mutex_t *lock = cs->lock;
    volatile int cnt = cs->cnt;

    for (int i = 0; i < 1e7; i++) {
        pthread_mutex_lock(lock);
        cs->cnt++;
        pthread_mutex_unlock(lock);
    }
}

void lockDemo() {
    printf("lockDemo:\n");

    volatile int cnt = 0;
    pthread_t t1, t2;
    pthread_create(&t1, NULL, countWorkerNoLock, (void *)&cnt);
    pthread_create(&t2, NULL, countWorkerNoLock, (void *)&cnt);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("no lock, expect cnt: %d, real cnt: %d, diff: %d\n", (int)2e7, cnt, (int)2e7-cnt);

    pthread_mutex_t lock;
    pthread_mutex_init(&lock, NULL);
    countStruct cs = {
        .lock = &lock,
        .cnt = 0,
    };
    pthread_create(&t1, NULL, countWorkerUseLock, (void *)&cs);
    pthread_create(&t2, NULL, countWorkerUseLock, (void *)&cs);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_mutex_destroy(&lock);
    printf("use lock, expect cnt: %d, real cnt: %d, diff: %d\n", (int)2e7, cs.cnt, (int)2e7-cs.cnt);
}

typedef struct waitStruct
{
    pthread_mutex_t *lock;
    pthread_cond_t *cond;
    bool ready;
} waitStruct;

void *waitWorker(void *arg) {
    waitStruct *ws = (waitStruct *)arg;
    pthread_mutex_t *lock = ws->lock;
    pthread_cond_t *cond = ws->cond;

    printf("enter waitWorker\n");
    pthread_mutex_lock(lock);
    while (!ws->ready) {
        printf("before waitWorker while\n");
        // pthread_cond_wait() will release lock here
        pthread_cond_wait(cond, lock);
        // after signal, code will regain lock here
        printf("after waitWorker while\n");
    }
    pthread_mutex_unlock(lock);
    printf("exit waitWorker\n");
}

void *signalWorker(void *arg) {
    waitStruct *ws = (waitStruct *)arg;
    pthread_mutex_t *lock = ws->lock;
    pthread_cond_t *cond = ws->cond;

    printf("enter signalWorker\n");
    pthread_mutex_lock(lock);
    ws->ready = true;
    printf("before signal\n");
    pthread_cond_signal(cond);
    printf("after signal\n");
    pthread_mutex_unlock(lock);
    printf("sleep in signalWorker\n");
    sleep(0.5);
    printf("exit signalWorker\n");
}

void condDemo() {
    printf("condDemo:\n");

    pthread_mutex_t lock;
    pthread_cond_t cond;
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_t t1, t2;
    waitStruct ws = {
        .lock = &lock,
        .cond = &cond,
        .ready = false,
    };
    pthread_create(&t1, NULL, waitWorker, (void *)&ws);
    pthread_create(&t2, NULL, signalWorker, (void *)&ws);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);
}