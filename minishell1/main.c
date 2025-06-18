/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beergin <beergin@student.42.tr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 00:29:53 by beergin           #+#    #+#             */
/*   Updated: 2025/06/19 01:27:43 by beergin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t g_signal_status = 0;

void handle_signal(int signo)
{
	if(signo == SIGINT)
	{
		g_signal_status = 1;
	}
	else if(signo == SIGQUIT)
	{
		g_signal_status = 2;
	}
}

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
			add_token(&tokens, create_token(ft_strdup("|"), TOKEN_PIPE));
			i++;
		}
		else if (line[i] == '<')
		{
			if (line[i + 1] == '<')
			{
				add_token(&tokens, create_token(ft_strdup("<<"),
						TOKEN_HERE_DOC));
				i += 2;
			}
			else
			{
				add_token(&tokens, create_token(ft_strdup("<"),
						TOKEN_REDIRECT_IN));
				i++;
			}
		}
		else if (line[i] == '>')
		{
			if (line[i + 1] == '>')
			{
				add_token(&tokens, create_token(ft_strdup(">>"),
						TOKEN_REDIRECT_APPEND));
				i += 2;
			}
			else
			{
				add_token(&tokens, create_token(ft_strdup(">>"),
						TOKEN_REDIRECT_OUT));
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
					add_token(&tokens, create_token(ft_substr(line, start, i
								- start), TOKEN_WORD));
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
					add_token(&tokens, create_token(ft_substr(line, start, i
								- start), TOKEN_WORD));
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
				while (line[i] && !is_whitespace(line[i])
					&& !is_metachar(line[i]) && line[i] != '\''
					&& line[i] != '"')
					i++;
				add_token(&tokens, create_token(ft_substr(line, start, i
							- start), TOKEN_WORD));
			}
		}
	}
	return (tokens);
}

int	main(void)
{
	char *line;
	t_token *tokens;
	t_token *current_token; //test için

	signal(SIGINT, handle_signal);

	while (1)
	{
		line = readline("$> ");
		if (line == NULL)
		{
			printf("exit\n");
			break;
		}
		if (line[0] != '\0')
		{
			add_history(line);
			tokens = lexer(line);
			if(tokens == NULL)
			{
				free(line);
				continue;
			}

			//test kısmı//
			printf("------TEST------\n");
			current_token = tokens;
			while(current_token)
			{
				printf("Value : %s , Type : %d\n", current_token->value, current_token->type);
				current_token = current_token->next;
			}
			printf("------SON-------\n");
		}
		else
			printf("Empty command.\n");
		free(line);
	}
	return (0);
}