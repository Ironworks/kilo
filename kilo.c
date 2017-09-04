#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>


struct termios orig_termios;

void disableRawMode() {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}
void enableRawMode() {

	tcgetattr(STDIN_FILENO, &orig_termios);
	atexit(disableRawMode);

	struct termios raw = orig_termios;
	raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON); //Disables ctrl-s and ctrl-q, fixes ctrl-m
	raw.c_oflag &= ~(OPOST); //Disables all output processing
	raw.c_cflag &= ~(CS8); //Set character size to 8 bits
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG); //Disables echo, concanical mode, ctrl-v, ctrl-c, ctrl-z signals

	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {

	enableRawMode();

	char c;
	while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {

		if (iscntrl(c)) {
			printf("%d\r\n", c);
		} else {
			printf("%d (%c)\r\n", c, c );
		}
	}

    return 0;
}
