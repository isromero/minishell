/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 17:51:23 by isromero          #+#    #+#             */
/*   Updated: 2024/03/29 20:03:18 by isromero         ###   ########.fr       */
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

void	init_replace_vars_heredoc(t_replace_vars_heredoc **replace_vars)
{
	*replace_vars
		= (t_replace_vars_heredoc *)malloc(sizeof(t_replace_vars_heredoc));
	if (*replace_vars == NULL)
		return ;
	(*replace_vars)->path = NULL;
	(*replace_vars)->var = NULL;
	(*replace_vars)->var_length = 0;
	(*replace_vars)->j = 0;
	(*replace_vars)->replace_length = 0;
	(*replace_vars)->replacement = NULL;
	(*replace_vars)->start = NULL;
	(*replace_vars)->end = NULL;
}

void	replace_vars_heredoc(t_cmd *cmd, char *buffer, int i)
{
	t_replace_vars_heredoc	*replace_vars;

	init_replace_vars_heredoc(&replace_vars);
	replace_vars->j = ++i;
	while (buffer[replace_vars->j] != '\0' && buffer[replace_vars->j] != ' '
		&& buffer[replace_vars->j] != '\n' && buffer[replace_vars->j] != '$'
		&& buffer[replace_vars->j] != '\t'
		&& !is_special(buffer[replace_vars->j])
		&& buffer[replace_vars->j] != SINGLE_QUOTE
		&& buffer[replace_vars->j] != DOUBLE_QUOTE)
	{
		replace_vars->var_length++;
		replace_vars->j++;
	}
	replace_vars->var = malloc(sizeof(char) * (replace_vars->var_length + 1));
	ft_strncpy(replace_vars->var, &buffer[i], replace_vars->var_length);
	replace_vars->var[replace_vars->var_length] = '\0';
	replace_vars->path = ft_getenv(replace_vars->var, cmd->env);
	free(replace_vars->var);
	if (replace_vars->path != NULL)
		path_var_exists(replace_vars, buffer, i);
	free(replace_vars);
}

void	path_var_exists(t_replace_vars_heredoc *replace, char *buffer, int i)
{
	replace->replace_length = ft_strlen(replace->path);
	replace->replacement = malloc(sizeof(char) * (replace->replace_length + 1));
	ft_strncpy(replace->replacement, replace->path, replace->replace_length);
	replace->replacement[replace->replace_length] = '\0';
	replace->start = &buffer[i - 1];
	replace->end = &buffer[i - 1 + replace->var_length];
	ft_memmove(replace->start + replace->replace_length, replace->end + 1,
		ft_strlen(replace->end));
	ft_memcpy(replace->start, replace->replacement, replace->replace_length);
	free(replace->path);
	free(replace->replacement);
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
