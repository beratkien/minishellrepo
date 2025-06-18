#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
#include <signal.h>

# define TOKEN_WORD 1
# define TOKEN_PIPE 2
# define TOKEN_REDIRECT_IN 3
# define TOKEN_REDIRECT_OUT 4
# define TOKEN_REDIRECT_APPEND 5
# define TOKEN_HERE_DOC 6

typedef struct s_token
{
	char			*value;
	int				type;
	struct s_token	*next;
}					t_token;

t_token				*lexer(char *line);

int					is_whitespace(char c);
int					is_metachar(char c);
t_token				*create_token(char *value, int type);
void				add_token(t_token **head, t_token *new_token);
void				free_tokens(t_token *head);

#endif