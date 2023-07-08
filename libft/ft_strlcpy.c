/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/13 17:55:13 by isromero          #+#    #+#             */
/*   Updated: 2023/07/08 17:15:19 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	unsigned int	a;

	a = 0;
	if (size != 0)
	{
		while (src[a] && a < size - 1)
		{
			dst[a] = src[a];
			a++;
		}
		dst[a] = 0;
	}
	return (ft_strlen(src));
}
