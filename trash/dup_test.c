#include "parser.h"


/*
** This can't work if we can't flush stdout :( 
*/

int main(void)
{
    int fd;
    int dup_stdout;

    fd = open("trash.txt", O_RDWR | O_CREAT);
    dup_stdout = dup(1);
    dup2(fd, 1);
    puts("I HAVE CRIPPLING DEPRESSION !!!!!!!!\n");
    dup2(dup_stdout, 1);
    close(dup_stdout);
    puts("I ALSO WANT TO DIE!!!\n");
    return (0);
}