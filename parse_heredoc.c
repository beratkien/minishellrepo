/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beergin <beergin@student.42.tr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 19:46:50 by beergin           #+#    #+#             */
/*   Updated: 2025/08/02 20:08:14 by beergin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	read_heredoc(const char *delim, char **heredoc_file)
{
	char	*line;
	int		fd;

	*heredoc_file = tmpnam(NULL);
	fd = open(*heredoc_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return ;
    while(1)
    {
        line = readline("> ");
        if(!line || (ft_strncmp(line, delim, ft_strlen(delim)) == 0 && line[ft_strlen(delim)] == '\n'))
        {
            free(line);
            break;
        }
        write(fd, line, ft_strlen(line));
        free(line);
    }
    close(fd);
}

void	parse_heredoc(t_command *cmd, t_token *token)
{
	t_token *tmp;

	tmp = token;
	while (tmp)
	{
		if (tmp->type == TOKEN_HERE_DOC)
		{
			if (tmp->next && tmp->next->type == TOKEN_WORD)
			{
				cmd->heredoc_delimiter = ft_strdup(tmp->next->value);
                read_heredoc(cmd->heredoc_delimiter, &cmd->heredoc_file);
			}
		}
		tmp = tmp->next;
	}
}