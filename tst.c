#include "parser.h"

int main(void)
{
    int fd;
    pid_t   pid;

    pid = fork();
    if (pid == 0)
    {
        fd = open("trash.txt", O_RDWR | O_CREAT);
        dup2(fd, 1);
        close(fd);
        printf("hello world!\r\n");
    //    fflush(stdout);
    }
    printf("wesh le 100\n");
    return (0);
}