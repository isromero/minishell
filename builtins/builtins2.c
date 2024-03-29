/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 21:59:54 by isromero          #+#    #+#             */
/*   Updated: 2024/03/29 20:04:21 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_pwd(t_cmd *cmd)
{
	char	dir[1024];
	char	*pwd;

	pwd = ft_getenv("$PWD", cmd->env);
	if (getcwd(dir, sizeof(dir)) != NULL)
		printf("%s\n", dir);
	else if (pwd)
		printf("%s\n", pwd);
	else
		perror("-minishell: ");
	free(pwd);
}

int	ft_export(t_cmd *cmd, int export_token)
{
	int		len_of_env;
	int		pos_var;
	char	**new_env;

	if (cmd->token[export_token + 1][0] == '=')
		return (export_no_var_error(cmd, export_token));
	pos_var = len_var_in_env(cmd, cmd->token[export_token + 1]);
	new_env = NULL;
	len_of_env = 0;
	while (cmd->env[len_of_env] != NULL)
		len_of_env++;
	if (var_exists(cmd, cmd->token[export_token + 1]))
		len_of_env--;
	new_env = malloc_new_env_builtin(cmd, len_of_env);
	if (var_exists(cmd, cmd->token[export_token + 1]))
	{
		free(new_env[pos_var]);
		new_env[pos_var] = ft_strdup(cmd->token[export_token + 1]);
	}
	else if (!var_exists(cmd, cmd->token[export_token + 1]))
		new_env[len_of_env] = ft_strdup(cmd->token[export_token + 1]);
	new_env[len_of_env + 1] = NULL;
	free(cmd->env);
	cmd->env = new_env;
	return (0);
}

void	ft_export2(t_cmd *cmd)
{
	int	pos_var;
	int	i;
	int	size;

	i = 0;
	size = 0;
	pos_var = len_var_in_env(cmd, "_=./minishell");
	if (!cmd->env || !cmd->env[0])
		return ;
	free(cmd->env[pos_var]);
	cmd->env[pos_var] = ft_strreplace(cmd->env[pos_var], cmd->env[pos_var],
			"_=/usr/bin/env");
	while (cmd->env[i] != NULL)
	{
		size = ft_strlen(cmd->env[i]);
		if (ft_strcmp(cmd->env[i] + size - 2, "''") != 0)
			printf("declare -x %s\n", cmd->env[i]);
		i++;
	}
}

void	ft_unset(t_cmd *cmd, int unset_token)
{
	int		i;
	int		j;
	int		len_of_env;
	char	**new_env;

	new_env = NULL;
	i = 0;
	j = 0;
	len_of_env = 0;
	while (cmd->env[len_of_env] != NULL)
		len_of_env++;
	new_env = (char **)malloc(sizeof(char *) * (len_of_env + 1));
	if (new_env == NULL)
		return ;
	while (cmd->env[i] != NULL)
	{
		if (!compare_variable(cmd->env[i], cmd->token[unset_token + 1]))
		{
			new_env[j] = ft_strdup(cmd->env[i]);
			j++;
		}
		i++;
	}
	new_env[j] = NULL;
	cmd->env = new_env;
}

void	ft_env(t_cmd *cmd)
{
	int	pos_var;
	int	i;
	int	size;

	i = 0;
	size = 0;
	pos_var = len_var_in_env(cmd, "_=./minishell");
	if (!cmd->env || !cmd->env[0])
		return ;
	free(cmd->env[pos_var]);
	cmd->env[pos_var] = ft_strreplace(cmd->env[pos_var], cmd->env[pos_var],
			"_=/usr/bin/env");
	while (cmd->env[i] != NULL)
	{
		size = ft_strlen(cmd->env[i]);
		if (ft_strcmp(cmd->env[i] + size - 2, "''") != 0)
			printf("%s\n", cmd->env[i]);
		i++;
	}
}
