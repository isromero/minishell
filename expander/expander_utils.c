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

char	*append_value(char *replaced_token, size_t replace, const char *value)
{
	char	*new_replaced_token;

	new_replaced_token = malloc(replace + ft_strlen(value) + 2);
	if (!new_replaced_token)
		return (NULL);
	ft_strncpy(new_replaced_token, replaced_token, replace);
	ft_strncpy(new_replaced_token + replace, value, ft_strlen(value));
	new_replaced_token[replace + ft_strlen(value)] = '\0';
	free(replaced_token);
	return (new_replaced_token);
}
