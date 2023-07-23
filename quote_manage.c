/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_manage.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 18:48:38 by isromero          #+#    #+#             */
/*   Updated: 2023/07/23 18:48:38 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_quotes(char *line)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (line[i] != '\0')
	{
		if (line[i] == DOUBLE_QUOTE || line[i] == SINGLE_QUOTE)
			count++;
		i++;
	}
	return (count);
}

int	count_double_quotes(char *token)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (token[i] != '\0')
	{
		if (token[i] == DOUBLE_QUOTE)
			count++;
		i++;
	}
	return (count);
}

int	count_single_quotes(char *token)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (token[i] != '\0')
	{
		if (token[i] == SINGLE_QUOTE)
			count++;
		i++;
	}
	return (count);
}

int	count_left_single_quotes(char *token)
{
	int	i;

	i = 0;
	while (token[i] != DOUBLE_QUOTE && token[i] == SINGLE_QUOTE
		&& token[i] != '\0')
		i++;
	return (i);
}

int	count_left_double_quotes(char *token)
{
	int	i;

	i = 0;
	while (token[i] != SINGLE_QUOTE && token[i] == DOUBLE_QUOTE
		&& token[i] != '\0')
		i++;
	return (i);
}
