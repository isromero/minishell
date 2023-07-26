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

void	free_replace_vars(t_replace_vars *replace_vars)
{
	free(replace_vars);
}

void	process_variables(t_cmd *cmd, char **token, t_replace_vars *replace)
{
	while (replace->j < replace->token_len)
	{
		if ((*token)[replace->j] == VARIABLE
			&& (*token)[replace->j + 1] != '\0')
		{
			replace->var_start = replace->j + 1;
			replace->var_len = get_variable_length(*token, replace->var_start);
			replace->var = get_variable(*token,
					replace->var_start, replace->var_len);
			replace->value = get_environment_value(replace->var, cmd->env);
			if (replace->value != NULL)
			{
				replace->replaced_token = append_value(replace->replaced_token,
						replace->replaced_len, replace->value);
				replace->replaced_len += ft_strlen(replace->value);
				free(replace->value);
			}
			free(replace->var);
			replace->j += replace->var_len;
		}
		else
			replace->replaced_token[replace->replaced_len++]
				= (*token)[replace->j];
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
	free_replace_vars(replace_vars);
}
