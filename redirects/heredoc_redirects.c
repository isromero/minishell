/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_redirects.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 18:50:01 by isromero          #+#    #+#             */
/*   Updated: 2023/07/24 18:50:01 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
		&& !is_redirects(buffer[j]))
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

void	replace_env_vars(t_cmd *cmd, char *buffer)
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

void	heredoc_redirect(t_cmd **cmd)
{
	int		fd;
	pid_t	pid;
	char	buffer[1024];
	ssize_t	bytes_read;
	char	*delim;

	cmd[0]->in_quote_heredoc = 0;
	pid = fork();
	signal(SIGINT, SIG_IGN);
	if (pid == -1)
	{
		perror("fork");
		exit(1);
	}
	else if (pid == 0)
	{
		fd = open("/tmp/heredocBURMITO", O_RDWR
				| O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
		if (fd == -1)
		{
			perror("");
			exit(1);
		}
		signal(SIGINT, SIG_IGN);
		signal(SIGINT, &handle_ctrlc_heredoc);
		while (1)
		{
			if (heredoc_content(cmd[0], fd) == 1)
				break ;
		}
		close(fd);
		fd = open("/tmp/heredocBURMITO", O_RDONLY);
		ft_memset(buffer, 0, sizeof(buffer));
		bytes_read = read(fd, buffer, sizeof(buffer));
		while ((bytes_read) > 0)
		{
			bytes_read = read(fd, buffer, sizeof(buffer));
			delim = find_heredoc_delim(cmd[0]);
			if (delim && delim[0] != '\"'
				&& delim[0] != '\'' && cmd[0]->in_quote_heredoc == 0)
				replace_env_vars(cmd[0], buffer);
			write(1, buffer, ft_strlen(buffer));
		}
		close(fd);
		exit(0);
	}
	else
	{
		wait(NULL);
		signal(SIGINT, &handle_ctrlc);
		if (unlink("/tmp/heredocBURMITO") == -1)
		{
			perror("unlink");
			exit(1);
		}
		exit(0);
	}
}

void	close_input_redirect(t_cmd *cmd)
{
	dup2(cmd->stdin, STDIN_FILENO);
	close(cmd->stdin);
}
