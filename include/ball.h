#ifndef BALL_H
#define BALL_H

#include "world.h"

struct Paddle;

struct Ball {
    int x;
    int y;
    int vx;
    int vy;
};

void move_ball(struct Ball *ball);
void bounce_off_horizontal_walls(struct Ball *ball, struct Field *field);
void bounce_off_paddles(struct Ball *ball, struct Paddle *paddle1, struct Paddle *paddle2);

// Reset the game
void reset_game(struct Ball *ball, struct Field *field, int direction);
// Show the game score
void current_game_score(int s1, int s2, struct Field *field);

#endif // !BALL_H
