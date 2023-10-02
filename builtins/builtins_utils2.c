/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adgutier <adgutier@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:47:28 by adgutier          #+#    #+#             */
/*   Updated: 2023/10/02 15:47:28 by adgutier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	cmd->env[pos_var] = ft_strreplace
		(cmd->env[pos_var], cmd->env[pos_var], "_=/usr/bin/env");
	while (cmd->env[i] != NULL)
	{
		size = ft_strlen(cmd->env[i]);
		if (ft_strcmp(cmd->env[i] + size - 2, "''") != 0)
			printf("declare -x %s\n", cmd->env[i]);
		i++;
	}
}
