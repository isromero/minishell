/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 20:00:18 by isromero          #+#    #+#             */
/*   Updated: 2024/03/29 20:00:19 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_ctrld_heredoc(int sig)
{
	(void)sig;
	ft_putchar_fd('\n', STDOUT_FILENO);
	exit(0);
}

void	handle_ctrlc_heredoc(int sig)
{
	(void)sig;
	ft_putchar_fd('\n', STDOUT_FILENO);
	exit(0);
}

void	handle_ctrlc(int sig)
{
	(void)sig;
	ft_putchar_fd('\n', STDOUT_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_status = 130;
}

void	handle_ctrlc2(int sig)
{
	(void)sig;
}

void	handle_ctrld(int sig)
{
	(void)sig;
	g_status = 0;
}
