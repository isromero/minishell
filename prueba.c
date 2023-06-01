/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prueba.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 20:25:01 by isromero          #+#    #+#             */
/*   Updated: 2023/06/01 20:25:01 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void    execute_first_pipes(t_cmd *cmd, int i, int count_pipes, int count_pids)
{
    pid_t   pid[cmd->n_processes];
    int     fd[cmd->n_pipes][2];

    pid[count_pids] = fork();
    if(pid[count_pids] == 0)
    {
        com = command_dir(cmd, cmd->token[i]);
        if (com != NULL)
        {
            exec_args = (char **)malloc(sizeof(char *) * (cmd->n_tokens - i + 1));
            j = i;
            while(j < cmd->n_tokens - 1 && cmd->token[j][0] != '|')
            {
                exec_args[j - i] = cmd->token[j];
                j++;
            }
            exec_args[j - i] = NULL;
            close(fd[count_pipes][READ_END]);
            dup2(fd[count_pipes][WRITE_END], STDOUT_FILENO);
            close(fd[count_pipes][WRITE_END]);
            execve(com, exec_args, NULL);
            perror("");
            exit(0);
        }
        if (!com && !is_argument(cmd->token[i][0])) 
        {
            perror("command not found");
            exit(1);
        }
        count_pids++;
    }
    close(fd[count_pipes][WRITE_END]);
	free(exec_args);
}