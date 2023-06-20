/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 20:34:02 by isromero          #+#    #+#             */
/*   Updated: 2023/06/14 20:34:02 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    handle_ctrld_heredoc()
{
	ft_putchar_fd('\n', STDOUT_FILENO);
	exit(0);
}

void    handle_ctrlc_heredoc()
{
    ft_putchar_fd('\n', STDOUT_FILENO);
    exit(0);
}

void    handle_ctrlc()
{
    ft_putchar_fd('\n', STDOUT_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
    g_status = 130;
}

void    handle_ctrld()
{
    g_status = 0;
}


