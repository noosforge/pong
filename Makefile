CC = clang
CFLAGS = -Wall -Wextra -Iinclude

SRC = src/main.c src/ball.c src/terminal.c src/paddle.c src/render.c
OBJ = $(SRC:src/%.c=build/%.o)

LIB_OBJ = $(filter-out build/main.o, $(OBJ))

TEST_SRC = tests/main_test.c
TEST_OBJ = $(TEST_SRC:tests/%.c=build/tests/%.o)

pong: $(OBJ)
	$(CC) $^ -o $@


# patter rule: .c -> .o
build/%.o: src/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

build/tests/%.o: tests/%.c
	@mkdir -p build/tests
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_OBJ) $(LIB_OBJ)
	$(CC) $^ -o build/test_runner
	./build/test_runner

.PHONY: clean test
clean:
	rm -rf build pong
