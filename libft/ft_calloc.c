/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/19 18:17:05 by isromero          #+#    #+#             */
/*   Updated: 2023/07/08 17:14:06 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	char	*a;
	size_t	i;

	i = 0;
	if (size > 2147483647 || count > 2147483647)
		return (0);
	a = malloc(count * size);
	if (!a)
		return (0);
	while (i < size * count)
	{
		a[i] = 0;
		i++;
	}
	return (a);
}
