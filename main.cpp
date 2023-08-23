#include <stdio.h>

void forkDemo();
void openDemo();
void fopenDemo();

int main(int ac, char **av)
{
    printf("From main.cpp!!!\n");

    forkDemo();
    openDemo();
    fopenDemo();

    return 0;
}
