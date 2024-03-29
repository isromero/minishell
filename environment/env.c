/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 17:48:36 by isromero          #+#    #+#             */
/*   Updated: 2024/03/29 20:04:02 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_getenv(const char *name, char **env)
{
	char	*value;
	char	*value_start;
	char	*value_end;
	int		i;

	value = NULL;
	i = 0;
	while (env[i] != NULL && env[i][0] != ' ')
	{
		if (ft_strncmp(name, env[i], ft_strlen(name)) == 0
			&& env[i][ft_strlen(name)] == '=')
		{
			value_start = env[i] + ft_strlen(name) + 1;
			value_end = ft_strchr(value_start, '|');
			if (value_end == NULL)
				value_end = ft_strchr(value_start, '\0');
			value = (char *)malloc(((value_end - value_start + 1) + 1)
					* sizeof(char));
			ft_strncpy(value, value_start, value_end - value_start + 1);
			value[value_end - value_start + 1] = '\0';
			return (value);
		}
		i++;
	}
	return (0);
}

void	malloc_env(t_cmd *cmd, char **env)
{
	int	i;
	int	env_count;

	i = 0;
	env_count = 0;
	while (env[i] != NULL)
	{
		env_count++;
		i++;
	}
	cmd->env = malloc((env_count + 1) * sizeof(char *));
	if (!cmd->env)
		return ;
}

void	init_env(t_cmd *cmd, char **env)
{
	if (env[0] == NULL)
		get_default_env(cmd);
	else
		cmd->env = ft_strstrdup(env);
}

void	get_default_env(t_cmd *cmd)
{
	char	pwd[4096];
	int		env_count;
	int		env_index;
	int		i;
	char	**default_env;

	env_count = 5;
	default_env = malloc((env_count + 1) * sizeof(char *));
	env_index = 0;
	i = 0;
	default_env[env_index++] = ft_strjoin("PWD=", getcwd(pwd, sizeof(pwd)));
	default_env[env_index++] = ft_strdup("SHLVL=1");
	default_env[env_index++]
		= ft_strdup("PATH=/usr/local/sbin:/usr/local/bin:/usr/bin:/bin");
	default_env[env_index++] = ft_strjoin("_=", "./minishell");
	default_env[env_index] = NULL;
	cmd->env = malloc((env_count + 1) * sizeof(char *));
	while (i < env_count)
	{
		cmd->env[i] = default_env[i];
		i++;
	}
	cmd->env[env_count] = NULL;
	free(default_env);
}
