/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 17:51:23 by isromero          #+#    #+#             */
/*   Updated: 2023/05/26 17:51:23 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_vars(t_cmd *cmd)
{
	int	i;
	char *path;
	char *aux;
	char *var;

	i = 0;
	aux = NULL;
	while(cmd->token[i])
	{
		if(cmd->token[i][0] == VARIABLE)
		{
			aux = cmd->token[i];
			var = &aux[1];

			// Quedarse solo con la lógica(está mal seguro)
			path = ft_getenv(var, cmd->env);
			if(path == NULL)
				return ;
			printf("minishell: %s\n", path);
		}
		i++;
	}
}
