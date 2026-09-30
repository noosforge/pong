#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

struct Field {
    int width;
    int height;
};

struct Ball {
    int x;
    int y;
    int vx;
    int vy;
};

struct Paddle {
    int x;
    int y;
    int height;
};

struct termios origin_termios;

void draw_field(struct Field f, struct Ball b, struct Paddle p1, struct Paddle p2) {
    // the top row
    printf("+");
    for (int i = 0; i < f.width; i++) {
        printf("-");
    }
    printf("+\n");

    // the body
    for (int y = 0; y < f.height; y++) {
        printf("|");

        // center space
        for (int x = 0; x < f.width; x++) {
            if (x == b.x && y == b.y) {
                printf("O");
            } else if ((x == p1.x && y >= p1.y) && (y < p1.y + p1.height)) {
                printf("|");

            } else if ((x == p2.x && y >= p2.y) && (y < p2.y + p2.height)) {
                printf("|");

            } else {
                printf(" ");
            }
        }

        printf("|\n");
    }

    // bottom row
    printf("+");
    for (int j = 0; j < f.width; j++) {
        printf("-");
    }
    printf("+\n");
}

void move_ball(struct Ball *ball) {
    // update the ball position
    (ball->x) = (ball->x) + ball->vx;
    (ball->y) = (ball->y) + (ball->vy);
}

void bounce_off_horizontal_walls(struct Ball *ball, struct Field *field) {
    // bounce vertical
    if (ball->y < 0) {
        (ball->y) = 0;
        (ball->vy) = -ball->vy;
    } else if (ball->y > field->height - 1) {
        (ball->y) = field->height - 1;
        (ball->vy) = -ball->vy;
    }
}

void bounce_off_paddles(struct Ball *ball, struct Paddle *paddle1, struct Paddle *paddle2) {
    // bounce on the left paddle
    if (((ball->x) == paddle1->x && (ball->y) >= paddle1->y) && (ball->y < paddle1->y + paddle1->height)) {
        (ball->x) = paddle1->x + 1;
        ball->vx = abs(ball->vx);

        // bounce on the right paddle
    } else if (((ball->x) == paddle2->x && (ball->y) >= paddle2->y) && (ball->y < paddle2->y + paddle2->height)) {
        (ball->x) = paddle2->x - 1;
        ball->vx = -abs(ball->vx);
    }
}

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

// Reset the game
void reset_game(struct Ball *ball, struct Field *field, int direction) {
    ball->x = field->width / 2;
    ball->y = field->height / 2;
    ball->vx = direction;
    ball->vy = 1; // hardcoded for now.
}

// Show the game score
void current_game_score(int s1, int s2, struct Field *field) {
    printf("Player 1: %d", s1);
    for (int j = 0; j < field->width; j += 2) {
        printf(" ");
    }
    printf("Player 1: %d", s2);
}

// MAIN FUNC
int main(void) {
    struct Field field = {.width = 40, .height = 12};
    struct Ball ball = {.x = 0, .y = 2, .vx = 1, .vy = 1};
    struct Paddle paddle1 = {.x = 1, .y = 4, .height = 4};
    struct Paddle paddle2 = {.x = field.width - 2, .y = 4, .height = 4};

    int score1 = 0;
    int score2 = 0;
    int pause_frames = 0;

    enable_raw_mode();
    enable_non_blocking();

    printf("\x1b[2J");   // clear screen once
    printf("\x1b[?25l"); // hide cursor

    int running = 1;
    while (running) {
        // Read the input (Non-Blocking)
        char c;
        ssize_t n = read(STDIN_FILENO, &c, 1);
        if (n == 1) {
            if (c == 'w')
                paddle1.y -= 1;
            if (c == 's')
                paddle1.y += 1;
            if (c == 'q')
                running = 0;
        }

        // clamp the paddles together
        if (paddle1.y < 0) {
            paddle1.y = 0;
        }
        if (paddle1.y + paddle1.height > field.height) {
            paddle1.y = field.height - paddle1.height;
        }

        // Draw the score.
        current_game_score(score1, score2, &field);

        if (pause_frames > 0) {
            pause_frames -= 1;
        } else {

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

            // Update the game.
            move_ball(&ball);
            bounce_off_horizontal_walls(&ball, &field);
            bounce_off_paddles(&ball, &paddle1, &paddle2);

            pause_frames -= 1;
        }

        // move cursor home
        printf("\x1b[H");

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
