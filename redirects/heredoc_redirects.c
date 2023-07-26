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

void	heredoc_child_process(t_cmd *cmd, int fd[2])
{
	char	buffer[1024];
	ssize_t	bytes_read;
	char	*delim;

	close(fd[0]);
	fd[1] = open("/tmp/heredocBURMITO", O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
	if (fd[1] == -1)
	{
		perror("open");
		exit(1);
	}
	signal(SIGINT, SIG_IGN);
	signal(SIGINT, &handle_ctrlc_heredoc);
	while (1)
	{
		if (heredoc_content(cmd, fd[1]) == 1)
			break;
	}
	close(fd[1]);
	int new_fd = open("./expanded.txt", O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
	if (new_fd == -1)
	{
		perror("open");
		exit(1);
	}
	int	old_fd = open("/tmp/heredocBURMITO", O_RDONLY);
	ft_memset(buffer, 0, sizeof(buffer));
	bytes_read = read(old_fd, buffer, sizeof(buffer));
	while ((bytes_read) > 0)
	{
		delim = find_heredoc_delim(cmd);
		if (delim && cmd->in_quote_delim_heredoc == 0)
			replace_env_vars(cmd, buffer);
		write(new_fd, buffer, ft_strlen(buffer));
		bytes_read = read(old_fd, buffer, sizeof(buffer));
	}
	close(new_fd);
	close(old_fd);
	exit(0);
}


void	heredoc_redirect(t_cmd *cmd)
{
    int fd[2];
    pid_t pid;

    cmd->in_quote_heredoc = 0;
    if (pipe(fd) == -1)
    {
        perror("pipe");
        exit(1);
    }
    pid = fork();
    signal(SIGINT, SIG_IGN);
    if (pid == -1)
    {
        perror("fork");
        exit(1);
    }
    else if (pid == 0)
		heredoc_child_process(cmd, fd);
    else
    {
        close(fd[1]);
        wait(NULL);
        signal(SIGINT, &handle_ctrlc);
    }
}

void	close_input_redirect(t_cmd *cmd)
{
	dup2(cmd->stdin, STDIN_FILENO);
	close(cmd->stdin);
}
