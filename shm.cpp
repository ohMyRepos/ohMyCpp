#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/wait.h>

typedef struct ipcStruct {
    pthread_mutex_t mutex;
    int count;
} ipcStruct_t;

void procWorker(ipcStruct_t *ipc, bool doLock) {
    for (int i = 0; i < 1e6; i++) {
        if (doLock) pthread_mutex_lock(&ipc->mutex);
        ipc->count++;
        if (doLock) pthread_mutex_unlock(&ipc->mutex);
    }
}

void shmDemo() {
    printf("shmDemo:\n");

    int shmid = shmget(IPC_PRIVATE, sizeof(ipcStruct_t), IPC_CREAT | 0600);
    ipcStruct_t *ipc = (ipcStruct_t *)shmat(shmid, NULL, 0);

    pthread_mutexattr_t mattr;
    pthread_mutexattr_init(&mattr);
    pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&ipc->mutex, &mattr);

    int childNum = 5;
    for (int i = 0; i < childNum; i++) {
        int pid = fork();
        if (pid == 0) {
            procWorker(ipc, true);
            exit(0);
        }
    }

    for (int i = 0; i < childNum; i++) {
        waitpid(-1, NULL, 0);
    }

    printf("final count: %d\n", ipc->count);
    pthread_mutex_destroy(&ipc->mutex);

    shmdt(ipc);
    shmctl(shmid, IPC_RMID, NULL);
}