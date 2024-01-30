# Keyboard

### Example

```c
#include "keyboard.h"
#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
	char		buf;
	size_t		i;
	t_keyboard 	*keyboard;
	
	buf = 0;
	i = 0;
	keyboard = keyboard_init('q');
	start_keylistener(keyboard, &buf);

	while (i < 10)
	{
		sleep(1);
		if (buf)
			printf("c: %c\r\n", buf);
		i++;
	}
	return 0;
}
```

### bitwise

- '&=' bitwise and
- '~' bitwise negation

### Flags

- ECHO = terminal shows what's typed
- ICANON = waits for a \n or EOF
- ISIG = enables standard behaviour for CTRL-Z and CTRL-C
- IXON = for CTRL-S and CTRL-Q
- IEXTEN = for CTRL-V and CTRL-O
- ICRNL = for CTRL-M
- OPOST = for output processing