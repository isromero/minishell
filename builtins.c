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

#include "minishell.h"

int	is_builtin(t_cmd *cmd, int n_token)
{
	if (ft_strcmp(cmd->token[n_token], "echo") == 0)
		return(1);
	else if (ft_strcmp(cmd->token[n_token], "cd") == 0)
		return(1);
	else if (ft_strcmp(cmd->token[n_token], "pwd") == 0)
		return(1);
	else if (ft_strcmp(cmd->token[n_token], "export") == 0)
		return(1);
	else if (ft_strcmp(cmd->token[n_token], "unset") == 0)
		return(1);
	else if (ft_strcmp(cmd->token[n_token], "env") == 0)
		return(1);
	else if (ft_strcmp(cmd->token[n_token], "exit") == 0)
		return(1);
	return (0);
}

//INTENTO DE OPTIMIZAR PERO PETA CON COMILLAS , DEJO PARA OPTIMIZAR TRNAUQILAMENTE
int ft_echo(t_cmd *cmd, int echo_token)
{
	int first_echo_token;
	int	first_iteration;

	first_echo_token = echo_token;
	first_iteration = 0;
	echo_token++; // Avanzar a la posición después de "echo"
	if (ft_strcmp(cmd->token[0], "echo") == 0 && ft_strcmp(cmd->token[1], "$?") == 0) 
	{
		ft_putnbr_fd(g_status, STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
		return (0);
	}
	if(ft_strcmp(cmd->token[first_echo_token + 1], "-n") == 0)
		echo_token++;
	while (cmd->token[echo_token])
	{
		 if(cmd->no_expand_vars[echo_token] == 0)
			replace_vars(cmd, &cmd->token[echo_token]);
		if(is_special(cmd->token[echo_token][0]) && (cmd->in_single_quote == 0 || cmd->in_double_quote == 0))
			break ;
		if(first_iteration != 0)
			printf(" %s", cmd->token[echo_token]);
		else if(first_iteration == 0)
		{
			printf("%s", cmd->token[echo_token]);
			first_iteration++;
		}
		echo_token++;
	}
	if (ft_strcmp(cmd->token[first_echo_token + 1], "-n") != 0)
		printf("\n");
	return (echo_token);
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
	int i = 0;
	int len_of_env = 0;
	int	pos_var = 0;
	char **new_env = NULL;
	
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

int len_var_in_env(t_cmd *cmd, char *token)
{
	int    i;
	int    eqpos;

	i = 0;
	eqpos = 0;
	while (token[eqpos] != '=')
		eqpos++;
	while (cmd->env[i])
	{
		if (ft_memcmp(cmd->env[i], token, eqpos + 1) == 0)
			return (i);
		i++;
	}
	return (0);
}

int var_exists(t_cmd *cmd, char *token)
{
	int    i;
	int    eqpos;

	i = 0;
	eqpos = 0;
	while (token[eqpos] != '=')
		eqpos++;
	while (cmd->env[i])
	{
		if (ft_memcmp(cmd->env[i], token, eqpos + 1) == 0)
			return (1);
		i++;
	}
	return (0);
}


bool compare_variable(const char* variable, const char* name)
{
	int i = 0;

	while (variable[i] != '=' && name[i] != '\0')
	{
		if (variable[i] != name[i])
			return false;
		i++;
	}
	return (variable[i] == '=' && name[i] == '\0');
}

/* CHECKEAR GESTIÓN DE UNSET SIN ARGUMENTOS */

void ft_unset(t_cmd *cmd, int unset_token)
{
	// Meter perrors con errores de argumentos(bad identifiers)
	int i = 0, j = 0;
	int len_of_env = 0;
	char **new_env = NULL;

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

void execute_builtin_exit(t_cmd *cmd, int exit_code)
{
	// Realiza cualquier limpieza necesaria antes de salir
	// Esto vale?????????????????????????????
	clean_tokens(cmd);
	free(cmd->line);
	free(cmd->prompt);
	free_matrix(cmd->env);
	free(cmd->no_expand_vars);
	/* HABRÍA QUE LIBERAR CMD->ENV */
	// Salir del programa con el código de salida especificado
	exit(exit_code);
}

void	ft_exit(t_cmd *cmd, int exit_token) // Con textos de argumentos creo que hay que devolver 2
{
	int	exit_code;

	if(is_number(cmd->token[exit_token + 1]) && ft_atoi(cmd->token[exit_token + 1]) >= INT_MIN && ft_atoi(cmd->token[exit_token + 1]) <= INT_MAX) // Si es un número cualquiera entonces es que hay un argumento número y:
	{
		exit_code = ft_atoi(cmd->token[exit_token + 1]);
		if(exit_code >= 0 && exit_code <= 255) // Habría que checkear si le pasas exit "numero random" debe devolver ese numero random o 1 predeterminado PENDIENTE
			execute_builtin_exit(cmd, exit_code);
		else if(exit_code < 0 || exit_code > 255) // Esto significa que está fuera de los límites del exit, y bash hace el % del valor pasado para hacer que esté en el rango válido
		{
			exit_code = (exit_code % 256);
			execute_builtin_exit(cmd, exit_code);
		}
	}
	else
		execute_builtin_exit(cmd, 1); // En el caso de que no sea ningun número ni nada devolvemos un error predeterminado, el 1
}
