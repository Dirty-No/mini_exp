/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execc.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaccary <smaccary@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/01 13:16:41 by smaccary          #+#    #+#             */
/*   Updated: 2021/02/04 16:35:11 by smaccary         ###   ########.fr       */
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

t_command	*new_command(char *cmd, char **argv, int fd_in, int fd_out)
{
	t_command	*new;

	new = malloc(sizeof(t_command));
	*new = (t_command){cmd, argv, fd_in, fd_out};
	return (new);
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

int			is_sep(char *token)
{
	ssize_t i;

	i = -1;
	while (SEPARATORS[++i])
	{
		if (ft_strcmp(SEPARATORS[i], token) == 0)
			return (1);
	}
	return (0);
}

int		count_cmd(char **tokens)
{
	int		count;
	char	**current;

	current = tokens;
	count = 1;
	while (*current)
	{
		if (is_sep(*current))
			count++;
		current++;	
	}
	return (count);
}


t_command	*get_next_command(char **tokens)
{
	static char	**current = NULL;
	static char **tokens_start = NULL;
	
	if (tokens != tokens_start)
	{
		tokens_start = tokens;
		current = tokens;
	}
	return (NULL);
}

t_list	*parse_list(char **tokens)
{
	t_command	*command;
	t_list		*lst;
	
	lst = NULL;
	while ((command = get_next_command(&tokens)))
		ft_lstadd_back(&lst, command);
	return (lst);
}



/*
t_list *get_command_list(char **tokens)
{
	t_list 		*lst;
	t_command	*cmd;
	
	
}
*/

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

/*
int main(void)
{
	char *tokens[] = {NULL};

	printf("len:%d\n", tokens_len(tokens));
	return (0);
}
*/

int main(void)
{
	char *tokens[] = {"echo", "hello", "world", "|", "grep", "world", NULL};
	char *tokens1[] = {"echo", "hello", "world", NULL};

	printf("cmd count: %d\n", count_cmd(tokens1));
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
