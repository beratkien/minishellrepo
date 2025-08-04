/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: md <md@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 11:32:44 by mdonmeze          #+#    #+#             */
/*   Updated: 2025/08/03 00:43:15 by md               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int handle_redirections(t_command *cmd)
{
	t_redirect	*redir;
	int			fd;

	if (!cmd->redirects)
		return(0);
	redir = cmd->redirects;
	while (redir)
	{
		if (redir->type == REDIR_OUT)
			fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (redir->type == REDIR_APPEND)
			fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else if (redir->type == REDIR_IN)
			fd = open(redir->filename, O_RDONLY);
		else if (redir->type == REDIR_HEREDOC)
			fd = open(redir->filename, O_RDONLY);
		else
		{
			redir = redir->next;
			continue;
		}
		if (fd == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			perror(redir->filename);
			return (-1);
		}
		if (redir->type == REDIR_IN || redir->type == REDIR_HEREDOC)
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);
		close(fd);
		redir = redir->next;
	}
	return (0);
}
//geçici dosyası silme fonksiyonu
void	cleanup_heredoc(t_command *pipeline)
{
	t_command	*current_cmd;
	t_redirect	*redir;

	current_cmd = pipeline;
	while (current_cmd)
	{
		redir = current_cmd->redirects;
		while(redir)
		{
			if(redir->type == REDIR_HEREDOC && redir->filename)
				unlink(redir->filename);
			redir = redir->next;
		}
		current_cmd = current_cmd->next;
	}
}
