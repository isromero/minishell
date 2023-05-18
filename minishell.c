/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 17:20:04 by isromero          #+#    #+#             */
/*   Updated: 2023/05/18 17:20:04 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse_args(t_cmd *cmd)
{
	int	i;

	i = 0;
	//Se necesitan más funcionalidades, a parte de dividir en tokens
	cmd->token = ft_split(cmd->line, ' ');
	while(cmd->token[i])
		i++;
	cmd->n_tokens = i;
}

char *get_prompt(t_cmd *cmd)
{
	char* args[] = { "bash", "-c", "echo -n \"$(whoami)@minishell:\"; pwd", NULL };
	return(execve("bash", args));
}

int	main(int argc, char **argv)
{
	t_cmd	cmd;
	char	*prompt;
	
	parse_args(&cmd);
	prompt = get_prompt(&cmd);
	while(1)
	{
		cmd.line = readline(prompt);
		free(cmd.line);
	}
    return (0);
}