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

/// MIRAR BACKUS-naur.... 

#include "minishell.h"
/* 									gsg	> hhdfhd.txt          ls -l | grep u > hola.txt > pepe.txt > rtewkig.txt > sfsf.txt
															me los ejecutas todo hasta > el nombre lo pillas hasta que encuentres un pipe un NULL */
														

/* 1. Me ejecutas todos los comandos hasta el redirect.
2. El stdout se mete en un archivo X nuevo.
3. Seguimos recorriendo hasta | o NULL
4. PERO si te meten otro redirect se crean archivos pero finaliza todo en el último */


int find_first_output_redirect(t_cmd *cmd)
{
	int len;

	len = 0;
	while(cmd->token[len] != NULL)
	{
		if(cmd->token[len][0] == OUTPUT_REDIRECT) /* Meter más tipos de redirects */
			return (len);
		len++;
	}
	return(0);
}

int is_redirect(t_cmd *cmd)
{
	int len;
	int n_redirects;
	

	len = 0;
	n_redirects = 0;
	while(cmd->token[len] != NULL)
	{
		if(cmd->token[len][0] == OUTPUT_REDIRECT) /* Meter más tipos de redirects */
			n_redirects++;
		len++;
	}
	if(n_redirects == 1)
		return (1);
	else if(n_redirects > 1)
		return (n_redirects);
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

int find_last_output_redirect(t_cmd *cmd)
{
	int len;

	len = cmd->n_tokens - 2;
	while(len >= 0)
	{
		printf("holasdfdsfsd: %d\n", len);
		if(cmd->token[len][0] == '>') /* Meter más tipos de redirects */
			return (len);
		len--;
	}
	return(0);
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
			open(cmd->token[i + 1], O_CREAT, S_IRUSR | S_IWUSR);
		i++;
	}
    cmd->stdout = dup(STDOUT_FILENO);
    dup2(fd, STDOUT_FILENO); 
    close(fd);
}

void close_redirect(t_cmd *cmd)
{
	dup2(cmd->stdout, STDOUT_FILENO);
    close(cmd->stdout);
}

