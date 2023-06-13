/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adgutier <adgutier@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 19:37:01 by adgutier          #+#    #+#             */
/*   Updated: 2023/06/13 19:37:01 by adgutier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//redirigimos al input
// si es comando que no puede recivir el input lo ejecutamos a pelo.
//La redirección < hola.txt intenta redirigir la entrada estándar del comando ls desde el archivo "hola.txt". 
//Sin embargo, como el comando ls no está diseñado para leer datos de entrada estándar, no se producirá ningún cambio en el comportamiento del comando.

int     is_input_redirect(t_cmd *cmd)
{
	int len;
	int n_redirects;
	

	len = 0;
	n_redirects = 0;
	while(cmd->token[len] != NULL)
	{
		if(cmd->token[len][0] == INPUT_REDIRECT) /* Meter más tipos de redirects */
			n_redirects++;
		len++;
	}
	if(n_redirects == 1)
		return (1);
	else if(n_redirects > 1)
		return (n_redirects);
	return(0);
}

int     find_first_input_redirect(t_cmd *cmd)
{
	int len;

	len = 0;
	while(cmd->token[len] != NULL)
	{
		if(cmd->token[len][0] == INPUT_REDIRECT) /* Meter más tipos de redirects */
			return (len);
		len++;
	}
	return(0);
}

int find_last_input_redirect(t_cmd *cmd)
{
	int len;

	len = cmd->n_tokens - 2;
	while(len >= 0)
	{
		if(cmd->token[len][0] == '>') /* Meter más tipos de redirects */
			return (len);
		len--;
	}
	return(0);
}

void    input_redirect(t_cmd *cmd)
{
	int	fd;
	int	len;

	len = find_first_input_redirect(cmd);
	// Pasarle el token que tiene el nombre correspondiente
	// O_CREAT crea el archivo si no existe
	// O_TRUNC trunca el archivo a tamaño cero si ya existe
	// S_IRUSR | S_IWUSR establece permisos para ser legible y escribible solo por propietario
	fd = open(cmd->token[len + 1], O_RDONLY | S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        perror("");
        return ;
    }
	// Guardamos el fd de la salida actual para en otro momento poder restaurar la stdout original más adelante
    cmd->stdin = dup(STDIN_FILENO);
	// Redireccionar la stdout al archivo de destino lo que hace que cualquier salida que se enviase a la stdout ahora se escriba en el archivo seleccionado
    dup2(fd, STDIN_FILENO); 
    close(fd);
	// Para volver a la stdout original habíamos guardado con dup el estado por lo que ahora duplicamos la stdout original en nuestro stdout
}

void	input_multiple_redirect(t_cmd *cmd)
{
	int	fd;
	int	len;
	int	i;

	i = 0;
	len = find_last_input_redirect(cmd);
	fd = open(cmd->token[len + 1], O_RDONLY | S_IRUSR | S_IWUSR);
	if (fd == -1)
    {
        perror("");
        return ;
    }
	while(i < len)
	{
		if(cmd->token[i][0] == INPUT_REDIRECT)
			open(cmd->token[i + 1], O_RDONLY | S_IRUSR | S_IWUSR);
		i++;
	}
    cmd->stdin = dup(STDIN_FILENO);
    dup2(fd, STDIN_FILENO); 
    close(fd);
}

void close_input_redirect(t_cmd *cmd)
{
	dup2(cmd->stdin, STDIN_FILENO);
    close(cmd->stdin);
}