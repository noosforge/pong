#ifndef PADDLE_H
#define PADDLE_H

#include "ball.h"
#include "world.h"

struct Ball;

struct Paddle {
    int x;
    int y;
    int height;
};

void clamp_paddles(struct Field *field, struct Paddle *paddle);
void ai_move_paddle(struct Field *field, struct Paddle *paddle2, struct Ball *ball);
void user_move_paddle(struct Paddle *paddle1, int *running, int c, int n);

#endif // !PADDLE_H
