#include <stdio.h>

void forkDemo();
void openDemo();
void fopenDemo();
void threadDemo();
void lockDemo();
void condDemo();

int main(int ac, char **av)
{
    printf("From main.cpp!!!\n");

    forkDemo();
    openDemo();
    fopenDemo();
    threadDemo();
    lockDemo();
    condDemo();

    return 0;
}
