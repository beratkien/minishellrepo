/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beergin <beergin@student.42.tr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 19:46:50 by beergin           #+#    #+#             */
/*   Updated: 2025/08/07 16:38:27 by beergin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*create_temp_filename(void)
{
	static int	counter = 0;
	char		*counter_str;
	char		*filename;

	counter_str = ft_itoa(counter++);
	filename = ft_strjoin("/tmp/.heredoc_", counter_str);
	free(counter_str);
	return (filename);
}

static char	*read_line(void)
{
	char	line[1024];
	char	c;
	int		i;

	i = 0;
	while (read(STDIN_FILENO, &c, 1) > 0 && i < 1023)
	{
		if (c == '\n')
			break ;
		line[i++] = c;
	}
	line[i] = '\0';
	if (i == 0 && c != '\n')
		return (NULL);
	return (ft_strdup(line));
}

	static void	read_heredoc(const char *delim, char **heredoc_file, t_shell *shell, int quote_type)
	{
		char	*line;
		char	*expanded;
		int		fd;
		int		expand;

		expand = is_expandable_delim(quote_type);
		printf("%d\n", expand);
		*heredoc_file = create_temp_filename();
		if (!*heredoc_file)
			return ;
		fd = open(*heredoc_file, O_WRONLY | O_CREAT | O_TRUNC, 0600);
		if (fd < 0)
		{
			perror("heredoc temp file");
			free(*heredoc_file);
			*heredoc_file = NULL;
			return ;
		}
		while (1)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "> ", 2);
			line = read_line();
			if (!line)
				break ;
			if (ft_strncmp(line, delim, ft_strlen(delim)) == 0
				&& ft_strlen(line) == ft_strlen(delim))
			{
				free(line);
				break ;
			}
			if (expand)
				expanded = expand_variables2(line, shell);
			else
				expanded = ft_strdup(line);
			write(fd, expanded, ft_strlen(expanded));
			write(fd, "\n", 1);
			free(line);
		}
		close(fd);
	}

void	parse_heredoc(t_command *cmd, t_token *token, t_shell *shell)
{
	t_token		*tmp;
	t_redirect	*heredoc_redir;

	tmp = token;
	while (tmp)
	{
		if (tmp->type == TOKEN_HERE_DOC)
		{
			if (tmp->next && tmp->next->type == TOKEN_WORD)
			{
				cmd->heredoc_delimiter = ft_strdup(tmp->next->value);
				// Parse sırasında heredoc'u handle et
				read_heredoc(cmd->heredoc_delimiter, &cmd->heredoc_file, shell, tmp->next->quote_type);
				if (cmd->heredoc_file)
				{
					heredoc_redir = malloc(sizeof(t_redirect));
					if (heredoc_redir)
					{
						heredoc_redir->type = REDIR_HEREDOC;
						heredoc_redir->filename = ft_strdup(cmd->heredoc_file);
						heredoc_redir->next = NULL;
						add_redirection(&cmd->redirects, heredoc_redir);
					}
				}
			}
		}
		tmp = tmp->next;
	}
}
