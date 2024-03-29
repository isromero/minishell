/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_manage.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 19:10:12 by isromero          #+#    #+#             */
/*   Updated: 2024/03/29 20:01:59 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_left_single_quotes(char *token)
{
	int	i;

	i = 0;
	while (token[i] != DOUBLE_QUOTE && token[i] == SINGLE_QUOTE
		&& token[i] != '\0')
		i++;
	return (i);
}

static int	check_for_orphan_quotes(char *token)
{
	int	in_single_quote;
	int	in_double_quote;
	int	i;

	in_double_quote = 0;
	in_single_quote = 0;
	i = 0;
	while (token[i] != '\0')
	{
		if (token[i] == SINGLE_QUOTE && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (token[i] == DOUBLE_QUOTE && !in_single_quote)
			in_double_quote = !in_double_quote;
		i++;
	}
	return (in_single_quote || in_double_quote);
}

static void	remove_quotes2(t_cmd *cmd, int i)
{
	int	j;
	int	k;
	int	in_double_quote;
	int	in_single_quote;

	j = 0;
	k = 0;
	in_double_quote = 0;
	in_single_quote = 0;
	if (i >= 1 && cmd->token[i - 1] != NULL
		&& ft_strcmp(cmd->token[i - 1], HEREDOC_REDIRECT) == 0
		&& (cmd->token[i][0] == '"'
		|| cmd->token[i][0] == '\''))
		cmd->in_quote_delim_heredoc = 1;
	while (cmd->token[i][j] != '\0')
	{
		if (cmd->token[i][j] == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else if (cmd->token[i][j] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else
			cmd->token[i][k++] = cmd->token[i][j];
		j++;
	}
	cmd->token[i][k] = '\0';
}

int	remove_quotes(t_cmd *cmd)
{
	int	i;

	i = 0;
	cmd->in_quote_delim_heredoc = 0;
	while (cmd->token[i])
	{
		if (check_for_orphan_quotes(cmd->token[i]))
		{
			printf("-minishell: no closing quote\n");
			return (-1);
		}
		remove_quotes2(cmd, i);
		if (cmd->token[0] != NULL && ft_strlen(cmd->token[0]) == 0)
			printf("-minishell: command not found\n");
		i++;
	}
	return (0);
}
