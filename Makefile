CC = gcc
FLAGS = -Werror -Wall -Wextra
NAME = keyboard.a

keyboard.o:
	$(CC) $(FLAGS) -I. -c keyboard.c -o keyboard.o

all: keyboard.o
	ar rcs $(NAME) keyboard.o
	ranlib $(NAME)
	@rm keyboard.o

.PHONY: all
	