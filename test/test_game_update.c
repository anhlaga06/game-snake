#include <assert.h>
#include <stdio.h>

#include "game.h"
#include "snake.h"

Node* makeSnake(int coords[][2], int len)
{
    Node *snake = NULL;

    for (int i = len - 1; i >= 0; i--) {
        snakeAddHead(&snake, coords[i][0], coords[i][1]);
    }

    return snake;
}

void checkSnake(Node *actual, Node *expected)
{
    while (actual && expected) {

        assert(actual->x == expected->x);
        assert(actual->y == expected->y);

        actual = actual->next;
        expected = expected->next;
    }

    assert(actual == NULL);
    assert(expected == NULL);
}

GameContext makeContext(Node *snake, int direction)
{
    GameContext ctx = {0};

    ctx.snake = snake;
    ctx.direction = direction;

    return ctx;
}

void testMoveRight()
{
    int coords[][2] = {
        {7,5},
        {6,5},
        {5,5}
    };

    Node *snake = makeSnake(coords,3);

    GameContext ctx = makeContext(snake, DIR_RIGHT);

    ctx.food.x = 100;
    ctx.food.y = 100;

    gameUpdate(&ctx);

    int expectedCoords[][2] = {
        {8,5},
        {7,5},
        {6,5}
    };

    Node *expected = makeSnake(expectedCoords,3);

    checkSnake(ctx.snake, expected);

    printf("RIGHT OK\n");
}

void testMoveLeft()
{
    int coords[][2] = {
        {7,5},
        {6,5},
        {5,5}
    };

    Node *snake = makeSnake(coords,3);

    GameContext ctx = makeContext(snake, DIR_LEFT);

    ctx.food.x = 100;
    ctx.food.y = 100;

    gameUpdate(&ctx);

    int expectedCoords[][2] = {
        {6,5},
        {7,5},
        {6,5}
    };

    Node *expected = makeSnake(expectedCoords,3);

    checkSnake(ctx.snake, expected);

    printf("LEFT OK\n");
}

void testMoveUp()
{
    int coords[][2] = {
        {7,5},
        {6,5},
        {5,5}
    };

    Node *snake = makeSnake(coords,3);

    GameContext ctx = makeContext(snake, DIR_UP);

    ctx.food.x = 100;
    ctx.food.y = 100;

    gameUpdate(&ctx);

    int expectedCoords[][2] = {
        {7,4},
        {7,5},
        {6,5}
    };

    Node *expected = makeSnake(expectedCoords,3);

    checkSnake(ctx.snake, expected);

    printf("UP OK\n");
}

void testMoveDown()
{
    int coords[][2] = {
        {7,5},
        {6,5},
        {5,5}
    };

    Node *snake = makeSnake(coords,3);

    GameContext ctx = makeContext(snake, DIR_DOWN);
    ctx.food.x = 100;
    ctx.food.y = 100;

    gameUpdate(&ctx);

    int expectedCoords[][2] = {
        {7,6},
        {7,5},
        {6,5}
    };

    Node *expected = makeSnake(expectedCoords,3);

    checkSnake(ctx.snake, expected);

    printf("DOWN OK\n");
}

void testEatFood()
{
    int coords[][2] = {
        {7,5},
        {6,5},
        {5,5}
    };

    Node *snake = makeSnake(coords,3);

    GameContext ctx = makeContext(snake, DIR_RIGHT);

    ctx.food.x = 8;
    ctx.food.y = 5;

    gameUpdate(&ctx);

    int expectedCoords[][2] = {
        {8,5},
        {7,5},
        {6,5},
        {5,5}
    };

    Node *expected = makeSnake(expectedCoords,4);

    checkSnake(ctx.snake, expected);

    printf("EAT FOOD OK\n");
}

int main()
{
    testMoveRight();
    testMoveLeft();
    testMoveUp();
    testMoveDown();
    testEatFood();

    printf("ALL TESTS PASSED\n");

    return 0;
}