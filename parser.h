/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/28 14:57:30 by smaccary          #+#    #+#             */
/*   Updated: 2021/02/11 16:42:22 by root             ###   ########.fr       */
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

#define SEPARATORS (char *[]){">>", "<", ">", "|", ";", NULL}
#define REDIRECTS (char *[]){">>", "<", ">", NULL}



typedef struct s_command
{
    char    *cmd;
    char    **argv;
    char    *sep;
    int     fd_output;
    int     fd_input;
}               t_command;

#endif

