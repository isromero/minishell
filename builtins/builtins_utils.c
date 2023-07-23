/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 21:58:11 by isromero          #+#    #+#             */
/*   Updated: 2023/07/23 21:58:11 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_builtin(t_cmd *cmd, int n_token)
{
	if (ft_strcmp(cmd->token[n_token], "echo") == 0)
		return(1);
	else if (ft_strcmp(cmd->token[n_token], "cd") == 0)
		return(1);
	else if (ft_strcmp(cmd->token[n_token], "pwd") == 0)
		return(1);
	else if (ft_strcmp(cmd->token[n_token], "export") == 0)
		return(1);
	else if (ft_strcmp(cmd->token[n_token], "unset") == 0)
		return(1);
	else if (ft_strcmp(cmd->token[n_token], "env") == 0)
		return(1);
	else if (ft_strcmp(cmd->token[n_token], "exit") == 0)
		return(1);
	return (0);
}

int len_var_in_env(t_cmd *cmd, char *token)
{
	int    i;
	int    eqpos;

	i = 0;
	eqpos = 0;
	while (token[eqpos] != '=')
		eqpos++;
	while (cmd->env[i])
	{
		if (ft_memcmp(cmd->env[i], token, eqpos + 1) == 0)
			return (i);
		i++;
	}
	return (0);
}

int var_exists(t_cmd *cmd, char *token)
{
	int    i;
	int    eqpos;

	i = 0;
	eqpos = 0;
	while (token[eqpos] != '=')
		eqpos++;
	while (cmd->env[i])
	{
		if (ft_memcmp(cmd->env[i], token, eqpos + 1) == 0)
			return (1);
		i++;
	}
	return (0);
}

bool compare_variable(const char* variable, const char* name)
{
	int i;

	i = 0;
	while (variable[i] != '=' && name[i] != '\0')
	{
		if (variable[i] != name[i])
			return false;
		i++;
	}
	return (variable[i] == '=' && name[i] == '\0');
}
