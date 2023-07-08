/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 19:53:58 by isromero          #+#    #+#             */
/*   Updated: 2023/07/08 17:14:38 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			i;
	unsigned char	*p;
	unsigned char	*b;

	p = (unsigned char *) s1;
	b = (unsigned char *) s2;
	i = 0;
	while (i < n)
	{
		if (p[i] == b[i])
			i++;
		else
			return (p[i] - b[i]);
	}
	return (0);
}
