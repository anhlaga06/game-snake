#include "input.h"
#include <conio.h>

bool readInput(char *c) {
    if (kbhit()) {
        char userInput;
        while (kbhit()) userInput = getch(); // flush
        *c = userInput;
        return true;
    }
    return false;
}