/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 17:47:35 by isromero          #+#    #+#             */
/*   Updated: 2023/05/26 17:47:35 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_builtin(t_cmd *cmd, int n_token)
{
    if (ft_strcmp(cmd->token[n_token], "cd") == 0)
    {
        ft_cd(cmd, n_token);
        n_token++;
    }
    else if (ft_strcmp(cmd->token[n_token], "pwd") == 0)
        ft_pwd(cmd);
    else if (ft_strcmp(cmd->token[n_token], "env") == 0)
        ft_env(cmd);
    else if (ft_strcmp(cmd->token[n_token], "export") == 0 && cmd->token[n_token + 1] != NULL && ft_strchr(cmd->token[n_token + 1], '='))
    {
        ft_export(cmd, n_token);
        n_token++;
    }
    else if (ft_strcmp(cmd->token[n_token], "unset") == 0 && cmd->token[n_token + 1] != NULL)
    {
        ft_unset(cmd, n_token);
        n_token++;
    }
    else if (ft_strcmp(cmd->token[n_token], "echo") == 0)
        n_token = ft_echo(cmd, n_token);
    else if (ft_strcmp(cmd->token[n_token], "exit") == 0)
        ft_exit(cmd, n_token);
    else
        return;
}

int ft_echo(t_cmd *cmd, int echo_token) 
{ 
    int i = 1; 
   if (ft_strcmp(cmd->token[0], "echo") == 0 && ft_strcmp(cmd->token[1], "$?") == 0) 
    { 
        ft_putnbr_fd(g_status, STDOUT_FILENO); 
        ft_putchar_fd('\n', STDOUT_FILENO); 
        return (0); 
    }
	if(ft_strcmp(cmd->token[echo_token + 1], "-n") == 0)
		i++;
    while (cmd->token[i]) 
    {
        if (cmd->no_expand_vars[i] == 0) 
            replace_vars(cmd, &cmd->token[i]); 
		if(is_special(cmd->token[echo_token][0]) && (cmd->in_single_quote == 0 || cmd->in_double_quote == 0))
			break ;
        printf("%s", cmd->token[i]); 
        if (cmd->token[i + 1] != NULL) 
            printf(" "); 
        i++; 
    } 
    if (ft_strcmp(cmd->token[echo_token + 1], "-n") != 0) 
        printf("\n"); 
    return (i); 
}

void	ft_cd(t_cmd *cmd, int cd_token)
{
	char	*path;
	char	cwd[1024];
	char	*oldpwd;

	path = cmd->token[cd_token + 1];
	oldpwd = ft_getenv("PWD", cmd->env);
	//chdir falla al cambiar al directorio especificado, no devuelve 0 
	if (chdir(path) != 0)
		printf("-minishell: cd: %s: Not a directory\n", path);
	getcwd(cwd, sizeof(cwd));
	setenv(oldpwd, cwd, 1);
	free(oldpwd);
}

/* CADA VEZ QUE SE LLAMA O CUANDO HACEMOS GETENV LA ULTIMA VARIABLE TIENE QUE CONTENER EL ULTIMO COMANDO ASI QUE: _=/usr/bin/env */
void ft_env(t_cmd *cmd)
{
	int pos_var;
	int i;
	int size;

	i = 0;
	size = 0;
	pos_var = len_var_in_env(cmd, "_=./minishell");
	if (!cmd->env || !cmd->env[0])
		return ;
	free(cmd->env[pos_var]);
	cmd->env[pos_var] = ft_strreplace(cmd->env[pos_var], cmd->env[pos_var], "_=/usr/bin/env");
	while (cmd->env[i] != NULL)
	{
		size = ft_strlen(cmd->env[i]);
		// Este strcmp evita imprimir variables de entorno que tengan un valor de dos comillas simples consecutivas (''), ya que generalmente 
		// se considera que estas no contienen información útil y podrían ser resultado de algún error o incoherencia en el sistema
		if (ft_strcmp(cmd->env[i] + size - 2, "''") != 0)
			printf("%s\n", cmd->env[i]);
		i++;
	}
}

void	ft_pwd(t_cmd *cmd)
{
	char	dir[1024];
	char	*pwd;

	pwd = ft_getenv("$PWD", cmd->env);
	if (getcwd(dir, sizeof(dir)) != NULL)
		printf("%s\n", dir);
	else if (pwd)
		printf("%s\n", pwd);
	else
		perror("-minishell: ");
	free (pwd);
}

/*	CHECKEAR GESTIÓN DE EXPORT SIN ARGUMENTOS */
void ft_export(t_cmd *cmd, int export_token) 
{
	int i;
	int len_of_env;
	int	pos_var;
	char **new_env;
	
	new_env = NULL;
	i = 0;
	len_of_env = 0;
	pos_var = 0;
	while (cmd->env[len_of_env] != NULL)
		len_of_env++;
	if(var_exists(cmd, cmd->token[export_token + 1]) == 1)
		len_of_env--;
	new_env = (char **)malloc(sizeof(char *) * (len_of_env + 2));
	i = 0;
	while (cmd->env[i] != NULL)
	{
		new_env[i] = cmd->env[i];
		i++;
	}
	if(var_exists(cmd, cmd->token[export_token + 1]) == 1)
	{
		pos_var = len_var_in_env(cmd, cmd->token[export_token + 1]);
		free(new_env[pos_var]);
		new_env[pos_var] = ft_strdup(cmd->token[export_token + 1]);
	}
	else if(var_exists(cmd, cmd->token[export_token + 1]) == 0)
		new_env[len_of_env] = ft_strdup(cmd->token[export_token + 1]);
	new_env[len_of_env + 1] = NULL;
	free(cmd->env);
	cmd->env = new_env;
}

/* CHECKEAR GESTIÓN DE UNSET SIN ARGUMENTOS */

void ft_unset(t_cmd *cmd, int unset_token)
{
	// Meter perrors con errores de argumentos(bad identifiers)
	int i;
	int j;
	int len_of_env;
	char **new_env;

	new_env = NULL;
	i = 0;
	j = 0;
	len_of_env = 0;
	while (cmd->env[len_of_env] != NULL)
		len_of_env++;
	new_env = (char **)malloc(sizeof(char *) * (len_of_env + 1));
	if(new_env == NULL)
		return ;
	while (cmd->env[i] != NULL)
	{
		if (!compare_variable(cmd->env[i], cmd->token[unset_token + 1]))
		{
			new_env[j] = ft_strdup(cmd->env[i]);
			j++;
		}
		i++;
	}
	new_env[j] = NULL;
	cmd->env = new_env;
}
