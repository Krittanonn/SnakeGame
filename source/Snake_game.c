#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

#define W 20
#define H 20
#define MAX 100

// === idk some funcs ===
void setup();
void draw();
void input();
void logic();
void go(int x, int y);
void hideCur();

// === lazy globals ===
typedef struct {
    int x, y;
} Pos;

enum Dir { STOP = 0, LEFT, RIGHT, UP, DOWN };
enum Dir dir;

Pos s[MAX]; // snake body
int len = 1;
Pos f; // fruit pos
int pts = 0;
int over = 0;
int speed = 200;

int main() {
    setup();
    hideCur();

    while (!over) {
        draw();
        input();
        logic();
        Sleep(speed); // chill a bit
    }

    go(0, H + 3);
    printf("rip snake. score: %d\n", pts);
    return 0;
}

void setup() {
    dir = RIGHT;
    s[0].x = W / 2;
    s[0].y = H / 2;
    f.x = rand() % W;
    f.y = rand() % H;
}

void draw() {
    go(0, 0);

    for (int i = 0; i < W + 2; i++) printf("#");
    printf("\n");

    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            if (x == 0) printf("#");

            int printed = 0;
            if (x == s[0].x && y == s[0].y) {
                printf("0"); // head
                printed = 1;
            } else if (x == f.x && y == f.y) {
                printf("X"); // fruit
                printed = 1;
            } else {
                for (int i = 1; i < len; i++) {
                    if (x == s[i].x && y == s[i].y) {
                        printf("o"); // body
                        printed = 1;
                        break;
                    }
                }
            }

            if (!printed) printf(" ");
            if (x == W - 1) printf("#");
        }
        printf("\n");
    }

    for (int i = 0; i < W + 2; i++) printf("#");
    printf("\nscore: %d\n", pts);
    printf("\nSpeed: %d\n", speed);
}

void input() {
    if (_kbhit()) {
        char ch = _getch(); // yeah we grab input here
        switch (ch) {
            case 'a': if (dir != RIGHT) dir = LEFT; break;
            case 'd': if (dir != LEFT)  dir = RIGHT; break;
            case 'w': if (dir != DOWN)  dir = UP; break;
            case 's': if (dir != UP)    dir = DOWN; break;
            case 'x': over = 1; break; // bail
        }
    }
}

void logic() {
    // move tail
    Pos prev = s[0];
    Pos tmp;
    for (int i = 1; i < len; i++) {
        tmp = s[i];
        s[i] = prev;
        prev = tmp;
    }

    // move head
    switch (dir) {
        case LEFT:  s[0].x--; break;
        case RIGHT: s[0].x++; break;
        case UP:    s[0].y--; break;
        case DOWN:  s[0].y++; break;
    }

    // wall = bad
    if (s[0].x < 0 || s[0].x >= W || s[0].y < 0 || s[0].y >= H)
        over = 1;

    // oops snake bit itself
    for (int i = 1; i < len; i++) { 
        if (s[0].x == s[i].x && s[0].y == s[i].y)
            over = 1;
    }

    // eat time
    if (s[0].x == f.x && s[0].y == f.y) {
        pts += 10;
        f.x = rand() % W;
        f.y = rand() % H;
        if (len < MAX) len++;
        
        if (speed > 30) speed -= 5;
    }
}

void go(int x, int y) {
    COORD c = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void hideCur() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO i;
    i.dwSize = 100;
    i.bVisible = FALSE;
    SetConsoleCursorInfo(h, &i);
}
