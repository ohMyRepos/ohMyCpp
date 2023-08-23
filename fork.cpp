#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void forkDemo() {
    printf("forkDemo:\n");

    int pid = fork();
    if (pid <0) {
        printf("fork error\n");
        exit(1);
    } else if (pid == 0) {
        printf("I'm Child!\n");
        exit(99);
    } else {
        int status = 0;
        int wpid = waitpid(pid, &status, 0);
        printf("I'm Parent! cpid %d, wpid %d, child exit code: %d\n", pid, wpid, WEXITSTATUS(status));
    }
}