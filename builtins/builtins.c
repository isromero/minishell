/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 17:47:35 by isromero          #+#    #+#             */
/*   Updated: 2023/05/26 17:47:35 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_builtin(t_cmd *cmd, int n_token)
{
	if (ft_strcmp(cmd->token[n_token], "cd") == 0)
		ft_cd(cmd, n_token++);
	else if (ft_strcmp(cmd->token[n_token], "pwd") == 0)
		ft_pwd(cmd);
	else if (ft_strcmp(cmd->token[n_token], "env") == 0)
		ft_env(cmd);
	else if (ft_strcmp(cmd->token[n_token], "export") == 0 
		&& cmd->token[n_token + 1] != NULL 
		&& ft_strchr(cmd->token[n_token + 1], '='))
		ft_export(cmd, n_token++);
	else if (ft_strcmp(cmd->token[n_token], "unset") == 0 
		&& cmd->token[n_token + 1] != NULL)
		ft_unset(cmd, n_token++);
	else if (ft_strcmp(cmd->token[n_token], "echo") == 0)
		n_token = ft_echo(cmd, n_token);
	else if (ft_strcmp(cmd->token[n_token], "exit") == 0)
		ft_exit(cmd, n_token);
	else
		return ;
}

int	print_echo(t_cmd *cmd, int echo_token, int first_iteration)
{
	while (cmd->token[echo_token])
	{
		if (cmd->no_expand_vars[echo_token] == 0)
			replace_vars(cmd, &cmd->token[echo_token]);
		if (is_special(cmd->token[echo_token][0]) 
			&& (cmd->in_single_quote == 0 || cmd->in_double_quote == 0))
			break ;
		if (first_iteration != 0)
			printf(" %s", cmd->token[echo_token]);
		else if (first_iteration == 0)
		{
			printf("%s", cmd->token[echo_token]);
			first_iteration++;
		}
		echo_token++;
	}
	return (echo_token);
}

int	ft_echo(t_cmd *cmd, int echo_token)
{
	int	first_echo_token;
	int	first_iteration;

	first_echo_token = echo_token;
	first_iteration = 0;
	echo_token++;
	if (ft_strcmp(cmd->token[0], "echo") == 0 
		&& ft_strcmp(cmd->token[1], "$?") == 0) 
	{
		ft_putnbr_fd(g_status, STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
		return (0);
	}
	if (ft_strcmp(cmd->token[first_echo_token + 1], "-n") == 0)
		echo_token++;
	echo_token += print_echo(cmd, echo_token, first_iteration);
	if (ft_strcmp(cmd->token[first_echo_token + 1], "-n") != 0)
		printf("\n");
	return (echo_token);
}

void	ft_cd(t_cmd *cmd, int cd_token)
{
	char	*path;
	char	cwd[1024];
	char	*oldpwd;

	path = cmd->token[cd_token + 1];
	oldpwd = ft_getenv("PWD", cmd->env);
	if (chdir(path) != 0)
		printf("-minishell: cd: %s: Not a directory\n", path);
	getcwd(cwd, sizeof(cwd));
	setenv(oldpwd, cwd, 1);
	free(oldpwd);
}

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
	free (pwd);
}
