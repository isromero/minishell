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
	else
	{
		free(line);
		printf("minishell: warning: here-document delimited by ");
		printf("end-of-file (wanted '%s')\n", find_heredoc_delim(cmd));
		exit(0);
	}
	return (0);
}

void	heredoc_write_replaced(t_cmd *cmd, int fd, int new_fd)
{
	char	buffer[1024];
	ssize_t	bytes_read;
	char	*delim;

	fd = open("/tmp/heredoc", O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		exit(1);
	}
	ft_memset(buffer, 0, sizeof(buffer));
	bytes_read = read(fd, buffer, sizeof(buffer));
	while ((bytes_read) > 0)
	{
		delim = find_heredoc_delim(cmd);
		if (delim && cmd->in_quote_delim_heredoc == 0)
			search_var_replace(cmd, buffer);
		write(new_fd, buffer, ft_strlen(buffer));
		bytes_read = read(fd, buffer, sizeof(buffer));
	}
}

void	heredoc_child_process(t_cmd *cmd, int fd)
{
	int	new_fd;

	signal(SIGINT, SIG_IGN);
	signal(SIGINT, &handle_ctrlc_heredoc);
	while (1)
	{
		if (heredoc_content(cmd, fd) == 1)
			break ;
	}
	close(fd);
	new_fd = open("/tmp/heredoc_expanded",
			O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
	if (new_fd == -1)
	{
		perror("open");
		exit(1);
	}
	heredoc_write_replaced(cmd, fd, new_fd);
	close(new_fd);
	close(fd);
	exit(0);
}

void	heredoc_redirect(t_cmd *cmd)
{
	pid_t	pid;
	int		fd;

	fd = open("/tmp/heredoc", O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
	if (fd == -1)
	{
		perror("open");
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
		close(fd);
		wait(NULL);
		signal(SIGINT, &handle_ctrlc);
	}
}
