/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/01 13:16:41 by smaccary          #+#    #+#             */
/*   Updated: 2021/02/16 22:50:18 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_command
	*new_command(char *cmd, char **argv, char *sep)
{
	t_command	*new;

	new = malloc(sizeof(t_command));
	*new = (t_command){cmd, argv, sep, 1, 0};
	return (new);
}

t_command
	*command_from_argv(char **argv, char *sep)
{
	return (new_command(ft_strdup(argv[0]), argv, sep));
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
		*fd_input = open(input_path_ptr[1], O_RDONLY);
		if (*fd_input < 0)
			printf("%s : %s: %s\n", SHELL_NAME, strerror(errno), input_path_ptr[1]);
	}
	return (mode);
}

sig_t blank(int a)
{
	(void)a;
	write(1, "\n", 1);
	return (NULL);
}
