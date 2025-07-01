#include "minishell.h"

// Fonksiyonların prototipleri (dosyanın başında)
static char	*get_command_path(char *cmd, t_shell *shell);
static void	execute_simple_command(t_command *cmd, t_shell *shell);
static int	execute_builtin(t_command *cmd, t_shell *shell);
static int	is_builtin(char *cmd);

/**
 * @brief  Bir komutun gömülü (built-in) olup olmadığını kontrol eder.
 * @return Gömülü ise 1, değilse 0 döner.
 */
static int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (1);
	if (ft_strncmp(cmd, "cd", 3) == 0)
		return (1);
	if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "export", 7) == 0)
		return (1);
	if (ft_strncmp(cmd, "unset", 6) == 0)
		return (1);
	if (ft_strncmp(cmd, "env", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "exit", 5) == 0)
		return (1);
	return (0);
}

/**
 * @brief Gömülü komutları çalıştıran yönlendirici.
 * @return Komutun çıkış kodunu döner.
 */
static int	execute_builtin(t_command *cmd, t_shell *shell)
{
	(void)shell; // cd, export gibi komutlar için ileride kullanılacak
	if (ft_strncmp(cmd->args[0], "echo", 5) == 0)
	{
		// echo fonksiyonun buraya yazılacak
		printf("BUILT-IN: echo çalıştı\n");
	}
	else if (ft_strncmp(cmd->args[0], "pwd", 4) == 0)
	{
		// pwd fonksiyonun buraya yazılacak
		char cwd[1024];
		if (getcwd(cwd, sizeof(cwd)))
			printf("%s\n", cwd);
	}
	// Diğer built-in'ler buraya eklenecek...
	return (0);
}

/**
 * @brief  Verilen komut adının tam yolunu PATH'ten bulur.
 */
static char	*get_command_path(char *cmd, t_shell *shell)
{
	char	**paths;
	char	*path_part;
	char	*full_path;
	int		i;

	if (!cmd || !cmd[0] || ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	i = 0;
	while (shell->envp && shell->envp[i] && ft_strncmp(shell->envp[i], "PATH=", 5) != 0)
		i++;
	if (!shell->envp || !shell->envp[i])
		return (NULL);
	paths = ft_split(shell->envp[i] + 5, ':');
	i = -1;
	while (paths[++i])
	{
		path_part = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(path_part, cmd);
		free(path_part);
		if (access(full_path, X_OK) == 0)
		{
			// free_paths(paths);
			return (full_path);
		}
		free(full_path);
	}
	// free_paths(paths);
	return (NULL);
}

/**
 * @brief  Harici bir komutu çocuk süreçte çalıştırır.
 */
static void	execute_simple_command(t_command *cmd, t_shell *shell)
{
	char	*path;

	path = get_command_path(cmd->args[0], shell);
	if (!path)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putendl_fd(cmd->args[0], 2);
		exit(127);
	}
	if (execve(path, cmd->args, shell->envp) == -1)
	{
		perror("minishell");
		free(path);
		exit(EXIT_FAILURE);
	}
}

/**
 * @brief  Ana çalıştırma fonksiyonu. Gelen komutu kontrol edip yönlendirir.
 */
void	execute_pipeline(t_command *pipeline, t_shell *shell)
{
	pid_t	pid;
	int		status;

	if (!pipeline || !pipeline->args || !pipeline->args[0])
		return ;
	// --- İŞTE KONTROL NOKTASI BURASI ---
	if (is_builtin(pipeline->args[0]))
	{
		// Komut gömülü ise, fork yapmadan doğrudan çalıştır.
		shell->last_exit_code = execute_builtin(pipeline, shell);
	}
	else
	{
		// Komut harici ise, fork ile yeni bir süreçte çalıştır.
		pid = fork();
		if (pid == -1)
			perror("fork");
		else if (pid == 0)
			execute_simple_command(pipeline, shell);
		else
		{
			waitpid(pid, &status, 0);
			if (WIFEXITED(status))
				shell->last_exit_code = WEXITSTATUS(status);
		}
	}
}
