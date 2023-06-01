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

int	main(int argc, char **argv, char **env)
{
	t_cmd	cmd;
	(void)argc;
	(void)argv;
	//print_minishell();
	cmd.env = env;
	while(1)
	{
		cmd.prompt = get_prompt(&cmd);
		cmd.line = readline(cmd.prompt);
		if(ft_strncmp(cmd.line, "", 1) > 0) 
			add_history(cmd.line);
		parse_args(&cmd);
		execute(&cmd);
	 	/* count_pipes(&cmd);
		count_processes(&cmd);
		execute_pipes(&cmd); */
		clean_tokens(&cmd);
		free(cmd.line);
		free(cmd.prompt);
	}
	rl_clear_history();
	free(cmd.env);
    return (0);
}