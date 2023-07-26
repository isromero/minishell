/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_manage2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 18:53:00 by isromero          #+#    #+#             */
/*   Updated: 2023/07/23 18:53:00 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	remove_double_quotes(char **token)
{
	int	len;
	int	i;
	int	j;

	len = ft_strlen(*token);
	i = 0;
	j = 0;
	while (i < len)
	{
		if ((*token)[i] != DOUBLE_QUOTE)
		{
			(*token)[j] = (*token)[i];
			j++;
		}
		i++;
	}
	(*token)[j] = '\0';
}

void	remove_single_quotes(char **token)
{
	int	len;
	int	i;
	int	j;

	len = ft_strlen(*token);
	i = 0;
	j = 0;
	while (i < len)
	{
		if ((*token)[i] != SINGLE_QUOTE)
		{
			(*token)[j] = (*token)[i];
			j++;
		}
		i++;
	}
	(*token)[j] = '\0';
}

void	remove_quotes2(t_cmd *cmd, int i)
{
	int	j;

	j = 0;
	if (i >= 1 && cmd->token[i - 1] != NULL
		&& ft_strcmp(cmd->token[i - 1], HEREDOC_REDIRECT) == 0)
		cmd->in_quote_delim_heredoc = 1;
	while (cmd->token[i][j] != '\0')
	{
		if (cmd->token[i][j] == DOUBLE_QUOTE)
		{
			remove_double_quotes(&cmd->token[i]);
			break ;
		}
		if (cmd->token[i][j] == SINGLE_QUOTE)
		{
			remove_single_quotes(&cmd->token[i]);
			break ;
		}
		j++;
	}
}

void	even_quotes(char *token, int left_double_quotes,
	int left_single_quotes, size_t len_token)
{
	if (left_double_quotes % 2 == 0
		&& (size_t)(count_single_quotes(token)
		+ count_double_quotes(token)) != len_token)
		remove_single_quotes(&token);
	if (left_single_quotes % 2 == 0
		&& (size_t)(count_single_quotes(token)
		+ count_double_quotes(token)) != len_token)
		remove_double_quotes(&token);
}

int	remove_quotes(t_cmd *cmd)
{
	int		left_double_quotes;
	int		left_single_quotes;
	size_t	len_token;
	int		i;

	cmd->in_quote_delim_heredoc = 0;
	i = 0;
	while (cmd->token[i])
	{
		len_token = ft_strlen(cmd->token[i]);
		left_double_quotes = count_left_double_quotes(cmd->token[i]);
		left_single_quotes = count_left_single_quotes(cmd->token[i]);
		if (count_double_quotes(cmd->token[i]) >= 2
			|| count_single_quotes(cmd->token[i]) >= 2)
		{
			remove_quotes2(cmd, i);
			even_quotes(cmd->token[i], left_double_quotes,
				left_single_quotes, len_token);
		}
		if (cmd->token[0] != NULL && ft_strlen(cmd->token[0]) == 0)
			printf("-minishell: command not found\n");
		i++;
	}
	return (0);
}
