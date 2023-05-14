/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/14 10:03:41 by isromero          #+#    #+#             */
/*   Updated: 2023/05/14 10:03:41 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft/libft.h"

typedef struct s_cmd
{
	char 	*line;
	char	**token;
	int		n_tokens;

}	t_cmd;

typedef struct s_command
{
	char *name;
	void(*func)(t_cmd *);
}	t_command;

void	cmd_ls(t_cmd *cmd);
void	cmd_cd(t_cmd *cmd);
void	cmd_echo(t_cmd *cmd);
t_command commands[] =
{
    {"ls", cmd_ls},
    {"cd", cmd_cd},
    {"echo", cmd_echo},
	{NULL, NULL}
};



#endif