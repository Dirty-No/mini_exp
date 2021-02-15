/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaccary <smaccary@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/01 13:16:41 by smaccary          #+#    #+#             */
/*   Updated: 2021/02/15 14:05:51 by smaccary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

size_t
	get_argv_len(char **tokens);

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

char
	**find_token(char *token, char **tokens_table)
{
	ssize_t i;

	i = -1;
	if (!token)
		return (tokens_table + get_argv_len(tokens_table));
	while (tokens_table[++i])
	{
		if (ft_strcmp(tokens_table[i], token) == 0)
			return (tokens_table + i);
	}
	return (NULL);
}

char
	**find_last_token(char *token, char **tokens_table)
{
	ssize_t i;
	char	**last;

	i = -1;
	last = NULL;
	if (!token)
		return (tokens_table + get_argv_len(tokens_table));
	while (tokens_table[++i])
	{
		if (ft_strcmp(tokens_table[i], token) == 0)
			last = tokens_table + i;
	}
	return (last);
}

char
	**tab_find_last_token(char **tokens, char **tokens_table)
{
	ssize_t i;
	char	**last;
	char	**current;

	i = -1;
	last = NULL;
	while (tokens[++i])
	{
		current = find_last_token(tokens[i], tokens_table);
		if (current > last)
			last = current;
	}
	return (last);
}

int
	is_sep(char *token)
{
	return ((int)find_token(token, SEPARATORS));
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

size_t
	get_argv_len(char **tokens)
{
	return (find_sep(tokens) - tokens);
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
	fflush(stdout);
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
	fflush(stdout);
}

void
	print_cmd_lst(t_list *lst)
{
	ft_lstiter(lst, (void *)print_command);
}

int	is_redirect(char *token)
{
	return ((int)find_token(token, REDIRECTS));
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

char
	**ref_in_tokens(char *ref, char **tokens)
{
	while (*tokens)
	{
		if (ref == *tokens)
			return (tokens);
		tokens++;
	}
	return (NULL);		
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


/*
** Parse redirects tokens, opens files and assign them accordingly to fd_input/output ( input : "<", output: ">") 
** Returns redirection status as a binary mask (replace = 001 | append = 010 | input = 100)
*/

int
	redirects_to_fds(char **redirects, int *fd_input, int *fd_output)
{
	char	**input_path_ptr;
	char	**output_path_ptr;
	int		mode;
	int		open_mode;
	

	output_path_ptr = tab_find_last_token(OUTPUT_REDIRECTS, redirects);
	mode = 0;
	open_mode = O_WRONLY | O_CREAT;
	if (output_path_ptr && *output_path_ptr && *(output_path_ptr + 1) && !is_sep(*(output_path_ptr + 1)))
	{
		mode |= (int)ft_strlen(*output_path_ptr);
		if (mode == 1)
			open_mode |= O_TRUNC;
		else if (mode == 2)
			open_mode |= O_APPEND;
		*fd_output = open(*(output_path_ptr + 1), open_mode, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	}
	input_path_ptr = tab_find_last_token(INPUT_REDIRECTS, redirects);
	if (input_path_ptr && *input_path_ptr && *(input_path_ptr + 1) && !is_sep(*(input_path_ptr + 1)))
	{
		mode |= INPUT_REDIRECT_MASK;
		*fd_input = open(*(input_path_ptr + 1), O_RDONLY);
	}
	return (mode);
}

sig_t blank(int a)
{
	(void)a;
	write(1, "\n", 1);
	return (NULL);
}

int
	main(int ac, char *argv[])
{
	//char *tokens_pipe[] = {"echo", "hello", "world", "|", "grep", "world", NULL};
	char *tokens_redir[] = {"echo", "hello", "world", ">>", "text.txt", NULL};
	//char *tokens_mult_redir[] = {"echo", "hello", "world", ">", "text.txt", ">", "text2.txt", ">", "text3.txt", "<", "input.txt", NULL};
	//char *tokens_redir_middle[] = {"echo", "hello", ">", "fuck me", "<", "kill me", "world", ">", "text.txt", ">", "text2.txt", ">", "text3.txt", "<", "input.txt", NULL};

	//char *tokens_mega[] = {"echo", "hello", "world", "|", "grep", "world", "|", "grep", "-o", "wo", ">", "text.txt", NULL};
	//char *tokens_mega1[] = {"echo", "hello", "world", "|", "grep", "world", "|", "grep", "-o", "wo", ">", "text1.txt", ">", "text2.txt", ">", "text3.txt", NULL};
	//char *tokens1[] = {"echo", "hello", "world", NULL};
	t_list	*lst;
	char	**tokens;
	char	**pure_tokens;
	char	**redirections;
	int		fd_input = -2;
	int		fd_output = -2;
	
	(void)ac;
	(void)argv;
	
	tokens = argv + 1;
	//tokens = tokens_redir;
	pure_tokens = get_pure_tokens(tokens);
	lst = parse_list(pure_tokens);
	print_cmd_lst(lst);

	redirections = extract_redirects(tokens);
	
	print_argv(redirections);
	
	redirects_to_fds(redirections, &fd_input, &fd_output);

	dup2(fd_output, 1);
	printf("hello world\n");
	close(fd_output);
	return (0);
}

/*
int main(void)
{
	t_command	command;
	pid_t		pid;

	command.argv = (char *[]){"cat", NULL};
vi 	command.cmd = "/bin/cat";
	command.fd_input = 0;
	command.fd_output = 1;
	pid = exec_command(&command);
	while (kill(pid, 0) == 0)
		;
	return (0);
}
*/

