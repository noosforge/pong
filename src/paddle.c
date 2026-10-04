#include "paddle.h"
#include "ball.h"
#include "world.h"

void clamp_paddles(struct Field *field, struct Paddle *paddle) {
    // clamp the paddles
    if (paddle->y <= 0) {
        paddle->y = 0;
    }
    if (paddle->y + paddle->height >= field->height) {
        paddle->y = field->height - paddle->height;
    }
}

void ai_move_paddle(struct Paddle *paddle2, struct Ball *ball) {
    int paddle_center = paddle2->y + paddle2->height / 2;

    if (ball->y < paddle_center) {
        paddle2->y -= 1;
    } else if (ball->y > paddle_center) {
        paddle2->y += 1;
    }
}

void user_move_paddle(struct Paddle *paddle1, int *running, int c, int n) {
    if (n == 1) {
        if (c == 'w')
            paddle1->y -= 1;
        if (c == 's')
            paddle1->y += 1;
        if (c == 'q')
            *running = 0;
    }
}
