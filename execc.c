/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execc.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaccary <smaccary@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/01 13:16:41 by smaccary          #+#    #+#             */
/*   Updated: 2021/02/01 14:58:20 by smaccary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int exec_command(t_command *command)
{
	int		    comand_pipes[2];
	extern char **environ;
	pid_t	    pid;
	
	pipe(comand_pipes); // Create the comand_pipes
	pid = fork();

	if (pid == 0)
	{
		dup2(comand_pipes[1], 1);
		
		execve(command->cmd, command->argv, environ);
		exit(0);
	}
	while (1)
	;
	close(comand_pipes[1]);
	close(comand_pipes[0]);
	return (0);
}

int main(void)
{
	t_command command;

	command.argv = (char *[]){"/bin/cat", NULL};
	command.cmd = "/bin/cat";
	command.fd_input = 0;
	command.fd_output = 1;
	exec_command(&command);
	return (0);
}