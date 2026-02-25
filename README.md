# game-snake
# 🐍 Snake Game in C (Console Based)

A real-time Snake game implemented in pure C using dynamic memory
management and a linked list data structure.

This project demonstrates low-level programming concepts including
manual memory management, state machines, real-time input handling, and
modular architecture.

------------------------------------------------------------------------

## 🚀 Features

-   Real-time keyboard input (non-blocking)
-   Linked list implementation for snake body
-   Dynamic memory allocation (`malloc` / `free`)
-   Collision detection (wall & self)
-   Score tracking
-   High score persistence using file I/O
-   State machine driven game loop
-   Modular architecture (.c / .h separation)

------------------------------------------------------------------------

## 🧠 Technical Highlights

### 1️⃣ Linked List Snake Body

``` c
typedef struct Node {
    int x;
    int y;
    struct Node *next;
} Node;
```

Each movement: - New head node is allocated - Tail node is removed
(unless food is eaten)

This ensures: - Dynamic growth - Efficient insert/remove operations - No
fixed size limitation

------------------------------------------------------------------------

### 2️⃣ Game Loop Architecture

``` c
while (game_running) {
    handle_input();
    update();
    render();
    sleep();
}
```

Responsibilities are separated: - Input handling - Game state update -
Rendering - Timing control

------------------------------------------------------------------------

### 3️⃣ State Machine

``` c
typedef enum {
    MENU,
    PLAYING,
    GAME_OVER,
    EXIT
} GameState;
```

Clear separation of logic prevents spaghetti code.

------------------------------------------------------------------------

### 4️⃣ Memory Management

-   Manual allocation for snake nodes
-   Proper deallocation on game over
-   No memory leaks
-   Heap usage instead of static arrays

------------------------------------------------------------------------

### 5️⃣ File Persistence

High score is stored in a file: - Loaded at startup - Saved on game
over - Demonstrates binary/text file I/O

------------------------------------------------------------------------

## 🗂 Project Structure
```
snake-game/
│
├── src/
│   ├── main.c
│   ├── game.c
│   ├── snake.c
│   ├── render.c
│   └── input.c
│
├── include/
│   ├── game.h
│   ├── snake.h
│   ├── render.h
│   └── input.h
│
├── data/
│   └── highscore.dat
│
├── build/        (optional, file build output)
│
├── Makefile
├── README.md
└── .gitignore
```
------------------------------------------------------------------------

## ⚙️ Build & Run
### Windows (MinGW)

------------------------------------------------------------------------

## 📚 Concepts Demonstrated

-   Pointers & dynamic memory
-   Linked list operations
-   Real-time console programming
-   Struct memory layout
-   File I/O
-   Separation of concerns
-   Defensive programming

