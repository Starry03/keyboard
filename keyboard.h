/*
	Desc: thread that handles key input
	Integration: #include <keyboard.h>,
		allocate a keyboard and call 'start_keylistener'
*/

#ifndef KEYBOARD_H
# define KEYBOARD_H

# include <pthread.h>
# include <stdbool.h>
# include <stdint.h>

typedef struct s_keyboard
{
	unsigned char	*buf;
	uint32_t		exit_char;
	bool			running;
	bool			can_change_key;
}					t_keyboard;

t_keyboard			*keyboard_init(uint32_t exit_char, unsigned char *buf);
void				keyboard_free(t_keyboard *keyboard);
pthread_t			start_keylistener(t_keyboard *keyboard);
void				set_can_change_key(t_keyboard *keyboard, bool value);
/* static void		*listen(void *args); */
void				enable_raw_mode(void);
void				disable_raw_mode(void);

void				keyboard_bruteforce_exit(t_keyboard *keyboard);
void				keyboard_safestop(t_keyboard *keyboard,
						pthread_t thread_id);

# define LINE_FEED 10
# define CARRIAGE_RETURN 13
# define ESC 27
# define ARROW_UP 129
# define ARROW_DOWN 141
# define ARROW_RIGHT 143
# define ARROW_LEFT 144
# define DELETE 127
#endif