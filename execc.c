/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execc.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaccary <smaccary@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/01 13:16:41 by smaccary          #+#    #+#             */
/*   Updated: 2021/02/02 16:24:49 by smaccary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int			tokens_len(char **tokens)
{
	char **current;

	current = tokens;
	while (*current)
		current++;
	return (current - tokens);
}

static void	dup2_check(int fd_src, int fd_dst)
{
	if (fd_src != fd_dst)
	{
		dup2(fd_src, fd_dst);
		close(fd_src);
	}
}

int			exec_command(t_command *command)
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
/*
int	fork_pipes (int n, t_command *cmd)
{
  int	i;
  pid_t	pid;
  int	in;
  int	fd [2];

  in = 0;
  for (i = 0; i < n - 1; ++i)
    {
      pipe(fd);
      spawn_proc (in, fd [1], cmd + i);
      close (fd [1]);
      in = fd [0];
    }

  if (in != 0)
    dup2 (in, 0);

  return execve (cmd[i].argv [0], (char * const *)cmd[i].argv);
}
*/


int main(void)
{
	char *tokens[] = {NULL};

	printf("len:%d\n", tokens_len(tokens));
	return (0);
}

/*
int main(void)
{
	t_command	command;
	pid_t		pid;

	command.argv = (char *[]){"cat", NULL};
	command.cmd = "/bin/cat";
	command.fd_input = 0;
	command.fd_output = 1;
	pid = exec_command(&command);
	while (kill(pid, 0) == 0)
		;
	return (0);
}
*/