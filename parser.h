/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaccary <smaccary@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/28 14:57:30 by smaccary          #+#    #+#             */
/*   Updated: 2021/02/08 12:19:57 by smaccary         ###   ########.fr       */
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

typedef struct s_command
{
    char    *cmd;
    char    **argv;
    char    *sep;
    int     fd_output;
    int     fd_input;
}               t_command;

#endif

