/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 19:53:28 by isma              #+#    #+#             */
/*   Updated: 2022/09/30 16:52:08 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	int		i;
	char	*m;

	i = 0;
	if (!s)
		return (0);
	m = (char *)malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!m)
		return (0);
	while (s[i])
	{
		m[i] = (*f)(i, s[i]);
		i++;
	}
	m[i] = '\0';
	return (m);
}
