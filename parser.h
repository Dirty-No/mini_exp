/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaccary <smaccary@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/28 14:57:30 by smaccary          #+#    #+#             */
/*   Updated: 2021/02/12 16:52:38 by smaccary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include "libft.h"

#define REDIR_REPLACE ">"
#define REDIR_APPEND ">>"
#define REDIR_INPUT "<"
#define PIPE "|"
#define SEMICOLON ";"


#define SEPARATORS (char *[]){REDIR_REPLACE, REDIR_APPEND, REDIR_INPUT, PIPE, SEMICOLON, NULL}
#define REDIRECTS (char *[]){REDIR_REPLACE, REDIR_APPEND, REDIR_INPUT, NULL}
#define OUTPUT_REDIRECTS (char *[]){REDIR_REPLACE, REDIR_APPEND, NULL}
#define INPUT_REDIRECTS (char *[]){REDIR_INPUT, NULL}

#define OUTPUT_REPLACE_MASK 0b001
#define OUTPUT_APPEND_MASK  0b010
#define INPUT_REDIRECT_MASK 0b100


typedef struct s_command
{
    char    *cmd;
    char    **argv;
    char    *sep;
    int     fd_output;
    int     fd_input;
}               t_command;

#endif

