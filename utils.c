/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 17:45:41 by isromero          #+#    #+#             */
/*   Updated: 2023/05/26 17:45:41 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strtok(char *str, const char *delim)
{
	static char	*save = NULL;
	char		*token;

	if (str != NULL)
		save = str;
	if (save == NULL || *save == '\0')
		return (NULL);
	while (*save && ft_strchr(delim, *save) != NULL)
		save++;
	if (*save == '\0')
		return (NULL);
	token = save;
	while (*save && ft_strchr(delim, *save) == NULL)
		save++;
	if (*save != '\0')
		*save++ = '\0';
	return (token);
}

char	**ft_strstrdup(char **strstr)
{
	char **dup;
	int i;
	int j;

	j = 0;
	while (strstr && strstr[j])
		j++;
	dup = malloc((j + 1) * sizeof(char *));
	if (!dup)
		return (NULL);
	i = 0;
	while (strstr[i])
	{
		dup[i] = ft_strdup(strstr[i]);
		i++;
	}
	dup[i] = NULL;
	return (dup);
}

int	is_number(const char *str)
{
	if (str == NULL || *str == '\0')
		return (0);
	while (*str != '\0')
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

void	free_matrix(char **matrix)
{
	int	i;

	i = 0;
	while (matrix[i] != NULL)
	{
		free(matrix[i]);
		i++;
	}
	free(matrix);
}
