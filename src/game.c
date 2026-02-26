#include "game.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include "snake.h"
#include "debug.h"

#define MAX_HEIGHT  15
#define MAX_WIDTH   30
#define UP      1
#define DOWN    2
#define LEFT    3
#define RIGHT   4
#define NONE    0

typedef struct Food
{
    int posX;
    int posY;
} Food;

int currDirection;
char screenBuffer[MAX_HEIGHT * (MAX_WIDTH + 1) + 1];

Node *snake;
Food food;


void gameInit() {
    printf("\033[2J");      // clear
    printf("\033[?25l");    // hide cursor

    snakeAddHead(&snake, 5, 5);
    snakeAddHead(&snake, 6, 5);
    snakeAddHead(&snake, 7, 5);
    
    currDirection = RIGHT;

    food.posX = 15;
    food.posY = 10;
}

void gameExit() {
    printf("\033[?25h");  // show cursor
}

int inputToDirection(char input) {
    if (input == 'a' || input == 75) return LEFT;
    if (input == 'd' || input == 77) return RIGHT;
    if (input == 's' || input == 80) return DOWN;
    if (input == 'w' || input == 72) return UP;
    return NONE;
}

int calcPosition(int posX, int posY) {
    return posY * (MAX_WIDTH + 1) + posX;
}

bool isOppositeDirection(int dir1, int dir2) {
    if (dir1 == LEFT && dir2 == RIGHT) return true;
    if (dir2 == LEFT && dir1 == RIGHT) return true;
    if (dir1 == UP && dir2 == DOWN) return true;
    if (dir2 == UP && dir1 == DOWN) return true;
    return false;
}
void handle_input(){
    if (kbhit()) {
        char userInput;
        while (kbhit()) userInput = getch(); // flush
        int nextDirect = inputToDirection(userInput);
        if (!isOppositeDirection(currDirection, nextDirect)) {
            currDirection = nextDirect;
        }
    }
    
}

void generateFood() {
    while (true)
    {
        food.posX = (rand() % (MAX_WIDTH - 2)) + 1;
        food.posY = (rand() % (MAX_HEIGHT - 2)) + 1;
        bool isFoodOverlapSnake = false;
        Node *node = snake;
        while (node)
        {
            if (node->x == food.posX && node->y == food.posY) {
                isFoodOverlapSnake = true;
                break;
            }
            node = node->next;
        }
        if (!isFoodOverlapSnake) {
            break;
        }
    }
    
}
void drawBackground() {
    for (int heightIdx = 0; heightIdx < MAX_HEIGHT; heightIdx++) {
        for (int widthIdx = 0; widthIdx < MAX_WIDTH; widthIdx++) {
            char r = ' ';
            bool isEdge = (heightIdx == 0) || (heightIdx == MAX_HEIGHT - 1) 
            || (widthIdx == 0) || (widthIdx == MAX_WIDTH - 1);
            if (isEdge) r = '#';
            int charIdx = calcPosition(widthIdx, heightIdx);
            screenBuffer[charIdx] = r;
            bool isEndLine = (widthIdx == MAX_WIDTH - 1);
            if (isEndLine) screenBuffer[charIdx + 1] = '\n';
        }
    }
}
void drawSnake() {
    int headPos = calcPosition(snake->x, snake->y);
    screenBuffer[headPos] = 'O';
    Node *snakeNode = snake->next;
    while (snakeNode)
    {
        int bodyPos = calcPosition(snakeNode->x, snakeNode->y);
        screenBuffer[bodyPos] = 'o';
        snakeNode = snakeNode->next;
    }
}
void drawFood() {
    int pos = calcPosition(food.posX, food.posY);
    screenBuffer[pos] = '*';
}
void gameDraw() {
    drawBackground();
    drawSnake();
    drawFood();
}

void gameUpdate(){
    switch (currDirection)
    {
        case RIGHT: {
            int headPosX = snake->x;
            headPosX = (headPosX + 1) % (MAX_WIDTH - 1);
            if (headPosX == 0) headPosX = 1;
            snakeAddHead(&snake, headPosX, snake->y);
            break;
        }
        case LEFT: {
            int headPosX = snake->x;
            if (headPosX == 1) headPosX = MAX_WIDTH - 2;
            else headPosX--;
            snakeAddHead(&snake, headPosX, snake->y);
            break;
        }
        case UP: {
            int headPosY = snake->y;
            if (headPosY == 1) headPosY = MAX_HEIGHT - 2;
            else headPosY--;
            snakeAddHead(&snake, snake->x, headPosY);
            break;
        }
        case DOWN: {
            int headPosY = snake->y;
            headPosY = (headPosY + 1) % (MAX_HEIGHT - 1);
            if (headPosY == 0) headPosY = 1;
            snakeAddHead(&snake, snake->x, headPosY);
            break;
        }
        
        default:
        break;
    }
    bool getEat = (snake->x == food.posX && snake->y == food.posY);
    if (getEat) {
        generateFood();
    } else {
        snakeRemoveTail(snake);
    }

    gameDraw();
}

void render(){
    printf("\033[H");
    printf("%s", screenBuffer);
    fflush(stdout);
}

void gameRun() {
    bool game_running = true;
    while (game_running) {
        handle_input();
        gameUpdate();
        render();
        Sleep(500);
    }
}