/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/14 17:54:38 by isromero          #+#    #+#             */
/*   Updated: 2023/07/08 17:14:41 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t					i;
	unsigned const char		*c;
	unsigned char			*p;

	i = 0;
	p = dst;
	c = src;
	if (p == NULL && c == NULL)
		return (NULL);
	while (i < n)
	{
		p[i] = c[i];
		i++;
	}
	return (dst);
}
