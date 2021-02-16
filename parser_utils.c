/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaccary <smaccary@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 15:14:23 by smaccary          #+#    #+#             */
/*   Updated: 2021/02/16 15:23:54 by smaccary         ###   ########.fr       */
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
