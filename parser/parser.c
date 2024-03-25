/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 17:53:11 by isromero          #+#    #+#             */
/*   Updated: 2023/05/26 17:53:11 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	parser_aux_save_token(t_cmd *cmd, int i, int len)
{
	char	*token;

	token = (char *)malloc((len + 1) * sizeof(char));
	if (!token)
		return ;
	ft_strncpy(token, cmd->line + i, len);
	token[len] = '\0';
	save_token(cmd, token);
}

int	parser_save_token(t_cmd *cmd)
{
	int		i;
	int		len;

	i = 0;
	len = 0;
	while (cmd->line[i] != '\0')
	{
		while (cmd->line[i] == ' ' && cmd->in_single_quote == false
			&& cmd->in_double_quote == false)
			i++;
		len = check_len_token(cmd, i);
		if (len > 0)
		{
			parser_aux_save_token(cmd, i, len);
			i += len;
		}
		else if (len == -1)
			return (-1);
	}
	return (len);
}

int	parse_args(t_cmd *cmd)
{
	cmd->n_tokens = 0;
	cmd->token = NULL;
	cmd->in_single_quote = false;
	cmd->in_double_quote = false;
	/* if (count_quotes(cmd->line) % 2 != 0)
	{
		printf("-minishell: no closing quote\n");
		return (-1);
	} */
	if (parser_save_token(cmd) == -1)
		return (-1);
	save_token(cmd, NULL);
	init_expand_vars(cmd);
	if (remove_quotes(cmd) == -1)
		return (-1);
	print_tokens(cmd);
	return (0);
}

void	save_token(t_cmd *cmd, char *token)
{
	int		i;
	char	**new_token;

	cmd->n_tokens++;
	new_token = (char **)malloc(cmd->n_tokens * sizeof(char *));
	if (new_token == NULL)
		return ;
	i = 0;
	if (cmd->token != NULL)
	{
		while (i < cmd->n_tokens - 1)
		{
			new_token[i] = cmd->token[i];
			i++;
		}
		free(cmd->token);
	}
	new_token[cmd->n_tokens - 1] = token;
	cmd->token = new_token;
}
