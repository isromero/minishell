/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 17:49:22 by isromero          #+#    #+#             */
/*   Updated: 2023/05/26 17:49:22 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_prompt(t_cmd *custom_prompt)
{
	char	*username;
	char	cwd[1024];
	size_t	prompt_length;

	username = exec_custom(custom_prompt, "/usr/bin/whoami", "whoami");
	getcwd(cwd, sizeof(cwd));
	prompt_length = ft_strlen(username) + ft_strlen(cwd)
		+ ft_strlen(COLOR_GREEN) \
		+ ft_strlen(COLOR_MAGENTA) + ft_strlen(COLOR_YELLOW)
		+ ft_strlen(COLOR_CYAN)
		+ ft_strlen(COLOR_WHITE) + 16;
	custom_prompt->prompt = (char *)malloc(prompt_length * sizeof(char));
	ft_strcpy(custom_prompt->prompt, COLOR_GREEN);
	ft_strcat(custom_prompt->prompt, username);
	ft_strcat(custom_prompt->prompt, COLOR_MAGENTA);
	ft_strcat(custom_prompt->prompt, "@minishell");
	ft_strcat(custom_prompt->prompt, COLOR_YELLOW);
	ft_strcat(custom_prompt->prompt, ":");
	ft_strcat(custom_prompt->prompt, COLOR_CYAN);
	ft_strcat(custom_prompt->prompt, cwd);
	ft_strcat(custom_prompt->prompt, COLOR_WHITE);
	ft_strcat(custom_prompt->prompt, "$ ");
	free(username);
	return (custom_prompt->prompt);
}

char	*update_output(int fd)
{
	char	*line;

	line = get_next_line(fd);
	line[ft_strlen(line) - 1] = 0;
	if (!line)
		return (0);
	return (line);
}

char	*exec_custom(t_cmd *cmd, char *full, char *args)
{
	pid_t	pid;
	int		fd[2];
	char	**matrix;
	char	*out;

	matrix = NULL;
	out = NULL;
	pipe(fd);
	pid = fork();
	if (pid == 0)
	{
		close(fd[READ_END]);
		matrix = ft_split(args, ' ');
		dup2(fd[WRITE_END], STDOUT_FILENO);
		close(fd[WRITE_END]);
		if (access(full, F_OK) == 0)
			execve(full, matrix, cmd->env);
		exit(0);
	}
	close(fd[WRITE_END]);
	free(matrix);
	waitpid(pid, NULL, 0);
	out = update_output(fd[READ_END]);
	close(fd[READ_END]);
	return (out);
}

void	print_minishell(void)
{
	printf("      ____                      ____                      __");
	printf("_           ____          ___           ____      ___ \n");
	printf("     /\\___\\         ___        /\\___\\         ___        ");
	printf("/\\  \\         /\\___\\        /\\  \\         /\\___\\    /\\__\\\n");
	printf("    /::|  |        /\\  \\      /::|  |        /\\  \\      /");
	printf("::\\  \\       /:/  /        /::\\  \\       /:/  /    /:/  /\n");
	printf("   /:|:|  |        \\:\\  \\    /:|:|  |        \\:\\  \\    ");
	printf("/:/\\:\\  \\     /:/__/        /:/\\:\\  \\     /:/  /    /:/  / \n");
	printf("  /:/|:|__|__      /::\\__\\  /:/|:|  |__      /::\\__\\  _\\");
	printf(":\\ \\:\\  \\   /::\\  \\ ___   /::\\ \\:\\  \\   /:/  /    /:/  /  \n");
	printf(" /:/ |::::\\__\\  __/:/\\/__/ /:/ |:| /\\__\\  __/:/\\/__/ ");
	printf("/\\ \\:\\ \\ \\__\\ /:/\\:\\  /\\__\\ /:/\\:\\ \\:\\__\\ /:/__/    /:/__/   \n");
	printf(" \\/__/~~/:/  / /\\/:/  /    \\/__|:|/:/  / /\\/:/  /    \\:\\ ");
	printf("\\:\\ \\/__/ \\/__\\:\\/:/  / \\:\\ \\:\\ \\/__/ \\:\\  \\    \\:\\  \\   \n");
	printf("       /:/  /  \\::/__/         |:/:/  /  \\::/__/      \\:\\ \\:\\__\\ ");
	printf("       \\::/  /   \\:\\ \\:\\__\\    \\:\\  \\    \\:\\  \\  \n");
	printf("      /:/  /    \\:\\__\\         |::/  /    \\:\\__\\       \\:\\/:/ ");
	printf(" /        /:/  /     \\:\\ \\/__/     \\:\\  \\    \\:\\  \\ \n");
	printf("     /:/  /      \\/__/         /:/  /      \\/__/        \\::/  / ");
	printf("       /:/  /       \\:\\__\\        \\:\\__\\    \\:\\__\\\n");
	printf("     \\/__/                     \\/__/                     \\/__/ ");
	printf("        \\/__/         \\/__/         \\/__/     \\/__/\n");
	printf("\n\n");
	printf("%70s\n\n\n", "MADE BY ADGUTIER && ISROMERO");
	return ;
}
