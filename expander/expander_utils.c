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
		&& token[var_start + var_len] != SINGLE_QUOTE
		&& token[var_start + var_len] != '/')
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

char	*append_value(t_replace_vars *replace, char **token)
{
	char	*new_replaced_token;

	new_replaced_token = malloc(replace->replaced_len + ft_strlen(replace->value) + ft_strlen(*token + replace->j) + 2);
	if (!new_replaced_token)
		return (NULL);
	ft_strncpy(new_replaced_token, replace->replaced_token, replace->replaced_len);
	ft_strncpy(new_replaced_token + replace->replaced_len, replace->value, ft_strlen(replace->value));
	new_replaced_token[replace->replaced_len + ft_strlen(replace->value)] = '\0';
	free(replace->replaced_token);
	return (new_replaced_token);
}
