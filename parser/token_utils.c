/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 17:49:03 by isromero          #+#    #+#             */
/*   Updated: 2023/05/26 17:49:03 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	clean_tokens(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->n_tokens)
	{
		free(cmd->token[i]);
		i++;
	}
	free(cmd->token);
	cmd->token = NULL;
	cmd->n_tokens = 0;
}

void	print_tokens(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->n_tokens)
	{
		printf("Token %d: %s\n", i, cmd->token[i]);
		i++;
	}
}

int	cmd_token_len(t_cmd *cmd, int len)
{
	int	i;
	int	is_double_quote;
	int	is_single_quote;

	is_double_quote = 0;
	is_single_quote = 0;
	i = 0;
	while (cmd->line[i + len] != '\0')
	{
		if ((cmd->line[i + len] == ' ' || is_special2(cmd->line[i + len]))
			&& is_double_quote % 2 == 0 && is_single_quote % 2 == 0)
			break ;
		else if (cmd->line[i + len] == DOUBLE_QUOTE)
			is_double_quote++;
		else if (cmd->line[i + len] == SINGLE_QUOTE)
			is_single_quote++;
		i++;
	}
	return (i);
}

int	check_len_token(t_cmd *cmd, int len)
{
	while (cmd->line[len] != '\0')
	{
		if (cmd->line[len] == VARIABLE)
			return (cmd_token_len(cmd, len));
		else if (cmd->line[len] == SINGLE_QUOTE)
			return (is_single_quote(cmd, len));
		else if (cmd->line[len] == DOUBLE_QUOTE)
			return (is_double_quote(cmd, len));
		else if (is_special2(cmd->line[len]))
			return (check_len_special(cmd, len));
		else if (cmd->line[len] != '\0')
			return (cmd_token_len(cmd, len));
	}
	return (0);
}

int	find_variables(char **token)
{
	int	i;

	i = 0;
	while (token[i][0] != '\0')
	{
		if (token[i][0] == VARIABLE)
			return (1);
		i++;
	}
	return (0);
}
