#include "ball.h"
#include "paddle.h"
#include "world.h"
#include <stdio.h>
#include <stdlib.h>

void move_ball(struct Ball *ball) {
    // update the ball position
    (ball->x) = (ball->x) + ball->vx;
    (ball->y) = (ball->y) + (ball->vy);
};

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

// Reset the game
void reset_game(struct Ball *ball, struct Field *field, int direction) {
    ball->x = field->width / 2;
    ball->y = field->height / 2;
    ball->vx = direction;
    ball->vy = (rand() % 2) ? 1 : -1;
}

// Show the game score
void current_game_score(int s1, int s2, struct Field *field) {
    printf("Player 1: %d", s1);
    for (int j = 0; j < field->width; j += 2) {
        printf(" ");
    }
    printf("Player 2: %d", s2);
}
