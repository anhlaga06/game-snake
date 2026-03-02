#include "game.h"

int main() {

    GameContext context;
    gameInit(&context);
    gameRun(&context);
    gameExit();

    return 0;
}