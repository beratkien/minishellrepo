/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdonmeze <mdonmeze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 00:29:53 by beergin           #+#    #+#             */
/*   Updated: 2025/06/27 02:35:39 by mdonmeze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal_status = 0;

void	handle_signal(int signo)
{
	if (signo == SIGINT)
	{
		g_signal_status = 1;
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (signo == SIGQUIT)
		g_signal_status = 2;
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

// --- ANA DEĞİŞİKLİK BURADA ---
int	main(int ac, char **av, char **envp)
{
	char		*line;
	t_token		*tokens;
	t_command	*commands;
	t_shell		shell;

	(void)ac;
	(void)av;
	shell.envp = envp;
	shell.last_exit_code = 0;
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		line = readline("minishell $> ");
		if (line == NULL)
		{
			printf("exit\n");
			break ;
		}
		if (line[0] != '\0')
		{
			add_history(line);
			tokens = lexer(line);
			if (tokens)
			{
				// Parser henüz hazır olmadığı için, lexer'dan gelen
				// token'ları doğrudan komut olarak kullanıyoruz.
				// Bu, 'ls -l' gibi basit komutlar için çalışır.
				commands = malloc(sizeof(t_command));
				commands->args = ft_split(line, ' ');
				commands->redirects = NULL;
				commands->next = NULL;
				if (commands)
				{
					execute_pipeline(commands, shell.envp);
					// Bellek temizliği (daha sonra robust hale getirilecek)
					int i = 0;
					while(commands->args[i])
						free(commands->args[i++]);
					free(commands->args);
					free(commands);
				}
				free_tokens(tokens);
			}
		}
		free(line);
	}
	return (shell.last_exit_code);
}
