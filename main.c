/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdonmeze <mdonmeze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 00:29:53 by beergin           #+#    #+#             */
/*   Updated: 2025/07/09 21:51:39 by mdonmeze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal_status = 0;

void	handle_signal(int signo)
{
	if (signo == SIGINT)
	{
		g_signal_status = 1;
	}
	else if (signo == SIGQUIT)
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

static char **copy_envp(char **envp)
{
    char **new_envp;
    int count = 0;
    int i;

    // Önce kaç tane var sayalım
    while (envp[count])
        count++;

    new_envp = malloc(sizeof(char *) * (count + 1));
    if (!new_envp)
        return (NULL);

    i = 0;
    while (i < count)
    {
        new_envp[i] = ft_strdup(envp[i]);
        if (!new_envp[i])
        {
            // Hata durumunda önceki kopyaları free et
            while (--i >= 0)
                free(new_envp[i]);
            free(new_envp);
            return (NULL);
        }
        i++;
    }
    new_envp[count] = NULL;
    return (new_envp);
}

static void free_envp(char **envp)
{
    int i = 0;

    if (!envp)
        return;

    while (envp[i])
    {
        free(envp[i]);
        i++;
    }
    free(envp);
}

int	main(int ac, char **av, char **envp)
{
    char		*line;
    t_token		*tokens;
    t_shell		shell;

    (void)ac;
    (void)av;

    shell.envp = copy_envp(envp);  // Kopyasını oluştur
    if (!shell.envp)
    {
        ft_putstr_fd("Failed to copy environment\n", 2);
        return (1);
    }
    shell.last_exit_code = 0;

    signal(SIGINT, handle_signal);
    signal(SIGQUIT, SIG_IGN);

    while (1)
    {
        line = readline("$> ");
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
                t_command *commands = parser(tokens);

                if (commands)
                {
                    execute_pipeline(commands, &shell);
                    free_commands(commands);
                }
                free_tokens(tokens);
            }
        }
        free(line);
    }

    free_envp(shell.envp);  // Program sonunda free et
    return (shell.last_exit_code);
}
