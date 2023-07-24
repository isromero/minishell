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

void	set_pwd_env(t_cmd *cmd, char **new_env, char *cwd, char *oldpwd)
{
	int		oldpwd_var;
	int		pwd_var;
	char	*actualpwd;
	char	*change_oldpwd;

	pwd_var = len_var_in_env(cmd, "PWD=");
	oldpwd_var = len_var_in_env(cmd, "OLDPWD=");
	actualpwd = 
		malloc(sizeof(char) * (ft_strlen("PWD=") + ft_strlen(cwd) + 2));
	if (actualpwd == NULL)
		return ;
	change_oldpwd = 
		malloc(sizeof(char) * (ft_strlen("OLDPWD=") + ft_strlen(oldpwd) + 2));
	if (change_oldpwd == NULL)
		return ;
	ft_strcpy(actualpwd, "PWD=");
	ft_strcpy(change_oldpwd, "OLDPWD=");
	free(new_env[pwd_var]);
	new_env[pwd_var] = ft_strdup(ft_strcat(actualpwd, cwd));
	free(new_env[oldpwd_var]);
	new_env[oldpwd_var] = ft_strdup(ft_strcat(change_oldpwd, oldpwd));
	free(actualpwd);
	free(change_oldpwd);
}

void	ft_cd(t_cmd *cmd, int cd_token)
{
	char	cwd[1024];
	char	*oldpwd;
	char	**new_env;
	int		len_of_env;

	oldpwd = ft_getenv("PWD", cmd->env);
	len_of_env = 0;
	new_env = NULL;
	if (chdir(cmd->token[cd_token + 1]) != 0)
	{
		printf("-minishell: cd: %s: Not a directory\n", 
			cmd->token[cd_token + 1]);
		return ;
	}
	getcwd(cwd, sizeof(cwd));
	while (cmd->env[len_of_env] != NULL)
		len_of_env++;
	new_env = malloc_new_env_builtin(cmd, len_of_env - 1);
	set_pwd_env(cmd, new_env, cwd, oldpwd);
	new_env[len_of_env] = NULL;
	free(cmd->env);
	cmd->env = new_env;
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
