CC = gcc
FLAGS = -Werror -Wall -Wextra
NAME = keyboard.a

LINK = ./$(NAME) -I.

all: keyboard.o
	ar rcs $(NAME) keyboard.o
	ranlib $(NAME)

keyboard.o:
	$(CC) $(FLAGS) -I. -c keyboard.c -o keyboard.o

clean:
	@rm keyboard.o

fclean: clean
	@rm $(NAME)

re: fclean all

test:
	$(CC) $(FLAGS) main.c $(LINK) -o main && ./main

.PHONY: all re clean fclean test
	