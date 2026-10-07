#include "ball.h"
#include "paddle.h"
#include "render.h"
#include "terminal.h"
#include "world.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

struct termios origin_termios;

// MAIN FUNC
int main(void) {
    struct Field field = {.width = 40, .height = 12};
    struct Ball ball = {.x = 0, .y = 2, .vx = 1, .vy = 1};
    struct Paddle paddle1 = {.x = 1, .y = 5, .height = 5};
    struct Paddle paddle2 = {.x = field.width - 2, .y = 5, .height = 5};

    int score1 = 0;
    int score2 = 0;
    int pause_frames = 0;

    srand(time(NULL));

    enable_raw_mode();
    enable_non_blocking();

    printf("\x1b[2J");   // clear screen once
    printf("\x1b[?25l"); // hide cursor

    int running = 1;
    while (running) {
        // Read the input (Non-Blocking)
        char c;
        ssize_t n = read(STDIN_FILENO, &c, 1);

        // Move the paddles
        user_move_paddle(&paddle1, &running, c, n);
        ai_move_paddle(&field, &paddle2, &ball);

        clamp_paddles(&field, &paddle1);
        clamp_paddles(&field, &paddle2);

        if (pause_frames > 0) {
            pause_frames -= 1;
        } else {

            // Update the game.
            move_ball(&ball);
            bounce_off_horizontal_walls(&ball, &field);
            bounce_off_paddles(&ball, &paddle1, &paddle2);

            // check scoring
            if (ball.x < paddle1.x) {
                score2 += 1;
                reset_game(&ball, &field, 1);
                pause_frames = 20;
            } else if (ball.x > paddle2.x) {
                score1 += 1;
                reset_game(&ball, &field, -1);
                pause_frames = 20;
            }
        }

        // move cursor home
        printf("\x1b[H");

        // Draw the score.
        current_game_score(score1, score2, &field);
        printf("\n");

        // Draw
        draw_field(field, ball, paddle1, paddle2);

        usleep(100000); // 0.1 seconds
    }

    disable_raw_mode();

    printf("\x1b[?25h"); // show cursor again
    printf("\x1b[2J");   // clear screen so the shell starts clean
    printf("\x1b[H");    // cursor home

    return 0;
}
