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

#include "pruebas.h"

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
	int		i;
	int		j;
	int		k;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
        exit(1);
	}
	else if(pid == 0)
	{
		i = 1;
		j = 0;
		k = ft_strlen(cmd->token);
		while(cmd->token[i])
		{
			printf("%s ", (cmd->token[i]));
			i++;
		}
		printf("\n");
		//Introducir función para manejar el uso de "" y ''
	}
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
	//Se necesitan más funcionalidades, a parte de dividir en tokens
	cmd->token = ft_split(cmd->line, ' ');
	while(cmd->token[i])
		i++;
	cmd->n_tokens = i;
}

void	cmd_pwd(t_cmd *cmd)
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
		execvp(cmd->token[0], cmd->token);
	}
	else
		wait(NULL);
}


void execute(t_cmd *cmd, char **path)
{
	int	i;

	i = 0;
	while(path[i])
	{
		//Incluir la búsqueda de comandos en PATH y borrar lo de abajo
		i++;
	}
    if (strcmp(cmd->token[0], "ls") == 0)
		cmd_ls(cmd);
    else if (strcmp(cmd->token[0], "cd") == 0)
        cmd_cd(cmd);
    else if (strcmp(cmd->token[0], "echo") == 0)
        cmd_echo(cmd);
	//Una vez se utiliza cd no funciona exit a no ser que repitas varias veces
	else if (strcmp(cmd->token[0], "exit") == 0)
		exit(0);
	else if (strcmp(cmd->token[0], "pwd") == 0)
		cmd_pwd(cmd);
    else
        printf("Comando desconocido\n");
}


int	main(char **env)
{
	t_cmd		cmd;
	t_command	*command;
	char *pwd;
	char cwd[1024];

	pwd = getcwd(cwd, sizeof(cwd));
	cmd.token = NULL;
	env = ft_split(getenv("PATH"), ':');
	while(1)
	{
		cmd.line = readline(pwd);
		parse_args(&cmd);
		execute(&cmd, env);
		free(cmd.line);
		free(cmd.token);
	}
    return (0);
}