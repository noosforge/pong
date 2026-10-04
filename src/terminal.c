#include "terminal.h"
#include "ball.h"
#include "fcntl.h"
#include "termios.h"
#include "unistd.h"
#include "world.h"
#include <stdio.h>

struct termios origin_termios;

void enable_raw_mode(void) {
    // read the current terminal settings and store it in a variable called "origin_termios"
    tcgetattr(STDIN_FILENO, &origin_termios);
    struct termios raw = origin_termios;

    // Switch off ECHO AND ICANON
    raw.c_lflag &= ~(ECHO | ICANON);

    // set the terminal to raw mode by draining the output and flushing the input
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void disable_raw_mode(void) {
    // set the terminal back to its original state.
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &origin_termios);
}

void enable_non_blocking(void) {
    // Get the current flags from file control system
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    // enable O-NONBLOCK -  a specific bit for non-blocking mode
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
}
