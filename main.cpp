#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

int main(int ac, char **av)
{
    unsigned long *p1;
    // unsigned double *p2;
    p1 = (unsigned long *) 0x800100;
    // p2 = (unsigned double *) 0x800110;
    p1 += 2;

    return 0;
}
