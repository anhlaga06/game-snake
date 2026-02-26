#include "snake.h"
#include <stdlib.h>
#include <stdbool.h>
#include "debug.h"

void snakeAddHead(Node **snake, int x, int y) {
    Node *head = (Node*) malloc(sizeof(Node));
    CHECK(head != NULL);
    head->x = x;
    head->y = y;
    if (*snake) {
        head->next = *snake;
        *snake = head;
    } else {
        head->next = NULL;
        *snake = head;
    }
}
void snakeRemoveTail(Node *snake) {
    if (snake == NULL) return;
    Node *node = snake;
    while (node->next)
    {
        bool isTail = node->next->next == NULL;
        if (isTail) {
            free(node->next);
            node->next = NULL;
            break;
        }
        node = node->next;
    }
}