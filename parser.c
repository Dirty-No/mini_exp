/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/01 13:16:41 by smaccary          #+#    #+#             */
/*   Updated: 2021/02/11 18:31:21 by root             ###   ########.fr       */
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

int
	find_token(char *token, char **tokens_table)
{
	ssize_t i;

	i = -1;
	while (tokens_table[++i])
	{
		if (!token || ft_strcmp(tokens_table[i], token) == 0)
			return (1);
	}
	return (0);
}

int
	is_sep(char *token)
{
	return (find_token(token, SEPARATORS));
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
	command = command_from_argv(dup_n_tab(current, end), *find_sep(current));
	current += end;
	if (*current)
		current++;
	return (command);
}

t_list
	*parse_list(char **tokens)
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

void
	print_argv(char **argv)
{
	printf("%p -> ", argv);
	if (argv && *argv)
	{
		printf("%s", "{");
		while (*argv)
		{
			printf("\"%s\"", *argv);
			argv++;
			if (*argv)
				printf("%s", ", ");
		}
		printf("%s", "}\n");
	}	
}

void
	print_command(t_command *command)
{
	printf("%p:\n", command);
	if (command)
	{
		printf("  - %-10s\"%s\"\n", "cmd:", command->cmd);
		printf("  - %-10s", "argv:");
		print_argv(command->argv);
		printf("  - %-10s%d\n  - %-10s%d\n", "input:", command->fd_input, "output:", command->fd_output);
		printf("  - %-10s\"%s\"\n\n", "sep:", command->sep);
	}
}

void
	print_cmd_lst(t_list *lst)
{
	ft_lstiter(lst, (void *)print_command);
}

int	is_redirect(char *token)
{
	return (find_token(token, REDIRECTS));
}

size_t
	count_avoided_tokens(char **tokens)
{
	size_t	count;

	count = 0;
	while (*tokens)
	{
		if (is_redirect(*tokens))
			count += 2;
		tokens++;
	}
	return (count);
}

/*
** Extracts redirections and their arguments and returns them in a char **
** -> {">", "arg1", "<", "arg2", ">>", "arg3"} 
*/

char
	**extract_redirects(char **tokens)
{
	char	**extracted;
	size_t	i;

	i = 0;
	extracted = ft_calloc(count_avoided_tokens(tokens) + 1, sizeof(char *));
	if (!extracted)
		return (NULL);
	while (*tokens)
	{
		if (is_redirect(*tokens))
		{
			extracted[i] = *tokens;
			if (*(tokens + 1))
				extracted[++i] = *(++tokens);
			i++;
		}
		tokens++;
	}
	return (extracted);
}

size_t
	pure_tokens_len(char **tokens, char **excluded_tokens)
{
	char	**current;
	size_t	len;

	current = tokens;
	len = 0;
	while (*current)
	{
		if (find_token(*current, excluded_tokens) == 0)
			len++;
		current++;
	}
	return (len);
}

int
	ref_in_tokens(char *ref, char **tokens)
{
	while (*tokens)
	{
		if (ref == *tokens)
			return (1);
		tokens++;
	}
	return (0);		
}

/*
** Return tokens without redirections and their arguments 
*/

char
	**get_pure_tokens(char **tokens)
{
	char	**pure_tokens;
	char	**extracted;
	char	**current;
	size_t	i;

	i = 0;
	current = tokens;
	extracted = extract_redirects(tokens);
	pure_tokens = ft_calloc(pure_tokens_len(tokens, extracted) + 1, sizeof(char *));
	while (*current)
	{
		if (ref_in_tokens(*current, extracted) == 0)
		{
			pure_tokens[i] = *current;
			i++;
		}
		current++;
	}
	return (pure_tokens);
}

int
	main(int ac, char *argv[])
{
	//char *tokens_pipe[] = {"echo", "hello", "world", "|", "grep", "world", NULL};
	//char *tokens_redir[] = {"echo", "hello", "world", ">", "text.txt", NULL};
	//char *tokens_mega[] = {"echo", "hello", "world", "|", "grep", "world", "|", "grep", "-o", "wo", ">", "text.txt", NULL};
	//char *tokens_mega1[] = {"echo", "hello", "world", "|", "grep", "world", "|", "grep", "-o", "wo", ">", "text1.txt", ">", "text2.txt", ">", "text3.txt", NULL};
	char *tokens1[] = {"echo", "hello", "world", NULL};
	t_list	*lst;
	char	**tokens;
	char	**pure_tokens;
	
	tokens = argv + 1;
	//tokens = tokens1;
	pure_tokens = get_pure_tokens(tokens);
	lst = parse_list(pure_tokens);
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

