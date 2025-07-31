NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g -I/usr/local/opt/readline/include
LDFLAGS = -lreadline -L/usr/local/opt/readline/lib

SRCS = main.c parser_utils.c parser_utils2.c parser.c executor.c builtins/pwd.c \
		redirections.c builtins/built.c builtins/cd.c builtins/env.c builtins/exit.c \
		lexer.c builtins/env_utils.c builtins/echo.c builtins/export.c builtins/unset.c

OBJS = $(SRCS:.c=.o)

LIBFT_DIR = libft

LIBFT = $(LIBFT_DIR)/libft.a

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LDFLAGS) -o $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR) all
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)
	@make -C $(LIBFT_DIR) clean
fclean: clean
	@rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean
re: fclean all

leaks:
	@valgrind	--leak-check=full \
				--suppressions=readline.supp \
				--show-leak-kinds=all \
				--track-origins=yes \
				--track-fds=yes \
				--verbose \
				./$(NAME)

.PHONY: all clean fclean re
