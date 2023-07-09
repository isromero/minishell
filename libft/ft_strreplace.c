/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strreplace.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 21:52:39 by isromero          #+#    #+#             */
/*   Updated: 2023/07/08 21:52:39 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
char	*ft_strreplace(char *str, char *search, char *replace)
{
	char	*result;
	char	*dest;
	char	*p;

	p = str;
	result = (char *)malloc(ft_strlen(str) + 1 + \
	(ft_strlen(replace) - ft_strlen(search)));
	if (!result)
		return (NULL);
	dest = result;
	while (*p)
	{
		if (ft_strncmp(p, search, ft_strlen(search)) == 0)
		{
			ft_strncpy(dest, replace, ft_strlen(replace));
			dest += ft_strlen(replace);
			p += ft_strlen(search);
		}
		else
			*dest++ = *p++;
	}
	*dest = '\0';
	return (result);
}

