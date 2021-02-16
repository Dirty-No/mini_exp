/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/28 14:57:30 by smaccary          #+#    #+#             */
/*   Updated: 2021/02/16 22:23:22 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <signal.h>
# include <string.h>
# include "libft.h"

# define REDIR_REPLACE ">"
# define REDIR_APPEND ">>"
# define REDIR_INPUT "<"
# define PIPE "|"
# define SEMICOLON ";"

# define SEPARATORS (char *[]){REDIR_REPLACE, REDIR_APPEND, REDIR_INPUT, PIPE, SEMICOLON, NULL}
# define REDIRECTS (char *[]){REDIR_REPLACE, REDIR_APPEND, REDIR_INPUT, NULL}
# define OUTPUT_REDIRECTS (char *[]){REDIR_REPLACE, REDIR_APPEND, NULL}
# define INPUT_REDIRECTS (char *[]){REDIR_INPUT, NULL}

# define OUTPUT_REPLACE_MASK 0b001
# define OUTPUT_APPEND_MASK  0b010
# define INPUT_REDIRECT_MASK 0b100

# ifndef DEBUG
#  define DEBUG 0
# endif

typedef struct	s_command
{
	char	*cmd;
	char	**argv;
	char	*sep;
	int		fd_output;
	int		fd_input;
}				t_command;

size_t			get_argv_len(char **tokens);
size_t			tab_size(char **table);
t_command		*new_command(char *cmd, char **argv, char *sep);
t_command		*command_from_argv(char **argv, char *sep);
char			**find_token(char *token, char **tokens_table);
char			**find_last_token(char *token, char **tokens_table);
char			**tab_find_last_token(char **tokens, char **tokens_table);
char			**get_pure_tokens(char **tokens);
int				is_sep(char *token);
char			**find_sep(char **tokens);
char			**dup_n_tab(char **table, size_t n);

t_list			*parse_list(char **tokens);
char			**extract_redirects(char **tokens);

int				redirects_to_fds(char **redirects, int *fd_input, int *fd_output);
	
void			print_cmd_lst(t_list *lst);
void			print_command(t_command *command);
void			print_argv(char **argv);
#endif
