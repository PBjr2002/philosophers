CC = cc
CFLAGS = -g -pthread -Wall -Wextra -Werror
NAME = philo

SOURCES = main.c actions.c utils.c init.c

OBJS_DIR = obj
OBJECTS = $(addprefix $(OBJS_DIR)/, $(SOURCES:%.c=%.o))

all: $(NAME)

$(NAME): $(OBJECTS)
		$(CC) $(CFLAGS) $(OBJECTS) -o $@

$(OBJS_DIR)/%.o:%.c
	mkdir -p $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
		rm -rf $(OBJS_DIR)

fclean: clean
		rm -f $(NAME)

sanitize: $(OBJECTS)
		$(CC) $(CFLAGS) -fsanitize=thread $(OBJECTS) -o $(NAME)

re: fclean all

.PHONY: all clean fclean re