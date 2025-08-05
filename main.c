/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdonmeze <mdonmeze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 00:29:53 by beergin           #+#    #+#             */
/*   Updated: 2025/08/05 13:47:56 by mdonmeze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_signal(int signo)
{
	if (signo == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

int	main(int ac, char **av, char **envp)
{
	char	*line;
	t_shell	shell;

	(void)ac;
	(void)av;
	shell.envp = copy_envp(envp);
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
		if (shell.token)
		{
			free_tokens(shell.token);
			shell.token = NULL;
		}
		if (shell.command)
		{
			free_commands(shell.command);
			shell.command = NULL;
		}

		line = readline("$> ");
		if (line == NULL)
		{
			printf("exit\n");
			break ;
		}
		if (line[0] != '\0')
		{
			add_history(line);
			if (ft_strncmp(line, "exit", 4) == 0 && (line[4] == '\0' || line[4] == ' '))
			{
				free(line);
				break;
			}
			shell.token = lexer(line);
			if (shell.token)
			{
				expand_variables(shell.token, &shell);
				shell.command = parser(shell.token);
				if (shell.command)
					execute_pipeline(shell.command, &shell);
			}
		}
		free(line);
	}
	if (shell.token)
	{
		free_tokens(shell.token);
		shell.token = NULL;
	}
	if (shell.command)
	{
		free_commands(shell.command);
		shell.command = NULL;
	}
	free_envp(shell.envp);
	return (shell.last_exit_code);
}
