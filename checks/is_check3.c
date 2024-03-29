/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_check3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 17:21:04 by isromero          #+#    #+#             */
/*   Updated: 2024/03/29 20:58:30 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_double_quote(t_cmd *cmd, int len)
{
	int	i;

	cmd->in_double_quote = true;
	i = 1;
	while (cmd->line[i + len] != '\0')
	{
		if (cmd->line[i + len] == DOUBLE_QUOTE)
		{
			cmd->in_double_quote = false;
			i++;
			break ;
		}
		i++;
	}
	return (i);
}

int	is_single_quote(t_cmd *cmd, int len)
{
	int	i;

	cmd->in_single_quote = true;
	i = 1;
	while (cmd->line[i + len] != '\0')
	{
		if (cmd->line[i + len] == SINGLE_QUOTE)
		{
			cmd->in_single_quote = false;
			i++;
			break ;
		}
		i++;
	}
	return (i);
}

int	check_len_special(t_cmd *cmd, int len)
{
	int	i;

	i = 0;
	while (cmd->line[i + len] != ' ' && is_special(cmd->line[i + len]))
		i++;
	if (i > 2)
	{
		printf("-minishell: syntax error near unexpected token '");
		i = 0;
		while (i < 2)
		{
			printf("%c", cmd->line[i + len + 2]);
			i++;
		}
		printf("'\n");
		return (-1);
	}
	else if (i == 1)
		return (1);
	else if (i == 2)
		return (2);
	return (-1);
}

int	is_heredoc_redirect(t_cmd *cmd, int len)
{
	int	n_redirects;

	n_redirects = 0;
	while (cmd->token[len] != NULL)
	{
		if (ft_strcmp(cmd->token[len], HEREDOC_REDIRECT) == 0)
			n_redirects++;
		len++;
	}
	if (n_redirects == 1)
		return (1);
	else if (n_redirects > 1)
		return (n_redirects);
	return (0);
}

int	is_append_redirect(t_cmd *cmd, int len)
{
	int	n_redirects;

	n_redirects = 0;
	while (cmd->token[len] != NULL)
	{
		if (ft_strcmp(cmd->token[len], APPEND_REDIRECT) == 0)
			n_redirects++;
		len++;
	}
	if (n_redirects == 1)
		return (1);
	else if (n_redirects > 1)
		return (n_redirects);
	return (0);
}
