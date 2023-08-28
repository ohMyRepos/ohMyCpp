#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *worker(void *arg){
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
    pthread_create(&t1, NULL, worker, (void *)"A");
    pthread_create(&t2, NULL, worker, (void *)"B");
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
        pthread_create(&t[i-startC], NULL, worker, &c[offset]);
    }
    for (char i = startC; i <= endC; i++)
    {
        pthread_join(t[i-startC], NULL);
    }
}