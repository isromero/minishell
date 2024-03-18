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

void	process_home_directory_expansion(t_cmd *cmd, char **token,
	t_replace_vars *rep)
{
	rep->var_start = rep->j + 1;
	rep->var_len = get_variable_length(*token, rep->var_start);
	rep->var = get_variable(*token, rep->var_start, rep->var_len);
	rep->value = ft_getenv("HOME", cmd->env);
	if (rep->value != NULL)
	{
		rep->replaced_token = append_value(rep, token);
		rep->replaced_len += ft_strlen(rep->value);
		free(rep->value);
	}
	free(rep->var);
	rep->j += rep->var_len;
	ft_strcat(rep->replaced_token, *token + rep->j + 1);
}

void	process_variables(t_cmd *cmd, char **token, t_replace_vars *rep)
{
	while (rep->j < rep->token_len)
	{
		if ((*token)[rep->j] == VARIABLE
			&& (*token)[rep->j + 1] != '\0')
		{
			rep->var_start = rep->j + 1;
			rep->var_len = get_variable_length(*token, rep->var_start);
			rep->var = get_variable(*token, rep->var_start, rep->var_len);
			rep->value = ft_getenv(rep->var, cmd->env);
			if (rep->value != NULL)
			{
				rep->replaced_token = append_value(rep, token);
				rep->replaced_len += ft_strlen(rep->value);
				free(rep->value);
			}
			free(rep->var);
			rep->j += rep->var_len;
			ft_strcat(rep->replaced_token, *token + rep->j + 1);
		}
		else if ((*token)[rep->j] == '~')
			process_home_directory_expansion(cmd, token, rep);
		else
			rep->replaced_token[rep->replaced_len++] = (*token)[rep->j];
		rep->j++;
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
		if (cmd->no_expand_vars[i] == 0
			&& ft_strcmp(cmd->token[i], "$?") != 0)
			replace_vars(cmd, &cmd->token[i]);
		i++;
	}
}
