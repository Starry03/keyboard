/*
	Desc: thread that handles key input
	Integration: #include <keyboard.h>,
		allocate a keyboard and call 'start_keylistener'
*/

#ifndef KEYBOARD_H
# define KEYBOARD_H

# include <stdbool.h>
# include <stdint.h>

typedef struct s_keyboard
{
	unsigned char	*buf;
	uint32_t		exit_char;
	bool			running;
}					t_keyboard;

t_keyboard			*keyboard_init(uint32_t exit_char, unsigned char *buf);
void				keyboard_free(t_keyboard *keyboard);
unsigned long		start_keylistener(t_keyboard *keyboard);
/* static void		*listen(void *args); */
void				enable_raw_mode(void);
void				disable_raw_mode(void);

void				keyboard_bruteforce_exit(t_keyboard *keyboard);
void				keyboard_safestop(t_keyboard *keyboard,
						unsigned long thread_id);

#endif