NAME = philo
CC = cc
SRC_DIR = ./src
BIN_DIR = ./bin

FILES = \
	env.c \
	main.c \
	philo.c \
	simulation.c \
	util.c

SRC = $(addprefix $(SRC_DIR)/,$(FILES))
OBJ = $(addprefix $(BIN_DIR)/,$(FILES:.c=.o))
INC = -I $(SRC_DIR)
LNK = -Lpthread
CFLAGS = -Wall -Wextra -Werror -g

all: $(NAME)

$(NAME): $(BIN_DIR) $(OBJ)
	$(CC) $(CFLAGS) $(INC) $(OBJ) -o $(NAME) $(LNK)

$(BIN_DIR):
	mkdir -p $(dir $(OBJ))

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $(INC) $< -o $@

clean:
	rm -rf $(BIN_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
