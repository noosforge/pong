#include <stdio.h>
#include <termios.h>
#include <unistd.h>

struct termios origin_termios;

void enable_raw_mode(void) {
    // reads the current terminal settings and stores it in a struct called origin_termios, so we can restore the terminal later.
    tcgetattr(STDIN_FILENO, &origin_termios);

    // Make a local copy so we can modify it. Modifying the original "origin_termios" would loose the original snapshot.
    struct termios raw = origin_termios;

    // Turn off "CONICAL MODE" and "ECHO", all other settings are left unchanged.
    // ~(ECHO | INCANON) perform OR operation on then to connect them.
    // Flip "NOT" them to get turn the bits that we want to turn off to 0s. Then "AND" with the local c_flag to remove them.
    raw.c_lflag &= ~(ECHO | ICANON);

    // Apply the modified setting to the terminal. Now the terminal is in raw mode.
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void disable_raw_mode(void) {
    // set the terminal, back to original state
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &origin_termios);
}

int main(void) {
    // switch to raw mode before reading the input.
    enable_raw_mode();

    printf("Press a key Ctrl-C to quit\n");

    while (1) {
        int c = getchar();
        printf("You pressed this charachter: %c (code %d)\n", c, c);
    }

    disable_raw_mode();
    return 0;
}
