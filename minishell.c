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

// /bin/comando inventado  echo $? devuelve 1
/* ls > hola | ls -l > puta | ls -a > buenas FALLA LA DEL MEDIO */
//grep $PATH???????????????????????????????????????

int	g_status;

int	main(int argc, char **argv, char **env)
{
	t_cmd	cmd;

	print_minishell();
	(void)argc;
	(void)argv;
	cmd.env = NULL;
	init_env(&cmd, env);
	unlink("/tmp/heredocBURMITO");
	while (1)
	{
		signal(SIGINT, &handle_ctrlc);
		cmd.prompt = get_prompt(&cmd);
		cmd.line = readline(cmd.prompt);
		if (cmd.line == NULL)
		{
			free(cmd.prompt);
			handle_ctrld();
			break ;
		}
		if (ft_strncmp(cmd.line, "", 1) > 0)
			add_history(cmd.line);
		if (parse_args(&cmd) == 0)
		{
			count_pipes(&cmd);
			executor(&cmd);
			clean_tokens(&cmd);
			free(cmd.no_expand_vars);
		}
		free(cmd.line);
		free(cmd.prompt);
	}
	rl_clear_history();
	free_matrix(cmd.env);
	return (g_status);
}
