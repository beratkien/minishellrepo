/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beergin <beergin@student.42.tr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 00:29:04 by beergin           #+#    #+#             */
/*   Updated: 2025/06/19 00:43:11 by beergin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_whitespace(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f'
		|| c == '\v')
		return (1);
	return (0);
}

int	is_metachar(char c)
{
	if (c == '|' || c == '<' || c == '>' || c == '$')
		return (1);
	return (0);
}

t_token *create_token(char *value, int type)
{
    t_token *new_token;
    
    new_token = (t_token *)malloc(sizeof(t_token));
    if(!new_token)
    {
        printf("malloc failed");
        return (NULL);
    }
    new_token->value = value;
    new_token->type = type;
    new_token->next = NULL;
    return (new_token);
}

void add_token(t_token **head, t_token *new_token)
{
    t_token *current;

    if(!*head)
        *head = new_token;
    else
    {
        current = *head;
        while(current->next)
            current = current->next;
        current->next = new_token;
    }
}

void free_tokens(t_token *head)
{
    t_token *tmp;
    while(head)
    {
        tmp = head;
        head = head->next;
        if(tmp->value)
            free(tmp->value);
        free(tmp);
    }
}