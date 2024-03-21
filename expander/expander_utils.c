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

size_t	get_variable_length(const char *token, size_t var_start)
{
	size_t	var_len;

	var_len = 0;
	while (token[var_start + var_len] != VARIABLE
		&& token[var_start + var_len] != '\0'
		&& token[var_start + var_len] != SINGLE_QUOTE
		&& token[var_start + var_len] != '/'
		&& ft_isalpha(token[var_start + var_len]))
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
	
	new_replaced_token = malloc(replace->replaced_len + ft_strlen(replace->value) + ft_strlen((*token) + replace->j) + 1);

	if (!new_replaced_token)
			return (NULL);
	if (replace->replaced_token)
			ft_strncpy(new_replaced_token, replace->replaced_token, replace->replaced_len);
	ft_strncpy(new_replaced_token + replace->replaced_len, replace->value, ft_strlen(replace->value) + 1);
	if (replace->replaced_token)
			free(replace->replaced_token);
	return new_replaced_token;
}

void	get_replaced_token(t_cmd *cmd, char **token, t_replace_vars *replace)
{
	replace->var_start = replace->j + 1;
	replace->var_len = get_variable_length(*token, replace->var_start);
	replace->var = get_variable(*token, replace->var_start, replace->var_len);
	if ((*token)[replace->j] == '~')
		replace->value = ft_getenv("HOME", cmd->env);
	else
		replace->value = ft_getenv(replace->var, cmd->env);
	if (replace->value != NULL)
	{
		replace->replaced_token = append_value(replace, token);
		replace->replaced_len += ft_strlen(replace->value);
		free(replace->value);
	}
	free(replace->var);
	replace->j += replace->var_len;
	ft_strcat(replace->replaced_token, *token + replace->j + 1);
}