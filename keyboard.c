#include "keyboard.h"
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define BUFFER_SIZE 3

struct termios			default_termios;
sem_t					keyboard_mutex;

/**
 * @brief Initialize a keyboard struct
 * @param exit_char character that will stop the key listener
 * @param buf buffer to store the key pressed
 */
t_keyboard	*keyboard_init(unsigned char exit_char, unsigned char *buf)
{
	t_keyboard	*keyboard;

	keyboard = (t_keyboard *)malloc(sizeof(t_keyboard));
	keyboard->exit_char = exit_char;
	keyboard->running = true;
	keyboard->key_can_change = true;
	keyboard->buf = buf;
	return (keyboard);
}

/**
 * @brief Free the keyboard struct
 */
inline void	keyboard_free(t_keyboard *keyboard)
{
	free(keyboard);
}

void	disable_raw_mode(void)
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &default_termios);
}

void	enable_raw_mode(void)
{
	struct termios	new_termios;

	tcgetattr(STDIN_FILENO, &default_termios);
	atexit(disable_raw_mode);
	new_termios = default_termios;
	new_termios.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
	new_termios.c_iflag &= ~(IXON);
	// new_termios.c_oflag &= ~(OPOST);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &new_termios);
}

static unsigned char	write_key(char *buf, unsigned char c, sem_t *mutex,
		unsigned char *local_buffer)
{
	keyboard_set_buf(buf, c, 1);
	memset(local_buffer, 0, BUFFER_SIZE);
	return (c);
}

/**
 * @brief Listen to key presses
 * @param arg t_keyboard struct
 */
static void	*listen(void *arg)
{
	t_keyboard		*keyboard;
	unsigned char	local_buffer[BUFFER_SIZE];
	sem_t			*mutex;

	keyboard = (t_keyboard *)arg;
	if (sem_init(&keyboard_mutex, false, 1) == -1)
	{
		perror("semaphore failed, check t_keyboard struct for errors\n");
		return (NULL);
	}
	mutex = &keyboard_mutex;
	enable_raw_mode();
	memset(local_buffer, 0, BUFFER_SIZE);
	while (keyboard->running && read(STDIN_FILENO, local_buffer, BUFFER_SIZE) !=
		-1)
	{
		if (!keyboard->key_can_change)
		{
			memset(local_buffer, 0, BUFFER_SIZE);
			continue ;
		}
		if (local_buffer[0] == ESC)
		{
			if (local_buffer[1] != '[')
			{
				write_key(keyboard->buf, ESC, mutex, local_buffer);
				continue ;
			}
			switch (local_buffer[2])
			{
			case 'A':
				write_key(keyboard->buf, ARROW_UP, mutex, local_buffer);
				break ;
			case 'B':
				write_key(keyboard->buf, ARROW_DOWN, mutex, local_buffer);
				break ;
			case 'C':
				write_key(keyboard->buf, ARROW_RIGHT, mutex, local_buffer);
				break ;
			case 'D':
				write_key(keyboard->buf, ARROW_LEFT, mutex, local_buffer);
				break ;
			default:
				write_key(keyboard->buf, ESC, mutex, local_buffer);
				break ;
			}
			continue ;
		}
		if (keyboard->exit_char == write_key(keyboard->buf, local_buffer[0],
				mutex, local_buffer))
			break ;
	}
	set_key_can_change(keyboard, false);
	sem_destroy(mutex);
	return (NULL);
}

/**
 * @brief Start the key listener thread
 * @return thread id
 */
pthread_t	start_keylistener(t_keyboard *keyboard)
{
	pthread_t	thread;

	*keyboard->buf = 0;
	set_key_can_change(keyboard, true);
	pthread_create(&thread, NULL, listen, (void *)keyboard);
	return (thread);
}

void	keyboard_bruteforce_exit(t_keyboard *keyboard)
{
	// set running to false -> exit thread
	keyboard->running = false;
}

void	keyboard_safestop(t_keyboard *keyboard, pthread_t thread_id)
{
	keyboard_bruteforce_exit(keyboard);
	pthread_join(thread_id, NULL);
}

void	set_key_can_change(t_keyboard *keyboard, bool value)
{
	keyboard->key_can_change = value;
}

/**
 * @brief utility to safely set buffer value (avoids race conditions)
 */
void	keyboard_set_buf(char *buf, char c, size_t size)
{
	if (!buf)
		return ;
	sem_wait(&keyboard_mutex);
	memset(buf, c, size);
	sem_post(&keyboard_mutex);
}

/**
 * @brief utility to reset buffer
 * @param buf buffer to set to 0
 */
void	keyboard_reset_buf(char *buf, size_t size)
{
	if (!buf)
		return ;
	keyboard_set_buf(buf, 0, size);
}
