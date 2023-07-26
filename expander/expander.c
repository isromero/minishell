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

void	init_expand_vars(t_cmd *cmd)
{
	int	i;

	i = 0;
	cmd->no_expand_vars = malloc((cmd->n_tokens) * sizeof(int));
	while (cmd->token[i])
	{
		if (count_left_single_quotes(cmd->token[i]) % 2 != 0
			&& cmd->token[i][0] == SINGLE_QUOTE)
			cmd->no_expand_vars[i] = 1;
		else
			cmd->no_expand_vars[i] = 0;
		i++;
	}
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
