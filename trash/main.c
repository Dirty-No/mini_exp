#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int main(int ac, char **av)
{
    int fd;

    (void)ac;
    //fd = open(av[1], O_WRONLY | O_CREAT, 644);
    fopen(av[1], "wb");
    //write(fd, NULL, 0);
    printf("fd : %d\n", fd);
  //  close(fd);
    return (0);
}