NAME = minishell

CC = gcc
CFLAGS = -Wall -Wextra -Werror

LIBS = -lreadline

SRCS = \
	./main.c \
	./tokenizer.c \
	./type_token.c \
	./usefull/ft_lstadd_back.c \
	./usefull/ft_strdup.c \
	./usefull/ft_strlen.c \
	./usefull/ft_lstclear.c \
	./usefull/ft_isalnum.c \
	./usefull/ft_isalpha.c \
	./usefull/ft_isdigit.c \
	./usefull/ft_strjoin.c \
	./usefull/is_char_token.c \
	./token_check_syntaxe.c \
	./usefull/ft_strcmp.c \

OBJS = $(SRCS:.c=.o)

GREEN = \033[0;32m
YELLOW = \033[0;33m
RED = \033[0;31m
RESET = \033[0m

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(YELLOW)🔧 Compilation de $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LIBS)
	@echo "$(GREEN)✅ Compilé avec succès !$(RESET)"

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)
	@echo "$(RED)🧹 Fichiers objets supprimés.$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(RED)🧼 Exécutable supprimé.$(RESET)"

re: fclean all

.PHONY: all clean fclean re