/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 12:14:44 by isromero          #+#    #+#             */
/*   Updated: 2023/07/08 17:59:10 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*save_next(char	*stash)
{
	char	*next;
	int		i;
	int		j;

	if (!stash)
		return (0);
	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (!stash[i] || !stash[i + 1])
	{
		free(stash);
		return (0);
	}
	next = malloc(ft_strlen2(stash) - i + 1);
	if (!next)
		return (0);
	j = 0;
	i++;
	while (stash[i])
		next[j++] = stash[i++];
	next[j] = '\0';
	free(stash);
	return (next);
}

char	*get_the_line(char	*stash)
{
	char	*line;
	int		i;
	int		j;
	int		k;

	i = 0;
	if (!stash)
		return (0);
	while (stash[i] && stash[i] != '\n')
		i++;
	line = malloc(i + 2);
	if (!line)
		return (0);
	j = 0;
	k = 0;
	while (stash[j] && stash[j] != '\n')
		line[k++] = stash[j++];
	if (stash[j] == '\n')
		line[k++] = stash[j++];
	line[k] = '\0';
	return (line);
}

char	*get_next_line(int fd)
{
	char		*buffer;
	char		*line;
	static char	*stash[FOPEN_MAX];
	int			bytes;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (0);
	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (0);
	bytes = 1;
	while (!ft_strchr2(stash[fd], '\n') && bytes != 0)
	{
		bytes = read(fd, buffer, BUFFER_SIZE);
		if (bytes <= 0)
			break ;
		buffer[bytes] = '\0';
		stash[fd] = ft_strjoin2(stash[fd], buffer);
	}
	free(buffer);
	line = get_the_line(stash[fd]);
	stash[fd] = save_next(stash[fd]);
	return (line);
}
