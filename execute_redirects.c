/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirects.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 10:52:19 by isromero          #+#    #+#             */
/*   Updated: 2023/07/22 10:52:19 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void execute_appends(t_cmd *cmd, char *com, char **exec_args, int i)
{
	if(is_append_redirect(cmd, i) == 1)
	{
		append_redirect(cmd);
		if(!is_builtin(cmd, i))
			execve(com, exec_args, cmd->env);
		else if(is_builtin(cmd, i))
			execute_builtin(cmd, i);
		close_output_redirect(cmd);
	}
	else if(is_append_redirect(cmd, i) > 1)
	{
		append_multiple_redirect(cmd);
		if(!is_builtin(cmd, i))
			execve(com, exec_args, cmd->env);
		else if(is_builtin(cmd, i))
			execute_builtin(cmd, i);
		close_output_redirect(cmd);
	}
}

void execute_output_redirects(t_cmd *cmd, char *com, char **exec_args, int i)
{
	if(is_output_redirect(cmd, i) == 1)
	{
		output_redirect(cmd);
		if(!is_builtin(cmd, i))
			execve(com, exec_args, cmd->env);
		else if(is_builtin(cmd, i))
			execute_builtin(cmd, i);
		close_output_redirect(cmd);
	}
	else if(is_output_redirect(cmd, i) > 1)
	{
		output_multiple_redirect(cmd);
		if(!is_builtin(cmd, i))
			execve(com, exec_args, cmd->env);
		else if(is_builtin(cmd, i))
			execute_builtin(cmd, i);
		close_output_redirect(cmd);
	}
}

void execute_input_redirects(t_cmd *cmd, char *com, char **exec_args, int i)
{
	if(is_input_redirect(cmd, i) == 1)
	{
		input_redirect(cmd);
		if(!is_builtin(cmd, i))
			execve(com, exec_args, cmd->env);
		else if(is_builtin(cmd, i))
			execute_builtin(cmd, i);
		close_input_redirect(cmd);
	}
	else if(is_input_redirect(cmd, i) > 1)
	{
		input_multiple_redirect(cmd);
		if(!is_builtin(cmd, i))
			execve(com, exec_args, cmd->env);
		else if(is_builtin(cmd, i))
			execute_builtin(cmd, i);
		close_input_redirect(cmd);
	}
}

void execute_heredoc_redirects(t_cmd *cmd, char *com, char **exec_args, int i)
{
	if(is_heredoc_redirect(cmd, i) == 1)
	{
		heredoc_redirect(&cmd);
		if(!is_builtin(cmd, i))
			execve(com, exec_args, cmd->env);
		else if(is_builtin(cmd, i))
			execute_builtin(cmd, i);
		close_input_redirect(cmd);
	}
	/* No sabemos si hay que gestionarlos */
   /*  else if(is_heredoc_redirect(cmd) > 1)
	{
		heredoc_multiple_redirect(cmd);
		execve(com, exec_args, cmd->env);
		close_input_redirect(cmd);
	} */ 
}

void execute_redirects(t_cmd *cmd, char *com, char **exec_args, int i)
{
	execute_appends(cmd, com, exec_args, i);
	execute_output_redirects(cmd, com, exec_args, i);
	execute_heredoc_redirects(cmd, com, exec_args, i);
	execute_input_redirects(cmd, com, exec_args, i);
}
