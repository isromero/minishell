/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 18:16:30 by isromero          #+#    #+#             */
/*   Updated: 2024/03/29 20:02:55 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*save_next(char *stash)
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

char	*get_the_line(char *stash)
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
	static char	*stash;
	int			bytes;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (0);
	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (0);
	bytes = 1;
	while (!ft_strchr2(stash, '\n') && bytes != 0)
	{
		bytes = read(fd, buffer, BUFFER_SIZE);
		if (bytes <= 0)
			break ;
		buffer[bytes] = '\0';
		stash = ft_strjoin2(stash, buffer);
	}
	free(buffer);
	line = get_the_line(stash);
	stash = save_next(stash);
	return (line);
}
