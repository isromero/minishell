/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adgutier <adgutier@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 18:22:28 by adgutier          #+#    #+#             */
/*   Updated: 2023/07/26 18:22:28 by adgutier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

size_t	get_variable_length(const char *token, size_t var_start)
{
	size_t	var_len;

	var_len = 0;
	while (token[var_start + var_len] != VARIABLE
		&& token[var_start + var_len] != '\0'
		&& token[var_start + var_len] != SINGLE_QUOTE)
		var_len++;
	return (var_len);
}

char	*get_variable(const char *token, size_t var_start, size_t var_len)
{
	char	*var;

	var = malloc(var_len + 1);
	ft_strncpy(var, token + var_start, var_len);
	var[var_len] = '\0';
	return (var);
}

char	*get_environment_value(const char *var, char **env)
{
	char	*value;

	value = ft_getenv(var, env);
	return (value);
}

char	*append_value(char *replaced_token, size_t replace, const char *value)
{
	char	*new_replaced_token;

	new_replaced_token = malloc(replace + ft_strlen(value) + 1);
	if (new_replaced_token == NULL)
	{
		free(replaced_token);
		return (NULL);
	}
	ft_strncpy(new_replaced_token, replaced_token, replace);
	ft_strncpy(new_replaced_token + replace, value, ft_strlen(value));
	new_replaced_token[replace + ft_strlen(value)] = '\0';
	free(replaced_token);
	return (new_replaced_token);
}

void	init_replace_vars(t_replace_vars **replace_vars)
{
	*replace_vars = (t_replace_vars *)malloc(sizeof(t_replace_vars));
	if (*replace_vars == NULL)
		return ;
	(*replace_vars)->token_len = 0;
	(*replace_vars)->j = 0;
	(*replace_vars)->value = NULL;
	(*replace_vars)->replaced_len = 0;
	(*replace_vars)->replaced_token = NULL;
	(*replace_vars)->var_start = 0;
	(*replace_vars)->var_len = 0;
	(*replace_vars)->var = NULL;
}
