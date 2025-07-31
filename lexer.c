/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beergin <beergin@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 21:43:44 by mdonmeze          #+#    #+#             */
/*   Updated: 2025/08/01 00:43:28 by beergin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*lexer(char *line)
{
	t_token	*tokens;
	int		i;
	int		start;

	i = 0;
	tokens = NULL;
	if (!line)
		return (NULL);
	while (line[i])
	{
		while (line[i] && is_whitespace(line[i]))
			i++;
		if (!line[i])
			break ;
		if (line[i] == '|')
		{
			add_token(&tokens, create_token(ft_strdup("|"), TOKEN_PIPE, 0));
			i++;
		}
		else if (line[i] == '<')
		{
			if (line[i + 1] == '<')
			{
				add_token(&tokens, create_token(ft_strdup("<<"), TOKEN_HERE_DOC, 0));
				i += 2;
			}
			else
			{
				add_token(&tokens, create_token(ft_strdup("<"), TOKEN_REDIRECT_IN, 0));
				i++;
			}
		}
		else if (line[i] == '>')
		{
			if (line[i + 1] == '>')
			{
				add_token(&tokens, create_token(ft_strdup(">>"), TOKEN_REDIRECT_APPEND, 0));
				i += 2;
			}
			else
			{
				add_token(&tokens, create_token(ft_strdup(">"), TOKEN_REDIRECT_OUT, 0));
				i++;
			}
		}
		else
		{
			start = i;
			if (line[i] == '\'')
			{
				i++;
				start = i;
				while (line[i] && line[i] != '\'')
					i++;
				if (line[i] == '\'')
				{
					add_token(&tokens, create_token(ft_substr(line, start, i - start), TOKEN_WORD, 1));
					i++;
				}
				else
				{
					printf("minishell: unclosed quote\n");
					free_tokens(tokens);
					return (NULL);
				}
			}
			else if (line[i] == '"')
			{
				i++;
				start = i;
				while (line[i] && line[i] != '"')
					i++;
				if (line[i] == '"')
				{
					add_token(&tokens, create_token(ft_substr(line, start, i - start), TOKEN_WORD, 2));
					i++;
				}
				else
				{
					printf("minishell: unclosed quote\n");
					free_tokens(tokens);
					return (NULL);
				}
			}
			else
			{
				while (line[i] && !is_whitespace(line[i]) && !is_metachar(line[i]) && line[i] != '\'' && line[i] != '"')
					i++;
				add_token(&tokens, create_token(ft_substr(line, start, i - start), TOKEN_WORD, 0));
			}
		}
	}
	return (tokens);
}
