/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdonmeze <mdonmeze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:42:55 by md                #+#    #+#             */
/*   Updated: 2025/07/12 20:17:31 by mdonmeze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_last_exit_status = 0;
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

static void execute_child_process(t_command *cmd, t_shell *shell, int in_fd, int out_fd)
{
	char	*path;

	if (in_fd != STDIN_FILENO)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (out_fd != STDOUT_FILENO)
	{
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
	if (handle_redirections(cmd) == -1)
		exit(EXIT_FAILURE);
	if (is_builtin(cmd->args[0]))
		exit(execute_builtin(cmd, shell));
	path = get_command_path(cmd->args[0], shell);
	if (!path)
	{
		ft_putstr_fd("minishell command not found: ", 2);
		ft_putendl_fd(cmd->args[0], 2);
		free_tokens(shell->token);
		free(path);
		free_envp(shell->envp);
		free_commands(cmd);
		exit(127);
	}
	execve(path, cmd->args, shell->envp);
	perror("minishell: execve");
	free_tokens(shell->token);
	free(path);
	free_envp(shell->envp);
	free_commands(cmd);
	exit(EXIT_FAILURE);
}
static void wait_for_child(pid_t last_pid, t_shell *shell)
{
	int status;

	waitpid(last_pid, &status, 0);
	while (wait(NULL) > 0)
		;
	if (WIFEXITED(status))
		shell->last_exit_code = WEXITSTATUS(status);
}

void execute_pipeline(t_command *pipeline, t_shell *shell)
{
	int		pipe_fd[2];
	int		in_fd;
	int		out_fd;
	pid_t	pid;

	if (pipeline->next == NULL && is_builtin(pipeline->args[0]))
	{
		shell->last_exit_code = execute_builtin(pipeline, shell);
		return ;
	}
	if (!pipeline || !pipeline->args || !pipeline->args[0])
		return ;

	in_fd = STDIN_FILENO;
	while(pipeline)
	{
		if (pipeline->next)
			pipe(pipe_fd);

		pid = fork();
		if (pid == 0)
		{
			if (pipeline->next)
				out_fd = pipe_fd[1];
			else
				out_fd = STDOUT_FILENO;
			if (pipeline->next)
				close(pipe_fd[0]);
			execute_child_process(pipeline, shell, in_fd, out_fd);
		}
		if (in_fd != STDIN_FILENO)
			close(in_fd);
		if (pipeline->next)
		{
			close(pipe_fd[1]);
			in_fd = pipe_fd[0];
		}
		pipeline = pipeline->next;
	}
	wait_for_child(pid, shell);
}
