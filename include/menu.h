#ifndef MENU_H
#define MENU_H
#include "game.h"

void handleMenuInput(GameContext *context, char input);
void updateMenu(GameContext *context);
void drawMenu(GameContext *context, char *buffer);

#endif