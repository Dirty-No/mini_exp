/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 15:15:38 by smaccary          #+#    #+#             */
/*   Updated: 2021/02/16 21:31:37 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void
	print_argv(char **argv)
{
	if (!DEBUG)
		return ;
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
	if (!DEBUG)
		return ;
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
	if (!DEBUG)
		return ;
	ft_lstiter(lst, (void *)print_command);
}
