/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 17:45:41 by isromero          #+#    #+#             */
/*   Updated: 2023/05/26 17:45:41 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *ft_strtok(char *str, const char *delim)
{
    static char *save = NULL;

    if (str != NULL)
        save = str;
    if (save == NULL || *save == '\0')
        return NULL;
    // Encuentra el inicio del token
    while (*save && strchr(delim, *save) != NULL)
        save++;
    if (*save == '\0')
        return NULL;
    char *token = save;
    // Encuentra el final del token
    while (*save && strchr(delim, *save) == NULL)
        save++;
    if (*save != '\0')
        *save++ = '\0';
    return token;
}