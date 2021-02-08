#include "parser.h"

static void
	dup2_check(int fd_src, int fd_dst)
{
	if (fd_src != fd_dst)
	{
		dup2(fd_src, fd_dst);
		close(fd_src);
	}
}

int
	exec_command(t_command *command)
{
	extern char **environ;
	pid_t	    pid;
	
	pid = fork();
	if (pid == 0)
	{
		dup2_check(command->fd_input, 0);
		dup2_check(command->fd_output, 1);
		pid = execve(command->cmd, command->argv, environ);
	}
	return (pid);
}