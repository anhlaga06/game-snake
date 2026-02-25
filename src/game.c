#include "game.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define MAX_WIDTH   30 
#define MAX_HEIGHT  15

char screenBuffer[MAX_HEIGHT * (MAX_WIDTH + 1) + 1];
typedef struct Snake
{
    int posX[3];
    int posY[3];
} Snake;

Snake snake;


void gameInit() {
    printf("\033[2J");      // clear 1 lần
    printf("\033[?25l");    // hide cursor

    snake.posX[0] = 5;
    snake.posX[1] = 6;
    snake.posX[2] = 7;

    snake.posY[0] = 5;
    snake.posY[1] = 5;
    snake.posY[2] = 5;
}

void gameExit() {
    printf("\033[?25h");  // show cursor
}

void handle_input(){
    int headPosX = snake.posX[2];
    headPosX = (headPosX + 1) % (MAX_WIDTH - 1);
    if (headPosX == 0) headPosX = 1;

    snake.posX[0] = snake.posX[1];
    snake.posX[1] = snake.posX[2];
    snake.posX[2] = headPosX;
}
void update(){
    for (int heightIdx = 0; heightIdx < MAX_HEIGHT; heightIdx++) {
        for (int widthIdx = 0; widthIdx < MAX_WIDTH; widthIdx++) {
            char r = ' ';
            bool isEdge = (heightIdx == 0) || (heightIdx == MAX_HEIGHT - 1) 
            || (widthIdx == 0) || (widthIdx == MAX_WIDTH - 1);
            if (isEdge) r = '#';
            int charIdx = heightIdx * (MAX_WIDTH + 1) + widthIdx;
            bool isSnakeHead = false, isSnake = false;
            for (int i = 0; i < 3; i++) {
                if ((heightIdx == snake.posY[i]) && (widthIdx == snake.posX[i])) {
                    isSnake = true;
                    if (i == 2) isSnakeHead =true;
                }
            }
            if (isSnakeHead) r = 'O';
            else if (isSnake) r = 'o';
            screenBuffer[charIdx] = r;

            bool isEndLine = (widthIdx == MAX_WIDTH - 1);
            if (isEndLine) screenBuffer[charIdx + 1] = '\n';
        }
    }
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
        update();
        render();
        Sleep(1000);
    }
}