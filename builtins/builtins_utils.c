/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 20:04:44 by isromero          #+#    #+#             */
/*   Updated: 2024/03/29 20:28:23 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execute_builtin(t_cmd *cmd, int n_token)
{
	if (ft_strcmp(cmd->token[n_token], "cd") == 0)
		return (ft_cd(cmd, n_token++));
	else if (ft_strcmp(cmd->token[n_token], "pwd") == 0)
		ft_pwd(cmd);
	else if (ft_strcmp(cmd->token[n_token], "env") == 0)
		ft_env(cmd);
	else if (ft_strcmp(cmd->token[n_token], "export") == 0 && cmd->token[n_token
			+ 1] != NULL && ft_strchr(cmd->token[n_token + 1], '='))
		return (ft_export(cmd, n_token++));
	else if (ft_strcmp(cmd->token[n_token], "export") == 0
		&& !cmd->token[n_token + 1])
		ft_export2(cmd);
	else if (ft_strcmp(cmd->token[n_token], "unset") == 0 && cmd->token[n_token
			+ 1] != NULL)
		ft_unset(cmd, n_token++);
	else if (ft_strcmp(cmd->token[n_token], "echo") == 0)
		n_token = ft_echo(cmd, n_token);
	else if (ft_strcmp(cmd->token[n_token], "exit") == 0)
		ft_exit(cmd, n_token);
	return (0);
}

int	len_var_in_env(t_cmd *cmd, char *token)
{
	int	i;
	int	eqpos;

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

int	var_exists(t_cmd *cmd, char *token)
{
	int	i;
	int	eqpos;

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

bool	compare_variable(const char *variable, const char *name)
{
	int	i;

	i = 0;
	while (variable[i] != '=' && name[i] != '\0')
	{
		if (variable[i] != name[i])
			return (false);
		i++;
	}
	return (variable[i] == '=' && name[i] == '\0');
}

char	**malloc_new_env_builtin(t_cmd *cmd, int len_of_env)
{
	char	**new_env;
	int		i;

	new_env = NULL;
	i = 0;
	new_env = (char **)malloc(sizeof(char *) * (len_of_env + 2));
	if (!new_env)
		return (0);
	while (cmd->env[i] != NULL)
	{
		new_env[i] = cmd->env[i];
		i++;
	}
	return (new_env);
}
