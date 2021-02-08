/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execc.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaccary <smaccary@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/01 13:16:41 by smaccary          #+#    #+#             */
/*   Updated: 2021/02/08 14:01:16 by smaccary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

size_t
	tab_size(char **table)
{
	char **current;

	current = table;
	while (*current)
		current++;
	return (current - table);		
}

int
	tokens_len(char **tokens)
{
	char **current;

	current = tokens;
	while (*current)
		current++;
	return (current - tokens);
}

t_command
	*new_command(char *cmd, char **argv, char *sep)
{
	t_command	*new;

	new = malloc(sizeof(t_command));
	*new = (t_command){cmd, argv, sep, 0, 1};
	return (new);
}

t_command
	*command_from_argv(char **argv, char *sep)
{
	return (new_command(ft_strdup(argv[0]), argv, sep));
}

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

int			is_sep(char *token)
{
	ssize_t i;

	i = -1;
	while (SEPARATORS[++i])
	{
		if (!token || ft_strcmp(SEPARATORS[i], token) == 0)
			return (1);
	}
	return (0);
}

char
	**dup_n_tab(char **table, size_t n)
{
	char	**dup;
	int		size;
	int		i;

	size = tab_size(table);
	if (n < size)
		size = n;
	dup = ft_calloc(sizeof(char *), size + 1);
	i = -1;
	while (++i < size)
		dup[i] = ft_strdup(table[i]);
	return (dup);
}

int
	count_cmd(char **tokens)
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

char
	**find_sep(char **tokens)
{
	char **current;
	
	current = tokens;
	while (!is_sep(*current))
		current++;
	return (current);
}

int
	get_argv_len(char **tokens)
{
	return (find_sep(tokens) - tokens);
}

t_command
	*get_next_command(char **tokens)
{
	static char	**current = NULL;
	static char **tokens_start = NULL;
	int			end;
	t_command	*command;

	if (tokens != tokens_start)
	{
		tokens_start = tokens;
		current = tokens;
	}
	if (current == NULL || *current == NULL)
		return (NULL);
	end = get_argv_len(current);
	command = command_from_argv(dup_n_tab(current, end), *find_sep(tokens));
	current += end;
	if (*current)
		current++;
	return (command);
}

t_list	*parse_list(char **tokens)
{
	t_command	*command;
	t_list		*lst;
	
	lst = NULL;
	while ((command = get_next_command(tokens)))
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

void	print_argv(char **argv)
{
	printf("%p -> ", argv);
	if (argv && *argv)
	{
		printf("%s", "{");
		while (*argv)
		{
			printf("%s", *argv);
			argv++;
			if (*argv)
				printf("%s", ", ");
		}
		printf("%s", "}\n");
	}	
}

void	print_command(t_command *command)
{
	printf("%p:\n", command);
	if (command)
	{
		printf("cmd: %s\n", command->cmd);
		printf("%s", "argv: ");
		print_argv(command->argv);
		printf("input: %d\noutput: %d\n", command->fd_input, command->fd_output);
		printf("sep: %s\n\n", command->sep);
	}
}

void	print_cmd_lst(t_list *lst)
{
	ft_lstiter(lst, print_command);
}

int main(void)
{
	char *tokens_pipe[] = {"echo", "hello", "world", "|", "grep", "world", NULL};
	char *tokens_redir[] = {"echo", "hello", "world", ">", "text.txt", NULL};
	char *tokens1[] = {"echo", "hello", "world", NULL};
	t_list	*lst;

	//printf("cmd count: %d\n", count_cmd(tokens1));
	lst = parse_list(tokens_redir);
	print_cmd_lst(lst);
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
