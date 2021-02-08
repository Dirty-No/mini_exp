#include "parser.h"

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