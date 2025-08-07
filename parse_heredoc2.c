/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredoc2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beergin <beergin@student.42.tr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 15:11:04 by beergin           #+#    #+#             */
/*   Updated: 2025/08/07 15:53:26 by beergin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_expandable_delim(int quote_type)
{
    if(quote_type == 1 || quote_type == 2)
        return (0);
    return (1);
}

char *expand_variables2(char *line, t_shell *shell)
{
    char *expanded;
    
    if (!line || !shell)
        return (NULL);
    expanded = expand_string(line, shell);
    return (expanded);
}