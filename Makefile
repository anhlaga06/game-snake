CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude
SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=build/%.o)
TARGET = build/snake.exe

SRC_NO_MAIN = $(filter-out src/main.c,$(SRC))

TEST_SRC = test/test_game_update.c
TEST_TARGET = build/test_update

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

build/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

test_snake:
	gcc -Wall -Wextra -std=c11 -Iinclude test/test_snake.c src/snake.c -o build/test_snake.exe

$(TEST_TARGET):
	$(CC) $(CFLAGS) $(TEST_SRC) $(SRC_NO_MAIN) -o $(TEST_TARGET)

clean:
	rm -f build/*.o build/*.exe