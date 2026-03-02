#include "menu.h"
#include "string.h"
#include "stdio.h"

typedef enum {
    INVALID_KEY,
    ARROW_UP,
    ARROW_DOWN,
    ARROW_LEFT,
    ARROW_RIGHT,
    ENTER
} KEY_PRESSS;

KEY_PRESSS inputToKey(char input) {
    if (input == 75) 
        return ARROW_LEFT;
    if (input == 77) 
        return ARROW_RIGHT;
    if (input == 80) 
        return ARROW_DOWN;
    if (input == 72) 
        return ARROW_UP;
    if (input == 13)
        return ENTER;
    return INVALID_KEY;
}

void handleMenu1Key(GameContext *context, KEY_PRESSS key) {
    unsigned int *activePtr = &context->menu.active;
    if (key == ARROW_DOWN) {
        (*activePtr)++;
        if (*activePtr > 2) 
            *activePtr = 1;
        if (*activePtr < 1) 
            *activePtr = 2;
    } else if (key == ARROW_UP) {
        (*activePtr)--;
    } else if (key == ENTER) {
        if (*activePtr == 2) {
            context->isExit = true;
        } else if (*activePtr == 1) {
            newGameInit(context);
        }
    }
}

void handleMenu2Key(GameContext *context, KEY_PRESSS key) {
    unsigned int *activePtr = &context->menu.active;
    if (key == ARROW_DOWN) {
        (*activePtr)++;
        if (*activePtr > 3) 
            *activePtr = 1;
        if (*activePtr < 1) 
            *activePtr = 3;
    } else if (key == ARROW_UP) {
        (*activePtr)--;
    } else if (key == ENTER) {
        if (*activePtr == 3) {
            context->isExit = true;
        } else if (*activePtr == 1) {
            context->state = STATE_GAME_RUNNING;
        } else if (*activePtr == 2) {
            gameDeinit(context);
            newGameInit(context);
        }
    }
}


void handleMenuInput(GameContext *context, char input) {
    KEY_PRESSS key = inputToKey(input);
    switch (context->menu.type)
    {
        case MENU_1: {
            handleMenu1Key(context, key);
            break;
        }
        case MENU_2: {
            handleMenu2Key(context, key);
            break;
        }
    
    default:
        break;
    }
}
void updateMenu(GameContext *context) {

}

int calcPosition1(int x, int y) {
    return y * (MAX_WIDTH + 1) + x;
}

void gameMenuBuilder(char optTex[][50], const unsigned int optCnt, const unsigned int optActive, char *screenBuffer) {
    char scoreBoard[MAX_WIDTH];
    char chooseEff[3];
    for (int i = 0; i < optCnt; i++) {
        if (optActive == i + 1) {
            strcpy(chooseEff, "->");
        } else {
            strcpy(chooseEff, "  ");
        }
        snprintf(scoreBoard, sizeof(scoreBoard), "%s%d. %s", chooseEff, i + 1, optTex[i]);
        int pos = calcPosition1(1, i + 1);
        memcpy(screenBuffer + pos, scoreBoard, strlen(scoreBoard));
    }
}

void drawMenu(GameContext *context, char *buffer) {
    switch (context->menu.type)
    {
        case MENU_1: {
            char menu[][50] = {
                "New game",
                "Exit"
            };
            gameMenuBuilder(menu,  2, context->menu.active, buffer);
            break;
        }
        case MENU_2: {
            char menu[][50] = {
                "Continue",
                "New game",
                "Exit"
            };
            gameMenuBuilder(menu,  3, context->menu.active, buffer);
            break;
        }
    
        default:
            break;
    }
}