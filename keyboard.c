#include "keyboard.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

struct termios	default_termios;

t_keyboard	*keyboard_init(uint32_t exit_char, unsigned char *buf)
{
	t_keyboard	*keyboard;

	keyboard = (t_keyboard *)malloc(sizeof(t_keyboard));
	keyboard->exit_char = exit_char;
	keyboard->running = true;
	keyboard->key_can_change = true;
	keyboard->buf = buf;
	return (keyboard);
}

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

static void	*listen(void *arg)
{
	const size_t	BUF_SIZE = 3;
	t_keyboard		*keyboard;
	unsigned char	local_buffer[BUF_SIZE];

	keyboard = (t_keyboard *)arg;
	enable_raw_mode();
	memset(local_buffer, 0, BUF_SIZE);
	while (keyboard->running && read(STDIN_FILENO, local_buffer, BUF_SIZE) !=
		-1)
	{
		if (!keyboard->key_can_change)
		{
			memset(local_buffer, 0, BUF_SIZE);
			continue ;
		}
		if (local_buffer[0] == ESC)
		{
			if (local_buffer[1] != '[')
			{
				*(keyboard->buf) = ESC;
				memset(local_buffer, 0, BUF_SIZE);
				continue ;
			}
			switch (local_buffer[2])
			{
			case 'A':
				*(keyboard->buf) = ARROW_UP;
				break ;
			case 'B':
				*(keyboard->buf) = ARROW_DOWN;
				break ;
			case 'C':
				*(keyboard->buf) = ARROW_RIGHT;
				break ;
			case 'D':
				*(keyboard->buf) = ARROW_LEFT;
				break ;
			default:
				*(keyboard->buf) = ESC;
				break ;
			}
			memset(local_buffer, 0, BUF_SIZE);
			continue ;
		}
		*(keyboard->buf) = local_buffer[0];
		if (local_buffer[0] == keyboard->exit_char)
			break ;
		memset(local_buffer, 0, BUF_SIZE);
	}
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
