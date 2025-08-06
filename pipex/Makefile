NAME = pipex
CC = cc
CFLAGS = -Wall -Wextra -Werror -g3
SRCS_DIR = src
OBJS_DIR = objs
INCLUDES_DIR = includes
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

SRCS = \
	init.c \
	parsing.c \
	command_parser.c \
	pipex.c \
	utils.c \
	debug_utils.c \
	redirect.c \

MANDATORY = \
	main.c \

BONUS = \
	main_bonus.c \
	here_doc_bonus.c \


SRCS_PATH = $(addprefix $(SRCS_DIR)/, $(SRCS))
MANDATORY_PATH = $(addprefix $(SRCS_DIR)/, $(MANDATORY))
BONUS_PATH = $(addprefix $(SRCS_DIR)/, $(BONUS))

OBJS = $(SRCS_PATH:$(SRCS_DIR)/%.c=$(OBJS_DIR)/%.o)
MANDATORY_OBJS = $(MANDATORY_PATH:$(SRCS_DIR)/%.c=$(OBJS_DIR)/%.o)
BONUS_OBJS = $(BONUS_PATH:$(SRCS_DIR)/%.c=$(OBJS_DIR)/%.o)

INCLUDES = -I $(INCLUDES_DIR) -I $(LIBFT_DIR)/includes

all: $(LIBFT) $(NAME)

$(NAME): $(OBJS) $(MANDATORY_OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(MANDATORY_OBJS) $(LIBFT) -o $(NAME)

bonus: $(LIBFT) $(NAME)_bonus

$(NAME)_bonus: $(OBJS) $(BONUS_OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(BONUS_OBJS) $(LIBFT) -o $(NAME)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

$(LIBFT):
	@echo "Construction de libft.a..."
	@$(MAKE) -C $(LIBFT_DIR)

clean:
	@echo "Nettoyage des fichiers objets..."
	@rm -rf $(OBJS_DIR)

fclean: clean
	@echo "Nettoyage de $(NAME) et de libft..."
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

fclean_bonus: clean
	@echo "Nettoyage de $(NAME), libft et fichiers bonus..."
	@rm -f $(NAME)
	@rm -f here_doc
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

re_bonus: fclean_bonus bonus

.PHONY: all bonus clean fclean fclean_bonus re re_bonus