# Keyboard

### Example

```c
#include "keyboard.h"
#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
	t_keyboard		*keyboard_handler;
	t_uchar			key_buf;
	pthread_t		keyboard_thread_id;
	size_t			i;
	
	i = 0;
	keyboard = keyboard_init('q', &key_buf);
	keyboard_thread_id = start_keylistener(keyboard_handler);
	while (i < 10)
	{
		sleep(1);
		if (key_buf)
			printf("c: %c\n", key_buf);
		i++;
	}
	keyboard_safestop(keyboard_handler, keyboard_thread_id);
	keyboard_free(keyboard_handler);
	return 0;
}
```

### Flags

- ECHO = terminal shows what's typed
- ICANON = waits for a \n or EOF
- ISIG = enables standard behaviour for CTRL-Z and CTRL-C
- IXON = enables CTRL-S and CTRL-Q
- IEXTEN = CTRL-V and CTRL-O
- ICRNL = CTRL-M
- OPOST = output processing