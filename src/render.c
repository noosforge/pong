#include "ball.h"
#include "paddle.h"
#include "world.h"
#include <stdio.h>

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
