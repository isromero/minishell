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

void	replace_vars_heredoc(t_cmd *cmd, char *buffer, int i)
{
	char	*path;
	char	*var;
	int		var_length;
	int		j;
	size_t	replace_length;
	char	*replacement;
	char	*start;
	char	*end;

	var = NULL;
	var_length = 0;
	j = ++i;
	while (buffer[j] != '\0' && buffer[j] != ' ' && buffer[j] != '\n'
		&& buffer[j] != '$' && buffer[j] != '\t' && !is_special(buffer[j])
		&& buffer[j] != SINGLE_QUOTE && buffer[j] != DOUBLE_QUOTE)
	{
		var_length++;
		j++;
	}
	var = malloc(sizeof(char) * (var_length + 1));
	ft_strncpy(var, &buffer[i], var_length);
	var[var_length] = '\0';
	path = ft_getenv(var, cmd->env);
	free(var);
	if (path != NULL)
	{
		replace_length = ft_strlen(path);
		replacement = malloc(sizeof(char) * (replace_length + 1));
		strncpy(replacement, path, replace_length);
		replacement[replace_length] = '\0';
		start = &buffer[i - 1];
		end = &buffer[i - 1 + var_length];
		ft_memmove(start + replace_length, end + 1, ft_strlen(end));
		ft_memcpy(start, replacement, replace_length);
		free(path);
		free(replacement);
	}
}

void	search_var_replace(t_cmd *cmd, char *buffer)
{
	int	i;

	i = 0;
	while (buffer[i] != '\0')
	{
		if (buffer[i] == '$')
			replace_vars_heredoc(cmd, buffer, i);
		i++;
	}
}
