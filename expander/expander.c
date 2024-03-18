/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars_normal.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adgutier <adgutier@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 18:24:56 by adgutier          #+#    #+#             */
/*   Updated: 2023/07/26 18:24:56 by adgutier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	process_variables(t_cmd *cmd, char **token, t_replace_vars *replace)
{
	while (replace->j < replace->token_len)
	{
		if (((*token)[replace->j] == VARIABLE
			&& (*token)[replace->j + 1] != '\0')
			|| (*token)[replace->j] == '~')
			get_replaced_token(cmd, token, replace);
		else
			replace->replaced_token[replace->replaced_len++] = (*token)[replace->j];
		replace->j++;
	}
}

void	process_token(t_cmd *cmd, char **token, t_replace_vars *replace_vars)
{
	replace_vars->token_len = ft_strlen(*token);
	replace_vars->replaced_token = malloc(replace_vars->token_len + 1);
	cmd->replaced_var += 1;
	process_variables(cmd, token, replace_vars);
	replace_vars->replaced_token[replace_vars->replaced_len] = '\0';
}

void	replace_vars(t_cmd *cmd, char **token)
{
	t_replace_vars	*replace_vars;

	init_replace_vars(&replace_vars);
	process_token(cmd, token, replace_vars);
	free(*token);
	*token = replace_vars->replaced_token;
	free(replace_vars);
}

void	replace_before_execute(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->n_tokens - 1)
	{
		if (cmd->no_expand_vars[i] == 0)
			replace_vars(cmd, &cmd->token[i]);
		i++;
	}
}
