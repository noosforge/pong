CC = clang
CFLAGS = -Wall -Wextra -Iinclude

SRC = src/main.c src/ball.c src/terminal.c src/paddle.c src/render.c
OBJ = $(SRC:src/%.c=build/%.o)


pong: $(OBJ)
	$(CC) $^ -o $@


# patter rule: .c -> .o
build/%.o: src/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf build pong
