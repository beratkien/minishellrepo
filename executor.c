/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beergin <beergin@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:42:55 by md                #+#    #+#             */
/*   Updated: 2025/06/27 03:37:32 by beergin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
int g_last_exit_status = 0;

static int apply_redirections(t_redirect *redirections)
{
    int fd;
    t_redirect *current_red;

    current_red = redirections;
    while (current_red)
    {
        if (current_red->type == TOKEN_REDIRECT_IN) // <
        {
            fd = open(current_red->filename, O_RDONLY);
            if (fd == -1)
            {
                perror(current_red->filename); // Hatanın olduğu dosya adını göster
                return (1); // Hata durumunda 1 döndür
            }
            if (dup2(fd, STDIN_FILENO) == -1) // Standart girdiyi yeniden yönlendir
            {
                perror("dup2 STDIN_FILENO failed");
                close(fd);
                return (1);
            }
            close(fd); // Orijinal dosya tanımlayıcısını kapat
        }
        else if (current_red->type == TOKEN_REDIRECT_OUT) // >
        {
            fd = open(current_red->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644); // Yazma, yoksa oluştur, varsa içeriği sil
            if (fd == -1)
            {
                perror(current_red->filename);
                return (1);
            }
            if (dup2(fd, STDOUT_FILENO) == -1) // Standart çıktıyı yeniden yönlendir
            {
                perror("dup2 STDOUT_FILENO failed");
                close(fd);
                return (1);
            }
            close(fd);
        }
        else if (current_red->type == TOKEN_REDIRECT_APPEND) // >>
        {
            fd = open(current_red->filename, O_WRONLY | O_CREAT | O_APPEND, 0644); // Yazma, yoksa oluştur, varsa sona ekle
            if (fd == -1)
            {
                perror(current_red->filename);
                return (1);
            }
            if (dup2(fd, STDOUT_FILENO) == -1)
            {
                perror("dup2 STDOUT_FILENO failed");
                close(fd);
                return (1);
            }
            close(fd);
        }
        else if (current_red->type == TOKEN_HERE_DOC) // <<< (veya <<)
        {
            // Here document (<<<) için daha karmaşık bir mantık gereklidir.
            // Genellikle:
            // 1. Geçici bir dosya oluşturulur veya pipe kullanılır.
            // 2. Sınırlayıcıya ulaşana kadar kullanıcıdan girdi okunur.
            // 3. Okunan girdiler geçici dosyaya/pipe'ın yazma ucuna yazılır.
            // 4. Çocuk süreçte, bu geçici dosya/pipe'ın okuma ucu STDIN'e yönlendirilir.
            // 5. Geçici dosya/pipe kapatılır ve gerektiğinde silinir.

            // Bu kısım, projenin "<<< should be given a delimiter, then read the input until a line containing the
            // gereksinimini karşılamalıdır.

            printf("minishell: here-doc redirection not fully implemented in this example\n");
            // Bu printf yerine write kullanmalısınız, ya da burada hata fırlatıp üst katmanda yakalamalısınız
            // ve orada izin verilen printf ile basmalısınız.
            return (1); // Örnekte şimdilik hata döndürüyoruz.
        }
        current_red = current_red->next; // Bir sonraki yönlendirmeye geç
    }
    return (0); // Tüm yönlendirmeler başarıyla uygulandı
}

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
static void execute_simple_command(t_command *cmd, t_shell *shell)
{
	char	*path;

	if (!cmd || !cmd->args || !cmd->args[0])
		exit(EXIT_FAILURE);

	path = get_command_path(cmd->args[0], shell->envp);
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

void execute_pipeline(t_command *pipeline, t_shell *shell)
{
	pid_t   pid;
    //int     pipe_fds[2]; döngü eklendiğinde kullanılacak
    int     fd_in;
    t_command *current_cmd;

    fd_in = STDIN_FILENO;
    current_cmd = pipeline;
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        shell->last_exit_code = 1; // Fork hatası
        return ;
    }
    if (pid == 0) // Çocuk süreç
    {
        if (apply_redirections(pipeline->redirects) != 0)
            exit(1);

        execute_simple_command(pipeline, shell); // shell yapısını geçir
    }
    else // Ana süreç
    {
        int status;
        waitpid(pid, &status, 0); // Çocuğun bitmesini bekle
        if (WIFEXITED(status)) // Çocuk normal şekilde çıktıysa
            shell->last_exit_code = WEXITSTATUS(status); // last_exit_code'u shell yapısında güncelle
        else // Çocuk sinyalle sona erdiyse
            shell->last_exit_code = 128 + WTERMSIG(status); // last_exit_code'u shell yapısında güncelle
    }

}
