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
void	output_redirect(t_cmd *cmd, int i)
{
	int	fd;
	int	len;
    int stdout;

	// Pasarle el token que tiene el nombre correspondiente
	// O_CREAT crea el archivo si no existe
	// O_TRUNC trunca el archivo a tamaño cero si ya existe
	// S_IRUSR | S_IWUSR establece permisos para ser legible y escribible solo por propietario
	fd = open(cmd->token[i + 2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        perror("");
        return ;
    }
	// Guardamos el fd de la salida actual para en otro momento poder restaurar la stdout original más adelante
    stdout = dup(STDOUT_FILENO);
	// Redireccionar la stdout al archivo de destino lo que hace que cualquier salida que se enviase a la stdout ahora se escriba en el archivo seleccionado
    dup2(fd, STDOUT_FILENO);
    close(fd);
	len = ft_strlen(cmd->token[i]);
	write(1, &cmd->token[i], len);
	// Para volver a la stdout original habíamos guardado con dup el estado por lo que ahora duplicamos la stdout original en nuestro stdout
    dup2(stdout, STDOUT_FILENO);
    close(stdout);
}

int main(int argc, char **argv)
{
	t_cmd cmd;
	output_redirect(argv, "hola.txt");
}