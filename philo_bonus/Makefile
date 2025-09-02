NAME = philo_bonus
CC = cc  #-fsanitize=thread -pthread -g3
CFLAGS = -Wall -Wextra -Werror -pthread
RM = rm -f

SRCS = ../philo/ft_atoi.c main.c ../philo/parse.c \
	bonus_utils.c ../philo/philo_utils.c clean.c \
	simulation.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all
