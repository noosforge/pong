#include "ball.h"
#include "paddle.h"
#include "world.h"
#include <assert.h>
#include <stdio.h>

void test_top_bounce() {
    struct Field field = {.width = 40, .height = 12};
    struct Ball ball = {.x = 5, .y = -1, .vx = 1, .vy = -1};

    bounce_off_horizontal_walls(&ball, &field);

    assert((ball.y) == 0);
    assert((ball.vy) == 1);
};

void test_bottom_bounce() {
    struct Field field = {.width = 40, .height = 12};
    struct Ball ball = {.x = 5, .y = 12, .vx = 1, .vy = 1};

    bounce_off_horizontal_walls(&ball, &field);

    assert((ball.y) == 11);
    assert((ball.vy) == -1);
};

void test_bounce_off_left_paddle() {
    struct Paddle paddle1 = {.x = 1, .y = 5, .height = 5};
    struct Paddle paddle2 = {.x = 38, .y = 5, .height = 5};
    struct Ball ball = {.x = 1, .y = 6, .vx = -1, .vy = -1};

    bounce_off_paddles(&ball, &paddle1, &paddle2);

    assert((ball.x) == 2);
    assert((ball.vx == 1));
    assert((ball.vy) == -1);
};

void test_bounce_off_right_paddle() {
    struct Paddle paddle1 = {.x = 1, .y = 5, .height = 5};
    struct Paddle paddle2 = {.x = 38, .y = 5, .height = 5};
    struct Ball ball = {.x = 38, .y = 6, .vx = 1, .vy = 1};

    bounce_off_paddles(&ball, &paddle1, &paddle2);

    assert((ball.x) == 37);
    assert((ball.vx == -1));
    assert((ball.vy) == -1);
};

int main(void) {
    // we place out tests
    test_top_bounce();
    test_bottom_bounce();

    test_bounce_off_left_paddle();
    test_bounce_off_right_paddle();

    printf("All tests passed :)\n");
    return 0;
}
