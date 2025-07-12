/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdonmeze <mdonmeze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 00:29:53 by beergin           #+#    #+#             */
/*   Updated: 2025/07/12 21:43:23 by mdonmeze         ###   ########.fr       */
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

void free_envp(char **envp)
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
	shell.command = NULL;
	shell.token = NULL;


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
            shell.token = lexer(line);
            if (shell.token)
            {
                shell.command = parser(shell.token);
                if (shell.command)
                {
                    execute_pipeline(shell.command, &shell);
                    free_commands(shell.command);
                }
                free_tokens(shell.token);
            }
        }
        free(line);
    }
	if (shell.token)
		free_tokens(shell.token);
	if (shell.command)
		free_commands(shell.command);
    free_envp(shell.envp);
    return (shell.last_exit_code);
}
