#include <stdio.h>

void forkDemo();
void openDemo();
void fopenDemo();
void threadDemo();

int main(int ac, char **av)
{
    printf("From main.cpp!!!\n");

    forkDemo();
    openDemo();
    fopenDemo();
    threadDemo();

    return 0;
}
