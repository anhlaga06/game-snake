typedef struct Node {
    int x;
    int y;
    struct Node *next;
} Node;

void snakeAddHead(Node **snake, int x, int y);
void snakeRemoveTail(Node *snake);