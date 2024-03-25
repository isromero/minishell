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

// echo hola << a -> Tiene que parar en el hola
// export "" test=a
// Tiene que exportarlo: export var=te"st
// bool quitar de todo el código

int	g_status;

void	init_minishell(t_cmd *cmd, int sig)
{
	while (1)
	{
		signal(SIGINT, &handle_ctrlc);
		signal(SIGQUIT, SIG_IGN);
		cmd->prompt = get_prompt(cmd);
		cmd->line = readline(cmd->prompt);
		if (cmd->line == NULL)
		{
			free(cmd->prompt);
			handle_ctrld(sig);
			break ;
		}
		if (ft_strncmp(cmd->line, "", 1) > 0)
			add_history(cmd->line);
		if (parse_args(cmd) == 0)
		{
			count_pipes(cmd);
			executor(cmd);
			clean_tokens(cmd);
			free(cmd->no_expand_vars);
		}
		free(cmd->line);
		free(cmd->prompt);
	}
}

int	main(int argc, char **argv, char **env)
{
	t_cmd	cmd;
	int		sig;

	(void)argc;
	(void)argv;
	sig = 0;
	cmd.env = NULL;
	/* print_minishell(); */
	init_env(&cmd, env);
	unlink("/tmp/heredoc");
	unlink("/tmp/heredoc_expanded");
	init_minishell(&cmd, sig);
	rl_clear_history();
	free_matrix(cmd.env);
	return (g_status);
}
