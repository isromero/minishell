/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_redirects_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 18:49:58 by isromero          #+#    #+#             */
/*   Updated: 2023/07/24 18:49:58 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_heredoc_redirect(t_cmd *cmd, int len)
{
	int	n_redirects;

	n_redirects = 0;
	while (cmd->token[len] != NULL)
	{
		if (ft_strcmp(cmd->token[len], HEREDOC_REDIRECT) == 0)
			n_redirects++;
		len++;
	}
	if (n_redirects == 1)
		return (1);
	else if (n_redirects > 1)
		return (n_redirects);
	return (0);
}

int	find_first_heredoc_redirect(t_cmd *cmd)
{
	int	len;

	len = 0;
	while (cmd->token[len] != NULL)
	{
		if (ft_strcmp(cmd->token[len], HEREDOC_REDIRECT) == 0)
			return (len);
		len++;
	}
	return (0);
}

int	find_last_heredoc_redirect(t_cmd *cmd)
{
	int	len;

	len = cmd->n_tokens - 2;
	while (len >= 0)
	{
		if (ft_strcmp(cmd->token[len], HEREDOC_REDIRECT) == 0)
			return (len);
		len--;
	}
	return (0);
}

char	*find_heredoc_delim(t_cmd *cmd)
{
	int		len;
	char	*delim;
	size_t	delim_len;

	delim = NULL;
	delim_len = 0;
	len = cmd->n_tokens - 2;
	while (len >= 0)
	{
		if (ft_strcmp(cmd->token[len], HEREDOC_REDIRECT) == 0)
		{
			delim = cmd->token[len + 1];
			delim_len = ft_strlen(delim);
			/* if ((delim[0] == '\'' || delim[0] == '\"') && delim_len > 1 && delim[delim_len - 1] == delim[0])
			{
				cmd->in_quote_heredoc = 1;
				memmove(delim, delim + 1, delim_len - 2);
				delim[delim_len - 2] = '\0';
			} */
			if ((delim[0] == '\'' || delim[0] == '\"') && delim_len > 1
				&& delim[delim_len - 1] != delim[0]
				&& cmd->in_quote_heredoc == 0)
				delim = "NOT DELIMITATORXXxXxxXXxxXXXXXX";
			return (delim);
		}
		len--;
	}
	return (0);
}

int	heredoc_content(t_cmd *cmd, int fd)
{
	char	*line;

	line = readline(">");
	if (ft_strcmp(line, find_heredoc_delim(cmd)) == 0)
	{
		free(line);
		return (1);
	}
	else if (line && *line != '\n')
	{
		ft_putstr_fd(line, fd);
		ft_putchar_fd('\n', fd);
		free(line);
	}
	else if (*line == '\n')
		ft_putchar_fd('\n', fd);
	return (0);
}
