/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 21:11:23 by isromero          #+#    #+#             */
/*   Updated: 2023/06/04 21:11:23 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_first_output_redirect(t_cmd *cmd)
{
	int len;

	len = 0;
	while(cmd->token[len] != NULL)
	{
		if(cmd->token[len][0] == OUTPUT_REDIRECT)
			return (len);
		len++;
	}
	return(0);
}

int	find_last_output_redirect(t_cmd *cmd)
{
	int len;

	len = cmd->n_tokens - 2;
	while(len >= 0)
	{
		if(cmd->token[len][0] == OUTPUT_REDIRECT)
			return (len);
		len--;
	}
	return(0);
}

int	find_first_append_redirect(t_cmd *cmd)
{
	int len;

	len = 0;
	while(cmd->token[len] != NULL)
	{
		if(ft_strcmp(cmd->token[len], APPEND_REDIRECT) == 0)
			return (len);
		len++;
	}
	return(0);
}

int	find_last_append_redirect(t_cmd *cmd)
{
	int len;

	len = cmd->n_tokens - 2;
	while(len >= 0)
	{
		if(ft_strcmp(cmd->token[len], APPEND_REDIRECT) == 0)
			return (len);
		len--;
	}
	return(0);
}

int	is_output_redirect(t_cmd *cmd, int len)
{
	int n_redirects;

	n_redirects = 0;
	while(cmd->token[len] != NULL/*  && cmd->token[len][0] != '|' */)
	{
		if(cmd->token[len][0] == OUTPUT_REDIRECT)
			n_redirects++;
		len++;
	}
	if(n_redirects == 1)
		return (1);
	else if(n_redirects > 1)
		return (n_redirects);
	return(0);
}

int	is_append_redirect(t_cmd *cmd, int len)
{
	int n_redirects;

	n_redirects = 0;
	while(cmd->token[len] != NULL)
	{
		if(ft_strcmp(cmd->token[len], APPEND_REDIRECT) == 0)
			n_redirects++;
		len++;
	}
	if(n_redirects == 1)
		return (1);
	else if(n_redirects > 1)
		return (n_redirects);
	return(0);
}

int	is_redirect_pipes(t_cmd *cmd, int i)
{
	int	n_redirects;

	n_redirects = 0;
	while(cmd->token[i] != NULL /* && cmd->token[i][0] != '|' */)
	{
		if(cmd->token[i][0] == OUTPUT_REDIRECT)
			n_redirects++;
		i++;
	}
	if(n_redirects >= 1)
		return(n_redirects);
	return(0);
}

void	output_redirect(t_cmd *cmd)
{
	int	fd;
	int	len;

	len = find_first_output_redirect(cmd);
	// Pasarle el token que tiene el nombre correspondiente
	// O_CREAT crea el archivo si no existe
	// O_TRUNC trunca el archivo a tamaño cero si ya existe
	// S_IRUSR | S_IWUSR establece permisos para ser legible y escribible solo por propietario
	fd = open(cmd->token[len + 1], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd == -1)
	{
		perror("");
		return ;
	}
	// Guardamos el fd de la salida actual para en otro momento poder restaurar la stdout original más adelante
	cmd->stdout = dup(STDOUT_FILENO);
	// Redireccionar la stdout al archivo de destino lo que hace que cualquier salida que se enviase a la stdout ahora se escriba en el archivo seleccionado
	dup2(fd, STDOUT_FILENO); 
	close(fd);
	// Para volver a la stdout original habíamos guardado con dup el estado por lo que ahora duplicamos la stdout original en nuestro stdout
}

void	output_multiple_redirect(t_cmd *cmd)
{
	int	fd;
	int	len;
	int	i;

	i = 0;
	len = find_last_output_redirect(cmd);
	fd = open(cmd->token[len + 1], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd == -1)
	{
		perror("");
		return ;
	}
	while(i < len)
	{
		if(cmd->token[i][0] == OUTPUT_REDIRECT)
			open(cmd->token[i + 1], O_CREAT | O_WRONLY | O_TRUNC , S_IRUSR | S_IWUSR);
		i++;
	}
	cmd->stdout = dup(STDOUT_FILENO);
	dup2(fd, STDOUT_FILENO); 
	close(fd);
}

void	append_redirect(t_cmd *cmd)
{
	int	fd;
	int	len;

	len = find_first_append_redirect(cmd);
	fd = open(cmd->token[len + 1], O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
	if (fd == -1)
	{
		perror("");
		return ;
	}
	cmd->stdout = dup(STDOUT_FILENO);
	dup2(fd, STDOUT_FILENO); 
	close(fd);
}

void	append_multiple_redirect(t_cmd *cmd)
{
	int	fd;
	int	len;
	int	i;

	i = 0;
	len = find_last_append_redirect(cmd);
	fd = open(cmd->token[len + 1], O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
	if (fd == -1)
	{
		perror("");
		return ;
	}
	while(i < len)
	{
		if(ft_strcmp(cmd->token[len], APPEND_REDIRECT) == 0)
			open(cmd->token[i + 1], O_CREAT | O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR);
		i++;
	}
	cmd->stdout = dup(STDOUT_FILENO);
	dup2(fd, STDOUT_FILENO); 
	close(fd);
}

void close_output_redirect(t_cmd *cmd)
{
	dup2(cmd->stdout, STDOUT_FILENO);
	close(cmd->stdout);
}
