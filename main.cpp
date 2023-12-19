#include <clocale>
#include <cstdlib>
#include <iostream>
#include <ncurses.h>

#include "game.h"

#define WINDOW_WIDTH 80
#define WINDOW_HEIGHT 24

void _cleanup_ncurses() {
    endwin();

    std::cout << "Thanks for playing!" << std::endl;
}

int main() {
    atexit(_cleanup_ncurses);

    initscr();
    setlocale(LC_ALL, "");

    //Check if the terminal is big enough
    int x, y;
    getmaxyx(stdscr, y, x);
    if (x < WINDOW_WIDTH || y < WINDOW_HEIGHT) {
        std::cerr << "Terminal size should be at least "
                  << WINDOW_WIDTH << "x" << WINDOW_HEIGHT
                  << " but is " << x << "x" << y << std::endl;
        exit(1);
    }

    cbreak();             // Line buffering disabled. pass on everything
    noecho();             // Do not echo input characters
    keypad(stdscr, TRUE); // Enable the keypad for non-char keys
    curs_set(0);          // Hide the cursor

    // Object test
//    WINDOW *win = newwin(10, WINDOW_WIDTH, 0, 0);
//    box(win, 0, 0);
//    refresh();
//    Object *obj = new PigSmall(win, 0, 0);
//    obj->draw();
//    wrefresh(win);

    Game game(WINDOW_WIDTH, WINDOW_HEIGHT);
    game.load_stage(1);

    return 0;
}
