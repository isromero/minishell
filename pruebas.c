/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pruebas.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 11:29:46 by isromero          #+#    #+#             */
/*   Updated: 2023/05/12 11:29:46 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cmd_ls(t_cmd *cmd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
        exit(1);
	}
	else if(pid == 0)
		execvp(cmd->token[0], cmd->token);
	else
		wait(NULL);
}

void	cmd_echo(t_cmd *cmd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
        exit(1);
	}
	else if(pid == 0)
		printf("%s\n", (cmd->token[1]));
		//Introducir función para manejar el uso de "" y ''
	else
		wait(NULL);
}

void	cmd_cd(t_cmd *cmd)
{
	pid_t	pid;
	char cwd[1024];

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
        exit(1);
	}
	else if(pid == 0)
	{
		getcwd(cwd, sizeof(cwd));
		chdir(cmd->token[1]);
	}
	else
		wait(NULL);
}

void	parse_args(t_cmd *cmd)
{
	int	i;

	i = 0;
	cmd->token = ft_split(cmd->line, ' ');
	while(cmd->token[i])
		i++;
	cmd->n_tokens = i;
}

void execute(t_cmd *cmd)
{
    if (strcmp(cmd->token[0], "ls") == 0)
		cmd_ls(cmd);
    else if (strcmp(cmd->token[0], "cd") == 0)
        cmd_cd(cmd);
    else if (strcmp(cmd->token[0], "echo") == 0)
        cmd_echo(cmd);
    else
        printf("Comando desconocido\n");
}

int	main(void)
{
	t_cmd		cmd;
	t_command	*command;

	cmd.token = NULL;
	while(1)
	{
		cmd.line = readline("Prompt > ");
		//Una vez se utiliza cd no funciona exit
		if(!strcmp("exit", cmd.line))
			return (0);
		parse_args(&cmd);
		execute(&cmd);
		free(cmd.line);
		free(cmd.token);
	}
    return (0);
}