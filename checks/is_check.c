/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_check.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 17:10:21 by isromero          #+#    #+#             */
/*   Updated: 2023/07/23 17:10:21 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_special(char c)
{
	return (c == INPUT_REDIRECT || c == PIPE || c == OUTPUT_REDIRECT);
}

int	is_variable(char c)
{
	return (c == VARIABLE);
}

int	is_redirects(char c)
{
	return (c == INPUT_REDIRECT || c == OUTPUT_REDIRECT);
}

int	is_redirects_double_char(char *token)
{
	return (ft_strcmp(APPEND_REDIRECT, token) == 0 || ft_strcmp(HEREDOC_REDIRECT, token) == 0);
}

int	is_argument(char c)
{
	return (c == ARGUMENT);
}
