#include "keyboard.h"
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

struct termios	default_termios;

t_keyboard	*keyboard_init(uint32_t exit_char, unsigned char *buf)
{
	t_keyboard	*keyboard;

	keyboard = (t_keyboard *)malloc(sizeof(t_keyboard));
	keyboard->exit_char = exit_char;
	keyboard->running = true;
	keyboard->can_change_key = true;
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
	t_keyboard		*keyboard;
	unsigned char	local_buffer;
	size_t			char_size;

	char_size = sizeof(unsigned char);
	keyboard = (t_keyboard *)arg;
	enable_raw_mode();
	while (keyboard->running && read(STDIN_FILENO, &local_buffer,
			char_size) > 0)
	{
		if (!keyboard->can_change_key)
			continue ;
		if (local_buffer == ESC)
		{
			if (read(STDIN_FILENO, &local_buffer, char_size) <= 0
				|| local_buffer != '[')
			{
				*(keyboard->buf) = ESC;
				continue ;
			}
			read(STDIN_FILENO, &local_buffer, char_size);
			switch (local_buffer)
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
			continue ;
		}
		*(keyboard->buf) = local_buffer;
		if (local_buffer == keyboard->exit_char)
			break ;
		local_buffer = 0;
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

void	set_can_change_key(t_keyboard *keyboard, bool value)
{
	keyboard->can_change_key = value;
}
