#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>

int doFork(char times[]) {
    int cid;
    switch (cid = fork()) {
    case 0:
        printf("forked-child %s times, pid: %d, ppid: %d\n", times, getpid(), getppid());
        break;
    default:
        printf("forked-parent, %s times, pid: %d, ppid: %d, cid: %d\n", times, getpid(), getppid(), cid);
        sleep(1);
        exit(0);
    }
    return cid;
}

void daemonize() {
    pid_t pid = getpid();
    pid_t ppid = getppid();
    printf("parent pid: %d, ppid: %d\n", pid, ppid);

    // 第一次 fork，创建子进程并让父进程退出
    pid_t _ = doFork("1st");

    // 忽略 SIGHUP 信号
    signal(SIGHUP, SIG_IGN);

    // 创建新会话
    setsid();

    // 第二次 fork，避免守护进程成为会话组长
    pid = doFork("2nd");

    // 清除文件权限掩码
    umask(0);
    int fd = open("/dev/null", O_RDWR);
    dup2(fd, STDIN_FILENO);
    dup2(fd, STDOUT_FILENO);

    if (fd > STDERR_FILENO) {
        close(fd);
    }

    while (true) {
        sleep(1);
    }
}