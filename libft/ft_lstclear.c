/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/29 12:40:23 by isromero          #+#    #+#             */
/*   Updated: 2022/09/30 16:41:11 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*a;

	if (!del)
		return ;
	if (lst)
	{
		while (*lst)
		{
			a = (*lst)->next;
			(*del)((*lst)->content);
			free(*lst);
			*lst = a;
		}
	}
}
