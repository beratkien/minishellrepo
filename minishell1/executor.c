/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdonmeze <mdonmeze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:42:55 by md                #+#    #+#             */
/*   Updated: 2025/06/27 02:02:12 by mdonmeze         ###   ########.fr       */
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
//path içerisinde geziyor ve konumları bölüyor.
char	*get_command_path(char *cmd, char **envp)
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
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	path = ft_split(envp[i] + 5, ':');
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
/*gelen argüman komut mu diye bakıyor eğer komutsa pathe bakıp çalıştırılabilir
olup olmadığına bakıyor.*/
static void execute_simple_command(t_command *cmd, char **envp)
{
	char	*path;

	if (!cmd || !cmd->args || !cmd->args[0])
		exit(EXIT_FAILURE);

	path = get_command_path(cmd->args[0], envp);
	if (!path)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putendl_fd(cmd->args[0], 2);
		exit(127);
	}

	if (execve(path, cmd->args, envp) == -1)
	{
		perror("minishell");
		free(path);
		exit(EXIT_FAILURE);
	}
}

void execute_pipeline(t_command *pipeline, char **envp)
{
	pid_t 	pid;
	int		status;

	if (!pipeline)
		return ;
	//buraya pipeları yönetecek bir döngü eklenecek!!!!!
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return ;

	}
	if (pid == 0)
	{
		// child süreçexecve("/bin/ls", )

		execute_simple_command(pipeline, envp);
	}
	else
	{
		//ana süreç
		//childin bitmesini bekle
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
		g_last_exit_status = WEXITSTATUS(status);
		else
			g_last_exit_status = 128 + WTERMSIG(status);
	}

}
