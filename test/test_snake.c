#include <assert.h>
#include <stdio.h>
#include "snake.h"

int snake_length(Node *snake) {
    int count = 0;
    while (snake) {
        count++;
        snake = snake->next;
    }
    return count;
}

int main() {
    Node *snake = NULL;

    snakeAddHead(&snake, 1, 1);
    snakeAddHead(&snake, 2, 1);
    snakeAddHead(&snake, 3, 1);

    assert(snake_length(snake) == 3);

    snakeRemoveTail(snake);

    assert(snake_length(snake) == 2);

    printf("All snake tests passed!\n");
    return 0;
}