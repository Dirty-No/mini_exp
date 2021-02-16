/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 15:35:46 by smaccary          #+#    #+#             */
/*   Updated: 2021/02/16 21:25:07 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int
	main(int ac, char *argv[])
{
	//char *tokens_pipe[] = {"echo", "hello", "world", "|", "grep", "world", NULL};
	//char *tokens_redir[] = {"echo", "hello", "world", ">>", "text.txt", NULL};
	//char *tokens_mult_redir[] = {"echo", "hello", "world", ">", "text.txt", ">", "text2.txt", ">", "text3.txt", "<", "input.txt", NULL};
	//char *tokens_redir_middle[] = {"echo", "hello", ">", "fuck me", "<", "kill me", "world", ">", "text.txt", ">", "text2.txt", ">", "text3.txt", "<", "input.txt", NULL};
	//char *tokens_mega[] = {"echo", "hello", "world", "|", "grep", "world", "|", "grep", "-o", "wo", ">", "text.txt", NULL};
	//char *tokens_mega1[] = {"echo", "hello", "world", "|", "grep", "world", "|", "grep", "-o", "wo", ">", "text1.txt", ">", "text2.txt", ">", "text3.txt", NULL};
	//char *tokens1[] = {"echo", "hello", "world", NULL};
	char	*exec_tokens[] = {"/bin/echo", "hello", "world", "|", "/bin/grep", "-o", "world", NULL};
	
	(void)ac;
	(void)argv;
	
	exec_from_tokens(exec_tokens);
	//tokens = tokens_redir;

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
