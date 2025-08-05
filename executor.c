/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdonmeze <mdonmeze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:42:55 by md                #+#    #+#             */
/*   Updated: 2025/08/05 13:47:51 by mdonmeze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void free_path(char **path)
{
	int	i;

	if (!path)
		return ;
	i = 0;
	while (path[i])
	{
		free(path[i]);
		i++;
	}
	free(path);
}

char	*get_command_path(char *cmd, t_shell *shell)
{
	char	**path;
	char	*path_part;
	char	*full_path;
	int		i;

	if (!cmd || !cmd[0])
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	i = 0;
	while (shell->envp && shell->envp[i] && ft_strncmp(shell->envp[i], "PATH=", 5) != 0)
		i++;
	if (!shell->envp || !shell->envp[i])
		return (NULL);
	path = ft_split(shell->envp[i] + 5, ':');
	if (!path)
		return (NULL);
	i = 0;
	while (path[i])
	{
		path_part = ft_strjoin(path[i], "/");
		full_path = ft_strjoin(path_part, cmd);
		free(path_part);
		if (access(full_path, X_OK) == 0)
		{
			free_path(path);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_path(path);
	return (NULL);
}

void execute_pipeline(t_command *pipeline, t_shell *shell)
{
	int		pipe_fd[2];
	int		in_fd;
	pid_t	pid;
	int		status;

	if (pipeline->next == NULL && is_builtin(pipeline->args[0]))
	{
		// Single builtin command
		if (pipeline->redirects)
		{
			pid = fork();
			if (pid == 0)
			{
				signal(SIGINT, SIG_DFL);
				signal(SIGQUIT, SIG_DFL);
				if (handle_redirections(pipeline) == -1)
					exit(EXIT_FAILURE);
				exit(execute_builtin(pipeline, shell));
			}
			waitpid(pid, &status, 0);
			if (WIFEXITED(status))
				shell->last_exit_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				shell->last_exit_code = 128 + WTERMSIG(status);
		}
		else
		{
			shell->last_exit_code = execute_builtin(pipeline, shell);
		}
		return ;
	}

	if (!pipeline || !pipeline->args || !pipeline->args[0])
		return ;

	in_fd = STDIN_FILENO;

	while (pipeline)
	{
		if (pipeline->next)
		{
			if (pipe(pipe_fd) == -1)
			{
				perror("pipe");
				return;
			}
		}

		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			return;
		}

		if (pid == 0)
		{
			// Child process - restore default signal handlers
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);

			if (in_fd != STDIN_FILENO)
			{
				dup2(in_fd, STDIN_FILENO);
				close(in_fd);
			}

			if (pipeline->next)
			{
				close(pipe_fd[0]);
				dup2(pipe_fd[1], STDOUT_FILENO);
				close(pipe_fd[1]);
			}

			// Handle redirections
			if (handle_redirections(pipeline) == -1)
				exit(EXIT_FAILURE);

			// Execute command
			if (is_builtin(pipeline->args[0]))
				exit(execute_builtin(pipeline, shell));
			else
			{
				char *path = get_command_path(pipeline->args[0], shell);
				if (!path)
				{
					ft_putstr_fd("minishell: command not found: ", 2);
					ft_putendl_fd(pipeline->args[0], 2);
					exit(127);
				}
				execve(path, pipeline->args, shell->envp);
				perror("minishell: execve");
				free(path);
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			if (in_fd != STDIN_FILENO)
				close(in_fd);

			if (pipeline->next)
			{
				close(pipe_fd[1]);
				in_fd = pipe_fd[0];
			}
			else
			{
				// Last command, wait for it
				waitpid(pid, &status, 0);
				if (WIFEXITED(status))
					shell->last_exit_code = WEXITSTATUS(status);
				else if (WIFSIGNALED(status))
					shell->last_exit_code = 128 + WTERMSIG(status);
			}
		}
		pipeline = pipeline->next;
	}

	// Wait for remaining processes
	while (wait(NULL) > 0)
		;

	// cleanup_heredoc(pipeline); // pipeline şu anda NULL olabilir, skip et
}
