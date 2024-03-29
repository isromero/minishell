/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 20:04:28 by isromero          #+#    #+#             */
/*   Updated: 2024/03/29 20:04:30 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	print_echo(t_cmd *cmd, int echo_token, int print_newline)
{
	int	first_iteration;

	first_iteration = 1;
	while (cmd->token[echo_token])
	{
		if (cmd->no_expand_vars[echo_token] == 0)
			replace_vars(cmd, &cmd->token[echo_token]);
		if (is_special(cmd->token[echo_token][0]) && (cmd->in_single_quote == 0
				|| cmd->in_double_quote == 0))
			break ;
		if (!first_iteration)
			printf(" %s", cmd->token[echo_token]);
		else
		{
			printf("%s", cmd->token[echo_token]);
			first_iteration = 0;
		}
		echo_token++;
	}
	if (print_newline)
		printf("\n");
	return (echo_token);
}

int	ft_echo(t_cmd *cmd, int echo_token)
{
	int	print_newline;

	print_newline = 1;
	echo_token++;
	while (cmd->token[echo_token] && starts_with_n(cmd->token[echo_token]))
	{
		print_newline = 0;
		echo_token++;
	}
	echo_token = print_echo(cmd, echo_token, print_newline);
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
	actualpwd = malloc(sizeof(char) * (ft_strlen("PWD=") + ft_strlen(cwd) + 2));
	if (actualpwd == NULL)
		return ;
	change_oldpwd = malloc(sizeof(char) * (ft_strlen("OLDPWD=")
				+ ft_strlen(oldpwd) + 2));
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

int	ft_cd(t_cmd *cmd, int cd_token)
{
	char	*oldpwd;

	oldpwd = ft_getenv("PWD", cmd->env);
	if (cmd->token[cd_token + 1] == NULL)
	{
		chdir(ft_getenv("HOME", cmd->env));
		return (0);
	}
	if (chdir(cmd->token[cd_token + 1]) != 0 && cmd->token[cd_token
			+ 1][0] != '-')
	{
		printf("-minishell: cd: %s: Not a directory\n", cmd->token[cd_token
			+ 1]);
		g_status = 1;
		return (1);
	}
	ft_cd2(cmd, cd_token, oldpwd);
	return (0);
}

int	ft_cd2(t_cmd *cmd, int cd_token, char *oldpwd)
{
	char	cwd[1024];
	char	**new_env;
	int		len_of_env;

	len_of_env = 0;
	new_env = NULL;
	getcwd(cwd, sizeof(cwd));
	while (cmd->env[len_of_env] != NULL)
		len_of_env++;
	new_env = malloc_new_env_builtin(cmd, len_of_env - 1);
	if (cmd->token[cd_token + 1][0] == '-')
	{
		oldpwd = ft_getenv("OLDPWD", cmd->env);
		chdir(oldpwd);
		set_pwd_env(cmd, new_env, oldpwd, cwd);
	}
	else
		set_pwd_env(cmd, new_env, cwd, oldpwd);
	new_env[len_of_env] = NULL;
	free(cmd->env);
	cmd->env = new_env;
	free(oldpwd);
	return (0);
}
