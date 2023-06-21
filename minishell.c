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

int g_status;

int	main(int argc, char **argv, char **env)
{
	t_cmd	cmd;
	(void)argc;
	(void)argv;
	//print_minishell();
	cmd.env = env;
	
	unlink("/tmp/heredocBURMITO");
	/* signal(SIGQUIT, SIG_IGN); */
	while(1)
	{
		signal(SIGINT, &handle_ctrlc);
		cmd.prompt = get_prompt(&cmd);
		cmd.line = readline(cmd.prompt);
		if (cmd.line == NULL)
		{
            handle_ctrld();
            break;
        }
		if(ft_strncmp(cmd.line, "", 1) > 0) 
			add_history(cmd.line);
		parse_args(&cmd);
		count_pipes(&cmd);
		executor(&cmd);
		clean_tokens(&cmd);
		free(cmd.line);
		free(cmd.prompt);
	}
	rl_clear_history();
	/* free(cmd.env); */
	
    return (g_status);
}