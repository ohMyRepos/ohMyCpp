#include <stdio.h>

void forkDemo();
void openDemo();
void fopenDemo();
void threadDemo();
void lockDemo();
void condDemo();
void pNcDemo();
void philosopherDemo();
void shmMutexDemo();
void shmSemphoreDemo();

int main(int ac, char **av) {
    printf("From main.cpp!!!\n");

    // forkDemo();
    // openDemo();
    // fopenDemo();
    // threadDemo();
    // lockDemo();
    // condDemo();
    // pNcDemo();
    // philosopherDemo();
    shmMutexDemo();
    shmSemphoreDemo();

    return 0;
}
