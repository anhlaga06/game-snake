#ifndef SNAKE_H
#define SNAKE_H

typedef struct {
    int x;
    int y;
} Point;

typedef struct Node {
    Point c;
    struct Node *next;
} Node;

void snakeAddHead(Node **snake, int x, int y);
void snakeRemoveTail(Node *snake);

#endif