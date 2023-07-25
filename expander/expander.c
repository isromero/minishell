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

#include "../minishell.h"

int	special_for_vars(char c)
{
	return (c == INPUT_REDIRECT || c == PIPE || c == OUTPUT_REDIRECT);
}

void	replace_vars(t_cmd *cmd, char **token)
{
	size_t	token_len;
	char	*replaced_token;
	size_t	replaced_len;
	size_t	j;
	char	*value;
	char	*var;
	char	*new_replaced_token;
	size_t	value_len;
	size_t	var_start;
	size_t	var_len;

	cmd->replaced_var += 1;
	token_len = ft_strlen(*token);
	replaced_token = malloc(token_len + 1);
	replaced_len = 0;
	j = 0;
	value = NULL;
	while (j < token_len)
	{
		if ((*token)[j] == VARIABLE && (*token)[j + 1] != '\0')
		{
			var_start = j + 1;
			var_len = 0;
			while ((*token)[j + 1 + var_len] != VARIABLE
				&& (*token)[j + 1 + var_len] != '\0'
				&& (*token)[j + 1 + var_len] != SINGLE_QUOTE)
				var_len++;
			var = malloc(var_len + 1);
			ft_strncpy(var, *token + var_start, var_len);
			var[var_len] = '\0';
			value = ft_getenv(var, cmd->env);
			if (value != NULL)
			{
				value_len = ft_strlen(value);
				new_replaced_token = malloc(replaced_len + value_len + 1);
				if (new_replaced_token == NULL)
				{
					free(replaced_token);
					free(value);
					return ;
				}
				ft_strncpy(new_replaced_token, replaced_token, replaced_len);
				ft_strncpy(new_replaced_token + replaced_len, value, value_len);
				new_replaced_token[replaced_len + value_len] = '\0';
				free(replaced_token);
				replaced_token = new_replaced_token;
				replaced_len += value_len;
				free(value);
			}
			free(var);
			j += var_len;
		}
		else
			replaced_token[replaced_len++] = (*token)[j];
		j++;
	}
	replaced_token[replaced_len] = '\0';
	free(*token);
	*token = replaced_token;
}
