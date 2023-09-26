#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

void openDemo() {
    printf("openDemo:\n");

    const char *fileName = "openDemo.txt";
    int fw = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
    char *s = "some string";
    write(fw, s, strlen(s));
    close(fw);

    char buf[1024] = {0};
    int fr = open(fileName, O_RDONLY, S_IRWXU);
    int cnt = read(fr, &buf, sizeof(buf));
    printf("read %d characters: %s\n", cnt, buf);
    close(fr);
}

void fopenDemo() {
    printf("fopenDemo:\n");

    const char *fileName = "fopenDemo.txt";
    FILE *fw = fopen(fileName, "wb");
    fprintf(fw, "some string");
    fclose(fw);

    char buf[1024];
    memset(buf, NULL, sizeof buf);
    FILE *fr = fopen(fileName, "rb");
    int cnt = fread(&buf, sizeof(buf[0]), sizeof(buf), fr);
    printf("read %d characters: %s\n", cnt, buf);
    fclose(fr);
}